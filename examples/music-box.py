"""
Music box - Four voices randomly choosing frequencies
over a common scale.

"""
# Set to True to control the global gain with analog input 0.
WITH_ANALOG_INPUT = True
#osc = True

#def sendm():
## send message "/foo/message1" with int, float and string arguments
#    liblo.send(target, "/foo/message1", 123, 456.789, "test")
#
#met = Metro(time=.125).play()
#trig = TrigFunc(met, sendm)

if WITH_ANALOG_INPUT:
    v = Tone(Input(2), 8) # analog input 0 => global gain
    r = Sig(Input(3)/10) # set port rise time
    f = Sig(Input(4)/10) # set port fall time
else:
    v = Sig(0.5)
freq = 400
this = Sig(freq)
mid_freqs = [midiToHz(m+7) for m in [60,62,63.93,65,67.01,69,71,72]]
high_freqs = [midiToHz(m+7) for m in [72,74,75.93,77,79.01]]
freqs = [mid_freqs,mid_freqs,high_freqs,high_freqs]

chx = Choice(choice=freqs, freq=[2,3,3,4])
port = Port(chx, risetime=r, falltime=f)
sines = SineLoop(port, feedback=[.057,.033,.035,.016],
		         mul=[.1,.1,.05,.05]*v).out()

#def printInput():
#    print r.value,f.value
#    print Input(3)

#Tr = TrigFunc(Change(r), printInput)
#Tf = TrigFunc(Change(f), printInput)


