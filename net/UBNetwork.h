#ifndef UBNETWORK_H
#define UBNETWORK_H

#include <QHostAddress>
#include <QTcpSocket>
#include <QByteArray>
#include <QQueue>
#include <QTimer>

#define PACKET_END "\r\r\n\n"

class UBNetwork : public QObject
{
    Q_OBJECT
public:
    explicit UBNetwork(QObject *parent = 0);

    void sendData(const QByteArray* data);
    QByteArray getData();

private:
    enum {
        PHY_PORT = 52001,
        TRACK_RATE = 1000,
    };

signals:

public slots:

private slots:
    void phyConnected();
    void phyDisconnected();
    void dataSent(qint64);

    void dataAvailable();

    void phyError(QAbstractSocket::SocketError);

    void phyTracker();

private:
    QHostAddress m_host;
    quint16 m_port;

    QTcpSocket* m_socket;

    QTimer* m_timer;

    QQueue<QByteArray*> m_send_buffer;
    QQueue<QByteArray*> m_receive_buffer;

    qint64 m_size;

    QByteArray m_data;
};

#endif // UBNETWORK_H
