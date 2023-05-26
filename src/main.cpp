#include <Arduino.h>
#include <BLEDevice.h>
#include <WiFi.h>
#include <WiFiManager.h>
#include <ESPAsyncWebServer.h>
#include <map>
#include "ble.h"

const String manidZhiJia = "9d22";
const std::map<const String,const String> commandsZhiJia = {
    {"bind0","03482867815427f72344e52df4c0f63c5a4a5f85f69ca919"},
    {"bind1","01362a6581565bf52346e72ff6c0f63e584a5f85f69ca919"},
    {"bind2","07352c6381505ef32340e129f0c0f6385e4a5f85f69ca919"},
    {"bind3","05342e6181525df12342e32bf2c0f63a5c4a5f85f69ca919"},
    {"bind4","0b33206f815c54ff234ced25fcc0f634524a5f85f69ca919"},
    {"bind5","0932226d815e57fd234eef27fec0f636504a5f85f69ca919"},
    {"bind6","0f31246b815852fb2348e921f8c0f630564a5f85f69ca919"},
    {"bind7","0d302669815a51f9234aeb23fac0f632544a5f85f69ca919"},
    {"bind8","133f3877814440e72354f53de4c0f62c4a4a5f85f69ca919"},
    {"bind9","113e3a75814643e52356f73fe6c0f62e484a5f85f69ca919"},

    {"on0","efb0c48b79b8cb1bdbaf09c1183809d7b64a5f85f69ca919"},
    {"on1","edcec68979bab719dbad0bc31a3809d5b44a5f85f69ca919"},
    {"on2","e3cdc88779b4ba17dba305cd143809dbba4a5f85f69ca919"},
    {"on3","e1ccca8579b6b915dba107cf163809d9b84a5f85f69ca919"},
    {"on4","e7cbcc8379b0b813dba701c9103809dfbe4a5f85f69ca919"},
    {"on5","e5cace8179b2bb11dba503cb123809ddbc4a5f85f69ca919"},
    {"on6","dbc9f0bf798c862fdb9b3df52c3809e3824a5f85f69ca919"},
    {"on7","d9c8f2bd798e852ddb993ff72e3809e1804a5f85f69ca919"},
    {"on8","dfc7f4bb79888c2bdb9f39f1283809e7864a5f85f69ca919"},
    {"on9","ddc6f6b9798a8f29db9d3bf32a3809e5844a5f85f69ca919"},

    {"off0","2f4c044b85780bdb276cc901d8c4f614764a5f85f69ca919"},
    {"off1","2d320649857a77d9276ecb03dac4f616744a5f85f69ca919"},
    {"off2","2b31004f857c72df2768cd05dcc4f610724a5f85f69ca919"},
    {"off3","2930024d857e71dd276acf07dec4f612704a5f85f69ca919"},
    {"off4","37371c53856068c32774d119c0c4f60c6e4a5f85f69ca919"},
    {"off5","35361e5185626bc12776d31bc2c4f60e6c4a5f85f69ca919"},
    {"off6","3335185785646ec72770d51dc4c4f6086a4a5f85f69ca919"},
    {"off7","31341a5585666dc52772d71fc6c4f60a684a5f85f69ca919"},
    {"off8","3f3b145b85686ccb277cd911c8c4f604664a5f85f69ca919"},
    {"off9","3d3a1659856a6fc9277edb13cac4f606644a5f85f69ca919"},

    {"a0","cf42e4517198eb3b298229e13830f6fa964a5f85f69ca919"},
    {"a1","cd3ce653719a973929802be33a30f6f8944a5f85f69ca919"},
    {"a2","b33f982d71e4ea4729fe559d4430f686ea4a5f85f69ca919"},
    {"a3","b13e9a2f71e6e94529fc579f4630f684e84a5f85f69ca919"},
    {"a4","b7399c2971e0e84329fa51994030f682ee4a5f85f69ca919"},
    {"a5","b5389e2b71e2eb4129f8539b4230f680ec4a5f85f69ca919"},
    {"a6","bb3b902571ece64f29f65d954c30f68ee24a5f85f69ca919"},
    {"a7","b93a922771eee54d29f45f974e30f68ce04a5f85f69ca919"},
    {"a8","bf35942171e8ec4b29f259914830f68ae64a5f85f69ca919"},
    {"a9","bd34962371eaef4929f05b934a30f688e44a5f85f69ca919"},

    {"b0","a3b8723d8b0e7dad2914bf77aecaf66c004a5f85f69ca919"},
    {"b1","a1c6703f8b0c01af2916bd75accaf66e024a5f85f69ca919"},
    {"b2","a7c576398b0a04a92910bb73aacaf668044a5f85f69ca919"},
    {"b3","a5c4743b8b0807ab2912b971a8caf66a064a5f85f69ca919"},
    {"b4","abc37a358b060ea5291cb77fa6caf664084a5f85f69ca919"},
    {"b5","a9c278378b040da7291eb57da4caf6660a4a5f85f69ca919"},
    {"b6","afc17e318b0208a12918b37ba2caf6600c4a5f85f69ca919"},
    {"b7","adc07c338b000ba3291ab179a0caf6620e4a5f85f69ca919"},
    {"b8","93cf420d8b3e3a9d29248f479ecaf65c304a5f85f69ca919"},
    {"b9","91ce400f8b3c399f29268d459ccaf65e324a5f85f69ca919"},

    {"ab0","6db8bc0971c0b36329da71b96030f6a2ce4a5f85f69ca919"},
    {"ab1","6fc6be0b71c2cf6129d873bb6230f6a0cc4a5f85f69ca919"},
    {"ab2","61c5b00571ccc26f29d67db56c30f6aec24a5f85f69ca919"},
    {"ab3","63c4b20771cec16d29d47fb76e30f6acc04a5f85f69ca919"},
    {"ab4","65c3b40171c8c06b29d279b16830f6aac64a5f85f69ca919"},
    {"ab5","67c2b60371cac36929d07bb36a30f6a8c44a5f85f69ca919"},
    {"ab6","79c1a81d71d4de7729ce65ad7430f6b6da4a5f85f69ca919"},
    {"ab7","7bc0aa1f71d6dd7529cc67af7630f6b4d84a5f85f69ca919"},
    {"ab8","7dcfac1971d0d47329ca61a97030f6b2de4a5f85f69ca919"},
    {"ab9","7fceae1b71d2d77129c863ab7230f6b0dc4a5f85f69ca919"},

    {"night0","7e544c1a9d3043932625814990dcf65d3e4a5f85f69ca919"},
    {"night1","7c2a4e189d323f912627834b92dcf65f3c4a5f85f69ca919"},
    {"night2","7a29481e9d343a972621854d94dcf6593a4a5f85f69ca919"},
    {"night3","78284a1c9d3639952623874f96dcf65b384a5f85f69ca919"},
    {"night4","062f34629d4840eb265df931e8dcf625464a5f85f69ca919"},
    {"night5","042e36609d4a43e9265ffb33eadcf627444a5f85f69ca919"},
    {"night6","022d30669d4c46ef2659fd35ecdcf621424a5f85f69ca919"},
    {"night7","002c32649d4e45ed265bff37eedcf623404a5f85f69ca919"},
    {"night8","0e233c6a9d4044e32655f139e0dcf62d4e4a5f85f69ca919"},
    {"night9","0c223e689d4247e12657f33be2dcf62f4c4a5f85f69ca919"},

    {"fon0","fdbad69973aad909d1b71bd30a3209cfa44a5f85f69ca919"},
    {"foff0","1f5a347b93483beb314af931e8d2f632464a5f85f69ca919"},
    {"fon1","e1c4ca8573b6bb15d1ab07cf163209d3b84a5f85f69ca919"},
    {"foff1","03242867935459f73156e52df4d2f62e5a4a5f85f69ca919"},
    {"fon2","e5c7ce8173b2bc11d1af03cb123209d7bc4a5f85f69ca919"},
    {"foff2","07272c6393505ef33152e129f0d2f62a5e4a5f85f69ca919"},
    {"fon3","e9c6c28d73beb11dd1a30fc71e3209dbb04a5f85f69ca919"},
    {"foff3","0b26206f935c53ff315eed25fcd2f626524a5f85f69ca919"},
    {"fon4","edc1c68973bab219d1a70bc31a3209dfb44a5f85f69ca919"},
    {"foff4","0f21246b935850fb315ae921f8d2f622564a5f85f69ca919"},
    {"fon5","d1c0fab573868f25d19b37ff263209e3884a5f85f69ca919"},
    {"foff5","3320185793646dc73166d51dc4d2f61e6a4a5f85f69ca919"},
    {"fon6","d5c3feb173828821d19f33fb223209e78c4a5f85f69ca919"},
    {"foff6","37231c5393606ac33162d119c0d2f61a6e4a5f85f69ca919"},
    {"fon7","d9c2f2bd738e852dd1933ff72e3209eb804a5f85f69ca919"},
    {"foff7","3b22105f936c67cf316edd15ccd2f616624a5f85f69ca919"},
    {"fon8","ddcdf6b9738a8e29d1973bf32a3209ef844a5f85f69ca919"},
    {"foff8","3f2d145b93686ccb316ad911c8d2f612664a5f85f69ca919"},
    {"fon9","c1cceaa573969335d18b27ef363209f3984a5f85f69ca919"},
    {"foff9","232c0847937471d73176c50dd4d2f60e7a4a5f85f69ca919"},

    {"unbind0","c9b6e2ad7f9eed3ddd8f2fe73e3e09f7904a5f85f69ca919"},
    {"unbind1","cbc8e0af7f9c913fdd8d2de53c3e09f5924a5f85f69ca919"},
    {"unbind2","c5cbeea17f929c31dd8323eb323e09fb9c4a5f85f69ca919"},
    {"unbind3","c7caeca37f909f33dd8121e9303e09f99e4a5f85f69ca919"},
    {"unbind4","c1cdeaa57f969e35dd8727ef363e09ff984a5f85f69ca919"},
    {"unbind5","c3cce8a77f949d37dd8525ed343e09fd9a4a5f85f69ca919"},
    {"unbind6","bdcf96d97feae049ddfb5b934a3e0983e44a5f85f69ca919"},
    {"unbind7","bfce94db7fe8e34bddf95991483e0981e64a5f85f69ca919"},
    {"unbind8","b9c192dd7feeea4dddff5f974e3e0987e04a5f85f69ca919"},
    {"unbind9","bbc090df7fece94fddfd5d954c3e0985e24a5f85f69ca919"},
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
        req->send(200, "text/plain", i->first);
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

