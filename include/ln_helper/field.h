#ifndef LN_HELPER__FIELD_H
#define LN_HELPER__FIELD_H

#include "ln_helper/helper.h"
#include "ln_helper/util.h"

namespace ln_helper {

class field {
    public:
        helper *parent;
        YAML::Node node;
    
        std::string name = "";
        std::string type = "";
        std::string dflt = "";
        bool is_array = false;
        ssize_t size = -1;
        bool is_bitfield = false;
        std::vector<std::pair<std::string, int> > bitfield;

    public:
        field(helper *parent, const std::string& name, const YAML::Node& config_node) :
            parent(parent), name(name)
        {
            node = YAML::Clone(config_node);
            type = ln_helper::get_as<std::string>(node, "type");
            dflt = ln_helper::get_as<std::string>(node, "default", "");
            is_array = ln_helper::get_as<bool>(node, "array", false);
            size = ln_helper::get_as<ssize_t>(node, "size", -1);

            if (node["bitfield"]) {
                is_bitfield = true;
                for (const auto& bt : node["bitfield"]) {
                    bitfield.push_back(std::make_pair<std::string, int>(
                                bt.first.as<std::string>(), get_as<int>(bt.second, "bitsize")));
                }
            }
        }
        
        size_t ln_size(void) const;
};

std::ostream& operator<<(std::ostream& os, const field p);
ln_signature_stream& operator<<(ln_signature_stream& os, const field p);
ln_mddef_stream& operator<<(ln_mddef_stream& os, const field p);

}; // namespace ln_helper
 
#endif // LN_HELPER__FIELD_H
 
