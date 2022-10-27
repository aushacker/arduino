//
// Z50 Keyboard Tester
//
// 26 October 2022
//

const int RESET = 3;
const int RD = 40;
const int WR = 41;

void setup() {
  Serial.begin(115200);


  DDRA = 0xff;  // AH
  DDRC = 0xff;  // AL
  DDRL = 0;     // Data

  digitalWrite(RESET, 0); // Active low, fake open drain
  digitalWrite(RD, 1);
  pinMode(RD, OUTPUT);  // Active low
  digitalWrite(WR, 1);
  pinMode(WR, OUTPUT);  // Active low
  assertReset();
}

void loop() {
  byte c = read(0xC000);
  if (c & (1 << 7)) {
    Serial.print(c);
    read(0xC010); // clear keyboard strobe
  }
}

// take reset low for 100ms
void assertReset() {
  pinMode(RESET, OUTPUT);
  delay(100);
  pinMode(RESET, INPUT);
}

byte read(unsigned int addr) {
  byte data;
  setAddress(addr);
  digitalWrite(RD, 0);
  delay(1);
  data = PINL;
  delay(1);
  digitalWrite(RD, 1);
  return data;
}

void setAddress(unsigned int addr) {
  PORTA = addr >> 8;
  PORTC = addr & 0xff;
}
