/*****************************************************************
 * Copyright (c) 2015 Texas Instruments and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *  Paul Gingrich - Initial API and implementation
 *****************************************************************/
var gc = gc || {};
gc.databind = gc.databind || {};

/** 
 * Class that implements IBindStreamingValue interface for a variable value binding.
 *
 * @constructor
 * @extends gc.databind.VariableLookupBindValue
 * 
 * @param {*} initialValue - the constant value returned by this binding. 
 * @param {boolean} [readOnly=false] - flag indicating if this binding is a constant (not writable by setValue()). 
 */
gc.databind.VariableBindStreamingValue = function(initialValue, readOnly) 
{
	gc.databind.VariableLookupBindValue.call(this, initialValue, readOnly);
};

gc.databind.VariableBindStreamingValue.prototype = new gc.databind.VariableLookupBindValue();

/**
 * Add a streaming listener for this bindable object. 
 *  
 * @param {gc.databind.IStreamingListener} listener - callback that will be notified for each new value received.  
 */
gc.databind.VariableBindStreamingValue.prototype.addStreamingListener = function(listener)
{
	this.fEvents.addListener('DataReceived', listener);
};

/**
 * Remove a streaming listener for this bindable object. 
 *  
 * @param {gc.databind.IStreamingListener} listener - callback that will be notified for each new value received.  
 */
gc.databind.VariableBindStreamingValue.prototype.removeStreamingListener = function(listener)
{
	this.fEvents.removeListener('DataReceived', listener);
};	

/**
 * Notifies client listener objects when new data is received.  
 * 
 * @param {object} nextValue - value just received
 */
gc.databind.VariableBindStreamingValue.prototype.notifyDataReceivedListeners = function(nextValue) 
{
	this.fEvents.fireEvent('DataReceived', nextValue);
};

gc.databind.VariableBindStreamingValue.prototype.updateValue = function(value, progress)
{
	this.notifyDataReceivedListeners(value);
	gc.databind.VariableLookupBindValue.prototype.updateValue.call(this, value, progress);
};


