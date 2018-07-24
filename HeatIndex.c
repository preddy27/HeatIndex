// Include the library:
#include "SparkFun_RHT03.h"


/////////////////////
// Pin Definitions //
/////////////////////
const int RHT03_DATA_PIN = 10; // RHT03 data pin

const int led = 7; //buzzer to arduino pin 2
const int buzzer = 9; //buzzer to arduino pin 9
const int motorPin = 8;//vibration motor to arduino pin 3
const int sensorPin = A0; //force sensor to arduino pin 3



                ///////////////////////////
                // RHT03 Object Creation //
                ///////////////////////////
RHT03 rht; // This creates a RTH03 object, which we'll use to interact with the sensor

void setup()
{
  Serial.begin(9600); // Serial is used to print sensor readings.

            // Call rht.begin() to initialize the sensor and our data pin
  rht.begin(RHT03_DATA_PIN);

  pinMode(led, OUTPUT); //initializes led
  pinMode(motorPin, OUTPUT); //initialized vibration motor
  pinMode(buzzer, OUTPUT); //initalizes buzzer
}

float tempHI;

float heatIndex (float latestHumidity, float latestTempF)
{

      if (latestHumidity < 13 && latestTempF > 80 && latestTempF < 120)
      {
        tempHI = -42.379 + 2.04901523*latestTempF + 10.14333127*latestHumidity - .22475541*latestTempF*latestHumidity - .00683783*latestTempF*latestTempF - .05481717*latestHumidity*latestHumidity
        + .00122874*latestTempF*latestTempF*latestHumidity + .00085282*latestTempF*latestHumidity*latestHumidity - .00000199*latestTempF*latestTempF*latestHumidity*latestHumidity -
        (((13.0 - latestHumidity) / 4.0) * sqrt((17.0 - sqrt((latestTempF - 95.0)*(latestTempF - 95.0))) / 17.0));
          if (tempHI < 80)
          {
            tempHI = 0.5 * (latestTempF + 61.0 + ((latestTempF-68.0)*1.2) + (latestHumidity*0.094));
            return tempHI;
          }
          else
          {
          return tempHI;
          }
      }
      else if (latestHumidity > 85 && latestTempF > 80 && latestTempF < 87)
      {
        tempHI = tempHI = -42.379 + 2.04901523*latestTempF + 10.14333127*latestHumidity - .22475541*latestTempF*latestHumidity - .00683783*latestTempF*latestTempF - .05481717*latestHumidity*
        latestHumidity + .00122874*latestTempF*latestTempF*latestHumidity + .00085282*latestTempF*latestHumidity*latestHumidity - .00000199*latestTempF*latestTempF*latestHumidity*latestHumidity
        + ((latestHumidity-85)/10) * ((87-latestTempF)/5);
          if (tempHI < 80)
          {
            tempHI = 0.5 * (latestTempF + 61.0 + ((latestTempF-68.0)*1.2) + (latestHumidity*0.094));
            return tempHI;
          }
          else
          {
          return tempHI;
          }
      }
      else
      {
        tempHI = -42.379 + 2.04901523*latestTempF + 10.14333127*latestHumidity - .22475541*latestTempF*latestHumidity - .00683783*latestTempF*latestTempF - .05481717*latestHumidity*latestHumidity
        + .00122874*latestTempF*latestTempF*latestHumidity + .00085282*latestTempF*latestHumidity*latestHumidity - .00000199*latestTempF*latestTempF*latestHumidity*latestHumidity;
      }
}

float heatStroke (float tempHI)
{
    if (tempHI > 90.0){
      delay(120000);
      if (tempHI > 90.0){
        digitalWrite(led, HIGH);
        tone(buzzer, 1000);
        digitalWrite(motorPin, HIGH);
      }
      else{
        digitalWrite(led, LOW);
        noTone(buzzer);
        digitalWrite(motorPin, LOW);
      }
    }
    if (tempHI > 80.0){
      delay(120000);
      if (tempHI > 80.0){
        digitalWrite(led, HIGH);
        noTone(buzzer);
        digitalWrite(motorPin, HIGH);
      }
      else{
        digitalWrite(led, LOW);
        noTone(buzzer);
        digitalWrite(motorPin, LOW);
      }
    }
    else{
      digitalWrite(led, LOW);
      noTone(buzzer);
      }
}

void loop()
{
  // Call rht.update() to get new humidity and temperature values from the sensor.
  int updateRet = rht.update();
  float latestHumidity = rht.humidity();
  float latestTempC = rht.tempC();
  float latestTempF = rht.tempF();

  // If successful, the update() function will return 1.
  // If update fails, it will return a value <0
  if (updateRet == 1)
  {
    // The humidity(), tempC(), and tempF() functions can be called -- after
    // a successful update() -- to get the last humidity and temperature
    // value


    // Now print the values:
    Serial.println("Humidity: " + String(latestHumidity, 1) + " %");
    Serial.println("Temp (F): " + String(latestTempF, 1) + " deg F");
    Serial.println("Temp (C): " + String(latestTempC, 1) + " deg C");

  }
  else
  {
    // If the update failed, try delaying for RHT_READ_INTERVAL_MS ms before
    // trying again.
    delay(RHT_READ_INTERVAL_MS);
  }

  heatIndex (latestHumidity, latestTempF);
  heatStroke (tempHI);


  delay(1000);
}
