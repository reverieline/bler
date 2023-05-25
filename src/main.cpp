#include <Arduino.h>
#include <BLEDevice.h>
#include <WiFi.h>
#include <WiFiManager.h>
#include <ESPAsyncWebServer.h>
#include <map>
#include "ble.h"

const String manidZhiJia = "9d22";
const std::map<const String,const String> commandsZhiJia = {
    {"bind","03482867815427f72344e52df4c0f63c5a4a5f85f69ca919"},
    {"on"/*a+b*/, "0db8dc6971a0d30329ba11d90030f6c2ae4a5f85f69ca919"},
    {"off","014c2a65855625f52742e72ff6c4f63a584a5f85f69ca919"},
};

#define COMMAND_STOP_DELAY 500

WiFiManager wm;
#define BTN_PIN 0

AsyncWebServer webServer(80);
const String webCommandPrefix = "/cmd/";

void startWebServer(){
  webServer.onNotFound([](AsyncWebServerRequest *req){
    req->send(404, "text/plain", "Not found");
  });

  for(auto i=commandsZhiJia.begin();i!=commandsZhiJia.end();i++){
    webServer.on(String(webCommandPrefix+i->first).c_str(),[](AsyncWebServerRequest *req){
      const String path=req->url();
      const String cmd=path.substring(webCommandPrefix.length());
      
      Serial.println("web request path: "+path);
      auto i=commandsZhiJia.find(cmd);
      if(i==commandsZhiJia.end()){
        Serial.println("command \""+cmd+"\" not found");
        req->send(404, "text/plain", "Not found");
      }else{
        Serial.println("start adv: manId:" + manidZhiJia + " data:" + i->second);
        try
        {
          startAdv(i->second, manidZhiJia);
        }
        catch (std::exception &e)
        {
          Serial.println(e.what());
        }
        
        delay(COMMAND_STOP_DELAY);
        delay(COMMAND_STOP_DELAY);
        stopAdv();
        Serial.println("stop adv");
        req->send(200, "text/plain", "Ok");
      }
    });
  }

  webServer.begin();
}

void setup()
{
  Serial.begin(9600);
  
  BLEDevice::init(_HOSTNAME);
  WiFi.mode(WIFI_STA);

  pinMode(BTN_PIN,INPUT);

  wm.setConfigPortalTimeout(60);
  wm.setConfigPortalBlocking(false);
  wm.setAPClientCheck(true);
  wm.setHostname(_HOSTNAME);
  wm.autoConnect(_AP_NAME,_AP_PASS);

  startWebServer();
}

void processSerial(void){
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
      Serial.println("start adv: manId:" + manidZhiJia + " data:" + i->second);

      try
      {
        startAdv(i->second, manidZhiJia);
      }
      catch (std::exception &e)
      {
        Serial.println(e.what());
      }
      
      delay(COMMAND_STOP_DELAY);
      stopAdv();
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
};


void loop()
{
  wm.process();
  
  //clear wifi settings on button press
  if(digitalRead(BTN_PIN)==LOW){
    delay(100);
    if(digitalRead(BTN_PIN)==LOW){
      wm.resetSettings();
      ESP.restart();
    }
  }

  //process comamnds on serial port
  processSerial();
}

