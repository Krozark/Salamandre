#ifndef STD_STD_HPP
#define STD_STD_HPP

#include <string>
#include <vector>

namespace std
{
    /**
     * \brief split a string in multiple sub string
     * \param string base to split
     * \param del the delemiteur
     * \return a vector that contain the sub string
     */
    std::vector<std::string> split(const std::string& string, const std::string& del);

    /**
     * \brief like the python join
     */
    template<typename T>
    std::string join(const std::string& del,const std::vector<T>& datas);

    /**
     * \brief create a directory.
     * \param dirpath the dir path name
     * \param permissions  (ignored in windows)
     * \return 1 if created, 2 if it was allready exist, 0 if errored
     */
    int createDir(const std::string& dirpath,const int permissions = 0777);

    /**
     * \brief get file list of a directory
     * \param dirpath the root dir
     * \return the list
     */
    std::vector<std::string> getFileList(const std::string& dirpath);

    /**
     * \brief get the dir list in a folder
     * \param dirpath the root dir
     * \return the dir list (without . and ..)
     */
    std::vector<std::string> getDirList(const std::string& dirpath);
}
#include <Salamandre-daemon/std.tpl>
#endif
