TEMPLATE = lib
TARGET = 
DEPENDPATH += .
INCLUDEPATH += . ../lib

CONFIG += debug

QT -= gui
QT += network

# Input
HEADERS += \
	../common/Communication.h \
	GdbInjector.h \
	Injector.h \
	RemoteLogger.h \
	RemotePlayback.h \

SOURCES += \
	../common/Communication.cpp \
	GdbInjector.cpp \
	Injector.cpp \
	RemoteLogger.cpp \
	RemotePlayback.cpp \
