#include <unistd.h>

#include <QDebug>
#include <QStringList>

#include "SQLiteWrapper.h"
#include "MosqConnect.h"

int main()
{
    qDebug() << "MQTT old data check app";
    SQLiteWrapper lite;

    class MosqConnect *mqtt;
    int rc;

    mosqpp::lib_init();

    mqtt = new MosqConnect(
            "MosqAlarm",
            "mosqhub",
            1883,
            &lite
            );

    while(1)
    {
        rc = mqtt->loop();
        if(rc)
        {
            mqtt->reconnect();
        }

        QStringList list = lite.getOldTopics();
        for (int i = 0; i < list.size(); ++i)
        {
            //qDebug() << list.at(i);
            mqtt->pub(list.at(i), "Alarm: MosqAlarm: No new data");
        }
    }
    mosqpp::lib_cleanup();
}
