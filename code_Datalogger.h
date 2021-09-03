#include <time.h>

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;

StaticJsonDocument<250> logentry;
char logtext[1024];

void create_logentry(char dt[11], char ts[9], int s, float t, float h, int p) {
    logentry.clear();
    logentry["dt"] = dt;
    logentry["ts"] = ts;
    logentry["s"] = s;
    logentry["t"] = t;
    logentry["h"] = h;
    logentry["p"] = p;
    serializeJson(logentry, logtext);
}

class Timestamp {
  public:
    char Date[11];
    char Time[9];
    char Year[5];
    char Month[3];
};

Timestamp getLogTimestamp() {
  struct tm timeinfo;
  Timestamp ts;
  if(!getLocalTime(&timeinfo)){
      strncpy(ts.Date, "1900-01-01", 11);
      strncpy(ts.Time, "00:00:00", 9);
    Serial.println("Failed to obtain time");
    return ts;
  }

  char currentDate[11];
  strftime(currentDate,11, "%F", &timeinfo);
  char currentTime[9];
  strftime(currentTime,9, "%T", &timeinfo);
  char currentyear[5];
  strftime(currentyear, 5, "%G", &timeinfo);
  char currentMonth[3];
  strftime(currentMonth, 3, "%m", &timeinfo);

  strncpy(ts.Date, currentDate, 11);
  strncpy(ts.Time, currentTime, 9);
  strncpy(ts.Year, currentyear, 5);
  strncpy(ts.Month, currentMonth, 3);
  return ts;
}


void Datalogger_Log(){
    if (Sensorinfo.get(0).temperature <=  min_loggingTemp) {
        Timestamp ts = getLogTimestamp();
        create_logentry(ts.Date, ts.Time, 0, Sensorinfo.get(1).temperature, Sensorinfo.get(1).humidity, 0);
        String logfilepath =  + "/" + String(ts.Month) + ".txt";
        String logfolder = "/" + String(ts.Year);
        char clogfilepath[10];
        char clogfolder[6];
        logfilepath.toCharArray(clogfilepath,10);
        logfolder.toCharArray(clogfolder, 6);
        logToFile(SD, clogfolder,clogfilepath, logtext);
    }
}


