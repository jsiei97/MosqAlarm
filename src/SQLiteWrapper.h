#ifndef  __SQLITEWRAPPER_H
#define  __SQLITEWRAPPER_H

#include <QSqlDatabase>

class SQLiteWrapper
{
    private:
        QString filename;
        QSqlDatabase db;

        void open();
        void close();

    public:
        SQLiteWrapper();
        //SQLiteWrapper()~;

        QString getId(QString topic);
        void updateTimestamp(QString topic);

};

#endif  // __SQLITEWRAPPER_H 
