/**
 * @file SQLiteWrapper.cpp
 * @author Johan Simonsson
 * @brief A SQLite wrapper
 */

/*
 * Copyright (C) 2014 Johan Simonsson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QDebug>
#include <QMutex>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QStringList>

#include "SQLiteWrapper.h"
#include "UnixTime.h"

#include "DebugDefines.h"

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

void SQLiteWrapper::init()
{
    myOut() << "SQLite init use db:" << filename;
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(filename);

    open();

    QStringList tableList = db.tables(QSql::Tables);
    if( tableList.size() == 0 )
    {
        myWarn() << "Warning empty db";

        QSqlQuery query;
        //mqtt topic, unix timestamp
        if( !query.exec("CREATE TABLE IF NOT EXISTS data \
                    ( \
                      id    INTEGER PRIMARY KEY AUTOINCREMENT, \
                      topic VARCHAR(256), \
                      time  INTEGER, \
                      alarm INTEGER DEFAULT 0 \
                    )")
          )
        {
            qFatal("Failed to create table!");
        }
    }

    close();
};

SQLiteWrapper::SQLiteWrapper()
{
    //filename = "MosqAlarm.db";
    filename = "/var/db/MosqAlarm.db";
    init();
};

SQLiteWrapper::SQLiteWrapper(QString file)
{
    filename = file;
    init();
};

QString SQLiteWrapper::getId(QString topic)
{
    //find id
    unsigned int id = 0;
    bool ok = false;

    QString str("SELECT id FROM data WHERE topic like :topic ORDER BY id DESC LIMIT 1");

    open();

    QSqlQuery q;
    q.prepare(str);
    q.bindValue(":topic",topic);
    q.exec();
    QSqlRecord rec = q.record();

    //myOut() << "Number of columns: " << rec.count();

    int nameCol = rec.indexOf("id"); // index of the field "name"
    //if size==1 ...
    while (q.next())
    {
        id = q.value(nameCol).toUInt(&ok);
        if(ok)
        {
            break;
        }
        //myOut() << q.value(nameCol).toString();
    }

    close();

    if(ok)
    {
        QString idS;
        idS.setNum(id);
        str.append(idS);
        //myOut() << idS;
        return idS;
    }
    else
    {
        myWarn() << "No id for topic" << topic;
        return NULL;
    }
}

void SQLiteWrapper::updateTimestamp(QString topic)
{
    QString id = getId(topic);

    open();

    if(id == NULL)
    {
        QString str("INSERT INTO data ( topic, time ) VALUES ('");
        str.append(topic);
        str.append("', '");
        str.append(UnixTime::toQString());
        str.append("')");
        //myOut() << str;

        QSqlQuery query;
        if( !query.exec(str) )
        {
            myErr() << "Error" << str;
            qFatal("Failed to insert");
        }
    }
    else
    {
        //update:
        //set current time
        //reset alarm values
        QString str("UPDATE data SET alarm=0, time='");
        str.append(UnixTime::toQString());
        str.append("' WHERE id='");
        str.append(id);
        str.append("'");
        //myOut() << str;

        QSqlQuery query;
        if( !query.exec(str) )
        {
            myErr() << "Error" << str;
            qFatal("Failed to update");
        }
    }

    close();
}

QStringList SQLiteWrapper::getOldTopics()
{
    QStringList list;

    unsigned int alarmLimit = 2*60*60; ///< Data older that Xs will trigger a alarm,

    unsigned int now  = UnixTime::get();
    unsigned int past = now-alarmLimit;

    //myOut() << now;
    //myOut() << past;

    QString str("SELECT id, topic FROM data WHERE time < :past AND alarm=0 ORDER BY topic");

    open();

    QSqlQuery q;
    q.prepare(str);

    QString strPast;
    strPast.setNum(past);
    q.bindValue(":past", strPast);

    q.exec();
    //myOut() << str;
    //myOut() << q.lastQuery();
    //myOut() << q.executedQuery();

    QSqlRecord rec = q.record();

    //myOut() << "Number of columns: " << rec.count();

    int colId    = rec.indexOf("id");    // index of the field "id"
    int colTopic = rec.indexOf("topic"); // index of the field "topic"
    while (q.next())
    {
        //myOut() << q.value(colId).toString() << q.value(colTopic).toString();
        myOut() << "Alarm data is old:" << alarmLimit << q.value(colTopic).toString();

        //id -> update alarm=1
        //topic -> list -> return

        QString str2("UPDATE data SET alarm=1 WHERE id=?");

        QSqlQuery query;
        query.prepare(str2);
        query.bindValue(0, q.value(colId).toString());

        //myOut() << str2;
        //myOut() << query.lastQuery();
        //myOut() << query.executedQuery();

        if( !query.exec() )
        {
            myErr() << "Error" << str2;
            qFatal("Failed to update");
        }
        list << q.value(colTopic).toString();
    }

    close();

    return list;
}
