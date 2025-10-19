#include "SessionLock.h"
#include "Common/Utils.h"

#ifdef TEST
#include "../Test/drogon/session.h"
#endif

#include <format>

namespace ikea400 {

	template class SessionLock<drogon::SessionPtr, false>;
	template class SessionLock<drogon::SessionPtr, true>;
#ifdef TEST
	template class SessionLock<Test::SessionPtr, false>;
	template class SessionLock<Test::SessionPtr, true>;
#endif

template <typename SessionPtrType, bool ASYNC>
std::atomic_size_t SessionLock<SessionPtrType, ASYNC>::sm_nNextUniqueId = {};

template <typename SessionPtrType, bool ASYNC>
SessionLock<SessionPtrType, ASYNC>::SessionLock(const SessionPtrType& session, std::string_view name, ELogginLevel logging) :
	m_pSession(session), m_szName(name), m_eLoggingLevel(logging), m_nUniqueId(++sm_nNextUniqueId)
{
}

template <typename SessionPtrType, bool ASYNC>
SessionLock<SessionPtrType, ASYNC>::~SessionLock()
{
	if (m_bOwnLock)
		Unlock();
}

template <typename SessionPtrType, bool ASYNC>
bool SessionLock<SessionPtrType, ASYNC>::TryLock(std::chrono::system_clock::duration lockTimeout)
{
	if (m_eLoggingLevel >= LOG_ALL)
		LOG_INFO << std::format("SessionLock<SessionPtrType, ASYNC>::TryLock '{}:{}'", m_szName, m_nUniqueId);

	using time_point = std::chrono::system_clock::time_point;
	m_pSession->modify<LockDataPtr>(m_szName, [this, lockTimeout](LockDataPtr& lockData) {

		if (!lockData) {
			if (m_eLoggingLevel >= LOG_ALL)
				LOG_INFO << std::format("SessionLock<SessionPtrType, ASYNC>::TryLock '{}' creating lock data", m_szName);
			lockData = std::make_shared<LockData>();
		}

		time_point now = std::chrono::system_clock::now();
		time_point timeout = lockData->timeout;
		if (timeout > now) {
			if (m_eLoggingLevel >= LOG_ALL)
				LOG_INFO << std::format("SessionLock<SessionPtrType, ASYNC>::TryLock '{}' tried to lock but was locked", m_szName);
			return;
		}


		if (timeout > now) {
			if (m_eLoggingLevel >= LOG_ALL)
				LOG_INFO << std::format("SessionLock<SessionPtrType, ASYNC>::TryLock '{}' tried to lock but was locked", m_szName);
			return;
		}

		if (timeout != time_point{}) {
			m_bWasTimeout = true;
			if (m_eLoggingLevel >= LOG_ERR) {
				LOG_INFO << std::format("SessionLock<SessionPtrType, ASYNC>::TryLock '{}' found timeout lock", m_szName);
			}
		}

		m_tpOurTimeout = now + lockTimeout;
		lockData->timeout = m_tpOurTimeout;
		lockData->ownerId = m_nUniqueId;
		m_bOwnLock = true;
		if (m_eLoggingLevel >= LOG_ALL)
			LOG_INFO << std::format("SessionLock<SessionPtrType, ASYNC>::TryLock '{}' locked until {}", m_szName, m_tpOurTimeout);
	});

	return m_bOwnLock;
}

template <typename SessionPtrType, bool ASYNC>
void SessionLock<SessionPtrType, ASYNC>::Unlock()
{
	if (m_eLoggingLevel >= LOG_ALL)
		LOG_INFO << std::format("SessionLock<SessionPtrType, ASYNC>::Unlock called by '{}' for '{}:{}'", m_szName, m_szName, m_nUniqueId);
	if (!m_bOwnLock) {
		if (m_eLoggingLevel >= LOG_ERR)
			LOG_INFO << std::format("SessionLock<SessionPtrType, ASYNC>::Unlock called by '{}' when lock not owned", m_szName);
		return;
	}

	m_pSession->modify<LockDataPtr>(m_szName, [this](const LockDataPtr& lockData) {
		using time_point = std::chrono::system_clock::time_point;
		time_point now = std::chrono::system_clock::now();

		if (m_tpOurTimeout < now)
		{
			if (m_eLoggingLevel >= LOG_ERR)
				LOG_ERROR << "CSessionLock '" << m_szName << "' timeout";
			return;
		}

		if (lockData->timeout == time_point{}) {
			if (m_eLoggingLevel >= LOG_ERR)
				LOG_FATAL << "CSessionLock '" << m_szName << "' supposed to own lock but is not locked";
			return;
		}

		if (lockData->timeout != m_tpOurTimeout) {
			if (m_eLoggingLevel >= LOG_ERR)
				LOG_FATAL << "CSessionLock '" << m_szName << "' supposed to own lock but doesnt";
			return;
		}

		lockData->timeout = {};
	});
}

template <typename SessionPtrType, bool ASYNC>
bool SessionLock<SessionPtrType, ASYNC>::WasTimeout() const noexcept
{
	return m_bWasTimeout;
}

}
