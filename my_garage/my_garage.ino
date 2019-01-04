#include <DHT.h>
#include <DHT_U.h>

/*####################################################################
 BOARD -> ARDUINO NANO
 sensor www.microbot.it temp+humidity 
 * +5V   -> 5V
 * GND   -> GND
 * DATA  -> digital pin 8

#######################################################################*/

//switch on vent on 
int humidity_on = 80;
//switch off vent on 
int humidity_off = 40;
//switch on hotter on 
int temper_on = 4;
//switch off hotter on 
int temper_off = 8;
unsigned int gettemp=0;

#include <SimpleDHT.h>
int pinDHT11 = 8;
SimpleDHT11 dht11(pinDHT11);

#define RELAY_HOT 2
#define RELAY_VENT 3
int states [RELAY_VENT+1] = {HIGH, HIGH,HIGH, HIGH};

void setup()
{
  pinMode(RELAY_VENT, OUTPUT); 
  pinMode(RELAY_HOT, OUTPUT);
    relayOn(RELAY_VENT);
    relayOn(RELAY_HOT);
  
  Serial.begin(9600);
  delay(1000);          // wait for sensor initialization
    while(!Serial){
      ; // wait for serial port to connect. Needed for native USB port only
    }

  Serial.println("\n[memCheck]");
  Serial.println(freeRam());

    relayOff(RELAY_VENT);
    relayOff(RELAY_HOT);
}

void loop()
{
   // count 60 seconds
   if(millis()/60000!=gettemp) {  
    gettemp=millis()/60000;
    //log(readDHT());
    if(gettemp %2 == 0) {
      checkHumidity();
    }
    else {
      checkTemper();
    }
   }
}

void readTemp(){
 Serial.println("=================================");
  Serial.println("Sample DHT11...");
  
  // read without samples.
  byte temperature = 0;
  byte humidity = 0;
  int err = SimpleDHTErrSuccess;
  if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Read DHT11 failed, err="); Serial.println(err);delay(1000);
    return;
  }
  
  Serial.print("Sample OK: ");
  Serial.print((int)temperature); Serial.print(" *C, "); 
  Serial.print((int)humidity); Serial.println(" H");
  
  // DHT11 sampling rate is 1HZ.
}
void relayOn(int relay){
  if(states[relay] != LOW){
    digitalWrite(relay, LOW);  //switch ON
    states[relay] = LOW;
    String msg = String("relay switch on ");
    msg += relay;
    log(msg);
  }
}

void relayOff(int relay){
  if(states[relay] != HIGH){
    digitalWrite(relay, HIGH);  // switch OFF
    states[relay] = HIGH;
    String msg = String("relay switch off ");
    msg += relay;
    log(msg);
  }
}

void log(String msg){
       //File myFile = SD.open("temper.log", FILE_WRITE);
       String record="";//get_time();
       record+=(" " + msg);
       Serial.println(record);
       //myFile.println(record);
       //myFile.close();
}

void checkHumidity(){
  int currentValue = getHumidity();
  if(currentValue >= humidity_on) {
    relayOn(RELAY_VENT);
  }
  if(currentValue <= humidity_off){
    relayOff(RELAY_VENT);
  }
}

void checkTemper(){
  int currentValue = getTemper();
  if(currentValue <= temper_on) {
    relayOn(RELAY_HOT);
  }
  if(currentValue >= temper_off){
    relayOff(RELAY_HOT);
  }
}

 
int getHumidity(){
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  // read without samples.
  byte temperature = 0;
  byte humidity = 0;
  int err = SimpleDHTErrSuccess;
  if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Read DHT11 failed, err="); Serial.println(err);delay(1000);
    return 0;
  }
  String msg = String("humidity is ");
  //************** DEBUG ONLY
  humidity = random(40)+60;
  msg += humidity;
  log(msg);
  return humidity;
}

int getTemper(){
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  // read without samples.
  byte temperature = 0;
  byte humidity = 0;
  int err = SimpleDHTErrSuccess;
  if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Read DHT11 failed, err="); Serial.println(err);delay(1000);
    return 100;
  }
  String msg = String("temperature is ");
  //************** DEBUG ONLY
  temperature = random(10);
  msg += temperature;
  log(msg);
  return temperature;
}
int freeRam () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}
