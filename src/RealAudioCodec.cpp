#include "RealAudioCodec.h";

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <limits.h>
#include <math.h>
#include <sndfile.hh>

void RealAudioCodec::encodeGolombToFile(vector<int>& input, vector<int>& predictors, vector<int>& m_vector, ofstream& file, int bs, int factor){
    vector<int>::iterator it;
    int target;
    int q, r;
    int nBits; // nBits = (int) log2(m);
    int m;
    int count = 100;

    int i = 0, blockCounter = 0, partition_idx = 0;
    for(it = input.begin(); it != input.end(); it++, i++){
        if(*it < 0){
            target = -(1 + *it  * 2);
        }
        else{
            target = 2* *it;
        }

        /* check bs */
        if ((i % bs) == 0){
            if((blockCounter % factor) == 0){
                m = m_vector.at(partition_idx++);
                nBits = (int) log2(m);
                writeToFile(nBits, 8, 'm', file); //write m value in the file;
            }
            blockCounter++;
            writeToFile(predictors.at(i/bs), 2, 'p', file);
        }

        /* calculate q  and r values*/
        q = target/m;
        r = target - q * m;
        writeToFile(q, q, 'q', file); //write q value in the file.
        writeToFile(r, nBits, 'r', file); //write r value in the file.
    }
    writeToFile(0, 0, 'f', file);
    cout << "Wrote a total of: " << partition_idx << " partitions.\n";
}

vector<int> RealAudioCodec::decodeGolombFromFile(ifstream& file, long total_samples, int bs, int factor){
    vector<int> decoded_samples;
    int blockCounter = 0;
    int m, nBits, predictor, q, r, original;
    int value;
    int last_values[4] = {0};
    int count = 100;

    for (long readsamples = 0; readsamples < total_samples; readsamples++){
        if ((readsamples % bs) == 0){
            if ((blockCounter % factor) == 0){
                nBits = (int) readNextByte(file);
                m = pow(2, nBits);
            }
            predictor = readNextPredictor(file);
            blockCounter++;
        }
        /* calculate q and r values */
        q = readNextQ(file);
        r = readNextR(file, nBits);

        value = m*q + r;
        if ((value % 2) == 0){
            value /= 2;
        }
        else{
            value = -((value+1)/2);
        }

        original = decodeSingleResidual(value, predictor, last_values, blockCounter == 52);
        //rearrange order of the last values;
        last_values[3] = last_values[2];
        last_values[2] = last_values[1];
        last_values[1] = last_values[0];
        last_values[0] = original;

        decoded_samples.push_back(original);

    }
    return decoded_samples;
}

int RealAudioCodec::decodeSingleResidual(int value, int predictor, int *last_values, bool debug){
    int nextValue;
    int order = predictor + 1;
    cout << "Using order: " << order << endl;
    if (order == 1) {
        nextValue = (value + last_values[0]);
    }
    else if (order == 2) {
        nextValue = (value + 2 * last_values[0] - last_values[1]);
    }
    else if (order == 3) {
        nextValue = (value + 3 * last_values[0] - 3 * last_values[1] + last_values[2]);
    }
    else if (order == 4) {
        nextValue = (value + 4 * last_values[0] - 6 * last_values[1] + 4 * last_values[2] - last_values[3]);

    }
    if (debug){
         cout << "Order was " << order << ", value: " << value << " lastvalues: [" << last_values[0] << " " << last_values[1]
              << " "<< last_values[2] << " "<< last_values[3] << "] result: " << nextValue
              << " alternatives: " << (value + last_values[0]) << " " << (value + 2 * last_values[1] - last_values[0])
              << " " << (value + 3 * last_values[2] - 3 * last_values[1] + last_values[0]) << " "
              << " " << (value + 4 * last_values[3] - 6 * last_values[2] + 4 * last_values[1] - last_values[0])
              << endl;
    }
    return nextValue;
}

int RealAudioCodec::decodeToWavFile(ifstream& file, vector<short>& outLEFT, vector<short>& outDELTA){
    buffer = 0;
    bits_in_buffer = 0;

    long total_samples = 0;
    unsigned int bs, channels, factor;
    unsigned int load_factor;
    //read file header
    readHeader(file, &total_samples, &bs, &channels, &factor, &load_factor);
    
    cout << "\nHeader:\nSamples: " << total_samples << "\nBlock size: " << bs << "\nChannels: " << channels << "\nPartition factor: " << factor << endl;
    if(load_factor != 0) {
        cout << "Reconstruction levels: " << ((SHRT_MAX - SHRT_MIN) / load_factor) << endl;
    }
    cout << "\nDecoding file...\n";
    vector<int> decoded_input = decodeGolombFromFile(file, total_samples, bs, factor);

    int value;

    if (load_factor != 0){
        double delta = (SHRT_MAX - SHRT_MIN)/((SHRT_MAX - SHRT_MIN) / load_factor);
        if (channels == 2){
            for (int i = 0; i < decoded_input.size(); i++){
                value = (int) (decoded_input.at(i) * delta);
                if (i < decoded_input.size() / 2){
                    outLEFT.push_back((short) value);
                }
                else{
                    outDELTA.push_back(value);
                } 

            }
        }
        else{
            for (int i = 0; i < decoded_input.size(); i++){
                 value = (int) (decoded_input.at(i) * delta);
                 outLEFT.push_back((short) value); 
            }
        }
    }
    else{
        if (channels == 2){
            for (int i = 0; i < decoded_input.size(); i++){
                value = (int) (decoded_input.at(i));
                if (i < decoded_input.size() / 2){
                    outLEFT.push_back((short) value);
                }
                else{
                    outDELTA.push_back(value);
                } 

            }
        }
        else{
            for (int i = 0; i < decoded_input.size(); i++){
                 value = (short) (decoded_input.at(i));
                 outLEFT.push_back((short) value); 
            }
        }
    }
}

vector<short> RealAudioCodec::lossyRecover(vector<short>& input){
    vector<short> output;
    short actual_val;
    short next_val;
    int mean;

    for(int i = 0; i < (input.size() - 1); i++){
        actual_val = input.at(i);
        next_val = input.at(i+1);

        mean = ((int) actual_val + (int) next_val)/2;
        output.push_back(actual_val);
        output.push_back((short) mean);
    }
    output.push_back(input.at(input.size()-1));
    return output;

}

vector<int> RealAudioCodec::lossyRecover(vector<int>& input){
    vector<int> output;
    short actual_val;
    short next_val;
    int mean;

    for(int i = 0; i < (input.size() - 1); i++){
        actual_val = input.at(i);
        next_val = input.at(i+1);
        mean = (actual_val + next_val)/2;
        output.push_back(actual_val);
        output.push_back(mean);
    }
    output.push_back(input.at(input.size()-1));
    return output;
}

void RealAudioCodec::readHeader(ifstream& file, long *samples, unsigned int *bs, unsigned int *channels, unsigned int *factor, unsigned int *lf){
    file.read((char*) samples, sizeof(long));
    file.read((char*) bs, sizeof(int));
    char tmp;
    file.read(&tmp, sizeof(char));
    *channels = (unsigned int) tmp;
    file.read(&tmp, sizeof(char));
    *factor = (unsigned int) tmp;
    file.read((char*) lf, sizeof(int));
}

void RealAudioCodec::writeHeader(ofstream& file, long samples, int bs, int channels, int fact, int lf){
    file.write((char*) &samples, sizeof(long));
    file.write((char*) &bs, sizeof(int));
    char tmp = (char) (channels & 0xFF);
    file.write(&tmp, sizeof(char));
    tmp = (char) (fact & 0xFF);
    file.write(&tmp, sizeof(char));
    file.write((char*) &lf, sizeof(int));
}

void RealAudioCodec::writeWavToFile(string filename, vector<short>& left, vector<int>& delta, int channels){
    short *sample = new short[2];
    SndfileHandle soundFile = SndfileHandle(filename, SFM_WRITE, SF_FORMAT_WAV | SF_FORMAT_DPCM_16, channels, 44100);
    cout << "Writing decoded file to: " << filename << endl;
    vector<short> all;
    if (channels == 2){
        for (int i = 0; i < left.size(); i++){
            all.push_back(left.at(i));
            all.push_back((short) (left.at(i) - delta.at(i)));
        }
    }
    else{
        for (int i = 0; i < left.size(); i++){
            all.push_back(left.at(i));
        }
    }

    sf_count_t total_size = all.size() * sizeof(short);
    soundFile.writeRaw(all.data(), total_size);
}

void RealAudioCodec::writeToFile(int data, int length, char type, ofstream& file){
     if (type == 'q') {
        int length_to_go = length;
        while(length_to_go >= 8) {
            buffer = (buffer << 8) | 0xFF;
            bits_in_buffer += 8;
            flush(file);
            length_to_go -= 8;
        }
        if(length_to_go > 0) {
            buffer = ((buffer << length_to_go) | ((0x00FF >> (8 - length_to_go)) & 0xFF));
            bits_in_buffer += length_to_go;
            flush(file);
        }
        buffer = buffer << 1;
        bits_in_buffer += 1;
        flush(file);
    }
    else if (type == 'r') {
        buffer = (buffer << length) | (data & buildOnes(length));
        bits_in_buffer += length;
        flush(file);
    }
    else if (type == 'p') {
        buffer = (buffer << length) | (data & 0x03);
        bits_in_buffer += length;
        flush(file);
    }
    else if (type == 'm') {
        buffer = (buffer << length) | (data & 0xFF);
        bits_in_buffer += length;
        flush(file);
    }
    else if (type == 'f') {
        forceFlush(file);
    }
}

void RealAudioCodec::flush(ofstream& file){
    char tmp[1];
    while (bits_in_buffer >= 8){
        tmp[0] = (char) ((buffer >> (bits_in_buffer - 8)) & 0xFF);
        file.write(tmp, 1);
        bits_in_buffer -= 8;
        buffer = buffer & buildOnes(bits_in_buffer);
    }

}

void RealAudioCodec::forceFlush(ofstream& file){
    char tmp[1];
    while (bits_in_buffer >= 8) {
        tmp[0] = (char) ((buffer >> (bits_in_buffer - 8)) & 0xFF);
        file.write(tmp, 1);
        bits_in_buffer -= 8;
        buffer = buffer & buildOnes(bits_in_buffer);
    }
    if (bits_in_buffer > 0) {
        tmp[0] = (char) ((buffer << (8 - bits_in_buffer)) & 0xFF);
        file.write(tmp, 1);
        buffer = 0;
        bits_in_buffer = 0;
    }
}

int RealAudioCodec::buildOnes(int n){
    unsigned int tmp = ~0;
    return (tmp >> (32-n));
}
