/***************************************************************************************************
 * Copyright (c) 2015 Texas Instruments and others All rights reserved. This program and the
 * accompanying materials are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 * 
 * Contributors: Paul Gingrich - Initial API and implementation
 **************************************************************************************************/
var gc = gc || {};
gc.fileCache = gc.fileCache || {};

if (gc.fileCache.readJsonFile === undefined)
{
	(function() // closure for private static methods and data.
	{
		var fileContents = {};
		
	    var loadFile = function(fileURL, parser) 
	    {
	        var promise = fileContents[fileURL];
	        if (!promise)
	        {
	            promise = Q.Promise(function(resolve, reject) 
	            {
	                var xmlHttp = new XMLHttpRequest();
	                xmlHttp.onloadend = function()
	                {
	                    if (xmlHttp.status === 200 || (xmlHttp.status === 0 && xmlHttp.responseText))
	                    {
	                        var data;
	                        try
	                        {
	                            data = parser(xmlHttp.responseText);
	                        }
	                        catch(e)
	                        {
	                        	
	                        }
	                        resolve(data);
	                    }
	                    else
	                    {
	                        reject(new Error("Can't read file: " + fileURL + ".  Status Code = " + xmlHttp.status));
	                    }
	                };
	
	                xmlHttp.open("GET", fileURL, true);
	                xmlHttp.send();
	                
	            });
	            fileContents[fileURL] = promise;
	        }
	        return promise;
	    };
	    
	    gc.fileCache.readJsonFile = function(fileURL)
	    {
	    	return loadFile(fileURL, JSON.parse);
	    };
	    
	    var nullParser = function(data) 
	    {
	    	return data; 
	    };
	    
	    gc.fileCache.readTextFile = function(fileURL) 
	    {
	    	return loadFile(fileURL, nullParser);
	    };
	    
	}());
}
    
