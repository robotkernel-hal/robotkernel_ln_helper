#include "ln_helper/helper.h"
#include "ln_helper/datatype.h"
#include "ln_helper/service.h"

using namespace ln_helper;

helper::helper(const YAML::Node config_node) {
    node = YAML::Clone(config_node);

    if (node["datatypes"]) {
        for (const auto& d : node["datatypes"]) {
            add_datatype(d);
        }
    }

    if (node["services"]) {
        for (const auto& d : node["services"]) {
            add_service(d);
        }
    }
}
        
service *helper::add_service(const YAML::Node& service_node) {
    service *svc = new service(this, service_node);
    svc_map[svc->name] = svc;
    return svc;
}

datatype *helper::add_datatype(const YAML::Node& datatype_node) {
    datatype *dt = new datatype(this, datatype_node);
    dt_map[dt->name] = dt;
    return dt;
}
