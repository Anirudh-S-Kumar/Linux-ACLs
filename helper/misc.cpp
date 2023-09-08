#include "misc.h"
using namespace Misc;

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
