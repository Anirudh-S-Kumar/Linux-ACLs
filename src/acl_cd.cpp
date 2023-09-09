#include "acl_cd.h"

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " <path>" << std::endl;
        return 1;
    }

    std::string path = argv[1];

    // get the ACL, and check if current user has read access
    ACL acl;

    return 0;
}
