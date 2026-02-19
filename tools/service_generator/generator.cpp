#include "config.h"
#include <dlfcn.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include <errno.h>

#include <list>
#include <fstream>
typedef void (*get_sd_t)(std::list<std::string>& sd_list);


#include "ln_helper/field.h"
#include "ln_helper/datatype.h"
#include "ln_helper/service.h"
#include "ln_helper/fs_tools.h"

using namespace std;
using namespace ln_helper;

int usage(void) {
    printf("usage: service_generator --input | -i <filename> [-v]\n");
    printf("\n");
    printf("  --input-file, -i <filename> file to get service definitions from\n");
    printf("  --output-dir, -o <dirname>  directory to place ln service files to\n");
    printf("  --verbose, -v               be more verbose\n");
    return 0;
}

int main(int argc, char **argv) {
    void *so_handle;
    list<string> input_file_names;
    string output_dir_name;
    bool verbose = false;

    for (int i = 1; i < argc; ++i) {
        if ((strcmp(argv[i], "--input-file") == 0) || (strcmp(argv[i], "-i") == 0)) {
            if (++i >= argc) {
                printf("--input-file filename missing\n");
                return usage();
            }

            input_file_names.push_back(string(argv[i]));
        } else if ((strcmp(argv[i], "--output-dir") == 0) || (strcmp(argv[i], "-o") == 0)) {
            if (++i >= argc) {
                printf("--output-dir dirname missing\n");
                return usage();
            }

            output_dir_name = string(argv[i]);
            if (!output_dir_name.empty() && (output_dir_name.back() == '/' || output_dir_name.back() == '\\')) {
                output_dir_name.pop_back(); // entfernt das letzte Zeichen
            }
        } else if ((strcmp(argv[i], "--verbose") == 0) || (strcmp(argv[i], "-v") == 0)) {
            verbose = true;
        }
    }

    if (input_file_names.size() == 0) {
        return usage();
    }

    std::cout << "robotkernel service generator for links-and-nodes" << std::endl;

    for (const auto& input_file_name : input_file_names) {
        std::cout << "loading file \"" << input_file_name << "\"" << std::endl;

        YAML::Node node = YAML::LoadFile(input_file_name);

        helper h(node);

        for (const auto& kv : h.svc_map) {
            service *svc = kv.second;
            ln_signature_stream lss;
            ln_mddef_stream mdss;

            stringstream ss;
            ss << *svc;
            lss << *svc;
            mdss << *svc;

            auto create_md_file = [&](std::string mddef_name, const ln_mddef_stream& mdss) {
                string file_name = mddef_name;
                string dir_name = output_dir_name;
                for (size_t pos = 0; (pos = file_name.find('/')) != std::string::npos; ) {
                    dir_name += "/" + file_name.substr(0, pos);
                    file_name = file_name.substr(pos + 1);
                }

                make_path(dir_name);
                file_name = dir_name + "/" + file_name;
                std::cout << "processing md \"" << mddef_name << "\", creating " << file_name << std::endl;

                std::ofstream file(file_name);
                file << mdss.str();
                file.close();
            };

            create_md_file(svc->name, mdss);

            for (const auto& kv : mdss.seen_defines) {
                if (h.dt_map.find(kv.first) != h.dt_map.end()) {
                    ln_mddef_stream dtmdss;
                    dtmdss << *h.dt_map[kv.first];

                    create_md_file(kv.first, dtmdss);
                }
            }
        }
    }
    
    return 0;
}

