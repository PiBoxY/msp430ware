/*jslint node: true */
"use strict";

var fs = require("fs");

module.exports.isWin = /^win/.test(process.platform);
module.exports.isLinux = /^linux/.test(process.platform);
module.exports.isOSX = /^darwin/.test(process.platform);

// delete non empty folders. Otherwise; node throws NONEMPTY error as per posix standard
var deleteFolderRecursive = function(path) {
	var files = [];
	if (fs.existsSync(path)) {
		files = fs.readdirSync(path);
		files.forEach(function(file) {
			var curPath = path + "/" + file;
			if (fs.lstatSync(curPath).isDirectory()) { // recurse
				deleteFolderRecursive(curPath);
			} else { // delete file
				fs.unlinkSync(curPath);
			}
		});
		fs.rmdirSync(path);
	}
};


module.exports.deleteFolderRecursive = deleteFolderRecursive;

var OS = "win";
if (module.exports.isLinux) {
	OS = "linux";
} else if (module.exports.isOSX) {
	OS = "osx";
}

module.exports.installerOS = OS;