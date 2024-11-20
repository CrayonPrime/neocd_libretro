#pragma once

enum retro_log_level
{
   RETRO_LOG_DEBUG = 0,
   RETRO_LOG_INFO,
   RETRO_LOG_WARN,
   RETRO_LOG_ERROR,

   RETRO_LOG_DUMMY = INT_MAX
};

namespace Libretro
{
    namespace Log
    {
        void init();
        void message(retro_log_level level, const char* format, ...);

    } // namespace Log
} // namespace Libretro

extern bool g_log_message_debug;
