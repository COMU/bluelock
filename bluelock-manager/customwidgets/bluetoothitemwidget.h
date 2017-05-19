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
#ifndef BLUETOOTHITEMWIDGET_H
#define BLUETOOTHITEMWIDGET_H

#include <QtWidgets/QWidget>

namespace Ui {
class BluetoothItemWidget;
}

class BluetoothItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BluetoothItemWidget(QString DeviceName,
                                 QString DeviceMAC,
                                 bool available = false,
                                 bool trusted = false,
                                 QWidget *parent = 0);
    ~BluetoothItemWidget();

    QString getDeviceMAC();
    void setDeviceName(QString deviceName);
    void setTrust(bool trusted);
    void setAvailable(bool available);

public slots:
    void trustCheckBoxClicked(bool trustState);

signals:
    void trustStateChanged(bool trustState);

private:
    Ui::BluetoothItemWidget *ui;
};

#endif // BLUETOOTHITEMWIDGET_H
