#include "ln_helper/field.h"
#include "ln_helper/datatype.h"

#include <iostream>
#include <sstream>

using namespace ln_helper;

std::ostream& ln_helper::operator<<(std::ostream& os, const ln_helper::field p) {
    os << "field{name='" << p.name << "', type='" << p.type << "'"; 
    if (p.dflt != "") {
        os << ", dflt='" << p.dflt << "'";
    }

    if (p.is_array) {
        os << ", array=true";
    }
    
    if (p.size >= 0) {
        os << ", size=" << p.size;
    }

    os << "}";
    return os;
}

ln_signature_stream& ln_helper::operator<<(ln_signature_stream& os, const ln_helper::field p) {
    size_t array_size = 0;

    if (p.type == "string") { // special case for ln/string
        if (p.is_array) {
            os << "uint32_t 4 1,[uint32_t 4 1,char* 1 1]* 4 1";
        } else {
            os << "uint32_t 4 1,char* 1 1";
        }
        array_size += 4;
    } else if (!is_builtin_dtype(p.type)) {
        if (p.is_array) {
            os << "uint32_t 4 1,["; // size field
        }

        if (p.parent->dt_map.find(p.type) != p.parent->dt_map.end()) {
            auto& f = *p.parent->dt_map[p.type];
            os << f;
            array_size += f.ln_size();
        } else {
            std::stringstream ss;
            ss << "GOT NOT BUILTIN TYPE: " << p.type << " -> NOT FOUND!!!\n";
            throw std::runtime_error(ss.str());
        }
    
        if (p.is_array) {
            os << "]* " << array_size << " 1";
        }
    } else {
        ssize_t dtype_size = ln_datatype_size(p.type);
        if (p.is_array) {
            os << "uint32_t 4 1," << p.type <<  "* " << dtype_size << " 1";
        } else {
            os << p.type << " " << dtype_size << " 1";
        }
    }

    return os;
}

ln_mddef_stream& ln_helper::operator<<(ln_mddef_stream& os, const ln_helper::field p) {
    std::string array_marker = "";

    if (p.is_bitfield) {
        for (const auto& bf : p.bitfield) {
            os << "# " << bf.first << ", " << bf.second << " bits\n";
        }
    }

    if (p.is_array) {
        array_marker = "*";
    }

    if (p.type == "string") { // special case for ln/string
        if (p.is_array) { // use ln/string type
            if (os.seen_defines.find("ln/string") == os.seen_defines.end()) {
                os << "define string as \"ln/string\"\n";
                os.seen_defines["ln/string"] = true;
            }

            os << "string" << array_marker << " " << p.name;
        } else {
            os << "char* " << p.name;
        }
    } else if (!is_builtin_dtype(p.type)) {
        std::string local_dtype = get_last_part(p.type);
        if (os.seen_defines.find(p.type) == os.seen_defines.end()) {
            os << "define " << local_dtype << " as \"" << p.type << "\"\n";
            os.seen_defines[p.type] = true;
        }
        
        if (p.size > 0) {
            os << local_dtype << " " << p.name << "[" << p.size << "]";
        } else {
            os << local_dtype << array_marker << " " << p.name;
        }
    } else if (p.type == "bool") {
        if (p.size > 0) {
            os << "uint8_t " << p.name << "[" << p.size << "]";
        } else {
            os << "uint8_t" << array_marker << " " << p.name;
        }
    } else {
        if (p.size > 0) {
            os << p.type << " " << p.name << "[" << p.size << "]";
        } else {
            os << p.type << array_marker << " " << p.name;
        }
    }
    return os;
}

size_t field::ln_size(void) const {
    size_t ret = 0;

    if (type == "string") { // special case for ln/string
        ret = 4;
    } else if (is_builtin_dtype(type)) {
        ret = ln_datatype_size(type);
    } else {
        if (parent->dt_map.find(type) != parent->dt_map.end()) {
            ret = parent->dt_map[type]->ln_size();
        } else {
            // TODO throw smth.
        }
    }

    return ret;
}

