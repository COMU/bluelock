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

#include <QDBusMessage>
#include <QDBusReply>
#include <QDBusConnection>

BlueConfPP::BlueConfPP(QMap<QString, BluetoothItem *> *devices,
                       QObject *parent) :
    QObject(parent)
{
    blueconf = new QLibrary("blueconf", this);
    selectedRecordDevices = devices;
    if (!blueconf->load())
        qDebug() << "Cannot access libblueconf.so";

    getConf = (get_conf) blueconf->resolve("get_conf");
    getRList = (get_rList) blueconf->resolve("get_rList");
    dRecordList = (d_record_list) blueconf->resolve("d_record_list");

    if (getConf) {
        getConf();

        if (getRList)
            rList = getRList();
        else
            rList = NULL;

        getDevicesList();
        dRecordList();

        blueconf->unload();

        delete blueconf;
    }
}

BlueConfPP::~BlueConfPP()
{
    QMap<QString, BluetoothItem *>::iterator i;
    while (!records.isEmpty()) {
        QPair<QString, QMap<QString, BluetoothItem *> *> r;
        r = records.takeFirst();
        for (i = r.second->begin(); i != r.second->end(); ++i)
            delete i.value();
    }
}

void BlueConfPP::save()
{
    QFile file(CONFIG_PATH);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Cannot open config file.";
        return;
    }

    qDebug() << "Config file opened.";
    QTextStream out(&file);
    out << "# Do NOT edit this file as it is auto-generated by bluelock-manager"
           "\n\n";
    QMap<QString, BluetoothItem *>::iterator i;
    for (int order = records.size() - 1; 0 <= order; order--) {
        QPair<QString, QMap<QString, BluetoothItem *> *> r = records.at(order);
        out << r.first << " {\n";
        for (i = r.second->begin(); i != r.second->end(); ++i) {
            BluetoothItem *item = i.value();
            if (item->getTrusted())
                out << "\t\""
                    << item->getName()
                    << "\" = "
                    << item->getMAC()
                    << ",\n";
        }
        out << "}\n\n";
    }
}

void BlueConfPP::getDevicesList()
{
    QString name = "";
    QDBusMessage mssg;
    mssg = QDBusMessage::createMethodCall("org.freedesktop.login1",
                                          "/org/freedesktop/login1/user/self",
                                          "org.freedesktop.DBus.Properties",
                                          "Get");
    QList<QVariant> args;
    args.append(QVariant("org.freedesktop.login1.User"));
    args.append(QVariant("Name"));
    mssg.setArguments(args);

    QDBusReply<QVariant> reply = QDBusConnection::systemBus().call(mssg);
    if (reply.isValid())
        name = reply.value().toString();

    bool usernameSelected = false;

    if (rList) {
        Record *r = rList->head;
        while (r) {
            if (!name.compare(r->username)) {
                qDebug() << "Selected username:" << QString(r->username);
                usernameSelected = true;
                records.append(QPair<QString, QMap<QString, BluetoothItem *> *>
                               (QString(r->username), selectedRecordDevices));
            }
            else {
                records.append(QPair<QString, QMap<QString, BluetoothItem *> *>
                               (QString(r->username),
                                new QMap<QString, BluetoothItem *>()));
            }
            Device *d = r->dListHead;
            while (d) {
                records.back().second->insert(QString(d->mac),
                                              new BluetoothItem(QString(d->mac),
                                                                QString(d->name),
                                                                true));
                d = d->next;
            }
            r = r->next;
        }
    }
    if (!usernameSelected)
        records.append(QPair<QString, QMap<QString, BluetoothItem *> *>
                       (QString(name), selectedRecordDevices));

    qDebug() << records;
}
