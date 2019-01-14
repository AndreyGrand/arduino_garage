#include <DHT.h>
#include <DHT_U.h>
#include <SD.h>
//#include <Wire.h>

/*####################################################################
  BOARD -> ARDUINO NANO
  sensor www.microbot.it temp+humidity
   +5V   -> 5V
   GND   -> GND
   DATA  -> digital pin 8

  #######################################################################*/

//switch on vent on
int humidity_on = 80;
//switch off vent on
int humidity_off = 40;
//switch on hotter on
int temper_on = 4;
//switch off hotter on
int temper_off = 8;
unsigned int gettemp = 0;

#include <SimpleDHT.h>
int pinDHT11 = 8;
SimpleDHT11 dht11(pinDHT11);

#define RELAY_HOT 2
#define RELAY_VENT 3
int states [RELAY_VENT + 1] = {HIGH, HIGH, HIGH, HIGH};

#define SD_CS 10
String iniFile = "settings.ini";
String logFile;
// variables will change:
int buttonVentState = LOW;         // variable for reading the pushbutton status
int buttonHotterState = LOW;         // variable for reading the pushbutton status
#define BUTTON_VENT  4         // pin for reading the pushbutton status
#define BUTTON_HOTTER  5         // pin for reading the pushbutton status
unsigned long time_pressed_vent = 0;
unsigned long time_pressed_hotter = 0;

void setup()
{
  pinMode(RELAY_VENT, OUTPUT);
  pinMode(RELAY_HOT, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(BUTTON_VENT, INPUT_PULLUP);
  pinMode(BUTTON_HOTTER, INPUT_PULLUP);
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  delay(1000);          // wait for sensor initialization

  Serial.print("\n[memCheck]");
  Serial.println(freeRam());

  readTemp();
  pinMode(SD_CS, OUTPUT);
  if (!SD.begin(SD_CS)) {
    Serial.println("initialization SD failed!");
    return;
  }
  Serial.println("initialization SD done.");
  logFile = "data";
  logFile += getCountFiles() ;
  logFile += ".log";
  Serial.println("Current log file is " + logFile);
  readSettings();
}
#define DELAY_BEFORE_PRESS 2000 //2 sec
void loop()
{
  ventButton();
  hotterButton();
  // count 60 seconds
  if (millis() / 60000 != gettemp) {
    gettemp = millis() / 60000;
    //log(readDHT());
    if (gettemp % 2 == 0) {
      checkHumidity();
    }
    else {
      checkTemper();
    }
  }
}
void ventButton() {
  //Serial.println(digitalRead(BUTTON_VENT));
  long dl = millis() -  time_pressed_vent;
  if (dl > DELAY_BEFORE_PRESS || dl < 0) {
    if (digitalRead(BUTTON_VENT) == LOW) {
      if (buttonVentState == LOW) {
        buttonVentState = HIGH;
        time_pressed_vent = millis() ;
        ventOn();
        Serial.println("Vent clicked first time for ON");
      }
      else {
        buttonVentState = LOW;
        time_pressed_vent = millis();
        ventOff();
        Serial.println("Vent clicked second time for OFF");
      }
    }
  }
}
void hotterButton() {
  long dl = millis() -  time_pressed_hotter;
  if (dl > DELAY_BEFORE_PRESS || dl < 0) {
    if (digitalRead(BUTTON_HOTTER) == LOW) {
      if (buttonHotterState == LOW) {
        buttonHotterState = HIGH;
        time_pressed_hotter = millis() ;
        hotterOn();
        Serial.println("Hotter clicked first time for ON");
      }
      else {
        buttonHotterState = LOW;
        time_pressed_hotter = millis();
        hotterOff();
        Serial.println("Hotter clicked second time for OFF");
      }
    }
  }
}
int getCountFiles() {
  File root = SD.open("/");
  int counter = 0;
  while (true) {

    File entry =  root.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    counter += 1;
    entry.close();
  }
  Serial.print("found files:");
  Serial.println(counter);
  return counter;
}
void readSettings() {
  if (checkSettings()) {
    File confFile = SD.open(iniFile);
    Serial.println(iniFile + " : ");

    humidity_on = readInt(confFile);
    humidity_off = readInt(confFile);
    String hum = String("humidity on ");
    hum += humidity_on;
    hum += " off ";
    hum += humidity_off;
    Serial.println(hum);

    temper_on = readInt(confFile);
    temper_off = readInt(confFile);
    confFile.close();
    String hot = String("hotter on ");
    hot += temper_on;
    hot += " off ";
    hot += temper_off;
    Serial.println(hot);
  }
}
int readInt(File file) {
  int k = 0;
  int num2 = 0;
  while (file.available()) {
    char s = file.read();
    if (s >= 48 && s <= 57) {
      k++;
      num2 = num2 * 10 + s - '0';
    }
    else if (k > 0) {
      break;
    }
  }
  String result = String();
  result += "-";
  result += num2;
  result += "-";
  Serial.println(result);
  return num2;
}

bool checkSettings() {
  Serial.print(iniFile);
  if (SD.exists(iniFile))
  {
    Serial.println(" file exists.");
    return true;
  }
  Serial.println(" file not exists.");
  return false;
}
void readTemp() {
  Serial.println("=================================");
  Serial.println("Sample DHT11...");

  // read without samples.
  byte temperature = 0;
  byte humidity = 0;
  int err = SimpleDHTErrSuccess;
  if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Read DHT11 failed, err="); Serial.println(err); delay(1000);
    return;
  }

  Serial.print("Sample OK: ");
  Serial.print((int)temperature); Serial.print(" *C, ");
  Serial.print((int)humidity); Serial.println(" H");

  // DHT11 sampling rate is 1HZ.
}
void ventOn() {

  if (states[RELAY_VENT] == HIGH || buttonVentState == HIGH) {
    digitalWrite(RELAY_VENT, LOW);  //switch ON
    states[RELAY_VENT] = LOW;
    String msg = String("relay switch on ");
    msg += RELAY_VENT;
    log(msg);
  }
}
void ventOff() {
  if (states[RELAY_VENT] == LOW && buttonVentState == LOW) {
    digitalWrite(RELAY_VENT, HIGH);  // switch OFF
    states[RELAY_VENT] = HIGH;
    String msg = String("relay switch off ");
    msg += RELAY_VENT;
    log(msg);
  }
}

void hotterOn() {

  if (states[RELAY_HOT] == HIGH || buttonVentState == HIGH) {
    digitalWrite(RELAY_HOT, LOW);  //switch ON
    states[RELAY_HOT] = LOW;
    String msg = String("relay switch on ");
    msg += RELAY_HOT;
    log(msg);
  }
}
void hotterOff() {
  if (states[RELAY_HOT] == LOW && buttonVentState == LOW) {
    digitalWrite(RELAY_HOT, HIGH);  // switch OFF
    states[RELAY_HOT] = HIGH;
    String msg = String("relay switch off ");
    msg += RELAY_HOT;
    log(msg);
  }
}

void log(String msg) {
  File myFile = SD.open(logFile, FILE_WRITE);
  String record = ""; //get_time();
  record += (" " + msg);
  Serial.println(record);
  myFile.println(record);
  myFile.close();
}

void checkHumidity() {
  int currentValue = getHumidity();
  if (currentValue >= humidity_on) {
    ventOn();
  }
  if (currentValue <= humidity_off) {
    ventOff();
  }
}

void checkTemper() {
  int currentValue = getTemper();
  if (currentValue <= temper_on) {
    hotterOn();
  }
  if (currentValue >= temper_off) {
    hotterOff();
  }
}


int getHumidity() {
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  // read without samples.
  byte temperature = 0;
  byte humidity = 0;
  int err = SimpleDHTErrSuccess;
  if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Read DHT11 failed, err="); Serial.println(err); delay(1000);
    return 0;
  }
  String msg = String("humidity is ");
  //************** DEBUG ONLY
  //humidity = random(40) + 60;
  msg += humidity;
  log(msg);
  return humidity;
}

int getTemper() {
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  // read without samples.
  byte temperature = 0;
  byte humidity = 0;
  int err = SimpleDHTErrSuccess;
  if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Read DHT11 failed, err="); Serial.println(err); delay(1000);
    return 100;
  }
  String msg = String("temperature is ");
  //************** DEBUG ONLY
  //temperature = random(10);
  msg += temperature;
  log(msg);
  return temperature;
}
int freeRam () {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}
