#include "rule_db.h"
#include "gtest/gtest.h"
#include "proto/storage_service.grpc.pb.h"

TEST(RuleDbTest, RuleAdditionAndRetrieval) {
	RuleDb rule_db;

	EXPECT_FALSE(rule_db.HasMatchingRule("key1", OperationType::GET));

	Rule r;
	r.set_key("key1");
	r.set_op_type(OperationType::GET);
	rule_db.AddRule(r);
	EXPECT_TRUE(rule_db.HasMatchingRule("key1", OperationType::GET));

	Rule retrieved = rule_db.GetMatchingRule("key1", OperationType::GET);
	EXPECT_EQ(retrieved.key(), "key1");
	EXPECT_EQ(retrieved.op_type(), OperationType::GET);

	rule_db.RemoveRule(r);
	EXPECT_FALSE(rule_db.HasMatchingRule("key1", OperationType::GET));
}