#ifndef INIPARSER_H
#define INIPARSER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <map>

class IniParser {
private:
	std::map<std::string, std::map<std::string, std::string>> data;

public:
	inline std::string trimWhitespace(const std::string& str) {
		std::size_t first = str.find_first_not_of(" \t");
		std::size_t last = str.find_last_not_of(" \t");
		if (first == std::string::npos) {
			return "";
		}
		return str.substr(first, last - first + 1);
	}

	inline std::string getValue(const std::string& section, const std::string& key) const {
		auto sectionIter = data.find(section);
		if (sectionIter == data.end()) {
			throw std::runtime_error("Section not found: " + section);
		}

		auto keyIter = sectionIter->second.find(key);
		if (keyIter == sectionIter->second.end()) {
			throw std::runtime_error("Key not found in section: " + key);
		}

		return keyIter->second;
	}
	inline void addSection(const std::string& section) {
		// check if the section already exists
		auto sectionIter = data.find(section);
		if (sectionIter == data.end()) {
			// if the section does not exist, add it
			data[section] = {};
		}
		else {
			// if the section already exists, overwrite it
			sectionIter->second.clear();
		}
	}

	inline void removeSection(const std::string& section) {
		data.erase(section);
	}

	inline void removeKey(const std::string& section, const std::string& key) {
		auto sectionIter = data.find(section);
		if (sectionIter != data.end()) {
			sectionIter->second.erase(key);
		}
	}

	void parseFromFile(const std::string& filename);
	void setValue(const std::string& section, const std::string& key, const std::string& value);
	void saveToFile(const std::string& filename) const;
};

#endif
