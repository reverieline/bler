#include <Arduino.h>
#include <BLEDevice.h>
#include <map>
#include "ble.h"

const String manidZhiJia = "9d22";
const std::map<String, String> commandsZhiJia = {
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
    if(c!='\n'){
      cmd += c;
      continue;
    }

    auto line = cmd;
    cmd = "";
    
    line.replace("\r", "");
    
    auto i = commandsZhiJia.find(line);
    if (i != commandsZhiJia.end())
    {
      Serial.printf("start adv: ");
      Serial.println(" manId:" + manidZhiJia + " data:" + i->second);

      try
      {
        start_adv(i->second, manidZhiJia);
      }
      catch (std::exception &e)
      {
        Serial.println(e.what());
      }
      
      delay(1000);
      stop_adv();
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
