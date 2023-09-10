#include "fget.h"


int main(int argc, char const *argv[])
{
    std::string file;
    if (argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " <path>" << std::endl;
        return 1;
    }

    file = argv[1];
    if (not Validation::validate_file(file)) return 1;

    // change to owner of the file
    if (seteuid(Misc::get_owner(file)) < 0){
            std::cerr << "Error: " << "Could not change to owner of " << file << std::endl;
            return 1;
    }

    // check if the user has read access
    ACL acl;
    if (not acl.load(file)){
        return 1; // ACLs must exist for reading files        
    }

    if (not Validation::verify_acl(acl, getuid(), "r")) return 1;

    // read from the file
    std::ifstream in_file(file);
    std::string line;
    while (std::getline(in_file, line)){
        std::cout << line << std::endl;
    }
    
    in_file.close();

    // change back to the user
    if (seteuid(getuid()) < 0){
        std::cerr << "Error: " << "Could not change back to user" << std::endl;
        return 1;
    }

    return 0;
}
