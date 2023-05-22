#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEAdvertising.h>
#include <exception>

#define MAX_MDATA_SIZE 26
uint8_t mdata[MAX_MDATA_SIZE];

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

void startAdv(String mData,String mId){
  if(mData.length()>(MAX_MDATA_SIZE-2)*2){
    throw std::runtime_error("Data packet is too large");
  }
  if(mId.length()!=2*2){
    throw std::runtime_error("Manufacturer id size must be 2 bytes");
  }

  //reverse mId byte order
  String mIdLE=mId.substring(2);
  mIdLE+=mId.substring(0,2);
  hexStringToBytes(mdata,mIdLE.c_str());

  const size_t dataSize=mData.length()/2;
  hexStringToBytes(mdata+2,mData.c_str());
  adata.manufacturer_len=2+dataSize;

  ::esp_ble_gap_config_local_privacy(true);
  ::esp_ble_gap_set_rand_addr(rndaddr);
  ::esp_ble_gap_config_adv_data(&adata);
  ::esp_ble_gap_start_advertising(&aparam);
}

void stopAdv(){
  ::esp_ble_gap_stop_advertising();
}

