#include <ls.hpp>

bool ls(std::vector<std::string> params) {
    //we have to check the params
    struct dirent* d;
    DIR* dr;
    dr = opendir(".");
    if (dr != NULL)
    {
        for (d = readdir(dr); d != NULL; d = readdir(dr))
        {
            std::cout << d->d_name << std::endl;
        }
        closedir(dr);
    }
    else {
        std::cout << "\nAn Error Occurred!";
    }
    return true;
}