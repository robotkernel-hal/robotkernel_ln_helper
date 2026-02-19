#ifndef LN_HELPER__FS_TOOLS_H
#define LN_HELPER__FS_TOOLS_H

#include <string>

namespace ln_helper {

bool is_dir_exist(const std::string& path);
bool make_path(const std::string& path);

}; // namespace ln_helper;

#endif // LN_HELPER__FS_TOOLS_H
 
