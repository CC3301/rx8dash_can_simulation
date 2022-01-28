#include <mcp_can.h>
#include <SPI.h>

MCP_CAN CAN0(10);

/* this is the data we want to stream out to the canbus
see can_message_meaning.csv for more detail on 0x201 message id
bytes 0 and 1 are RPM signal, direct decimal correlation, idle would be ~ 800 RPM so 800(10) = 
bytes 2 and 3 are unknown so set to 0
bytes 4 and 5 are vehicle speed where X/100 kph so 50 kph = 5000, 200 kph = 20000
byte 6 is throttle position, 200 = 100% throttle input
byte 7 is unknown so set to 0
*/
byte data[8] = {0x03, 0x20, 0x00, 0x00, 0x13, 0x88, 0xc8, 0x00};

// setup
void setup() {
	// start serial communication
    Serial.begin(115200);

    // Initialize MCP2515 running at 16MHz with a baudrate of 500kb/s and the masks and filters disabled.
    if (CAN0.begin(MCP_ANY, CAN_500KBPS, MCP_16MHZ) == CAN_OK) {
		Serial.println("MCP2515 Initialized Successfully!");
	} else {
		Serial.println("Error Initializing MCP2515...");
	}

	// Change to normal mode to allow messages to be transmitted
    CAN0.setMode(MCP_NORMAL);   
}

// keep repeating
void loop() {
    // send data:  ID = 0x201 "speed" message as defined in rx8.kcd
	// Data length = 8 bytes, 'data' = array of data bytes to send
    byte sndStat = CAN0.sendMsgBuf(0x201, 0, 8, data);

    if(sndStat != CAN_OK) {
        Serial.println("Error Sending Message...");
    }

	// send string representation of the message via serial comm
	Serial.println("0010 00000001 0 8 0011 00100000 00000000 00000000 00010011 10001000 11001000 00000000");

    delay(10);  // we need to send this data every 10ms
}