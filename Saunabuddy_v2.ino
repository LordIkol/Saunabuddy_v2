/*
 Name:		Saunabuddy_v2.ino
 Author:	Marc Rieken
*/


#include "class_Tempinfo.h"
#include "code_globalvars.h"
#include "code_SoftTimers.h"
#include "code_lcd.h"
#include "code_sensors.h"
#include "code_wifi.h"
#include "code_mesh.h"
#include "code_SD.h"
#include "code_webserver.h"
#include "code_Datalogger.h"


#define DEEP_SLEEP_TIME 15

void gotoDeepSleep() {
    Serial.println("Saunabuddy müde, Saunabuddy geht schlafen!!");

    /* esp_sleep_enable_timer_wakeup(DEEP_SLEEP_TIME * 1000000);
     esp_deep_sleep_start();*/
}

void setup() {
    Serial.begin(115200);
    Wire.begin();
    initialize_TempinfoList();
    waitAutoconnect = initialize_wifi();
    if (waitAutoconnect) {
        Serial.println("Connected to Wifi");
        initialize_SD();
        initialize_webserver();
    }
    else {
        Webserver_Listen.setTimeOutTime(1000);
        Webserver_Listen.reset();
    }
    //initialize_mesh();
    initialize_sensors();
    initialize_lcd();
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    //deleteFile(SD,"/2021/08.txt");
    Sensors_Update.setTimeOutTime(1000);
    Sensors_Update.reset();
    Lcd_Update.setTimeOutTime(2000);
    Lcd_Update.reset();
    Datalogger_Update.setTimeOutTime(10000);
    Datalogger_Update.reset();
}


void loop() {
    // put your main code here, to run repeatedly:
    if (Webserver_Listen.hasTimedOut())
    {
        Portal.handleClient();
        Webserver_Listen.reset();
    }

    if (Sensors_Update.hasTimedOut())
    {
        int activesensors_len = activeSensors.size();
        for (int i = 0; i < activesensors_len; i++) {
            read_sensor_data(activeSensors.get(i));
        }
        Sensors_Update.reset();
    }

    if (Lcd_Update.hasTimedOut())
    {
        if (current_lcdSensor == activeSensors.size()) {
            current_lcdSensor = 0;
        }

        int sensorinfoslot = activeSensors.get(current_lcdSensor).infoslot;
        lcd_showSensorInfo_Big(sensorinfoslot);
        current_lcdSensor++;
        Lcd_Update.reset();
    }

    if (Datalogger_Update.hasTimedOut()) {
        Datalogger_Log();
        Datalogger_Update.reset();
    }

    /* if (Check_SleepCondition.hasTimedOut())
     {
         if (Sensorinfo.get(0).temperature <= 40.0) {
             gotoDeepSleep();
         }
         Check_SleepCondition.reset();
     }*/






}
