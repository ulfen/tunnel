#ifndef ENDPOINT_H
#define ENDPOINT_H

#include <QObject>

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
    virtual void putData(const QByteArray &data) = 0;
    virtual bool open() = 0;
    virtual void close() = 0;
    virtual void showDialog() = 0;
};

#endif // ENDPOINT_H
