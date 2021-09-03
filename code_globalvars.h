#include <LinkedList.h>
#include "math.h"
#include "Wire.h"
#include <stdint.h>

#define MUX_Address 0x70
bool Saunaswitch = false;

void tcaselect(uint8_t i2c_bus) {
  if (i2c_bus > 7) return;
  Wire.beginTransmission(MUX_Address);
  Wire.write(1 << i2c_bus);
  Wire.endTransmission();
}

class activeSensor {
public:
    int i2cport;
    int infoslot;
};

// env variable
float temperature_outside;
float temperature_inside;
float humidity;
bool waitAutoconnect;
float min_loggingTemp = 40.0;

int current_lcdSensor = 0;
bool showhumidity = false;

LinkedList2<Tempinfo> Sensorinfo;
LinkedList2<activeSensor> activeSensors;

void initialize_TempinfoList() {
  for (int i = 0; i < 6; i++) {
    Tempinfo temp;
    temp.temperature = 0.0;
    temp.humidity = 0.0;
    Sensorinfo.add(i, temp);
  }
}

Tempinfo getSensorData(int sensorID) {
  int activesensors_len = activeSensors.size();
  if (sensorID >= activesensors_len) {
    Serial.println("Sensor not found");
  }
  Tempinfo sensordata = Sensorinfo.get(sensorID);
  return sensordata;
}

float truncate(float val, byte dec)
{
  float x = val * pow(10, dec);
  float y = round(x);
  float z = x - y;
  if ((int)z == 5)
  {
    y++;
  } else {}
  x = y / pow(10, dec);
  return x;
}

