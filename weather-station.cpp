//Code by @diezc
#include "Adafruit_BMP085/Adafruit_BMP085.h"
#include "Adafruit_DHT/Adafruit_DHT.h"

#define DHTPIN 2
#define DHTTYPE DHT11

#define ONE_HOUR_MILLIS 3600000

DHT dht(DHTPIN, DHTTYPE);
Adafruit_BMP085 bmp;

double temperature=0;
double humidity=0;

unsigned long lastSync =0;

int led1 = D0;
int led2 = D1;
int led0 = D7;

double temptmp = 0.0;

int calibre=50;
int light=0;
bool lightPrev = false;

float tdht=0;

float tbmp180=0;
double pressure=0;

void setup()
{
   Spark.function("led", ledControl);
   Spark.variable("temperature", &temperature, DOUBLE);
   Spark.variable("light", &light, INT);
   Spark.variable("humidity", &humidity, DOUBLE);
   Spark.variable("pressure", &pressure, DOUBLE);
   Time.zone(3);
   
   pinMode(led1, OUTPUT);
   pinMode(led2, OUTPUT);
   pinMode(led0, OUTPUT);

   digitalWrite(led1, LOW);
   digitalWrite(led2, LOW);
   digitalWrite(led0, LOW);

   pinMode(A0, INPUT);
   pinMode(A1, INPUT);
   
   Serial.begin(9600);
   dht.begin();
   iniBMP080();
}
void loop()
{

if(Spark.connected()){
    RGB.control(true);
    RGB.color(0, 0, 0);
    digitalWrite(led0, LOW);
    lightpt();
    tempdt();
    tempdht();
    readBMP180();
    temperature = (tdht+temptmp+tbmp180)/3;
    checkTime();
}else{
    digitalWrite(led0, HIGH);
}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int ledControl(String command)
{
   int state = 0;
   //find out the pin number and convert the ascii to integer
   int pinNumber = (command.charAt(1) - '0') - 1;
   //Sanity check to see if the pin numbers are within limits
   if (pinNumber < 0 || pinNumber > 1) return -1;
   // find out the state of the led
   if(command.substring(3,7) == "HIGH") state = 1;
   else if(command.substring(3,6) == "LOW") state = 0;
   else return -1;
   // write to the appropriate pin
   digitalWrite(pinNumber, state);
   return 1;
}
//////////////////////////////////////////////////////////////////////////
void tempdt(){
    double voltage = 0.0;
    int reading = 0;
    
    reading = analogRead(A0);
    voltage = (reading * 3.3) / 4095;
 
    temptmp= (voltage - 0.5) * 100;
    Serial.print("\n Temperature TMP36: ");
    Serial.print(temptmp);
    Serial.println("");
}
/////////////////////////////////////////////////////////////////////////
void lightpt(){
light=analogRead(A1);
light=map(light,0,4095,0,100);
if(light > calibre && lightPrev == false){
    Spark.publish("lightON");
    lightPrev = true;
}
if(light < calibre && lightPrev == true){
    lightPrev = false;
}

Serial.print("\n Light: ");
Serial.print(light);
Serial.print("%");
Serial.println("");
}
//////////////////////////////////////////////////////////////////////////
void tempdht(){
    humidity = dht.getHumidity();
    tdht = dht.getTempCelcius();
    Serial.print("\n Temperature DHT:");
    Serial.print(tdht);
    Serial.print("\n Humidity DHT:");
    Serial.print(humidity);
    Serial.println("");
}
//////////////////////////////////////////////////////////////////////////
void checkTime(){
 if(millis() - lastSync > ONE_HOUR_MILLIS*12){
    Spark.syncTime();
    lastSync = millis();
  }
}
/////////////////////////////////////////////////////////////////////////
void iniBMP080(){
	if (!bmp.begin()) {
		Serial.println("Could not find a valid BMP085 sensor, check wiring!");
		while (1) {}
	}
}
/////////////////////////////////////////////////////////////////////////
void readBMP180(){
    tbmp180=bmp.readTemperature();
    pressure=bmp.readPressure()/100;
    
    Serial.print("Temperature = ");
    Serial.print(tbmp180);
    Serial.println(" *C");
    
    Serial.print("Pressure = ");
    Serial.print(pressure);
    Serial.println(" Pa");

    // pressure of 1013.25 millibar = 101325 Pascal
}
///////////////////////////////////////////////////////////////////////////
