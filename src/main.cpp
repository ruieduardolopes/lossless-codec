#include <vector>

#include "boost/filesystem.hpp"
#include "boost/program_options.hpp"
#include "AdvancedPredictor.h"
#include "Golomb.h"
#include "NaivePredictor.h"
#include "AudioHandler.h"

using namespace std;

constexpr int BLOCK_SIZE = 512;
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

int writeEncodedPredictorIndex(int writtenSamples, bstream& file, AdvancedPredictor& predictor) {
    if (writtenSamples % BLOCK_SIZE == 0) {
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
    return writtenSamples + 1;
}

int writeHeader(bstream& file, long size, long nFrames, AudioHandler& audio) {
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

int readHeader(bstream& file, long *size, long *nFrames, int *format, int *samplerate, int *channels) {
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

    int nFrames = 0;
    vector<int> residuals;
    vector<short> samples;
    vector<short> original;
    AudioHandler audio = AudioHandler("sample07.wav", 1);
    AdvancedPredictor predictor = AdvancedPredictor();
    predictor.setFramesBufferSize(BLOCK_SIZE);

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

    // ENCODING to file
    bstream output = bstream{ "output.wavz", ios::out|ios::binary };
    // cout << "Before write" << endl;
    writeHeader(output, residuals.size(), nFrames, audio);
    // cout << "After write" << endl;
    Golomb golomb = Golomb(3);
    // cout << "Residual's size is " << (residuals.size()) << endl;
    int writtenSamples = 0;
    for (auto residual : residuals) {
        // cout << "Writing encoded predictor" << endl;
        writtenSamples = writeEncodedPredictorIndex(writtenSamples, output, predictor);
        // cout << "WrittenSamples" << endl;
        golomb.encode(residual, output);
    }
    golomb.endEncode(output);
    
    // cout << "End ENCODE" << endl;
    // Golomb golomb = Golomb(2);
    // AdvancedPredictor predictor = AdvancedPredictor();
    // predictor.setFramesBufferSize(BLOCK_SIZE);
    // vector<short> samples;

    // DECODING from file
    bstream input = bstream{ "output.wavz", ios::in|ios::binary };
    vector<int> fromFile;
    int readSamples = 0;
    vector<char> predictorUsed;
    long size, numberOfFrames;
    int format, samplerate, channels;
    readHeader(input, &size, &numberOfFrames, &format, &samplerate, &channels);
    cout << "Residual's size is " << size << endl;
    cout << "Residual's numberOfFrames is " << numberOfFrames << endl;
    cout << "Residual's format is " << format << endl;
    cout << "Residual's samplerate is " << samplerate << endl;
    cout << "Residual's channels is " << channels << endl;

    for (int i = 0; i != size; i++) {
        if (readSamples++ % BLOCK_SIZE == 0) {
            predictorUsed.push_back(input.readNBits(2));
        }
        fromFile.push_back(golomb.decode(input));
    }


    predictor.setResiduals(fromFile);
    predictor.setUsedPredictor(predictorUsed);

    predictor.revert();
    samples = predictor.getRevertSamples();
    cout << "Samples:" << endl;
    for (int i = 0; i != 10000; i++) {
        cout << samples[i] << ", ";
    }
    cout << endl;
    AudioHandler::save("file2.compressed.wav", samples, numberOfFrames, format, channels, samplerate);
    return 0;
}

