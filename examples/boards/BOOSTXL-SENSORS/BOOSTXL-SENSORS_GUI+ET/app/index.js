

/*
*  Boilerplate code for creating computed data bindings
*/
document.addEventListener('gc-databind-ready', function() 
{

 
  
    /* 
	*   Add custom computed value databindings here, using the following method:
	*
    *   function gc.databind.registry.bind(targetBinding, modelBinding, [getter], [setter]);
	*
	*   param targetBinding - single binding string or expression, or array of binding strings for multi-way binding.
	*   param modelBinding - single binding string or expression, or array of binding strings for multi-way binding.
	*   param getter - (optional) - custom getter function for computing the targetBinding value(s) based on modelBinding value(s).
	*   param setter - (optional) - custom setter function for computing the modelBinding value(s) based on targetBinding value(s).
    */
	
	// For example, a simple computed values based on simple expression
	// gc.databind.registry.bind('widget.id.propertyName', "targetVariable == 1 ? 'binding is one' : 'binding is not one'");
	
	// Or a custom two-way binding with custome getter and setter functions.  (setter is optional)  (getter only indicates one-way binding)
	// gc.databind.registry.bind('widget.id.propertyName', "targetVariable", function(value) { return value*5/9 + 32; }, function(value) { (return value-32)*9/5; });
	
	// Event 1 to n bindings
	/* 
    gc.databind.registry.bind('widget.date.value', 
        // dependant bindings needed in order to compute the date, in name/value pairs.
        {
            weekday: 'widget.dayOfWeek.selectedText',
            day: 'widget.dayOfMonth.value',
            month: 'widget.month.selectedText',
            year: 'widget.year.value'
        }, 
        // getter for date computation
        function(values) 
        {
            // compute and return the string value to bind to the widget with id 'date'
            return values.weekday + ', ' + values.month + ' ' + values.day + ', ' + values.year;
        }
    ); 
	*/
	
	var scaleFactorBME = 0.001;
	var scaleFactorMag = 0.0625;
	var scaleFactorAccel = 0.0000586888902;
	var scaleFactorGyro = 0.06097560976;
	var scaleFactorPress = 0.000953001
	

    gc.databind.registry.getBinding('uart.light.lux').addChangedListener({
        onValueChanged: function(x, z){
           var widget = document.getElementById('gesture_image');
           var togG = document.getElementById('toggleLightGesture');
           var togS = document.getElementById('toggleOPT');

            if (widget && z<200 && togG.checked && togS.checked) {
                
                    widget.style.opacity = (z/200);
            }
               else  widget.style.opacity = 1;
            
        }
    });
	gc.databind.registry.bind('widget.ti_widget_analoggauge.value','uart.enviro.press',function(z){
	    return ((z*scaleFactorPress).toFixed(2));
	});
	gc.databind.registry.bind('widget.ti_widget_analoggauge1.value','uart.enviro.humid',function(z){
	    return ((z*scaleFactorBME).toFixed(2));
	});
	gc.databind.registry.bind('widget.ti_widget_analoggauge2.value','uart.enviro.amb_temp',function(z){
	    return ((z*.01).toFixed(2));
	});
	gc.databind.registry.bind('widget.ti_widget_analoggauge.odometer','uart.enviro.press',function(z){
	    return ((z*scaleFactorPress).toFixed(2));
	});
	gc.databind.registry.bind('widget.ti_widget_analoggauge1.odometer','uart.enviro.humid',function(z){
	    return ((z*scaleFactorBME).toFixed(2));
	});
	gc.databind.registry.bind('widget.ti_widget_analoggauge2.odometer','uart.enviro.amb_temp',function(z){
	    return ((z*.01).toFixed(2));
	});
	
	gc.databind.registry.bind('widget.ti_widget_magnitudeLabel.label', 
        // dependant bindings needed in order to compute the date, in name/value pairs.
        {
            x: 'uart.accel.x',
            y: 'uart.accel.y',
            z: 'uart.accel.z'
        }, 
        // getter for date computation
        function(value) 
        {
            // compute and return the string value to bind to the widget with id 'date'
            var result = Math.sqrt(Math.pow((value.x*scaleFactorAccel),2)+Math.pow((value.y*scaleFactorAccel),2)+Math.pow((value.z*scaleFactorAccel),2));
            return result.toFixed(3);
        });
    
  	
	//accel
	gc.databind.registry.bind('widget.ti_app_tri_axis_display.xValue','uart.accel.x',function(z){
	    return (z*scaleFactorAccel);
	});
	
	gc.databind.registry.bind('widget.ti_app_tri_axis_display.yValue','uart.accel.y',function(z){
	    return (z*scaleFactorAccel);
	});
	gc.databind.registry.bind('widget.ti_app_tri_axis_display.zValue','uart.accel.z',function(z){
	    return (z*scaleFactorAccel);
	});
	//Gyro
	gc.databind.registry.bind('widget.ti_app_tri_axis_display1.xValue','uart.gyro.x',function(z){
	    return (z*scaleFactorGyro);
	});
	gc.databind.registry.bind('widget.ti_app_tri_axis_display1.yValue','uart.gyro.y',function(z){
	    return (z*scaleFactorGyro);
	});
	gc.databind.registry.bind('widget.ti_app_tri_axis_display1.zValue','uart.gyro.z',function(z){
	    return (z*scaleFactorGyro);
	});
    //Mag
    gc.databind.registry.bind('widget.ti_app_tri_axis_display2.xValue','uart.mag.x',function(z){
	    return (z*scaleFactorMag);
	});
	gc.databind.registry.bind('widget.ti_app_tri_axis_display2.yValue','uart.mag.y',function(z){
	    return (z*scaleFactorMag);
	});
	gc.databind.registry.bind('widget.ti_app_tri_axis_display2.zValue','uart.mag.z',function(z){
	    return (z*scaleFactorMag);
	});

	
	gc.databind.registry.bind('widget.rxTextArea.textContent', 'uart.$rawData', function(data) {
	    try {
	        return JSON.stringify(data, null, 4);
	    }
	    catch (e) {
	        return e.toString();
	    }
	    
	});

});


    var openLink = function(link){
		var isNodeWebkit = (typeof process !== undefined);
	    if (isNodeWebkit) {
	        // open link with default browser
	       var gui = require('nw.gui');
           gui.Shell.openExternal(link);
        } else {
            // open link in a new window
           window.open(link);
		}
    }


/*
*  Boilerplate code for creating custom actions
*/
document.addEventListener('gc-nav-ready', function() 
{
    /* 
	*   Add custom actions for menu items using the following api:
	*
    *   function gc.nav.registryAction(id, runable, [isAvailable], [isVisible]);
	*
	*   param id - uniquely identifies the action, and should correspond to the action property of the menuaction widget.
	*   param runable - function that performs the custom action.
	*   param isAvailable - (optional) - function called when the menu action is about to appear.  Return false to disable the action, or true to enable it.
	*   param isVisible - (optional) - function called when the menu action is about to appear.  Return false to hide the action, or true to make it visible.
    */
	
	// For example,
	// gc.nav.registerAction('myCustomCloseAction', function() { window.close(); }, function() { return true; }, function() { return true; });
	
	// Alternatively, to programmatically disable a menu action at any time use:
	// gc.nav.disableAction('myCustomCloseAction);    then enable it again using:  gc.nav.enableAction('myCustomCloseAction'); 

	gc.nav.registerAction('onlineHelpAction',function(){openLink("http://processors.wiki.ti.com/index.php/TI_Cloud_Agent_Troubleshooting_Guide");},function() {return true;}, function(){ return true;});
	gc.nav.registerAction('e2eAction',function(){openLink("http://e2e.ti.com/support/development_tools/code_composer_studio/");},function() {return true;}, function(){ return true;});
	
    

 


});
    


/*
*  Boilerplate code for working with components in the application gist
*/

var templateObj; // need to wait for polymer-ready event before getting element
var spawn = require('child_process').spawn; //ENERGYTRACE
var fs = require('fs');                     //ENERGYTRACE
var stuneIndex = 0;                         //ENERGYTRACE
var etNumRuns = 0;
var isCalculating = false; 

// variables for reading csv file
var bite_size = 256,
    readbytes = 0,
    measuringET = 0,
    readingFile = 0,
    file,
    lastLineFeed,
    lineArray,
    currentArray,
    powerArray;


var isImageVisible = true;

document.addEventListener('polymer-ready', function() {
    templateObj = document.querySelector('#template_obj');
	templateObj.imageIndex = 0;
    templateObj.selectedDeviceIndex = 0;
    templateObj.isRunning = false;
    templateObj.backplane = undefined;
    templateObj.serialIO = undefined;
    templateObj.dataFromSerialPort = [];
    templateObj.unusedChars = undefined;
    templateObj.numBadValues = 0;  // number of values that are not valid ascii characters
    templateObj.rxDataCount = 0;  // number of characters received

    /* By default, use the device Info provided by the New Project Wizard.
     * If you wish to support more than one device, you can set the device info programmatically
     * by uncommenting the following data structure and its usage in the templateObj.ready function
     * at the bottom of this file.*/
/*	templateObj.deviceInfo =  [
        {   boardName: 'MSP-EXP430G2 Rev 1.5',
            deviceName: 'MSP430G2553',
            fileFolderName: 'app/binfiles/',
            fileName: 'ReadTempJSON_2553.cpp.hex',
            startBtnName: 'P1.1',
            msgWaitingForData: "Press the Start Button (P1.1 on your MSP-EXP430G2553 Launchpad) to start reading the temperature - the Green LED should turn on.",
            msgConnectDevice: "Please connect your MSP-EXP430G2553 Launchpad to a USB port of your computer and then click Connect.",
            imageFolderName:'images',
            boardImage:'launchpad-mspexp430g2-02.jpg',
            startBtnImage:'startbutton-mspexp430g2-02.jpg',
            id: 'menuItemForDevice_0',
            index: 0,
            selected: true
        }];
   */
   
   //ENERGYTRACE
       function measureCurrent(stune) {
        if (stuneIndex == 1)
        {
            console.log("connect -d msp432 -c ccBoard0.dat xds\n");
            stune.stdin.write("connect -d msp432 -c ccBoard0.dat xds\n")
        }
        if (stuneIndex == 2)
        {
            var etms = templateObj.$.etms_selector.value;
            
            console.log("energytrace -o current.csv -D " + etms + " et\n");
            stune.stdin.write("energytrace -o current.csv -D " + etms + " et\n")
            measuringET = 1;
            readingFile = 1;
            readbytes = 0;
            currentArray = [];
            powerArray = [];
            fs.unlink('../app/resources/commands/ccs_base/emulation/analysis/bin/stune/current.csv', function(err){console.log(err); readFile();});

        }
        if (stuneIndex == 3)
        {
            console.log("quit\n");
            stune.stdin.write("quit\n")
            stuneIndex = 0;
        }
    }
    
    function readFile() {
        fs.open('../app/resources/commands/ccs_base/emulation/analysis/bin/stune/current.csv', 'r', function(err, fd) {
            if (err){
                console.log(err);
                readFile();
            }
            else
            {
                file = fd;
                readsome();
            }
        });
        
        function readsome() {
            var stats = fs.fstatSync(file); // yes sometimes async does not make sense!
            if(stats.size<readbytes+1) {
                console.log('Waiting for new data to read...');
                if (measuringET)
                    setTimeout(readsome, 300);
                else
                {
                    readingFile = 0;
                    fs.close(file);
                    
                    
                    var d = []; 
                     d = currentArray; ;
                        
                    var avg = 0;
                    for (i = 0; i <= d.length-1; i++){
                        avg += parseFloat(d[i]);
                    }
                    avg /= d.length;
                        
                    if (avg == 0 || isNaN(avg)) {
                        console.log("\nFailed to measure current");
                    }
                    else {
                        console.log("Average current: " + avg);
                        templateObj.$.et_label.label= (avg.toFixed(4)) + " mA";
                        isCalculating=false;
                        
                    }
                    
                }
                    
            }
            else {
                fs.read(file, new Buffer(bite_size), 0, bite_size, readbytes, processsome);
            }
        }
        
        function processsome(err, bytecount, buff) {
            lastLineFeed = buff.toString('utf-8', 0, bytecount).lastIndexOf('\n');
        
            if(lastLineFeed > -1){
        
                // Split the buffer by line
                lineArray = buff.toString('utf-8', 0, bytecount).slice(0,lastLineFeed).split('\n');
                
                //console.log("NEWLINE: " + lineArray);
        
                for(i=0;i<lineArray.length;i++){
                    // split each line by comma
                    var line = lineArray[i].split(',');
                    
                    // push current measurement into array
                    if (!isNaN(parseFloat(line[2])))
                        currentArray.push(parseFloat(line[2])/1000000);
                    // push power measurement into array
                    if (!isNaN(parseFloat(line[3])))
                        powerArray.push(parseFloat(line[3]));
                }
                
                // Update LineGraph with live data from csv
            
                templateObj.$.ti_widget_linegraph.series_1_Values = currentArray;
               //templateObj.$.ti_widget_linegraph.series_1_Values = powerArray;
        
                // Set a new position to read from
                readbytes+=lastLineFeed+1;
            } else {
                // No complete lines were read
                readbytes+=bytecount;
            }
            if (readingFile)
                process.nextTick(readsome);
            else
                fs.close(file);
        }
    }

    
    
    
    templateObj.etButtonClickHdlr = function(e){
        console.log("etButtonClicked");
        
        var optStat, tmpStat, bmeStat, accStat, gyroStat, magStat; 
        
        if (isCalculating == false){
            isCalculating = true;
            if (etNumRuns > 0) {
                if (etNumRuns > 1 ) {
                     templateObj.$.settings_bitfield2.bitValues =  templateObj.$.settings_bitfield1.bitValues;
                    templateObj.$.et_label2.label = templateObj.$.et_label1.label;
                }
                
                templateObj.$.settings_bitfield1.bitValues =  templateObj.$.settings_bitfield.bitValues;
                templateObj.$.et_label1.label = templateObj.$.et_label.label;
            }
            etNumRuns++;     
            
            if (templateObj.$.toggleOPT.checked){
                optStat = templateObj.$.ti_widget_droplist1.selectedText;
            }
            else{
                optStat = "OFF";
            }
            
            if (templateObj.$.toggleTMP.checked){
                tmpStat = templateObj.$.ti_widget_droplist.selectedText;
            }
            else{
                tmpStat = "OFF";
            }
            
            if (templateObj.$.toggleBME.checked){
                bmeStat = templateObj.$.ti_widget_droplist2.selectedText;
            }
            else{
                bmeStat = "OFF";
            }
            
            accStat = templateObj.$.ti_widget_droplist3.selectedText;
            
            
            if (templateObj.$.toggleIMU.checked){
                gyroStat = templateObj.$.ti_widget_droplist3.selectedText;
                magStat = templateObj.$.ti_widget_droplist4.selectedText;
                
            }
            else{
                gyroStat = "OFF";
                magStat = "OFF";
            }
            
            templateObj.$.settings_bitfield.bitValues = optStat + "," + tmpStat + "," + bmeStat + "," + accStat + "," + gyroStat + "," + magStat;
            
            templateObj.$.et_label.label = "Calculating"
            
            var stune = spawn('cmd', ['/c', 'stune.exe'], {cwd: "../app/resources/commands/ccs_base/emulation/analysis/bin/stune/"});
            
            stune.on('exit', function (data) {
                console.log("Parsing CSV current measurement file");
                var csv;
                measuringET = 0;
                
                stune.kill('SIGINT');
            });
            stune.stdout.on('data', function(data) {
                var output = data.toString();
                console.log(output);
                if (output.indexOf("stune>") >= 0)
                {
                    stuneIndex++;
                    measureCurrent(stune);
                    
                }
            });
            stune.stderr.pipe(process.stderr);
            
        }
    };


    templateObj.btnVisibilityChangedHdlr = function(){
        

    };
    
    templateObj.serialPortVisibilityChangedHdlr = function (e) {

        if (templateObj.serialIO.isSerialPortUIVisible) {
           // templateObj.$.ti_widget_button.disabled = false;

        } else {
            //templateObj.$.ti_widget_button.disabled = true;
        }
    
    };
    
    templateObj.statusUpdateHdlr = function(e){
       // templateObj.$.status_label.label = templateObj.serialIO.status;
    };
    
    templateObj.onButtonClickHdlr = function(e){
        if (templateObj.$.ti_widget_button.innerHTML === "Open Serial Port"){
           templateObj.serialIO.updateStateMachine('userOpenSerialPort');
           templateObj.$.ti_widget_button.innerHTML = "Close Serial Port";
        } else {
           templateObj.serialIO.updateStateMachine('userCloseSerialPort');
           templateObj.$.ti_widget_button.innerHTML = "Open Serial Port";
        }
    };
    
    
    templateObj.onConnectButtonClickHdlr = function(e){
        if (templateObj.$.ti_widget_button_connect.innerHTML === 'Disconnect'){
            templateObj.backplane.disconnect();
            templateObj.$.ti_widget_button_connect.disabled = true;
        } else {
            templateObj.backplane.connect();
        }
    };
    
    
    
    templateObj.serialSend = function(){
        templateObj.serialIO.serialSend(templateObj.$.serialTx.value);
    };
    
    templateObj.clearSerial = function(){
        templateObj.$.rxTextArea.value="";
    };
    
    templateObj.isValueValid = function(val){
        //var result = true;
        // replace the following logic with whatever makes sense for your application:
        //if (val === 0){
            // result = false;
            // templateObj.numBadValues++;
        //}
        //return result;
    };
/*
    templateObj.serialPortOutputHdlr = {
        text: function (message,buffer) {
            var MAX_NUM_CHARACTERS = 1024;  // You  might want to change this to e.g. 2x the max length of an incoming message
            var oldText = templateObj.$.rxTextArea.value;
            var oldLen = oldText.length;
            var newLen = message.length + oldLen;
            if (newLen > MAX_NUM_CHARACTERS){
                oldLen = newLen - MAX_NUM_CHARACTERS;
                if (oldLen > 0){
                    oldText = oldText.substring(oldLen);    
                } else {
                    oldText = "";
                }
                
            }
            
            templateObj.$.rxTextArea.value = oldText + message;
        }
    
    };
*/

/* works:
    templateObj.serialPortOutputHdlr = {
        text: function (message,buffer) {
            //$scope.serialOut += message;
            var n = 0;
            //console.log('in $scope.serialPortOutput: message = '+message);
    
            var debugStr = '';
            var newValue = '';
            var NUM_VALUES_TO_PLOT = 295;
    
            templateObj.$.rxTextArea.value = message;
            
            
            try{
            var parsedJSON = JSON.parse(message);
            var gestureCode = parsedJSON.dominant.code;
            }
            catch(err){
                
            }
                
                if(templateObj.$.toggleGestures.checked)
                {
                    switch(gestureCode){
                        case 0:
                            
                            templateObj.$.gesture_image.imagePath = "images/no_gesture.svg";
                            
                            break;
                        case 1:
                            if (parsedJSON.gyro.x > 0)
                            {
                            templateObj.$.gesture_image.imagePath = "images/rot_x_pos.svg";
                            }
                            else templateObj.$.gesture_image.imagePath = "images/rot_x_neg.svg";
                            
                            break;
        
                        case 2:
                            if (parsedJSON.gyro.y > 0)
                            {
                            templateObj.$.gesture_image.imagePath = "images/rot_y_pos.svg";
                            }
                            else templateObj.$.gesture_image.imagePath = "images/rot_y_neg.svg";
                            
                            break;
        
                        case 3:
                            if (parsedJSON.gyro.z > 0)
                            {
                            templateObj.$.gesture_image.imagePath = "images/rot_z_pos.svg";
                            }
                            else templateObj.$.gesture_image.imagePath = "images/rot_z_neg.svg";
                            
                            break;
                            
                        case 4:
                            templateObj.$.gesture_image.imagePath = "images/trav_x.svg";
                            
                            break;
        
                        case 5:
                            templateObj.$.gesture_image.imagePath = "images/trav_y.svg";
                            
                            break;
        
                        case 6:
                            templateObj.$.gesture_image.imagePath = "images/trav_z.svg";
                            
                            break;
                        
                        default:
                        break;
                            
                    }
                }
                
                else {
                	templateObj.$.gesture_image.imagePath = "images/disabled_gestures.svg";
                }


            } 
    
        };
*/
    var receivedMsgs = '';
    var msgHistory = [0];
    var maxNumMsgsToDisplay = 1;
    
    templateObj.serialPortOutputHdlr = {
        text: function (rxMsg,buffer) {
            //$scope.serialOut += message;
            var n = 0;
            //console.log('in $scope.serialPortOutput: message = '+message);
    
            var debugStr = '';
            var newValue = '';
            var NUM_VALUES_TO_PLOT = 295;
            receivedMsgs = receivedMsgs + rxMsg;
            var message = '';
            while (receivedMsgs.indexOf('\n') !== -1){
                message = receivedMsgs.substring(0,receivedMsgs.indexOf('\n')+1);
		        receivedMsgs = receivedMsgs.substring(message.length);

                msgHistory.push(message);
                if (msgHistory.length > maxNumMsgsToDisplay){
                    msgHistory.shift();  // remove the first element from the list
                }
                templateObj.$.rxTextArea.value = msgHistory.join();
                //console.log("Rx msg = "+message);
                try{
                    var parsedJSON = JSON.parse(message);
                    var gestureCode = parsedJSON.dominant.code;
                }
                catch(err){
                    //console.log("Failed to parse messge ["+message+"]")
                }
                    
                    if(templateObj.$.toggleGestures.checked)
                    {
						var ok = true;
						if (gestureCode > 0) {
							if ((!parsedJSON) || (!parsedJSON.gyro)) {
								//console.log('Bad parsedJSON: '+message); //parsedJSON);

								ok = false;
							}
						}
						if (ok) {
							switch(gestureCode){
								case 0:
									
									templateObj.$.gesture_image.imagePath = "images/no_gesture.svg";
									
									break;
								case 1:
									
									if (parsedJSON.gyro.x > 0)
									{
									templateObj.$.gesture_image.imagePath = "images/rot_x_pos.svg";
									}
									else templateObj.$.gesture_image.imagePath = "images/rot_x_neg.svg";
									
									break;
				
								case 2:
									if (parsedJSON.gyro.y > 0)
									{
									templateObj.$.gesture_image.imagePath = "images/rot_y_pos.svg";
									}
									else templateObj.$.gesture_image.imagePath = "images/rot_y_neg.svg";
									
									break;
				
								case 3:
									if (parsedJSON.gyro.z > 0)
									{
									templateObj.$.gesture_image.imagePath = "images/rot_z_pos.svg";
									}
									else templateObj.$.gesture_image.imagePath = "images/rot_z_neg.svg";
									
									break;
									
								case 4:
									templateObj.$.gesture_image.imagePath = "images/trav_x.svg";
									
									break;
				
								case 5:
									templateObj.$.gesture_image.imagePath = "images/trav_y.svg";
									
									break;
				
								case 6:
									templateObj.$.gesture_image.imagePath = "images/trav_z.svg";
									
									break;
								
								default:
								break;
									
							}
						}
                    }
                    
                    else {
                    	templateObj.$.gesture_image.imagePath = "images/disabled_gestures.svg";
                    }

                }
            } 
    
        };
    
   
   
templateObj.programDownloadCompleteHdlr = function(event){
        if (templateObj.programLoader.wasProgramDownloadSuccessful){
            // programming the target device has completed.
            // The safest way to reconnect at this point is to reload the application.
            // Wait for 2 seconds to allow the 'Flash successful' message to be displayed.
            window.setTimeout(function(){
                window.location.reload(true);
            },2000);
        }    
    }// Please use this file to pull in any additional javascript files needed by your application

   

    
	templateObj.ready = function(){
	   templateObj.async(function(){
	       // to access a widget with an id of 'widget_id', use templateObj.$.widgetId 
	        templateObj.backplane = gc.services['ti-core-backplane'];
	        templateObj.$.et_button.addEventListener("click", templateObj.etButtonClickHdlr); //ENERGYTRACE
            templateObj.backplane.addEventListener("btnVisibilityChanged", templateObj.btnVisibilityChangedHdlr);
            templateObj.serialIO = gc.services['ti-service-serialio']
            templateObj.serialIO.addEventListener("serialPortUIVisibilityChanged",templateObj.serialPortVisibilityChangedHdlr);
            templateObj.programLoader = gc.services['ti-service-programloader']
            templateObj.programLoader.addEventListener("programDownloadComplete",templateObj.programDownloadCompleteHdlr)
            templateObj.serialIO.addEventListener("statusMsgUpdated",templateObj.statusUpdateHdlr);
            templateObj.serialIO.addEventListener("serialPortsReadyToBeOpened",templateObj.serialPortVisibilityChangedHdlr);
            templateObj.serialIO.serialPortTextOutputHdlr = templateObj.serialPortOutputHdlr;
            
            templateObj.$.uart.addEventListener('serialioModelReceivingData',function(){
                    window.setTimeout(function(){
                        //gc.databind.registry.getBinding('uart.i').setValue(0);
                        gc.databind.registry.getBinding('uart.i').setValue(1); 
                    }, 100);
            });
            
            templateObj.$.toggleOPT.addEventListener('changed',function(event){
                console.log('enable OPT clicked');
        	    var enabled = templateObj.$.toggleOPT.checked;
        	    var intValue = 0;
        	    if (enabled){
        	        intValue = 1;
        	    }
        	    gc.databind.registry.getBinding('uart.o').setValue(intValue);

        	});
            
        	templateObj.$.ti_widget_droplist.selectedIndex = 0;
            templateObj.$.ti_widget_droplist.addEventListener('click',function(event){
                console.log('tmp Hz changed');
        	    var intValue = templateObj.$.ti_widget_droplist.selectedValue;
        	   // var intValue = 0;
        	   // if (enabled){
        	   //     intValue = 1;
        	   // }
        	    gc.databind.registry.getBinding('uart.st').setValue(intValue);

        	});
        	
            templateObj.$.ti_widget_droplist1.selectedIndex = 0;
            templateObj.$.ti_widget_droplist1.addEventListener('click',function(event){
                console.log('opt Hz changed');
        	    var intValue = templateObj.$.ti_widget_droplist1.selectedValue;
        	   // var intValue = 0;
        	   // if (enabled){
        	   //     intValue = 1;
        	   // }
        	    gc.databind.registry.getBinding('uart.so').setValue(intValue);

        	});
        	
        	            templateObj.$.ti_widget_droplist2.selectedIndex = 0;
            templateObj.$.ti_widget_droplist2.addEventListener('click',function(event){
                console.log('bme Hz changed');
        	    var intValue = templateObj.$.ti_widget_droplist2.selectedValue;
        	   // var intValue = 0;
        	   // if (enabled){
        	   //     intValue = 1;
        	   // }
        	    gc.databind.registry.getBinding('uart.se').setValue(intValue);

        	});
        	
        	templateObj.$.ti_widget_droplist3.selectedIndex = 2;
            templateObj.$.ti_widget_droplist3.addEventListener('click',function(event){
                console.log('bmi Hz changed');
        	    var intValue = templateObj.$.ti_widget_droplist3.selectedValue;
        	   // var intValue = 0;
        	   // if (enabled){
        	   //     intValue = 1;
        	   // }
        	    gc.databind.registry.getBinding('uart.si').setValue(intValue);

        	});
        	
        	templateObj.$.ti_widget_droplist4.selectedIndex = 5;
            templateObj.$.ti_widget_droplist4.addEventListener('click',function(event){
                console.log('bmm Hz changed');
        	    var intValue = templateObj.$.ti_widget_droplist4.selectedValue;
        	   // var intValue = 0;
        	   // if (enabled){
        	   //     intValue = 1;
        	   // }
        	    gc.databind.registry.getBinding('uart.sm').setValue(intValue);

        	});
        	
        	
        	templateObj.$.toggleTMP.checked = true;
        	templateObj.$.toggleTMP.addEventListener('changed',function(event){
        	    console.log('enable TMP clicked');
        	    var enabled = templateObj.$.toggleTMP.checked;
        	    var intValue = 0;
        	    if (enabled){
        	        intValue = 1;
        	    }
        	    gc.databind.registry.getBinding('uart.t').setValue(intValue);

        	});
        	
        	templateObj.$.toggleBME.checked = true;
        	templateObj.$.toggleBME.addEventListener('changed',function(event){
        	    console.log('enable BME clicked');
        	    var enabled = templateObj.$.toggleBME.checked;
        	    var intValue = 0;
        	    if (enabled){
        	        intValue = 1;
        	    }
        	    gc.databind.registry.getBinding('uart.e').setValue(intValue);

        	});
        	
        	templateObj.$.toggleIMU.checked = true;
        	templateObj.$.toggleIMU.addEventListener('changed',function(event){
        	    console.log('enable AccGyro clicked');
        	    var enabled = templateObj.$.toggleIMU.checked;
        	    var intValue = 0;
        	    if (enabled){
        	        intValue = 1;
        	    }
        	    gc.databind.registry.getBinding('uart.i').setValue(intValue);
        	    
        	});

        	templateObj.$.toggleGestures.checked = true;
        	templateObj.$.toggleGestures.addEventListener('changed',function(event){
        	    console.log('enable Gestures clicked');
        	    var enabled = templateObj.$.toggleGestures.checked;
        	    var intValue = 0;
        	    if (enabled){
        	        intValue = 1;
        	    }
        	    gc.databind.registry.getBinding('uart.r').setValue(intValue);
        	    
        	});
        	/*
        	templateObj.$.paper_icon_button.addEventListener('click',function(event){

        	        templateObj.$.light_image.visible = !templateObj.$.light_image.visible;
        	        
        	        templateObj.$.tmp_image.visible = false;
        	        templateObj.$.enviro_image.visible = false
        	        templateObj.$.accel_image.visible = false;
        	        templateObj.$.gyro_image.visible = false
        	        templateObj.$.mag_image.visible = false;
        	        templateObj.$.et_image.visible = false;
        	        
        	})
        	templateObj.$.paper_icon_button1.addEventListener('click',function(event){

        	        templateObj.$.tmp_image.visible = !templateObj.$.tmp_image.visible;
        	        
        	        templateObj.$.light_image.visible = false;
        	        templateObj.$.enviro_image.visible = false
        	        templateObj.$.accel_image.visible = false;
        	        templateObj.$.gyro_image.visible = false
        	        templateObj.$.mag_image.visible = false;
                    templateObj.$.et_image.visible = false;
        	   
        	})
        	templateObj.$.paper_icon_button2.addEventListener('click',function(event){

        	        templateObj.$.enviro_image.visible = !templateObj.$.enviro_image.visible;
        	        
        	        templateObj.$.light_image.visible = false;
        	        templateObj.$.tmp_image.visible = false;
        	        templateObj.$.accel_image.visible = false;
        	        templateObj.$.gyro_image.visible = false
        	        templateObj.$.mag_image.visible = false;
                    templateObj.$.et_image.visible = false;
        	   
        	})
        	templateObj.$.paper_icon_button3.addEventListener('click',function(event){

        	        templateObj.$.accel_image.visible = !templateObj.$.accel_image.visible;
        	        
        	        templateObj.$.light_image.visible = false;
        	        templateObj.$.tmp_image.visible = false;
        	        templateObj.$.enviro_image.visible = false
        	        templateObj.$.gyro_image.visible = false
        	        templateObj.$.mag_image.visible = false;
                    templateObj.$.et_image.visible = false;
        	   
        	})
        
        	templateObj.$.paper_icon_button4.addEventListener('click',function(event){

        	        templateObj.$.gyro_image.visible = !templateObj.$.gyro_image.visible;
        	        
        	        templateObj.$.light_image.visible = false;
        	        templateObj.$.tmp_image.visible = false;
        	        templateObj.$.enviro_image.visible = false
        	        templateObj.$.accel_image.visible = false;
        	        templateObj.$.mag_image.visible = false;
                    templateObj.$.et_image.visible = false;
        	   
        	})
        	templateObj.$.paper_icon_button5.addEventListener('click',function(event){

        	        templateObj.$.mag_image.visible = !templateObj.$.mag_image.visible;
        	        
        	        templateObj.$.light_image.visible = false;
        	        templateObj.$.tmp_image.visible = false;
        	        templateObj.$.enviro_image.visible = false
        	        templateObj.$.accel_image.visible = false;
        	        templateObj.$.gyro_image.visible = false
        	        templateObj.$.et_image.visible = false;
        	   
        	})
        	
        		templateObj.$.et_hint_btn.addEventListener('click',function(event){

        	        templateObj.$.et_image.visible = !templateObj.$.et_image.visible;
        	        
        	        templateObj.$.mag_image.visible= false;
        	        templateObj.$.light_image.visible = false;
        	        templateObj.$.tmp_image.visible = false;
        	        templateObj.$.enviro_image.visible = false
        	        templateObj.$.accel_image.visible = false;
        	        templateObj.$.gyro_image.visible = false
        	        
        	   
        	})
*/

        	
	   },1);
	};
});

   