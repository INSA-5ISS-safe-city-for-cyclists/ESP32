#include <Arduino.h>
#include <TFLI2C.h> // TFLuna-I2C Library v.0.2.0
#include <Wire.h>

TFLI2C tflI2C;

// Use these defaults or insert your own values
int16_t tfAddr_def = 0x10; // default I2C address
int16_t tfAddr_lidar1 = 0x11;
int16_t tfAddr_lidar2 = 0x12;
uint16_t tfFrame = TFL_DEF_FPS; // default frame rate

// device variables passed back by getData
int16_t tfDist1 = 0;
int16_t tfDist2 = 0; // distance in centimeters

uint16_t dist_max = 1500;
uint8_t *p_tfl = (uint8_t *)&dist_max;

//  This is a group of various sample
//  commands that can be called at setup.
void sampleCommands(uint8_t adr, uint8_t new_adr)
{
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
		tflI2C.printStatus(); // `printStatus()` is for troubleshooting,
							  //  It's not necessary for operation.
	delay(500);

	Serial.print("NEW_ADR: ");
	tflI2C.readReg(0x22, new_adr);

	Serial.print("Set Frame Rate to: ");
	if (tflI2C.Set_Frame_Rate(tfFrame, new_adr))
	{
		Serial.println(tfFrame);
	}
	else
		tflI2C.printStatus();
	delay(500);

	// if (!tflI2C.writeReg(0x30, new_adr, p_tfl[0]))
	// 	exit(EXIT_FAILURE);
	// if (!tflI2C.writeReg(0x31, new_adr, p_tfl[1]))
	// 	exit(EXIT_FAILURE);
}

void setup()
{
	Serial.begin(115200); // Initialize Serial port
	Wire.begin();		  // Initialize Wire library

	Serial.println("TFLI2C example code"); // say "Hello!"
	// Execute a group of commands.
	// Comment this out if not needed.
	sampleCommands(tfAddr_def, tfAddr_lidar1);
	sampleCommands(tfAddr_def, tfAddr_lidar2);
}

void test_adress()
{
	byte error, address;
	int nDevices;
	Serial.println("Scanning...");
	nDevices = 0;
	for (address = 1; address < 127; address++)
	{
		Wire.beginTransmission(address);
		error = Wire.endTransmission();
		if (error == 0)
		{
			Serial.print("I2C device found at address 0x");
			if (address < 16)
			{
				Serial.print("0");
			}
			Serial.println(address, HEX);
			nDevices++;
		}
		else if (error == 4)
		{
			Serial.print("Unknow error at address 0x");
			if (address < 16)
			{
				Serial.print("0");
			}
			Serial.println(address, HEX);
		}
	}
	if (nDevices == 0)
	{
		Serial.println("No I2C devices found\n");
	}
	else
	{
		Serial.println("done\n");
	}
	delay(5000);
}

void loop()
{
	test_adress();
	// If data is read without error...
	// if (tflI2C.getData(tfDist1, tfAddr_lidar1))
	// {
	// 	Serial.println("Dist: "); // ...print distance,
	// 	Serial.println(tfDist1);
	// }
	// else
	// 	tflI2C.printStatus(); // else, print error status.

	// if (tflI2C.getData(tfDist2, tfAddr_lidar2))
	// {
	// 	Serial.println("Dist: "); // ...print distance,
	// 	Serial.println(tfDist2);
	// }
	// else
	// 	tflI2C.printStatus();
	// // else, print error status.
}