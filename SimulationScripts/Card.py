####################################################################################
#
#  Card.py
#  Defines the class Card, a class to read card files for general input information
#  Input information to be used in simulation
#
####################################################################################

import sys
import shlex

class Card:
    
    def __init__(self, filename):
        self.name = filename
        self.data = {}

        infile = open(filename,'r')
        for line in infile:
            hashtag = line.find('#')
            line = line[:hashtag]

            if line.strip().endswith('Start'):
                keyPos = line.rfind('Start')
                keyVal = line[:keyPos]
                self.data[keyVal] = ''
                for line in infile:
                    if line.startswith('%sEnd'%(keyVal)):
                        break
                    self.data[keyVal] += line

            words = shlex.split(line)
            if len(words) < 2:
                continue
            if len(words) > 2 and not words[2].startswith('#'):
                continue
            self.data[words[0]] = words[1]

    def Print(self):
        print 'Card info from file:', self.name
        for info in self.data:
            print '%s: %s' % (info, self.data[info])


    def GetFileName(self):
        return self.name

    def Has(self,info):
        if not info in self.data:
            return False
        return True
           
    def Get(self,info):
        if not self.Has(info):
            print info, 'not found in card', self.name
            sys.exit()

        return self.data[info]
