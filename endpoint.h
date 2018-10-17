#ifndef ENDPOINT_H
#define ENDPOINT_H

#include <QObject>

QT_BEGIN_NAMESPACE

class QTimer;

QT_END_NAMESPACE

class Endpoint : public QObject
{
    Q_OBJECT
public:
    explicit Endpoint(QObject *parent = nullptr);
    ~Endpoint();

signals:
    void getData(const QByteArray &data) const;
    void statusMessage(const QString &message) const;

public slots:
    virtual void putData(const QByteArray &data);
    virtual bool open();
    virtual void close();
    virtual void showDialog() = 0;

protected slots:
    virtual void update();
    virtual bool writeData(const QByteArray &data) = 0;

protected:
    QQueue<qint8> *m_queue = nullptr;
    QByteArray waiting;
    QTimer *m_timer = nullptr;
    int max_burst_length = 1;
    int max_packet_length = 1;
};

#endif // ENDPOINT_H
