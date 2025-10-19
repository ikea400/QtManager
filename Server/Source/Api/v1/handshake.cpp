#include "handshake.h"

#include "../../Utils/SessionLock.h"
#include "../../Utils/ResponseBuilder.h"

void api::v1::Handshake::Hello(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback)
{
	const SessionPtr& session = req->getSession();

	auto builder = ikea400::ResponseBuilder::Get(req, __FUNCTION__);

	try {
		ikea400::SessionLock lock(session, kSessionLockName);

		if (!lock.TryLock()) {
			return callback(builder->Failure("Another request is already in progress for this session.", k423Locked));
		}

		return callback(builder->Success());
	}
	catch (const std::exception& e) {
		LOG_ERROR << __FUNCTION__ " exception: " << e.what();
	}

	return callback(builder->InternalServerError());
}

void api::v1::Handshake::Exchange(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback)
{
}

void api::v1::Handshake::Verify(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback)
{
}
