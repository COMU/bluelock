#ifndef BLUETOOTHITEM_H
#define BLUETOOTHITEM_H

#include <QString>

class BluetoothItem
{
public:
    explicit BluetoothItem(QString MAC = "0",
                           QString name = "0",
                           bool trusted = false,
                           bool available = false);

    QString getMAC() const;
    void setMAC(const QString &value);

    QString getName() const;
    void setName(const QString &value);

    bool getTrusted() const;
    void setTrusted(bool value);

    bool getAvailable() const;
    void setAvailable(bool value);

private:
    QString MAC;
    QString name;
    bool trusted;
    bool available;
};

#endif // BLUETOOTHITEM_H
