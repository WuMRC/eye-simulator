int redLED = 2; // LED pin for pressure warning
int greenLED = 6; // LED pin for start use
int yellowLED = 4; // LED pin for initialization
int buzzerPin = 9; // Buzzer pin
int pressurePin = A0; // Pin to take pressure reading
int pressureValue; // Pressure value in analog units
float pressureInmmHg; // Pressure value in mmHg
float desiredVoltage = (((10.0*4.0/500.0/0.7500061683)+2.5)*1024/5); // Desired analog read value (assuming 17mmHg)
float basePressure; // Pressure that will be used as base (intraocular pressure)
const float zeroPressure = 2.5*1024/5;
// long delayValue = 1000000/2500/2; // Delay for buzzer

// Required variables for running average
const int numReadings = 10;
float total = 0;
float average;

// Test variables
int averageCounter = 0;
int canBuzz = 0;
boolean turnOnLED = false;

void setup(){
 
 pinMode(yellowLED, OUTPUT); 
  
 pinMode(redLED, OUTPUT);
 
 pinMode(greenLED, OUTPUT);
 
 pinMode(buzzerPin, OUTPUT);
 
 Serial.begin(9600); 
}

void loop(){
 
  if (canBuzz == 1){
     Serial.print("mmHg: ");
     Serial.print(pressureInmmHg);
     Serial.print("\tAnalog Value: ");
     Serial.print(pressureValue);
     Serial.print("\tAverage: ");
     Serial.print(average);
     Serial.print("\tBase Pressure: ");
     Serial.print(basePressure);
     Serial.print("\tDes: ");
     Serial.print(desiredVoltage);
     Serial.print("\r\n");
     Serial.print("Warning Signal\r\n");
     
     digitalWrite(buzzerPin,HIGH); // The buzzer of this can emit sound with basic high voltage.
     delay(4000);
     digitalWrite(buzzerPin,LOW);
     digitalWrite(redLED,LOW);
     
     // Reset
     averageCounter = 0;
     basePressure = 0;
     canBuzz=0;
     total=0;     
   }  
  else {
     pressureValue = analogRead(pressurePin);
     pressureInmmHg = (((float)pressureValue * 5 / 1024) - 2.5) * 500 / 4 * 0.750061683; 
 
     // Turn on LED if at desiredVoltage -1, (range room of 4.58mmHg)
     if (pressureValue > desiredVoltage -1 && pressureValue < desiredVoltage + 9)
     {
         // Set variable to allow LED lighting
         if( averageCounter == 0){
           averageCounter = 1;
         }
     
         // Turn on yellow LED when user has reached desiredVoltage
         if (averageCounter >= 0 && averageCounter <= 10){
           turnOnLED = true;
         }
         else {
           turnOnLED = false;
         }
     
     } 
     else {
       turnOnLED = false;
     }
 
     if (turnOnLED == true)
     {
      digitalWrite(yellowLED, HIGH); 
     }
     else {
      digitalWrite(yellowLED, LOW);
     }

    if (averageCounter >= 1 && averageCounter <= 10)
    {
      total += pressureValue;
      average = (float)total / (float)numReadings; 
 
      if (averageCounter == 10){
         basePressure = average;
      }
      
      averageCounter += 1;
    }

    if (averageCounter >= 10) {
      digitalWrite(greenLED, HIGH);
    }
    else{
      digitalWrite(greenLED, LOW);
    }

     Serial.print("mmHg: ");
     Serial.print(pressureInmmHg);
     Serial.print("\tAnalog Value: ");
     Serial.print(pressureValue);
     Serial.print("\tAverage: ");
     Serial.print(average);
     Serial.print("\tBase Pressure: ");
     Serial.print(basePressure);
     Serial.print("\tDes: ");
     Serial.print(desiredVoltage);
     Serial.print("\r\n");
 
     // If pressure gets too low
     if( (pressureValue < basePressure - 17) || (pressureValue < zeroPressure-5) || ((pressureValue < zeroPressure+5) && (averageCounter >= 10)) )
     //if(pressureValue < zeroPressure)
     {
       digitalWrite(redLED, HIGH);
      digitalWrite(greenLED, LOW);
      canBuzz = 1;
     }
 
     else {
      digitalWrite(redLED, LOW);
      digitalWrite(buzzerPin, LOW); 
     }
  } 
}

