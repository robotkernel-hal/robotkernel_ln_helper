#include "ln_helper/helper.h"
#include "ln_helper/datatype.h"
#include "ln_helper/service.h"
#include "ln_helper/process_data.h"

using namespace ln_helper;

helper::helper(const YAML::Node config_node) {
    node = YAML::Clone(config_node);

    if (node["datatypes"]) {
        for (const auto& d : node["datatypes"]) {
            add_datatype(d.first.as<std::string>(), d.second);
        }
    }

    if (node["services"]) {
        for (const auto& d : node["services"]) {
            add_service(d.first.as<std::string>(), d.second);
        }
    }

    if (node["pds"]) {
        for (const auto& d : node["pds"]) {
            add_pd(d.first.as<std::string>(), d.second);
        }
    }
}
        
process_data *helper::add_pd(const std::string& name, const YAML::Node& pd_node) {
    process_data *pd = new process_data(this, name, pd_node);
    pd_map[pd->name] = pd;
    return pd;
}

service *helper::add_service(const std::string& name, const YAML::Node& service_node) {
    service *svc = new service(this, name, service_node);
    svc_map[svc->name] = svc;
    return svc;
}

datatype *helper::add_datatype(const std::string& name, const YAML::Node& datatype_node) {
    datatype *dt = new datatype(this, name, datatype_node);
    dt_map[dt->name] = dt;
    return dt;
}
