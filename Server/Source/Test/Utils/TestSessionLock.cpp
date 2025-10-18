#ifdef TEST

#include "../../Utils/SessionLock.h"
#include "../drogon/session.h"

#include <gtest/gtest.h>

using SessionTest = ikea400::Test::Session;
using SessionPtrTest = std::shared_ptr<SessionTest>;
using SessionLockTest = ikea400::SessionLock<SessionPtrTest, false>;

TEST(SessionLockTest, TryLockUnlock)
{
    auto session = std::make_shared<SessionTest>();
    
    SessionLockTest lock(session, "testlock", SessionLockTest::LOG_NONE);

    // Test TryLock
    EXPECT_TRUE(lock.TryLock(std::chrono::seconds(1)));

    // Test Unlock
    lock.Unlock();
    // Après Unlock, le lock ne doit plus être possédé
    EXPECT_FALSE(lock.WasTimeout());
}

TEST(SessionLockTest, TimeoutBehavior)
{
    auto session = std::make_shared<SessionTest>();
    SessionLockTest lock(session, "timeoutlock", SessionLockTest::LOG_NONE);
    SessionLockTest lock2(session, "timeoutlock", SessionLockTest::LOG_NONE);

    // Simule un lock déjà expiré
    lock.TryLock(std::chrono::seconds(-1));

	EXPECT_TRUE(lock2.TryLock(std::chrono::seconds(1)));

    EXPECT_TRUE(lock2.WasTimeout());
}

TEST(SessionLockTest, DoubleLockAttempt)
{
    auto session = std::make_shared<SessionTest>();
    SessionLockTest lock1(session, "doublelock", SessionLockTest::LOG_NONE);
    SessionLockTest lock2(session, "doublelock", SessionLockTest::LOG_NONE);
    EXPECT_TRUE(lock1.TryLock(std::chrono::seconds(1)));
    EXPECT_FALSE(lock2.TryLock(std::chrono::seconds(1)));
    lock1.Unlock();
    EXPECT_TRUE(lock2.TryLock(std::chrono::seconds(1)));
}

#endif // TEST