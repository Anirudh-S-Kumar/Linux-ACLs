#ifndef MISC_H
#define MISC_H

#include <iostream>
#include <string>
#include <filesystem>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/xattr.h>
#include <pwd.h>

namespace Misc{
    std::string full_path(std::string file);
    std::string name_from_uid(uid_t uid);
    uid_t uid_from_name(std::string name);
    uid_t get_owner(std::string file);
    int gen_perm(std::string perm);
    std::string print_perm(int perm);
}

#endif