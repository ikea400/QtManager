#include "handshake.h"
#include "../../Utils/SessionLock.h"

void api::v1::Handshake::Hello(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback)
{
	const SessionPtr& session = req->getSession();

	try {
		ikea400::SessionLock lock(session, "sm_szSessionLockName");

		if (!lock.TryLock()) {
			auto response = drogon::HttpResponse::newHttpJsonResponse(Json::Value("Please wait between request"));
			response->setStatusCode(k408RequestTimeout);
			callback(response);
			return;
		}

		auto response = drogon::HttpResponse::newHttpJsonResponse(Json::Value("Handshake started"));
		response->setStatusCode(k200OK);
		callback(response);
		return;
	}
	catch (const std::exception& e) {
		LOG_ERROR << __FUNCTION__ " exception: " << e.what() << "\n";
	}

	auto response = drogon::HttpResponse::newHttpJsonResponse(Json::Value("Internal server error"));
	response->setStatusCode(k500InternalServerError);
	callback(response);
}

void api::v1::Handshake::Exchange(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback)
{
}

void api::v1::Handshake::Verify(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback)
{
}
