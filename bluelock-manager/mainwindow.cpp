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
    this->setWindowTitle(APPLICATION_DISPLAY_NAME);
    ui->pushButtonSave->setDisabled(true);
    about = NULL;

    localDevice = new QBluetoothLocalDevice(this);
    /*
     * Disable UI if there aren't any local devices existed.
     */
//    if (!localDevice->allDevices().count())
//        ui->centralWidget->setDisabled(true);

    /*
     * Clear statusMessage STATUS_MESSAGE_TIME later
     */
    statusMessageTimer = new QTimer(this);
    connect(statusMessageTimer, SIGNAL(timeout()),
            this, SLOT(clearStatusMessage()));

    /*
     * Stop Device Searching BT_SCAN_TIME later
     */
    searchTimer = new QTimer(this);
    connect(searchTimer, SIGNAL(timeout()),
            this, SLOT(stopScan()));

    discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);
    connect(discoveryAgent, SIGNAL(updateDevices(QBluetoothDeviceInfo)),
            this, SLOT(updateDevices(QBluetoothDeviceInfo)));
    connect(ui->pushButtonSearch, SIGNAL(released()),
            this, SLOT(startScan()));
    connect(localDevice,
            SIGNAL(hostModeStateChanged(QBluetoothLocalDevice::HostMode)),
            this,
            SLOT(hostModeStateBehavior(QBluetoothLocalDevice::HostMode)));
    connect(ui->pushButtonOnOff, SIGNAL(released()),
            this, SLOT(setLocalDeviceMode()));
    connect(ui->actionExit, SIGNAL(triggered(bool)),
            this, SLOT(exit()));
    connect(ui->actionAbout, SIGNAL(triggered(bool)),
            this, SLOT(openAbout()));
    connect(ui->actionAbout_Qt, SIGNAL(triggered(bool)),
            this, SLOT(openAboutQt()));

    // local device combobox
    localDeviceModel = new QStandardItemModel(this);
    QStandardItem *mparent = localDeviceModel->invisibleRootItem();
    QList<QBluetoothHostInfo> localDevices = localDevice->allDevices();
    for (int i = 0; i < localDevices.size(); ++i) {
        QStandardItem *item = new QStandardItem(
                    localDevices.at(i).address().toString());
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
    for (i = remotes->begin(); i != remotes->end(); ++i)
        addDevice(i.value());
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

void MainWindow::addDevice(BluetoothItem *bItem)
{
    QListWidgetItem *listItem = new QListWidgetItem(ui->listWidget);
    BluetoothItemWidget *itemWidget = new BluetoothItemWidget(
                bItem->getName(),
                bItem->getMAC(),
                bItem->getAvailable(),
                bItem->getTrusted(),
                ui->listWidget);
    listItem->setSizeHint(itemWidget->sizeHint());
    ui->listWidget->addItem(listItem);
    ui->listWidget->setItemWidget(listItem, itemWidget);
    /*
     * connects itemWidget->ui->checkBox_trusted with bItem->trusted
     */
    connect(itemWidget, SIGNAL(trustStateChanged(bool)),
            bItem, SLOT(updateTrustState(bool)));
    /*
     * connects itemWidget->ui->checkBox_trusted with this->ui->pushButtonSave
     *
     * this->ui->pushButtonSave's availability must be enabled when the
     * checkBox_trusted's state of any itemWidget changes.
     */
    connect(itemWidget, SIGNAL(trustStateChanged(bool)),
            this, SLOT(updateSaveButton()));
}

void MainWindow::removeDevice()
{

}

void MainWindow::startScan()
{
    qDebug() << "Discovered device count:"
             << discoveryAgent->discoveredDevices().count();
    discoveryAgent->stop();
    foreach (QBluetoothDeviceInfo deviceInfo,
             discoveryAgent->discoveredDevices()) {
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
    foreach (QBluetoothDeviceInfo deviceInfo,
             discoveryAgent->discoveredDevices()) {
        qDebug() << &deviceInfo << " " << deviceInfo.name()
                 << " Cached:" << deviceInfo.isCached()
                 << " Valid:" << deviceInfo.isValid();
    }
}

void MainWindow::updateDevices(const QBluetoothDeviceInfo &device)
{
    qDebug() << "Found device:" << device.name() << '('
             << device.address().toString() << ')';

    BluetoothItem *i = remotes->value(device.address().toString(),
                                      new BluetoothItem());
    /*
     * compare default "BluetoothItem" MAC with "device" MAC for realising that
     * "device" is new or not.
     */
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
//        return;
    } else {
        BluetoothItem *bItem = new BluetoothItem(device.address().toString(),
                                                 device.name(),
                                                 false,
                                                 true);
        remotes->insert(device.address().toString(), bItem);

        addDevice(bItem);
    }
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

void MainWindow::updateSaveButton()
{
    ui->pushButtonSave->setEnabled(true);
}

void MainWindow::exit()
{
    QApplication::exit();
}

void MainWindow::openAbout()
{
    if (!about)
        about = new AboutDialog(this);

    about->show();
}

void MainWindow::openAboutQt()
{
    QApplication::aboutQt();
}
