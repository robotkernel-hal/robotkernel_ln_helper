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
        //! @brief Construction from robotkernel pd format
        /*!
         * @param parent[in]        Pointer to helper parent.
         * @param name[in]          Field name.
         * @param config_node[in]   Node containing config for field.
         */
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
        
        //! @brief Construction from lnrk pd format
        /*!
         * @param parent[in]        Pointer to helper parent.
         * @param config_node[in]   Node containing lnrk pd config.
         */
        field(helper *parent, const YAML::Node& config_node, unsigned int& cnt) :
            parent(parent)
        {
            for (const auto& entry : config_node) {
                // should be only one
                std::string dtype = entry.first.as<std::string>();
                name = entry.second.as<std::string>();

                unsigned int n_elements = 1;

                if (dtype.substr(dtype.size() - 1, 1) == "]" && dtype.find('[')) {
                    std::string::size_type p = dtype.find('[');
                    std::string n_elements_str = dtype.substr(p + 1, dtype.size() - p - 1 - 1);
                    n_elements = atoi(n_elements_str.c_str());
                    dtype = dtype.substr(0, p);
                }

                if (dtype == "skip_bytes") {
                    size = atoi(name.c_str());
                    type = "uint8_t";
                    name = (stream_helper() << "unused_" << cnt);
                    cnt++;
                } else {
                    size = n_elements;
                    type = dtype;
                }
                    
                if (size <= 1) size = -1;
                else is_array = true;
            }
        }

        size_t ln_size(void) const;
};

std::ostream& operator<<(std::ostream& os, const field p);
ln_signature_stream& operator<<(ln_signature_stream& os, const field p);
ln_mddef_stream& operator<<(ln_mddef_stream& os, const field p);
rk_def_stream& operator<<(rk_def_stream& os, const field p);

}; // namespace ln_helper
 
#endif // LN_HELPER__FIELD_H
 
