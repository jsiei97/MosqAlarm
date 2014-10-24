
#include <QDebug>
#include "SQLiteWrapper.h"
#include "MosqConnect.h"

int main()
{
    qDebug() << "Qt SQLite test app";
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
    }
    mosqpp::lib_cleanup();
}
