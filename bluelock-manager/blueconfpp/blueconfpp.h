/*
 *  Copyright (C) 2017 Çağrı Ulaş <cagriulas@gmail.com>
 *  Copyright (C) 2017 Canberk Koç <canberkkoc@gmail.com>
 *
 *  This file is part of bluelock-manager.
 *
 *  bluelock-manager is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  bluelock-manager is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with bluelock-manager.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef BLUECONFPP_H
#define BLUECONFPP_H

#include <QObject>
#include <QLibrary>
#include <QDebug>
#include <QMap>
#include <QFile>

#include <customwidgets/bluetoothitemwidget.h>
#include <bluetoothitem.h>

extern "C" {
    #include <structures.h>
}

class BlueConfPP : public QObject
{
    Q_OBJECT
public:
    explicit BlueConfPP(QMap<QString, BluetoothItem *> *devices, QObject *parent = 0);
    ~BlueConfPP();

    void save();

private:
    QLibrary *blueconf;
    RecordList *rList;

    QList<QPair<QString, QMap<QString, BluetoothItem *> *>> records;
    QMap<QString, BluetoothItem *> *selectedRecordDevices;

    typedef void (*d_record_list)();
    d_record_list dRecordList;
    typedef void (*get_conf)();
    get_conf getConf;
    typedef RecordList* (*get_rList)();
    get_rList getRList;

    void getDevicesList();
};

#endif // BLUECONFPP_H
