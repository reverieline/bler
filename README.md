# BLER
ESP32 BLE remote control for ZhiJia (ZhiGuang) LED controllers.
# Usage
1. Flash firmware  to ESP32 devboard.
3. Connect to BLER_AP WiFi access point (password: 12345678) and configure home network connection.
4. Send GET requests to HTTP command endpoint: http://\<device ip\>/cmd/\<command name\>\<room id: 0..9\>
    
# Binding
To bind device to room 1:
- Power off device.
- Direct your browser to http://bler.lan/cmd/bind1 .
- Simultaneously power on device.
The device should blink several times to indicate successful binding.
       
# Unbinding
To unbind device use the same steps s for binding:
- Power off device.
- Direct your browser to http://bler.lan/cmd/unbind1 .
- Simultaneously power on device.
The device should blink several times to indicate successful binding.

# Room 0
To send command to all devices, use room id 0. For example, to put all light on, send GET request to http://bler.lan/cmd/on0

# Available commands
- bind - bind device to remote, to specific group (room)
- unbind
- on
- off
- a - put first LED channel to max brightness and second channel to zero
- b - put second LED channel to max brightness and first channel to zero
- ab - both LED cahnnels to max brightness
- night
