/*jslint node: true */
"use strict";

var path = require("path");
var format = require("util").format;
var util = require("../../util/util");

var userMessages = function() {


	var beginInstallationMsg = "Additional packages need to be installed to support %s. Starting Install Process.\n";
	var endInstallationMsg = "Installation completed successfully.\n";
	var downloadingComponentMsg = "Downloading package : %s\n";
	var installingComponentMsg = "Installing package : %s\n";
	var installingWinDriverMsg = "Installing package ( Windows driver please accept the UAC dialog ): %s.\n";
	var componentInstalledMsg = "Installed package : %s\n";
	var componentInstallErrorMsg = "An error occurred while installing package: %s\n. Error : %s\n";
	var targetIsNotSupportedMsg = beginInstallationMsg + "Could not find needed packages. %s may not be supported on this OS.\n";

	var proxyFilePath = path.resolve(path.join(__dirname, "..", "..", "util", "proxy.js"));
	var errorRemoteServerMsg = "An error occurred while retrieving information from the remote server. The most likely cause of this is incorrect proxy settings.\n";
	if (util.isLinux) {
		errorRemoteServerMsg += "Please ensure that the http_proxy env variable is set correctly or\n";
	}
	errorRemoteServerMsg += "To manually override the proxy settings see : " + proxyFilePath + "\n";
	errorRemoteServerMsg += "The Cloud IDE page needs to be refreshed for the changes to take effect\n";

	var unknownErrorMsg = "An unknown error occurred : %s.\n";
	var waitingForPackageMsg = "Installation still in progress for package: %s.\n";
	var installationFailedMsg = "Installation failed : %s.\n";

	return {
		beginInstallation: function(target) {
			return format(beginInstallationMsg, target);
		},

		endInstallation: function() {
			return format(endInstallationMsg);
		},

		downloadingComponent: function(component) {
			return format(downloadingComponentMsg, component);
		},

		installingComponent: function(component) {
			return format(installingComponentMsg, component);
		},

		installingWinDriver: function(component) {
			return format(installingWinDriverMsg, component);
		},

		installedComponent: function(component) {
			return format(componentInstalledMsg, component);
		},

		componentInstallError: function(component, error) {
			return format(componentInstallErrorMsg, component, error);
		},

		targetIsNotSupportedError: function(target) {
			return format(targetIsNotSupportedMsg, target, target);
		},

		remoteServerError: function() {
			return format(errorRemoteServerMsg);
		},

		unknownError: function(err) {
			return format(unknownErrorMsg, err);
		},

		waitingForPackage: function(component) {
			return format(waitingForPackageMsg, component);
		},

		installationFailed: function(err) {
			return format(installationFailedMsg, err);
		}

	};
}();

module.exports = userMessages;