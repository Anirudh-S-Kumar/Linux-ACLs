#include "getacl.h"
#include <validation.h>
#include <misc.h>

#define BUFFER_SIZE 256
#define DEBUG_MODE

int main(int sysc, char** sysv){
    std::string file;
    if (sysc < 2){
        std::cout << "Usage: " << "getacl" << " <file>" << std::endl;
        return 1;
    }

    file = sysv[1];
    if (!Validation::validate_file(file)) return 1;

    std::string acl_str;
    acl_str.resize(BUFFER_SIZE);
    ssize_t acl_attr = getxattr(file.c_str(), "user.acl",  &acl_str[0], acl_str.size());
    if (acl_attr == -1){
        std::cerr << "Error: " << "Could not get ACL for " << file << std::endl;
        return 1;
    }

    ACL acl(acl_str);

    // check if person is allowed to see the ACL
    if (!Validation::verify_acl(acl, getuid())) return 1;
    std::cout << acl << std::endl;    
}