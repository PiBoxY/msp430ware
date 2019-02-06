/*jslint node: true */
"use strict";

var createModule = require("../module").createModule;
var createInstaller = require("../installer/installer");

var createEventData = function(eventID, progressData) {
    // change the progress into events to be consistent with ds
    var data = {
        name: "Installing Target Support ( First Time Only )",
        subActivity: progressData.message,
        id: eventID,
        isComplete: progressData.isComplete
    };

    return data;
};

module.exports = {

    name: "TargetSupport",

    create: function() {

        // get cloudAgentInstallerServerURL from the config file set by the agent.setProperty call 
        /// by the client
        var config = require("../config.js");
        var installer = createInstaller(config.cloudAgentInstallerServerURL);

        var eventID = 0;

        return createModule(this.name).then(

            function(targetSupport) {

                targetSupport.commands.add = function(ccxmlFilePath) {

                    eventID++;

                    return installer.addSupport(ccxmlFilePath).progress(function(progressData) {
                        targetSupport.triggerEvent("progress", createEventData(eventID, progressData));
                    });
                };

                targetSupport.commands.purge = function() {
                    return installer.purge();
                };

                return {
                    port: targetSupport.getPort()
                };

            }

        );

    }

};