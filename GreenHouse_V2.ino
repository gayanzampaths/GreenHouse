#include <FirebaseArduino.h>
 #include <ArduinoJson.h>
#include  <ESP8266WiFi.h>

#define FIREBASE_HOST "control-system-a00e9.firebaseio.com"
#define WIFI_SSID "RS" // Change the name of your WIFI
#define WIFI_PASSWORD "1234567890" // Change the password of your WIFI

int WET= 16; // Wet Indicator at Digital pin D0
int DRY= 2;  // Dry Indicator at Digital pin D4
int sense_Pin = 0; // sensor input at Analog pin A0
int pump = 5;

int value = 0;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  
  WiFi.begin (WIFI_SSID, WIFI_PASSWORD);
   while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println ("");
  Serial.println ("WiFi Connected!");
  Firebase.begin(FIREBASE_HOST);

  pinMode(WET, OUTPUT);
  pinMode(DRY, OUTPUT);
  pinMode(pump, OUTPUT);
  delay(2000);
}

void loop() {
  // put your main code here, to run repeatedly:

  Serial.print("MOISTURE LEVEL : ");

  value= analogRead(sense_Pin);
  value= value/10;
  Serial.println(value);

  if(!digitalRead(pump)==HIGH){
    Serial.print("pump on");
  }

  if(value>30)
  {
      //digitalWrite(WET, HIGH);
      digitalWrite(pump, HIGH);
  }else if(value<30 && Firebase.getString("switch")=="true"){
     //digitalWrite(DRY,HIGH);
     digitalWrite(pump, HIGH);
  }else if(value<30){
    digitalWrite(pump, LOW);
  }
  delay(1000);
  digitalWrite(WET,LOW);
  digitalWrite(DRY, LOW);

  Firebase.setFloat ("Soil moisture", value);
  delay(200);

//  String path = "/";
//  FirebaseObject object = Firebase.get(path);

//  int x = object.getInt("switch");

  Serial.println(Firebase.getString("switch"));

  if(Firebase.getString("switch")=="true"){
    digitalWrite(pump, HIGH);
  }else{
    digitalWrite(pump, LOW);
  }

//  if(Firebase.getInt("switch")){
//    Serial.print("Pump Start.");
//  }
}
