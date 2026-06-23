#include "ln_helper/process_data.h"

using namespace ln_helper;

std::ostream& ln_helper::operator<<(std::ostream& os, const ln_helper::process_data p) {
    os << "process_data{name='" << p.name << "', fields=[ " << std::endl;
    for (const auto& f : p.fields) {
        if (f.is_bitfield) {
            for (const auto& bf : f.bitfield) {
                os << "# " << bf.first << ", " << bf.second << " bits" << std::endl;
            }
        }
        os << f << std::endl;
    }
    os << " ]," << std::endl;
    return os;
}

ln_signature_stream& ln_helper::operator<<(ln_signature_stream& os, const ln_helper::process_data p) {
    for (unsigned i = 0; i < p.fields.size(); ++i) {
        field f = p.fields[i];
        os << f;

        if ((i+1) < p.fields.size()) {
            os << ",";
        }
    }

    return os;
}

ln_mddef_stream& ln_helper::operator<<(ln_mddef_stream& os, const ln_helper::process_data p) {
    for (const auto& f : p.fields) {
        os << f << "\n";
    }

    return os;
}

rk_def_stream& ln_helper::operator<<(rk_def_stream& os, const ln_helper::process_data p) {
    for (const auto& f : p.fields) {
        os << f << "\n";
    }

    return os;
}
