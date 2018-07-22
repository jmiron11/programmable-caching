#include "rule_db.h"

std::string RuleDb::GetRuleKey(const std::string& key, OperationType op) const {
	return OperationType_Name(op) + ":" + key; 
}

void RuleDb::AddRule(const Rule& r) {
	std::lock_guard<std::mutex> lock(db_mutex_);

	rule_db_[GetRuleKey(r.key(), r.op_type())] = r;
}
void RuleDb::RemoveRule(const Rule& r) {
	std::lock_guard<std::mutex> lock(db_mutex_);

	rule_db_.erase(GetRuleKey(r.key(), r.op_type()));
}

bool RuleDb::HasMatchingRule(const std::string& key, OperationType op) const {
	std::lock_guard<std::mutex> lock(db_mutex_);

	return rule_db_.find(GetRuleKey(key, op)) != rule_db_.end();	
}

Rule RuleDb::GetMatchingRule(const std::string& key, OperationType op) const {
	std::lock_guard<std::mutex> lock(db_mutex_);

	return rule_db_.at(GetRuleKey(key, op));
}