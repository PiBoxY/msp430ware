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
    var StreamingDataBind = function(parent, fieldName)
    {
        gc.databind.VariableBindStreamingValue.call(this);

        this.setStale(true);

        this.parentBind = parent;
        this.fieldName = fieldName;

        parent.addStreamingListener(this);
    };

    StreamingDataBind.prototype = new gc.databind.VariableBindStreamingValue();

    StreamingDataBind.prototype.onDataReceived = function(newValue)
    {
        try
        {
            var value = newValue[this.fieldName];
            if (value !== undefined)
            {
                this.setData(value);
                this.setStale(false);
            }
        }
        catch (e)
        {
        }
    };

    StreamingDataBind.prototype.onValueChanged = function(oldValue, newValue, progress)
    {
        this.sendValue(newValue, progress);
    };

    StreamingDataBind.prototype.sendValue = function(value, progress)
    {
        var data = {};
        data[this.fieldName] = value;
        this.parentBind.sendValue(data, progress);
    };

    gc.databind.AbstractStreamingDataModel = function(name)
    {
        gc.databind.AbstractBindFactory.call(this, name);

        this._streamingDataBind = new gc.databind.VariableBindStreamingValue({});
        this._streamingDataBind.setIndex();
        var that = this;
        this._streamingDataBind.sendValue = function(value)
        {
            that.sendValue(value);
        };

        this._targetConnectedBind = new gc.databind.VariableBindValue(false, true);
    };

    gc.databind.AbstractStreamingDataModel.prototype = new gc.databind.AbstractBindFactory();

    gc.databind.AbstractStreamingDataModel.prototype.createNewBind = function(name)
    {
        if (name.indexOf('$') === 0)
        {
            if (name === '$rawData')
            {
                return this._streamingDataBind;
            }
            else if (name === "$target_connected")
            {
                return this._targetConnectedBind;
            }
        }
        var pos = name.lastIndexOf('.');
        var parentBind;
        if (pos > 0)
        {
            parentBind = this.getBinding(name.substring(0, pos));
            name = name.substring(pos + 1);
        }
        else
        {
            parentBind = this._streamingDataBind;
        }
        return new StreamingDataBind(parentBind, name);
    };

    gc.databind.AbstractStreamingDataModel.prototype.receiveData = function(nextValue)
    {
        this._streamingDataBind.setData(nextValue);
    };

    gc.databind.AbstractStreamingDataModel.prototype.setConnectedState = function(newState)
    {
        this._targetConnectedBind.updateValue(newState);
    };
}());
