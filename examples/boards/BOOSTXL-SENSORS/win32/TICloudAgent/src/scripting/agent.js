/*jslint node: true */
"use strict";

var Q = require("q");
var WebSocketLibImport = require("ws");
var logger = require("../logger.js");
var deasync = require("deasync");
var AgentHostApp = {
		Socket : WebSocketLibImport
};

//create the client side module
var createClientModule = function( name, port, subProtocol) {
	var doneCurCmd = false;
	// command dispatch module
	var commandDispatch = function() {

		var commandID = 1; // start from 1 ( 0 could be mistaken for false, in certain places)
		var pendingCommands = {};

		return {

			exec: function( ws, commandName, data ) {
				var defCommand = Q.defer();
				var obj = {
						command: commandName,
						id: commandID++,
						data: data
				};

				pendingCommands[ obj.id ] = defCommand;
				ws.send( JSON.stringify( obj ) , function (err) {
					if (err) {
						console.log("ws.send callback err = " + err);
					}
				});
				
				return defCommand.promise;
			},

			ret: function( retObj ) {

				var response = retObj.response;
				var error = retObj.error;
				var data = retObj.data;

				// it's should only be one of these
				var id = response || error;
				var defCommand = pendingCommands[ id ];

				if( defCommand ) {

					if ( response ) {
						defCommand.resolve( data );
					} else {
						defCommand.reject( data );
					}

					pendingCommands[ id ] = null;

				} else {
					console.log( "commandDispatch : ret , Error, no promise found corresponding to id : " + id );
				}
				
				doneCurCmd = true;
			}

		};
	}();

	var socketUrl = function() {
		return "ws://127.0.0.1:" + port;
	};

	var moduleObj = {};

	moduleObj.getSubModule = function() {

		var subModulePromises = {};

		return function( subModuleName ) {

			var subModulePromise = subModulePromises[ subModuleName ];

			if ( !subModulePromise ) {

				subModulePromise = moduleObj.createSubModule( subModuleName ).then(
						function( data ) {
							return createClientModule( subModuleName, data.port, data.subProtocol );
						}
				);

				subModulePromises[ subModuleName ] = subModulePromise;
			}

			return subModulePromise;
		};
	}();

	function createCommand( ws, fullCommandName ) {

		// add namespace
		var commandNameParts = fullCommandName.split( "." );
		// everything up to the last part is part of the namespace
		var parentObj = moduleObj;
		// keep track of nested namespaces
		var parentNamespace = "";

		for ( var i = 0; i < commandNameParts.length - 1; i++ ) {

			var currentNamespacePart = commandNameParts[ i ];
			parentNamespace += commandNameParts[ i ];

			var newObj = parentObj[ currentNamespacePart ];

			if ( !newObj ) {
				// lets create it
				newObj = {};
			}

			// it becomes the new parent
			parentObj[ currentNamespacePart ] = newObj;
			parentObj = newObj;

			parentNamespace = parentNamespace + ".";
		}

		var commandName = commandNameParts[ commandNameParts.length - 1 ];

		parentObj[ commandName ] = function() {
			var data = Array.prototype.slice.call( arguments );
			logger.info( "calling : " + commandName + " " + doneCurCmd);
			doneCurCmd = false;
			var ret = commandDispatch.exec( ws, fullCommandName, data );

			while(!doneCurCmd) {
				deasync.sleep(100);
			}

			logger.info( "done calling : " + commandName );
			return ret;
		};
	}

	var setUpWS = function() {
		var setUpDef = Q.defer();
		var ws = new AgentHostApp.Socket( socketUrl(), subProtocol );
		moduleObj.websocket = ws;
		
		ws.onclose = function() {
			if ( moduleObj.onclose ) {
				moduleObj.onclose();
			}
		};

		ws.onerror = function() {
			if ( moduleObj.onerror ) {
				moduleObj.onerror();
			}
		};

		var doneConnect;
		(function syncOpenSocket() {
			ws.on("open", function ack(err) {
				logger.info(err);
				doneConnect = true;
			});	
			while(!doneConnect) {
				require("deasync").runLoopOnce();
			}
		}());

		ws.onmessage = function( msgEvt ) {
			logger.info( "ws.onmessage: " + msgEvt.data);
			
			var retObj = JSON.parse( msgEvt.data );
			if ( !retObj.event ) {
				commandDispatch.ret( retObj );	
			} else if ("cio" === retObj.event) {
				console.log(retObj.data.message);
			}

		};

		createCommand( ws, "listCommands" ); // everymodule should have a listCommands command

		moduleObj.listCommands().then( function( dataObj ) {
			var commands = dataObj.commands;
			// create commands
			for ( var i = 0; i < commands.length; i++ ) {
				createCommand( ws, commands[ i ] );
			}

			setUpDef.resolve( moduleObj );

		}, function() {
			setUpDef.reject( moduleObj.name + " : listCommands is not defined" );
		} );

		return setUpDef.promise;
	};
	
	return setUpWS();
};

module.exports = createClientModule;

