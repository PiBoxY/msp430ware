/*jslint node: true */
"use strict";

var vm = require("vm");
var deasync = require("deasync");
var scriptingEnvironment = require("./api/scripting_env.js");
var basicContext = require("./basic_context.js");

var MODULE = function (module, scriptAgent) {
	//augment agent here in the future to add more functionalities

	var evaluator = {};
	evaluator.module = module;
		
	function createContextObj() {
		var basicContextInst = basicContext(module, scriptAgent).instance();
		
		return {
			ScriptingEnvironment: scriptingEnvironment(basicContextInst),

			terminate: function() {
				console.log("closing websocket, and terminate process.");
				module.websocket.close();
			}
		};
	}

	evaluator.createNewContext = function () {
		evaluator.mycontext = vm.createContext(createContextObj());
	};

	evaluator.runInContext = function(line) {
		if (!evaluator.mycontext) {
			evaluator.createNewContext();
		}

		vm.runInContext(line, evaluator.mycontext);
	};

	return evaluator;
};

module.exports = MODULE;