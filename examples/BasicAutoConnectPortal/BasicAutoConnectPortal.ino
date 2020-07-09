#include "WiFiConnect.h" 

#include <WiFiClient.h>                      
#ifdef ARDUINO_ARCH_ESP8266  
#include <ESP8266HTTPClient.h>
#else  
#include <HTTPClient.h>
#endif

WiFiConnect wc;

// For internet connection
WiFiClient client;
HTTPClient http;
 
void configModeCallback(WiFiConnect *mWiFiConnect) {
  Serial.println("Entering Access Point");
}


void startWiFi(boolean showParams = false) {
 
  wc.setDebug(true);
  
  /* Set our callbacks */
  wc.setAPCallback(configModeCallback);

  //wc.resetSettings(); //helper to remove the stored wifi connection, comment out after first upload and re upload

    /*
       AP_NONE = Continue executing code
       AP_LOOP = Trap in a continuous loop - Device is useless
       AP_RESET = Restart the chip
       AP_WAIT  = Trap in a continuous loop with captive portal until we have a working WiFi connection
    */
    if (!wc.autoConnect()) { // try to connect to wifi
      /* We could also use button etc. to trigger the portal on demand within main loop */
      wc.startConfigurationPortal(AP_WAIT);//if not connected show the configuration portal
    }
}


void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(100);
  }
  Serial.println("....");
  Serial.println("....");
  delay (5000);
  startWiFi();

}

void loop() {
    delay(100);
    Serial.println("Testing with connection to internet...");

    http.begin(client, "http://ftp.mirrorservice.org/sites/archive.ubuntu.com/ubuntu/");

    int httpCode = http.GET();
    
    if (httpCode > 0) 
    {    
      
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);
      
      // file found at server
      if (httpCode == HTTP_CODE_OK) 
      {
          Serial.println("HTTP_CODE_OK");
      }
      
    } else {
       Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
    
    http.end();  
  
    // Wifi Dies? Start Portal Again
    if (WiFi.status() != WL_CONNECTED) {
      if (!wc.autoConnect()) wc.startConfigurationPortal(AP_WAIT);
    }
  
  delay (20000);
}
