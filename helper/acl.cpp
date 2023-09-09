#include "acl.h"
#define BUFFER_SIZE 1024

ACL::ACL() {}
ACL::~ACL() {}

ACL::ACL(std::string serialized_acl) {
    std::istringstream iss(serialized_acl);
    boost::archive::binary_iarchive ia(iss);
    ia >> *this;
}

ACL::ACL(int user) {
    acl.insert(user);
}

void ACL::add(int user) {
    acl.insert(user);
}

void ACL::remove(int user) {
    acl.erase(user);
}

bool ACL::check(int user) {
    return acl.find(user) != acl.end();
}

void ACL::set_owner(uid_t user) {
    owner = user;
}

uid_t ACL::get_owner() {
    return owner;
}

int ACL::top() {
    return *acl.begin();
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
    os << "ACL: { ";
    // convert the uid to username and then print
    for (int user : acl.acl) {
        os << Misc::name_from_uid(user) << " ";
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
