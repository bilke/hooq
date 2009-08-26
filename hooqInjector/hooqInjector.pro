TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += . ../lib

CONFIG += debug

QT -= gui
QT += network

# Input
HEADERS += \
	../lib/Communication.h \
	GdbInjector.h \
	Injector.h \
	RemoteLogger.h \
	RemotePlayback.h \

SOURCES += \
	../lib/Communication.cpp \
	GdbInjector.cpp \
	Injector.cpp \
	RemoteLogger.cpp \
	RemotePlayback.cpp \
	main.cpp \
