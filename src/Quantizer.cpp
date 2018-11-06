#include "Quantizer.h"

using namespace std;

constexpr size_t FRAMES_BUFFER_SIZE = 65536;            // Buffer for reading frames
vector<int> Quantizer::residualSamples;
vector<short> Quantizer::revertedSamples;

Quantizer::Quantizer(SndfileHandle soundFileHandler, int factor) {
	Quantizer::codingFactor = factor;												// save value of quantization factor
	Quantizer::format = soundFileHandler.format();									// save format of input file
	Quantizer::frames = soundFileHandler.frames();									// save number of frames of input file
	Quantizer::channels = soundFileHandler.channels();								// save number of channels available
	Quantizer::samplerate = soundFileHandler.samplerate();							// save sampling rate of the input file
	std::vector<short> tmp(FRAMES_BUFFER_SIZE * soundFileHandler.channels());		// save vector of samples
	size_t nFrames;																	//
	while ((nFrames = soundFileHandler.readf(tmp.data(), FRAMES_BUFFER_SIZE))) {	// while there is a new frame to get on buffer,
		Quantizer::numberOfFrames += nFrames;
		for (auto sample : tmp) {													//     for sample in tmp
			Quantizer::samples.push_back(sample);									//         move sample to our list of samples
		}																			//
		tmp.resize(nFrames * soundFileHandler.channels());							//     clean tmp to a new vector with proper size.
	}
}

Quantizer::Quantizer(vector<short> samples, int factor, int flag) {
	Quantizer::codingFactor = factor;												// save value of quantization factor
	Quantizer::flag = flag;															// record activation flag value
	Quantizer::samples = samples;													// record samples 
}

int Quantizer::encode() {
	std::cout << Quantizer::samples.size() << std::endl;
	for (auto sample : Quantizer::samples) {										// for each 16-bit sample in samples,
		Quantizer::encodedSamples.push_back(sample >> Quantizer::codingFactor);		//     push back a truncated version on codingFactor-bits to encodedSample;
	}																				//
	return 0;																		// and finish by exit successfully.
}

int Quantizer::decode() {
	std::cout << Quantizer::encodedSamples.size() << std::endl;
	std::cout << Quantizer::decodedSamples.size() << std::endl;
	for (auto sample : Quantizer::encodedSamples) {									// for each 16-bit sample in encodedSamples,
		Quantizer::decodedSamples.push_back(sample << Quantizer::codingFactor);		//     push back a truncated version on codingFactor-bits to decodedSample;
	}																				//
	return 0;																		// and finish by exit successfully.
}

int Quantizer::saveBinary() {
	bstream file { "output.wavq", std::ios::out|std::ios::binary };					// create the output file
	if (file.is_open()) {															// if the file is considered open, then
		for (auto sample : Quantizer::encodedSamples) {								//     for each encoded sample,
			for (int i = 16 - Quantizer::codingFactor; i >= 0; i--) {				//         for each included bit on encoding,
				file.writeBit(sample >> i);											//             put each sample in order on file, bit-by-bit.
			}																		//
		}																			//
	}																				//
	file.grantWrite();																// grant that file counts with possible leftovers.
	file.close();																	// close the file.
	return 0;																		// exit successfully.
}

int Quantizer::loadBinary() {
	bstream file { "output.wavq", std::ios::in|std::ios::binary };					// create the input file
	Quantizer::decodedSamples.clear();												// clears all the elements present in vector decodedSamples
	if (file.is_open()) {															// if the file is considered open, then
		for (int i = 0; i != Quantizer::frames; i++) {								//     for each sample,
			uint8_t sample = 0;														//         create a constructable sample
			for (int j = 0; j != 16 - Quantizer::codingFactor ; j++) {				//         for each sample bit,
				sample |= (file.readBit() << j);									//             construct sample
			}																		//
			Quantizer::decodedSamples.push_back(sample);							//         
		}																			//
	}																				//
	file.close();																	// close the file.
	return 0;																		// exit successfully.
}

int Quantizer::saveWAVFile(std::string filename) {
	if (Quantizer::decodedSamples.empty()) {																	// if there are no decoded samples, then
		return 1; 																								//     do nothing and exit with error 1 (TODO)
	} 																											//
	SndfileHandle file { filename, SFM_WRITE, Quantizer::format, Quantizer::channels, Quantizer::samplerate };  // create audio file with precise characteristics
	// file.writeRaw(Quantizer::decodedSamples.data(), Quantizer::decodedSamples.size()*sizeof(short));								// write data into the WAV file
	std::cout << Quantizer::numberOfFrames << std::endl;
	file.writef(Quantizer::decodedSamples.data(), Quantizer::numberOfFrames);								// write data into the WAV file
	return 0;																									// exit successfully
}

// vector<short> Quantizer::quantize(vector<short>& samples, int factor){
// 	std::cout << samples.size() << std::endl;
// 	for (auto sample : samples) {										// for each 16-bit sample in samples,
// 		residualSamples.push_back(sample >> factor);		//     push back a truncated version on codingFactor-bits to encodedSample;
// 	}																				//
// 	return residualSamples;	
// }

