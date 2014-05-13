#ifndef SALAMANDRE_FILEINFO_HPP
#define SALAMANDRE_FILEINFO_HPP

#include <string>

namespace ntw {
    class Serializer;
}

namespace salamandre
{
    struct FileInfoFrom
    {
        //file
        long int version;
        int id_medecin;
        int id_patient;
        std::string filename;
        //src
        int port;
        std::string ip;
    };

    class FileInfoFromPath
    {
        public:
        //file
        int id_medecin;
        int id_patient;
        std::string filename;
        std::string path;
        //src
        int port;
        std::string ip;

        bool operator==(const FileInfoFromPath& other)const;
    };

    class FileHasher{
        public:
            std::size_t operator()(const FileInfoFromPath &v) const;
    };

    struct FileInfo {
        long int version;
        int id_medecin;
        int id_patient;
        std::string filename;

        friend ntw::Serializer& operator<<(ntw::Serializer& ser,const FileInfo& self);
        friend ntw::Serializer& operator>>(ntw::Serializer& ser,FileInfo& self);
    };
}

#endif
