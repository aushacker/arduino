/*
 * Runs a 16 bit timer in CTC mode on a Mega2560 R3 board.
 * Generates a basic DCC idle frame. 
 *
 * aushacker
 * 12 Nov 2024
 */

const int PIN_DCC = 46;     // OCR5A output pin to drive DCC signal
const int PIN_DEBUG1 = 22;  // Utility pin for code tracing in logic analyser
const int PIN_DEBUG2 = 23;  // Utility pin for code tracing in logic analyser

// Counter periods for DCC logic @16MHz
const int DCC_ONE = 928 - 1;    // 58us (half period for a 1 bit)
const int DCC_ZERO = 1600 - 1;  // 100us (half period for a 0 bit)

// FSM to control DCC frame
const byte PREAMBLE = 0;
const byte GAP = 1;
const byte VALUE = 2;
const byte IDLE = 3;

volatile byte state = IDLE;
volatile byte count = 100;  // extended preamble to handle startup

volatile byte idx;
volatile byte frame[4];
volatile byte frame_size;

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
  static byte i = 0;

  if (state == IDLE) {
    if (i++ % 2) {
      frame[0] = frame[2] = 0xff;
      frame[1] = 0;
    } else {
      frame[0] = 3;
      frame[1] = 0x58;
      frame[2] = 3 ^ 0x58;
    }

    frame_size = 3;
    idx = 0;

    state = PREAMBLE;
    count = 15;
  }
}

/*
 * Generates DCC frame from timer and frame[].
 */
ISR(TIMER5_COMPA_vect) {
  static byte value = 0;
  static byte toggle = 1;

  digitalWrite(PIN_DEBUG1, HIGH);

  toggle ^= 1;  // ignore every second interrupt to allow a full high/low cycle

  if (toggle) {
    switch (state) {
      case PREAMBLE:
        if (--count == 0) {
          state = GAP;
          count = 1;
          OCR5A = DCC_ZERO;
        }
        break;
      case GAP:
        if (--count == 0) {
          state = VALUE;
          digitalWrite(PIN_DEBUG2, HIGH);
          count = 8;
          value = frame[idx++];
          if(value & 0b10000000) {
            OCR5A = DCC_ONE;
          } else {
            OCR5A = DCC_ZERO;
          }
        }
        break;
      case VALUE:
        if (--count == 0) {
          if (idx >= frame_size) {
            state = IDLE;
            OCR5A = DCC_ONE;
          } else {
            state = GAP;
            count = 1;
            OCR5A = DCC_ZERO;
          }
          digitalWrite(PIN_DEBUG2, LOW);
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
        state = IDLE;
        OCR5A = DCC_ONE;
    }
  }

  digitalWrite(PIN_DEBUG1, LOW);
}
