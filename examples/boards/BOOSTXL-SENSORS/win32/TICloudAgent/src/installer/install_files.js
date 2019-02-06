/*jslint node: true */
"use strict";

// handle installing zip packages that are just simple files and require no additonal processing

var fs = require("fs");
var path = require("path");
var ncp = require("../node_modules/ncp");
var Q = require("../node_modules/q");

var moveFiles = function(from, to) {

	var deferred = Q.defer();

	var filesToMove = fs.readdirSync(from);

	if (filesToMove.length !== 1) {
		deferred.reject("Expected only one root folder");
	} else {
		var file = filesToMove[0];
		var toFile = path.join(to, file);
		var fromFile = path.join(from, file);

		ncp(fromFile, toFile, {
			clobber: false
		}, function(err) {
			if (err) {
				deferred.reject(err);
			} else {
				deferred.resolve();
			}
		});

	}

	return deferred.promise;
};

var installFiles = function(remoteServer, userDataRoot) {

	return function(componentInfo) {

		var componentInstallPath = componentInfo.path;

		return remoteServer.downloadComponent(componentInfo).then(function(tmpPath) {
			var fromPath = tmpPath;
			var toPath = path.join(userDataRoot, componentInstallPath);
			return moveFiles(fromPath, toPath);
		}).then(function() {
			// install interface expects the name when install is successful
			return componentInfo.name;
		});

	};
};


module.exports = function(userDataRoot, remoteServer) {

	var nop = function() {}; // do nothing function

	return {
		install: installFiles(remoteServer, userDataRoot),
		quiet: nop,
		offline: nop
	};

};