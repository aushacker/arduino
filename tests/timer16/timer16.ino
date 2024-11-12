/*
 * Runs a 16 bit timer in CTC mode on a Mega2560 R3 board.
 *
 * aushacker
 * 11 Nov 2024
 */
const int PIN_DCC = 46;
const int PIN_DEBUG1 = 22;
const int PIN_DEBUG2 = 23;

// Counter periods for DCC logic @16MHz
const int DCC_ONE = 928 - 1;
const int DCC_ZERO = 1600 - 1;

// FSM to control DCC frame
const byte PREAMBLE = 0;
const byte GAP1 = 1;
const byte VALUE1 = 2;
const byte GAP2 = 3;
const byte VALUE2 = 4;
const byte GAP3 = 5;
const byte VALUE3 = 6;

volatile byte state = PREAMBLE;
volatile byte count = 100;  // extended preamble to handle startup

void setup() {
  pinMode(PIN_DCC, OUTPUT);
  digitalWrite(PIN_DCC, HIGH);

  pinMode(PIN_DEBUG1, OUTPUT);
  digitalWrite(PIN_DEBUG1, LOW);
  pinMode(PIN_DEBUG2, OUTPUT);
  digitalWrite(PIN_DEBUG2, LOW);

  // Timer 5 is in CTC mode, toggling OCR5A on every match
  // Clock source is ClkIO/1
  TCCR5A = 0b01000000; // OCR5A1=toggle, WGM1:0=00 (CTC)
  TCCR5B = 0b00001001; // WGM3:2=01 (CTC), ClkIO/1
  TIMSK5 = 0b00000010; // Interrupt on OCIE5A
  OCR5A = DCC_ONE;
}

void loop() {
  // put your main code here, to run repeatedly:

}

ISR(TIMER5_COMPA_vect) {
  static byte value = 0;
  static byte toggle = 1;

  digitalWrite(PIN_DEBUG1, HIGH);

  toggle ^= 1;

  if (toggle) {
    switch(state) {
      case PREAMBLE:
        if (--count == 0) {
          state = GAP1;
          count = 1;
          OCR5A = DCC_ZERO;
        }
        break;
      case GAP1:
        if (--count == 0) {
          state = VALUE1;
          digitalWrite(PIN_DEBUG2, HIGH);
          count = 8;
          value = 0xa5;
          if (value & 0b10000000) {
            OCR5A = DCC_ONE;
          } else {
            OCR5A = DCC_ZERO;
          }
        }
        break;
      case VALUE1:
        if (--count == 0) {
          state = GAP2;
          digitalWrite(PIN_DEBUG2, LOW);
          count = 1;
          OCR5A = DCC_ZERO;
        } else {
          // shift left
          value = value << 1;
          if (value & 0b10000000) {
            OCR5A = DCC_ONE;
          } else {
            OCR5A = DCC_ZERO;
          }
        }
        break;
      case GAP2:
        if (--count == 0) {
          state = VALUE2;
          digitalWrite(PIN_DEBUG2, HIGH);
          count = 8;
          value = 0x33;
          if (value & 0b10000000) {
            OCR5A = DCC_ONE;
          } else {
            OCR5A = DCC_ZERO;
          }
        }
        break;
      case VALUE2:
        if (--count == 0) {
          state = GAP3;
          digitalWrite(PIN_DEBUG2, LOW);
          count = 1;
          OCR5A = DCC_ZERO;
        } else {
          // shift left
          value = value << 1;
          if (value & 0b10000000) {
            OCR5A = DCC_ONE;
          } else {
            OCR5A = DCC_ZERO;
          }
        }
        break;
      case GAP3:
        if (--count == 0) {
          state = VALUE3;
          digitalWrite(PIN_DEBUG2, HIGH);
          count = 8;
          value = 0xcc;
          if (value & 0b10000000) {
            OCR5A = DCC_ONE;
          } else {
            OCR5A = DCC_ZERO;
          }
        }
        break;
      case VALUE3:
        if (--count == 0) {
          state = PREAMBLE;
          digitalWrite(PIN_DEBUG2, LOW);
          count = 14;
          OCR5A = DCC_ONE;
        } else {
          // shift left
          value = value << 1;
          if (value & 0b10000000) {
            OCR5A = DCC_ONE;
          } else {
            OCR5A = DCC_ZERO;
          }
        }
        break;
      default:
        state = PREAMBLE;
        count = 14;
        OCR5A = DCC_ONE;
    }
  }

  digitalWrite(PIN_DEBUG1, LOW);
}
