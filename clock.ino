#include <WiFi.h>
#include <time.h>
#include <ST7032.h>
#include <Wire.h>
#define JST     3600* 9
#define PSW1      13
#define PSW2      14
#define PSW3      26
#define LED1      16
#define LED2      17
#define SDA       21
#define SCL       22
#define SERVO     25
#define INT       27
#define SOLENOID  32
#define PUMP      33
#define PRESS     35
ST7032 lcd;
const char* ssid = "ASUS";
const char* password = "fumi1130";

void setup() {
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(PSW1, INPUT_PULLUP);
  pinMode(PSW2, INPUT_PULLUP);
  pinMode(PSW3, INPUT_PULLUP);
  pinMode(SOLENOID, OUTPUT);
  pinMode(PUMP, OUTPUT);
  Wire.begin(21, 22, 400000); // SDA (21), SCL (22) on ESP32, 400 kHz rate
  Serial.begin(115200);
  delay(100);
  Serial.print("\n\nStart\n");
  lcd.begin(8, 2);
  lcd.setContrast(30);
  lcd.println("Connect ");
  lcd.setCursor(0,1);
  lcd.println("Init NTP");
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(500);
    digitalWrite(LED1,HIGH);
    digitalWrite(LED2,LOW);
  }
  
  
  Serial.println();
  Serial.printf("Connected, IP address: ");
  Serial.println(WiFi.localIP());
  
  configTime( JST, 0, "ntp.nict.jp", "ntp.jst.mfeed.ad.jp");

  digitalWrite(LED1,LOW);
  digitalWrite(LED2,HIGH);
}

void loop() {
  time_t t;
  struct tm *tm;
  static const char *wd[7] = {"Sun","Mon","Tue","Wed","Thr","Fri","Sat"};
  char s[20];
  struct tm timeInfo;
  getLocalTime(&timeInfo);
  if(timeInfo.tm_hour<12) {
    sprintf(s, "AM %02d/%02d",
            timeInfo.tm_mon + 1, timeInfo.tm_mday);
  }
  else{
    sprintf(s, "PM %02d/%02d",
            timeInfo.tm_mon + 1, timeInfo.tm_mday);
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(s);
  lcd.setCursor(0,1);
  sprintf(s, "%02d:%02d:%02d",
          timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec);
  lcd.print(s);
  if(timeInfo.tm_sec%2==0) digitalWrite(LED2,LOW);
  else digitalWrite(LED2,HIGH);
  delay(1000);
  
}
