#include "bios.h"
#include "neogeocd.h"
#include "libretro_log.h"

#include <cstdio>

NeoGeoCD* g_neocd = nullptr;

bool g_log_message_debug = 0;

void Libretro::Log::message(retro_log_level level, const char* format, ...)
{
    if (level == RETRO_LOG_DEBUG && !g_log_message_debug) {
        return;
    }

	auto destpipe = stdout;

    if (level >= RETRO_LOG_WARN) {
		destpipe = stderr;
	}

    va_list args;
    va_start(args, format);
    vfprintf(destpipe, format, args);
    va_end(args);
}


extern "C"
void bizhawk_neocd_unload_game()
{
	//g_neocd->memory.backupRam, Memory::BACKUPRAM_SIZE
    //Libretro::BackupRam::save();
}

extern "C"
void bizhawk_neocd_reset()
{
	fprintf(stderr, "NEOCD RESETTING!!!\n");
    if (!g_neocd) {
        g_neocd = new NeoGeoCD;
	    g_neocd->initialize();
	}
	else {
	    g_neocd->reset();
	}
}

extern "C"
void bizhawk_neocd_run()
{
    if (!g_neocd) {
        g_neocd = new NeoGeoCD;
	    g_neocd->initialize();
	}

	// Libretro has a speedhack that accelerates emulation during CD loading, by running frames as long
	// CD data reads are happening. Probably works well for neogeo games which likely never use in-game
	// data reads from CD since most of them rely on streaming music during gameplay.
	//
	// However the feature is probably not useful to 

    // Run one frame
    g_neocd->cdSectorDecodedThisFrame = false;
    g_neocd->runOneFrame();

    // Send audio and video to the frontend
    //libretro.audioBatch(reinterpret_cast<const int16_t*>(&neocd->audio.buffer.ymSamples[0]), neocd->audio.buffer.sampleCount);
    //libretro.video(neocd->video.frameBuffer, Video::FRAMEBUFFER_WIDTH, Video::FRAMEBUFFER_HEIGHT, Video::FRAMEBUFFER_WIDTH * sizeof(uint16_t));
}

extern "C"
void bizhawk_neocd_get_audio_chunk()
{
	//g_neocd->audio.buffer.ymSamples[0]
	//g_neocd->audio.buffer.sampleCount
}

extern "C"
void bizhawk_neocd_get_video_frame()
{
	//g_neocd->video.frameBuffer, Video::FRAMEBUFFER_WIDTH, Video::FRAMEBUFFER_HEIGHT, Video::FRAMEBUFFER_WIDTH * sizeof(uint16_t)
}
