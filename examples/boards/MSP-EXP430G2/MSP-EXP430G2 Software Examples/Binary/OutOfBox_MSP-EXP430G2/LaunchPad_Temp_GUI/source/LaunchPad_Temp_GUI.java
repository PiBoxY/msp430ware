import processing.core.*; 
import processing.xml.*; 

import processing.serial.*; 

import java.applet.*; 
import java.awt.Dimension; 
import java.awt.Frame; 
import java.awt.event.MouseEvent; 
import java.awt.event.KeyEvent; 
import java.awt.event.FocusEvent; 
import java.awt.Image; 
import java.io.*; 
import java.net.*; 
import java.text.*; 
import java.util.*; 
import java.util.zip.*; 
import java.util.regex.*; 

public class LaunchPad_Temp_GUI extends PApplet {

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
int backColor;
int dataRead;
boolean portChosen = false;
int COMPort;
int [] keyIn = new int[3];
int i, keyIndex=0;



// The serial port:
Serial myPort; 

public void setup()
{
  //setup window

  size(1000, 500);
  smooth();
  //load font
  fontA = loadFont("CourierNew36.vlw");
  
  // Set the font, its size (in units of pixels), and alignment
  textAlign(CENTER);
  
  //Set background color
  int baseColor = color(70);
  backColor = baseColor;
  //import serial library
  

  // List all the available serial ports, then give prompt
  println(Serial.list());
  println("Please type in the serial COM port that your LaunchPad is connected to.");
  
  background(backColor);
  stroke(255);
  textFont(fontA, 13);
  text("Please, select the COM port that your LaunchPad is connected to.", 265, 20);
  textAlign(LEFT);
  for(i=0; i<Serial.list().length; i++){
    text("[" + i + "] " + Serial.list()[i], 215, 38+13*i);
  }
  //wait for keypress --> Refer to void keyPressed()
}

public void draw()
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
      println("\u00b0");
      
      //Update on screen GUI
      text("Current Temperature: ", 420, 60);
      text(dataRead, 835, 60);      
      text("\u00b0", 900, 60);      
      
    }
  }
}

//wait for key press. Once key is entered, initialize serial com port
public void keyPressed() { 
  if(portChosen == false){
    
    if (key != 10) //Enter
      keyIn[keyIndex++] = key-48;
    else
    {
      COMPort = 0;
      for (i = 0; i < keyIndex; i++)
        COMPort = COMPort * 10 + keyIn[i];      
      println(COMPort);
      myPort = new Serial(this, Serial.list()[COMPort], 2400);
      portChosen = true;
      textFont(fontA, 60); // change font size & alignment for temp readings
      textAlign(CENTER);
    }
  }
}


  static public void main(String args[]) {
    PApplet.main(new String[] { "--bgcolor=#E2E2E2", "LaunchPad_Temp_GUI" });
  }
}
