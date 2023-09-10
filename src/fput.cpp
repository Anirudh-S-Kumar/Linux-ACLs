#include "fput.h"

int main(int sys_argc, char const *sys_argv[]){
    std::string file, text;
    if (sys_argc < 3){
        std::cout << "Usage: " << sys_argv[0] << " <path>" << " \"<text>\"" <<  std::endl;
        return 1;
    }

    file = sys_argv[1];
    text = sys_argv[2];

    if (not Validation::validate_file(file)) return 1;

    // change to owner of the file
    if (seteuid(Misc::get_owner(file)) < 0){
            std::cerr << "Error: " << "Could not change to owner of " << file << std::endl;
            return 1;
    }

    // check if the user has write access
    ACL acl;
    if (not acl.load(file)){
        if (not Validation::verify_owner(getuid(), file)) return 1;
        acl.set_owner(getuid());
        acl.add(getuid(), 7);
        //change to owner to save the file

        
        if (not acl.save(file)) return 1;
    }

    else{
        if (not Validation::verify_acl(acl, getuid(), "w")) return 1;
    }

    // write to the file


    std::ofstream out_file(file, std::ios::trunc);
    out_file << text << std::endl;

    if (out_file.fail() || out_file.bad()) {
        std::cerr << "Error: " << "Could not write to file " << file << std::endl;
        return 1;
    }

    out_file.close();

    // change back to the user
    if (seteuid(getuid()) < 0){
        std::cerr << "Error: " << "Could not change back to user" << std::endl;
        return 1;
    }

    std::cout << "Successfully wrote to " << file << std::endl;

    return 0;
}