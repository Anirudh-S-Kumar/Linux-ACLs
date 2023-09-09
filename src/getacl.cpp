#include "getacl.h"
#include <validation.h>
#include <misc.h>

#define BUFFER_SIZE 256
#define DEBUG_MODE

int main(int sysc, char** sysv){
    std::string file;
    if (sysc < 2){
        std::cout << "Usage: " << "getacl" << " <file>" << std::endl;
        return 1;
    }

    file = sysv[1];
    if (!Validation::validate_file(file)) return 1;

    ACL acl;
    bool res = acl.load(file);
    if (not res){
        std::cout << "No ACL found for " << file << std::endl;
        return 1;
    }
    // check if person is allowed to see the ACL
    if (!Validation::verify_acl(acl, getuid())) return 1;
    std::cout << acl << std::endl;    
}