/*jslint node: true */
"use strict";


var Q = require("../node_modules/q");

var logger = require("../logger");
var userMessages = require("./user_messages");
var readCCXMLFile = require("./ccxml");

var createInstaller = function(userDataRoot, remoteServer, db, options) {

	if (!options)
		options = {};

	var installHandlers = {
		files: require("./install_files")(userDataRoot, remoteServer),
		windriver: require("./install_dpinst")(userDataRoot, remoteServer),
		osxpkg: require("./install_osx_pkg")(userDataRoot, remoteServer)
	};

	for (var key in installHandlers) {

		var handler = installHandlers[key];
		if (options.quiet)
			handler.quiet();

		if (options.offline)
			handler.offline();
	}

	var installComponent = function(componentInfo) {

		var installHandler = installHandlers[componentInfo.type];
		return installHandler.install(componentInfo);

	};

	var createInstallFuncs = function(db, componentsToInstall, deferred) {
		var installFuncs = [];

		var makeFunc = function(component) {
			return function() {

				var componentInfo = componentsToInstall[component];

				if (componentInfo.type === "windriver") {
					deferred.notify({
						message: userMessages.installingWinDriver(component)
					});
				} else {
					deferred.notify({
						message: userMessages.installingComponent(component)
					});
				}

				logger.info(userMessages.installingComponent(component));
				return installComponent(componentInfo);

			};
		};

		for (var component in componentsToInstall) {

			if (!db.isComponentInstalled(component)) {
				installFuncs.push(makeFunc(component));
			}

		}

		return installFuncs;
	};

	var runInstallFuncs = function(installFuncs, deferred) {
		// sequentially call all install funcs
		var newPromise = Q; // re-assign Q to avoid JSHint warning
		var result = newPromise(true);
		installFuncs.forEach(function(f) {
			result = result.then(f).then(function(componentName) {
				// add each installed component to the db
				db.addInstalledComponent(componentName);
				deferred.notify({
					message: userMessages.installedComponent(componentName)
				});
				logger.info(userMessages.installedComponent(componentName));
			});
		});

		return result;
	};

	return {

		// Full target support based on a ccxml file
		addSupport: function(ccxmlFilePath) {

			// keep an outer promise so we can pump out notify messages
			var deferred = Q.defer();

			var ccxmlFile = readCCXMLFile(ccxmlFilePath);

			var targetKey = ccxmlFile.getDeviceID() + "_" + ccxmlFile.getConnectionID();

			// target support already exists
			if (db.isTargetSupported(targetKey)) {
				logger.info("Target support already exists : " + targetKey);
				deferred.resolve(db.getTargetLoaderType(targetKey));
				return deferred.promise;
			}

			var loader;

			var fail = function(err) {
				// at least one component failed;
				deferred.notify({
					message: userMessages.installationFailed(err),
					isComplete: true
				});
				logger.info(err);

				if (err && err.stack) {
					logger.info(err.stack);
				}

				deferred.reject({
					message: userMessages.installationFailed(err)
				});
			};

			var success = function() {
				// all components installed successfully
				db.addSupportedTarget(targetKey, loader);
				deferred.notify({
					message: userMessages.endInstallation(ccxmlFilePath),
					isComplete: true
				});
				deferred.resolve(db.getTargetLoaderType(targetKey));
			};

			// lets check with the server the target is supported
			remoteServer.getInstallInfo(ccxmlFile).then(function(installInfo) {

				if (!installInfo.isTargetSupported) {
					// target is not supported
					deferred.notify({
						message: userMessages.targetIsNotSupportedError(ccxmlFilePath)
					});
					throw "Target is not supported"; // fail out of the promise chain
				}

				// target is supported so we can begin the installation process
				deferred.notify({
					message: userMessages.beginInstallation(ccxmlFilePath)
				});
				logger.info(userMessages.beginInstallation(ccxmlFilePath));

				return installInfo;

			}).then(
				function(installInfo) {

					var componentsToInstall = installInfo.components;
					loader = installInfo.loaderType;
					logger.info("Installing Components: [" + JSON.stringify(componentsToInstall) + "]");

					// create a func for each component to install
					var installFuncs = createInstallFuncs(db, componentsToInstall, deferred);
					return runInstallFuncs(installFuncs, deferred);

				}
			).then(
				success
			).fail(
				fail
			).catch(
				fail
			);


			return deferred.promise;

		},

		// add a single component
		addComponent: function(componentName) {
			return remoteServer.getComponentInfo(componentName).then(function(componentInfo) {
				if (!db.isComponentInstalled(componentName)) {
					return installComponent(componentInfo).then(function() {
						db.addInstalledComponent(componentName);
					});
				}
			});
		},

		purge: function() {

			var deferred = Q.defer();
			deferred.resolve();

			db.purge();

			return deferred.promise;
		}

	};

};


// create an installer obj
module.exports = function(cloudAgentInstallerServerURL, options) {

	var config = require("../config.js");

	var remoteServer = require("./remote_server")(cloudAgentInstallerServerURL);
	var db = require("./db")(config.userDataRoot, config.loadersRoot);
	var installer = createInstaller(config.userDataRoot, remoteServer, db, options);

	return installer;
};