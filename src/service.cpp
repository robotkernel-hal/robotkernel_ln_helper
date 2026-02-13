#include "ln_helper/service.h"

using namespace ln_helper;

std::ostream& ln_helper::operator<<(std::ostream& os, const ln_helper::service p) {
    os << "service{name='" << p.name << "', request=[ " << std::endl;
    for (const auto& f : p.request) {
        os << f << std::endl;
    }
    os << " ]," << std::endl;
    os << "response=[ " << std::endl;
    for (unsigned i = 0; i < p.response.size(); ++i) {
        field f = p.response[i];
        os << f << std::endl;

        if ((i+1) < p.response.size()) {
            os << ",";
        }
    }
    os << " ]" << std::endl;
    return os;
}

ln_signature_stream& ln_helper::operator<<(ln_signature_stream& os, const ln_helper::service p) {
    for (const auto& f : p.request) {
        os << f;
    }
    for (unsigned i = 0; i < p.response.size(); ++i) {
        field f = p.response[i];
        os << f;

        if ((i+1) < p.response.size()) {
            os << ",";
        }
    }
    return os;
}

ln_mddef_stream& ln_helper::operator<<(ln_mddef_stream& os, const ln_helper::service p) {
    os << "service\n";

    os << "request\n";
    for (const auto& f : p.request) {
        os << f << "\n";
    }

    os << "response\n";
    for (const auto& f : p.response) {
        os << f << "\n";
    }


    return os;
}
