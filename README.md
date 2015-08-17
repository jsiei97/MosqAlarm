MosqAlarm
=========

If no updated for X time, then send a alarm

This application will do this:
* Subscribes to all topics on the MQTT server
* When a new message comes it will save the topic and a timestamp in a SQLite database
* Regurlaly check if the data is older than threshold, and send a alarm once.
* The alarm is sent to the same topic that had too old data.
