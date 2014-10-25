
#include <QDebug>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QStringList>
#include <QMutex>

#include "UnixTime.h"
#include "SQLiteWrapper.h"


void SQLiteWrapper::open()
{
    mutex.lock();
    //Please note that if there is no file, it will be created!
    if( !db.open() )
    {
        qFatal("Failed to open db file!");
    }
}

void SQLiteWrapper::close()
{
    db.close();
    mutex.unlock();
}

SQLiteWrapper::SQLiteWrapper()
{
    //qDebug() << "Qt SQLite test app";
    filename = "test.db";
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(filename);

    open();

    QStringList tableList = db.tables(QSql::Tables);
    if( tableList.size() == 0 )
    {
        qDebug() << "Warning empty db";

        QSqlQuery query;
        //mqtt topic, unix timestamp
        if( !query.exec("CREATE TABLE IF NOT EXISTS data \
                    ( \
                      id    INTEGER PRIMARY KEY AUTOINCREMENT, \
                      topic VARCHAR(256), \
                      time  INTEGER \
                    )")
          )
        {
            qFatal("Failed to create table!");
        }
    }

    close();
};

QString SQLiteWrapper::getId(QString topic)
{
    open();

    //find id
    unsigned int id = 0;
    bool ok = false;

    QString str("SELECT id FROM data WHERE topic like '");
    str.append(topic);
    str.append("' LIMIT 1");

    QSqlQuery q(str);
    QSqlRecord rec = q.record();

    //qDebug() << "Number of columns: " << rec.count();

    int nameCol = rec.indexOf("id"); // index of the field "name"
    //if size==1 ...
    while (q.next())
    {
        id = q.value(nameCol).toUInt(&ok);
        if(ok)
        {
            break;
        }

        //qDebug() << q.value(nameCol).toString(); // output all names
    }

    close();

    if(ok)
    {
        QString idS;
        idS.setNum(id);
        str.append(idS);
        return idS;
    }
    else
    {
        return NULL;
    }
}

void SQLiteWrapper::updateTimestamp(QString topic)
{
    //qDebug() << UnixTime::get();

    //"select id from data where topic='rum1';"
    QString id = getId(topic);

    open();
    if(id == NULL)
    {

        QString str("INSERT INTO data ( topic, time ) VALUES ('");
        str.append(topic);
        str.append("', '");
        str.append(UnixTime::toQString());
        str.append("')");
        qDebug() << str;

        QSqlQuery query;
        if( !query.exec(str) )
        {
            qDebug() << "Error" << str;
            qFatal("Failed to insert");
        }
    }
    else
    {
        //update
        QString str("UPDATE data SET time='");
        str.append(UnixTime::toQString());
        str.append("' WHERE id='");
        str.append(id);
        str.append("'");
        qDebug() << str;

        QSqlQuery query;
        if( !query.exec(str) )
        {
            qDebug() << "Error" << str;
            qFatal("Failed to update");
        }
    }

    close();
}
