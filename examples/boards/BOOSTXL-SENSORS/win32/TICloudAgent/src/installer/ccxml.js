/*jslint node: true */
"use strict";

var fs = require("fs");

var getMatch = function() {

	var typeToRegex = {
		devices: /<instance.*href="devices\/(.*?)\.xml" .*xmlpath="devices".*>/i,
		connections: /<instance.*href="connections\/(.*?)\.xml" .*xmlpath="connections".*>/i
	};

	return function(ccxmlContents, type) {
		var regex = typeToRegex[type];
		var match = ccxmlContents.match(regex);

		if (!match) {
			return null;
		}

		return match[1];
	};

}();

var readCCXMLFile = function(filePath) {

	var ccxmlContents = fs.readFileSync(filePath).toString();

	var deviceID = getMatch(ccxmlContents, "devices");
	var connectionID = getMatch(ccxmlContents, "connections");

	return {
		getDeviceID: function() {
			return deviceID;
		},
		getConnectionID: function() {
			return connectionID;
		}
	};
};


module.exports = readCCXMLFile;