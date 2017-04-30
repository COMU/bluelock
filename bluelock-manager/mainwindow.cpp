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
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "customwidgets/bluetoothitemwidget.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pushButtonSave->setDisabled(true);

    localDevice = new QBluetoothLocalDevice(this);
    if (!localDevice->allDevices().count())
        ui->centralWidget->setDisabled(true);

    statusMessageTimer = new QTimer(this);
    connect(statusMessageTimer, SIGNAL(timeout()),
            this, SLOT(clearStatusMessage()));

    searchTimer = new QTimer(this);
    connect(searchTimer, SIGNAL(timeout()),
            this, SLOT(stopScan()));

    discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);
    connect(discoveryAgent, SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)),
            this, SLOT(deviceDiscovered(QBluetoothDeviceInfo)));
    connect(ui->pushButtonSearch, SIGNAL(released()),
            this, SLOT(startScan()));
    connect(localDevice, SIGNAL(hostModeStateChanged(QBluetoothLocalDevice::HostMode)),
            this, SLOT(hostModeStateBehavior(QBluetoothLocalDevice::HostMode)));
    connect(ui->pushButtonOnOff, SIGNAL(released()),
            this, SLOT(setLocalDeviceMode()));
    connect(discoveryAgent, SIGNAL(objectNameChanged(QString)),
            this, SLOT(deviceInfoUpdated(QString)));

    // local device combobox
    localDeviceModel = new QStandardItemModel(this);
    QStandardItem *mparent = localDeviceModel->invisibleRootItem();
    QList<QBluetoothHostInfo> localDevices = localDevice->allDevices();
    for (int i = 0; i < localDevices.size(); ++i) {
        QStandardItem *item = new QStandardItem(localDevices.at(i).address().toString());
        mparent->appendRow(item);
        mparent = item;
    }
    ui->comboBoxLocalDevices->setModel(localDeviceModel);

    hostModeStateBehavior(localDevice->hostMode());
    qDebug() << localDevice->hostMode();

    remotes = new QMap<QString, BluetoothItem *>();

    // blueconf
    conf = new BlueConfPP(remotes, this);

    QMap<QString, BluetoothItem *>::iterator i;
    for (i = remotes->begin(); i != remotes->end(); ++i) {
        BluetoothItem *item = i.value();
        addDevice(item->getName(), item->getMAC(), item->getAvailable(), item->getTrusted());
    }
}

MainWindow::~MainWindow()
{
    discoveryAgent->stop();
    localDevice->setHostMode(QBluetoothLocalDevice::HostPoweredOff);

    delete ui;

    delete searchTimer;
    delete statusMessageTimer;
    delete conf;
    delete discoveryAgent;
    delete localDevice;
    delete localDeviceModel;
}

void MainWindow::sendStatusMessage(QString message, messageType type)
{
    switch (type) {
    case error:
        ui->statusBar->setStyleSheet("color: red");
        break;
    case info:
        ui->statusBar->setStyleSheet("color: blue");
        break;
    default:
        ui->statusBar->setStyleSheet("color: black");
    }
    ui->statusBar->showMessage(message);
    statusMessageTimer->start(STATUS_MESSAGE_TIME);
}

void MainWindow::clearStatusMessage()
{
    ui->statusBar->clearMessage();
    statusMessageTimer->stop();
}

void MainWindow::addDevice(QString DeviceName,
                           QString DeviceMAC,
                           bool available,
                           bool trusted)
{
    QListWidgetItem *listItem = new QListWidgetItem(ui->listWidget);
    BluetoothItemWidget *itemWidget = new BluetoothItemWidget(DeviceName, DeviceMAC, available, trusted, ui->listWidget);
    listItem->setSizeHint(itemWidget->sizeHint());
    ui->listWidget->addItem(listItem);
    ui->listWidget->setItemWidget(listItem, itemWidget);
}

void MainWindow::removeDevice()
{

}

void MainWindow::startScan()
{
    qDebug() << "Discovered device count:" <<discoveryAgent->discoveredDevices().count();
    discoveryAgent->stop();
    foreach (QBluetoothDeviceInfo deviceInfo, discoveryAgent->discoveredDevices()) {
        deviceInfo.setCached(true);
    }
    discoveryAgent->start();
    searchTimer->start(BT_SCAN_TIME);
    ui->statusBar->showMessage(tr("Searching for remote devices..."));
    ui->pushButtonSearch->setEnabled(false);
}

void MainWindow::stopScan()
{
    discoveryAgent->stop();
    searchTimer->stop();
    ui->statusBar->clearMessage();
    ui->pushButtonSearch->setEnabled(true);
    qDebug() << "Devices:";
    foreach (QBluetoothDeviceInfo deviceInfo, discoveryAgent->discoveredDevices()) {
        qDebug() << &deviceInfo << " " << deviceInfo.name()
                 << " Cached:" << deviceInfo.isCached()
                 << " Valid:" << deviceInfo.isValid();
    }
}

void MainWindow::deviceDiscovered(const QBluetoothDeviceInfo &device)
{
    qDebug() << "Found device:" << device.name() << '(' << device.address().toString() << ')';

    BluetoothItem *i = remotes->value(device.address().toString(), new BluetoothItem());
    if (i->getMAC() != BluetoothItem().getMAC()) {
        for (int i = 0; i < ui->listWidget->count(); i++) {
            BluetoothItemWidget *itemWidget = static_cast<BluetoothItemWidget *>
                    (ui->listWidget->itemWidget(ui->listWidget->item(i)));
            if (itemWidget->getDeviceMAC() == device.address().toString()) {
                itemWidget->setDeviceName(device.name());
                itemWidget->setAvailable(true);
            }
        }
        i->setName(device.name());
        i->setAvailable(true);
        return;
    }

    remotes->insert(device.address().toString(),
              new BluetoothItem(device.address().toString(),
                                device.name(),
                                false,
                                true));

    addDevice(device.name(), device.address().toString(), true, false);
}

void MainWindow::scanLocalDevice()
{

}

void MainWindow::setLocalDeviceMode()
{
    if (localDevice->hostMode() == QBluetoothLocalDevice::HostPoweredOff) {
        localDevice->setHostMode(QBluetoothLocalDevice::HostConnectable);
        if (!localDevice->allDevices().count())
            sendStatusMessage(tr("Local device not found!"), error);
    } else {
        localDevice->setHostMode(QBluetoothLocalDevice::HostPoweredOff);
    }
    qDebug() << localDevice->address() << localDevice->hostMode();
}

void MainWindow::hostModeStateBehavior(const QBluetoothLocalDevice::HostMode hm)
{
    if (hm == QBluetoothLocalDevice::HostPoweredOff) {
        ui->pushButtonOnOff->setText(tr("Enable"));
        ui->pushButtonSearch->setDisabled(true);
        sendStatusMessage(tr("Bluetooth device is off!"), info);
    } else {
        ui->pushButtonOnOff->setText(tr("Disable"));
        ui->pushButtonSearch->setEnabled(true);
        sendStatusMessage(tr("Bluetooth device is on!"), info);
    }
}


