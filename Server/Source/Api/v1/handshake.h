#pragma once
#include <drogon/HttpController.h>

namespace api::v1
{
	using namespace drogon;

	class Handshake : public HttpController<Handshake>
	{
	public:
		struct SessionData
		{

		};

		using SessionDataPtr = std::shared_ptr<SessionData>;
	public:
		METHOD_LIST_BEGIN
		METHOD_ADD(Handshake::Hello, "/Hello", Post); //path is /api/v1/handshake/hello
		METHOD_ADD(Handshake::Exchange, "/Exchange", Post); //path is /api/v1/handshake/exchange
		METHOD_ADD(Handshake::Verify, "/Verify", Post); //path is /api/v1/handshake/verify
		METHOD_LIST_END

		SessionDataPtr GetSessionData(const SessionPtr& session) {
			return session->get<SessionDataPtr>(kSessionDataName);
		}
	protected:
		void Hello(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback);
		void Exchange(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback);
		void Verify(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback);
	private:
		static constexpr const std::string_view kSessionDataName = "handshake_data";
		static constexpr const std::string_view kSessionLockName = "handshake_lock";
	};
}