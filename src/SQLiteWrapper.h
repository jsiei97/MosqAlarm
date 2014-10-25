#ifndef  __SQLITEWRAPPER_H
#define  __SQLITEWRAPPER_H

#include <QSqlDatabase>
#include <QMutex>

class SQLiteWrapper
{
    private:
        QString filename;
        QSqlDatabase db;
        QMutex mutex;

        void open();
        void close();

    public:
        SQLiteWrapper();
        //SQLiteWrapper()~;

        QString getId(QString topic);
        void updateTimestamp(QString topic);
        QStringList getOldTopics();
};

#endif  // __SQLITEWRAPPER_H 
