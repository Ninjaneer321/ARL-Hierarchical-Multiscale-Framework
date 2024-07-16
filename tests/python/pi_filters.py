import hms
import os
import sys

#
# Allow SWIG type objects to be pickled
#
class PickleableSWIG:

    def __setstate__(self, state):
        self.__dict__ = state

    def __getstate__(self):
        state = self.__dict__
        if "this" in state:
            del state["this"]
        return state

class PiArgument(hms.Argument, PickleableSWIG):

    def __init__(self, x1, x2):
        hms.Argument.__init__(self)
        self.x1 = x1
        self.x2 = x2
        return
    
class PiInputFilter(hms.InputFilter, PickleableSWIG):
    
    def __init__(self, pointFileName):
        hms.InputFilter.__init__(self)
        self.pointFileName = pointFileName
        return
    
    def apply(self, argument, directory):
        path = os.path.join(directory, self.pointFileName)
        f = open(path, "w")
        f.write(repr(argument.x1) + "\n")
        f.write(repr(argument.x2) + "\n")
        
class PiOutputFilter(hms.OutputFilter, PickleableSWIG):

    def __init__(self):
        hms.OutputFilter.__init__(self)
        return

    def apply(self, directory, stdOut, argument):
        f = open(stdOut)
        value = float(f.readline())
        return PiValue(value)

class PiValue(hms.Value, PickleableSWIG):
    
    def __init__(self, value):
        hms.Value.__init__(self)
        self.value = value
        return


    
