var env = ScriptingEnvironment.instance();

env.traceWrite("start");

var ccxml = "C:/Users/a0389354/a0389354_view12/install/ccs_base/DebugServer/bin/SystemSetup.ccxml";
var program = "C:/Users/a0389354/workspace_v6_1/c64xle_sim/Debug/c64xle_sim.out";

var server = env.getServer("DebugServer.1");
server.setConfig(ccxml);
var session = server.openSession();
env.traceWrite("before connect");
session.target.connect();

session.expression.evaluate('DEBUG_SetStringProperty("AutoRunToLabelName","")');
env.traceWrite("before load");
session.memory.loadProgram(program);

var isHalted;
while (!isHalted) {
	var result = session.expression.evaluate("GEL_IsHalted()");
//	if (result && result.value) {
	isHalted =  result.value > 0;
//	}
}
env.traceWrite("before disconnect");
session.target.disconnect();

session.terminate();
server.stop();

env.traceWrite("end");