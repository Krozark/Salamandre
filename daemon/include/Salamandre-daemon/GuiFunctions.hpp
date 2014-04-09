#ifndef SALAMANDRE_GUIFUNCTIONS_HPP
#define SALAMANDRE_GUIFUNCTIONS_HPP

#include <Socket/SocketSerialized.hpp>

namespace salamandre
{
namespace gui
{
    int dispatch(int id,ntw::SocketSerialized& request);
}
}
#endif
