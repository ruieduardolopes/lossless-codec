#ifndef PREDICTOR_H
#define PREDICTOR_H

#include <vector>
#include <sndfile.hh>

using namespace std;

class Predictor {
    public:
        virtual int predict() = 0;
        virtual int revert() = 0;
        void setSamples(vector<short> samples) {
            originalAudioSamples = samples;
        }
        void setPredictedSamples(vector<short> samples) {
            predictedAudioSamples = samples;
        }
        void setRevertSamples(vector<short> samples) {
            revertedAudioSamples = samples;
        }
        vector<short> getPredictedSamples() {
            return predictedAudioSamples;
        }
        vector<short> getRevertSamples() {
            return revertedAudioSamples;
        }
        vector<short> getOriginalSamples() {
            return originalAudioSamples;
        }
    protected:
        vector<short> originalAudioSamples;
        vector<short> predictedAudioSamples;
        vector<short> revertedAudioSamples;
};

#endif