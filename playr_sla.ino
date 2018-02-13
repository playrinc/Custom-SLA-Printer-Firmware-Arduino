//Playr Labs SLA Printer
//2016 Chris Galzerano
//Playr Inc

//Current Tank Depth: 2.8cm
//Planned Depth Tank: 3.0cm

//Distance between calibration marks: 9.4cm
//Steps between calibration: 

//Layer Thickness: 0.050mm
//Layer Time: 8000ms

//Bottom Layer Time: 14000ms
//Number of bottom layers: 3

//Direction: Plate steps DOWN
//Unit: mm

//x. pause for bottom layer time
//1. Set slice number
//2. G1 Z-5.0 F25.0     (Move down 5mm at speed 25%)
//   G1 Z4.95 F100.0    (Move up 4.95mm at speed 100%)
//3. pause for 10600ms
//4. Repeat x to 3 two more times (for a total of 3, the number of bottom layers)
//5. set x to pause for layer time
//6. repeat x to 3 for the number of layers remaining
//7. pause a bit
//8. Raise the plate up to display the print

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
int Distance = 0;  // Record the number of steps we've taken

//Config Variables
float tankDepth = 30;
float calibrationDistance = 94;
float stepsForCalibration = 18500;
float layerThickness = 0.025;
float layerTime = 8000;
float bottomLayers = 3;
float bottomLayerTime = 14000;
float g1Distance = 5.00;
float g1Speed = 0.25;
float g2Distance = 4.95;
float g2Speed = 1;
float interim = 10600;
int totalLayers = 0;
int currentLayer = 0;

float calibrationSteps = 0;
float stepsPerMm = 196.00;

void setup() {     
  //Setup Motors
          
  pinMode(8, OUTPUT);     
  pinMode(9, OUTPUT); 
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);

  //Setup Serial
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Playr SLA Printer 1.0"); 
  inputString.reserve(200);
  
}

void step(int steps) {
  for (int i = 0; i < steps; i++) {
    digitalWrite(9, HIGH);
    delay(1);          
    digitalWrite(9, LOW); 
    delay(1); 
    calibrationSteps++;
  }
}

void stepAtSpeed(int steps, float speed2) {
  for (int i = 0; i < steps; i++) {
    digitalWrite(9, HIGH);
    delay(1/speed2);          
    digitalWrite(9, LOW); 
    delay(1/speed2); 
    calibrationSteps++;
  }
}

void moveDistanceAtSpeed(float distance, float speed2) {
  stepAtSpeed(distance*stepsPerMm, speed2);
}

void loop() {
  if (stringComplete) { 
    if (inputString.equals("direction up")) {
      Serial.println("Direction set to move up.");
      setMotorDirectionUp(); 
    }
    else if (inputString.equals("direction down")) {
      Serial.println("Direction set to move down."); 
      setMotorDirectionDown();
    }
    else if (inputString.equals("help")) {
       Serial.println("Playr SLA Help Menu\r\n1. direction [up/down]\r\n2. step [int number of steps]");
    }
    else if (inputString.equals("stop")) {
       stopMotors();
       Serial.println("Motors Stopped.");
    }
    if (inputString.equals("print")) {
       Serial.println("Beginning Print");
       Serial.println(String(currentLayer) + " out of " + String(totalLayers) + " layers");
       startPrinting();
    }
    if (inputString.equals("stopPrint")) {
       Serial.println("Stopping Print");
       stopPrinting();
    }
    if (inputString.equals("resetCalibration")) {
      Serial.println("Current steps for calibration: 0");
      calibrationSteps = 0; 
    }
    if (inputString.equals("testFunction")) {
      testFunction();
    }
    if (inputString.equals("calibrate")) {
      stepsForCalibration = calibrationSteps;
      stepsPerMm = stepsForCalibration/calibrationDistance;
      Serial.println("Axis Calibrated");
      Serial.println("stepsForCalibration " + String(stepsForCalibration));
    }
    else if (inputString.indexOf("setTankDepth") != -1) {
       String value = inputString.substring(12);
       tankDepth = value.toFloat();
       Serial.println("Tank depth set to " + value);
    }
    else if (inputString.indexOf("setCalibrationDistance") != -1) {
       String value = inputString.substring(22);
       calibrationDistance = value.toFloat();
       Serial.println("Calibration distance set to "+ value);
    }
    else if (inputString.indexOf("setLayerThickness") != -1) {
       String value = inputString.substring(17);
       layerThickness = value.toFloat();
       Serial.println("Layer thickness set to " + value);
    }
    else if (inputString.indexOf("setLayerTime") != -1) {
       String value = inputString.substring(12);
       layerTime = value.toFloat();
       Serial.println("Layer time set to " + value);
    }
    else if (inputString.indexOf("setBottomLayers") != -1) {
       String value = inputString.substring(15);
       bottomLayers = value.toFloat();
       Serial.println("Bottom layers set to " + value);
    }
    else if (inputString.indexOf("setBottomLayerTime") != -1) {
       String value = inputString.substring(18);
       bottomLayerTime = value.toFloat();
       Serial.println("Bottom layer time set to " + value);
    }
    else if (inputString.indexOf("setG1Distance") != -1) {
      String value = inputString.substring(13);
      g1Distance = value.toFloat();
      Serial.println("G1 Distance set to " + value);
    }
    else if (inputString.indexOf("setG1Speed") != -1) {
       String value = inputString.substring(10);
       g1Speed = value.toFloat();
       Serial.println("G1 Speed set to " + value);
    }
    else if (inputString.indexOf("setG2Distance") != -1) {
       String value = inputString.substring(13);
       g2Distance = value.toFloat();
       Serial.println("G1 Distance set to " + value);
    }
    else if (inputString.indexOf("setG2Speed") != -1) {
       String value = inputString.substring(10);
       g2Speed = value.toFloat();
       Serial.println("G1 Distance set to " + value);
    }
    else if (inputString.indexOf("setInterim") != -1) {
       String value = inputString.substring(10);
       interim = value.toFloat();
       Serial.println("Interim set to " + value);
    }
    else if (inputString.indexOf("totalLayers") != -1) {
       String value = inputString.substring(12);
       Serial.println("value " + value);
       totalLayers = value.toInt();
       Serial.println("Total layers set to " + String(value.toInt()));
    }
    if (inputString.length() > 5) {
      if (inputString.substring(0,4).equals("step")) {
        String value = inputString.substring(4);
        int stepsToTake = value.toInt();
        step(stepsToTake);
      }
    }
    inputString = "";
    stringComplete = false;
  }
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    if (inChar == '\n') {
      stringComplete = true;
    }
    else {
      inputString += inChar;
    }
  }
}

void setMotorDirectionUp() {
  digitalWrite(8, HIGH);
}

void setMotorDirectionDown() {
  digitalWrite(8, LOW);
}

void stopMotors() {
  digitalWrite(9, LOW);
  delay(500);
}

void startPrinting() {
  currentLayer = 0;
  for (int i = 0; i < bottomLayers; i++) {
    delay(bottomLayerTime);
    currentLayer++;
    Serial.println("Current Layer: " + String(currentLayer));
    setMotorDirectionDown();
    moveDistanceAtSpeed(g1Distance, g2Speed);
    setMotorDirectionUp();
    moveDistanceAtSpeed(g2Distance, g2Speed);
    delay(interim);
  }
  Serial.println("finished bottom layers");
  Serial.println("total layers: " + String(totalLayers));
  for (int i = currentLayer; i < totalLayers; i++) {
    delay(layerTime);
    currentLayer++;
    Serial.println("Current Layer: " + String(currentLayer));
    setMotorDirectionDown();
    moveDistanceAtSpeed(g1Distance, g2Speed);
    
    setMotorDirectionUp();
    moveDistanceAtSpeed(g2Distance, g2Speed);
    
    delay(interim);
  }
  delay(5000);
  moveDistanceAtSpeed(30, 1.0);
}

void stopPrinting() {
  setMotorDirectionUp();
  moveDistanceAtSpeed(3.0, g2Speed);
}

void testFunction() {
  Serial.println("direction down. G1 layer & speed " + String(g1Distance) + " ... " + String(g1Speed));
  setMotorDirectionDown();
  moveDistanceAtSpeed(g1Distance, g2Speed);

Serial.println("direction up. G2 layer & speed " + String(g2Distance) + " ... " + String(g2Speed));
  
    setMotorDirectionUp();
    moveDistanceAtSpeed(g2Distance, g2Speed);

}

