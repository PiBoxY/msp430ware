/*jslint node: true */
"use strict";

var deasync = require("deasync");
var util = require("util");


var MODULE = function(module, scriptAgent) {

	function augmentPromiseFunction(func) {

		var oldFunction = func;
		//convert to non Promise based
		var newfunc = function(inputObj) {
			var done;
			var ret;

			oldFunction(inputObj).then(function(result) {
				ret = result;
			}).fail(function(err) {
				console.error("error: " + err.message);
			}).fin(function() {
				done = true;
			});

			while (!done) {
				deasync.runLoopOnce();
			}

			return ret;
		};

		return newfunc;
	}

	function augmentFunctions(module) {
		var done;

		module.listCommands().then(function(result) {
			for (var i = 0; i < result.commands.length; i++) {
				var name = result.commands[i];
				var tokens = name.split(".");

				var container = module;
				while (1 != tokens.length) {
					container = module[tokens[0]];
					tokens.shift();
				}

				container[tokens[0]] = augmentPromiseFunction(container[tokens[0]]);
			}
		}).fail(function(err) {
			console.error("error: " + util.inspect(err, false, null));
		}).fin(function() {
			done = true;
		}).done();

		while (!done) {
			deasync.runLoopOnce();
		}

		return module;
	}


	function createInstance() {

		return {
			console: console,

			DSLite: augmentFunctions(module),

			createSubmodule: function(name, subModuleInfo) {
				var retCore;
				var doneCurrent;

				scriptAgent(name, subModuleInfo.port, subModuleInfo.subProtocol)
					.then(function(moduleObj) {
						//console.log("submodule created: " + util.inspect(moduleObj, false, null));
						retCore = moduleObj;
						doneCurrent = true;
					});

				while (!doneCurrent) {
					deasync.runLoopOnce();
				}

				return augmentFunctions(retCore);
			},
			
			inspect: function(obj) {
				console.log(util.inspect(obj, false, null));
			}
		};
	}

	var inst;
		return {
		instance: function() {
			if (!inst) {
				inst = createInstance();
			}
			//console.log("return basic context " + inst);
			return inst;
		}
	};
};

module.exports = MODULE;