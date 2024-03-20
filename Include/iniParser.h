#ifndef INIPARSER_H
#define INIPARSER_H

#include <iostream>
#include <cctype>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <unordered_map>
#include <stdexcept>

class IniParser {
private:
	std::unordered_map<std::string, std::unordered_map<std::string, std::string>> data;

	std::string trimWhitespace(const std::string& str) const {
		auto isNotSpace = [](char c) { return !std::isspace(static_cast<unsigned char>(c)); };
		auto start = std::find_if(str.begin(), str.end(), isNotSpace);
		auto end = std::find_if(str.rbegin(), str.rend(), isNotSpace).base();

		if (start < end) {
			return std::string(start, end);
		}
		else {
			return "";
		}
	}

public:
	std::string getValue(const std::string& section, const std::string& key) const {
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

	void addSection(const std::string& section) {
		auto sectionIter = data.find(section);
		if (sectionIter == data.end()) {
			data[section] = {};
		}
		else {
			sectionIter->second.clear();
		}
	}

	void removeSection(const std::string& section) {
		data.erase(section);
	}

	void removeKey(const std::string& section, const std::string& key) {
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
		if (line.empty() || line[0] == ';' || line[0] == '#') {
			continue;
		}

		if (line[0] == '[' && line.back() == ']') {
			currentSection = line.substr(1, line.size() - 2);
			if (currentSection.empty()) {
				throw std::runtime_error("Empty section name encountered.");
			}

			continue;
		}

		auto delimiterPos = line.find('=');
		if (delimiterPos == std::string::npos) {
			throw std::runtime_error("Invalid line format: " + line);
		}

		std::string key = line.substr(0, delimiterPos);
		std::string value = line.substr(delimiterPos + 1);

		key = trimWhitespace(key);
		value = trimWhitespace(value);

		if (currentSection.empty()) {
			throw std::runtime_error("Key-value pair encountered outside of a section.");
		}

		data[currentSection][key] = value;
	}
}

void IniParser::setValue(const std::string& section, const std::string& key, const std::string& value) {
	std::string trimmedKey = trimWhitespace(key);

	auto sectionIter = data.find(section);
	if (sectionIter == data.end()) {
		data[section][trimmedKey] = value;
	}
	else {
		auto& sectionMap = sectionIter->second;
		auto keyIter = sectionMap.find(trimmedKey);
		if (keyIter == sectionMap.end()) {
			sectionMap[trimmedKey] = value;
		}
		else {
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
		file << "[" << trimWhitespace(sectionPair.first) << "]" << std::endl;
		for (const auto& keyValue : sectionPair.second) {
			std::string trimmedKey = trimWhitespace(keyValue.first);
			std::string trimmedValue = trimWhitespace(keyValue.second);

			file << trimmedKey << " = " << trimmedValue << std::endl;
		}
		file << std::endl;
	}
}

#endif // INIPARSER_H
