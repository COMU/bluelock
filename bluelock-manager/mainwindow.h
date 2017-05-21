/*
 *  Copyright (C) 2017  Çağrı Ulaş <cagriulas@gmail.com>
 *  Copyright (C) 2017  Canberk Koç <canberkkoc@gmail.com>
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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothLocalDevice>
#include <QtWidgets/QMainWindow>
#include <QDebug>
#include <QTimer>
#include <QStandardItemModel>
#include <QMap>

#include <aboutdialog.h>

#include <blueconfpp/blueconfpp.h>
#include <bluetoothitem.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    enum messageType { error, info };
    Q_ENUM(messageType)

private slots:
    void startScan();
    void stopScan();
    void saveConfig();

    void updateDevices(const QBluetoothDeviceInfo &device);

    void scanLocalDevice();

    void sendStatusMessage(QString message, messageType type);
    void clearStatusMessage();

    void setLocalDeviceMode();

    void hostModeStateBehavior(const QBluetoothLocalDevice::HostMode hm);

    void updateSaveButton();

    void exit();
    void openAbout();
    void openAboutQt();

private:
    void addDevice(BluetoothItem *bItem);
    void removeDevice();

    Ui::MainWindow *ui;
    AboutDialog *about;

    QTimer *searchTimer;
    QTimer *statusMessageTimer;
    BlueConfPP *conf;
    QMap<QString, BluetoothItem *> *remotes;
    QBluetoothDeviceDiscoveryAgent *discoveryAgent;
    QBluetoothLocalDevice *localDevice;
    QStandardItemModel *localDeviceModel;
};

#endif // MAINWINDOW_H
