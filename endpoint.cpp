#include "endpoint.h"

#include <QQueue>
#include <QTimer>

Endpoint::Endpoint(QObject *parent) :
    QObject(parent),
    m_queue(new QQueue<qint8>),
    m_timer(new QTimer(this))
{
    connect(m_timer, &QTimer::timeout, this, &Endpoint::update);
    m_timer->start(10);
}

Endpoint::~Endpoint()
{
    m_timer->stop();
    close();
    delete m_timer;
    delete m_queue;
}

void Endpoint::putData(const QByteArray &data)
{
    for (auto &x : data){
        m_queue->enqueue(x);
    }
}

bool Endpoint::open()
{
    m_queue->clear();
    return true;
}

void Endpoint::close()
{
    m_queue->clear();
}

void Endpoint::update()
{
    while (!m_queue->isEmpty())
    {
        char byte = m_queue->head();
        QByteArray data(1, byte);
        bool success = writeData(data);
        if (!success)
            break;
        m_queue->dequeue();
    }
}
