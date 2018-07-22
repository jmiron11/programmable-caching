#include "gtest/gtest.h"
#include "redis_storage_interface.h"

#include <glog/logging.h>
#include <unordered_map>

namespace {

std::string gen_string(int len) {
	std::string generated;
	generated.reserve(len);

    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < len; ++i) {
        generated[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    generated[len] = 0;

  return generated;
}

}	 // namespace

// Test only works if the server is started up
class RedisTest : public ::testing::Test {
 public:
	RedisTest() { }

	void SetUp() override {
		redis_ = new RedisInterface("localhost", "6379");
	}

	void TearDown() override {
		delete redis_;
	}

	RedisInterface* redis_;
};

TEST_F(RedisTest, OpenPutGet) {
	// STORE <KEY, VALUE2>
	StorageInterface::Status s = redis_->Put("KEY", "VALUE2");
	EXPECT_EQ(s, StorageInterface::OK);

	// Missing key returns error
	std::string value;
	s = redis_->Get("MISSING_KEY", &value);
	EXPECT_EQ(s, StorageInterface::GENERIC_ERROR);

	// Existing key KEY returns VALUE2.
	s = redis_->Get("KEY", &value);
	EXPECT_EQ(s, StorageInterface::OK);
	EXPECT_EQ(value, "VALUE2");
}

TEST_F(RedisTest, GetAllKeys) {
	StorageInterface::Status s;
	std::unordered_map<std::string, int> put_keys;

	s = redis_->Put("KEY1", "");
	put_keys["KEY1"] = 0;
	EXPECT_EQ(s, StorageInterface::OK);
	s = redis_->Put("KEY2", "");
	put_keys["KEY2"] = 0;
	EXPECT_EQ(s, StorageInterface::OK);

	std::vector<std::string> keys;
	s = redis_->GetAllKeys(&keys);
	EXPECT_EQ(s, StorageInterface::OK);

	// Keys returned must be exactly the set of put_keys.
	EXPECT_EQ(put_keys.size(), keys.size());

	for(const auto& ele : keys) {
		EXPECT_NE(put_keys.find(ele), put_keys.end());
	}
}

TEST_F(RedisTest, ReadWrite100) {
	StorageInterface::Status s;
	std::unordered_map<std::string, std::string> generated_pairs;

	// Generate 100 different key value pairs and PUT them into database.
	for(int i = 0; i < 100; ++i) {
		std::string key = gen_string(20);
		std::string value = gen_string(20);

		generated_pairs[key] = value;
		s = redis_->Put(key, value);
		EXPECT_EQ(s, StorageInterface::OK);
	}	

	// Check that each get request returns the value PUT into the database.
	std::string value_returned;
	for(const std::pair<std::string, std::string>& pair : generated_pairs) {
		s = redis_->Get(pair.first, &value_returned);
		EXPECT_EQ(s, StorageInterface::OK);
		EXPECT_EQ(pair.second, value_returned);
	}
}