#define SIM800_TX_PIN 51
#define SIM800_RX_PIN 50
int RXPin = 53, TXPin = 52;
int GPSBaud = 9600;

#include <Wire.h>
#include <stdlib.h>
#include "Wire.h"
#include "SIM900.h"
#include <sms.h>
#include <avr/wdt.h>
#include <Adafruit_SleepyDog.h>

#include <DHT.h>
#include <DHT_U.h>
#include "I2Cdev.h"
#include "MPU6050.h"
#include <BMP085.h>

#include "TinyGPS.h"
TinyGPS gps;
#define GPS_TX_DIGITAL_OUT_PIN 19
#define GPS_RX_DIGITAL_OUT_PIN 18

long startMillis;
long secondsToFirstLocation = 0;

#define DEBUG

float latitude = 0.0;
float longitude = 0.0;

#include <SoftwareSerial.h>
SoftwareSerial SerialAT(SIM800_RX_PIN, SIM800_TX_PIN); // RX, TX
SoftwareSerial gpsSerial(RXPin, TXPin);

// Your GPRS credentials
// Leave empty, if missing user or pass
const char apn[] = "v-internet";
const char user[] = "";
const char pass[] = "";

SMSGSM sms;
#define PIN 5
#define TYPE DHT22
int rain;
DHT dht(PIN, TYPE);
MPU6050 accelgyro;
BMP085 pressure_m;

boolean started = false;
const int smsLength = 400;

char phoneNo[20];
char admin[20] = "+84855810034";
char admin2[20] = "+84901285973";
//char admin3[20] = "+84981370202";

//admin
const char BcryptPassword[100] = "?2y?12$fTJT94j5a41Ho4EnGqe0Cea2yCZIozv4yrerlLUn7RqI8nYXJzaiu";
const char smsText[100];

//Temp
const float avgtemp = 29.5;
const char TemperatureString[40];

//Humid
const float avghum = 79.5;
const char HumidityString[30];

//Rain
const char RainString[5];

//Pressure
const char PressureString[35];

//Altitude
const char AltitudeString[40];

//All Function
const char AzureIotString[110];
const char AllString[630];

//Wrong command
const char unknow[52];

//ChatBot
const char txt2[5];
const char txt3[100];

//conversation
const char hi[26];
const char thank[15];
const char passw[4];
const char selfintroduce[83];
const char Instructionstring[630];
const char emotion[67];


//Other Measurement
String statuss ;
String rainstatus ;
int AccelerationX, AccelerationY, AccelerationZ;
int RotationX, RotationY, RotationZ;
const int sensorMin = 0;     // sensor minimum
const int sensorMax = 1024;  // sensor maximum
long randNumber;
const char ResetString[40];

class Weather
{
  public:
    float temperature = dht.readTemperature();
    int temp = temperature;
    float humidity = dht.readHumidity();
    int humid = humidity;
    float temperaturee = pressure_m.bmp085GetTemperature();
    float pressure = pressure_m.bmp085GetPressure();
    int pr = pressure/100;
    float altitude = pressure_m.calcAltitude(pressure);
    int alt = altitude;
};

void setup() {
  Wire.begin();

  // Set console baud rate
  Serial.begin(2400);
  delay(1500);
  Serial1.begin(9600);
  delay(3000);

  gpsSerial.begin(GPSBaud);
  delay(3000);
  SerialAT.begin(2400);
  delay(3000);

  beginSim();
  delay(3000);

  dht.begin();
  delay(3000);
  pressure_m.bmp085Calibration();
  delay(3000);
  accelgyro.initialize();
  delay(3000);

  if (gsm.begin(2400)) { //2400
    delay(3000);
    Serial.println(F("\nstatus=READY"));
    started = true;
  }

  randomSeed(analogRead(1));
  randNumber = random(1000, 9999);
  sprintf(txt2, "%d", randNumber);

  if (started)
  {
    Serial.println(txt2);
    //sms.SendSMS(admin, txt2);
    //delay(5000);
    //sms.SendSMS(admin2, txt2);
    //delay(5000);
    //sms.SendSMS(admin3, txt2);
    //delay(5000);
  }
  pinMode(GPS_TX_DIGITAL_OUT_PIN, INPUT);
  pinMode(GPS_RX_DIGITAL_OUT_PIN, INPUT);

  startMillis = millis();
  Serial.println("Starting");


}


void beginSim()
{
  SerialAT.println("AT+CGATT=1");
  delay(2000);
  toSerial();

  SerialAT.println("AT+CREG=1");
  delay(2000);
  toSerial();

  SerialAT.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");
  delay(2000);
  toSerial();

  // bearer settings
  SerialAT.println("AT+SAPBR=3,1,\"APN\",\"v-internet\"");
  delay(2000);
  toSerial();

  SerialAT.println("AT+SAPBR=0,1");
  delay(2000);
  toSerial();

  // bearer settings
  SerialAT.println("AT+SAPBR=1,1");
  delay(2000);
  toSerial();

  SerialAT.println("AT+SAPBR=2,1");
  delay(2000);
  toSerial();
}

void loop() {

  mainsms();

  //caution();
}
void mainsms()
{
  Weather wea;

  //Temp
  sprintf(TemperatureString, "The Weather's Temperature: %d ", wea.temp);
  //Temp-end

  //Conversation
  sprintf(hi, "Hi, service is ready ;)");
  sprintf(thank, "Your Welcome");
  sprintf(emotion, "Sorry, i cannot handle emotion, thinking and personal information");
  sprintf(selfintroduce, "I am Arigculture monitoring assisstant, ask me anything you want about your farm");
  //Conversation-end

  //All Function
  sprintf(AzureIotString, "The Weather today is:\nTemperature: %d*C\nHumidity: %d percent\nPressure: %d Pa\nsample001.azureiotcentral.com", wea.temp, wea.humid, wea.pr);
  sprintf(Instructionstring, "Here is my abilities:\n1.I can monitor the surrounding environment.\n2.I can send those values into the Azure Central account that the IDEV gave you.\n3.I can easily check my Balance and Location as well so asked me whenever you want.\n4.Not only locating myself, I can scan for nearby Athena so that if I can't find the extract location, I will extract the nearby location\n5.I can also delete all old messages and remotely reboot by only you\n6.I can do the forecaster job\nOh and before using any command, just remember to add the pin code I gave you in the message, the pins will automatically change each reboot\n Have fun ;)");

  //Humid
  sprintf(HumidityString, "The Weather's Humidity: %d%", wea.humid);
  //Humid-end

  //Rain
  sprintf(RainString, "%s",  rainstatus);
  //Rain-end
  //Air Pressure
  sprintf(PressureString, "The Weather's Pressure : %d Pa", wea.pr);
  //Air Pressure-end

  //Altitude
  sprintf(AltitudeString, "The Weather's Altitude : %d meters", wea.alt);
  //Altitude-end

  //Wrong Command
  sprintf(unknow, "Sorry , i cannot understand please retry again");
  sprintf(ResetString, "Please wait for a minutes...... ");
  //Wrong Command-end


  //Encrypt
  sprintf(txt3, "%s", BcryptPassword);
  //Encrypt -end
  //Serial.println(RainString);
  if (started)
  {
    char smsIndex; //position of sms
    smsIndex = sms.IsSMSPresent(SMS_UNREAD); //get position of an unread sms
    if ((int)smsIndex)
    {
      if (sms.GetSMS(smsIndex, phoneNo, smsText, smsLength))
      {
        delSMS();
        String txt = String(smsText);
        txt.toLowerCase();
        delay(3000);
        Serial.println(F("\ string Received"));
        if ( txt.indexOf(F("ability")) >= 0  || txt.indexOf(F("guide")) >= 0 )
        {
          sms.SendSMS(phoneNo, Instructionstring);
          delay(1000);
          Serial.println(F("\nSMS sent OK"));
        }
        else if ( txt.indexOf(F("azure")) >= 0 && txt.indexOf(txt2) >= 0)
        {
          sms.SendSMS(phoneNo, AzureIotString);
          Serial.println(F("\nSMS sent OK"));
          delay(3000);
          HttpSendPara();
          delay(1000);
        }
        else if (txt.indexOf(F("temp")) >= 0 && txt.indexOf(txt2) >= 0)
        {
          sms.SendSMS(phoneNo, TemperatureString);
          Serial.println(F("\nSMS sent OK"));
          delay(1000);
          HttpSendPara();
        }
        else if (txt.indexOf(F("humid")) >= 0  && txt.indexOf(txt2) >= 0)
        {
          sms.SendSMS(phoneNo, HumidityString);
          Serial.println(F("\nSMS sent OK"));
          delay(1000);
          HttpSendPara();
        }

        else if ( txt.indexOf(F("press")) >= 0 && txt.indexOf(txt2) >= 0)
        {
          sms.SendSMS(phoneNo, PressureString);
          Serial.println(F("\nSMS sent OK"));
          delay(1000);
          HttpSendPara();
        }

        else if (txt.indexOf(F("alt")) >= 0 && txt.indexOf(txt2) >= 0)
        {
          sms.SendSMS(phoneNo, AltitudeString);
          Serial.println(F("\nSMS sent OK"));
          delay(1000);
          HttpSendPara();
        }
        else if ( txt.indexOf(F("rain")) >= 0  && txt.indexOf(txt2) >= 0)
        {
          predict_Rain();
          Serial.println(F("\nSMS sent OK"));
          delay(1000);
        }
        else if (txt.indexOf(F("refresh")) >= 0 && txt.indexOf(txt2) >= 0)
        {
          delSMS();
          Serial.println(F("\nReceived"));
          delay(1000);
        }
        else if ( txt.indexOf(F("check")) >= 0  || txt.indexOf(F("money")) >= 0 ||  txt.indexOf(F("balance")) >= 0 && txt.indexOf(txt2) >= 0)
        {
          checkaccount();
        }
        else if ( txt.indexOf(F("all")) >= 0 || txt.indexOf(F("total")) >= 0 || txt.indexOf(F("everything")) >= 0  && txt.indexOf(txt2) >= 0)
        {
          checktotal();
        }
        else if ( txt.indexOf(F("reset")) >= 0  || txt.indexOf(F("restart")) >= 0 && txt.indexOf(txt3) >= 0)
        {
          Serial.println(F("Because am on the trip, with my favor rocketship"));
          sms.SendSMS(phoneNo, ResetString);
          delay(5000);
          //resetFunc();
        }
        else if ( txt.indexOf(F("forecast")) >= 0 || txt.indexOf(F("guess")) >= 0 && txt.indexOf(txt2) >= 0)
        {
          weatherforecast();
        }
        else if (  txt.indexOf(F("compass")) >= 0 && txt.indexOf(txt2) >= 0)
        {
          gy87_measuring();
        }
        else if (txt.indexOf(F("hi")) >= 0  || txt.indexOf(F("hello")) >= 0  )
        {
          sms.SendSMS(phoneNo, hi);
          Serial.println(F("\nSMS sent OK"));
          delay(1000);

        }
        else if ( txt.indexOf(F("what")) >= 0 || txt.indexOf(F("who")) >= 0 && txt.indexOf(F("you")) >= 0  )
        {
          sms.SendSMS(phoneNo, selfintroduce);
          Serial.println(F("\nSMS sent OK"));
          delay(1000);

        }
        else if ( txt.indexOf(F("how")) >= 0 || txt.indexOf(F("think")) >= 0  && txt.indexOf(F("you")) >= 0  )
        {
              sms.SendSMS(phoneNo, emotion);
          Serial.println(F("\nSMS sent OK"));
          delay(1000);
        }
        else if (txt.indexOf(F("thank")) >= 0 )
        {
          sms.SendSMS(phoneNo, thank);
          Serial.println(F("\nSMS sent OK"));
          delay(1000);
        }
        /*
          else if (txt.indexOf(F("sleep")) >= 0  || txt.indexOf(F("deep")) >= 0 )
          {
          Serial.print("I'm sleeping now! ");
          Watchdog.sleep(20000);
          Serial.print("I'm awake now! ");

          }
        */
        else if (txt.indexOf(F("gps")) >= 0 || txt.indexOf(F("location")) >= 0 && txt.indexOf(txt2) >= 0)
        {
          //Real GPS
         Serial.println(F("This this spartar"));
          readLocation();
          
        }
        else
        {
          sms.SendSMS(phoneNo, unknow);
          Serial.println(F("\nTry again"));
          delay(1000);
        }
      }
    }
  }
  else {
    Serial.println(F("Offline"));
  }
}

//Reset Function
void resetFunc() {
  wdt_disable();
  wdt_enable(WDTO_15MS);
  while (1) {}
}

void predict_Rain()
{
  Weather wea;
  int sensorReading = analogRead(A0);
  int range = map(sensorReading, sensorMin, sensorMax, 0, 3);
  switch (range)
  {
    case 0:    // Sensor getting wet
      sprintf(RainString, "Status: Flood");
      sms.SendSMS(phoneNo, RainString);
      Serial.println(RainString);
      break;
    case 1:    // Sensor getting wet
      sprintf(RainString, "Status: Rain");
      sms.SendSMS(phoneNo, RainString);
      Serial.println(RainString);
      break;
    case 2:    // Sensor dry
      sprintf(RainString, "Status: Not Raining");
      sms.SendSMS(phoneNo, RainString);
      Serial.println(RainString);
      break;
  }
}

void HttpSendPara()
{
  Weather wea;
  beginSim();
  SerialAT.println("AT+HTTPINIT");
  delay(2000);
  toSerial();

  SerialAT.println("AT+HTTPPARA=\"CID\",1\r");
  delay(2000);
  toSerial();

  SerialAT.print("AT+HTTPPARA=\"URL\",\"http://dev-demo-123.azurewebsites.net/api/HttpTrigger1?");
  delay(50);
  SerialAT.print("code=qdp9Kl6tEndxXa0CrgEoTYv2sUKzJJi1B6GvpfDOZdTbkIceL/CKXQ==");
  delay(50);
  SerialAT.print("&temp=");
  delay(50);
  SerialAT.print(wea.temp);
  delay(50);
  SerialAT.print("&humid=");
  delay(50);
  SerialAT.print(wea.humid);
  delay(50);
  SerialAT.print("&press=");
  delay(50);
  SerialAT.print(wea.pr);
  delay(50);
  SerialAT.print("&alt=");
  delay(50);
  SerialAT.print(wea.alt);
  delay(50);
  SerialAT.print("\"\r\n");
  delay(2000);
  toSerial();

  SerialAT.println("AT+HTTPACTION=1");
  delay(2000);
  toSerial();

  SerialAT.print("AT+HTTPREAD");
  delay(2000);
  toSerial();

  SerialAT.println("AT+HTTPTERM");
  delay(2000);
  toSerial();

}

void delSMS() { // Delete All messages
  SerialAT.print("AT+CMGDA=\"");
  SerialAT.println("DEL ALL\"");
  delay(500);
  Serial.println( "All Messages Deleted" );
}

void checkaccount()
{
  char account[60];
  gsm.SimpleWriteln("AT+CUSD=1,\"*101#\"");
  delay(5000);
  char resp[60];
  gsm.read(resp, 60);
  delay(2000);
  strncpy(account, resp, 60);
  sms.SendSMS(phoneNo, account);
  Serial.println(F("\nSMS sent OK"));
}
void checklocation()
{
  char locate[60];
  gsm.SimpleWriteln("AT+CLBS=1,1");
  delay(5000);
  char location[60];
  gsm.read(location, 60);
  delay(2000);
  strncpy(locate, location, 60);
  sms.SendSMS(phoneNo, locate);
  Serial.println(F("\nSMS sent OK"));
}
//All Command
void checktotal()
{
  char account[60];
  Weather wea;
  gsm.SimpleWriteln("AT+CUSD=1,\"*101#\"");
  delay(10000);
  char resp[60];
  gsm.read(resp, 60);
  delay(5000);
  strncpy(account, resp, 60);
  delay(5000);
  //
  bool newData = false;
  unsigned long chars = 0;
  unsigned short sentences, failed;
  const char GPSString[70];

  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (Serial1.available())
    {
      int c = Serial1.read();
      //      Serial.print((char)c); 
      ++chars;
      if (gps.encode(c)) 
        newData = true;
    }
  }

  if (newData)
  {

    if (secondsToFirstLocation == 0) {
      secondsToFirstLocation = (millis() - startMillis) / 1000;
    }
    unsigned long age;
    gps.f_get_position(&latitude, &longitude, &age);

    latitude == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : latitude;
    longitude == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : longitude;

    //GPS
    int lati = latitude; 
    int longi = longitude;
    double beflati = round((latitude - lati) * 1000); // 10.5435-10 = 0.5435*1,000,000
    double beflongi = round((longitude - longi) * 1000);
    int beflatir = beflati;
    int beflongir = beflongi;
   
    sprintf(AllString, "The Weather today is:\nTemperature: %d *C\nHumidity: %d percent\nAir Pressure: %d Pa\nAltitude: %d meters\nSim Balance: %s\nLocation:\nhttp://maps.google.com/maps?q=loc:/@%d.%d,%d.%d\n", wea.temp, wea.humid, wea.pr, wea.alt, account,lati, beflatir, longi, beflongir);

  sms.SendSMS(phoneNo, AllString);
  Serial.println(F("\nSMS sent OK"));
    
  }
 
  
}

void weatherforecast()
{
  Weather wea;
  if (wea.temp <= avgtemp && wea.humid >= avghum ) {
    statuss = "Rain!";
  }
  else if (wea.temp > avgtemp && wea.humid > avghum) {
    statuss = "Sun!";
  }
  else if (wea.temp <= avgtemp && wea.humid <= avghum ) {
    statuss = "Cool!";
  }
  int n = statuss.length();
  char statu[n + 1];
  strcpy(statu, statuss.c_str());
  for (int i = 0; i < n; i++)
    sms.SendSMS(phoneNo, statu);
  Serial.println(F("\nSMS sent OK"));

  return true;
}

void caution()
{
  Weather wea;

  //send rain caution
  if (wea.temp > 80) {
    statuss = "Fire Caution!! Alert";
    int n = statuss.length();
    char statu[n + 1];
    strcpy(statu, statuss.c_str());
    for (int i = 0; i < n; i++)
      sms.SendSMS(phoneNo, statu);
  }

}

void gy87_measuring()
{
  Weather wea;

  accelgyro.getAcceleration(&AccelerationX, &AccelerationY, &AccelerationZ);
  accelgyro.getRotation(&RotationX, &RotationY, &RotationZ);

  const char WindString[300];
  sprintf(WindString, "The Wind today is : \n Accelarator: \n X/Y/Z = %d/%d/%d m/s \n \n Rotation: \n X/Y/Z= %d/%d/%d \n Air Pressure = %d Pa \n Altitude = %d meter\n ", AccelerationX, AccelerationY, AccelerationZ, RotationX, RotationY, RotationZ, wea.pr, wea.alt);
  Serial.println(WindString);
  sms.SendSMS(phoneNo, WindString);
  Serial.println(F("\nSMS sent OK"));
  delay(3000);

}
void readLocation() {
  bool newData = false;
  unsigned long chars = 0;
  unsigned short sentences, failed;
  const char GPSString[60];

  // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (Serial1.available())
    {
      int c = Serial1.read();
      //      Serial.print((char)c); // if you uncomment this you will see the raw data from the GPS
      ++chars;
      if (gps.encode(c)) // Did a new valid sentence come in?
        newData = true;
    }
  }

  if (newData)
  {
    // we have a location fix so output the lat / long and time to acquire
    if (secondsToFirstLocation == 0) {
      secondsToFirstLocation = (millis() - startMillis) / 1000;
    }
    unsigned long age;
    gps.f_get_position(&latitude, &longitude, &age);

    latitude == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : latitude;
    longitude == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : longitude;

    //GPS
    int lati = latitude; //10.5435 -> 10
    int longi = longitude;
    double beflati = round((latitude - lati) * 1000); // 10.5435-10 = 0.5435*1,000,000
    double beflongi = round((longitude - longi) * 1000);
    int beflatir = beflati;
    int beflongir = beflongi;

    sprintf(GPSString, "http://maps.google.com/maps?q=loc:/@%d.%d,%d.%d", lati, beflatir, longi, beflongir);
    //GPS-end

    sms.SendSMS(phoneNo, GPSString);
    Serial.println(F("\nSMS sent OK"));
  }

  if (chars == 0) {
    // if you haven't got any chars then likely a wiring issue
    Serial.println("Check wiring");
  }

}
void toSerial()
{
  while (Serial.available())
  {
    SerialAT.write(SerialAT.read());
  }
  while (SerialAT.available() != 0)
  {
    Serial.write(SerialAT.read());
  }
}
