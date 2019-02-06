/*jslint node: true */
"use strict";

var path = require("path");
var logger = require("../logger.js");
var util = require("../../util/util.js");
var config = require("../config.js");
var createModule = require("../module.js").createModule;
var Q = require("../node_modules/q");


var userMessages = function() {
    return {
        loadFailed: function() {
            return "\nPlease power cycle the device and try again\n";
        }
    };
}();

var platform = function() {

    var ext = util.isWin ? ".exe" : "";

    return {
        ext: ext,
        loadersDirPath: config.loadersRoot,
        isWin: util.isWin,
        isLinux: util.isLinux
    };

}();

var lm4FlashLoaderSpec = {
    execFile: "./lm4flash" + platform.ext,
    workingDirPath: path.resolve(platform.loadersDirPath + "/lm4flash"),
    getLoadProgramArgs: function(filePath) {
        return ["-S", 0, filePath];
    },
    success: function(err, data) {
        if (err) {
            return false;
        } else if (data.toString().length === 0) {
            return false;
        } else {
            return true;
        }
    }
};


var mspdebugSpec = function() {

    var checkError = function(err) {
        if (err) {
            return false;
        } else {
            return true;
        }
    };

    return {
        execFile: "./mspdebug" + platform.ext,
        workingDirPath: path.resolve(platform.loadersDirPath + "/mspdebug"),
        getLoadProgramArgs: function(filePath) {
            return ["rf2500", "--force-reset", "erase", "load " + filePath];
        },
        success: checkError
    };
}();



var makeLoader = function(spec) {

    var exec = require("child_process").execFile;
    if (platform.isLinux) {
        // set the ld library path to include current directory.. needed for msp430
        process.env.LD_LIBRARY_PATH = ".";
    }

    var loadProgramExec = function(filePath) {

        var deferred = Q.defer();

        deferred.notify("Flashing Device...\n");
        logger.info("Programming File -> " + filePath);
        logger.info("Util -> " + spec.execFile);
        logger.info("Args -> " + spec.getLoadProgramArgs(filePath));
        logger.info("Working Dir ->" + spec.workingDirPath);

        exec(spec.execFile, spec.getLoadProgramArgs(filePath), {
            cwd: spec.workingDirPath
        }, function(err, stdout, stderr) {
            var message = "err : " + (err ? err.toString() : "none") + " stdout: " + stdout + " stderr : " + stderr;
            logger.info(message);
            if (spec.success(err, stdout)) {
                deferred.notify("Flash Successful!\n");
                deferred.resolve();
            } else {
                var errormsg = "Flash Failed : " + stderr + "\n";

                if (spec.diagnosticInfo) {
                    errormsg += spec.diagnosticInfo(err, stdout);
                }

                errormsg += userMessages.loadFailed();
                deferred.notify(errormsg);
                deferred.reject(errormsg);
            }
        });

        return deferred.promise;
    };

    return {
        loadProgram: function(filePath) {
            return loadProgramExec(filePath);
        }
    };
};



var lm4FlashLoader = null;
var mspdebugLoader = null;

var getLoader = function(loader) {

    if (loader === "lm4flash") {
        if (lm4FlashLoader === null) {
            lm4FlashLoader = makeLoader(lm4FlashLoaderSpec);
        }
        return lm4FlashLoader;
    } else if (loader === "mspdebug") {
        if (mspdebugLoader === null) {
            mspdebugLoader = makeLoader(mspdebugSpec);
        }
        return mspdebugLoader;
    }

    throw "Unsupported Loader: " + loader;

};

var loadProgram = function(loaderType, filePath) {
    var loader = getLoader(loaderType);
    return loader.loadProgram(filePath);
};


module.exports = {

    name: "Flash",

    create: function() {

        return createModule(this.name).then(
            function(flash) {

                flash.commands.loadProgram = function(loaderType, filePath) {
                    logger.info("loadProgram -> loaderType: " + loaderType + " filePath : " + filePath);
                    return loadProgram(loaderType, filePath);
                };

                return {
                    port: flash.getPort()
                };
            }
        );

    }

};