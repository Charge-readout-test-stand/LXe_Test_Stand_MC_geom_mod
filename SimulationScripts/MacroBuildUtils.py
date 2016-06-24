####################################################################################
#
#  MacroBuildUtils.py
#  Defines useful utilities to build macros for nEXO simulations
#
####################################################################################

import Card, SimUtils
import math
import sys

def CreateMacro(info, card):

    macroName = info['MacroFileName']
    if card.Has('UseScratchDir') and card.Get('UseScratchDir').lower() != 'no':
      g4Name = info['ScratchDir'] + '/' + info['G4FileName'].split('/')[-1]
    else:
      g4Name = info['G4FileName']
    pos = info['FullLocationName']
    isotope = info['Isotope']

    macroFile = open(macroName,'w')
    macroFile.write(GetHeader(g4Name,card.Get('G4TreeName')))
    macroFile.write(GetBody(card))
    macroFile.write(GetParticle(isotope))
    macroFile.write(GetLocation(pos, card))
    macroFile.write(GetRunBeam(card))
    macroFile.close()
    
    return True

def GetHeader(g4Name,treeName):
    
    return """
/analysis/setOutputROOTFileName %s
/analysis/setOutputROOTTreeName %s

""" % (g4Name, treeName)

def GetBody(card):
   
    return card.Get('MacroModel')

def GetParticle(isotope):

    if isotope in ['bb2n','bb0n','gps']:
        return "/generator/setGenerator %s \n" % (isotope)

    Z = SimUtils.GetAtomicNumber(isotope)
    A = SimUtils.GetAtomicMass(isotope)

    ionBody = """
/generator/setGenerator gps

/gps/energy 0 keV
/gps/ion %d %d
/grdm/nucleusLimits %d %d %d %d

""" % (Z,A,A,A,Z,Z)

    return ionBody

def GetRunBeam(card):
    
    events = int(card.Get('TotalNumberOfEvents')) // int(card.Get('NumberOfJobs'))
    #branchEvents = int(math.floor(events * ratio))

    return """/run/beamOn %d \n""" % (events)

def GetLocation(location, card):
    
    locationType = card.Get('LocationType')

    if locationType == 'ScriptConfinement':

        stdLines = """
/gps/pos/type Volume
/gps/pos/shape Cylinder
/gps/pos/centre 0 0 0 cm
/gps/pos/halfz %d cm
/gps/pos/radius %d cm
/gps/pos/confine %s
\n
"""
	volSize = 500
        if location == '/nEXO/TPCVessel': volSize = 200
        return stdLines % (volSize,volSize,location)

    elif locationType == 'ScriptDefinition':
        
        if location == 'Cathode':
            return """
/gps/pos/type Plane
/gps/pos/shape Circle
/gps/pos/radius 64.6 cm
/gps/pos/rot1 0 1 0
/gps/pos/rot2 0 0 1
/gps/pos/centre 63.2 0 0 cm
/gps/ang/type iso

"""
        elif location == 'Cathode2':
            return """
/gps/pos/type Volume
/gps/pos/shape Cylinder
/gps/pos/centre 0 0 0 cm
/gps/pos/halfz 200 cm
/gps/pos/radius 200 cm
/gps/pos/confine /nEXO/TPCInternals/LXe/Cathode

"""
        
        else:
            print 'Location specified in card was not found in script!'
            sys.exit()

    elif locationType == 'LocationMacro':
        return card.Get(location)
        
    print 'Location not correctly specified in card!'
    sys.exit() 

    return
