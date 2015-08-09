MosqAlarm
=========

If no updated for X time, then send a alarm

This application will do this:
* Subscribes to all topics on the MQTT server
* When a new message comes it will save the topic and a timestamp in a SQLite database
* Regurlaly check if the data is older than threshold, and send a alarm once.
* The alarm is sent to the same topic that had to old data.


Build and install
=================

Build with
```
qmake
make -j
```

Create the .deb
```
./createdeb.sh
```

And then your package can be inspected and installed with
```
dpkg --info MosqAlarm_*.deb
dpkg --contents MosqAlarm_*.deb
```

If all is ok, then install
```
sudo dpkg -i MosqAlarm_*.deb
```

