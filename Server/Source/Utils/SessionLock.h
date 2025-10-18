#pragma once
#include <chrono>
#include <string>
#include <string_view>
#include <type_traits>
#include <atomic>

#include <drogon/drogon.h>

namespace ikea400
{
	//template <bool ASYNC>
	template <typename SessionPtrType = drogon::SessionPtr, bool ASYNC = false>
	class SessionLock
	{
	public:
		enum ELogginLevel
		{
			LOG_NONE,
			LOG_ERR,
			LOG_ALL,
		};
		using string_type = std::conditional_t<ASYNC, std::string, std::string_view>;
		using session_type = std::conditional_t<ASYNC, SessionPtrType, const SessionPtrType&>;
	public:
		// Avoid copy/move
		SessionLock(const SessionLock& other) = delete;
		SessionLock(SessionLock&&) = default;

		SessionLock(const SessionPtrType& session, std::string_view name, ELogginLevel logging = LOG_ERR);
		~SessionLock();

		bool TryLock(std::chrono::system_clock::duration lockTimeout = std::chrono::seconds(10));
		void Unlock();

		bool WasTimeout() const noexcept;
	private:
		struct LockData
		{
			LockData() = default;
			std::chrono::system_clock::time_point timeout{};
			size_t ownerId{};
		};
		using LockDataPtr = std::shared_ptr<LockData>;
	private:
		const string_type m_szName;
		session_type m_pSession;
		std::chrono::system_clock::time_point m_tpOurTimeout;
		size_t m_nUniqueId = 0;
		const ELogginLevel m_eLoggingLevel = LOG_NONE;
		bool m_bWasTimeout = false;
		bool m_bOwnLock = false;

		static std::atomic_size_t sm_nNextUniqueId;
		static constexpr const std::string_view scm_svSessionLockName = "SessionLock_map";
	};

}

