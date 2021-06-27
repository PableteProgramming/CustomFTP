#include <ls.hpp>
#include <func.hpp>

std::string ls(std::string p) {
    //we have to check the params
    if (p != "") {
        std::vector<std::string> params = SplitV(p, ' ');
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
        return "\r\033[31mThe content of " + directory + " is :\033[0m\n" + EndStrip(r);
    }
    return "\rPlease specify the directory to list";
}