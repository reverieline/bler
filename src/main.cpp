#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEAdvertising.h>
#include <map>

std::map<String,String> commands={
  {"on","fbb0d09f79acdf0fdbbb1dd50c3809c3a24a5f85f69ca919"},
  {"off","054c2e61855221f12746e32bf2c4f63e5c4a5f85f69ca919"},
};

uint8_t mdata[]={
  0x22,0x9d,
  0x2d,0xb2,0xa0,0x49,0xab,0x7a,0x09,0xe0,0x96,0x6d,0xc9,0x03,0xbd,0xff,0x7d,0x8c,0x74,0x4a,0x5f,0x85,0xf6,0x9c,0xa9,0x19
};

esp_ble_adv_data_t adata=
{
    .set_scan_rsp = false,
    .include_name = false,
    .include_txpower = false,
    
    // .min_interval = 0x20,
    // .max_interval = 0x40,
    .min_interval = 0x00,
    .max_interval = 0x00,
    

    .appearance = 0x00,
    .manufacturer_len = sizeof(mdata),
    .p_manufacturer_data = mdata,
    .service_data_len = 0,
    .p_service_data = nullptr,
    .service_uuid_len = 0,
    .p_service_uuid = nullptr,
    .flag = (ESP_BLE_ADV_FLAG_GEN_DISC | ESP_BLE_ADV_FLAG_BREDR_NOT_SPT)
};

esp_ble_adv_params_t aparam = {
  .adv_int_min = 0x20,
  .adv_int_max = 0x40,

  .adv_type = ADV_TYPE_IND,
  .own_addr_type = BLE_ADDR_TYPE_RANDOM,
  .peer_addr = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},
  .peer_addr_type = BLE_ADDR_TYPE_RANDOM,
  .channel_map = ADV_CHNL_ALL,
  .adv_filter_policy = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY
};

esp_bd_addr_t rndaddr={
  0x3c,0xaa,0xbb,0xcc,0xdd,0xee
};

byte nibble(char c)
{
  if (c >= '0' && c <= '9')
    return c - '0';

  if (c >= 'a' && c <= 'f')
    return c - 'a' + 10;

  if (c >= 'A' && c <= 'F')
    return c - 'A' + 10;

  return 0;
}

void hexStringToBytes(byte *byteArray, const char *hexString)
{
  bool oddLength = strlen(hexString) & 1;

  byte currentByte = 0;
  byte byteIndex = 0;

  for (byte charIndex = 0; charIndex < strlen(hexString); charIndex++)
  {
    bool oddCharIndex = charIndex & 1;

    if (oddLength)
    {
      // If the length is odd
      if (oddCharIndex)
      {
        // odd characters go in high nibble
        currentByte = nibble(hexString[charIndex]) << 4;
      }
      else
      {
        // Even characters go into low nibble
        currentByte |= nibble(hexString[charIndex]);
        byteArray[byteIndex++] = currentByte;
        currentByte = 0;
      }
    }
    else
    {
      // If the length is even
      if (!oddCharIndex)
      {
        // Odd characters go into the high nibble
        currentByte = nibble(hexString[charIndex]) << 4;
      }
      else
      {
        // Odd characters go into low nibble
        currentByte |= nibble(hexString[charIndex]);
        byteArray[byteIndex++] = currentByte;
        currentByte = 0;
      }
    }
  }
}

void start_adv(String shex){
  const size_t size=shex.length()/2;
  byte *barray=new byte[size];

  hexStringToBytes(barray,shex.c_str());
  memcpy(mdata+2,barray,size);
  adata.manufacturer_len=2+size;

  delete barray;
  barray=nullptr;

  ::esp_ble_gap_config_local_privacy(true);
  ::esp_ble_gap_set_rand_addr(rndaddr);
  ::esp_ble_gap_config_adv_data(&adata);
  ::esp_ble_gap_start_advertising(&aparam);
}

void stop_adv(){
  ::esp_ble_gap_stop_advertising();
}

void setup(){
  Serial.begin(9600);
  BLEDevice::init("BLER");
}

void loop(){
  static String cmd;
  while(Serial.available()){
    char c=(char)Serial.read();
    Serial.write(c);
    cmd+=c;
    auto nli=cmd.indexOf('\n');
    if(nli>=0){
      auto line=cmd.substring(0,nli);
      line.replace("\r","");
      line.replace("\n","");
      cmd="";

      const String starts="start:";
      if(0==line.indexOf(starts)){
        auto shex=line.substring(starts.length());
        shex.replace(" ","");
        shex.replace(":","");
        Serial.println(shex);

        start_adv(shex);
        Serial.println("adv started");
        continue;
      }else if(0==line.indexOf("stop")){
        stop_adv();
        Serial.println("adv stopped");
        continue;
      }

      auto val=commands.find(line);
      if(val!=commands.end()){
        Serial.printf("start adv: ");
        Serial.println(val->second);
        start_adv(val->second);
        delay(1000);
        stop_adv();
        Serial.println("stop adv");

        continue;
      }

      Serial.println("unknown command");
    }
  }
}

