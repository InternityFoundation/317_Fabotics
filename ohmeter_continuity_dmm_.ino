int analogPin= A0;
int raw= 0;
int Vin= 5;
float Vout= 0;
float R1= 10000;
float R2= 0;
float v= 0;
void setup()
{pinMode(A0,INPUT);
  pinMode(13,OUTPUT);
Serial.begin(9600);
}

void loop()
{


raw= analogRead(analogPin);
Serial.println(raw);
delay(1000);
if(raw<=1021) 
{
v= raw * Vin;
Vout= (v)/1024.0;
v= (Vin/Vout) -1;
R2= R1 * v;
R2=R2;
Serial.print("Vout: ");
Serial.println(Vout);
Serial.print("R2: ");
Serial.println(R2);

digitalWrite(13,LOW);
}

if(raw>1021)
{
  digitalWrite(13,HIGH);
}



}
