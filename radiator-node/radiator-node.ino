// Enable debug prints to serial monitor
#define MY_DEBUG 

// Enable and select radio type attached
#define MY_RADIO_NRF24

#define MY_NODE_ID 50
#define CHILD_ID_TEMP_IN 10
#define CHILD_ID_TEMP_OUT 11

#include <SPI.h>
#include <MySensors.h>  
#include <DallasTemperature.h>
#include <OneWire.h>

#define COMPARE_TEMP 0 // Send temperature only if changed? 1 = Yes 0 = No

#define ONE_WIRE_BUS 3 // Pin where dallase sensor is connected 

#define MAX_ATTACHED_DS18B20 16
unsigned long SLEEP_TIME = 30000; // Sleep time between reads (in milliseconds)
OneWire oneWire(ONE_WIRE_BUS); // Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
DallasTemperature sensors(&oneWire); // Pass the oneWire reference to Dallas Temperature. 

float lastTemperature[MAX_ATTACHED_DS18B20];

int numSensors=0;
bool receivedConfig = false;
bool metric = true;
// Initialize temperature message
MyMessage msg(0,V_TEMP);

void before()
{
  // Startup up the OneWire library
  sensors.begin();
}

void setup()  
{ 
  // requestTemperatures() will not block current thread
  sensors.setWaitForConversion(false);
}

void presentation() {
  // Send the sketch version information to the gateway and Controller
  sendSketchInfo("Radiator Node", "1.0");

  // Fetch the number of attached temperature sensors  
  numSensors = sensors.getDeviceCount();

  // Present all sensors to controller
  present(CHILD_ID_TEMP_IN,S_TEMP);
  present(CHILD_ID_TEMP_OUT,S_TEMP);
 
}

void loop()     
{     
  // Fetch temperatures from Dallas sensors
  sensors.requestTemperatures();

  // Read temperatures and send them to controller 

  for (int i=0; i<numSensors && i<MAX_ATTACHED_DS18B20; i++) {

    // Fetch and round temperature to one decimal
    float temperature =  sensors.getTempCByIndex(i); 

    // Only send data if temperature has changed and no error
    #if COMPARE_TEMP == 1
    if (lastTemperature[i] != temperature && temperature != -127.00 && temperature != 85.00) {
    #else
    if (temperature != -127.00 && temperature != 85.00) {
    #endif

      // Send in the new temperature
      if( i == 0 ){
        // sensor in
        send(msg.setSensor(CHILD_ID_TEMP_IN).set(temperature,1));
      } else if (i == 1){
        send(msg.setSensor(CHILD_ID_TEMP_OUT).set(temperature,1));
      }
            
      // Save new temperatures for next compare
      lastTemperature[i]=temperature;
    }
    
  }
  sleep(SLEEP_TIME);
}
