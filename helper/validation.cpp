#include "validation.h"
// #define DEBUG_MODE


namespace Validation{
    bool validate_user(std::string user){
        /*
        * check if user exists, else exit
        */
        if (getpwnam(user.c_str()) == NULL){
            std::cerr << "Error: " << user << " does not exist" << std::endl;
            return false;
        }
        return true;
    }

    bool validate_file(std::string file){
        /*
        * check if file exists and is not a directory, else exit
        */ 
        std::filesystem::path full_path = std::filesystem::absolute(file);

        if (std::filesystem::exists(full_path)) {
            if (std::filesystem::is_regular_file(full_path) || std::filesystem::is_directory(full_path)) {
                return true;
            } else {
                std::cerr << "Error: " << full_path.c_str() << " is not a file or directory" << std::endl;
            }

        } else {
            std::cerr << "Error: " << full_path.c_str() << " does not exist" << std::endl;
        }
        return false;
    }

    bool verify_owner(uid_t uid, std::string file){
        uid_t owner = Misc::get_owner(file);

        #ifdef DEBUG_MODE
            std::cerr << "uid: " << uid << std::endl;
            std::cerr << "owner: " << owner << std::endl;
        #endif

        if (owner == uid){
            return true;
        } else {
            std::cerr << "Error: " << "You do not have the permissions to edit ACL for " << file << std::endl;
        }
        return false;
    }

    bool verify_acl(ACL acl, std::string user){
        if (acl.check(getpwnam(user.c_str())->pw_uid)){
            return true;
        } else {
            std::cerr << "Error: " << user << " does not have the permissions to edit ACL for this file/drectory" << std::endl;
        }
        return false;
    }

    bool verify_acl(ACL acl, uid_t user){
        #ifdef DEBUG_MODE
            std::cout << "verify_acl: " << std::endl;
            std::cout << acl << std::endl;
            std::cout << user << std::endl;
            std::cout << Misc::name_from_uid(user) << std::endl;
            std::cout << Misc::name_from_uid(acl.top()) << std::endl;
        #endif

        if (acl.check(user)){
            return true;
        } else {
            std::cerr << "Error: " << "You does not have the permissions to edit ACL for this file/directory" << std::endl;
        }
        return false;
    }
}