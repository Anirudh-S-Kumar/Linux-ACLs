#include "simple_sudo.h"


int main(int argc, char const *argv[])
{
    std::string command;
    std::string executable;
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <command> [args...]" << std::endl;
        return 1;
    }

    executable = argv[1];

    for (int i = 1; i < argc; i++) {
        command += argv[i];
        command += " ";
    }

    // getting the executable path
    std::string which_command = "which " + executable;
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(which_command.c_str(), "r"), pclose);
    if (!pipe) {
        std::cerr << "which command failed" << std::endl;
        return 1;
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    result.erase(std::remove(result.begin(), result.end(), '\n'), result.end());
    
    // if binary doesn't exist, exit
    if (result.empty()) {
        std::cerr << "Error: " << executable << " not found" << std::endl;
        return 1;
    }

    
    executable = result;
    //getting the ACL and checking if the user has execute permission
    ACL acl;
    if (not acl.load(executable)) {
        return 1;
    }
    
    // changing the user id to the owner of the executable
    uid_t current_user = getuid();
    if (setuid(Misc::get_owner(executable)) != 0) {
        std::cerr << "seteuid failed: " << std::strerror(errno) << std::endl;
        return 1;
    }

    // executing the command
    std::system(command.c_str());

    // changing the user id back to the original user
    if (seteuid(current_user) != 0) {
        std::cerr << "seteuid failed: " << std::strerror(errno) << std::endl;
        return 1;
    }

    return 0;
}
