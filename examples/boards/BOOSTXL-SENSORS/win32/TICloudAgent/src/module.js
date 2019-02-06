/*jslint node: true */
"use strict";

var sockets = require("./sockets.js");
var logger = require("./logger.js");

var Q = require("q");

var createModule = function(name) {

	var port = null;
	var ws = null;


	var log = function(msg) {
		logger.info("Module( " + name + " )->" + msg);
	};

	var createResponseHandler = function(type, msgData, ws) {
		return function(data) {

			var obj = {
				data: data
			};

			obj[type] = msgData.id;
			log("Ret Command ( " + type + ") : " + msgData.command + " : " + msgData.id);
			ws.send(JSON.stringify(obj));
		};
	};

	var moduleObj = {

		commands: { // basic commands avialable for each submodule

			listSubModules: function() {

				var deferred = Q.defer();
				var subModules = this.subModules;
				var subModuleNames = [];
				for (var subModule in subModules) {
					if (subModules.hasOwnProperty(subModule)) {
						subModuleNames.push(subModule);
					}
				}

				var retObj = {
					subModules: subModuleNames
				};

				deferred.resolve(retObj);

				return deferred.promise;
			},

			createSubModule: function(subModuleName) {
				var subModuleDefinition = this.subModules[subModuleName];
				log("Creating Sub Module : " + subModuleName);
				return subModuleDefinition.create();
			},

			listCommands: function() {

				var deferred = Q.defer();
				var commands = this.commands;

				var commandNames = [];
				for (var command in commands) {
					if (commands.hasOwnProperty(command)) { // don't return the listCommands command
						commandNames.push(command);
					}
				}

				var retObj = {
					commands: commandNames
				};

				deferred.resolve(retObj);

				// default implementation
				return deferred.promise;
			}

		},

		subModules: {},

		triggerEvent: function(eventName, eventData) {

			var obj = {
				event: eventName,
				data: eventData
			};

			ws.send(JSON.stringify(obj));

		},

		getPort: function() {
			return port;
		}

	};

	// init sets up the web socket communication and sets up support for RPC calls 
	// init must be resolved before any other function 
	var setUp = function() {


		return sockets.createWSServer().then(

			function(wss) {

				log("Web socket server started!");

				// handle opening a websocket connection
				wss.on("connection", function(newWs) {

					// We support one host side module to one remote module mapping, thus there
					// should only be one connection open
					if (wss.clients.length > 1) {
						log("Too many clients. Exiting");
						process.exit();
					}

					ws = newWs;

					ws.on("message", function(message) {

						var msgData = JSON.parse(message);
						var command = moduleObj.commands[msgData.command];
						var errorHandler = createResponseHandler("error", msgData, ws);

						if (command) {
							log("Exec Command : " + msgData.command + ":" + msgData.id);
							var responseHandler = createResponseHandler("response", msgData, ws);

							try {
								command.apply(moduleObj, msgData.data).done(responseHandler, errorHandler);
							} catch (err) {
								errorHandler({
									message: err.stack
								});
							}

						} else {
							var msg = "Command not defined : " + command;
							log(msg);
							errorHandler({
								message: msg
							});
						}

					});

					ws.on("close", function() {
						if (moduleObj.onclose) {
							moduleObj.onclose();
						}
						log("Socket closed. Exiting");
					});

				});

				wss.on("close", function() {
					if (moduleObj.onclose) {
						moduleObj.onclose();
					}
				});


				// set the port
				port = wss._server.address().port;

				return moduleObj;

			}

		);

	};

	return setUp();

};

module.exports.createModule = createModule;