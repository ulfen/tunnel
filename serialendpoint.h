#ifndef SERIALENDPOINT_H
#define SERIALENDPOINT_H

#include "endpoint.h"
#include <QSerialPort>

class SettingsDialog;

class SerialEndpoint : public Endpoint
{
    Q_OBJECT
public:
    explicit SerialEndpoint(QObject *parent = nullptr);
    ~SerialEndpoint();

public slots:
    virtual void putData(const QByteArray &data);
    virtual bool open();
    virtual void close();
    virtual void showDialog();

    void readData();
    void handleError(QSerialPort::SerialPortError error);

private:
    SettingsDialog *m_settings = nullptr;
    QSerialPort *m_serial = nullptr;
};

#endif // SERIALENDPOINT_H
