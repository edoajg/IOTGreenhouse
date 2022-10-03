
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#define BLYNK_PRINT Serial
#define DHTPIN 4
#define DHTTYPE DHT11
int rele = 5;
int rele2 = 12;
int riegos = 0;
int llenadas = 0;
const int trigPin = 13;
const int echoPin = 15;
long duration;
int distance;
int nivel;
DHT dht(DHTPIN,DHTTYPE);
BlynkTimer timer;

// Auth Token from Blynk App.

char auth[] = "2d3562901ed142f9ac32fc75de228182";


char ssid[] = "xx";
char pass[] = "xxxxx"; 

void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  if (isnan(h) || isnan(t)) {
    Serial.println(h);
    Serial.println(t);
    return;
  }
  // ready to send any value at any time.
  // we should not send more than 10 values per second.
  Blynk.virtualWrite(V5, h);
  Blynk.virtualWrite(V6, t);

}

void control(){
int bomba =  digitalRead(rele);
int moist = analogRead(A0);
if(moist<200){
  if(bomba == HIGH){
    if(moist<180){
      digitalWrite(rele, LOW);
    }
}
}
if(moist>200){
  if(bomba == LOW){
  riegos++;
  digitalWrite(rele, HIGH);
  Blynk.virtualWrite(V0, riegos);
  Blynk.notify("Regando");
}
  if(moist>750){
    digitalWrite(rele, LOW);
    }
}
// level control
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
// Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(echoPin, HIGH);
// Calculating the distance
distance= duration*0.034/2;
nivel = distance-88;
Blynk.virtualWrite(V1, abs(nivel));
Serial.print("Distance: ");
Serial.println(distance);
Serial.println(abs(nivel));
int bomba2 =  digitalRead(rele2);
if(nivel<30){
  if(bomba2 = LOW){
  digitalWrite(rele2, HIGH); 
  llenadas++; 
  Blynk.virtualWrite(V1, llenadas);
  }
}
if(nivel>70){
  digitalWrite(rele2, LOW); 
}
}

void setup()
{
  // Debug console
  Serial.begin(9600);
  pinMode(5, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT);
  

  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);
  dht.begin();
  timer.setInterval(10000L, sendSensor);
  timer.setInterval(2000, control);
}
void loop()
{
Blynk.run();
timer.run();
}
