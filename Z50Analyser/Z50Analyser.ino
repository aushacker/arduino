//
// Z50 Bus Analyser
//
// Based on Ben Eaters design.
//
// March 2022
//

const int RD = 40;
const int WR = 41;

void setup() {
  Serial.begin(115200);

  DDRA = 0;  // AH
  DDRC = 0;  // AL
  DDRL = 0;  // Data

  pinMode(RD, INPUT);  // Active low
  pinMode(WR, INPUT);  // Active low
}

void loop() {
  char buff[16];

  char rW = (digitalRead(RD) == 0) ? 'r' : (digitalRead(WR) == 0) ? 'W' : '?';
  
  sprintf(buff, "%02X%02X %02X %c\n", PINC, PINA, PINL, rW);
  Serial.print(buff);
}
