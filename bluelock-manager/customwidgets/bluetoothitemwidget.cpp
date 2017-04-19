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
#include "bluetoothitemwidget.h"
#include "ui_bluetoothitemwidget.h"

BluetoothItemWidget::BluetoothItemWidget(QString DeviceName,
                                         QString DeviceMAC,
                                         bool available,
                                         QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BluetoothItemWidget)
{
    ui->setupUi(this);

    ui->label_name->setText(DeviceName);
    ui->label_mac->setText(DeviceMAC);
    ui->checkBox_state->setChecked(available);
}

BluetoothItemWidget::~BluetoothItemWidget()
{
    delete ui;
}
