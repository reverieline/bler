#include <Arduino.h>
#include <BLEDevice.h>
#include <map>
#include "ble.h"

std::map<String, String> commandsZhiJia = {
    {"on", "fbb0d09f79acdf0fdbbb1dd50c3809c3a24a5f85f69ca919"},
    {"off", "054c2e61855221f12746e32bf2c4f63e5c4a5f85f69ca919"},
};

void setup()
{
  Serial.begin(9600);
  BLEDevice::init("BLER");
}

void loop()
{
  static String cmd;
  while (Serial.available())
  {
    char c = (char)Serial.read();
    Serial.write(c);
    cmd += c;
    auto nli = cmd.indexOf('\n');

    if (nli >= 0)
    {
      auto line = cmd.substring(0, nli);
      line.replace("\r", "");
      line.replace("\n", "");
      cmd = "";

      auto val = commandsZhiJia.find(line);
      if (val != commandsZhiJia.end())
      {
        String manId = "9d22";
        Serial.printf("start adv: ");
        Serial.println(" manId:" + manId + " data:" + val->second);

        try
        { // catch start_adv() exceptions
          start_adv(val->second, manId);
          delay(1000);
          stop_adv();
        }
        catch (std::exception &e)
        {
          Serial.println(e.what());
        }

        Serial.println("stop adv");
        continue;
      }

      if(line.indexOf("help")>=0){
        auto i=commandsZhiJia.begin();
        while(i!=commandsZhiJia.end()){
          Serial.println(i->first);
          i++;
        }
        continue;
      }

      Serial.println("unknown command");
    }
  }
}
