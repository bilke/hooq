TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += . ../lib

QT -= gui
QT += network

# Input
HEADERS += \
	../lib/Communication.h \
	GdbInjector.h \
	Injector.h \
	RemoteLogger.h \

SOURCES += \
	../lib/Communication.cpp \
	GdbInjector.cpp \
	Injector.cpp \
	RemoteLogger.cpp \
	main.cpp \
