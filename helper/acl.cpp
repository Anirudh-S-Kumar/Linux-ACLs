#include "acl.h"

ACL::ACL() {}
ACL::~ACL() {}

ACL::ACL(std::string serialized_acl) {
    std::istringstream iss(serialized_acl);
    boost::archive::binary_iarchive ia(iss);
    ia >> acl;
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

std::string ACL::serialize() {
    std::ostringstream oss;
    boost::archive::binary_oarchive oa(oss);
    oa << acl;
    return oss.str();
}

std::ostream& operator<<(std::ostream& os, const ACL& acl) {
    os << "ACL: { ";
    // convert the uid to username and then print
    for (int user : acl.acl) {
        struct passwd *pw = getpwuid(user);
        if (pw != NULL) {
            os << pw->pw_name << " ";
        } else {
            os << user << " ";
        }
    }
    os << "}";
    return os;
}
