#include "AudioHandler.h"

AudioHandler::AudioHandler(const char * filename) {
    audio = SndfileHandle(filename);
    loadAudioSpecs();
}

AudioHandler::AudioHandler(const char * filename, int blockLength) {
    audio = SndfileHandle(filename);
    loadAudioSpecs();
    this->blockLength = blockLength;
}

int AudioHandler::loadAudioSpecs() {
    channels = audio.channels();
    format = audio.format();
    samplerate = audio.samplerate();
    return 0;
}

int AudioHandler::loadAll() {
    size_t nFrames;
    switch (format & SF_FORMAT_SUBMASK) {
        case SF_FORMAT_PCM_S8:
            vector<char> tmp(FRAMES_BUFFER_SIZE*channels);
            while ((nFrames = audio.readf(tmp.data(), FRAMES_BUFFER_SIZE))) {
                for (auto sample : tmp) {
                    samples_8.push_back(sample);
                }
                tmp.resize(nFrames * channels);
            }
            break;
        case SF_FORMAT_PCM_16:
            vector<short> tmp(FRAMES_BUFFER_SIZE*channels);
            while ((nFrames = audio.readf(tmp.data(), FRAMES_BUFFER_SIZE))) {
                for (auto sample : tmp) {
                    samples_16.push_back(sample);
                }
                tmp.resize(nFrames * channels);
            }
            break;
        case SF_FORMAT_PCM_32:
            vector<int> tmp(FRAMES_BUFFER_SIZE*channels);
            while ((nFrames = audio.readf(tmp.data(), FRAMES_BUFFER_SIZE))) {
                for (auto sample : tmp) {
                    samples_32.push_back(sample);
                }
                tmp.resize(nFrames * channels);
            }
            break;
        default:
            // TODO : handle this error
            return 1;
    }
    return 0;
}

int AudioHandler::loadBlock() {
    if (blockLength == 0) {
        return 1; // TODO : handle this error
    }
    int count = 0;
    switch (format & SF_FORMAT_SUBMASK) {
        case SF_FORMAT_PCM_S8:
            samples_8.clear();
            vector<char> tmp(FRAMES_BUFFER_SIZE*channels);
            while ((nFrames = audio.readf(tmp.data(), FRAMES_BUFFER_SIZE))) {
                for (auto sample : tmp) {
                    samples_8.push_back(sample);
                }
                tmp.resize(nFrames * channels);
                if (++count % blockLength == 0) {
                    return 0;
                }
            }
            break;
        case SF_FORMAT_PCM_16:
            samples_16.clear();
            vector<short> tmp(FRAMES_BUFFER_SIZE*channels);
            while ((nFrames = audio.readf(tmp.data(), FRAMES_BUFFER_SIZE))) {
                for (auto sample : tmp) {
                    samples_16.push_back(sample);
                }
                tmp.resize(nFrames * channels);
                if (++count % blockLength == 0) {
                    return 0;
                }
            }
            break;
        case SF_FORMAT_PCM_32:
            samples_32.clear();
            vector<int> tmp(FRAMES_BUFFER_SIZE*channels);
            while ((nFrames = audio.readf(tmp.data(), FRAMES_BUFFER_SIZE))) {
                for (auto sample : tmp) {
                    samples_32.push_back(sample);
                }
                tmp.resize(nFrames * channels);
                if (++count % blockLength == 0) {
                    return 0;
                }
            }
            break;
        default:
            // TODO : handle this error
            break;
    }
    if (nFrames == 0) {
        return 2; // TODO : handle this error
    }
    return 0;
}

vector<char> AudioHandler::getSamples_8() {
    if ((format & SF_FORMAT_SUBMASK) != SF_FORMAT_PCM_S8) {
        throw BadAudioFormatException;
    }
    return samples_8;
}

vector<short> AudioHandler::getSamples_16() {
    if ((format & SF_FORMAT_SUBMASK) != SF_FORMAT_PCM_16) {
        throw BadAudioFormatException;
    }
    return samples_16;
}

vector<int> AudioHandler::getSamples_32() {
    if ((format & SF_FORMAT_SUBMASK) != SF_FORMAT_PCM_32) {
        throw BadAudioFormatException;
    }
    return samples_32;
}