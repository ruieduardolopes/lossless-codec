#include <vector>
#include <boost/algorithm/string.hpp>

#include "boost/filesystem.hpp"
#include "boost/program_options.hpp"
#include "AdvancedPredictor.h"
#include "Golomb.h"
#include "NaivePredictor.h"
#include "AudioHandler.h"

using namespace std;

constexpr int BLOCK_SIZE = 256;
constexpr int PREDICTOR_OVERHEAD = 2;
constexpr int GOLOMB_OVERHEAD = 0;
string VERSION = "1.0.0";

template <class T>
ostream& operator<<(ostream& os, const vector<T>& v)
{
    copy(v.begin(), v.end(), ostream_iterator<T>(cout, " "));
    return os;
}

typedef vector<string> string_vector;

int parse_arguments(int argc, char *argv[], int *lossyFactor, bool *decode, bool *dynamicMValue, bool *quiet, string_vector& input_files) {
	try {
		std::string appName = boost::filesystem::basename(argv[0]);
		namespace po = boost::program_options;
		po::options_description desc("Options");
		desc.add_options()
			("help,h", "Print this help message")
			("lossy,l", po::value<int>(lossyFactor), "Encode file with lossy mechanism")
			("decode,d", "Decode WAVz audio file")
            ("m-static,s", "runs encoder/decoder function with a per block Golomb M value, with M=2^k")
            ("quiet,q", "Quiet mode")
			("version,v", "See version");
        po::options_description hidden("Hidden options");
        hidden.add_options()
            ("input-file", po::value< vector<string> >(), "input file");
        
        po::options_description cmdline_options;
        cmdline_options.add(desc).add(hidden);

        po::positional_options_description p;
        p.add("input-file", -1);

		po::variables_map vm;
		try {
			po::store(po::command_line_parser(argc, argv).options(cmdline_options).positional(p).run(), vm);
			if (vm.count("help")) { 
				std::cout << "This command encodes/decodes WAV audio files in no time, composing/decomposing \n" << 
							 "WAVz files, as PCM-16 bit WAV audio compressed files.\n\n" 
						  << "Usage: \n" 
						  << "losslesscdc [option...] file \n\n" 
						  << "  {-d | --decode}\n"
						  << "    decode FILE, where FILE is a WAVz compressed file\n"
						  << "  {-l | --lossy FACTOR} \n"
						  << "    encode FILE with lossy mechanism by a factor of FACTOR bits\n"
                          << "  {-s | --m-static} \n"
						  << "    runs encoder/decoder function with a per block Golomb M value, with M=2^k\n"
					      << "  {-h | --help} \n"
						  << "    prints this help message\n"
						  << "  {-v | --version} \n"
						  << "    see version"
						  << std::endl;
				return 3;
			}
			if (vm.count("version")) {
				std::cout << "losslesscdc (Lossless WAV codec) v" + VERSION + "\n" 
						  << "This is free software; see the source for copying conditions.  There is NO\n"
						  << "warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE." << std::endl;
				return 3;
			}
            *decode = vm.count("decode");
            if (*decode) {
                cout << "decode on" << endl;
            } 
            if (vm.count("lossy")) {
                *lossyFactor = vm["lossy"].as<int>();
                if (*decode) {
                    cerr << "Argument \"lossy factor\" (-l | --lossy) has been ignored on decoding. Reading from file, instead" << endl;
                }
                cout << "lossyFactor of " << *lossyFactor << endl;
            }
            if (*lossyFactor < 0 || *lossyFactor > 15) {
                std::cerr << "ERROR: " << "The quantization factor must be an integer from 0 to 15." << std::endl;
                return 1; // TODO : handle this error
            }
            *dynamicMValue = !vm.count("m-static");
            if (*decode) {
                cout << "m-static on" << endl;
            } 
            *quiet = vm.count("quiet");
            if (*decode) {
                cout << "quiet on" << endl;
            } 
            if (vm.count("input-file")) {
                input_files = vm["input-file"].as<string_vector>();
                cout << "input-file is " << input_files << endl;
            }
            po::notify(vm);
		} catch (boost::program_options::required_option& e) {
			std::cerr << "ERROR: " << e.what() << std::endl;
			return 1; 
		} catch (boost::program_options::error& e) {
			std::cerr << "ERROR: " << e.what() << std::endl;
			return 1;
		}
	} catch (std::exception& e) {
		std::cerr << "Unhandled Exception reached the top of main: "
				  << e.what() << ", application will now exit" << std::endl;
		return 2; 
	}
	return 0;
}

void writeEncodedPredictorIndex(int writtenSamples, bstream& file, AdvancedPredictor& predictor) {
    switch (predictor.getUsedPredictorOn(writtenSamples/BLOCK_SIZE)) {
        case 0: 
            file.writeBit(0);
            file.writeBit(0);
            break;
        case 1:
            file.writeBit(0);
            file.writeBit(1);
            break;
        case 2: 
            file.writeBit(1);
            file.writeBit(0);
            break;
        case 3:
            file.writeBit(1);
            file.writeBit(1);
            break;
        default:
            break;
    }
}

int estimateGolombMValue(vector<int> vec, int blockLength) {
    int i = 0;
    int mValue;
    int quantity;
    int quotient;
    int toReturn;
    int bitAmount;
    int kValue = 2;
    long golombFileLength = -1;
    vector<int>::iterator iterator;
    long golombFileLengthTemp = LONG_MAX;

    while (golombFileLength <= golombFileLengthTemp) {
        if (golombFileLength != -1 && golombFileLength < golombFileLengthTemp) {
            golombFileLengthTemp = golombFileLength;
        }
        golombFileLength = 0;
        toReturn = mValue;
        mValue = (int)pow(2, kValue);
        bitAmount = (int)log2(mValue);
        for (iterator = vec.begin(); iterator != vec.end(); i++, iterator++) {
            if (*iterator < 0) {
                quantity = -(1 + *iterator * 2);
            } else {
                quantity = 2 * *iterator;
            }
            quotient = quantity / mValue;
            golombFileLength += (quotient + 1); // from unary coding
            golombFileLength += bitAmount;      // from truncated coding (considering that m = 2^k!)
            if (i % blockLength == 0) {
                golombFileLength += PREDICTOR_OVERHEAD + GOLOMB_OVERHEAD;
            }
        }
        kValue++;
        if (kValue > 15) {
            break;
        }
    }
    return toReturn;
}

Golomb estimateAndWriteEncodedKGolomb(bstream& file, vector<int>* vec) {
    int m = estimateGolombMValue(*vec, BLOCK_SIZE);
    file.writeNBits(log2(m), 4);
    return Golomb(m);
}

int writeHeader(bstream& file, uint64_t size, uint64_t nFrames, AudioHandler& audio, int lossyFactor) {
    for (int k = 63; k >= 0; k--) {
        file.writeBit(size >> k);
    }
    for (int p = 63; p >= 0; p--) {
        file.writeBit(nFrames >> p);
    }
    for (int i = 31; i >= 0; i--) {
        file.writeBit(audio.getFormat() >> i);
    }
    for (int j = 31; j >= 0; j--) {
        file.writeBit(audio.getSamplerate() >> j);
    }
    for (int m = 31; m >= 0; m--) {
        file.writeBit(audio.getChannels() >> m);
    }
    if (lossyFactor != 0) {
        for (int m = 3; m >= 0; m--) {
            file.writeBit(lossyFactor >> m);
        }
    }
    return 0;
}

int readHeader(bstream& file, uint64_t *size, uint64_t *nFrames, int *format, int *samplerate, int *channels, int* lossyFactor, bool isLossy) {
    *size = file.readNBits(64);
    *nFrames = file.readNBits(64);
    *format = file.readNBits(32);
    *samplerate = file.readNBits(32);
    *channels = file.readNBits(32);
    if (isLossy) {
        *lossyFactor = file.readNBits(4);
    }
    return 0;
}

int main(int argc, char *argv[]) {
    bool dynamicMValue = 1, decode, quiet;
    int lossyFactor = 0;
    vector<string> input_files;
    int errorCode = parse_arguments(argc, argv, &lossyFactor, &decode, &dynamicMValue, &quiet, input_files);
    if (errorCode == 3) {
        exit(0);
    }

    vector<string> filename;
    boost::split(filename, input_files[0], [](char c){return c == '.';});
    if (decode == false) {
        if (filename.back().compare("wav") != 0) {
            cerr << "Wrong input file." << endl;
            return 1; // TODO : handle this error
        }
        AudioHandler audio = AudioHandler(input_files[0].data(), 1);
        AdvancedPredictor predictor = AdvancedPredictor(lossyFactor);
        vector<short> samples;
        predictor.setFramesBufferSize(BLOCK_SIZE);
        int nFrames = 0;
        vector<short> original;
        vector<int> residuals;
        while (true) {
            int frames;
            if ((frames = audio.loadBlock()) == -2) {
                break;
            }
            samples = audio.getSamples_16();
            original.insert(original.end(), samples.begin(), samples.end());
            nFrames += frames;
            for (int i = 0; i != (int)(samples.size()/BLOCK_SIZE); i++) {
                vector<short> block = vector<short>(samples.begin()+(BLOCK_SIZE*i), samples.begin()+(BLOCK_SIZE*(i+1)));
                predictor.setSamples(block);
                predictor.predict();
            }
        }
        residuals = predictor.getResiduals();

        bstream output = bstream{ lossyFactor == 0 ? string(filename[0]+".wavz").data() : string(filename[0]+".wavqz").data() , ios::out|ios::binary };

        writeHeader(output, residuals.size(), nFrames, audio, lossyFactor);

        Golomb golomb = Golomb(estimateGolombMValue(residuals, BLOCK_SIZE)); // To perform globl
        int writtenSamples = 0;
        for (int i = 0; i != residuals.size(); i++) {
            if (writtenSamples++ % BLOCK_SIZE == 0) {
                writeEncodedPredictorIndex(writtenSamples-1, output, predictor);
                if (dynamicMValue) {
                    vector<int> block;
                    block.insert(block.begin(), residuals.begin()+((writtenSamples/BLOCK_SIZE)*BLOCK_SIZE), residuals.begin()+((writtenSamples/BLOCK_SIZE)*BLOCK_SIZE+BLOCK_SIZE));
                    golomb = estimateAndWriteEncodedKGolomb(output, &block);
                }
            }
            golomb.encode(residuals[i], output);
        }
        golomb.endEncode(output);
    } else {
        if (filename.back().compare("wavz") != 0 && filename.back().compare("wavqz") != 0) {
            cerr << "Wrong input file." << endl;
            return 1; // TODO : handle this error
        }
        Golomb golomb = Golomb(2);
        bstream input = bstream{ input_files[0].data() , ios::in|ios::binary };
        vector<int> fromFile;
        int readSamples = 0;
        vector<char> predictorUsed;
        uint64_t size, numberOfFrames;
        int format, samplerate, channels, lossyFactor = 0;
        readHeader(input, &size, &numberOfFrames, &format, &samplerate, &channels, &lossyFactor, filename[1].compare("wavqz") == 0);
        AdvancedPredictor predictor = AdvancedPredictor(lossyFactor);
        predictor.setFramesBufferSize(BLOCK_SIZE);
        string finalName = filename[0];
        for (int i = 0; i != size; i++) {
            if (readSamples++ % BLOCK_SIZE == 0) {
                predictorUsed.push_back(input.readNBits(2));
                if (dynamicMValue) {
                    golomb = Golomb(pow(2,input.readNBits(4)));
                }
            }
            fromFile.push_back(golomb.decode(input));
        }
        predictor.setUsedPredictor(predictorUsed);
        predictor.setResiduals(fromFile);
        predictor.revert();
        vector<short> samples = predictor.getRevertSamples(); 
        AudioHandler audio2 = AudioHandler();
        audio2.save(string(filename[0]+".recovered.wav"), samples, numberOfFrames, format, channels, samplerate); 
    }
    return 0;
}



