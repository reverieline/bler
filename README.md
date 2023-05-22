# BLER
ESP32 BLE remote control for ZhiJia (ZhiGuang) LED controllers.
# Usage
1. Flash firmware  to ESP32 devboard.
3. Connect to BLER_AP WiFi access point and configure home network connection.
4. Send GET requests to HTTP command endpoint: http://\<device ip\>/cmd/\<command name\>
    
    For example, direct your browser to:
    - http://bler.lan/cmd/on
    - http://bler.lan/cmd/off
       
