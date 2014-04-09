#ifndef SALAMANDRE_GUIFUNCTIONS_HPP
#define SALAMANDRE_GUIFUNCTIONS_HPP

#include <Socket/SocketSerialized.hpp>

namespace salamandre
{
/**
 * \brief namespace for the  local calls (with the gui)
 */
namespace gui
{
    /**
     * \brief store all the callback ids for the gui
     */
    enum func {
        newFile = 1, ///< to call whene the gui add files to save
        sync ///< to call whene un sync is need
    };

    /***
     * \brief The distpatch function
     */
    int dispatch(int id,ntw::SocketSerialized& request);
}
}
#endif
