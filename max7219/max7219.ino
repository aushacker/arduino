/*
 * Example bit bashing the max7219 display chip.
 * 
 * Aushacker 17 July 2019
 */

const int CS_N = 5;
const int CLK = 6;
const int DOUT = 7;

// 7-seg
//const unsigned int data[] = { 0x09FF, 0x0A08, 0x0B07, 0x0C01, 0x0800, 0x0701, 0x0602, 0x0503, 0x0404, 0x0305, 0x0206, 0x0107 };
// dot matrix
const unsigned int data[] = { 0x0900, 0x0A08, 0x0B07, 0x0C01, 0x0801, 0x0702, 0x0604, 0x0508, 0x0410, 0x0320, 0x0240, 0x0180 };

void setup()
{
  pinMode(CS_N, OUTPUT);
  pinMode(CLK, OUTPUT);
  pinMode(DOUT, OUTPUT);

  digitalWrite(CLK, LOW);
  digitalWrite(DOUT, LOW);

  deassertCS();

  Serial.println(sizeof(data));

  for (int i = 0; i < sizeof(data)/2; i++)
  {
    send(data[i]);
  }
}

void loop()
{
}

void send(unsigned int d)
{
  assertCS();
  for (int i = 0; i < 16; i++)
  {
    digitalWrite(DOUT, ((d & 0x8000) ? HIGH : LOW));
    cycleClock();
    d = d << 1; 
  }
  deassertCS(); 
}

void assertCS()
{
    digitalWrite(CS_N, LOW);
}

void cycleClock()
{
  digitalWrite(CLK, HIGH);
  digitalWrite(CLK, LOW);
}

void deassertCS()
{
    digitalWrite(CS_N, HIGH);
}

