#pragma once

#include <QWidget>
#include "ui_LoginWidget.h"

class LoginWidget : public QWidget
{
	Q_OBJECT

public:
	LoginWidget(QWidget *parent = nullptr);
	~LoginWidget();

private:
	Ui::LoginWidgetClass ui;
};

