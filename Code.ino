
#define BLYNK_TEMPLATE_ID "TMPL3o-C2oO-o" //YOUR BLYNK_TEMPLATE_ID 
#define BLYNK_DEVICE_NAME "GPS Module"   //YOUR BLYNK_DEVICE_NAME
#define BLYNK_AUTH_TOKEN "T7Skc_8VRK10MkSTiPUtFwWPAIRH89mJ"  //YOUR BLYNK_AUTH_TOKEN

char auth[] = BLYNK_AUTH_TOKEN;
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "*****"; //YOUR HOTSPOT NAME
char pass[] = "******"; //YOUR HOTSPOT PASSWORD

#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

//GPS Module Settings
//GPS Module RX pin to ESP32 17
//GPS Module TX pin to ESP32 16
#include <TinyGPSPlus.h> 
#define RXD2 16
#define TXD2 17
HardwareSerial neogps(2);
TinyGPSPlus gps;

BlynkTimer timer;

#define INTERVAL 1000L

void sendGps()
{
  while(neogps.available())
  {
    if (gps.encode(neogps.read()))
    {
      break;
    }
  }
  if (!gps.location.isValid())
  {
    Serial.println("Failed to read True location from GPS Module!");
    return;
  }
  
  //get latitude and longitude
  float latitude = gps.location.lat();
  float longitude = gps.location.lng();
  float speed = gps.speed.kmph();
  
  Serial.print("Latitude:  ");
  Serial.println(latitude, 6);
  Serial.print("Longitude: ");
  Serial.println(longitude, 6);
  Serial.print("Speed: ");
  Serial.println(speed, 6);
  
  Blynk.virtualWrite(V1, String(latitude, 6));
  Blynk.virtualWrite(V2, String(longitude, 6));
  Blynk.virtualWrite(V0, String(speed));
 
}

void setup()
{
 
  Serial.begin(115200);

  Blynk.begin(auth, ssid, pass);
 
  neogps.begin(9600, SERIAL_8N1, RXD2, TXD2);
  Serial.println("neogps serial initialize");
  delay(10);
  
  timer.setInterval(INTERVAL, sendGps);
 
}

void loop(){
  Blynk.run();
  timer.run();
}
