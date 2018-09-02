#include "canendpoint.h"
#include "connectdialog.h"

#include <QCanBus>
#include <QCanBusFrame>
#include <QVariant>
#include <QQueue>

CanEndpoint::CanEndpoint(QObject *parent) :
    Endpoint(parent)
{
    m_connectDialog = new ConnectDialog;
}

CanEndpoint::~CanEndpoint()
{
    close();
    delete m_connectDialog;
}

bool CanEndpoint::open()
{
    const ConnectDialog::Settings p = m_connectDialog->settings();

    QString errorString;
    m_canDevice = QCanBus::instance()->createDevice(p.pluginName, p.deviceInterfaceName,
                                                    &errorString);
    if (!m_canDevice) {
        emit statusMessage(tr("Error creating device '%1', reason: '%2'")
                           .arg(p.pluginName).arg(errorString));
        return false;
    }

    connect(m_canDevice, &QCanBusDevice::errorOccurred, this, &CanEndpoint::processErrors);
    connect(m_canDevice, &QCanBusDevice::framesReceived, this, &CanEndpoint::processReceivedFrames);
    connect(m_canDevice, &QCanBusDevice::framesWritten, this, &CanEndpoint::processFramesWritten);

    if (p.useConfigurationEnabled) {
        for (const ConnectDialog::ConfigurationItem &item : p.configurations)
            m_canDevice->setConfigurationParameter(item.first, item.second);
    }

    if (!m_canDevice->connectDevice()) {
        emit statusMessage(tr("Connection error: %1").arg(m_canDevice->errorString()));

        delete m_canDevice;
        m_canDevice = nullptr;
        return false;
    } else {
        QVariant bitRate = m_canDevice->configurationParameter(QCanBusDevice::BitRateKey);
        if (bitRate.isValid()) {
            emit statusMessage(tr("Plugin: %1, connected to %2 at %3 kBit/s")
                               .arg(p.pluginName).arg(p.deviceInterfaceName)
                               .arg(bitRate.toInt() / 1000));
        } else {
            emit statusMessage(tr("Plugin: %1, connected to %2")
                               .arg(p.pluginName).arg(p.deviceInterfaceName));
        }
    }
    return true;
}

void CanEndpoint::close()
{
    if (!m_canDevice)
        return;

    m_canDevice->disconnectDevice();
    delete m_canDevice;
    m_canDevice = nullptr;
}

void CanEndpoint::showDialog()
{
    m_connectDialog->show();
}

bool CanEndpoint::writeData(const QByteArray &data)
{
    QCanBusFrame frame(0x200, data);
    return sendFrame(frame);
}

void CanEndpoint::processReceivedFrames()
{
    if (!m_canDevice)
        return;

    while (m_canDevice->framesAvailable()) {
        const QCanBusFrame frame = m_canDevice->readFrame();

        QString view;
        if (frame.frameType() == QCanBusFrame::ErrorFrame)
        {
            view = m_canDevice->interpretErrorFrame(frame);
            emit statusMessage(view);
        }
        else if (frame.frameType() == QCanBusFrame::DataFrame)
        {
            if (frame.frameId() == 0x180)
            {
                const QByteArray data = frame.payload();
                emit getData(data);
            }
        }
    }
}

bool CanEndpoint::sendFrame(const QCanBusFrame &frame) const
{
    if (!m_canDevice)
        return false;

    return m_canDevice->writeFrame(frame);
}

void CanEndpoint::processErrors(QCanBusDevice::CanBusError error) const
{
    switch (error) {
    case QCanBusDevice::ReadError:
    case QCanBusDevice::WriteError:
    case QCanBusDevice::ConnectionError:
    case QCanBusDevice::ConfigurationError:
    case QCanBusDevice::UnknownError:
        emit statusMessage(m_canDevice->errorString());
        break;
    default:
        break;
    }
}

void CanEndpoint::processFramesWritten(qint64 count)
{
    Q_UNUSED(count)
}
