#pragma once

#include "libretro.h"

namespace Libretro
{
    namespace Log
    {
        void init();
        void message(retro_log_level level, const char* format, ...);

    } // namespace Log
} // namespace Libretro

extern bool g_log_message_debug;
