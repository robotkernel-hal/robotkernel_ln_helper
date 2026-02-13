#ifndef LN_MD_HELPER__H
#define LN_MD_HELPER__H

#include <iostream>
#include <map>
#include <algorithm>

#include "yaml-cpp/yaml.h"

namespace ln_md_helper {

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

static bool ends_with(const std::string& a, const std::string& b) {
    if (b.size() > a.size()) return false;
    return std::equal(a.begin() + a.size() - b.size(), a.end(), b.begin());
}


static std::string service_datatype_to_ln(std::string datatype) {
    if (datatype == "string")
        return std::string("char*");
    else if (datatype == "bitarr8_t") 
        return std::string("uint8_t");
    else if (datatype == "bitarr16_t") 
        return std::string("uint16_t");
    else if (datatype == "bitarr32_t") 
        return std::string("uint32_t");

    return datatype;
}

static bool ln_datatype_is_primitive(std::string datatype) {
    if (datatype == "string")
        return false;

    return true;
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

static void rk_datatype_to_ln(const std::string& rk_datatype, std::string& ln_datatype, int& array) 
{
    array = 0; // defaults to scalar
     
    if (rk_datatype == "string") {
        ln_datatype = std::string("char*");
    } else if (ln_datatype_size_map.find(rk_datatype) != ln_datatype_size_map.end()) {
        ln_datatype = rk_datatype;
    } else {
        if (rk_datatype.compare(0, 3, "int") == 0) {
            // decode bits
            size_t pos_1 = 3;
            size_t pos_2 = rk_datatype.find('_');

            std::string bits = rk_datatype.substr(pos_1, pos_2);
        }
    }
}

bool is_builtin_dtype(const std::string& type_name) {
    const std::vector<std::string> builtin_dtypes{
        "int8_t", "int16_t", "int32_t", "int64_t",
        "uint8_t", "uint16_t", "uint32_t", "uint64_t",
        "float", "double", "char", "bool", "string"
    };

    return std::find(builtin_dtypes.begin(), builtin_dtypes.end(), type_name) != builtin_dtypes.end();
}

//struct custom_dtype {
//    public: 
//        const std::string name;
//        const std::vector<struct custom_dtype> childs; 
//
//    public:
//        custom_dtype(const std::string& name) : name(name) {
//            if (!is_builtin_dtype(dtype)) {
//
//            }
//        }
//        
//        void signature(std::stringstream& ss) {
//            if (is_builtin_dtype) {
//                ss_signature << ln_dt << " " << ln_dt_size << " " << "1";
//            }
//        }
//};

static void process_node(const YAML::Node& node,
        std::stringstream& ss_md, std::stringstream& ss_signature,
        std::map<std::string, std::string>& sub_mds,
        std::map<std::string, YAML::Node>& custom_dtypes)
{
    std::map<std::string, std::string> defines_map;

    auto process_entry = [&](const std::string& name, const std::string& dtype, bool is_array = false) -> void {
        std::string clean_name = name;
        std::replace(clean_name.begin(), clean_name.end(), '.', '_');

        if (starts_with(dtype, "vector/") || is_array) {
            std::string real_dtype = dtype;

            const size_t equals_idx = dtype.find_first_of('/');
            if (std::string::npos != equals_idx) { real_dtype = dtype.substr(equals_idx + 1); }
                
            //signature "uint32_t 4 1,[uint32_t 4 1,char* 1 1]* 8 1|uint32_t 4 1,[uint32_t 4 1,char* 1 1]* 8 1"

            bool is_primitive = ln_datatype_is_primitive(real_dtype);

            std::string ln_dt = service_datatype_to_ln(real_dtype);
            int ln_dt_size = ln_datatype_size(ln_dt);

//you     : |uint32_t 4 1,[uint32_t 4 1,[uint32_t 4 1,char* 1 1] 4 1,[uint32_t 4 1,char* 1 1] 4 1,[uint32_t 4 1,char* 1 1] 4 1,[uint32_t 4 1,char* 1 1] 4 1,uint8_t 1 1]* 21 1,[uint32_t 4 1,char* 1 1] 4 1
//provider: |uint32_t 4 1,description* 0 1,uint32_t 4 1,char* 1 1

            if (is_primitive) {
                if (is_builtin_dtype(dtype)) {
                    ss_signature << "uint32_t 4 1," << ln_dt << "* " << ln_dt_size << " 1";
                    ss_md << ln_dt << "* " << clean_name << std::endl;
                } else {
                    printf("try to have a look at custom dtype.. %s\n", dtype.c_str());
                    
                    for (YAML::const_iterator it = custom_dtypes[dtype].begin(); it != custom_dtypes[dtype].end(); ++it) {
                        std::string name = (*it)["name"].as<std::string>();
                        std::string dtype = (*it)["type"].as<std::string>();
                        bool is_array = (*it)["array"] ? (*it)["array"].as<bool>() : false;
                        //process_entry(name, dtype, is_array);
                    }
                }
            } else {
                ss_signature << "uint32_t 4 1,[";

                std::string sub_md_name = std::string("gen/") + dtype;

                if (real_dtype == "string") {
                    // special case, use ln builtin type for that
                    sub_md_name = "ln/string";
                } else {
                    std::stringstream ss_sub_md;
                    ss_sub_md << ln_dt << " data" << std::endl;
                    sub_mds[dtype] = ss_sub_md.str();
                }

                if (defines_map.find(real_dtype) == defines_map.end()) {
                    ss_md << "define " << real_dtype << " as \"" << sub_md_name << "\"" << std::endl;
                    defines_map[real_dtype] = sub_md_name;
                } else if (defines_map[real_dtype] != sub_md_name) {                            
                    std::cout << "error: defines_map[" << real_dtype << "]=" << defines_map[real_dtype] << ", but we need " << sub_md_name << std::endl;
                }

                ss_md << real_dtype << "* " << clean_name << std::endl;

                if (ends_with(ln_dt, std::string("*")))
                    ss_signature << "uint32_t 4 1,";

                ss_signature << ln_dt << " " << ln_dt_size << " " << "1";

                ss_signature << "]* " << sizeof(void*) << " 1";
            }
        } else {
            std::string ln_dt = service_datatype_to_ln(dtype);
            int ln_dt_size = ln_datatype_size(ln_dt);
            ss_md << ln_dt << " " << clean_name << std::endl;

            if (ends_with(ln_dt, std::string("*")))
                ss_signature << "uint32_t 4 1,";

            ss_signature << ln_dt << " " << ln_dt_size << " " << "1";
        }
    };

    YAML::Emitter tmp;
    tmp << node;
    printf("processing: \n%s\n", tmp.c_str());

    for (YAML::const_iterator it = node.begin(); it != node.end(); ++it) {
        if (it != node.begin())
            ss_signature << ",";

        if (it->IsMap() && (*it)["name"]) { // complex format
            std::string name = (*it)["name"].as<std::string>();
            std::string dtype = (*it)["type"].as<std::string>();
            bool is_array = (*it)["array"] ? (*it)["array"].as<bool>() : false;

            process_entry(name, dtype, is_array);
        } else { // simple format
            for (const auto& kv : *it) {
                std::string key   = kv.first.as<std::string>();
                std::string value = kv.second.as<std::string>();
                process_entry(value, key);
            }
        }
    }
}

inline void create_ln_message_defition(const std::string& sd,
        std::map<std::string, std::string>& mds_map) 
{
    std::stringstream ss_md, ss_signature;
    YAML::Node message_definition = YAML::Load(sd);
    ss_md << "service" << std::endl;
    std::string name;
    std::string signature;
    std::string md;
    std::map<std::string, std::string> sub_mds;
    std::map<std::string, YAML::Node> custom_dtypes;

    if (message_definition["name"]) {
        name = message_definition["name"].as<std::string>();
    }

    if (message_definition["request"]) {
        ss_md << "request" << std::endl;

        const YAML::Node& request = message_definition["request"];
        process_node(request, ss_md, ss_signature, mds_map, custom_dtypes);
    }

    ss_signature << "|";

    if (message_definition["response"]) {
        ss_md << "response" << std::endl;
        const YAML::Node& response = message_definition["response"];
        process_node(response, ss_md, ss_signature, mds_map, custom_dtypes);
    }

    signature = ss_signature.str();
    md = ss_md.str();

    mds_map[name] = md;
}

}; // namespace ln_md_helper

#endif // LN_MD_HELPER__H
