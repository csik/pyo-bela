#pragma once

#include "m_pyo.h"
#include <Bela.h>

typedef int callPtr(int);

class Pyo {
    public:
        ~Pyo();
        void setup(int nChannels, int bufferSize, int sampleRate, int nAnalogChannels, int nDigitalChannels);
        void process(float *buffer);
        void fillin(const float *buffer);
        void analogin(const float *buffer);
        void analogout(float *buffer);
        void clear();
        int get_global();
        void midievent(int status, int data1, int data2);
        int loadfile(const char *file, int add);
        int exec(const char *msg);
        int value(const char *name, float value);
        int value(const char *name, float *value, int len);
        int set(const char *name, float value);
        int set(const char *name, float *value, int len);

    private:
        int nChannels;
        int bufferSize;
        int sampleRate;
        int nAnalogChannels;
        int nTotalChannels;
        PyThreadState *interpreter;
        float *pyoInBuffer;
        float *pyoOutBuffer;
        callPtr *pyoCallback;
        int pyoId;
        char pyoMsg[262144];
};
