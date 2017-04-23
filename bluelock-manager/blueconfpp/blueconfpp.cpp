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
#include "blueconfpp.h"

BlueConfPP::BlueConfPP(QObject *parent) : QObject(parent)
{
    blueconf = new QLibrary("blueconf", this);

    getConf = (get_conf) blueconf->resolve("get_conf");
    get_rList getRList = (get_rList) blueconf->resolve("get_rList");
    dRecordList = (d_record_list) blueconf->resolve("d_record_list");

    if (getConf) {
        getConf();

        selectedRecordDevices = new QHash<QString, Device *>();

        if (getRList)
            rList = getRList();

        selectedRecord = selectRecord();
    }
}

BlueConfPP::~BlueConfPP()
{
    dRecordList();

    blueconf->unload();
}

void BlueConfPP::getDevicesList()
{
    if (selectedRecord) {
        if (!selectedRecord->size)
            return;
        uint deviceCount = selectedRecord->size;
        Device *d = selectedRecord->dListHead;
        while (deviceCount) {
            selectedRecordDevices->insert(QString(deviceCount), d);
            d = d->next;
            deviceCount--;
        }
    }
}

Record *BlueConfPP::selectRecord()
{
    QString name = qgetenv("USER");
    if (name.isEmpty())
        name = qgetenv("USERNAME");

    if (rList) {
        Record *r = rList->head;
        while (r) {
            if (!name.compare(r->username)) {
                qDebug() << "Selected username:" << QString(r->username);
                return r;
            }
            r = r->next;
        }
    }
    return NULL;
}
