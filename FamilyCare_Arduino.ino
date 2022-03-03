
#include "FirebaseESP8266.h"  
#include <ESP8266WiFi.h>

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;


int limitValue=5;

int x=D4;
int y=D5;
int z=D7;


#define FIREBASE_HOST "smart-family-care-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "cxKdrJTP9tveNLsdb30e1ojapQMOLQApZyP7H0tA"
#define WIFI_SSID "PAPPURAJ-M21"
#define WIFI_PASSWORD "000000001"//"P@r@cet@mol500"


FirebaseData firebaseData;
FirebaseData loadData;

FirebaseJson json;


bool upS(String _name,String _value){
  return Firebase.setString(firebaseData, "/CrippleCare/"+_name, _value);
}

bool downS(String mail){
  if (Firebase.getString(loadData, "/Users/"+mail+"/danger")){
    return loadData.stringData()=="1"?true:false;
  }else return false;
}



void setup(void) {
  Serial.begin(115200);

  pinMode(D5,OUTPUT);
  pinMode(D6,OUTPUT);
  pinMode(D7,OUTPUT);


  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);



  
  while (!Serial) {
    delay(10); // will pause Zero, Leonardo, etc until serial console opens
  }

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
      
    }
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.println("");
  delay(100);
}

void loop() {

  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  /* Print out the values */
  Serial.print(a.acceleration.x);
  Serial.print(",");
  Serial.print(a.acceleration.y);
  Serial.print(",");
  Serial.print(a.acceleration.z);
 // Serial.print(", ");
//  Serial.print(g.gyro.x);
//  Serial.print(",");
//  Serial.print(g.gyro.y);
//  Serial.print(",");
//  Serial.print(g.gyro.z);
  Serial.println("");


  int y=a.acceleration.y;
  int z=a.acceleration.z;



//  p++;
//
//  if(p>50){
//    
//      if(y>5)
//    upS("signal","1");
//  else if(y<-5)
//    upS("signal","2");
//  else
//    upS("signal","0");  
//    
//  }else if(p>100){
//      upS("tempareture",String(temp.temperature));
//      p=0;
//  }


    
  if(z>limitValue){  //Washroom
       upS("signal","1");
        digitalWrite(D5,0);
        digitalWrite(D6,0);
        digitalWrite(D7,1);
  }
      else if(z<limitValue*-1){  //Emergency
        upS("signal","2");
        digitalWrite(D5,0);
        digitalWrite(D6,1);
        digitalWrite(D7,0);
      }
  else if(y<limitValue*-1){ //Medicine
        upS("signal","3");
        digitalWrite(D5,0);
        digitalWrite(D6,1);
        digitalWrite(D7,1);
  }

  else if(y>limitValue){// Food
        upS("signal","4");
        digitalWrite(D5,1);
        digitalWrite(D6,0);
        digitalWrite(D7,0);
  }
  else if(checkDanger()){
        digitalWrite(D5,1);
        digitalWrite(D6,1);
        digitalWrite(D7,1);
  }

  else{
            digitalWrite(D5,0);
        digitalWrite(D6,0);
        digitalWrite(D7,0);
      upS("signal","0");
  }
  
  
  delay(50);

  //upS("tempareture",String(temp.temperature));
  //delay(50);

 // checkDanger();

  
}



bool checkDanger(){

  if(downS("mamuncse1999@gmail!com")){
     Serial.println("Danger");
     return true;
  }
     
  else
      Serial.println("Normal");
      return false;    
  
}
