# Qt4 GUI testing with Hooq #


## Overview ##

- Records and playbacks QEvents in any dynamically-linked Qt application
- Useful for testing
- Useful for macros
- Useful for pseudo-screencasts (in a significantly bandwidth-reduced form)

### Requirements ###

- Qt 4.5 or higher
- The qtscriptgenerator-generated QtScript bindings

### Directories ###

- *common*: classes used in multiple places.
- *hooq*: GUI application for day to day use - uses hooqInjector, and generates QtScript
	files from the socket XML. When replaying, the QtScript scripts create the XML which
	is sent back to the application.
- *hooqbackup*: CLI application for manipulating .hqt backup files
- *hooqcli*: CLI application for testing - uses hooqInjector, and deals with the raw
 	XML that goes over the socket between hooqInjector and injectedHooq.
- *hooqInjector*: library for injecting injectedHooq into another process.
- *injectedHooq*: spy/event injection library inserted into the other process.
- *qscripthighlighter*: verbatim copy of a QSyntaxHighlighter for QtScript taken from Qt Creator.
 	Used by the hooq GUI application.
- *uilib*: UI classes that may be directly useful in other applications.

### Method ###

- The logger/player libraries are injected into any Qt application's process
- The logger receives all QEvents via QInternal::registerCallback() (sorry for QInternal)
- The player posts events via QCoreApplication::postEvent()

### Injection method ###

- *Linux/OSX*: gdb/gdbserver is attached to the process; then, the extra library is loaded
 	via dlopen(), and "startHooq()" is called.
- *Windows*: we install injectedHooq as a Windows Message hook for the main thread for the
	process; this makes Windows map the DLL into the other processes' memory. On the
	first windows message received in that thread, Hooq gets started; we trigger this
	by sending a WM_NULL message. Previously, LoadLibrary was called via
	CreateRemoteThread - while simpler, this approach didn't end up working, as we need
	our setup code to be ran from the GUI thread to setup the network socket.


## How to use ##

- Clone Hooq and qtscriptgenerator to the same directory

        git clone git://github.com/bilke/hooq.git
        git clone git://gitorious.org/qt-labs/qtscriptgenerator.git

- Build qtscriptgenerator

        cd qtscriptgenerator
        export QTDIR=/usr/lib64/qt4   # This may vary
        cd generator
        qmake
        make
        ./generator
        cd ../qtbindings
        qmake
        make

- Build Hooq

        cd hooq
        qmake
        make qmake
        make all

- Run Hooq

        cd hooq
        ./hooq