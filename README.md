# ESP8266 MQTT temperature monitoring
This is a simple MQTT based Arduino(ESP8266) code to publish temperature sensor data to the server. It continuously publishes temperature data to an MQTT topic and then gets back the proper action from the server. After that it will do the requested action and publishes an acknowledge back to another MQTT topic to be used by the server.

# Associated resources
- Introduction and review video: [youtu.be/zXzmXzBmWdY](https://youtu.be/zXzmXzBmWdY)
- Monitoring back-end code repo: [AyubIRZ/go-mqtt-monitoring-server](https://github.com/AyubIRZ/go-mqtt-monitoring-server)

## Notes
- This is a simple demonstration of the project. You may not use it in production without reviewing the code and changing it to the proper version of your use!
- Data transmission between clients is not secured with TLS or authentication! It's simple and dumb!
-  Any contribution, PR or submitting issues using github issue tracker will be appreciated!
