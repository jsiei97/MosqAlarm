
#include <QDebug>
#include "SQLiteWrapper.h"
#include "MosqConnect.h"

int main()
{
    qDebug() << "Qt SQLite test app";
    SQLiteWrapper lite;

    qDebug() << "id rum1:" << lite.getId("rum1");
    qDebug() << "id rum2:" << lite.getId("rum2");

    lite.updateTimestamp("rum1");
    lite.updateTimestamp("rum2");

    qDebug() << "id rum1:" << lite.getId("rum1");
    qDebug() << "id rum2:" << lite.getId("rum2");

       class MosqConnect *mqtt;
        int rc;

        mosqpp::lib_init();

        mqtt = new MosqConnect(
                "MosqAlarm",
                "mosqhub",
                1883
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
