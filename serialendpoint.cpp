#include "serialendpoint.h"
#include "settingsdialog.h"

#include <QMessageBox>
#include <QQueue>

SerialEndpoint::SerialEndpoint(QObject *parent) :
    Endpoint(parent),
    m_settings(new SettingsDialog),
    m_serial(new QSerialPort(this))
{
    connect(m_serial, &QSerialPort::errorOccurred, this, &SerialEndpoint::handleError);
    connect(m_serial, &QSerialPort::readyRead, this, &SerialEndpoint::readData);
}

SerialEndpoint::~SerialEndpoint()
{
    close();
    delete m_settings;
    delete m_serial;

}

bool SerialEndpoint::open()
{
    const SettingsDialog::Settings p = m_settings->settings();
    QString port = p.name;
    if (!port.startsWith("\\\\.\\"))
        port = "\\\\.\\" + port;
    m_serial->setPortName(port);
    m_serial->setBaudRate(p.baudRate);
    m_serial->setDataBits(p.dataBits);
    m_serial->setParity(p.parity);
    m_serial->setStopBits(p.stopBits);
    m_serial->setFlowControl(p.flowControl);
    if (m_serial->open(QIODevice::ReadWrite)) {
        emit statusMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
                           .arg(p.name).arg(p.stringBaudRate).arg(p.stringDataBits)
                           .arg(p.stringParity).arg(p.stringStopBits).arg(p.stringFlowControl));
    } else {
        emit statusMessage(tr("Open error"));

        QMessageBox::critical(static_cast<QWidget*>(this->parent()), tr("Error"), m_serial->errorString());
    }
    return m_serial->isOpen();
}

void SerialEndpoint::close()
{
    if (m_serial->isOpen())
        m_serial->close();
}

void SerialEndpoint::showDialog()
{
    m_settings->show();
}

bool SerialEndpoint::writeData(const QByteArray &data)
{
    qint64 written = m_serial->write(data);
    return written == data.length();
}

void SerialEndpoint::readData()
{
    const QByteArray data = m_serial->readAll();
    emit getData(data);
}

void SerialEndpoint::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(static_cast<QWidget*>(this->parent()), tr("Critical Error"), m_serial->errorString());
        close();
    }
}
