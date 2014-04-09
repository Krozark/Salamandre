#ifndef SALAMANDRE_SERVERFUNCTIONS_HPP
#define SALAMANDRE_SERVERFUNCTIONS_HPP

#include <Socket/SocketSerialized.hpp>

namespace salamandre
{
namespace srv
{
    int dispatch(int id,ntw::SocketSerialized& request);
}
}
#endif
