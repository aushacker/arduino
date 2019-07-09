/*
 * A trivial example the reads and writes an int variable
 * using a pointer.
 * 
 * Aushacker 9 July 2019
 */

int a = 1;
int * p;

void setup()
{
  Serial.begin(9600);
  delay(100);

  // Initial value of variable 'a'
  Serial.println(a);

  // Initialise pointer, 'p' stores the address of variable 'a'
  p = &a;

  // Pointer dereferencing (read case)
  // Output the value pointed to by 'p' i.e. variable 'a'
  Serial.println(*p);

  // Pointer dereferencing (write case)
  // Change value in memory pointed to by 'p'
  *p = 42;

  // Output the value pointed to by 'p' i.e. variable 'a'
  Serial.println(*p);

  // Final value of variable 'a'
  Serial.println(a);
}

void loop()
{
}
