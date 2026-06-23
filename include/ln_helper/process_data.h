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

        if (node.Type() == YAML::NodeType::Scalar) {
            printf("unhandled Scalar datatype\n");
        } else if (node.Type() == YAML::NodeType::Map) {
            // robotkernel pd format
            for (const auto& f : node) {
                fields.push_back(field(parent, f.first.as<std::string>(), f.second));
            }
        } else if (node.Type() == YAML::NodeType::Sequence) {
            unsigned int cnt = 0;

            // lnrk pd format
            for (const auto& f : node) {
                fields.push_back(field(parent, f, cnt));
            }
        } else {
            printf("unknown node type\n");
        }
    }
};

std::ostream& operator<<(std::ostream& os, const process_data p);
ln_signature_stream& operator<<(ln_signature_stream& os, const process_data p);
ln_mddef_stream& operator<<(ln_mddef_stream& os, const process_data p);
rk_def_stream& operator<<(rk_def_stream& os, const process_data p);

}; // namespace ln_helper

#endif // LN_HELPER__PROCESS_DATA_H


