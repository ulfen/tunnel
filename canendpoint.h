#ifndef CANENDPOINT_H
#define CANENDPOINT_H

#include "endpoint.h"
#include <QCanBusDevice>

class ConnectDialog;

class CanEndpoint : public Endpoint
{
    Q_OBJECT
public:
    CanEndpoint(QObject *parent = nullptr);
    ~CanEndpoint();

public slots:
    virtual bool open();
    virtual void close();
    virtual void showDialog();

protected slots:
    virtual bool writeData(const QByteArray &data);
    void processReceivedFrames();
    bool sendFrame(const QCanBusFrame &frame) const;
    void processErrors(QCanBusDevice::CanBusError error) const;
    void processFramesWritten(qint64 count);

private:
    ConnectDialog *m_connectDialog = nullptr;
    QCanBusDevice *m_canDevice = nullptr;
};

#endif // CANENDPOINT_H
