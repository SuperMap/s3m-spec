#pragma once

#include <string>

namespace ahoy {
	class Option {
	public:
		std::string key;
		bool required = false;
		int indexValue = -1;
		std::string descriptionValue;

		virtual bool parse(const int argc, const char** argv, int* positionPtr, std::string* parseError) {
			return false;
		};

		virtual std::string helpPrefix() {
			return "  " + std::string(key.length() == 1 ? "-" : "--") + key;
		};

		virtual std::string helpSuffix() {
			return (descriptionValue == "" ? "" : descriptionValue + "\t") + std::string(indexValue >= 0 ? " [" + std::to_string(indexValue) + "]" : "") + std::string(required ? " [required]" : "");
		};

		virtual std::string help() {
			return helpPrefix() + "\t" + helpSuffix();
		};
		
		virtual ~Option() {}
	};
}
