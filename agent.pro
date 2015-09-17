#-------------------------------------------------
#
# Project created by QtCreator 2015-04-29T04:42:50
#
#-------------------------------------------------

QT -= gui
QT += network serialport

TARGET = agent
TEMPLATE = app

target.path = /home/root/net-phy
INSTALLS += target

include(QsLog/QsLog.pri)
include(alglib/alglib.pri)

INCLUDEPATH += apm/ \
    apm/mavlink/ \
    apm/comm/ \
    apm/uas/ \
    apm/ui/ \
    net/ \

SOURCES +=\
    apm/comm/AbsPositionOverview.cc \
    apm/comm/LinkInterface.cpp \
    apm/comm/LinkManager.cc \
    apm/comm/MAVLinkDecoder.cc \
    apm/comm/MAVLinkProtocol.cc \
    apm/comm/MissionOverview.cc \
    apm/comm/RelPositionOverview.cc \
    apm/comm/serialconnection.cc \
    apm/comm/UASObject.cc \
    apm/comm/VehicleOverview.cc \
    apm/comm/LinkManagerFactory.cpp \
    apm/comm/UDPLink.cc \
    apm/comm/UDPClientLink.cc \
    apm/comm/TCPLink.cc \
    apm/uas/SlugsMAV.cc \
    apm/uas/senseSoarMAV.cpp \
    apm/uas/PxQuadMAV.cc \
    apm/uas/ArduPilotMegaMAV.cc \
    apm/uas/QGCMAVLinkUASFactory.cc \
    apm/uas/QGCUASParamManager.cc \
    apm/uas/UAS.cc \
    apm/uas/UASManager.cc \
    apm/uas/UASParameter.cpp \
    apm/uas/UASWaypointManager.cc \
    apm/globalobject.cc \
    apm/LogCompressor.cc \
    apm/QGC.cc \
    apm/QGCGeo.cc \
    apm/Waypoint.cc \
    apm/QGCCore.cc \
    apm/main.cc \
    UBAgent.cpp \
    net/UBNetwork.cpp

HEADERS += \
    apm/comm/AbsPositionOverview.h \
    apm/comm/CallConv.h \
    apm/comm/LinkInterface.h \
    apm/comm/LinkManager.h \
    apm/comm/MAVLinkDecoder.h \
    apm/comm/MAVLinkProtocol.h \
    apm/comm/MissionOverview.h \
    apm/comm/ProtocolInterface.h \
    apm/comm/QGCMAVLink.h \
    apm/comm/RelPositionOverview.h \
    apm/comm/serialconnection.h \
    apm/comm/SerialLinkInterface.h \
    apm/comm/UASObject.h \
    apm/comm/VehicleOverview.h \
    apm/comm/LinkManagerFactory.h \
    apm/comm/UDPLink.h \
    apm/comm/UDPClientLink.h \
    apm/comm/TCPLink.h \
    apm/uas/SlugsMAV.h \
    apm/uas/senseSoarMAV.h \
    apm/uas/PxQuadMAV.h \
    apm/uas/ArduPilotMegaMAV.h \
    apm/uas/QGCMAVLinkUASFactory.h \
    apm/uas/QGCUASParamManager.h \
    apm/uas/UAS.h \
    apm/uas/UASInterface.h \
    apm/uas/UASManager.h \
    apm/uas/UASParameter.h \
    apm/uas/UASWaypointManager.h \
    apm/configuration.h \
    apm/globalobject.h \
    apm/LogCompressor.h \
    apm/MG.h \
    apm/QGC.h \
    apm/QGCGeo.h \
    apm/Waypoint.h \
    apm/QGCCore.h \
    UBAgent.h \
    net/UBNetwork.h

