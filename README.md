# Smart Scale

**Project originally made by [davidevertuani](https://github.com/davidevertuani/ScreenlessGFitSmartScale), I am working on updating the code and using an e-ink display instead of the LEDs**

# Required Libs

* [ESP8266 for Arduino](https://github.com/esp8266/Arduino)
* [HX711](https://github.com/bogde/HX711)
* [ArduinoJson](https://github.com/bblanchon/ArduinoJson)

# Compiling

To build with VS code ensure all files are added to "forcedInclude" in c_cpp_properties.json
```javascript
{
    "configurations": [
        {
            "forcedInclude": [
                "${workspaceFolder}/SmartScale/Google_Messages.ino",
                "${workspaceFolder}/SmartScale/GoogleFit.ino",
                "${workspaceFolder}/SmartScale/Scale.ino",
                "${workspaceFolder}/SmartScale/User.ino"
            ]
        }
    ],
    "version": 4
}
```
