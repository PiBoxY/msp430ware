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
  fontA = loadFont("CourierNew36.vlw");
  
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
  textFont(fontA, 13);
  text("Please, select the COM port that your LaunchPad is connected to.", 265, 20);
  textAlign(LEFT);
  for(i=0; i<Serial.list().length; i++){
    text("[" + i + "] " + Serial.list()[i], 215, 38+13*i);
  }
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

