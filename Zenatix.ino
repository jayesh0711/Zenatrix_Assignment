#include "ThingSpeak.h"
#include "WiFi.h"
#include <Wire.h>  // Include Library for HTU21D
#include "SparkFunHTU21D.h"

// -------Enter your WiFi Details------
char ssid[] = "Jayesh";      // SSID
char pass[] = "12344567";  // Password
// ------------------------------------

#define HTU21DPIN 4             // Pin where the HTU21D is connected
#define CHILLER_PIN 5     // Pin connected to the chiller switch
#define ENTRY_PIR_PIN 6  // Pin connected to the entry PIR sensor
#define EXIT_PIR_PIN 7   // Pin connected to the exit PIR sensor
#define BUZZER_PIN 8      // Pin connected to the buzzer

float temp;
int noOfPeople = 0;
bool chillerPowerStatus = false;

WiFiClient client;

// ThingSpeak Details
unsigned long myChannelField = 2539365;  // Channel ID
const int ChannelField1 = 1;              // for Temperature
const int ChannelField2 = 2;              // for Chiller power status
const int ChannelField3 = 3;              // for Number of people
const char *myWriteAPIKey = "VLJDKRN3R4FCZ0QH"; // Your Write API Key

// Create an instance of the object
HTU21D myHumidity;

void setup() {
  Serial.begin(115200);
  delay(10);
  myHumidity.begin();
  pinMode(CHILLER_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  WiFi.mode(WIFI_STA);
  pinMode(ENTRY_PIR_PIN, INPUT_PULLUP);
  pinMode(EXIT_PIR_PIN, INPUT_PULLUP);
  ThingSpeak.begin(client);
}

void loop()
 {
  // WiFi Setup
  if (WiFi.status() != WL_CONNECTED)
   {
    Serial.print("Attempting to Connect to SSID: ");
    Serial.println(ssid);
  }
  while (WiFi.status() != WL_CONNECTED) 
  {
    WiFi.begin(ssid, pass);
    Serial.print(".");
    delay(5000);
  }
  Serial.println("\nConnected.");

  // Read temperature
  temp = myHumidity.readTemperature();

  // Update chiller status based on temperature
  if (temp > 25)
  
   {
    digitalWrite(CHILLER_PIN, HIGH);  // Turn on chiller
    chillerPowerStatus = true;
  } else if (temp < 24) 
  {
    digitalWrite(CHILLER_PIN, LOW);   // Turn off chiller
    chillerPowerStatus = false;
  }

  
  if (digitalRead(ENTRY_PIR_PIN) == HIGH) 
  
  {
   
    delay(1000); 
    if (digitalRead(ENTRY_PIR_PIN) == HIGH && noOfPeople < 100)
     {
      noOfPeople++;
      if (noOfPeople == 100) 
      
      {
        
        Serial.println("Hall is full! Entry gate closed.");
        digitalWrite(BUZZER_PIN, HIGH); 
        
      }
      delay(1000);
    }
  }


  if (digitalRead(EXIT_PIR_PIN) == HIGH && noOfPeople > 0)
  
   {
    
    delay(1000); 
    if (digitalRead(EXIT_PIR_PIN) == HIGH)
    
     {
      noOfPeople--;
      if (noOfPeople == 90)
      
       {
        
        Serial.println("Entry gate can be reopened.");
        digitalWrite(BUZZER_PIN, HIGH);
           
      }
      delay(1000);         // Wait to avoid multiple detections

    }

  }

  
  Serial.print("Time: ");
  Serial.print(millis());
  Serial.print(" Temperature: ");
  Serial.print(temp, 1);
  Serial.print("°C Chiller Power Status: ");
  Serial.print(chillerPowerStatus ? "On" : "Off");
  Serial.print(" No. of People: ");
  Serial.println(noOfPeople);

  
  ThingSpeak.writeField(myChannelField, ChannelField1, temp, myWriteAPIKey);
  ThingSpeak.writeField(myChannelField, ChannelField2, chillerPowerStatus ? 1 : 0, myWriteAPIKey);
  ThingSpeak.writeField(myChannelField, ChannelField3, noOfPeople, myWriteAPIKey);

  
  if (noOfPeople > 90 && noOfPeople < 100) {
    digitalWrite(BUZZER_PIN, LOW);
  }

  // Wait for 1 minute before sending the next data
  delay(60000); // 60,000 milliseconds = 1 minute
}



// #include "ThingSpeak.h"
// #include "WiFi.h"
// #include <Wire.h>  // Include Library for HTU21D
// #include "SparkFunHTU21D.h"

// // -------Enter your WiFi Details------
// char ssid[] = "Jayesh";      // SSID
// char pass[] = "12344567";  // Password
// // ------------------------------------

// // #define HTU21DPIN 4       // Pin where the HTU21D is connected
// // #define CHILLER_PIN 5     // Pin connected to the chiller switch
// // #define ENTRY_PIR_PIN 6  // Pin connected to the entry PIR sensor
// // #define EXIT_PIR_PIN 7   // Pin connected to the exit PIR sensor
// // #define BUZZER_PIN 8      // Pin connected to the buzzer

// float temp;
// int noOfPeople = 0;
// bool chillerPowerStatus = false;

// WiFiClient client;

// // ThingSpeak Details
// unsigned long myChannelField = 2539365;  // Channel ID
// const int ChannelField1 = 1;              // for Temperature
// const int ChannelField2 = 2;              // for Chiller power status
// const int ChannelField3 = 3;              // for Number of people
// const char *myWriteAPIKey = "G8EAS3BY0CHEOJHD"; // Your Write API Key

// // Create an instance of the object
// HTU21D myHumidity;

// void setup() {
//   Serial.begin(115200);
//   delay(10);
//   // myHumidity.begin();
//   // pinMode(CHILLER_PIN, OUTPUT);
//   // pinMode(BUZZER_PIN, OUTPUT);
//   WiFi.mode(WIFI_STA);
//   // pinMode(ENTRY_PIR_PIN, INPUT_PULLUP);
//   // pinMode(EXIT_PIR_PIN, INPUT_PULLUP);
//   ThingSpeak.begin(client);
// }

// void loop() {
//   // WiFi Setup
//   if (WiFi.status() != WL_CONNECTED) {
//     Serial.print("Attempting to Connect to SSID: ");
//     Serial.println(ssid);
//   }
//   while (WiFi.status() != WL_CONNECTED) {
//     WiFi.begin(ssid, pass);
//     Serial.print(".");
//     delay(5000);
//   }
//   Serial.println("\nConnected.");

//   // Read temperature
//   // temp = myHumidity.readTemperature();

//   // // Update chiller status based on temperature
//   // if (temp > 25) {
//   //   digitalWrite(CHILLER_PIN, HIGH);  // Turn on chiller
//   //   chillerPowerStatus = true;
//   // } else if (temp < 24) {
//   //   digitalWrite(CHILLER_PIN, LOW);   // Turn off chiller
//   //   chillerPowerStatus = false;
//   // }

//   // Hardcode values for Temperature_of_hall, Chiller_power_status, and No_of_people_in_hall
//   float Temperature_of_hall = 25.5; 
//   bool Chiller_power_status = true;  
//   int No_of_people_in_hall =  3;     

//   // Printing data to Serial Terminal
//   Serial.print("Time: ");
//   Serial.print(millis());
//   Serial.print(" Temperature: ");
//   Serial.print(Temperature_of_hall, 1);
//   Serial.print("°C Chiller Power Status: ");
//   Serial.print(Chiller_power_status ? "On" : "Off");
//   Serial.print(" No. of People: ");
//   Serial.println(No_of_people_in_hall);

//   // Sending data to ThingSpeak Cloud
//   ThingSpeak.writeField(myChannelField, ChannelField1, Temperature_of_hall, myWriteAPIKey);
//   ThingSpeak.writeField(myChannelField, ChannelField2, Chiller_power_status ? 1 : 0, myWriteAPIKey);
//   ThingSpeak.writeField(myChannelField, ChannelField3, No_of_people_in_hall, myWriteAPIKey);

//   // Wait for 1 minute before sending the next data
//   delay(600); // 60,000 milliseconds = 1 minute
// }


