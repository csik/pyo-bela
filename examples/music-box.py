"""
Music box - Four voices randomly choosing frequencies
over a common scale.

"""


# Set to True to control the global gain with analog input 0.
WITH_ANALOG_INPUT = True
# comment following line to remove OSC compatibility
#osc = True
dig = True

if WITH_ANALOG_INPUT:
    v = Tone(Input(2), 8) # analog input 0 => global gain
    r = Sig(Input(3)/10) # set port rise time
    f = Sig(Input(4)/10) # set port fall time
else:
    v = Sig(0.5)
    r - Sig(.4)
    f = Sig(.3)

mid_freqs = [midiToHz(m+7) for m in [60,62,63.93,65,67.01,69,71,72]]
high_freqs = [midiToHz(m+7) for m in [72,74,75.93,77,79.01]]
freqs = [mid_freqs,mid_freqs,high_freqs,high_freqs]

chx = Choice(choice=freqs, freq=[2,3,3,4])
port = Port(chx, risetime=r, falltime=f)
sines = SineLoop(port, feedback=[.057,.033,.035,.016],
		         mul=[.1,.1,.05,.05]*v).out()
##

def pinsfunc(pin):
    3*3

def pinsChanged():
    arg = 3*3
    #print ("pin changed {} to value {}".format(arg, pins[arg].value))
    #print("some change")
    for i in range(0,4):
        if pins[i].value != te.freq[i]:
            pins[i].value = te.freq[i]
            light_handler(i)
    change_flag.value = 0
#    a = OscDataSend("if", 1234, "/osc-acknowledge")
#    msg = [1,3.14159]
#    a.send(msg)

def light_handler(pin):
    pass #print pin

pins =    [Sig(0),
           Sig(0),
           Sig(0),
           Sig(0)]

#pinsigs = [Change(pins[0]),
#           Change(pins[1]),
#           Change(pins[2]),
#           Change(pins[3])]
#
#pintrigs= [TrigFunc(pinsigs[0], pinsfunc, 0),
#           TrigFunc(pinsigs[1], pinsfunc, 1),
#           TrigFunc(pinsigs[2], pinsfunc, 2),
#           TrigFunc(pinsigs[3], pinsfunc, 3)]

#class Digipins(PyoObjectBase):
#    def __init__(self):
#        PyoObjectBase.__init__(self)
#        self.freq = [0.0,0.0,0.0,0.0]
#
#    def printpins(self):
#        loadpins();
#        pass # print self.freq[0]

class Test(Trig):
     def __init__(self):
         Trig.__init__(self)
         self.freq = [0.0,0.0,0.0,0.0]

     def process_pins(self):
         print(self.freq)


te = Test()
change_flag = Sig(0.0)
ch = Change(change_flag)
tr = TrigFunc(ch, pinsChanged)
#
#def loadpins():
#    print('test')
#    for i in range(0,4):
#        if pins[i].value != te.freq[i]:
#            print pins
#            pins[i].value = te.freq[i]
#

#asig = Change(allpins)
#atrig = TrigFunc(asig, loadpins)


#while(1):
#    loadpins()

