#ifndef AUDIOHANDLER_H
#define AUDIOHANDLER_H

#include <iostream>
#include <vector>
#include <sndfile.hh>

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
        int getChannels();
        int getFormat();
        int getSamplerate();
        int save(std::string filename, vector<short>& samples, int numberOfFrames);
        static int save(std::string filename, vector<short>& samples, int numberOfFrames, int format, int samplerate, int channels);
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