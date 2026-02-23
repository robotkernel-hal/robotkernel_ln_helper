#ifndef LN_HELPER__HELPER_H
#define LN_HELPER__HELPER_H

#include <map>

#include "yaml-cpp/yaml.h"

namespace ln_helper {

// forward decls
class datatype;
class service;

class helper {
    public:
        YAML::Node node;
        std::map<std::string, datatype *> dt_map;
        std::map<std::string, service *> svc_map;

    public:
        helper(const YAML::Node config_node = YAML::Node());

        service *add_service(const std::string& name, const YAML::Node& service_node);
        datatype *add_datatype(const std::string& name, const YAML::Node& datatype_node);
};

}; // namespace helper

#endif // LN_HELPER__HELPER_H

