/*******************************************************************************
 * Copyright (c) 2015 Texas Instruments and others All rights reserved. This
 * program and the accompanying materials are made available under the terms of
 * the Eclipse Public License v1.0 which accompanies this distribution, and is
 * available at http://www.eclipse.org/legal/epl-v10.html
 * 
 * Contributors: Gingrich, Paul - Initial API and implementation
 ******************************************************************************/
var gc = gc || {};
gc.databind = gc.databind || {};

(function() // closure for private static methods and data.
{
    var SerialPortModel = function()
    {
    };

    SerialPortModel.prototype = new gc.databind.AbstractStreamingDataModel('uart');

    var serialIO;
    SerialPortModel.prototype.sendValue = function(value, progress)
    {
        var jsonString = JSON.stringify(value);
        if (serialIO)
        {
            serialIO.serialSend(jsonString + '\n');
        }
        else
        {
            console.log('Discarded tx uart data: ' + jsonString);
        }
    };

    var serialPortInstance = new SerialPortModel();
    var partialJsonMessage = "";
    var callbackForDataValidation;

    gc.databind.SerialPortModel =
        {
            serialPortRecievedHandler :
            {
                text : function(message)
                {
                    var packets = (partialJsonMessage + message).split("\n");
                    var size = packets.length - 1;
                    for (var i = 0; i < size; i++)
                    {
                        var packet = packets[i];
                        try
                        {
                            // remove any leading or trailing garbage characters
                            var cleanPacket = packet.substring(packet.indexOf('{'), packet.lastIndexOf('}') + 1);
                            // remove any spaces between : and the value
                            while (cleanPacket.indexOf(': ') > 0)
                            {
                                cleanPacket =
                                    cleanPacket.substring(0, cleanPacket.indexOf(': ') + 1) + cleanPacket.substring(cleanPacket.indexOf(': ') + 2).trim();
                            }
                            serialPortInstance.receiveData(JSON.parse(cleanPacket));
                            if (callbackForDataValidation)
                            {
                                serialPortInstance.setConnectedState(true);
                                callbackForDataValidation();
                                callbackForDataValidation = undefined;
                            }
                        }
                        catch (e)
                        {
                            console.log('SerialPortModel: received non JSON data string');
                        }
                    }
                    partialJsonMessage = packets[size];
                }
            },
            connect : function(dataValidationCallback)
            {
                partialJsonMessage = "";
                callbackForDataValidation = dataValidationCallback;
                if (!dataValidationCallback)
                {
                    serialPortInstance.setConnectedState(true);
                }
            },
            disconnect : function()
            {
                callbackForDataValidation = undefined;
                partialJsonMessage = "";
                serialPortInstance.setConnectedState(false);
            },
            setSerialIOInstance : function(serialIOInstance)
            {
                serialIO = serialIOInstance;
            }
        };

    // register program model with bindingRegistry
    gc.databind.registry.registerModel(new gc.databind.StorageProviderBindFactoryWrapper(serialPortInstance), true);

}());
