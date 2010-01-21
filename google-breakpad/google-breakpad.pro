TEMPLATE = lib
TARGET = breakpad
DEPENDPATH += .
INCLUDEPATH += .
# debug/release in hooq
include (../../common.pri) 

QT -= core gui
CONFIG += static

unix {
	INCLUDEPATH += src
	INCLUDEPATH += src/client/linux
	INCLUDEPATH += src/third_party/linux/include

	SOURCES += \
		src/common/string_conversion.cc \
		src/common/linux/file_id.cc \
		src/common/linux/guid_creator.cc \
		src/common/convert_UTF.c \
		src/client/minidump_file_writer.cc \
		src/client/linux/handler/exception_handler.cc \
		src/client/linux/minidump_writer/linux_dumper.cc \
		src/client/linux/minidump_writer/minidump_writer.cc \

}

win32 {
	INCLUDEPATH += src
	INCLUDEPATH += src/client/windows
	INCLUDEPATH += src/third_party/windows/include

	SOURCES += \
		src/common/windows/guid_string.cc \
		src/client/windows/handler/exception_handler.cc \
		src/client/windows/crash_generation/client_info.cc \
		src/client/windows/crash_generation/crash_generation_client.cc \
		src/client/windows/crash_generation/crash_generation_server.cc \
		src/client/windows/crash_generation/minidump_generator.cc \

}