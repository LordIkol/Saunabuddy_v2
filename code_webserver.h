#include <AsyncTCP.h>
#include <AsyncJson.h>
#include <ESPAsyncWebServer.h>
#include "index.h"



// Web server running on port 80
AsyncWebServer server(80);


// JSON data buffer
StaticJsonDocument<250> jsonDocument;
char buffer[1024];

void create_json(int sensorID, String tag, float value, String unit) {
  jsonDocument.clear();
  jsonDocument["sensorID"] = sensorID;
  jsonDocument["type"] = tag;
  jsonDocument["value"] = value;
  jsonDocument["unit"] = unit;
  serializeJson(jsonDocument, buffer);
}

void add_json_object(int sensorID, String tag, float value, String unit) {
  JsonObject obj = jsonDocument.createNestedObject();
  obj["sensorID"] = sensorID;
  obj["type"] = tag;
  obj["value"] = value;
  obj["unit"] = unit;
}

void setup_routing() {

  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    //String s = MAIN_page; //Read HTML contents
    request->send(SD, "/Website/index.html","text/html"); //Send web page
  });

  server.on("/switch", HTTP_GET, [](AsyncWebServerRequest* request) {
      broadcast();
      Saunaswitch = !Saunaswitch;
      request->send(200, "OK", "text/html"); //Send web page
  });


  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest * request) {
    int sensor;
    if (request->hasParam("sensor")) {
      sensor = request->getParam("sensor")->value().toInt();
    }
    else {
      request->send(200, "text/html", "No Sensor Parameter found");
    }
    int sensorID = activeSensors.get(sensor).infoslot;
    Tempinfo sensordata = Sensorinfo.get(sensorID);
    create_json(sensor, "Temperature", truncate(sensordata.temperature, 1), "°C");
    request->send(200, "application/json", buffer);
  });

  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest * request) {
    int sensor;
    if (request->hasParam("sensor")) {
      sensor = request->getParam("sensor")->value().toInt();
    }
    else {
      request->send(200, "text/html", "No Sensor Parameter found");
    }
    int sensorID = activeSensors.get(sensor).infoslot;
    Tempinfo sensordata = Sensorinfo.get(sensorID);
    create_json(sensor, "humidity", sensordata.humidity, "%");
    request->send(200, "application/json", buffer);
  });
  server.on("/env", HTTP_GET, [](AsyncWebServerRequest * request) {
    int sensor;
    if (request->hasParam("sensor")) {
      sensor = request->getParam("sensor")->value().toInt();
    }
    else {
      request->send(200, "text/html", "No Sensor Parameter found");
    }
    int sensorID = activeSensors.get(sensor).infoslot;
    Tempinfo sensordata = Sensorinfo.get(sensorID);
    jsonDocument.clear();
    add_json_object(sensor, "temperature", truncate(sensordata.temperature, 1), "°C");
    add_json_object(sensor, "humidity", truncate(sensordata.humidity, 1), "%");
    serializeJson(jsonDocument, buffer);
    request->send(200, "application/json", buffer);
  });

  server.on("/tempdata-month", HTTP_GET, [](AsyncWebServerRequest * request) {
      String month;
      String year;
      if (request->hasParam("month") && request->hasParam("year")) {
          month = request->getParam("month")->value();
          year = request->getParam("year")->value();
          String path = "/" + year + "/" + month + ".txt";
          request->send(SD, path, "text/html");
      }
      else {
          request->send(200, "text/html", "Please pass month and Year Parameters");
      }
  });

  server.on("/tempdata-year", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SD, "/yeardata.txt", "text/html");
  });

  server.on("/tempdata-crazy", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SD, "/bigdata.txt", "text/html");
  });

  server.serveStatic("/", SD, "/");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
}

void initialize_webserver() {
  setup_routing();
  server.begin();
}
