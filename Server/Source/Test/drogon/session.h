#pragma once
#ifdef TEST
#include <map>
#include <any>
#include <string>
#include <mutex>
#include <memory>

namespace ikea400::Test
{
	class Session
	{
	public:
        struct string_compare
        {
            using is_transparent = void;

            [[nodiscard]] bool operator()(const char* lhs,
                const char* rhs) const noexcept
            {
                return lhs < rhs;
            }

            [[nodiscard]] bool operator()(std::string_view lhs,
                std::string_view rhs) const noexcept
            {
                return lhs < rhs;
            }

            [[nodiscard]] bool operator()(const std::string& lhs,
                const std::string& rhs) const noexcept
            {
                return lhs < rhs;
            }
        };

        using SessionMap = std::map<std::string, std::any, string_compare>;
	public:
		template <typename Callable>
		void modify(Callable&& handler)
		{
			std::lock_guard<std::mutex> lck(m_mutex);
			handler(m_sessionMap);
		}

        template <typename T, typename Callable>
        void modify(std::string_view key, Callable&& handler)
        {
            std::lock_guard<std::mutex> lck(m_mutex);
            auto it = m_sessionMap.find(key);
            if (it != m_sessionMap.end())
            {
                if (typeid(T) == it->second.type())
                {
                    handler(*(std::any_cast<T>(&(it->second))));
                }
                else
                {
                    LOG_ERROR << "Bad type";
                }
            }
            else
            {
                auto item = T();
                handler(item);
                m_sessionMap.insert(std::make_pair(std::string(key), std::any(std::move(item))));
            }
        }
	private:
		SessionMap m_sessionMap;
		mutable std::mutex m_mutex;
	};

	using SessionPtr = std::shared_ptr<Session>;
}
#endif