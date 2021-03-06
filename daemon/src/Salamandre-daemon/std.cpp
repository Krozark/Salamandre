#include <Salamandre-daemon/std.hpp>

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>

namespace std
{
    std::vector<std::string> split(const std::string& string, const std::string& del)
    {
        size_t first = 0,
               second = 0,
               end = string.size(),
               len = del.size();
        std::vector<std::string> tokens;

        while ((second = string.find(del,first)) != (std::string::npos))
        {
            size_t dif = second - first;
            if (dif)
                tokens.push_back(string.substr(first,dif));
            first = second + len;
        }
        if (first != end)
            tokens.push_back(string.substr(first));
        return tokens;
    }

    int createDir(const std::string& dirpath,const int permissions)
    {
        int res = 1; //0 => error, 1 => created, 2 => already exist
        auto sp = split(dirpath,"/");


        std::string current;
        const unsigned int _size = sp.size();
        for(unsigned int i=0; i<_size and res != 0;++i)
        {

            current += sp[i] + "/";

            #if __WIN32
            res = ::mkdir(current.c_str());
            #else
            res = ::mkdir(current.c_str(), permissions);
            #endif

            if(res == 0)
                res = 1;
            else if(errno == EEXIST)
                res = 2;
            else
                res = 0;
        }
        return res;
    }

    std::vector<std::string> getFileList(const std::string& dirpath)
    {
        DIR *curDir;
        std::vector<std::string> res;

        if ((curDir = opendir(dirpath.c_str())) == NULL)
            return res;

        dirent *curEntry =readdir(curDir);
        while (curEntry != NULL)
        {
            if(curEntry->d_type == DT_REG)
                res.push_back(curEntry->d_name);
            curEntry =readdir(curDir);
        }

        return res;
    }

    std::vector<std::string> getDirList(const std::string& dirpath)
    {
        DIR *curDir;
        std::vector<std::string> res;

        if ((curDir = opendir(dirpath.c_str())) == NULL)
            return res;

        dirent *curEntry =readdir(curDir);
        while (curEntry != NULL)
        {
            if(curEntry->d_type == DT_DIR
               and std::string(curEntry->d_name) != ".."
               and std::string(curEntry->d_name) != ".")
                    res.push_back(curEntry->d_name);
            curEntry =readdir(curDir);
        }

        return res;
    }
}
