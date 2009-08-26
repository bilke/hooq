#include <QApplication>
#include <QPushButton>

int main(int argc, char** argv)
{
	QApplication app(argc, argv);
	QPushButton button;
	button.setObjectName("Test button");
	button.setText("Foo");
	button.connect(&button, SIGNAL(clicked()), &app, SLOT(quit()));
	button.show();
	return app.exec();
}
