#include <Salamandre-daemon/FileInfo.hpp>

#include <Socket/Serializer.hpp>
#include <functional>

namespace salamandre
{

    bool FileInfoFromPath::operator==(const FileInfoFromPath& other)const
    {
        return path == other.path;
    }

    size_t FileHasher::operator()(const FileInfoFromPath &v) const
    {
        return std::hash<std::string>()(v.path);
    }

    ntw::Serializer& operator<<(ntw::Serializer& ser,const FileInfo& self)
    {
        ser<<self.version<<self.id_medecin<<self.id_patient<<self.filename;
        return ser;
    }

    ntw::Serializer& operator>>(ntw::Serializer& ser,FileInfo& self)
    {
        ser>>self.version>>self.id_medecin>>self.id_patient>>self.filename;
        return ser;
    }
}
