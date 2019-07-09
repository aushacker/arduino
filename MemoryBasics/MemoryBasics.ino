/*
 * A trivial that shows the difference between static and 
 * automatic memory allocation.
 * 
 * Aushacker 9 July 2019
 */

int a = 1;  // variable allocated 'static' (implicit due to global scope)

void setup()
{
  static int b = 2; // variable allocated 'static' (explicit but not global)
  int c = 3;        // variable allocated 'auto'

  Serial.begin(9600);
  delay(100);

  Serial.print("a: 0x");
  Serial.println((unsigned int) &a, HEX);

  Serial.print("b: 0x");
  Serial.println((unsigned int) &b, HEX);

  Serial.print("c: 0x");
  Serial.println((unsigned int) &c, HEX);
}

void loop()
{
}

