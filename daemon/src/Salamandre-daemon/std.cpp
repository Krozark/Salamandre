#include <Salamandre-daemon/std.hpp>

#include <sys/types.h>
#include <sys/stat.h>
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

    int createDir(const std::string& dirpath)
    {
        int res = 1; //0 => error, 1 => created, 2 => already exist
        auto sp = split(dirpath,"/");

        std::string current;
        const unsigned int _size = sp.size();
        for(unsigned int i=0; i<_size and res != 0;++i)
        {
            current += sp[i] + "/";
            res = mkdir(current.c_str(), 0777);
            if(res == 0)
                res = 1;
            else if(errno == EEXIST)
                res = 2;
            else
                res = 0;
        }
        return res;
    }
}
