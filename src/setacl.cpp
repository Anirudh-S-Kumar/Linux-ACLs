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


    bool res = acl.load(file);
    if (res){
        if (!Validation::verify_acl(acl, getuid())) return 1;
        if (seteuid(acl.get_owner()) < 0){
            std::cerr << "Error: " << "Could not change to owner of " << file << std::endl;
            return 1;
        }
    }
    // if acl doesn't exist, create a new one with the person calling setacl, 
    else if (not res){
        if (not Validation::verify_owner(getuid(), file)) return 1;
        acl.set_owner(getuid());
        acl.add(getuid());
    }

    // adding user, storing it in the xattr
    acl.add(Misc::uid_from_name(user));
    res = acl.save(file);

    if (res){
        std::cout << "Successfully added " << user << " to ACL of " << file << std::endl;
    }

    return 0;
}