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
        static std::vector<short> quantize(std::vector<short> samples, int factor){
                residualSamples.clear();
            	std::cout << samples.size() << std::endl;
                for (auto sample : samples) {										// for each 16-bit sample in samples,
                    residualSamples.push_back(sample >> factor);		//     push back a truncated version on codingFactor-bits to encodedSample;
                }																				//
                return residualSamples;	
        }
        Quantizer(SndfileHandle soundFileHandler, int factor);
        Quantizer(std::vector<short> samples, int factor, int flag);
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
        int flag;
        int numberOfFrames = 0;
        std::vector<short> samples;
        std::vector<short> encodedSamples;
        std::vector<short> decodedSamples;
        static std::vector<short> residualSamples;
       
};

#endif