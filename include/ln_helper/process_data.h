#ifndef LN_HELPER__PROCESS_DATA_H
#define LN_HELPER__PROCESS_DATA_H

#include "ln_helper/helper.h"
#include "ln_helper/util.h"
#include "ln_helper/field.h"

namespace ln_helper {

class process_data {
public:
    helper *parent;
    YAML::Node node;
    std::string name;
    std::vector<field> fields;

public:
    process_data(helper *parent, const std::string& name, const YAML::Node& config_node) :
        parent(parent), name(name)
    {
        node = YAML::Clone(config_node);

        for (const auto& f : node) {
            fields.push_back(field(parent, f.first.as<std::string>(), f.second));
        }
    }
};

std::ostream& operator<<(std::ostream& os, const process_data p);
ln_signature_stream& operator<<(ln_signature_stream& os, const process_data p);
ln_mddef_stream& operator<<(ln_mddef_stream& os, const process_data p);

}; // namespace ln_helper

#endif // LN_HELPER__PROCESS_DATA_H


