#include <Adafruit_SHT31.h>

Adafruit_SHT31 sht31 = Adafruit_SHT31();



void initialize_sensors() {
  for (int i = 7; i > 1; i--) {
    tcaselect(i);
    if (!sht31.begin()) {
        Serial.println("Couldn't find SHT31 on Slot: " + String(i));
    }
    else {
        Serial.println("Found SHT31 on slot: " + String(i));
        float t = sht31.readTemperature();
        if (!isnan(t)) { 
            activeSensor as;
            as.i2cport = i;
            as.infoslot = i - 2;
            activeSensors.add(as);
        }
        else {
            Serial.println("Couldn't read SHT31 on Slot: " + String(i));
        }
    }
  }
}

void read_sensor_data(activeSensor sensor) {
  tcaselect(sensor.i2cport);
  float t = sht31.readTemperature();
  float h = sht31.readHumidity();
  if (! isnan(t)) {  // check if 'is not a number'
   // Serial.print("Temp *C = "); Serial.println(t);
  }
  else {
    t = 0.0;
    Serial.println("Failed to read temperature");
  }

  if (! isnan(h)) {  // check if 'is not a number'
    //Serial.print("Hum. % = "); Serial.println(h);
  }
  else {
    h = 0.0;
    Serial.println("Failed to read humidity");
  }
  Tempinfo temp;
  temp.temperature = t;
  temp.humidity = h;
  //Serial.println("Temp: " + String(t));
  Sensorinfo.set(sensor.infoslot, temp);
}
