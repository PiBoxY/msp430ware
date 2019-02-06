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
 * This interface represents bindable object whose value can be streamed. 
 *  
 * If a binding is provided by a target that streams data (like a usb port)
 * then this binding has the ability to pass the data to the client whether or not
 * the data has changed or not since the last data was received. 
 * 
 *  @interface
 *  @extends gc.databind.ILookupBindValue
 */
gc.databind.IBindStreamingValue = function()
{
};

gc.databind.IBindStreamingValue.prototype = new gc.databind.ILookupBindValue();

/**
 * Add a streaming listener for this bindable object. 
 *  
 * @param {gc.databind.IStreamingListener} listener - callback that will be notified for each new value received.  
 */
gc.databind.IStreamingListener.prototype.addStreamingListener = function(listener)
{
};

/**
 * Remove a streaming listener for this bindable object. 
 *  
 * @param {gc.databind.IStreamingListener} listener - callback that will be notified for each new value received.  
 */
gc.databind.IStreamingListener.prototype.removeStreamingListener = function(listener)
{
};