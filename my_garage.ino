#include <DHT.h>
#include <DHT_U.h>

/*####################################################################
 Kostas Tsirogiannis
 tsirogk@gmail.com
 
 I recombined two example sketches to make 
 Arduino Yun display temp and humidity to LCD (and optionally serial)
 Made from a combination of original dht11 library and the blog 
 http://www.geeetech.com/wiki/index.php/Electric_thermometer_by_using_DHT11_sensor_module
 
 BOARD -> ARDUINO YUN
 sensor www.microbot.it temp+humidity 
 * +5V   -> 5V
 * GND   -> GND
 * DATA  -> digital pin 8

 HISTORY:
 Mirko Prosseda - Original version (21/07/2013)
#######################################################################*/


//#include <LiquidCrystal.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#define DHTTYPE           DHT11 
#define DHTPIN 8 
//LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
DHT_Unified dht(DHTPIN, DHTTYPE);
void setup()

{
  Serial.begin(9600);
  Serial.println("DHT11 Humidity and Temperature Digital Sensor test sketch");
  printSensorInfo();
  delay(1000);          // wait for sensor initialization
//lcd.begin(16, 2);
}

void loop()
{
Serial.print("Sensor status: ");
readTemp();

  delay(2500);
}
void printSensorInfo(){
   // Print temperature sensor details.
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.println("Temperature");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" *C");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" *C");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" *C");  
  Serial.println("------------------------------------");
  // Print humidity sensor details.
  dht.humidity().getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.println("Humidity");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println("%");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println("%");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println("%");  
  Serial.println("------------------------------------");
}
void readTemp(){
 // Get temperature event and print its value.
  sensors_event_t event;  
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println("Error reading temperature!");
  }
  else {
    Serial.print("Temperature: ");
    Serial.print(event.temperature);
    Serial.println(" *C");
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println("Error reading humidity!");
  }
  else {
    Serial.print("Humidity: ");
    Serial.print(event.relative_humidity);
    Serial.println("%");
  }
}
