#pragma once
#include <QMainWindow>
#include <QStackedWidget.h>
#include "ui_LoginMainWindow.h"

class LoginWidget;

class LoginMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	LoginMainWindow(QWidget *parent = nullptr);
	~LoginMainWindow();

signals:
	void loggedIn();

private:
	Ui::LoginMainWindowClass m_Ui;

	QStackedWidget* m_pStackedWidget = nullptr;

	LoginWidget* m_pLoginWidget = nullptr;
};

