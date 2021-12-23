#pragma once

#include <fstream>
#include <ogg/ogg.h>
#include <vorbis/vorbisfile.h>
#include "alHelpers.h"
#include "abstractSound.h"

/*
 * This code is largely copied verbatim from:
 * https://indiegamedev.net/2020/01/16/how-to-stream-ogg-files-with-openal-in-c/
 * It has been modified to fit in the context of a sound manager.
 * It has also been modified to be able to optionally disable looping,
 * and to be able to play the same sound more than once.
 * This code was in the public domain and was not under any licenses or restrictions.
 */

namespace chira {
    constexpr std::size_t OGG_NUM_BUFFERS = 4;
    constexpr ALsizei OGG_BUFFER_SIZE = 65536;

    struct oggStreamData {
        ALuint buffers[OGG_NUM_BUFFERS];
        std::string filename;
        std::ifstream file;
        std::uint8_t channels;
        std::int32_t sampleRate;
        std::uint8_t bitsPerSample;
        ALsizei size;
        ALuint source;
        ALsizei sizeConsumed = 0;
        ALenum format;
        OggVorbis_File oggVorbisFile;
        std::int_fast32_t oggCurrentSection = 0;
        std::size_t duration;
    };

    class oggFileSound : public abstractSound {
    public:
        bool init(const std::string& filename);
        bool init(const std::string& filename, float pitch_, float gain_, const glm::vec3& position_, soundType type_, bool loop_, bool is3d_) override;
        void play() override;
        void update() override;
        void stop() override;
        void discard() override;
    private:
        bool readFile(const std::string& filename);
        void seekBeginning();
        static std::size_t readOggVorbisCallback(void* destination, std::size_t size1, std::size_t size2, void* fileHandle);
        static std::int32_t seekOggVorbisCallback(void* fileHandle, ogg_int64_t to, std::int32_t type);
        static long int tellOggVorbisCallback(void* fileHandle);
        oggStreamData audioData{};
        bool hasBeenPlayedPreviously = false; // fixes bug where sound cannot be played twice
    };
}
