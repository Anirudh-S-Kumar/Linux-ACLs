#ifndef ACL_H
#define ACL_H


#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/set.hpp>
#include <sstream>
#include <pwd.h>
#include "misc.h"

class ACL {
private:
    uid_t owner;
    std::set<int> acl;
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int) {
        ar & owner;
        ar & acl;
    }

public:
    ACL();
    ACL(std::string serialized_acl);
    ACL(int user);
    ~ACL();
    void set_owner(uid_t user);
    uid_t get_owner();
    void add(int user);
    void remove(int user);
    bool check(int user);
    int top();
    std::string serialize();
    friend std::ostream& operator<<(std::ostream& os, const ACL& acl);
    bool load(std::string file);
    bool save(std::string file);

};

#endif