#include <Salamandre-daemon/FileInfo.hpp>

#include <Socket/Serializer.hpp>

namespace salamandre
{

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
