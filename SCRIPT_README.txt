The scripts are written in QtScript, a dialect of ECMAScript; most of QtCore
and QtGui are bound - for example, you can get the user's home directory like
this:

 var home = QDesktopServices.storageLocation(QDesktopServices.HomeLocation);

In general, consult http://doc.trolltech.com/, replacing "::" with ".".

The following additional methods are defined:

 - objectFromPath("ParentWidget::ChildWidget::GrandChildWidget");
 	Returns a reference to a widget in the application under test.

	This will generally be automatically generated via the record
	functionality.

 - assert(bool predicate);
 	Throws an exception if the specified predicate is false.

 - compare(var result, var expected);
 	Throws an exception if the provided values are different; this is
	preferable to assert(result == expected), as it provides a more
	detailed error message.

 - importExtension("extensionName");
	Attempts to load the specified QtScript extension. If the extension can
	not be loaded, an exception is thrown. Example:

	 importExtension("qt.sql"); // loads QtSql

There are several options while recording to make the resulting script easier
to read and alter under the "Options" menu, all of which are turned on by
default:

 - Ignore Mouse Movements
 	Mouse movements will not be recorded - clicks, keypresses, and all
	other events will be kept though. In most cases, mouse movements are a
	huge amount of irrelevant spam.

 - Simplify Strings
	This analyses key presses/releases to try and figure out the actual
	text; for example, without it, typing 'Foo' will result in (pseudocode):

	 press(Qt.Key_Shift);
	 wait();
	 press(Qt.Key_F);
	 wait();
	 release(Qt.Key_F);
	 wait();
	 release(Qt.Key_Shift);
	 wait();
	 press(Qt.Key_O);
	 wait();
	 release(Qt.Key_O);
	 wait();
	 press(Qt.Key_O);
	 wait();
	 release(Qt.Key_O);

	In reality, it will be longer than the above. When this option is
	enabled, instead, the following will be written to the script:

	sendText("Foo");

 - Use Variables for Object References
	This reduces the number of calls to objectFromPath; for example:

	 objectFromPath("MainWindow::centralWidget::tabWidget::mainTab::LineEdit-1").foo();
	 objectFromPath("MainWindow::centralWidget::tabWidget::mainTab::LineEdit-1").bar();

	Will be replaced with:

	 var LineEdit_1 = objectFromPath("MainWindow::centralWidget::tabWidget::mainTab::LineEdit-1");
	 LineEdit_1.foo();
	 LineEdit_1.bar();

 - Use Variables for Strings
	This replaces strings for setText with variables declared at the top of the file. For example:

	 foo();
	 sendText("/home/fred/someFile.txt");
	 baz();

	Would be replaced with:

	 var string0 = "/home/fred/someFile.txt";
	 foo();
	 sendText(string0);
	 baz();
