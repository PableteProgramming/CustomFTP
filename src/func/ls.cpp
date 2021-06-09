#include <ls.hpp>
#include <func.hpp>

std::string ls(std::vector<std::string> params) {
    //we have to check the params

    std::string directory;

    if (params.empty())
    {
        directory = ".";
    }
    else if (params.size() > 1)
    {
        return "Too many parameters passed!";
    }
    else
    {
        directory = params[0];
    }

    struct dirent* d;
    DIR* dr;
    dr = opendir(directory.c_str());
    std::string r;
    if (dr != NULL)
    {
        //r = "\r\r";
        for (d = readdir(dr); d != NULL; d = readdir(dr))
        {
            if (std::string(d->d_name) != "." && std::string(d->d_name) != "..")
            {
                r += std::string(d->d_name);
                r += '\n';
            }
            //std::cout << d->d_name << std::endl;
        }
        closedir(dr);
    }
    else {
        return "\r" + directory + " is not a valid directory!";
    }
    return "\rThe content of " + directory + " is :\n" + EndStrip(r);
    //return EndStrip(r);
}