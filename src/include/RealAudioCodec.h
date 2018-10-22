#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <map>
#include <vector>
#include <string>

using namespace std;

class RealAudioCodec {
    private:
        int buffer = 0;
        int bits_in_buffer = 0;
    public:
        void encodeGolombToFile(vector<int>& input, vector<int>& predictors, vector<int>& m_vector, ofstream& file, int bs, int factor);
        vector<int> decodeGolombFromFile(ifstream& file, long total_samples, int bs, int factor);
        int decodeSingleResidual(int value, int predictor, int *last_values, bool debug);
        int decodeToWavFile(ifstream& file, vector<short>& outLEFT, vector<short>& outDELTA);
        vector<short> lossyRecover(vector<short>& input);
        vector<int> lossyRecover(vector<int>& input);

        /* read and write functions */
        void readHeader(ifstream& file, long *samples, unsigned int *bs, unsigned int *channels, unsigned int *factor, unsigned int *lf);
        void writeHeader(ofstream& file, long samples, int bs, int channels, int fact, int lf);
        void writeWavToFile(string filename, vector<short>& left, vector<int>& delta, int channels);
        void writeToFile(int data, int length, char type, ofstream& file);

        void replenish(ifstream& file);
        char readNextByte(ifstream& file);
        int readNextPredictor(ifstream& file);
        int readNextQ(ifstream& file);
        int readNextR(ifstream& file, int number_of_bits);

        int buildOnes(int n);
        void flush(ofstream& file);
        void forceFlush(ofstream& file);
};