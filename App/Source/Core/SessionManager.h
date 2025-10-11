#pragma once
#include <QUrl>
#include <QNetworkAccessManager>

class SessionManager
{
public:
	SessionManager(const QUrl& url);

private:
	QUrl m_Url;

	QNetworkAccessManager* m_pNetworkManager = nullptr;
};

