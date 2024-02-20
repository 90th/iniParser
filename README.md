# Lightweight C++ INI Parser

This is a lightweight C++ library for parsing and manipulating INI (Initialization) files. INI files are simple text files that store configuration data in a key-value pair format, organized into sections.

## Features

- Parse INI files and access their contents.
- Add, modify, or remove sections, keys, and values.
- Save modified data back to the INI file.

## Usage

1. Include the `IniParser.h` header file in your C++ project.
2. Instantiate an `IniParser` object.
3. Use the provided methods to parse, manipulate, and save INI files.

```cpp
#include "IniParser.h"
#include <iostream>

int main() {
    // Instantiate an IniParser object
    IniParser parser;

    try {
        // Parse an existing INI file
        parser.parseFromFile("example.ini");

        // Access and modify data
        parser.setValue("Section1", "Key1", "NewValue");
        parser.addSection("NewSection");
        parser.setValue("NewSection", "NewKey", "Value");

        // Save modified data back to the INI file
        parser.saveToFile("example.ini");
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
```
## Handles Comments Gracefully
### Example:

```ini
; This is a comment
# This is also a comment
[Section1]
Key1=Value1
```

## API Reference

- `parseFromFile(const std::string& filename)`: Parse an INI file specified by the filename.
- `getValue(const std::string& section, const std::string& key)`: Get the value associated with a key in a specific section.
- `setValue(const std::string& section, const std::string& key, const std::string& value)`: Set the value of a key in a specific section.
- `addSection(const std::string& section)`: Add a new section to the INI file.
- `removeSection(const std::string& section)`: Remove a section and all its contents from the INI file.
- `removeKey(const std::string& section, const std::string& key)`: Remove a key from a specific section.
- `saveToFile(const std::string& filename)`: Save the current state of the INI file to a new file.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
