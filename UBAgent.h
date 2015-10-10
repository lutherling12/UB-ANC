#ifndef UBAGENT_H
#define UBAGENT_H

#include <QObject>

#include "QGCCore.h"
#include "UBNetwork.h"

#include "ArduPilotMegaMAV.h"

class UBAgent : public QGCCore
{
    Q_OBJECT
public:
    UBAgent(int &argc, char* argv[]);
    ~UBAgent();

    void initialize();

private:
    enum {
        BAUD_RATE = 115200,

        GPS_ACCURACY = 5,
        ALT_MIN = 1,
        ALT_MAX = 4,
        LOITER_TIME = 20,
        START_DELAY = 10000,
        TRACK_RATE = 1000,
    };

    enum mission_stage {
        STAGE_START,
        STAGE_LOITER,
        STAGE_STOP
    } m_stage;

private:
    ArduPilotMegaMAV* m_uav;
    bool m_led_on;
    int m_loiter_timer;

private:
    void stageStart();
    void stageStop();
    void stageLoiter();

signals:

private slots:
    void handleNewLink(int linkid);
    void handleActiveUAV(UASInterface* uas);
    void handleHeartbeatTimeout(bool, uint);

    void missionTracker();

private:
    QTimer* m_trackTimer;
    UBNetwork* m_net;

    QByteArray m_msg;
};

#endif // UBAGENT_H
