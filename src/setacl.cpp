#include "setacl.h"

#define BUFFER_SIZE 1024
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
    if (!Validation::validate_user(user)) return 1;
    if (!Validation::validate_file(file)) return 1;


    acl_str.resize(BUFFER_SIZE);
    ssize_t acl_attr = getxattr(file.c_str(), "user.acl",  &acl_str[0], BUFFER_SIZE);
    if (acl_attr >= 0){
        acl = ACL(acl_str);
        // ACL list exists, check if the calling user is permitted to change the acl
        if (!Validation::verify_acl(acl, getuid())) return 1;
        // Change to the owner of the file using seteuid
        #ifdef DEBUG_MODE
            std::cout << acl << std::endl;
            std::cout << "Current UID: " << getuid() << std::endl;
            std::cout << "Owner UID: " << acl.get_owner() << std::endl;
        #endif
        
        if (setuid(acl.get_owner()) < 0){
            std::cout << setuid(acl.get_owner()) << std::endl;
            std::cerr << "Error: " << "Could not change to owner of " << file << std::endl;
            return 1;
        }
    }
    // if acl doesn't exist, create a new one with the person calling setacl, 
    else if (acl_attr == -1){
        if (!Validation::verify_owner(getuid(), file)) return 1;
        acl.set_owner(getuid());
        acl.add(getuid());
    }

    // adding user, storing it in the xattr
    acl.add(Misc::uid_from_name(user));
    std::string write_acl_str = acl.serialize();

    #ifdef DEBUG_MODE
        std::cout << "Current UID: " << getuid() << std::endl;
    #endif

    if (setxattr(file.c_str(), "user.acl", write_acl_str.c_str(), write_acl_str.size(), 0) == -1){
        std::cerr << "Error: " << "Could not set ACL for " << file << std::endl;
        return 1;
    }

    return 0;
}