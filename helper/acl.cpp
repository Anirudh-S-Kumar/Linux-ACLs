#include "acl.h"
#define BUFFER_SIZE 1024

ACL::ACL() {}
ACL::~ACL() {}

ACL::ACL(std::string serialized_acl) {
    std::istringstream iss(serialized_acl);
    boost::archive::binary_iarchive ia(iss);
    ia >> *this;
}

ACL::ACL(int user, int perms) {
    acl[user] = perms;
}

void ACL::add(int user, int perms) {
    acl[user] = perms;
}

void ACL::remove(int user) {
    acl.erase(user);
}

bool ACL::check(int user, int perms) {
    return ((acl.find(user) != acl.end()) && ((acl[user] & perms) == perms));
}

void ACL::set_owner(uid_t user) {
    owner = user;
}

uid_t ACL::get_owner() {
    return owner;
}

std::string ACL::serialize() {
    /*
    Serializes the ACL object into a string
    */
    std::ostringstream oss;
    boost::archive::binary_oarchive oa(oss);
    oa << *this;
    return oss.str();
}

std::ostream& operator<<(std::ostream& os, const ACL& acl) {
    os << "Owner: " << Misc::name_from_uid(acl.owner) << std::endl;
    os << "ACL: { \n";
    // convert the uid to username and then print
    for (auto& user : acl.acl) {
        os << '\t' << Misc::name_from_uid(user.first) << ":\t" << Misc::print_perm(user.second) << "\n";
    }
    os << "}";
    return os;
}

bool ACL::load(std::string file){
    /*
    Loads the ACL from the xattr of the file
    */
    std::filesystem::path full_path = std::filesystem::absolute(file);

    std::string acl_str;
    acl_str.resize(BUFFER_SIZE);
    ssize_t acl_attr = getxattr(file.c_str(), "user.acl",  &acl_str[0], acl_str.size());
    if (acl_attr == -1){
        std::cerr << "No ACL found for " << file << std::endl;
        return false;
    }

    *this = ACL(acl_str);
    return true;
}

bool ACL::save(std::string file){
    /*
    Stores the serialized ACL in the xattr of the file
    */
    std::filesystem::path full_path = std::filesystem::absolute(file);

    std::string acl_str = serialize();
    if (setxattr(file.c_str(), "user.acl", acl_str.c_str(), acl_str.size(), 0) == -1){
        std::cerr << "Error: " << "Could not set ACL for " << file << std::endl;
        return false;
    }
    return true;
}
