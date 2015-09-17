#include "phylink.h"
#include "QsLog.h"

#include <QCoreApplication>

PHYLink::PHYLink(QObject *parent) : QObject(parent) {
    m_host = QHostAddress(QHostAddress::LocalHost);

    m_port = PHY_PORT;
    m_size = 0;

    m_socket = new QTcpSocket(this);

    connect(m_socket, SIGNAL(bytesWritten(qint64)), this, SLOT(dataSent(qint64)));
    connect(m_socket, SIGNAL(connected()), this, SLOT(phyConnected()));
    connect(m_socket, SIGNAL(disconnected()), this, SLOT(phyDisconnected()));
    connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(phyError(QAbstractSocket::SocketError)));
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(dataAvailable()));

    m_timer = new QTimer(this);
    m_timer->setInterval(TRACK_RATE);

    connect(m_timer, SIGNAL(timeout()), this, SLOT(phyTracker()));

    m_timer->start();
}

void PHYLink::sendData(const QByteArray* data) {
    QByteArray* stream = new QByteArray(*data);

    m_send_buffer.enqueue(stream);
}

QByteArray PHYLink::getData() {
    QByteArray data;

    if (!m_receive_buffer.isEmpty()) {
        QByteArray* stream = m_receive_buffer.dequeue();

        data = *stream;

        delete stream;
    }

    return data;
}

void PHYLink::dataSent(qint64 size) {
    m_size -= size;

    if (!m_size) {
        delete m_send_buffer.dequeue();
        QLOG_DEBUG() << "The message sended";

        m_size = 0;
    }
}

void PHYLink::dataAvailable() {
    m_data += m_socket->readAll();

    while (m_data.contains(PACKET_END)) {
        int bytes = m_data.indexOf(PACKET_END);
        QByteArray* data = new QByteArray(m_data.left(bytes));

        m_receive_buffer.enqueue(data);
        QLOG_DEBUG() << "We received: " << data;

        m_data = m_data.mid(bytes + qstrlen(PACKET_END));
    }
}

void PHYLink::phyConnected() {
    QLOG_DEBUG() << "We connect to the PHY";
}

void PHYLink::phyDisconnected() {
    QLOG_DEBUG() << "We disconnected from PHY!";
    QCoreApplication::exit(-1);
}

void PHYLink::phyError(QAbstractSocket::SocketError) {
   QLOG_DEBUG() << "PHY ERROR: " << m_socket->errorString();
}

void PHYLink::phyTracker() {
    if (m_socket->state() != QAbstractSocket::ConnectedState) {
        m_socket->connectToHost(m_host, m_port);
    } else if (!m_send_buffer.isEmpty() && !m_size) {
        QByteArray* data = m_send_buffer.first();
        data->append(PACKET_END);
        m_size = data->size();

        m_socket->write(*data);
    }
}
