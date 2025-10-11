#include "LoginMainWindow.h"

#include "../LoginWidget/LoginWidget.h"

LoginMainWindow::LoginMainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	m_Ui.setupUi(this);

	m_pStackedWidget = new QStackedWidget(this);

	m_pLoginWidget = new LoginWidget(this);

	m_pStackedWidget->addWidget(m_pLoginWidget);

	m_pStackedWidget->setCurrentWidget(m_pLoginWidget);

	setCentralWidget(m_pStackedWidget);
}

LoginMainWindow::~LoginMainWindow()
{}

