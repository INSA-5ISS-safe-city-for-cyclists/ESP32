#include <Arduino.h>
#include <TFLI2C.h>  // TFLuna-I2C Library v.0.2.0
#include <Wire.h>

TFLI2C tflI2C;

// Use these defaults or insert your own values
int16_t  tfAddr = TFL_DEF_ADR;    // default I2C address
uint16_t tfFrame = TFL_DEF_FPS;   // default frame rate

// device variables passed back by getData
int16_t  tfDist = 0 ;   // distance in centimeters
int16_t  tfFlux = 0 ;   // signal quality in arbitrary units
int16_t  tfTemp = 0 ;   // temperature in 0.01 degree Celsius

// other device variables
uint16_t tfTime = 0;    // device clock in milliseconds
uint8_t  tfVer[3];      // device version number
uint8_t  tfCode[14];    // device serial number

// sub-loop counter for Time display
uint8_t tfCount = 0;

//  This is a group of various sample
//  commands that can be called at setup.
void sampleCommands( uint8_t adr)
{
    Serial.print( "Device Address: ");
    Serial.println( adr);

    Serial.print("System Reset: ");
    if( tflI2C.Soft_Reset( adr))
    {
        Serial.println( "Passed");
    }
    else tflI2C.printStatus();  // `printStatus()` is for troubleshooting,
                                //  It's not necessary for operation.
    delay(500);

    Serial.print( "Get Firmware Version: ");
    if( tflI2C.Get_Firmware_Version( tfVer, adr))
    {
      Serial.print( tfVer[2]);
      Serial.print( ".");
      Serial.print( tfVer[1]);
      Serial.print( ".");
      Serial.println( tfVer[0]);
    }
    else tflI2C.printStatus();    
    delay(500);

    Serial.print( "Get Serial Number: ");
    if( tflI2C.Get_Prod_Code( tfCode, adr))
    {
      for( uint8_t i = 0; i < 14; ++i)
      {
        Serial.print( char( tfCode[i]));
      }
      Serial.println();
    }
    else tflI2C.printStatus();
    delay(500);

    // In main 'loop', command to print
    // device time in milliseconds is
    // called every 10 loops.
    Serial.print( "Get Time: ");
    if( tflI2C.Get_Time( tfTime, adr))
    {
      Serial.println(  tfTime);
    }
    else tflI2C.printStatus();
    delay(500);

    Serial.print( "Set Frame Rate to: ");
    if( tflI2C.Set_Frame_Rate( tfFrame, adr))
    {
      Serial.println(  tfFrame);
    }
    else tflI2C.printStatus();
    delay(500);
    
    //  Read frame rate back from the device
    Serial.print( "Get Frame Rate: ");
    if( tflI2C.Get_Frame_Rate( tfFrame, adr))
    {
      Serial.println(  tfFrame);
    }
    else tflI2C.printStatus();
    delay(500);

}

void setup()
{
    Serial.begin( 115200);  // Initialize Serial port
    Wire.begin();           // Initialize Wire library

    Serial.println( "TFLI2C example code"); // say "Hello!"
    Serial.println( "4 NOV 2021");          // and add date

    // Execute a group of commands.
    // Comment this out if not needed.
    sampleCommands( tfAddr);

	uint16_t dist_max = 1500;
	uint8_t * p_tfl = (uint8_t *) &dist_max ;
	if(!tflI2C.writeReg(0x30, TFL_DEF_ADR, p_tfl[0])) exit(EXIT_FAILURE) ;
	if(!tflI2C.writeReg(0x31, TFL_DEF_ADR, p_tfl[1])) exit(EXIT_FAILURE) ;
}

void loop()
{
    // If data is read without error...
    if( tflI2C.getData( tfDist, tfFlux, tfTemp, tfAddr))
    {
        Serial.print("Dist: ");      // ...print distance,
        Serial.print(tfDist);
        Serial.print(" | Flux: ");   // ...print quality
        Serial.print(tfFlux);

        // Convert temperature from hundredths
        // of a degree to a whole number and...
        tfTemp = int16_t( tfTemp / 100);

        Serial.print(" | Temp: ");     // ...print temperature.
        Serial.println( tfTemp);
    }
    else tflI2C.printStatus();        // else, print error status.
	// Serial.print("MAX_Dist_LOW: ");
	// tflI2C.readReg(0x30, tfAddr);
	
	// Serial.print("MAX_Dist_HIGH: ");
	// tflI2C.readReg(0x31, tfAddr);
	
	// Serial.print("DIST_LOW: ");
	// tflI2C.readReg(0x00, tfAddr);
	
	// Serial.print("DIST_HiGHT: ");
	// tflI2C.readReg(0x01, tfAddr);
	
    delay( 50);
}