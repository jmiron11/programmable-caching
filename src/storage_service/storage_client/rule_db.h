#ifndef RULES_DB_H_
#define RULES_DB_H_

#include "proto/storage_service.grpc.pb.h"

#include <unordered_map>
#include <string>

class RuleDb {
 public:
 	void AddRule(const Rule& r);
 	void RemoveRule(const Rule& r);
 	bool HasMatchingRule(const std::string& key, OperationType op) const;
 	Rule GetMatchingRule(const std::string& key, OperationType op) const;
 private:
 	std::string GetRuleKey(const std::string& key, OperationType op) const;

 	mutable std::mutex db_mutex_;
	std::unordered_map<std::string, Rule> rule_db_;
};



#endif