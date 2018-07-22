#include "redis_storage_interface.h"

RedisInterface::RedisInterface(std::string hostname, std::string port) {
	context_ = redisConnect(hostname.c_str(), std::stoi(port));
}

RedisInterface::~RedisInterface() {
	redisFree(context_);
}

StorageInterface::Status RedisInterface::Get(const std::string& key,
                           std::string* value) {

	redisReply *reply = (redisReply*)redisCommand(context_, "GET %s", key.c_str());
  freeReplyObject(reply);
	return StorageInterface::OK;
}

StorageInterface::Status RedisInterface::Put(const std::string& key,
                           const std::string& value) {
	redisReply *reply = (redisReply*)redisCommand(context_, "SET %s %s", key.c_str(), value.c_str());
  freeReplyObject(reply);
	return StorageInterface::OK;

}

StorageInterface::Status RedisInterface::Remove(const std::string& key) {
	redisReply *reply = (redisReply*)redisCommand(context_, "DEL %s", key.c_str());
  freeReplyObject(reply);
	return StorageInterface::OK;
}

StorageInterface::Status RedisInterface::GetAllKeys(std::vector<std::string>* keys) {
	redisReply *rr = (redisReply*)redisCommand(context_, "KEYS *");
  for(unsigned int i=0; i < rr->elements; i++)
  {
  	keys->push_back(std::string(rr->element[i]->str));
  }
  freeReplyObject(rr);
	return StorageInterface::OK;
}
