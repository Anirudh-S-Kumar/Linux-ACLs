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
        if (uid == 0) return true;
        uid_t owner = Misc::get_owner(file);
        if (owner == uid){
            return true;
        } else {
            std::cerr << "Error: " << "You do not have the permissions for " << file << std::endl;
        }
        return false;
    }

    bool verify_acl(ACL acl, uid_t user, std::string perm){
        if (user == 0) return true;
        if (acl.check(user, Misc::gen_perm(perm))){
            return true;
        } else {
            std::cerr << "Error: " << "You do not have the permissions for this file/directory" << std::endl;
        }
        return false;
    }
}