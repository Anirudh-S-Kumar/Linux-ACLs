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

class ACL {
private:
    std::set<int> acl;

public:
    ACL();
    ACL(std::string serialized_acl);
    ACL(int user);
    ~ACL();
    void add(int user);
    void remove(int user);
    bool check(int user);
    std::string serialize();
    friend std::ostream& operator<<(std::ostream& os, const ACL& acl);

};

#endif