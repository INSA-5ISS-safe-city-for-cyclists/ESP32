# ESP32 + LIDAR TF-LUNA
## Safe city for cyclists - Embedded Part
In this part, we will show you how to initialize the different part of the embedded system with the Lidar and the ESP32.

## Installation - Hardware
<br />
<div align="center">
  <img src="https://firebasestorage.googleapis.com/v0/b/trade-dz.appspot.com/o/000000%2FESP32-%2B-LIDAR.jpg?alt=media&token=a42e5dc5-e47c-490e-9809-e4ed81abea82" alt="Logo" width="720">
</div>




## Installation - Software 

The project is built with PlatformIo Framework you must upload the code with it. moreover yu have to link these libriries on `platform.ini`
```ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
monitor_speed = 112500
lib_deps = 
	https://github.com/nkolban/ESP32_BLE_Arduino.git
	https://github.com/budryerson/TFLuna-I2C.git
	bblanchon/ArduinoJson@^6.18.5
```


At the beginning both Lidar have a default address 0x10. If we connect them, only one will be detected because they have the same addresses. So we have to configure them one by one before connecting them at the same time.
To do this, we must call this function in the setup by putting an address.
```cpp
void set_new_adress(uint8_t adr, uint8_t new_adr)
{
	Serial.print("System Reset: ");
	if (tflI2C.Soft_Reset(adr))
	{
		Serial.println("Passed");
	}
	else
		tflI2C.printStatus();
	delay(500);

	Serial.print("Set I2C Address to: ");
	if (tflI2C.Set_I2C_Addr(new_adr, adr))
	{
		Serial.println(new_adr);
	}
	else
		tflI2C.printStatus();
	delay(500);

	Serial.print("System Reset: ");
	if (tflI2C.Soft_Reset(adr))
	{
		Serial.println("Passed");
	}
	else
		tflI2C.printStatus();
	delay(500);

	Serial.print("NEW_ADR: ");
	tflI2C.readReg(0x22, new_adr);

```

You need to call this in the setup, and for the first Lidar you can put 0x11.

```cpp
void setup()
{
	Serial.begin(115200); // Initialize Serial port
	Wire.begin();		  // Initialize Wire library

	my_ble.init_BLE(); // Initialize BLE
	my_ble.start_BLE();
	set_new_adress(0x10,0x11);
}
```
For the second you can put 0x12 for example. 

```cpp
void setup()
{
	Serial.begin(115200); // Initialize Serial port
	Wire.begin();		  // Initialize Wire library

	my_ble.init_BLE(); // Initialize BLE
	my_ble.start_BLE();
	set_new_adress(0x10,0x12);
}
```


When the configuration of the two Lidar have been done, you can launch the following function in the setup to test if the two addresses have been well recognized, i.e. 0x11 and 0x12 for our example.

By default, the Lidar measures up to 8 meters. To prevent them from capturing too much noise, you can set the maximum distance measured by the sensors with this code.

```cpp

void set_max_distance_adress(uint8_t adr, uint16_t dist_max)
{
	uint8_t *p_tfl = (uint8_t *)&dist_max;
	if (!tflI2C.writeReg(0x30, adr, p_tfl[0]))
		exit(EXIT_FAILURE);
	if (!tflI2C.writeReg(0x31, adr, p_tfl[1]))
		exit(EXIT_FAILURE);

	tflI2C.Save_Settings(adr);
	delay(500);
	tflI2C.Soft_Reset(adr);
	delay(500);
}

```
For example, you cant limit the maximum distance to 3m for the Lidar addressed at 0x11

```cpp
void setup()
{
	Serial.begin(115200); // Initialize Serial port
	Wire.begin();		  // Initialize Wire library

	my_ble.init_BLE(); // Initialize BLE
	my_ble.start_BLE();
	set_max_distance_adress(0x11,3000); 
}

```





