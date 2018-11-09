#ifndef AUDIOHANDLER_H
#define AUDIOHANDLER_H

#include <iostream>
#include <vector>
#include <sndfile.hh>

using namespace std;

constexpr size_t FRAMES_BUFFER_SIZE = 65536;

class AudioHandler {
    public:
        AudioHandler();
        AudioHandler(const char * filename);
        AudioHandler(const char * filename, int blockLength);
        int loadAll();
        int loadBlock();
        int getFormat();
        int getChannels();
        int getSamplerate();
        vector<char> getSamples_8();
        vector<short> getSamples_16();
        vector<int> getSamples_32();
        int save(std::string filename, vector<short>& samples, int numberOfFrames);
        int save(std::string filename, vector<short>& samples, int numberOfFrames, int format, int channels, int samplerate);
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