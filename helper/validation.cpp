#include "validation.h"

#define DEBUG_MODE

bool Validation::validate_user(std::string user){
    /*
    * check if user exists, else exit
    */
    if (getpwnam(user.c_str()) == NULL){
        std::cerr << "Error: " << user << " does not exist" << std::endl;
        return false;
    }
    return true;
}

bool Validation::validate_file(std::string file){
    /*
    * check if file exists and is not a directory, else exit
    */    
    std::filesystem::path full_path = std::filesystem::absolute(file);

    struct stat sb;
    if ((stat(full_path.c_str(), &sb) == 0) && S_ISREG(sb.st_mode)){
        return true;
    } else {
        std::cerr << "Error: " << full_path.c_str() << " is not a file" << std::endl;
    }
    return false;
}

bool Validation::verify_owner(uid_t uid, std::string file){
    std::filesystem::path full_path = std::filesystem::absolute(file);

    struct stat sb;
    stat(full_path.c_str(), &sb);

    #ifdef DEBUG_MODE
        std::cerr << "uid: " << uid << std::endl;
        std::cerr << "sb.st_uid: " << sb.st_uid << std::endl;
    #endif

    if (sb.st_uid == uid){
        return true;
    } else {
        std::cerr << "Error: " << "You do not have the permissions to edit ACL for " << file << std::endl;
    }
    return false;
}

bool Validation::verify_acl(ACL acl, std::string user){
    if (acl.check(getpwnam(user.c_str())->pw_uid)){
        return true;
    } else {
        std::cerr << "Error: " << user << " does not have the permissions to edit ACL for " << std::endl;
    }
    return false;
}