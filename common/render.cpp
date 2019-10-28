#include <Bela.h>
#include <cmath>
#include <iostream>
#include <Utilities.h>
#include "PyoClass.h"
#include "Python.h"
#include <math.h>

#include <Bela.h>
#include <OSCServer.h>
#include <OSCClient.h>

Pyo pyo;

void Bela_userSettings(BelaInitSettings *settings) {
    settings->periodSize = 64;
    settings->uniformSampleRate = 1;
    settings->numAnalogInChannels = 8;
    settings->numAnalogOutChannels = 8;
    settings->numDigitalChannels = 16;
    settings->analogOutputsPersist = 0;
}


OSCServer oscServer;
OSCClient oscClient;

int localPort = 7562;
int remotePort = 7563;
const char* remoteIp = "127.0.0.1";

bool gpin = 0;
int frequency = 100;
float gwaitaminute = 0;
int gdelay = 0;
bool GOSC = 0;

void parseMessage(oscpkt::Message msg){
    rt_printf("received message to: %s\n", msg.addressPattern().c_str());
    int intArg;
    float floatArg;
    if (msg.match("/osc-test").popInt32(intArg).popFloat(floatArg).isOkNoMoreArgs()){
        rt_printf("received int %i and float %f\n", intArg, floatArg);
    }
}

bool setup(BelaContext *context, void *userData) {
    // Initialize a pyo server.
    pyo.setup(context->audioOutChannels, context->audioFrames, 
              context->audioSampleRate, context->analogOutChannels,
              context->digitalFrames);
    // Load a python file.
    char filename[] = "main.py";
    int ret = pyo.loadfile(filename, 0);
    if (pyo.get_python_attr("osc")) {    
        // OSC CODE
        oscServer.setup(localPort);//this is not actually needed if you do not need to receive messages
	    oscClient.setup(remotePort, remoteIp);
        // the following code sends an OSC message to address /osc-setup
        // then waits 1 second for a reply on /osc-setup-reply
        bool handshakeReceived = false;
        oscClient.sendMessageNow(oscClient.newMessage.to("/osc-setup").end());
        oscServer.receiveMessageNow(1000);
        while (oscServer.messageWaiting()){
            if (oscServer.popMessage().match("/osc-setup-reply")){
                handshakeReceived = true;
            }
        }
        if (handshakeReceived){
            rt_printf("handshake received!\n");
        } else {
             rt_printf("timeout!\n");
        } 
        GOSC = 1;
    }
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
    frequency = pyo.get_global();
    if (gwaitaminute > 7500) {
        gwaitaminute = 0;
    }
    //pyo.set_global(gwaitaminute);
    gwaitaminute+=2;
    if (gdelay >= 20) {
        gdelay = 0;

        char fchar[32];
        sprintf(fchar, "this.value=%f", gwaitaminute );
        pyo.exec(fchar);
    }
    gdelay = gdelay+1;
    
    if (GOSC) {
        // receive OSC messages, parse them, and send back an acknowledgment
        while (oscServer.messageWaiting()){
            parseMessage(oscServer.popMessage());
            oscClient.queueMessage(oscClient.newMessage.to("/osc-acknowledge").add(5).add(4.2f).add(std::string("OSC message received")).end());
        }
    }
    //exec("this = ")
    //if (digitalRead(context, 0, 12) != gpin){
    //    gpin = !gpin;
    //    pyo.value("this", 300+100*gpin);
    //}
}

void cleanup(BelaContext *context, void *userData) {}

