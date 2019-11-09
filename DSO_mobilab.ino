#include <EEPROM.h>
int numSamples=0;
long t, t0;
uint8_t x,val;
float v;
int addr = 0;
byte dat[1500];

byte value;
void setup()
{
  Serial.begin(115200);

  ADCSRA = 0;             // clear ADCSRA register
  ADCSRB = 0;             // clear ADCSRB register
  ADMUX |= (1 & 0x07);    // set A0 analog input pin
  ADMUX |= (1 << REFS0);  // set reference voltage
  ADMUX |= (1 << ADLAR);  // left align ADC value to 8 bits from ADCH register

  // sampling rate is [ADC clock] / [prescaler] / [conversion clock cycles]
  // for Arduino Uno ADC clock is 16 MHz and a conversion takes 13 clock cycles
 // ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);    // 32 prescaler for 38.5 KHz
 //ADCSRA |= (1 << ADPS2);                     // 16 prescaler for 76.9 KHz
// ADCSRA |=  (1 << ADPS1) | (1 << ADPS0);     // 8 prescaler for 153.8 KHz
  ADCSRA &= 0b11111000;
  ADCSRA |= 0b00000011; 
  ADCSRA |= (1 << ADATE); // enable auto trigger
  ADCSRA |= (1 << ADIE);  // enable interrupts when measurement complete
  ADCSRA |= (1 << ADEN);  // enable ADC
  ADCSRA |= (1 << ADSC);  // start ADC measurements
}

ISR(ADC_vect)
{
  
  
  dat[addr] = ADCH;
  addr++;// read 8 bit value from ADC
  //Serial.println("y");
 if(addr>=1500){
  ADCSRA &=0b10111111;
  
     
    for(int i=0;i<1500;i++)
    {
      Serial.println(dat[i],DEC);
      //Serial.print("x");
     
     }
     ADCSRA |= (1 << ADSC);
     addr=0;
     
 }

  
}
  
void loop()
{
  
  
  
}
