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
#include "ln_helper/process_data.h"
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
    string indir, outdir;
    list<string> infns;

    for (int i = 1; i < argc; ++i) {
        if ((strcmp(argv[i], "--indir") == 0) || (strcmp(argv[i], "-i") == 0)) {
            if (++i >= argc) {
                printf("--indir filename missing\n");
                return usage();
            }

            indir = string(argv[i]);
        } else if ((strcmp(argv[i], "--outdir") == 0) || (strcmp(argv[i], "-o") == 0)) {
            if (++i >= argc) {
                printf("--outdir dirname missing\n");
                return usage();
            }

            outdir = string(argv[i]);
            if (!outdir.empty() && (outdir.back() == '/' || outdir.back() == '\\')) {
                outdir.pop_back(); // entfernt das letzte Zeichen
            }
        } else {
            infns.push_back(argv[i]);
        }
    }

    if ((indir == "") || (outdir == "") || (infns.size() == 0)) {
        return usage();
    }

    std::cout << "robotkernel service generator for links-and-nodes" << std::endl;

    for (const auto& input_file_name : infns) {
        string fn = indir + "/" + input_file_name;
        std::cout << "loading file \"" << fn << "\"" << std::endl;

        YAML::Node node = YAML::LoadFile(fn);

        helper h(node);
            
        auto create_md_file = [&](std::string mddef_name, const ln_mddef_stream& mdss) {
            string file_name = mddef_name;
            string dir_name = outdir;
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

        for (const auto& kv : h.svc_map) {
            service *svc = kv.second;
            ln_signature_stream lss;
            ln_mddef_stream mdss;

            stringstream ss;
            ss << *svc;
            lss << *svc;
            mdss << *svc;

            create_md_file(svc->name, mdss);

            for (const auto& kv : mdss.seen_defines) {
                if (h.dt_map.find(kv.first) != h.dt_map.end()) {
                    ln_mddef_stream dtmdss;
                    dtmdss << *h.dt_map[kv.first];

                    create_md_file(kv.first, dtmdss);
                }
            }
        }

        for (const auto& kv : h.dt_map) {
            datatype *dt = kv.second;
            ln_mddef_stream mdss;
            mdss << *dt;

            create_md_file(dt->name, mdss);
        }

        for (const auto& kv : h.pd_map) {
            process_data *pd = kv.second;
            ln_mddef_stream mdss;
            mdss << *pd;

            create_md_file(pd->name, mdss);
        }
    }
    
    return 0;
}

