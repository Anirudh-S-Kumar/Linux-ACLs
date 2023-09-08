#include "setacl.h"

#define BUFFER_SIZE 256
#define DEBUG_MODE

int main(int argc, char** argv){
    std::string user;
    std::string file;
    std::string acl_str;
    ACL acl;

    if (argc < 3){
        std::cout << "Usage: " << "setacl" << " <user> <file>" << std::endl;
        return 1;
    }

    user = argv[1];
    file = argv[2];

    // check for valid user and file
    Validation validation;
    if (!validation.validate_user(user)) return 1;
    if (!validation.validate_file(file)) return 1;

    // get euid, and check if user is root or owner of file
    #ifdef DEBUG_MODE
        std::cout << getuid() << std::endl;
        std::cout << geteuid() << std::endl;
    #endif


    if (!validation.verify_owner(getuid(), file)) return 1;

    // TODO: Check the ACL list as well
    acl_str.resize(BUFFER_SIZE);
    ssize_t acl_attr = getxattr(file.c_str(), "user.acl",  &acl_str[0], acl_str.size());
    if (acl_attr == -1){
        acl.add(getuid());
    }
    else{
        acl = ACL(acl_attr);
        std::cout << acl << std::endl;
        if (!validation.verify_acl(acl, user)) return 1;
    }
    
    // adding user to ACL, storing it in the xattr
    acl.add(getpwnam(user.c_str())->pw_uid);
    acl_str = acl.serialize();
    
    if (setxattr(file.c_str(), "user.acl", acl_str.c_str(), acl_str.size(), 0) == -1){
        std::cerr << "Error: " << "Could not set ACL for " << file << std::endl;
        return 1;
    }

    return 0;
}