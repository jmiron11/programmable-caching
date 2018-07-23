#include "gtest/gtest.h"

#include "proto/scheduler.grpc.pb.h"
#include "proto/storage_service.grpc.pb.h"
#include "serverless_system_view.h"

TEST(ServerlessSystemViewTest, GetSchedulableEngines) {
	ServerlessSystemView system_view(StorageName::EPHEMERAL,
	                                 StorageName::FAKE_PERSISTANT);


	GetViewReply reply;
	GetViewReply::ManagerView* v = reply.add_view();
	v->set_name("managed:EPHEMERAL:abc:abc");
	v->set_uri("hostname:4012");
	GetViewReply::ClientView* c = reply.add_client();
	c->set_name("client:abcdef");
	c->set_uri("hostname:4021");
	system_view.ParseGetViewReply(reply);

	std::vector<std::string> engines;
	system_view.GetSchedulableEngines(&engines);
	EXPECT_EQ(engines.size(), 0);

	system_view.AddExecutionEngine("hostname:1312", "hostname:1423", 8);
	system_view.GetSchedulableEngines(&engines);
	EXPECT_EQ(engines.size(), 0);

	system_view.ParseGetViewReply(reply);
	system_view.GetSchedulableEngines(&engines);
	EXPECT_EQ(engines.size(), 1);

	// No change upon repeated calls with same GetViewReply.
	system_view.ParseGetViewReply(reply);
	system_view.GetSchedulableEngines(&engines);
	EXPECT_EQ(engines.size(), 1);
}

TEST(ServerlessSystemViewTest, GetEngineState) {
	ServerlessSystemView system_view(StorageName::EPHEMERAL,
	                                 StorageName::FAKE_PERSISTANT);

	// Initialize the system view.
	system_view.AddExecutionEngine("hostname:1312", "hostname:1423", 8);
	GetViewReply reply;
	GetViewReply::ManagerView* v = reply.add_view();
	v->set_name("managed:EPHEMERAL:abc:abc");
	v->set_uri("hostname:4012");
	GetViewReply::ClientView* c = reply.add_client();
	c->set_name("client:abcdef");
	c->set_uri("hostname:4021");
	system_view.ParseGetViewReply(reply);

	ServerlessSystemView::EngineState e =
	  system_view.GetEngineState("hostname:1312");
	EXPECT_EQ(e.client_name, "client:abcdef");
}

TEST(ServerlessSystemViewTest, GetPersistantFileMgr) {
	ServerlessSystemView system_view(StorageName::EPHEMERAL,
	                                 StorageName::FAKE_PERSISTANT);

	// Initialize the system view.
	system_view.AddExecutionEngine("hostname:1312", "hostname:1423", 8);
	GetViewReply reply;
	GetViewReply::ManagerView* v = reply.add_view();
	v->set_name("managed:FAKE_PERSISTANT:abc:abc");
	v->set_uri("needforspeedunderground:2");
	v->add_key("key");
	GetViewReply::ClientView* c = reply.add_client();
	c->set_name("client:abcdef");
	c->set_uri("hostname:4021");
	system_view.ParseGetViewReply(reply);

	// no manager associated with hostname exists yet.
	std::vector<std::string> engines;
	system_view.GetSchedulableEngines(&engines);
	EXPECT_EQ(engines.size(), 0);

	std::string mgr = system_view.GetPersistentMgrForFile("key");
	EXPECT_EQ(mgr, "managed:FAKE_PERSISTANT:abc:abc");
}

TEST(ServerlessSystemViewTest, GetEphemeralView) {
	ServerlessSystemView system_view(StorageName::EPHEMERAL,
	                                 StorageName::FAKE_PERSISTANT);

	// Initialize the system view.
	system_view.AddExecutionEngine("hostname:1312", "hostname:1423", 8);
	GetViewReply reply;
	GetViewReply::ManagerView* v = reply.add_view();
	v->set_name("managed:EPHEMERAL:abc:abc");
	v->set_uri("hostname:4012");
	v->add_key("key1");
	v->add_key("key2");
	GetViewReply::ClientView* c = reply.add_client();
	c->set_name("client:abcdef");
	c->set_uri("hostname:4021");
	system_view.ParseGetViewReply(reply);

	std::vector<std::string> engines;
	system_view.GetSchedulableEngines(&engines);
	EXPECT_EQ(engines.size(), 1);
	EXPECT_EQ(engines[0], "hostname:1312");

	std::vector<std::string> keys =
	  system_view.GetEphemeralStorageView("hostname:1312");
	EXPECT_EQ(keys.size(), 2);
	EXPECT_TRUE(keys[0] == "key1" || keys[1] == "key1");
	EXPECT_TRUE(keys[0] == "key2" || keys[1] == "key2");
}