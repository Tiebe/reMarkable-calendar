# Define the project name
TEMPLATE = lib
TARGET = calendar
CONFIG += shared

# Define the Qt modules required
QT += quick qml core network

# Define the C++ standard version
CONFIG += c++11

# Specify the source files
SOURCES += main.cpp Calendar.cpp icalendarlib/date.cpp icalendarlib/icalendar.cpp icalendarlib/types.cpp \
    datefilterproxymodel.cpp \
    qteventmodel.cpp

HEADERS += Calendar.h icalendarlib/date.h icalendarlib/icalendar.h icalendarlib/types.h \
    datefilterproxymodel.h \
    qtevent.h \
    qteventmodel.h

QMAKE_CXXFLAGS += -fPIC 

# QMAKE_CXX = aarch64-remarkable-linux-g++
