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
#include <SimpleDHT.h>
int pinDHT11 = 8;
SimpleDHT11 dht11(pinDHT11);
//LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup()

{
  Serial.begin(9600);
  delay(1000);          // wait for sensor initialization
//lcd.begin(16, 2);
}

void loop()
{
  Serial.print("Sensor status: ");
  readTemp();

  delay(2500);
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
