/*jslint node: true */
"use strict";

var prompt = require("prompt");
var deasync = require("deasync");
var doneCurrent = false;
var WebSocketLibImport = require("ws");
var ws = new WebSocketLibImport( "ws://localhost:" + 1337 + "/");

function onErr(err) {
	console.log(err);
	return 1;
}

ws.on("open", function open() {
	console.log("prompt> connect to server on 1337.");

	ws.on("message", function(data) {
		console.log("prompt> reply from server: " + data);
	});

	prompt.start();

	while(true) {
		doneCurrent = false;

		prompt.get(["line"], function (err, result) { 
			if (err) { return onErr(err); }
			console.log(" line: " + result.line);
			ws.send(result.line);
			//ws.close();
			doneCurrent = true;
		});

		while(!doneCurrent) {
			deasync.runLoopOnce();
		}
	}
});

