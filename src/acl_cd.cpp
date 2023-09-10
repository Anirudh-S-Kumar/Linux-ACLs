#include "acl_cd.h"

bool print_ls(const std::string& path){
    std::string command = "ls -lha " + path;
    FILE* pipe = popen(command.c_str(), "r");
    if (not pipe) {
        std::cerr << "Error: " << "Could not execute command" << std::endl;
        return false;
    }
    char buffer[128];
    std::string result = "";
    while (fgets(buffer, 128, pipe) != NULL) {
        result += buffer;
    }
    pclose(pipe);
    std::cout << result << std::endl;
    return true;
}


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
        }
    // if acl exists, check if user has execute permission
    else{
        if (not Validation::verify_acl(acl, getuid(), "x")) return 1;

        if (seteuid(acl.get_owner()) < 0){
            std::cerr << "Error: " << strerror(errno) << std::endl;
            return 1;
        }
    }

    std::cout << "CD successful. Showing the contents of the directory :- " << std::endl;
    std::cout << "---------------------------------------------------------" << std::endl;
    if (not print_ls(path)) return 1;

    return 0;
}
