#ifndef VALIDATION_H
#define VALIDATION_H

#include <iostream>
#include <string>
#include <filesystem>
#include <unistd.h>
#include <sys/stat.h>
#include <pwd.h>
#include "acl.h"

class Validation {
public:
    bool validate_user(std::string user);
    bool validate_file(std::string file);
    bool verify_owner(uid_t uid, std::string file);
    bool verify_acl(ACL acl, std::string user);
};


#endif