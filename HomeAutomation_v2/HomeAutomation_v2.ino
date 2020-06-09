#include <FirebaseArduino.h>                                                //Libraries needed
#include <ESP8266WiFi.h>
#include <DHT.h>
#define FIREBASE_HOST "homeautomation-a5960.firebaseio.com"                 // the project name address from firebase id
#define FIREBASE_AUTH "pzw6wguz1Ulrjhik7yYCKEBXo4aHmkVd6GNV97KE"            // the secret key generated from firebase  
#define WIFI_SSID "Redmi"                                                   // input your home or public wifi name 
#define WIFI_PASSWORD "87654321"                                            //password of wifi ssid
#define DHTPIN D4                                                           // what digital pin we're connected to
#define DHTTYPE DHT11                                                       // select dht type as DHT 11 or DHT22
DHT dht(DHTPIN, DHTTYPE);                             
#define lm35_pin A0                                                         // LM35 O/P analog pin    
#define trigP D6                                                            //Trigger Pin and Echo Pin of Ultrasonic Sensor 
#define echoP D7                                                            //D6 and D7 respectively
long duration;                                                              //To measure distance and duration between  
int distance;                                                               //obstacle and ultrasonic
void setup() 
{
  Serial.begin(9600);
  delay(1000);                
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                     //try to connect with wifi
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());                                            //print local IP address
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);                              // connect to firebase
  dht.begin();                                                               //Start reading dht sensor
  pinMode(trigP, OUTPUT);                                                    // Sets the trigPin as an Output
  pinMode(echoP, INPUT);                                                     // Sets the echoPin as an Input
}

void loop() 
{
  //Code for AM2302 or DHT22 or DHT11
  float h = dht.readHumidity();                                              // Reading humidity  
  if (isnan(h)) 
  {                                                                          // Check if any reads failed and exit early 
                                                                             //(to try again).
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  Serial.print("Humidity: ");  Serial.print(h);
  String fireHumid = String(h) + String("%");                                //convert integer humidity to string humidity
  //Firebase.pushString("/DHT11/Humidity", fireHumid);                         //setup path and send readings 
  delay(1000);
  
  //Code for LM35
  float vref = 3.3;
  float resolution = vref/1023;
  float temperature = analogRead(A0);                                       //Calculating the temperature in Celsius
  temperature = (temperature*resolution);
  temperature = temperature*100;
  Serial.println(temperature);
  Serial.print("Temperature: ");  Serial.print(temperature);  Serial.println("°C ");
  String fireTemp = String(temperature) + String("°C");                    //convert integer temperature to string temperature
  //Firebase.pushString("/DHT11/Temperature", fireTemp);                                //setup path and send readings

  //Code for UltraSonic Sensor
  digitalWrite(trigP, LOW);   // Makes trigPin low
  delayMicroseconds(2);       // 2 micro second delay 

  digitalWrite(trigP, HIGH);  // tigPin high
  delayMicroseconds(10);      // trigPin high for 10 micro seconds
  digitalWrite(trigP, LOW);   // trigPin low

  duration = pulseIn(echoP, HIGH);   //Read echo pin, time in microseconds
  distance= duration*0.034/2;        //Calculating actual/real distance

  Serial.print("Distance = ");        //Output distance on arduino serial monitor 
  Serial.println(distance);
  delay(10000);
}
