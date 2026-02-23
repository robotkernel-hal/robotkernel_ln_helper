#ifndef LN_HELPER__DATATYPE_H
#define LN_HELPER__DATATYPE_H

#include "ln_helper/helper.h"
#include "ln_helper/util.h"
#include "ln_helper/field.h"

namespace ln_helper {

class datatype {
    public: 
        helper *parent;
        YAML::Node node;
        std::string name;
        std::vector<field> childs; 

    public:
        datatype(helper *parent, const std::string& name, const YAML::Node& config_node) :
            parent(parent), name(name)
        {
            node = YAML::Clone(config_node);

            for (const auto& f : ln_helper::get_as<YAML::Node>(node, "fields")) {
                childs.push_back(field(parent, f.first.as<std::string>(), f.second));
            }
        }
        
        size_t ln_size(void) const {
            size_t ret = 0;

            for (const auto& f : childs) {
                ret += f.ln_size();
            }

            return ret;
        }
};

std::ostream& operator<<(std::ostream& os, const datatype p);
ln_signature_stream& operator<<(ln_signature_stream& os, const datatype p);
ln_mddef_stream& operator<<(ln_mddef_stream& os, const datatype p);

}; // namespace ln_helper

#endif // LN_HELPER__DATATYPE_H
     
