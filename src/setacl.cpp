#include "setacl.h"

#define BUFFER_SIZE 1024
// #define DEBUG_MODE

int main(int argc, char** argv){
    std::string user;
    std::string file;
    std::string perms;
    std::string acl_str;
    ACL acl;

    if (argc < 4){
        std::cout << "Usage: " << "setacl" << "-[rwx] <user> <file>" << std::endl;
        return 1;
    }
    perms = argv[1];
    user = argv[2];
    file = argv[3];

    // check for valid user and file
    if (!Validation::validate_user(user)) return 1;
    if (!Validation::validate_file(file)) return 1;


    #ifdef DEBUG_MODE
        std::cout << "Before loading ACL" << std::endl;
        std::cout << "UID: " << getuid() << std::endl;
        std::cout << "EUID: " << geteuid() << std::endl;
    #endif

    if (seteuid(Misc::get_owner(file)) < 0){
            std::cerr << "Owner of file: " << Misc::get_owner(file) << std::endl;
            std::cerr << "Error: " << "Could not change to owner of " << file << std::endl;
            return 1;
    }

    #ifdef DEBUG_MODE
        std::cout << "After changing to owner" << std::endl;
        std::cout << "UID: " << getuid() << std::endl;
        std::cout << "EUID: " << geteuid() << std::endl;
    #endif

    bool res = acl.load(file);
    if (res){
        if (!Validation::verify_acl(acl, getuid(), "w")) return 1;
    }
    // if acl doesn't exist, create a new one with the person calling setacl, 
    else if (not res){
        if (not Validation::verify_owner(getuid(), file)) return 1;
        acl.set_owner(getuid());
        acl.add(getuid(), 7);
    }

    

    // adding user, storing it in the xattr
    acl.add(Misc::uid_from_name(user), Misc::gen_perm(perms));
    res = acl.save(file);

    if (res){
        std::cout << "Successfully added " << user << " to ACL of " << file << std::endl;
    }

    // change back to the original user
    if (seteuid(getuid()) < 0){
            std::cerr << "Error: " << "Could not change to owner of " << file << std::endl;
            return 1;
    }

    #ifdef DEBUG_MODE
        std::cout << "After saving ACL" << std::endl;
        std::cout << "UID: " << getuid() << std::endl;
        std::cout << "EUID: " << geteuid() << std::endl;
    #endif

    return 0;
}