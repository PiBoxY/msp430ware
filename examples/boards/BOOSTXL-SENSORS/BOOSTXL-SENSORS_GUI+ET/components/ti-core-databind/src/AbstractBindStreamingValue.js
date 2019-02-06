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
 * Abstract class that implements IBindStreamingValue interface. Clients can either 
 * instantiate this class directly or create classes derived from it for 
 * their value bindable object.   
 *
 * @constructor
 * @extends AbstractLookupBindValue
 * @implements gc.databind.IBindStreamingValue
 * 
 * @param {string} [defaultType] - the default type, used only if value is null. 
 */
gc.databind.AbstractBindStreamingValue = function(defaultType) 
{
	gc.databind.AbstractLookupBindValue.call(this);
};

gc.databind.AbstractBindStreamingValue.prototype = new gc.databind.AbstractBind();

/**
 * Add a streaming listener for this bindable object. 
 *  
 * @param {gc.databind.IStreamingListener} listener - callback that will be notified for each new value received.  
 */
gc.databind.AbstractBindStreamingValue.prototype.addStreamingListener = function(listener)
{
	this.fEvents.addListener('DataReceived', listener);
};

/**
 * Remove a streaming listener for this bindable object. 
 *  
 * @param {gc.databind.IStreamingListener} listener - callback that will be notified for each new value received.  
 */
gc.databind.AbstractBindStreamingValue.prototype.removeStreamingListener = function(listener)
{
	this.fEvents.removeListener('DataReceived', listener);
};	

/**
 * Notifies client listener objects when new data is received.  
 * 
 * @param {object} nextValue - value just received
 */
gc.databind.AbstractBindStreamingValue.prototype.notifyDataReceivedListeners = function(nextValue) 
{
	this.fEvents.fireEvent('DataReceived', nextValue);
};

/**
 * Method called when the first streaming listener is added to the list.
 * Derived classes can override this method to be notified for this event.
 */
gc.databind.AbstractBindStreamingValue.prototype.onFirstDataReceivedListenerAdded = function() 
{
};

/**
 * Method called when the last streaming listener is removed from the list.
 * Derived classes can override this method to be notified for this event.
 */
gc.databind.AbstractBindStreamingValue.prototype.onLastDataReceivedListenerRemoved = function() 
{
};

gc.databind.AbstractBindStreamingValue.prototype.updateValue = function(value, progress)
{
	this.notifyDataReceivedListeners(value);
	gc.databind.VariableLookupBindValue.prototype.updateValue.call(this, value, progress);
};



