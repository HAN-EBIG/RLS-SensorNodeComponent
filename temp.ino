/*
 * *****************************
 * @Date        25/03/2017
 * @Author      Ties Klappe
 * @Sketchinfo  Node that measures the temperature on digital pin 2
 * @Hardware    DS18B20, Arduino nano, Denver 2600mAh powerbank
 * @Libraries   OneWire.h, MySensors 2.1
 * *****************************
 */


//#define MY_DEBUG

//MS precompile defines
#define MY_RADIO_NRF24
#define MY_NODE_ID 51

//Libraries 
#include <MySensors.h>
#include <OneWire.h>

//SensorID
#define CHILD_ID_TEMP 2

//Arduino sleep time
unsigned long SLEEP_TIME = 900000; 


MyMessage msg(CHILD_ID_TEMP, V_TEMP);


void presentation()
{
  // Send the sketch version information to the gateway and Controller
  sendSketchInfo("Temp Sensor", "1.0");

  // Register all sensors to gateway (they will be created as child devices)
  present(CHILD_ID_TEMP, S_TEMP);
  Serial.begin(115200);
}

void loop()
{

  float tempValue = getSensorValue();
  //Serial.println(getSensorValue());

  if (tempValue <= 45 && tempValue >= 5) {
    send(msg.set(tempValue, 2));
  }
  sleep(SLEEP_TIME);
  //delay(1000);
}

float getSensorValue()  {

  OneWire ds(2);

  /*____________________________________________________________________*/

  byte i;
  byte present = 0;
  byte data[12];
  byte addr[8];

  ds.reset_search();

  if ( !ds.search(addr)) {
    ds.reset_search();
    return -1000.00;
  }

  /*____________________________________________________________________*/

  for ( i = 0; i < 8; i++) {
  }

  if ( OneWire::crc8( addr, 7) != addr[7]) {
    return -1000.00;
  }

  if ( addr[0] == 0x10) {
  }
  else if ( addr[0] == 0x28) {
  }

  else {
    return -1000.00;
  }

  /*____________________________________________________________________*/

  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);        // start conversion, with parasite power on at the end

  present = ds.reset();
  ds.select(addr);
  ds.write(0xBE);         // Read Scratchpad

  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
  }

  /*____________________________________________________________________*/

  byte MSB = data[1];
  byte LSB = data[0];

  /*____________________________________________________________________*/

  float tempRead = ((MSB << 8) | LSB); //using two's compliment
  float TemperatureSum = tempRead / 16;

  return TemperatureSum;

}



