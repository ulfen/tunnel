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
    QTimer *m_timer = nullptr;
};

#endif // ENDPOINT_H
