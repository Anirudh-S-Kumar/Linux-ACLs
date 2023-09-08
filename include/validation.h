#ifndef VALIDATION_H
#define VALIDATION_H

#include <iostream>
#include <string>
#include <filesystem>
#include <unistd.h>
#include <sys/stat.h>
#include <pwd.h>
#include "acl.h"
#include "misc.h"

namespace Validation {
    bool validate_user(std::string user);
    bool validate_file(std::string file);
    bool verify_owner(uid_t uid, std::string file);
    bool verify_acl(ACL acl, std::string user);
    bool verify_acl(ACL acl, int user);
};


#endif