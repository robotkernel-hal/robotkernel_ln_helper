#include "ln_helper/datatype.h"

using namespace ln_helper;

std::ostream& ln_helper::operator<<(std::ostream& os, const ln_helper::datatype p) {
    os << "datatype{name='" << p.name << "', fields=[ " << std::endl;
    for (unsigned i = 0; i < p.childs.size(); ++i) {
        auto f = p.childs[i];
        os << f << std::endl;
    }
    os << " ]" << std::endl;
    return os;
}

ln_signature_stream& ln_helper::operator<<(ln_signature_stream& os, const ln_helper::datatype p) {
    for (unsigned i = 0; i < p.childs.size(); ++i) {
        auto f = p.childs[i];
        os << f;

        if ((i+1) < p.childs.size()) {
            os << ",";
        }
    }

    return os;
}

ln_mddef_stream& ln_helper::operator<<(ln_mddef_stream& os, const ln_helper::datatype p) {
    for (unsigned i = 0; i < p.childs.size(); ++i) {
        auto f = p.childs[i];
        os << f;

        if ((i+1) < p.childs.size()) {
            os << "\n";
        }
    }
    return os;
}
