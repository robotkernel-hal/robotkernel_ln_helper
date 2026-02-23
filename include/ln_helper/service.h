#ifndef LN_HELPER__SERVICE_H
#define LN_HELPER__SERVICE_H

#include "ln_helper/helper.h"
#include "ln_helper/util.h"
#include "ln_helper/field.h"

namespace ln_helper {

class service {
    public:
        helper *parent;
        YAML::Node node;
        std::string name;
        std::vector<field> request;
        std::vector<field> response;

    public:
        service(helper *parent, const std::string& name, const YAML::Node& config_node) :
            parent(parent), name(name)
        {
            node = YAML::Clone(config_node);

            if (node["request"]) {
                for (const auto& f : node["request"]) {
                    request.push_back(field(parent, f.first.as<std::string>(), f.second));
                }
            }
            
            if (node["response"]) {
                for (const auto& f : node["response"]) {
                    response.push_back(field(parent, f.first.as<std::string>(), f.second));
                }
            }
        }
};

std::ostream& operator<<(std::ostream& os, const service p);
ln_signature_stream& operator<<(ln_signature_stream& os, const service p);
ln_mddef_stream& operator<<(ln_mddef_stream& os, const service p);

}; // namespace ln_helper
   
#endif // LN_HELPER__SERVICE_H

