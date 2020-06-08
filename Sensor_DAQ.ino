#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>



#define DHTPIN 2
#define DHTTYPE DHT11 // DHT 11
DHT dht(DHTPIN, DHTTYPE);

const int buttonPin = 3;
int buttonState;

int analogPin = 0;
int raw = 0;
int Vin = 4.6;
float Vout = 0;
float R1 = 18000000;
float R2 = 0;
float buffer = 0;
float R2a;
float R2b = 0;
float s;

void setup() {
  // put your setup code here, to run once:
   lcd.begin(16,2); 
  Serial.begin(9600);
  Serial.println("CLEARDATA");
   
  Serial.println("LABEL, DATE, TIME, RH %, TEMP *C, S %, R1 MOhm, R2 MOhm, R3 MOhm, Glu *mg/dL");
   dht.begin();

   pinMode(buttonPin, INPUT);

raw = analogRead(analogPin);

if (raw)
{
  buffer = raw * Vin;
  Vout = (buffer) / 1024.0;
  buffer = (Vin / Vout) - 1;
  R2 = (R1 * buffer)/1000000;
  delay(2000);
  
}
lcd.clear();
lcd.setCursor(0,0);
lcd.print("Press Start  ");
}

void loop() {
  // put your main code here, to run repeatedly:
    buttonState = digitalRead(buttonPin); 
  if (buttonState == LOW){
    //Serial.print(buttonState);
    //Serial.println("Push start");
    return;
    //delay(2000);
   
   }
  else if (buttonState == HIGH)
 {
   delay(2000);
  }
  
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  raw = analogRead(analogPin);

if (raw)
{
  buffer = raw * Vin;
  Vout = (buffer) / 1024.0;
  buffer = (Vin / Vout) - 1;
  R2a = (R1 * buffer)/1000000;
  

lcd.clear();
lcd.setCursor(0,0);
lcd.print("Detecting...  ");
 delay(30000);
}

raw = analogRead(analogPin);
if (raw)
{
  buffer = raw * Vin;
  Vout = (buffer) / 1024.0;
  buffer = (Vin / Vout) - 1;
  R2b = (R1 * buffer)/1000000;

  s = ((R2b - R2) / R2) * 100;
}

if (isnan(h) || isnan(t) || isnan(s) || isnan(R2) || isnan(R2a) || isnan(R2b)) {
  Serial.println("Failed to read from Humidty & Acetone sensor!");
  return;
}

else if (R2>R2b){
  s = 0;
  R2= 0;
  R2a = 0;
  R2b= 0;
  Serial.print("DATA,DATE,TIME,");
  Serial.print(h);
  Serial.print(",");
  Serial.print(t);
  Serial.print(",");
  Serial.print(s);
  Serial.print(",");
  Serial.print(R2);
  Serial.print(",");
  Serial.print(R2a);
  Serial.print(",");
  Serial.println(R2b);
   lcd.clear();
lcd.setCursor(0,0);
lcd.print("Refresh");
  return;
  
}


else if (s>3000){
  s = 0;
  R2= 0;
  R2a=0;
  R2b= 0;
  Serial.print("DATA,DATE,TIME,");
  Serial.print(h);
  Serial.print(",");
  Serial.print(t);
  Serial.print(",");
  Serial.print(s);
  Serial.print(",");
  Serial.print(R2);
  Serial.print(",");
  Serial.print(R2a);
  Serial.print(",");
  Serial.println(R2b);
  lcd.clear();
lcd.setCursor(0,0);
lcd.print("Refresh ");
  return;
}

Serial.print("DATA,DATE,TIME,");
Serial.print(h);
Serial.print(",");
Serial.print(t);
Serial.print(",");
Serial.print(s);
Serial.print(",");
Serial.print(R2);
Serial.print(",");
Serial.print(R2a);
Serial.print(",");
Serial.println(R2b);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("S%: ");
lcd.print(s, 2);
lcd.print("%");
lcd.setCursor(0, 1);
lcd.print("RH:");
lcd.print(h);
lcd.setCursor(9, 1);
lcd.print("T:");
lcd.print(t);

delay(3000);
  }
  
