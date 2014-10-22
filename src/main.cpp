
#include <QDebug>
#include "SQLiteWrapper.h"

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

}
