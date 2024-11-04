/*
*/
#include <stdint.h>

#define LF 10

byte data[256];

void setup() {
  // Output program pins, turn off all mosfets
  DDRC = 0xff;
  PORTC = 0;

  
  exitProgMode();

  Serial.begin(19200);
  Serial.println("PROM Shield v0.1");
}

void loop() {
  int cmd;

  Serial.print(">");

  wait:
  while (Serial.available() == 0) // wait
    ;

  cmd = Serial.read();

  if (cmd == LF)
    goto wait;
  Serial.print((char)cmd);
  Serial.println();

  switch(cmd) {
  case 'd':
    dumpData();
    break;
  case 'r':
    readDevice();
    break;
  default:
    Serial.println("d - dump data");
    Serial.println("r - read device");
    break;
  }
}

/*
 * Prints a hex dump of the 256 data bytes n the buffer.
 */
void dumpData() {
  uint16_t index = 0;

  for(uint8_t i = 0; i < 16; i++) {
    printByte(index);
    for(uint8_t j = 0; j < 16; j++) {
      Serial.print(' ');
      printByte(data[index++]);
    }
    Serial.println();
  }
}

void printByte(byte b) {
  if(b < 0x10)
    Serial.print('0');

  Serial.print(b, HEX);
}

void readDevice() {

}

void exitProgMode() {
 
  // Address control pins
  DDRA = 0;

}