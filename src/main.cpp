/**
 * @file main.cpp
 * @author Johan Simonsson
 * @brief Main for the MosqAlarm program
 */

/*
 * Copyright (C) 2015 Johan Simonsson
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


#include <unistd.h>

#include <QDebug>
#include <QStringList>

#include "SQLiteWrapper.h"
#include "MosqConnect.h"

/// @todo args path to MosqAlarm.db
int main()
{
    qDebug() << "MQTT old data check app";
    SQLiteWrapper lite;
    /// @todo call SQLiteWrapper with filename as args

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
