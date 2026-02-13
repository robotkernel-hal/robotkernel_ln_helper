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
        service(helper *parent, const YAML::Node& config_node) :
            parent(parent)
        {
            node = YAML::Clone(config_node);
            name = ln_helper::get_as<std::string>(node, "name");

            if (node["request"]) {
                for (const auto& f : node["request"]) {
                    request.push_back(field(parent, f));
                }
            }
            
            if (node["response"]) {
                for (const auto& f : node["response"]) {
                    response.push_back(field(parent, f));
                }
            }
        }
};

std::ostream& operator<<(std::ostream& os, const service p);
ln_signature_stream& operator<<(ln_signature_stream& os, const service p);
ln_mddef_stream& operator<<(ln_mddef_stream& os, const service p);

}; // namespace ln_helper
   
#endif // LN_HELPER__SERVICE_H

