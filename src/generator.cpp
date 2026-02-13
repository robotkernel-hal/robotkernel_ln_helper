#include "config.h"
#include <dlfcn.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include <errno.h>

#include <list>
//#include "ln_md_helper.h"
#include <fstream>
typedef void (*get_sd_t)(std::list<std::string>& sd_list);

#include "fs_tools.h"

#include "ln_helper/field.h"
#include "ln_helper/datatype.h"
#include "ln_helper/service.h"

using namespace std;
//using namespace ln_md_helper;
using namespace ln_helper;

int usage(void) {
    printf("usage: service_generator --input | -i <filename> [-v]\n");
    printf("\n");
    printf("  --input_file, -i <filename> file to get service definitions from\n");
    printf("  --verbose, -v               be more verbose\n");
    return 0;
}

int main(int argc, char **argv) {
    void *so_handle;
    string input_file_name;
    bool verbose = false;

    for (int i = 1; i < argc; ++i) {
        if ((strcmp(argv[i], "--input") == 0) || (strcmp(argv[i], "-i") == 0)) {
            if (++i >= argc) {
                printf("--input filename missing\n");
                return usage();
            }

            input_file_name = string(argv[i]);

        } else if ((strcmp(argv[i], "--verbose") == 0) || (strcmp(argv[i], "-v") == 0)) {
            verbose = true;
        }
    }

    if (input_file_name == "") {
        return usage();
    }

    printf( "service_generator\n" );

    YAML::Node node = YAML::LoadFile(input_file_name);

    helper h(node);

    for (const auto& kv : h.svc_map) {
        service *svc = kv.second;
        ln_signature_stream lss;
        ln_mddef_stream mdss;

        stringstream ss;
        ss << *svc;
        printf("got service \n\"%s\"\n", ss.str().c_str());

        lss << *svc;
        printf("got sig\n%s\n", lss.str().c_str());

        mdss << *svc;
        printf("\ngot mddef\n%s\n", mdss.str().c_str());

        for (const auto& kv : mdss.seen_defines) {
            printf("\nalso need \"%s\"\n", kv.first.c_str());;

            if (h.dt_map.find(kv.first) != h.dt_map.end()) {
                ln_mddef_stream dtmdss;
                dtmdss << *h.dt_map[kv.first];
                printf("%s\n", dtmdss.str().c_str());
            }
        }
    }
#if 0
    if (node["datatypes"]) {
        for (const auto& d : node["datatypes"]) {
            ln_signature_stream lss;

            datatype *dt = new datatype(&h, d);
            h.dt_map[dt->name] = dt;

            stringstream ss;
            ss << *dt;
            printf("got datatype \n\"%s\"\n", ss.str().c_str());

            lss << *dt;
            printf("got sig\n%s\n", lss.str().c_str());
        }
    }
    
    if (node["services"]) {
        for (const auto& d : node["services"]) {
            ln_signature_stream lss;

            service *svc = new service(&h, d);
            h.svc_map[svc->name] = svc;

            stringstream ss;
            ss << *svc;
            printf("got service \n\"%s\"\n", ss.str().c_str());

            lss << *svc;
            printf("got sig\n%s\n", lss.str().c_str());
        }
    }
#endif

#if 0
    string name = ln_md_helper::get_as<string>(node, "name");

    if (node["request"]) {
        printf("got request field\n");

        printf("typedef struct %s_request {\n", name.c_str());

        for (const auto& entry : node["request"]) {
            for (const auto& kv : entry) {
                printf("  %s %s;\n", kv.first.as<string>().c_str(), kv.second.as<string>().c_str());
                
            }
        }
        printf("} %s_request_t;\n", name.c_str());
    }

    if (node["response"]) {
        printf("got response field\n");
    }

    printf("\n");
    printf("class %s_service_base : \n"
            "    public robotkernel::service\n"
            "{\n"
            "    public:\n"
            "        virtual void %s_handler(const %s_request_t& req, %s_response_t& resp) = 0;\n"
            "};\n\n", name.c_str(), name.c_str(), name.c_str(), name.c_str());
#endif
    
    return 0;
}

