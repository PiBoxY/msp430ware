/*jslint node: true */
"use strict";


var path = require("path");
var fs = require("fs");
var os = require("os");

var Q = require("../node_modules/q");
var request = require("../node_modules/request");
var chmodr = require("../node_modules/chmodr");
var Zip = require("../node_modules/node-7z");

var util = require("../../util/util");
var proxyfinder = require("../../util/find_proxy");
var logger = require("../logger");
var userMessages = require("./user_messages");


// remote server prototype object
var createRemoteServer = function(cloudAgentInstallerServerURL) {

	if (!cloudAgentInstallerServerURL)
		throw "cloudAgentInstallerServerURL must be defined!";


	var artificatsDir = path.join(os.tmpdir(), "/ticloudagent_artifacts");
	// clean up existing ARTIFACTS_DIR
	// and create a fresh one
	if (fs.existsSync(artificatsDir)) {
		util.deleteFolderRecursive(artificatsDir);
	}

	fs.mkdirSync(artificatsDir);


	var getProxy = function() {

		var cachedProxy;
		return function() {

			var deferred = Q.defer();

			if (cachedProxy) {
				deferred.resolve(cachedProxy);
			} else {
				proxyfinder.get(function(proxy) {

					logger.info("installer.js using proxy: " + proxy);

					// append http if needed
					if (proxy !== "" && proxy.indexOf("http") < 0) {
						proxy = "http://" + proxy;
					}

					cachedProxy = proxy;
					deferred.resolve(cachedProxy);
				}, cloudAgentInstallerServerURL);
			}

			return deferred.promise;
		};

	}();

	var requestJSONFromServer = function(url) {

		return getProxy().then(function(proxy) {


				var deferred = Q.defer();
				request({
					"url": url,
					rejectUnauthorized: false,
					"proxy": proxy
				}, function(error, response, body) {
					if (!error && response.statusCode == 200) {
						deferred.resolve(JSON.parse(body));
					} else {
						deferred.reject(userMessages.remoteServerError());
					}
				});

				return deferred.promise;

			}

		);
	};

	return {

		// get the install info data
		getInstallInfo: function(ccxmlFile) {

			var path = cloudAgentInstallerServerURL + "/ticloudagentinstaller/getInstallInfo?";
			path += "os=" + util.installerOS;
			path += "&deviceID=" + ccxmlFile.getDeviceID();
			path += "&connectionID=" + ccxmlFile.getConnectionID();

			logger.info("Getting Install Info: " + path);
			return requestJSONFromServer(path);

		},

		getComponentInfo: function(componentName) {
			var path = cloudAgentInstallerServerURL + "/ticloudagentinstaller/getComponentInfo?";
			path += "component=" + componentName;

			logger.info("Getting Component Info: " + path);
			return requestJSONFromServer(path);
		},

		downloadComponent: function() {

			var componentPathAPIUrl = "/ticloudagentinstaller/getComponent?os=" + util.installerOS + "&component=";
			var getComponentPath = function(componentName) {
				return cloudAgentInstallerServerURL + componentPathAPIUrl + componentName;
			};

			var getZipFileName = function(componentName) {
				return path.join(artificatsDir, componentName + ".7z");
			};

			var getUnzippedFolderName = function(componentName) {
				return path.join(artificatsDir, componentName);
			};

			return function(componentInfo) {

				var componentName = componentInfo.name;
				var url = getComponentPath(componentName);

				return getProxy().then(function(proxy) {


					var deferred = Q.defer();

					var zipFileName = getZipFileName(componentName);
					var unzippedFolder = getUnzippedFolderName(componentName);
					var writeStream = fs.createWriteStream(zipFileName);
					logger.info("Get: " + url);

					var responseOK = false;
					request.get({
							"url": url,
							rejectUnauthorized: false,
							"proxy": proxy
						})
						.on("response", function(response) {
							logger.info("Response: " + response.statusCode);
							if (response.statusCode === 200) {
								logger.info("Saving to temporary location: " + zipFileName);
								responseOK = true;
							} else {
								logger.info("Download failed");
							}
						})
						.on("error", function(err) {
							deferred.reject(userMessages.unknownError(err));
						})
						.pipe(writeStream);

					writeStream.on("finish", function() {

						if (responseOK) {
							try {
								var extractor = new Zip();
								extractor.extractFull(zipFileName, unzippedFolder)
									.then(function() {
										chmodr.sync(unzippedFolder, parseInt("755", 8));
										deferred.resolve(unzippedFolder);
									})
									.catch(function(err) {
										deferred.reject(userMessages.unknownError(err));
									});
							} catch (err) {
								deferred.reject(userMessages.unknownError(err));
							}
						} else
							deferred.reject(userMessages.unknownError("Download Failed"));
					});

					return deferred.promise;

				});

			};

		}()

	};
};


module.exports = createRemoteServer;