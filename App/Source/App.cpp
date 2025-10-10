#include "Common/Utils.h"

#include <iostream>

#include <QtCore/qstring.h>

int main()
{
	Ikea400::Utils::DoSomethingUseful(42);
	std::cout << "Hello, World! from App" << std::endl;

	QString qtString = "Hello from Qt!";
	std::cout << qtString.toStdString() << std::endl;

	return 0;
}