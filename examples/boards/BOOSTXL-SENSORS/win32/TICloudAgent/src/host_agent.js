/*jslint node: true */
"use strict";

var VERSION = "2.5";

var Q = require("q");
var logger = require("./logger.js");
var createModule = require("./module.js").createModule;
var fs = require("fs");
var config = require("./config.js");

module.exports.start = function() {

    logger.info("Starting Agent!");

    return createModule("Agent").then(

        function(agent) {

            // load submodules from default location
            var moduleJSFiles = fs.readdirSync("modules");
            for (var i = 0; i < moduleJSFiles.length; i++) {
                // define submodules
                try {
                    var subModuleDef = require("./modules/" + moduleJSFiles[i]);
                    logger.info("Discovered Module : " + subModuleDef.name);
                    agent.subModules[subModuleDef.name] = subModuleDef;
                } catch (err) {
                    logger.info("Failed to load module : " + moduleJSFiles[i] + " : " + err.stack);
                }
            }


            agent.commands.addConfigProperty = function(name, value) {

                var deferred = Q.defer();

                config[name] = value;
                logger.info("Setting property " + name + " : " + value);

                deferred.resolve();
                return deferred.promise;

            };

            // handle the onclose event
            agent.onclose = function() {
                logger.info("Exiting agent process!");
                process.exit();
            };


            return agent.getPort();
        }

    ).then(function(port) {
        return {
            port: port,
            version: VERSION
        };
    });
};