#pragma once

#include <drogon/Session.h>
#include <drogon/HttpRequest.h>
#include <drogon/HttpResponse.h>

#include <memory>
#include <string>

namespace ikea400
{

	class ResponseBuilder;
	using ResponseBuilderPtr = std::shared_ptr<ResponseBuilder>;

	class ResponseBuilder
	{
	public:
		// Avoid copy construction and assignment
		ResponseBuilder(const ResponseBuilder&) = delete;
		ResponseBuilder& operator=(const ResponseBuilder&) = delete;

		drogon::HttpResponsePtr InternalServerError();
		drogon::HttpResponsePtr BadRequest(const std::string& reason = "Bad request");
		drogon::HttpResponsePtr BadRequest(const Json::Value& body, const std::string& reason = "Bad request");
		drogon::HttpResponsePtr BadRequest(Json::Value&& body, const std::string& reason = "Bad request");

		drogon::HttpResponsePtr Failure(const std::string& reason, drogon::HttpStatusCode status, void(*failureCallback)() = []() {});
		drogon::HttpResponsePtr Failure(const Json::Value& body, const std::string& reason, drogon::HttpStatusCode status, void(*failureCallback)() = []() {});
		drogon::HttpResponsePtr Failure(Json::Value&& body, const std::string& reason, drogon::HttpStatusCode status, void(*failureCallback)() = []() {});

		drogon::HttpResponsePtr Success(void(*failureCallback)() = []() {});
		drogon::HttpResponsePtr Success(const Json::Value& body, void(*failureCallback)() = []() {});
		drogon::HttpResponsePtr Success(Json::Value&& body, void(*failureCallback)() = []() {});

		drogon::HttpResponsePtr CreateResponse(Json::Value&& body, drogon::HttpStatusCode status, void(*failureCallback)());

		static ResponseBuilderPtr Get(const drogon::HttpRequestPtr& request, std::string_view logName);

	private:
		ResponseBuilder(drogon::SessionPtr session, std::string&& logName);
		~ResponseBuilder() noexcept;
	private:
		std::string m_szLogName;
		drogon::SessionPtr m_pSession;

		bool m_bResponseBuilt = false;

		static constexpr const inline std::string_view kAttributeName = "response_builder";
	};
}