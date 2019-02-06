/*jslint node: true */
"use strict";

var createModule = require("../module.js").createModule;
var fileStorage = require("../file_storage.js");
var Q = require("../node_modules/q");


/**
 *   Decode and write base64Data to file
 *   Data is a "TEMP" directory
 *   @fileName - Name of the file where the data should be stored
 *   @parentDir - Name of the directory under the "TEMP" where the file should be stored
 *   @base64Data - Data to be decoded and stored
 *
 */
var writeFile = function(fileName, base64Data) {

    var deferred = Q.defer();

    try {
        var destFilePath = fileStorage.write(fileName, base64Data);
        deferred.resolve({
            path: destFilePath
        });
    } catch (e) {
        deferred.reject(e);
    }

    return deferred.promise;
};

module.exports = {

    name: "File",

    create: function() {
        return createModule(this.name).then(

            function(file) {

                file.commands.write = writeFile;

                return {
                    port: file.getPort()
                };
            }


        );
    }

};