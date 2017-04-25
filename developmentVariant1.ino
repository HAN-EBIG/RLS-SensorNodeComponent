/**

 *
 * DESCRIPTION
 *
 * Interrupt driven binary switch example with dual interrupts
 * Connect one button or door/window reed switch between
 * digitial I/O pin 3 (BUTTON_PIN below) and GND and the other
 * one in similar fashion on digital I/O pin 2.
 * This example is designed to fit Arduino Nano/Pro Mini
 *
 */


// Enable debug prints to serial monitor
#define MY_DEBUG

#define MY_RADIO_NRF24

#include <MySensors.h>

#define SKETCH_NAME "Binary Sensor"
#define SKETCH_MAJOR_VER "1"
#define SKETCH_MINOR_VER "0"

#define PRIMARY_CHILD_ID 3

#define PRIMARY_BUTTON_PIN 2   // Arduino Digital I/O pin for button/reed switch


// Change to V_LIGHT if you use S_LIGHT in presentation below
MyMessage msg(PRIMARY_CHILD_ID, V_TRIPPED);
MyMessage msg2(SECONDARY_CHILD_ID, V_TRIPPED);

void setup()
{
	// Setup the buttons
	pinMode(PRIMARY_BUTTON_PIN, INPUT);

	// Activate internal pull-ups
	digitalWrite(PRIMARY_BUTTON_PIN, HIGH);
}

void presentation()
{
	// Send the sketch version information to the gateway and Controller
	sendSketchInfo(SKETCH_NAME, SKETCH_MAJOR_VER "." SKETCH_MINOR_VER);

	// Register binary input sensor to sensor_node (they will be created as child devices)
	present(PRIMARY_CHILD_ID, S_DOOR);
}

// Loop will iterate on changes on the BUTTON_PINs
void loop()
{
	uint8_t value;
	static uint8_t sentValue=2;

	// Short delay to allow buttons to properly settle
	sleep(5);

	value = digitalRead(PRIMARY_BUTTON_PIN);

	if (value != sentValue) {
		// Value has changed from last transmission, send the updated value
		send(msg.set(value==HIGH));
		sentValue = value;
	}

	value = digitalRead(SECONDARY_BUTTON_PIN);

	// Sleep until something happens with the sensor
	sleep(PRIMARY_BUTTON_PIN-2, CHANGE, SECONDARY_BUTTON_PIN-2, CHANGE, 0);
}


