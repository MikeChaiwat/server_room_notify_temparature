
#include "DHT.h"
#include <TridentTD_LineNotify.h>


#define DHTPIN 2     // Digital pin connected to the DHT sensor
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 3 (on the right) of the sensor to GROUND (if your sensor has 3 pins)
// Connect pin 4 (on the right) of the sensor to GROUND and leave the pin 3 EMPTY (if your sensor has 4 pins)
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);

/************************|
 * ตั้งค่า Static IP ตรงนี้   |
 ************************/
// IPAddress local_IP(172, 17, 194, 163);
// IPAddress gateway(172, 17, 194, 254);
// IPAddress subnet(255, 255, 254, 0);
// IPAddress primaryDNS(172, 17, 193, 2);

unsigned long HoursToMilli(unsigned int hours);

unsigned long MinToMilli(unsigned int minute);

#define SSID        "Kasda 48AE KP322" // Kasda 48AE KP322
#define PASSWORD    "12345678"
#define LINE_TOKEN  "q9gA9KVGqjjVtaSH2hUBqhRZaAXPL4gPy09Esz0EkqE"

unsigned long period = MinToMilli(1); //ระยะเวลาที่ต้องการรอ
unsigned long last_time = 0; //ประกาศตัวแปรเป็น global เพื่อเก็บค่าไว้ไม่ให้ reset จากการวน loop

unsigned long normal_report_period = HoursToMilli(3);
unsigned long last_time_normal_report = 0;

unsigned long abnormal_report_period = MinToMilli(2);
unsigned long last_time_abnormal_report = 0;

unsigned long warning_report_period = MinToMilli(30);
unsigned long last_time_warning_report = 0;

unsigned long danger_report_period = MinToMilli(1);
unsigned long last_time_danger_report = 0;

float null = null;

void setup() {
  Serial.begin(115200);
  SetupDHT();

  Serial.println(LINE.getVersion());

  // WiFi.config(local_IP, primaryDNS, gateway, subnet);
 
  WiFi.begin(SSID, PASSWORD);
  Serial.printf("WiFi connecting to %s\n",  SSID);
  while(WiFi.status() != WL_CONNECTED) { Serial.print("."); delay(400); }
  Serial.printf("\nWiFi connected\nIP : ");
  Serial.println(WiFi.localIP()); 

  // กำหนด Line Token
  LINE.setToken(LINE_TOKEN);

  // First Notify
  float temp = DHTReadTemperature();
 
   if(isnan(temp)) {
      Serial.println("DHT ERROR");
   }else{
      if(StatusServerRoom(temp) == 1 ) {
      Serial.print(F("Temparature: "));
      Serial.print(temp);
      Serial.println(F("°C "));
      Serial.println(F("____________First Notify______________"));
      String linetxt = "ESP 32 Start...\nTemparature: " +
                String(temp) +
                "°C \n" +
               "Status: Normal";
      LINE.notify(linetxt);
    }else if(StatusServerRoom(temp) == 2 ) {
      Serial.print(F("Temparature: "));
      Serial.print(temp);
      Serial.println(F("°C "));
      Serial.println(F("____________First Notify______________"));
      String linetxt = "ESP 32 Start...\nTemparature: " +
                String(temp) +
                "°C \n" +
               "Status: Abnormal";
      LINE.notify(linetxt);
    }else if(StatusServerRoom(temp) == 3 ) {
      Serial.print(F("Temparature: "));
      Serial.print(temp);
      Serial.println(F("°C "));
      Serial.println(F("____________First Notify______________"));
      String linetxt = "ESP 32 Start...\nTemparature: " +
                String(temp) +
                "°C \n" +
               "Status: Warning";
      LINE.notify(linetxt);
    }else if(StatusServerRoom(temp) == 4 ) {
      Serial.print(F("Temparature: "));
      Serial.print(temp);
      Serial.println(F("°C "));
      Serial.println(F("____________First Notify______________"));
      String linetxt = "ESP 32 Start...\nTemparature: " +
                String(temp) +
                "°C \n" +
               "Status: Danger";
      LINE.notify(linetxt);
    }
   }
  
  
}

void loop() {
  // Wait a few seconds between measurements.
  if(millis() - last_time > period) {
    last_time = millis();
    float temp = DHTReadTemperature();
    Serial.print(F("Temparature: "));
    Serial.print(temp);
    Serial.println(F("°C "));
    Serial.println(F("___________interval measure______________"));
    if(isnan(temp)) {
       Serial.println("DHT ERROR");
      //ReportError();
    }
    else{
      if(StatusServerRoom(temp) == 1 ) ReportNormal(temp);
      else if(StatusServerRoom(temp) == 2 ) ReportAbnormal(temp);
      else if(StatusServerRoom(temp) == 3 ) ReportWarning(temp);
      else if(StatusServerRoom(temp) == 4 ) ReportDanger(temp);
    }
  }
}

void SetupDHT(){
  Serial.println(F("DHTxx test!"));
  dht.begin();
}

void ReportNormal(float temp){
  if(millis() - last_time_normal_report > normal_report_period){
    last_time_normal_report = millis();
    Serial.print(F("Temparature: "));
    Serial.print(temp);
    Serial.println(F("°C "));
    Serial.println(F("______________Line notify__________________"));

    String linetxt = "\nTemparature: " +
              String(temp) +
              "°C \n" +
             "Status: Normal";
    LINE.notify(linetxt);
  }
}

void ReportAbnormal(float temp){
  if(millis() - last_time_abnormal_report > abnormal_report_period){
    last_time_abnormal_report = millis();
    Serial.print(F("Temparature: "));
    Serial.print(temp);
    Serial.println(F("°C "));
    Serial.println(F("_____________Line notify_____________________"));

    String linetxt = "\nTemparature: " +
              String(temp) +
              "°C \n" +
             "Status: Abnormal";
    LINE.notify(linetxt);
  }
}

void ReportWarning(float temp){
  if(millis() - last_time_warning_report > warning_report_period){
    last_time_warning_report = millis();
    Serial.print(F("Temparature: "));
    Serial.print(temp);
    Serial.println(F("°C "));
    Serial.println(F("_____________Line notify____________________"));

    String linetxt = "\nTemparature: " +
              String(temp) +
              "°C \n" +
             "Status: Warning";
    LINE.notify(linetxt);
  }
}

void ReportDanger(float temp){
  if(millis() - last_time_danger_report > danger_report_period){
    last_time_danger_report = millis();
    Serial.print(F("Temparature: "));
    Serial.print(temp);
    Serial.println(F("°C "));
    Serial.println(F("_____________Line notify_______________________"));

    String linetxt = "\nTemparature: " +
              String(temp) +
              "°C \n" +
             "Status: Danger";
    LINE.notify(linetxt);
  }
}



float DHTReadTemperature(){
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)

  // Read temperature as Celsius (the default)
  // float temp = dht.readTemperature();
  float temp = 55;
  
  
  // Check if any reads failed and exit early (to try again).
  if (isnan(temp)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return null;
  }
  return temp;
}

int StatusServerRoom(float temp){
  if(temp > 0 && temp <= 30){
    return 1; // Status = Normal
  }else if(temp > 30 && temp <= 40){
    return 2; // Status = Abnormal
  }else if(temp > 40 && temp <= 50){
    return 3; // Status = Warning
  }else if(temp > 50){
    return 4; // Status = Danger
  }
  return 0;
}

unsigned long MinToMilli(unsigned int minute){
  return minute/0.001*60;
}

unsigned long HoursToMilli(unsigned int hours){
  return hours*3600000;
}

void SetTimeAlign(){
    last_time_normal_report = millis();
    last_time_abnormal_report = millis();
    last_time_warning_report = millis();
    last_time_danger_report = millis();
}
