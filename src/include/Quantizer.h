#ifndef WAVQUANT_H
#define WAVQUANT_H

#include <cerrno>
#include <vector>
#include <fstream>
#include <iostream>
#include <sndfile.hh>

#include "bstream.h"



class Quantizer {
    public:
        Quantizer(SndfileHandle soundFileHandler, int factor);
        int encode(); 
        int decode();
        int saveBinary();
        int loadBinary();
        int saveWAVFile(std::string filename);

    private:
        int codingFactor;
        int format;
        int channels;
        int samplerate;
        int frames;
        int numberOfFrames = 0;
        std::vector<short> samples;
        std::vector<short> encodedSamples;
        std::vector<short> decodedSamples;
};

#endif