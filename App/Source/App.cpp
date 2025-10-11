#include "UI/LoginMainWindow/LoginMainWindow.h"

#include "Common/Utils.h"

#include <iostream>

#include <QtWidgets/QApplication>

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);

	LoginMainWindow loginWindow;

	loginWindow.show();

	return a.exec();
}