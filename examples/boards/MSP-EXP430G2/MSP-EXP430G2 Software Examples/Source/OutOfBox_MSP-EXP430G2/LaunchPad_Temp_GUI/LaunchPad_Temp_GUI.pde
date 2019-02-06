/*
 * LaunchPad_Temp_GUI.pde
 *
 * LaunchPad Temperature GUI
 *
 * Copyright (C) {YEAR} Texas Instruments Incorporated - http://www.ti.com/ 
 * 
 * 
 *  Redistribution and use in source and binary forms, with or without 
 *  modification, are permitted provided that the following conditions 
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright 
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the   
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
*/


/**
 * LaunchPad GUI
 * 
 * Select COM port that LaunchPad is connected to
 * receive serial communication from LaunchPad for 
 * instantaneous temperature readings.
 *
**/
PFont fontA;
char instruct;
color backColor;
int dataRead;
boolean portChosen = false;
int COMPort;
int [] keyIn = new int[3];
int i, keyIndex=0;



// The serial port:
Serial myPort; 

void setup()
{
  //setup window

  size(1000, 500);
  smooth();
  //load font

  
  // Set the font, its size (in units of pixels), and alignment
  textAlign(CENTER);
  
  //Set background color
  color baseColor = color(70);
  backColor = baseColor;
  //import serial library
  import processing.serial.*;

  // List all the available serial ports, then give prompt
  println(Serial.list());
  println("Please type in the serial COM port that your LaunchPad is connected to.");
  
  background(backColor);
  stroke(255);
  textSize(13);
  text("Please, select the COM port that your LaunchPad is connected to.", 265, 20);
  textAlign(LEFT);
  for(i=0; i<Serial.list().length; i++){
    text("[" + i + "] " + Serial.list()[i], 215, 38+13*i);
  }
  textSize(30);
  text("# =",20, 350);
  //wait for keypress --> Refer to void keyPressed()
}

void draw()
{
  if(portChosen == true){
    dataRead= myPort.read();
    if(dataRead != -1){
      
      //clear previous temperature reading from screen
      background(backColor);
      stroke(255);
      //Update console
      print("Temp: ");
      print(dataRead); 
      println("°");
      
      //Update on screen GUI
      text("Current Temperature: ", 420, 60);
      text(dataRead, 835, 60);      
      text("°", 900, 60);      
      
    }
  }
}

//wait for key press. Once key is entered, initialize serial com port
void keyPressed() { 
  if(portChosen == false){
    
    if (key != 10) //Enter
    {
      keyIn[keyIndex++] = key-48;
      textSize(30);
      text(keyIn[keyIndex-1],100 + (keyIndex*20), 350);
    }
    else
    {
      COMPort = 0;
      for (i = 0; i < keyIndex; i++)
        COMPort = COMPort * 10 + keyIn[i];      
      println(COMPort);
      myPort = new Serial(this, Serial.list()[COMPort], 2400);
      portChosen = true;
      textSize( 60); // change font size & alignment for temp readings
      textAlign(CENTER);
    }
  }
}

