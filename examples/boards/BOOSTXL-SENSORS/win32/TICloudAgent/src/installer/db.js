/*jslint node: true */
"use strict";

var path = require("path");
var fs = require("fs");

var util = require("../../util/util");
var logger = require("../logger.js");

var createDB = function(userDataRoot, loadersRoot) {

	var userDir = userDataRoot;
	var dbRoot = path.join(userDir, "/db");
	var installComponentsJSONFile = path.join(dbRoot, "/installed.json");
	var supportedTargetsJSONFile = path.join(dbRoot, "/targets.json");

	if (!fs.existsSync(dbRoot)) {
		logger.info("Creating db dir : " + dbRoot);
		fs.mkdirSync(dbRoot);
	}

	var loadersDir = loadersRoot;
	if (!fs.existsSync(loadersDir)) {
		logger.info("Creating loaders dir : " + loadersDir);
		fs.mkdirSync(loadersDir);
	}


	var installed = {};
	try {
		installed = JSON.parse(fs.readFileSync(installComponentsJSONFile, "utf8"));
	} catch (err) {
		logger.info("Could not open installed.json : " + err);
	}

	var addInstalledComponent = function(component) {
		installed[component] = "0.0.0"; // installed component and version, everything is 0.0.0 to start with
		try {
			fs.writeFileSync(installComponentsJSONFile, JSON.stringify(installed));
			logger.info("Added to Installed Components DB:" + component);
		} catch (err) {
			logger.info("Could not add to Installed Components DB:" + component);
		}

	};

	var supported = {};
	try {
		supported = JSON.parse(fs.readFileSync(supportedTargetsJSONFile, "utf8"));
	} catch (err) {
		logger.info("Could not open targets.json : " + err);
	}

	var addSupportedTarget = function(target, loader) {
		supported[target] = loader; //
		try {
			fs.writeFileSync(supportedTargetsJSONFile, JSON.stringify(supported));
			logger.info("Added to Supported Target DB:" + target);
		} catch (err) {
			logger.info("Could not add to Supported Target DB:" + target);
		}
	};

	var purge = function() {


		util.deleteFolderRecursive(dbRoot);
		util.deleteFolderRecursive(loadersDir);

		// recreate the folders
		fs.mkdirSync(dbRoot);
		fs.mkdirSync(loadersDir);

		supported = {};
		installed = {};

	};

	var isComponentInstalled = function(component) {
		return installed[component] !== undefined;
	};

	var isTargetSupported = function(target) {
		return supported[target] !== undefined;
	};

	var getTargetLoaderType = function(target) {
		return supported[target];
	};

	return {

		addInstalledComponent: addInstalledComponent,
		addSupportedTarget: addSupportedTarget,
		isComponentInstalled: isComponentInstalled,
		isTargetSupported: isTargetSupported,
		getTargetLoaderType: getTargetLoaderType,
		purge: purge

	};


};

module.exports = function(userDataRoot, loadersRoot) {
	var db = createDB(userDataRoot, loadersRoot);
	return db;
};