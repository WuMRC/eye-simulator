const int redLED = 2; // LED pin for pressure warning
const int greenLED = 6; // LED pin for start use
const int yellowLED = 4; // LED pin for initialization
const int buzzerPin = 9; // Buzzer pin
const int pressurePin = A0; // Pin to take pressure reading
int pressureValue = 0; // Pressure value in analog units
float pressureInmmHg; // Pressure value in mmHg
const float desiredPressure = 50.0; // Threshold for initialzation - 18mmHg
float desiredVoltage = mmHgToValue(desiredPressure); // Desired analog read value 
// 0.45 mmHg per 1 in ADC
float basePressure; // Pressure that will be used as base (somewhat like intraocular pressure)
const float zeroPressure = 2.5*1024/5;
// long delayValue = 1000000/2500/2; // Delay for buzzer

// Required variables for running average
const int numReadings = 20;
float total = 0;
float average;

// Test variables
int averageCounter = 0;
int canBuzz = 0;
unsigned long buzzTimeCount = 0;
boolean turnOnLED = false;

void setup(){
 
 pinMode(yellowLED, OUTPUT); 
  
 pinMode(redLED, OUTPUT);
 
 pinMode(greenLED, OUTPUT);
 
 pinMode(buzzerPin, OUTPUT);
 
 Serial.begin(9600); 
}

void loop(){
  unsigned long currentMillis = millis();
  static int pri1=0, pri2=0;

  pri2 = pri1;
  pri1 = pressureValue;
  pressureValue = analogRead(pressurePin);
  pressureInmmHg = (((float)pressureValue * 5 / 1024) - 2.5) * 500 / 4 * 0.750061683; 
  
  if (canBuzz == 1){
     if( currentMillis - buzzTimeCount < 2000 )
     {
       Serial.print(currentMillis / 1000.0);
       Serial.print("\t");
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
       Serial.print("\r");
       Serial.print(" - Warning Signal\r\n");
     
       //digitalWrite(buzzerPin,HIGH); // The buzzer of this can emit sound with basic high voltage.
     }
     else
     {
       digitalWrite(buzzerPin,LOW);
       digitalWrite(redLED,LOW);
       // Reset
       averageCounter = 0;
       basePressure = 0;
       canBuzz=0;
       total=0;     
     }
   }  
  else { // No Bussing - General Operation Routine!
     //pressureValue = analogRead(pressurePin);
     //pressureInmmHg = (((float)pressureValue * 5 / 1024) - 2.5) * 500 / 4 * 0.750061683; 
 
     // Turn on LED if...
     if (pressureValue > desiredVoltage -5 && pressureValue < desiredVoltage + 15)
     {
         // Set variable to allow LED lighting
         if( averageCounter == 0){
           averageCounter = 1;
         }
     
         // Turn on yellow LED when user has reached desiredVoltage
         if (averageCounter >= 0 && averageCounter <= numReadings){
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

    if (averageCounter >= 1 && averageCounter <= numReadings)
    {
      total += pressureValue;
      average = (float)total / (float)numReadings; 
 
      if (averageCounter == numReadings){
         basePressure = average;
      }
      
      averageCounter += 1;
    }

    if (averageCounter >= numReadings) {
      digitalWrite(greenLED, HIGH);
    }
    else{
      digitalWrite(greenLED, LOW);
    }

     Serial.print(currentMillis / 1000.0);
     Serial.print("\t");
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
     if( (pressureValue < basePressure - 87) || (pressureValue < zeroPressure-10) || ((pressureValue < mmHgToValue(10)) && (averageCounter >= 20)) )
     //if(pressureValue < zeroPressure)
     {
       digitalWrite(redLED, HIGH);
       digitalWrite(greenLED, LOW);
       canBuzz = 1;
      
       buzzTimeCount = millis();
       digitalWrite(buzzerPin,HIGH); // The buzzer of this can emit sound with basic high voltage.
     }
     else {
      digitalWrite(redLED, LOW);
      digitalWrite(buzzerPin, LOW); 
     }
  } 
}

int mmHgToValue(float iPres)
{
  return (((iPres*4.0/500.0/0.7500061683)+2.5)*1024/5);
}

