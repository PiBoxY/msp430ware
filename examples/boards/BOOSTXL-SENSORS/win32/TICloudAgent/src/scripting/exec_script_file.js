/*jslint node: true */
"use strict";

var createScriptingEngine = require("./main.js");
var WebSocketLibImport = require("ws");
var fs = require("fs");

var filePath = "./scriptToRun.js";

var isRemote = false;

createScriptingEngine(isRemote).then(function (evaluator) {

	if (isRemote) {
		console.log("opening remote evaluator socket....");
		var ws = new WebSocketLibImport( "ws://localhost:" + 1337 + "/");
		ws.on("open", function open() {
			console.log("prompt> connect to server on 1337.");

			ws.on("message", function(data) {
				console.log("prompt> reply from server: " + data);
			});

			fs.readFile(filePath, {encoding: "utf-8"}, function(err,data){
				if (!err){
					//console.log('received data: ' + data);
					ws.send(data + "\nterminate();");
					ws.close();
				}else{
					console.log(err);
				}

			});

		}) ;
	} else {
		console.log("evaluating locally....");
		fs.readFile(filePath, {encoding: "utf-8"}, function(err,data){
			if (!err){
				evaluator.runInContext(data + "\nterminate();");
			}else{
				console.log(err);
			}

		});
	}
	return evaluator;
});
