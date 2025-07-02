#pragma once

#include <string>
#include <map>
#include <set>
#include <vector>

#include "ahoy/Controller.h"
#include "ahoy/TypedOption.h"

namespace ahoy {
	class Parser : Controller {
	private:
		bool _strict = false;
		bool _quiet = false;
		std::string _name;
		std::map<std::string, Option*> _options;
		std::map<std::string, std::string> _aliases;
		std::map<std::string, std::set<std::string>> _reverseAliases;
		std::map<int, std::string> _indices;
		std::vector<std::string> _examples;
		std::vector<std::string> _usage;
		std::set<std::string> _required;

	public:
		template <class T>
		TypedOption<T>* define(std::string key, T* output) {
			TypedOption<T>* option = new TypedOption<T>(key, output);
			option->_controller = this;
			_options[key] = option;
			return option;
		}
		
		~Parser();
		Parser* example(std::string value);
		virtual std::set<std::string> getAliases(std::string key);
		std::string help();
		Parser* name(std::string value);
		bool parse(const int argc, const char** argv);
		virtual void registerAlias(std::string original, std::string alias);
		virtual void registerIndex(std::string key, int index);
		virtual void registerRequire(std::string key);
		Parser* strict(bool enabled);
		Parser* quiet(bool enabled);
		Parser* usage(std::string value);
	};
}