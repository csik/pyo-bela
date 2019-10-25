#include <Bela.h>
#include <cmath>
#include <iostream>
#include <Utilities.h>
#include "PyoClass.h"
#include "Python.h"

Pyo pyo;

void Bela_userSettings(BelaInitSettings *settings) {
    settings->periodSize = 32;
    settings->uniformSampleRate = 1;
    settings->numAnalogInChannels = 8;
    settings->numAnalogOutChannels = 8;
    settings->numDigitalChannels = 16;
    settings->analogOutputsPersist = 0;
}

bool gpin = 0;

bool setup(BelaContext *context, void *userData) {
    // Initialize a pyo server.
    pyo.setup(context->audioOutChannels, context->audioFrames, 
              context->audioSampleRate, context->analogOutChannels,
              context->digitalFrames);
    // Load a python file.
    char filename[] = "main.py";
    int ret = pyo.loadfile(filename, 0);
    if (ret != 0) {
        printf("Error: file \"%s\" not found", filename);
        return false;
    }
        return true;
}

void render(BelaContext *context, void *userData) {
    // Fill pyo input buffer (channels 0-1) with audio samples.
    pyo.fillin(context->audioIn);
    // Fill pyo input buffer (channels 2+) with analog inputs.
    pyo.analogin(context->analogIn);
    // Call pyo processing function and retrieve back stereo outputs.
    // pyo.digitalin(context);
    // Call pyo processing function and retrieve back stereo outputs.
    pyo.process(context->audioOut);
    // Get back pyo output channels 2+ as analog outputs.
    if (context->analogOut != NULL) {
        pyo.analogout(context->analogOut);
    }
    pyo.value("this", pyo.get_global());
    //if (digitalRead(context, 0, 12) != gpin){
    //    gpin = !gpin;
    //    pyo.value("this", 300+100*gpin);
    //}
}

void cleanup(BelaContext *context, void *userData) {}

