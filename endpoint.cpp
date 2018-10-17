#include "endpoint.h"

#include <QQueue>
#include <QTimer>

Endpoint::Endpoint(QObject *parent) :
    QObject(parent),
    m_queue(new QQueue<qint8>),
    m_timer(new QTimer(this))
{
    connect(m_timer, &QTimer::timeout, this, &Endpoint::update);
    m_timer->setSingleShot(true);
    m_timer->start(10);
    max_burst_length = 1;
    max_packet_length = 1;
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
    bool success = true;
    int burst = 0;

    while (success)
    {
        if (!waiting.length())
        {
            success = false;
            while (waiting.length() < max_packet_length && !m_queue->isEmpty())
            {
                char byte = m_queue->dequeue();
                waiting.append(byte);
            }
        }

        if (waiting.length())
        {
            success = writeData(waiting);
            if (success)
            {
                waiting.clear();
            }
        }

        if (++burst >= max_burst_length)
            break;
    }

    m_timer->start(success ? 1 : 10);
}
