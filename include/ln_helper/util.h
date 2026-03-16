#ifndef LN_HELPER__UTIL_H
#define LN_HELPER__UTIL_H

#include <string>
#include <algorithm>
#include <iostream>

#include "yaml-cpp/yaml.h"

namespace ln_helper {

class ln_mddef_stream {
private:
    std::string buffer;
    
public:
    std::map<std::string, bool> seen_defines;

public:
    // Stream-ähnliche Methoden
    ln_mddef_stream& operator<<(const std::string& s) {
        buffer += s;
        return *this;
    }
    
    ln_mddef_stream& operator<<(const char* s) {
        buffer += s;
        return *this;
    }
    
    template <typename T>
    ln_mddef_stream& operator<<(T n) {
        buffer += std::to_string(n);
        return *this;
    }

    // Optional: Ausgabe in std::cout
    void flush() const {
        std::cout << buffer;
    }

    // Optional: Rückgabe als std::string
    std::string str() const {
        return buffer;
    }

    // Optional: Clear
    void clear() {
        buffer.clear();
    }
};

class ln_signature_stream {
private:
    std::string buffer;

public:
    // Stream-ähnliche Methoden
    ln_signature_stream& operator<<(const std::string& s) {
        buffer += s;
        return *this;
    }
    
    ln_signature_stream& operator<<(const char* s) {
        buffer += s;
        return *this;
    }
    
    template <typename T>
    ln_signature_stream& operator<<(T n) {
        buffer += std::to_string(n);
        return *this;
    }

    // Optional: Ausgabe in std::cout
    void flush() const {
        std::cout << buffer;
    }

    // Optional: Rückgabe als std::string
    std::string str() const {
        return buffer;
    }

    // Optional: Clear
    void clear() {
        buffer.clear();
    }
};

/**
 * @brief Retrieve a typed value from a YAML node.
 *
 * Extracts a value associated with the given key and converts it
 * to the requested type.
 *
 * @tparam type
 *         Target type for conversion.
 *
 * @param node
 *        YAML node containing the key.
 *
 * @param key
 *        Key to look up in the node.
 *
 * @return
 *        Value converted to the requested type.
 *
 * @throws YAML::Exception
 *         If the key does not exist or the value cannot be converted.
 *
 * @note
 * This function is strict:
 * - Missing keys result in an exception.
 * - Type mismatches result in an exception.
 */
template <typename type>
type get_as(const YAML::Node& node, const std::string key) {
    if (!node[key]) {
        YAML::Emitter out;
        out << node;

        std::stringstream ss;
        ss << "ERROR: key \"" << key << "\" not fount!\n\n" << out.c_str() << std::endl;
        throw std::runtime_error(ss.str());
    }

    try {
        // gcc3.3 need this syntax for calling this template
        return node[key].template as<type>();
    } catch(const std::exception& e) {
        std::stringstream ss;
        ss << "ERROR: key \"" << key << "\" is probably of wrong data-type:\n" << e.what() << std::endl;
        throw std::runtime_error(ss.str());
    }
}

/**
 * @brief Retrieve a typed value from a YAML node with a default fallback.
 *
 * Attempts to extract a value associated with the given key and convert
 * it to the requested type. If the key does not exist or conversion fails,
 * the provided default value is returned.
 *
 * @tparam type
 *         Target type for conversion.
 *
 * @param node
 *        YAML node containing the key.
 *
 * @param key
 *        Key to look up in the node.
 *
 * @param dflt
 *        Default value to return if the key is missing or invalid.
 *
 * @return
 *        Extracted value or the default.
 *
 * @note
 * This function is non-throwing for missing keys.
 *
 * @example
 * @code
 * int timeout = get_as<int>(cfg, "timeout", 1000);
 * @endcode
 */
template <typename type>
type get_as(const YAML::Node& node, const std::string key, type dflt) {
    if (!node[key])
        return dflt;

    try {
        // gcc3.3 need this syntax for calling this template 
        return node[key].template as<type>();
    } catch(const std::exception& e) {
        std::stringstream ss;
        ss << "ERROR: key \"" << key << "\" is probably of wrong data-type:\n" << e.what() << std::endl;
        throw std::runtime_error(ss.str());
    }   
}

// Checks whether `str' starts with `start' ignoring case
static bool starts_with(const std::string& str, const std::string& start) {
    if (&start == &str) 
        return true; // str and start are the same string

    if (start.length() > str.length()) 
        return false;

    for (size_t i = 0; i < start.length(); ++i) {
        if (start[i] != str[i]) 
            return false;
    }

    return true;
}

static bool is_builtin_dtype(const std::string& type_name) {
    const std::vector<std::string> builtin_dtypes{
        "int8_t", "int16_t", "int32_t", "int64_t",
        "uint8_t", "uint16_t", "uint32_t", "uint64_t",
        "float", "double", "char", "bool", "string"
    };

    return std::find(builtin_dtypes.begin(), builtin_dtypes.end(), type_name) != builtin_dtypes.end();
}

static std::pair<std::string, int> ln_datatype_size_data[] = {
    std::make_pair("int64_t", 8),
    std::make_pair("uint64_t", 8),
    std::make_pair("int32_t", 4),
    std::make_pair("uint32_t", 4),
    std::make_pair("int16_t", 2),
    std::make_pair("uint16_t", 2),
    std::make_pair("int8_t", 1),
    std::make_pair("uint8_t", 1),
    std::make_pair("int64_t*", 8),
    std::make_pair("uint64_t*", 8),
    std::make_pair("int32_t*", 4),
    std::make_pair("uint32_t*", 4),
    std::make_pair("int16_t*", 2),
    std::make_pair("uint16_t*", 2),
    std::make_pair("int8_t*", 1),
    std::make_pair("uint8_t*", 1),
    std::make_pair("float", 4),
    std::make_pair("double", 8),
    std::make_pair("char*", 1)
};

static std::map<std::string, int> ln_datatype_size_map(
        ln_datatype_size_data,
        ln_datatype_size_data + sizeof(ln_datatype_size_data) / 
        sizeof(ln_datatype_size_data[0]));

static int ln_datatype_size(const std::string& ln_datatype) {
    if (ln_datatype_size_map.find(ln_datatype) != 
            ln_datatype_size_map.end())
        return ln_datatype_size_map[ln_datatype];

    return 0;
}

static std::string get_last_part(const std::string& path) {
    std::istringstream iss(path);
    std::string part;
    std::string lastPart;

    // Alle Teile durch '/' trennen
    while (std::getline(iss, part, '/')) {
        lastPart = part; // Das letzte Teil bleibt übrig
    }

    return lastPart;
}

}; // namespace ln_helper
   
#endif // LN_HELPER__UTIL_H

