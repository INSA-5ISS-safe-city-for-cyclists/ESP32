#include <Arduino.h>
#include <TFLI2C.h>  // TFLuna-I2C Library v.0.2.0
#include <Wire.h>

TFLI2C tflI2C;

// Use these defaults or insert your own values
int16_t  tfAddr1 = 0x10;
int16_t tfAddr2 = 0x11;     // default I2C address
uint16_t tfFrame = TFL_DEF_FPS;   // default frame rate

// device variables passed back by getData
int16_t  tfDist1 = 0 ;
int16_t  tfDist2 = 0 ;   // distance in centimeters

//  This is a group of various sample
//  commands that can be called at setup.
void sampleCommands( uint8_t adr)
{
   Serial.print("System Reset: ");
    if( tflI2C.Soft_Reset( adr))
    {
        Serial.println( "Passed");
    }
    else tflI2C.printStatus();  // `printStatus()` is for troubleshooting,
                                //  It's not necessary for operation.
    delay(500);

    Serial.print( "Set Frame Rate to: ");
    if( tflI2C.Set_Frame_Rate( tfFrame, adr))
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
    // Execute a group of commands.
    // Comment this out if not needed.
    sampleCommands( tfAddr1);
	//sampleCommands(tfAddr2);

	uint16_t dist_max = 1500;
	uint8_t * p_tfl = (uint8_t *) &dist_max ;
	if(!tflI2C.writeReg(0x30, 0x10, p_tfl[0])) exit(EXIT_FAILURE) ;
	if(!tflI2C.writeReg(0x31, 0x10, p_tfl[1])) exit(EXIT_FAILURE) ;
}

void loop()
{
    // If data is read without error...
    if( tflI2C.getData( tfDist1, tfAddr1))
    {
        Serial.println("Dist: ");      // ...print distance,
        Serial.println(tfDist1);

    }
    else tflI2C.printStatus();        // else, print error status.
	
    delay( 50);

	/*    if( tflI2C.getData( tfDist2, tfAddr2))
    {
        Serial.print("Dist: ");      // ...print distance,
        Serial.print(tfDist2);

    }
    else tflI2C.printStatus();  */      // else, print error status.
}