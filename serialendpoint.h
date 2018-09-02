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
    virtual bool open();
    virtual void close();
    virtual void showDialog();

protected slots:
    virtual bool writeData(const QByteArray &data);
    void readData();
    void handleError(QSerialPort::SerialPortError error);

private:
    SettingsDialog *m_settings = nullptr;
    QSerialPort *m_serial = nullptr;
};

#endif // SERIALENDPOINT_H
