#include "misc.h"
using namespace Misc;

#define BUFFER_SIZE 1024

namespace Misc{

    std::string full_path(std::string file){
        std::filesystem::path full_path = std::filesystem::absolute(file);
        return std::string(full_path.c_str());
    }   

    std::string name_from_uid(uid_t uid){
        struct passwd *pw = getpwuid(uid);
        if (pw){
            return std::string(pw->pw_name);
        }
        return std::string();
    }

    uid_t uid_from_name(std::string name){
        struct passwd *pw = getpwnam(name.c_str());
        if (pw){
            return pw->pw_uid;
        }
        return -1;
    }

    uid_t get_owner(std::string file){
        std::filesystem::path full_path = std::filesystem::absolute(file);

        struct stat sb;
        stat(full_path.c_str(), &sb);
        return sb.st_uid;
    }

    int gen_perm(std::string perms){
        int rval = 0;
        for(auto& i : perms){
            if (i == 'r') rval |= 4;
            else if (i == 'w') rval |= 2;
            else if (i == 'x') rval |= 1;
        }

        return rval;
    }

    std::string print_perm(int perm){
        std::string rval = "";
        if (perm & 4) rval += "r";
        else rval += "-";

        if (perm & 2) rval += "w";
        else rval += "-";
        
        if (perm & 1) rval += "x";
        else rval += "-";
        
        return rval;
    }
}