#include "acl_cd.h"

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " <path>" << std::endl;
        return 1;
    }

    std::string path = argv[1];
    if (not Validation::validate_file(path)) return 1;

    // get the ACL, and check if current user has read access
    ACL acl;

    // if no acl exists, create one, and change to that directory
    if (not acl.load(path)){
        if (not Validation::verify_owner(getuid(), path)) return 1;
        acl.set_owner(getuid());
        acl.add(getuid(), 7);
        
        if (not acl.save(path)) return 1;
        
        if (chdir(path.c_str()) < 0) {
            // print the error number and error message
            std::cerr << chdir(path.c_str()) << ": " << strerror(errno) << std::endl;

            std::cerr << "Error: " << "Could not change directory to " << path << std::endl;
            return 1;
        }

        // execute a shell command to change the current working directory of the parent process
        std::string command = "cd " + Misc::full_path(path);
        system(command.c_str());
        }

    else{
        if (not Validation::verify_acl(acl, getuid(), "rwx")) return 1;
        if (chdir(path.c_str()) < 0){
            std::cerr << "Error: " << "Could not change directory to " << path << std::endl;
            return 1;
        }
    }

    return 0;
}
