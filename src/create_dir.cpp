#include "create_dir.h"

#define BUFFER_SIZE 1024


bool create_dir_and_save_acl(std::string dir_name)
{
    // Create directory
    if (mkdir(dir_name.c_str(), 0755) == -1)
    {
        std::cerr << "Error: " << strerror(errno) << std::endl;
        return false;
    }

    // Set ACL
    ACL acl;
    acl.set_owner(getuid());
    acl.add(getuid(), 7);
    if (not acl.save(dir_name)) return false;
    std::cout << "Directory created successfully" << std::endl;
    return true;
}



int main(int argc, char const *argv[])
{
    std::string dir_name;
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " <dir_name>" << std::endl;
        return 1;
    }

    dir_name = argv[1];

    // get acl of current directory
    ACL current_acl;
    if (not current_acl.load(getcwd(NULL, 0))){
        // if there is no acl, create one if the user is the owner
        if (not Validation::verify_owner(getuid(), getcwd(NULL, 0))) return 1;
        
        // create acl for current directory
        current_acl.set_owner(getuid());
        current_acl.add(getuid(), 7);
        if (not current_acl.save(getcwd(NULL, 0))) return 1;
    }

    else{
        // if there is acl, checck if user has permission to create directory
        if (not Validation::verify_acl(current_acl, getuid(), "w")) return 1;
        
        // change to the owner of the directory
        if (seteuid(current_acl.get_owner()) < 0){
            std::cerr << "Error: " << strerror(errno) << std::endl;
            return 1;
        }
    }

        // creating directory and saving acl
    if (not create_dir_and_save_acl(dir_name)){
        std::cerr << "Error: " << "Could not create directory" << std::endl;
        return 1;
    }

    if (seteuid(getuid()) < 0){
        std::cerr << "Error: " << strerror(errno) << std::endl;
        return 1;
    }

    return 0;
}
