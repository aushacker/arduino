/*
 * A trivial sketch demonstrating the difference between passing a 
 * structure by value and passing by reference.
 * 
 * Aushacker 9 July 2019
 */

struct Point {
  int x;
  int y; 
};

struct Point p1 = { 2, 3 };

void setup()
{
  Serial.begin(9600);
  delay(100);

  // Show initial conditions, structure located on heap
  Serial.print("&p1: 0x");
  Serial.println((unsigned int) &p1, HEX);
  Serial.print("p1.x: ");
  Serial.println(p1.x);
  Serial.println();

  byValue(p1);
  // Change has been lost
  Serial.print("p1.x: ");
  Serial.println(p1.x);
  Serial.println();

  byReference(&p1);
  // Change has 'stuck'
  Serial.print("p1.x: ");
  Serial.println(p1.x);
}

void byValue(struct Point p1Copy)
{
  // Show p1Copy is allocated on stack
  Serial.print("&p1Copy: 0x");
  Serial.println((unsigned int) &p1Copy, HEX);

  // Change the value i.e. make a change to the COPY
  p1Copy.x = 42;
  Serial.print("p1Copy.x: ");
  Serial.println(p1Copy.x);
}

void byReference(struct Point * p1Ref)
{
  // Show p1Ref is allocated on heap
  Serial.print("&p1Ref: 0x");
  Serial.println((unsigned int) &p1Ref, HEX);
  Serial.print("p1Ref: 0x");
  Serial.println((unsigned int) p1Ref, HEX);

  // Change the value i.e. this changes the ORIGINAL
  p1Ref->x = 42;
  Serial.print("p1Ref->x: ");
  Serial.println(p1Ref->x);
}

void loop()
{
}

