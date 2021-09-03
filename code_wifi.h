#include <WiFi.h>
#include "website_ap.h"
#include <WebServer.h>
#include <AutoConnect.h>
#include <ESPmDNS.h>

WebServer autoconnectServer(80);
AutoConnect Portal(autoconnectServer);

bool initialize_wifi() {
  Serial.println("Initialize wifi");
  AutoConnectConfig  Config;
  //Config.immediateStart = true;
  Config.hostName = "SaunaBuddy";
  Portal.config(Config);
  if (Portal.begin()) {
    Serial.println("WiFi connected: " + WiFi.localIP().toString());
    if (MDNS.begin("SaunaBuddy")) {
      MDNS.addService("http", "tcp", 80);
    }
    autoconnectServer.stop();
    return true;
  }
  else {
    return false;
  } 
}
