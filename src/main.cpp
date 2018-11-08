#include <vector>

#include "boost/filesystem.hpp"
#include "boost/program_options.hpp"
#include "AdvancedPredictor.h"
#include "Golomb.h"
#include "NaivePredictor.h"
#include "AudioHandler.h"

using namespace std;

constexpr int BLOCK_SIZE = 512;
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

int parse_arguments(int argc, char *argv[], bool *lossy, bool *decode, bool *quiet, string_vector& input_files) {
	try {
		std::string appName = boost::filesystem::basename(argv[0]);
		namespace po = boost::program_options;
		po::options_description desc("Options");
		desc.add_options()
			("help,h", "Print this help message")
			("lossy,l", "Encode file with lossy mechanism")
			("decode,d", "Decode WAVz audio file")
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
						  << "  {-l | --lossy} \n"
						  << "    encode FILE with lossy mechanism\n"
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
                cout << "decode" << endl;
            }
            *lossy = vm.count("lossy");
            if (*lossy) {
                cout << "lossy" << endl;
            }
            *quiet = vm.count("quiet");
            if (*quiet) {
                cout << "quiet" << endl;
            }
            if (vm.count("input-file")) {
                input_files = vm["input-file"].as<string_vector>();
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
    cout << "the chosen M was " << toReturn << endl;
    return toReturn;
}

Golomb estimateAndWriteEncodedKGolomb(bstream& file, vector<int>* vec) {
    int m = estimateGolombMValue(*vec, BLOCK_SIZE);
    file.writeNBits(log2(m), 4);
    cout << "Deciding..."<< endl;
    return Golomb(m);
}

int writeHeader(bstream& file, uint64_t size, uint64_t nFrames, AudioHandler& audio) {
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
    return 0;
}

int readHeader(bstream& file, uint64_t *size, uint64_t *nFrames, int *format, int *samplerate, int *channels) {
    *size = file.readNBits(64);
    *nFrames = file.readNBits(64);
    *format = file.readNBits(32);
    *samplerate = file.readNBits(32);
    *channels = file.readNBits(32);
    return 0;
}

int main(int argc, char *argv[]) {
    bool lossy, decode, quiet;
    vector<string> input_files;
    parse_arguments(argc, argv, &lossy, &decode, &quiet, input_files);
    AudioHandler audio = AudioHandler("Pink Floyd - Pigs on the Wing, Part 1.wav", 1);
    AdvancedPredictor predictor = AdvancedPredictor();
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
    cout << "length: " << residuals.size() << endl;
    bstream output = bstream{ "output.wavz", ios::out|ios::binary };

    writeHeader(output, residuals.size(), nFrames, audio);

    Golomb golomb = Golomb(estimateGolombMValue(residuals, BLOCK_SIZE));
    int writtenSamples = 0;
    // cout << "before:" << endl;
    // for (auto sample : predictor.getUsedPredictorVector()) {
    //     cout << (int)sample << ", ";
    // }
    // cout << endl;

    cout << "predictors:" << endl;
    for (int i = 0; i != 500; i++) {
        cout << (int)predictor.getUsedPredictorVector()[i] << ", ";
    }
    cout << endl;

    // cout << "BEFORE:" << endl;
    // for (int j = 88; j != 91; j++) {
    //     for (int i = 0; i != 512; i++) {
    //         cout << residuals[i+(j*BLOCK_SIZE)] << ", ";
    //     }
    //     cout << endl;
    // }
    // cout << endl;

    cout << "writing to file:" << endl;
    for (int i = 0; i != 10000; i++) {
        cout << residuals[i] << ", ";
    }
    cout << endl;

    vector<int> ks;

    for (int i = 0; i != residuals.size(); i++) {
        if (writtenSamples++ % BLOCK_SIZE == 0) {
            writeEncodedPredictorIndex(writtenSamples-1, output, predictor);
            vector<int> block;
            block.insert(block.begin(), residuals.begin()+((writtenSamples/BLOCK_SIZE)*BLOCK_SIZE), residuals.begin()+((writtenSamples/BLOCK_SIZE)*BLOCK_SIZE+BLOCK_SIZE));
            golomb = estimateAndWriteEncodedKGolomb(output, &block);
            ks.push_back(golomb.mValue);
        }
        golomb.encode(residuals[i], output);
    }
    cout << endl;
    golomb.endEncode(output);

    cout << "First K's:" << endl;
    for (auto element : ks) {
        cout << element << ", ";
    }
    cout << endl;

    // Golomb golomb = Golomb(64);
    // AdvancedPredictor predictor = AdvancedPredictor();
    // predictor.setFramesBufferSize(BLOCK_SIZE);
    // vector<short> samples;













    bstream input = bstream{ "output.wavz", ios::in|ios::binary };
    vector<int> fromFile;
    cout << "DECODED:" << endl;
    int readSamples = 0;
    vector<char> predictorUsed;

    uint64_t size, numberOfFrames;
    int format, samplerate, channels;
    readHeader(input, &size, &numberOfFrames, &format, &samplerate, &channels);

    vector<int> kss;

    cout << "read samples : " ;
    // cout << "channels are " << channels << endl;
    cout << "AFTER:" << endl;
    for (int i = 0; i != size; i++) {
        if (readSamples++ % BLOCK_SIZE == 0) {
            // cout << (readSamples-1) << ", ";
            predictorUsed.push_back(input.readNBits(2));
            golomb = Golomb(pow(2,input.readNBits(4)));
            kss.push_back(golomb.mValue);
            // if (i < 1024) cout << "P: " << (int)predictorUsed.back() << ", ";
        }
        fromFile.push_back(golomb.decode(input));
        // if (i < 1024) cout << fromFile.back() << ", ";
    }
    cout << endl;

    cout << "Second K's:" << endl;
    for (auto element : kss) {
        cout << element << ", ";
    }
    cout << endl;

    cout << "predictors:" << endl;
    for (int i = 0; i != 500; i++) {
        cout << (int)predictorUsed[i] << ", ";
    }
    cout << endl;

    cout << "read from file:" << endl;
    for (int i = 0; i != 10000; i++) {
        cout << fromFile[i] << ", ";
    }
    cout << endl;

    // for (int j = 88; j != 91; j++) {
    //     for (int i = 0; i != 512; i++) {
    //         cout << fromFile[i+(j*BLOCK_SIZE)] << ", ";
    //     }
    //     cout << endl;
    // }
    // cout << endl;

    cout << "length: " << fromFile.size() << endl;
    predictor.setUsedPredictor(predictorUsed);

    // cout << "after:" << endl;
    // for (auto sample : predictor.getUsedPredictorVector()) {
    //     cout << (int)sample << ", ";
    // }
    // cout << endl;

    predictor.setResiduals(fromFile);
    predictor.revert();
    // cout << "size of int is " << sizeof(uint64_t) << endl;
    samples = predictor.getRevertSamples(); 
    // audio.save("file2.compressed.wav", samples, nFrames);
    AudioHandler audio2 = AudioHandler();
    audio2.save("file2.compressed.wav", samples, nFrames, audio.getFormat(), audio.getChannels(), audio.getSamplerate()); 
    // cout << "Saved new file with ..." << endl;
    // // cout << "samples : " << samples << endl;
    // cout << "numberOfFrames : " << numberOfFrames << endl;
    // cout << "format : " << format << endl;
    // cout << "channels : " << channels << endl;
    // cout << "samplerate : " << samplerate << endl;
    return 0;
}



