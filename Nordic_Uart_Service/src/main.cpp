#include <Arduino.h>
#include <ArduinoBLE.h>

// Define the Nordic UART Service
BLEService Nus_service("6E400001B5A3F393E0A9E50E24DCCA9E");

BLEStringCharacteristic Nus_service_WriteChar("6E400002B5A3F393E0A9E50E24DCCA9E", BLEWrite, 20);
BLEStringCharacteristic Nus_service_ReadChar("6E400003B5A3F393E0A9E50E24DCCA9E", BLERead | BLENotify,20);

// Define the Battery Service
BLEService batteryService("180F");
BLEUnsignedCharCharacteristic batteryLevelChar("2A19", BLERead | BLENotify);

int oldBatteryLevel = 0;
long previousMillis = 0;

void updateBatteryLevel();
void Nus_read_value();

void setup() {
    Serial.begin(9600);
    while (!Serial);

    pinMode(2, OUTPUT);

    if (!BLE.begin()) {
        Serial.println("Starting BLE failed!");
        while (1);
    }

    BLE.setLocalName("Nordic Uart Service");

    // Add characteristics to the Nordic UART Service
    Nus_service.addCharacteristic(Nus_service_WriteChar);
    Nus_service.addCharacteristic(Nus_service_ReadChar);

    // Add characteristic to the Battery Service
    batteryService.addCharacteristic(batteryLevelChar);

    // Add services to the BLE peripheral
    BLE.addService(Nus_service);
    BLE.addService(batteryService);

    // Start advertising
    BLE.advertise();
    Serial.println("Bluetooth device active, waiting for connections...");
}

void loop()
{
    BLEDevice central = BLE.central();
    if (central) {
        Serial.print("Connected to central: ");
        Serial.println(central.address());
        digitalWrite(2, HIGH);

        while (central.connected()) 
        {
            long currentMillis = millis();
            if (currentMillis - previousMillis >= 5000) {
                previousMillis = currentMillis;
                updateBatteryLevel();
            }
            Nus_read_value();

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

void Nus_read_value() 
{
    if (Nus_service_WriteChar.written()) 
    {
        char receivedData[20]; 
        int bytesRead = Nus_service_WriteChar.readValue(receivedData,sizeof(receivedData));
        if (bytesRead > 0) 
        {
            receivedData[bytesRead] = '\0'; 
            Serial.print("Received: ");
            Serial.println(receivedData);
            Nus_service_ReadChar.writeValue(receivedData);
        }
    }
}
