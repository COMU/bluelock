#ifndef BLUETOOTHITEM_H
#define BLUETOOTHITEM_H

#include <QObject>
#include <QString>

#include <customwidgets/bluetoothitemwidget.h>

class BluetoothItem : public QObject
{
    Q_OBJECT

public:
    explicit BluetoothItem(QString MAC = "0",
                           QString name = "0",
                           bool trusted = false,
                           bool available = false,
                           QObject *parent = 0);

    QString getMAC() const;
    void setMAC(const QString &value);

    QString getName() const;
    void setName(const QString &value);

    bool getTrusted() const;
    void setTrusted(bool value);

    bool getAvailable() const;
    void setAvailable(bool value);

public slots:
    void updateTrustState(bool trustState);

private:
    QString MAC;
    QString name;
    bool trusted;
    bool available;
};

#endif // BLUETOOTHITEM_H
