#include "bluetoothitem.h"

BluetoothItem::BluetoothItem(QString MAC,
                             QString name,
                             bool trusted,
                             bool available)
{
    this->MAC = MAC;
    this->name = name;
    this->trusted = trusted;
    this->available = available;
}

QString BluetoothItem::getMAC() const
{
    return MAC;
}

void BluetoothItem::setMAC(const QString &value)
{
    MAC = value;
}

QString BluetoothItem::getName() const
{
    return name;
}

void BluetoothItem::setName(const QString &value)
{
    name = value;
}

bool BluetoothItem::getTrusted() const
{
    return trusted;
}

void BluetoothItem::setTrusted(bool value)
{
    trusted = value;
}

bool BluetoothItem::getAvailable() const
{
    return available;
}

void BluetoothItem::setAvailable(bool value)
{
    available = value;
}
