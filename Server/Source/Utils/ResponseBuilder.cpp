#include "ResponseBuilder.h"

#include <drogon/Attribute.h>

namespace ikea400
{
    using namespace drogon;

    ResponseBuilderPtr ikea400::ResponseBuilder::Get(const HttpRequestPtr& request, std::string_view logName)
    {
        const AttributesPtr& attributes = request->getAttributes();
        if (const auto& builder = attributes->get<ResponseBuilderPtr>(kAttributeName)) {
            return builder;
        }

        struct make_shared_enabler : public ResponseBuilder {
            make_shared_enabler (SessionPtr session, std::string&& logName)
                : ResponseBuilder(std::move(session), std::move(logName))
            {
			}
        };
		ResponseBuilderPtr builder = std::make_shared<make_shared_enabler>(request->getSession(), std::string(logName));
        attributes->insert(logName, builder);
        return builder;
    }

    ResponseBuilder::ResponseBuilder(SessionPtr session, std::string&& logName) :
		m_pSession(std::move(session)), m_szLogName(std::move(logName))
    {
    }

    ResponseBuilder::~ResponseBuilder() noexcept
    {
        if (!m_bResponseBuilt) {
            LOG_WARN << "ResponseBuilder '" << m_szLogName << "' was destroyed without building a response";
		}
    }

    HttpResponsePtr ResponseBuilder::InternalServerError()
    {
        return Failure("Internal server error", k500InternalServerError);
    }

    HttpResponsePtr ResponseBuilder::BadRequest(const std::string& reason)
    {
        return Failure(reason, k400BadRequest);
    }

    HttpResponsePtr ResponseBuilder::BadRequest(const Json::Value& body, const std::string& reason)
    {
        return Failure(body, reason, k400BadRequest);
    }

    HttpResponsePtr ResponseBuilder::BadRequest(Json::Value&& body, const std::string& reason)
    {
        return Failure(std::move(body), reason, k400BadRequest);
    }

    HttpResponsePtr ResponseBuilder::Failure(const std::string& reason, HttpStatusCode status, void(*errorCallback)())
    {
        return Failure(Json::Value{}, reason, status, errorCallback);
    }

    HttpResponsePtr ResponseBuilder::Failure(const Json::Value& body, const std::string& reason, HttpStatusCode status, void(*errorCallback)())
    {
        Json::Value modifiedBody = body;
        modifiedBody["success"] = false;
        modifiedBody["reason"] = reason;
        return CreateResponse(std::move(modifiedBody), status, errorCallback);
    }

    HttpResponsePtr ResponseBuilder::Failure(Json::Value&& body, const std::string& reason, HttpStatusCode status, void(*errorCallback)())
    {
        body["success"] = false;
        body["reason"] = reason;
        return CreateResponse(std::move(body), status, errorCallback);
    }

    HttpResponsePtr ResponseBuilder::Success(void(*errorCallback)())
    {
        return Success(Json::Value{}, errorCallback);
    }

    HttpResponsePtr ResponseBuilder::Success(const Json::Value& body, void(*errorCallback)())
    {
        Json::Value modifiedBody = body;
        modifiedBody["success"] = true;
        return CreateResponse(std::move(modifiedBody), k200OK, errorCallback);
    }

    HttpResponsePtr ResponseBuilder::Success(Json::Value&& body, void(*errorCallback)())
    {
        body["success"] = true;
        return CreateResponse(std::move(body), k200OK, errorCallback);
    }

    HttpResponsePtr ResponseBuilder::CreateResponse(Json::Value&& body, HttpStatusCode status, void(*errorCallback)())
    {
        m_bResponseBuilt = true;

        auto response = HttpResponse::newHttpJsonResponse(std::move(body));
        response->setStatusCode(status);
        return response;
    }

} // namespace ikea400
