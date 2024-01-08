#include "config.h"
#include <dlfcn.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include <errno.h>

#include <list>
#include "ln_md_helper.h"
#include <fstream>
typedef void (*get_sd_t)(std::list<std::string>& sd_list);

#include "fs_tools.h"

using namespace std;

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

    string name = node["name"].as<string>();

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
    
    return 0;
}

