#include <Arduino.h>
#include <ArduinoBLE.h>

/*************************************************************************/
#include <Wire.h>
#include "SPI.h"
#include <Adafruit_Sensor.h>
#include "Adafruit_BMP280.h"
/*************************************************************************/
// Define the Battery Service
BLEService batteryService("180F");
BLEUnsignedCharCharacteristic batteryLevelChar("2A19", BLERead | BLENotify);

//Environmental monitoring - EVM
BLEService EVMService("181A");
BLEUnsignedIntCharacteristic     EVMLevelChar_temperature("2A6E", BLERead|BLENotify);
BLEUnsignedIntCharacteristic     EVMLevelChar_pressure("2A6D", BLERead|BLENotify);
BLEUnsignedIntCharacteristic     EVMLevelChar_altitude("2AB3", BLERead|BLENotify);


int oldBatteryLevel = 0;
long previousMillis = 0;

void updateBatteryLevel();

/**************************************************************************/
#define BMP280_ADDRESS 0x76
void BMP_280_setup();
void BMP_280_loop();
Adafruit_BMP280 bmp; // I2C
/*************************************************************************/

void setup() 
{

        Serial.begin(9600);
        /***************************************************************/
        BMP_280_setup();
        /***************************************************************/
        
        while (!Serial);
        pinMode(2, OUTPUT);

        if (!BLE.begin()) 
        {
            Serial.println("Starting BLE failed!");
            while (1);
        }

        BLE.setLocalName("ESS MONITOR");  
        BLE.setDeviceName("ESS DEVICE");  //Environmental Sensing Service

        BLE.setAppearance(0x0300);        // Set appearance to a generic sensor

        batteryService.addCharacteristic(batteryLevelChar);               //  battery
        EVMService.addCharacteristic(EVMLevelChar_temperature);           //  ESS
        EVMService.addCharacteristic(EVMLevelChar_pressure);
        EVMService.addCharacteristic(EVMLevelChar_altitude);


        // Add services to the BLE peripheral
        BLE.addService(batteryService);
        BLE.addService(EVMService);
        // Start advertising
        BLE.advertise();
        Serial.println("Bluetooth device active, waiting for connections...");

}

void loop()
{
    BLEDevice central = BLE.central();
    if (central) 
    {
        Serial.print("Connected to central: ");
        Serial.println(central.address());
        digitalWrite(2, HIGH);

        while (central.connected()) 
        {
            long currentMillis = millis();
            if (currentMillis - previousMillis >= 5000) 
            {
                previousMillis = currentMillis;
                updateBatteryLevel();  
                BMP_280_loop();  
            }
        }
        digitalWrite(2, LOW);
        Serial.print("Disconnected from central: ");
        Serial.println(central.address());
    }

}

void updateBatteryLevel() {
    int battery = analogRead(A0);
    int batteryLevel = map(battery, 0, 1023, 0, 100);
    Serial.print("Battery Level % is now: ");
    Serial.println(batteryLevel);
    batteryLevelChar.writeValue(batteryLevel);
}


void BMP_280_setup()
{
   
    Serial.begin(9600);
    Serial.println(F("BMP280 Forced Mode Test."));
    if (!bmp.begin(BMP280_ADDRESS)) 
    {
        Serial.println(F("Could not find a valid BMP280 sensor, check wiring or "
                        "try a different address!"));
        while (1) delay(10);
    }
    /* Default settings from datasheet. */
    bmp.setSampling(Adafruit_BMP280::MODE_FORCED,     /* Operating Mode. */
                    Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                    Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                    Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                    Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
}

void  BMP_280_loop()
{
    //Read values from the sensor:

	int temperature = bmp.readTemperature();
    float pressure = bmp.readPressure();
	int altimeter = bmp.readAltitude (1050.35); 
    Serial.println(temperature);
    Serial.println(pressure);
    Serial.println(altimeter);
    EVMLevelChar_temperature.writeValue(temperature*100);
    EVMLevelChar_pressure.writeValue(pressure);
    EVMLevelChar_altitude.writeValue(altimeter*0.1);
}
