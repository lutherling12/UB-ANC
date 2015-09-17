#include "UBAgent.h"

#include "ArduPilotMegaMAV.h"
#include "SerialLinkInterface.h"

UBAgent::UBAgent(int &argc, char* argv[]) : QGCCore(argc, argv),
    m_uav(NULL),
    m_stage(STAGE_START),
    m_led_on(false),
    m_loiter_timer(0)
{

    m_net = new UBNetwork(this);

    m_msg.append(MAV_CMD_NAV_TAKEOFF);
}

UBAgent::~UBAgent() {
}

void UBAgent::initialize() {

    QGCCore::initialize();

    connect(LinkManager::instance(), SIGNAL(newLink(int)), this, SLOT(handleNewLink(int)));
}

void UBAgent::handleNewLink(int linkid) {
    if(LinkManager::instance()->getLinkType(linkid) == LinkInterface::SERIAL_LINK) {
        SerialLinkInterface* link = static_cast<SerialLinkInterface*>(LinkManager::instance()->getLink(linkid));

        link->setPortName("ttyACM0");
        link->setBaudRate(BAUD_RATE);

        LinkManager::instance()->connectLink(linkid);

        connect(UASManager::instance(), SIGNAL(activeUASSet(UASInterface*)), this, SLOT(handleActiveUAV(UASInterface*)));
    }
}

void UBAgent::handleActiveUAV(UASInterface* uav)
{
    UASManager::instance()->getActiveUASWaypointManager();

    m_uav = static_cast<ArduPilotMegaMAV*>(uav);

    m_uav->setHeartbeatEnabled(true);
    connect(m_uav, SIGNAL(heartbeatTimeout(bool, uint)), this, SLOT(handleHeartbeatTimeout(bool, uint)));

    m_trackTimer = new QTimer(this);
    m_trackTimer->setInterval(TRACK_RATE);

    connect(m_trackTimer, SIGNAL(timeout()), this, SLOT(missionTracker()));

//    QTimer::singleShot(START_DELAY, m_trackTimer, SLOT(start()));

    m_trackTimer->start();
}

void UBAgent::handleHeartbeatTimeout(bool, uint) {
    QLOG_FATAL() << "The master has lost the connetction to APM!!!";

    QCoreApplication::exit(-1);
}

void UBAgent::missionTracker() {
//    if (m_led_on) {
//        m_led_on = false;
//    } else {
//        m_led_on = true;
//    }

    if (!m_uav->isArmed()) {
        return;
    }

//    if (m_start_time < START_DELAY) {
//        m_start_time++;
//        return;
//    }

    switch (m_stage) {
    case STAGE_START:
        stageStart();
        break;
    case STAGE_LOITER:
        stageLoiter();
        break;
    case STAGE_STOP:
        stageStop();
        break;
    }
}

void UBAgent::stageStart() {
    if (m_uav->getAltitudeRelative() < ALT_MIN) {

        if (m_uav->getSatelliteCount() < GPS_ACCURACY)
            return;

        if (m_uav->getCustomMode() != ApmCopter::GUIDED) {
            m_uav->setMode(MAV_MODE_FLAG_CUSTOM_MODE_ENABLED, ApmCopter::GUIDED);
            return;
        }

        if (!m_uav->isArmed()) {
            m_uav->executeCommand(MAV_CMD_COMPONENT_ARM_DISARM, 1, 1, 0, 0, 0, 0, 0, 0, 0);
            return;
        }

        m_uav->executeCommand(MAV_CMD_NAV_TAKEOFF, 1, 0, 0, 0, 0, 0, 0, ALT_MAX, 0);
        return;
    }

    if (m_uav->getAltitudeRelative() > ALT_MAX - ALT_MIN) {
            m_uav->executeCommand(MAV_CMD_NAV_LOITER_TIME, 1, LOITER_TIME, 0, 0, 0, 0, 0, 0, 0);
            m_loiter_timer = QGC::groundTimeSeconds();
            m_stage = STAGE_LOITER;
    }
}

void UBAgent::stageLoiter() {
    if ((QGC::groundTimeSeconds() - m_loiter_timer > LOITER_TIME)) {
        m_uav->executeCommand(MAV_CMD_NAV_LAND, 1, 0, 0, 0, 0, 0, 0, 0, 0);
        m_net->sendData(&m_msg);
        m_stage = STAGE_STOP;
        return;
    }

    if (m_uav->getAltitudeRelative() > ALT_MAX + ALT_MIN) {
        m_uav->executeCommand(MAV_CMD_NAV_LOITER_TIME, 1, LOITER_TIME, 0, 0, 0, 0, 0, 0, 0);
    }
}

void UBAgent::stageStop() {
    m_net->sendData(&m_msg);

    if (m_uav->getAltitudeRelative() > ALT_MAX - ALT_MIN) {
        m_uav->executeCommand(MAV_CMD_NAV_LAND, 1, 0, 0, 0, 0, 0, 0, 0, 0);
        return;
    }

    if (m_uav->getAltitudeRelative() < ALT_MIN) {
        if (m_uav->isArmed()) {
            m_uav->executeCommand(MAV_CMD_COMPONENT_ARM_DISARM, 1, 0, 0, 0, 0, 0, 0, 0, 0);
            return;
        }

        QLOG_INFO() << "Mission is done ...";
        QCoreApplication::exit(0);
    }
}

