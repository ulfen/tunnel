#ifndef SERIALENDPOINT_H
#define SERIALENDPOINT_H

#include <QObject>
#include <QSerialPort>

class SettingsDialog;

class SerialEndpoint : public QObject
{
    Q_OBJECT
public:
    explicit SerialEndpoint(QObject *parent = nullptr);
    ~SerialEndpoint();

signals:
    void getData(const QByteArray &data) const;
    void statusMessage(const QString &message) const;

public slots:
    void putData(const QByteArray &data);
    bool open();
    void close();
    void showDialog();

    void readData();
    void handleError(QSerialPort::SerialPortError error);

private:
    SettingsDialog *m_settings = nullptr;
    QSerialPort *m_serial = nullptr;
};

#endif // SERIALENDPOINT_H
