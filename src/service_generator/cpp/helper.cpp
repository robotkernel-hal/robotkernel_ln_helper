#include "ln_helper/helper.h"
#include "ln_helper/datatype.h"
#include "ln_helper/service.h"

using namespace ln_helper;

helper::helper(const YAML::Node config_node) {
    node = YAML::Clone(config_node);

    if (node["datatypes"]) {
        for (const auto& d : node["datatypes"]) {
            datatype *dt = new datatype(this, d);
            dt_map[dt->name] = dt;
        }
    }

    if (node["services"]) {
        for (const auto& d : node["services"]) {
            service *svc = new service(this, d);
            svc_map[svc->name] = svc;
        }
    }
}
