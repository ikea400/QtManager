#include "SessionManager.h"

SessionManager::SessionManager(const QUrl& url) : m_Url(url)
{
	m_pNetworkManager = new QNetworkAccessManager();
}
