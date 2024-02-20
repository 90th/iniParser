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

void IniParser::parseFromFile(const std::string& filename) {
	std::ifstream file(filename);
	if (!file.is_open()) {
		throw std::runtime_error("Failed to open file: " + filename);
	}

	std::string line;
	std::string currentSection;

	while (std::getline(file, line)) {
		// skip empty lines and comments
		if (line.empty() || line[0] == ';' || line[0] == '#') {
			continue;
		}

		// check if line defines a new section
		if (line[0] == '[' && line.back() == ']') {
			currentSection = line.substr(1, line.size() - 2);

			// validate section name format
			if (currentSection.empty()) {
				throw std::runtime_error("Empty section name encountered.");
			}

			continue;
		}

		// parse key-value pairs
		auto delimiterPos = line.find('=');
		if (delimiterPos == std::string::npos) {
			throw std::runtime_error("Invalid line format: " + line);
		}

		// extract key and value
		std::string key = line.substr(0, delimiterPos);
		std::string value = line.substr(delimiterPos + 1);

		// trim whitespace around key and value
		key = trimWhitespace(key);
		value = trimWhitespace(value);

		// ensure the section name is valid
		if (currentSection.empty()) {
			throw std::runtime_error("Key-value pair encountered outside of a section.");
		}

		// overwrite existing value if the same key is encountered again within the same section
		data[currentSection][key] = value;
	}
}

void IniParser::setValue(const std::string& section, const std::string& key, const std::string& value) {
	// trim leading and trailing whitespace from the key
	std::string trimmedKey = key;
	trimmedKey.erase(0, trimmedKey.find_first_not_of(" \t"));
	trimmedKey.erase(trimmedKey.find_last_not_of(" \t") + 1);

	// check if the section exists
	auto sectionIter = data.find(section);
	if (sectionIter == data.end()) {
		// if the section doesn't exist, create it and add the key-value pair
		data[section][trimmedKey] = value;
	}
	else {
		// if the section exists, check if the key already exists in that section
		auto& sectionMap = sectionIter->second;
		auto keyIter = sectionMap.find(trimmedKey);
		if (keyIter == sectionMap.end()) {
			// if the key doesn't exist in the section, add it
			sectionMap[trimmedKey] = value;
		}
		else {
			// if the key exists in the section, update its value
			keyIter->second = value;
		}
	}
}

void IniParser::saveToFile(const std::string& filename) const {
	std::ofstream file(filename);
	if (!file.is_open()) {
		throw std::runtime_error("Failed to open file for writing: " + filename);
	}

	for (const auto& sectionPair : data) {
		file << "[" << sectionPair.first << "]" << std::endl;
		for (const auto& keyValue : sectionPair.second) {
			// trim leading and trailing whitespace from the key and value
			std::string trimmedKey = keyValue.first;
			trimmedKey.erase(0, trimmedKey.find_first_not_of(" \t"));
			trimmedKey.erase(trimmedKey.find_last_not_of(" \t") + 1);

			std::string trimmedValue = keyValue.second;
			trimmedValue.erase(0, trimmedValue.find_first_not_of(" \t"));
			trimmedValue.erase(trimmedValue.find_last_not_of(" \t") + 1);

			file << trimmedKey << " = " << trimmedValue << std::endl;
		}
		file << std::endl;  // add an empty line between sections
	}
}

#endif 
