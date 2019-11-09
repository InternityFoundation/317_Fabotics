//-----------------------------------------------------------------------------
// Copyright 2018 Peter Balch
// subject to the GNU General Public License
//-----------------------------------------------------------------------------

#include <math.h>

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

const long BAUDRATE  = 115200;  // Baud rate of UART in bps

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

const byte numberOfDigits = 7; // number of digits in the frequense
byte freqSGLo[numberOfDigits] = {0, 0, 0, 1, 0, 0, 0}; // 1000Hz 
byte freqSGHi[numberOfDigits] = {0, 0, 0, 0, 2, 0, 0}; // 20kHz  

const int wSine     = 0b0000000000000000;
const int wTriangle = 0b0000000000000010;
const int wSquare   = 0b0000000000101000;

int waveType = wSquare;

const int SG_fsyncPin = 2;
const int SG_CLK = 3;
const int SG_DATA = 4;
const int SG_POWER = 6;
//int SG_iSweep,SG_nSweep;

//-----------------------------------------------------------------------------
//returns 10^y
//-----------------------------------------------------------------------------
unsigned long Power(int y) {
  unsigned long t = 1;
  for (byte i = 0; i < y; i++)
    t = t * 10;
  return t;
}

//-----------------------------------------------------------------------------
//calculate the frequency from the array.
//-----------------------------------------------------------------------------
unsigned long calcFreq(byte* freqSG) {
  unsigned long i = 0;
  for (byte x = 0; x < numberOfDigits; x++)
    i = i + freqSG[x] * Power(x);
  return i;
}

//-----------------------------------------------------------------------------
// SG_WriteRegister
//-----------------------------------------------------------------------------
void SG_WriteRegister(word dat) {
    Serial.println("SG_WriteRegister");

  digitalWrite(SG_CLK, LOW);
  digitalWrite(SG_CLK, HIGH);

  digitalWrite(SG_fsyncPin, LOW);
  for (byte i = 0; i < 16; i++) {
    if (dat & 0x8000)
      digitalWrite(SG_DATA, HIGH);
    else
      digitalWrite(SG_DATA, LOW);
    dat = dat << 1;
    digitalWrite(SG_CLK, HIGH);
    digitalWrite(SG_CLK, LOW);
  }
  digitalWrite(SG_CLK, HIGH);
  digitalWrite(SG_fsyncPin, HIGH);
}

//-----------------------------------------------------------------------------
// SG_Reset
//-----------------------------------------------------------------------------
void SG_Reset() {
    Serial.println("");

  delay(100);
  SG_WriteRegister(0x100);
  delay(100);
}

//-----------------------------------------------------------------------------
// SG_freqReset
//    reset the SG regs then set the frequency and wave type
//-----------------------------------------------------------------------------
void SG_freqReset(long frequency, int wave) {
    Serial.println("SG_freqReset");

  long fl = frequency * (0x10000000 / 25000000.0);
  SG_WriteRegister(0x2100);
  SG_WriteRegister((int)(fl & 0x3FFF) | 0x4000);
  SG_WriteRegister((int)((fl & 0xFFFC000) >> 14) | 0x4000);
  SG_WriteRegister(0xC000);
  SG_WriteRegister(wave);
  waveType = wave;
}

void InitSigGen(void) {
    Serial.println("InitSigGen");

  pinMode(SG_POWER, OUTPUT);
  digitalWrite(SG_POWER, HIGH);

  pinMode(SG_DATA, OUTPUT);
  pinMode(SG_CLK, OUTPUT);
  pinMode(SG_fsyncPin, OUTPUT);
  digitalWrite(SG_fsyncPin, HIGH);
  digitalWrite(SG_CLK, HIGH);
  SG_Reset();
  SG_freqReset(calcFreq(freqSGLo), waveType);
}

//-----------------------------------------------------------------------------
// SerialCommand
//   if a byte is available in teh seril input buffer
//   execute it as a command
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// InitSigGen
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Main routines
// The setup function
//-----------------------------------------------------------------------------
void setup (void) {
  // Open serial port with a baud rate of BAUDRATE b/s
  
  Serial.begin(BAUDRATE);

  Serial.println("SigGen " __DATE__); // compilation date
  Serial.println("OK");

  pinMode(LED_BUILTIN, OUTPUT);

  InitSigGen();
}

//-----------------------------------------------------------------------------
// Main routines
// loop
//-----------------------------------------------------------------------------
void loop (void) {
  Serial.println("loop");

  //SerialCommand();
}
