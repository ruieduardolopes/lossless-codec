#ifndef AUDIOHANDLER_H
#define AUDIOHANDLER_H

#include <iostream>
#include <vector>
#include <sndfile.hh>

#include "BadFormatException.h"

using namespace std;

constexpr size_t FRAMES_BUFFER_SIZE = 65536;

class AudioHandler {
    public:
        AudioHandler(const char * filename);
        AudioHandler(const char * filename, int blockLength);
        int loadAll();
        int loadBlock();
        vector<char> getSamples_8();
        vector<short> getSamples_16();
        vector<int> getSamples_32();
    private:
        int loadAudioSpecs();
        int blockLength;
        int channels;
        int format;
        int samplerate;
        SndfileHandle audio;
        vector<char> samples_8;
        vector<short> samples_16;
        vector<int> samples_32;
};

#endif