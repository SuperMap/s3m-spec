#pragma once

#include "ahoy/Option.h"
#include "ahoy/Controller.h"

namespace ahoy {
	template <class T>
	class TypedOption : public Option {
	public:
		Controller* _controller;
		T* output;
		bool hasDefault = false;
		T defaultValue;
		int countValue = -1;

		TypedOption(std::string key, T* output) {
			this->key = key;
			this->output = output;
		};
		TypedOption<T>* alias(std::string alias) {
			_controller->registerAlias(key, alias);
			return this;
		}
		TypedOption<T>* count(int count) {
			countValue = count;
			return this;
		}
		TypedOption<T>* defaults(T value) {
			hasDefault = true;
			defaultValue = value;
			*output = value;
			return this;
		}
		TypedOption<T>* description(std::string value) {
			descriptionValue = value;
			return this;
		}
		TypedOption<T>* index(int index) {
			_controller->registerIndex(key, index);
			indexValue = index;
			return this;
		}
		std::string helpPrefix() {
			std::string prefix = "";
			for (std::string alias : _controller->getAliases(key)) {
				prefix += ", ";
				prefix += (alias.length() == 1 ? "-" : "--") + alias;
			}
			return Option::helpPrefix() + prefix;
		}
		std::string help();
		virtual bool parse(const int argc, const char** argv, int* positionPtr, std::string* parseError);
		TypedOption<T>* require() {
			_controller->registerRequire(key);
			required = true;
			return this;
		}
	};
}
