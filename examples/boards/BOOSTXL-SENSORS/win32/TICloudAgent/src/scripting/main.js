/*jslint node: true */
"use strict";

var WebSocketServer = require("ws").Server;
var Q = require("q");
var deferred = Q.defer();

var scriptAgent = require("./agent.js");
var createEvaluator = require("./context_eval.js");
var spawnDSLite = require("../modules/dslite.js");

var MODULE = function(isRemote) {
	return spawnDSLite.create().then(function(portInfo) {
		return scriptAgent(spawnDSLite.name, portInfo.port, undefined);
	}).then(function(module) {
		return createEvaluator(module, scriptAgent);
	}).then(function(evaluator) {
		if (isRemote) {
			console.log("remote eval of script....");
			var wss = new WebSocketServer({ port: 1337 });
			wss.on("connection", function connection(ws) {
				ws.on("message", function incoming(message) {
					evaluator.runInContext(message);
				});

				deferred.resolve(ws);
			});

			deferred.promise.then(function (clientWs) {
				console.log("client opened ws: " + clientWs);
			}).fail(function (error) {
				console.error("error: %s", error);
			});
		}
//		console.log("return evaluator " + evaluator);
		return evaluator;
	}).fail(console.error);
	//todo: error handling
};

module.exports = MODULE;

















