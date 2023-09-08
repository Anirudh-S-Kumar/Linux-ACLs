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
    if (!Validation::validate_user(user)) return 1;
    if (!Validation::validate_file(file)) return 1;

    // get euid, and check if user is root or owner of file
    #ifdef DEBUG_MODE
        std::cout << getuid() << std::endl;
        std::cout << geteuid() << std::endl;
    #endif


    if (!Validation::verify_owner(getuid(), file)) return 1;

    acl_str.resize(BUFFER_SIZE);
    ssize_t acl_attr = getxattr(file.c_str(), "user.acl",  &acl_str[0], acl_str.size());
    if (acl_attr > 0){
        acl = ACL(acl_attr);
        // ACL list exists, check if the calling user is permitted to change the acl
        if (!Validation::verify_acl(acl, getuid())) return 1;

        // Change to the owner of the file using seteuid, and modifying the acl
        setuid(Misc::get_owner(file));
        acl.add(Misc::uid_from_name(user));
    }
    // if acl doesn't exist, create a new one with the person calling setacl, 
    // and the user passed as argument as the only users
    else if (acl_attr == 0){
        if (!Validation::verify_owner(getuid(), file)) return 1;
        acl.add(getuid()); // adding the user calling setacl
        acl.add(getpwnam(user.c_str())->pw_uid); // adding the user passed as argument
    }

    // storing it in the xattr
    acl_str = acl.serialize();
    
    if (setxattr(file.c_str(), "user.acl", acl_str.c_str(), acl_str.size(), 0) == -1){
        std::cerr << "Error: " << "Could not set ACL for " << file << std::endl;
        return 1;
    }

    return 0;
}