/*jslint node: true */
"use strict";


var spawn = require("child_process").spawn;
var config = require("../config.js");
var path = require("path");
var logger = require("../logger.js");

var Q = require("../node_modules/q");

var spawnDS = function() {

  logger.info("Starting DS!!!!!");

  var deferred = Q.defer();

  var execFile = "./DSLite";
  var workingDirPath = path.resolve(config.loadersRoot + "/DSLite/DebugServer/bin");
  var params = [];

  logger.info("spawnDS : execFile = " + execFile + " args = " + params + " cwd = " + workingDirPath);
  var lp = spawn(execFile, params, {
    cwd: workingDirPath,
    detached: true
  });

  lp.stdout.on("data", function(data) {

    var dataStr = data.toString();
    logger.info("DS Lite : " + dataStr);

    if (dataStr.indexOf("Error") > -1) {
      logger.info(dataStr);
      deferred.reject({
        message: dataStr
      });
      return;
    }

    try {
      var dataObj = JSON.parse(dataStr);
      if (dataObj.port) {
        logger.info("Started DS Lite : " + dataStr);
        deferred.resolve(dataObj);
      }
    } catch (e) {
      // ignore non json data
    }

  });

  lp.stderr.on("data", function(data) {
    deferred.reject({
      message: data.toString()
    });
  });

  lp.on("close", function() {
    logger.info("DSLite process : close event");
  });

  lp.on("exit", function() {
    logger.info("DSLite process : exit event");
  });

  lp.on("disconnect", function() {
    logger.info("DSLite process : disconnect event");
  });


  return deferred.promise;
};



module.exports = {

  name: "DS",
  create: function() {
    return spawnDS();
  }
};