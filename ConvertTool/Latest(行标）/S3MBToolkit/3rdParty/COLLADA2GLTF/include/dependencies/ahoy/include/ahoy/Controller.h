#pragma once

#include <set>

class Controller {
public:
	virtual std::set<std::string> getAliases(std::string key) { return std::set<std::string>(); };
	virtual void registerAlias(std::string original, std::string alias) {};
	virtual void registerIndex(std::string key, int index) {};
	virtual void registerRequire(std::string key) {};
};