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
        Quantizer(std::vector<short> samples, int factor, int flag);
        int encode();
        int decode();
        int saveBinary();
        int loadBinary();
        int saveWAVFile(std::string filename);
        int getEncodedResidualSamples();
        int getDecodedResidualSamples();

    private:
        int codingFactor;
        int format;
        int channels;
        int samplerate;
        int frames;
        int flag;
        int numberOfFrames = 0;
        std::vector<short> samples;
        std::vector<short> encodedSamples;
        std::vector<short> decodedSamples;
        std::vector<short> residualsamples;

};

#endif