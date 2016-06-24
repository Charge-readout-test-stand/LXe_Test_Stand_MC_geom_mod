####################################################################################
#
#  SimUtils.py
#  Defines common and useful utilities and information related to nEXO simulations
#
#  Send comments to licciard@triumf.ca
#
####################################################################################

import ROOT
import os, sys
from Card import Card

def GetElementSymbolAndMass(fullName):
    
    if fullName in ['bb2n','bb0n']:
        return 0, 0

    symbol = ''
    mass = ''

    for letter in fullName:
        if letter.isdigit():
            mass += letter
        else:
            symbol += letter

    return symbol, mass

def GetAtomicNumber(ionName):
    """ Ion name should contain two letters and the atomic mass """

    zs = {'Ru': '44', 'Re': '75', 'Rf': '104', 'Rg': '111', 'Ra': '88', 'Rb': '37', 'Rn': '86', 'Rh': '45', 'Be': '4', 'Ba': '56', 'Bh': '107', 'Bi': '83', 'Bk': '97', 'Br': '35', 'Uuh': '116', 'H': '1', 'P': '15', 'Os': '76', 'Hg': '80', 'Ge': '32', 'Gd': '64', 'Ga': '31', 'Uub': '112', 'Pr': '59', 'Pt': '78', 'Pu': '94', 'C': '6', 'Pb': '82', 'Pa': '91', 'Pd': '46', 'Cd': '48', 'Po': '84', 'Pm': '61', 'Hs': '108', 'Uuq': '114', 'Uup': '115', 'Uus': '117', 'Uuo': '118', 'Ho': '67', 'Hf': '72', 'K': '19', 'He': '2', 'Md': '101', 'Mg': '12', 'Mo': '42', 'Mn': '25', 'O': '8', 'Mt': '109', 'S': '16', 'W': '74', 'Zn': '30', 'Eu': '63', 'Es': '99', 'Er': '68', 'Ni': '28', 'No': '102', 'Na': '11', 'Nb': '41', 'Nd': '60', 'Ne': '10', 'Np': '93', 'Fr': '87', 'Fe': '26', 'Fm': '100', 'B': '5', 'F': '9', 'Sr': '38', 'N': '7', 'Kr': '36', 'Si': '14', 'Sn': '50', 'Sm': '62', 'V': '23', 'Sc': '21', 'Sb': '51', 'Z': '40', 'Sg': '106', 'Se': '34', 'Co': '27', 'Cm': '96', 'Cl': '17', 'Ca': '20', 'Cf': '98', 'Ce': '58', 'Xe': '54', 'Lu': '71', 'Cs': '55', 'Cr': '24', 'Cu': '29', 'La': '57', 'Li': '3', 'Tl': '81', 'Tm': '69', 'Lr': '103', 'Th': '90', 'Ti': '22', 'Te': '52', 'Tb': '65', 'Tc': '43', 'Ta': '73', 'Yb': '70', 'Db': '105', 'Dy': '66', 'Ds': '110', 'I': '53', 'U': '92', 'Y': '39', 'Ac': '89', 'Ag': '47', 'Uut': '113', 'Ir': '77', 'Am': '95', 'Al': '13', 'As': '33', 'Ar': '18', 'Au': '79', 'At': '85', 'In': '49'}

    symbol, mass = GetElementSymbolAndMass(ionName)
    if not symbol in zs: return 0
    return int(zs[symbol])
        
def GetAtomicMass(ionName):
    """ Ion name should contain two letters and the atomic mass """

    symbol, mass = GetElementSymbolAndMass(ionName)

    return int(mass)

def GetVolumeName(firstName = '',lastName = ''):
    
    if firstName != '' and lastName != '':
        return firstName + lastName

    if firstName != '':
        return firstName
    if lastName != '':
        return lastName

    print 'No volume defined for this simulation, quitting...'
    sys.exit()

    return ''

def GetRadialAndAxialCoordinates(x,y,z, card):

    if card.Get('AxialCoordinate').lower() == 'x':
        return y, z, x
    elif card.Get('AxialCoordinate').lower() == 'y': 
        return z, x, y
    elif card.Get('AxialCoordinate').lower() == 'z': 
        return x, y, z
    else:
        print 'Axial coordinate must be defined as x, y or z!'
        sys.exit()
        
def GetRadialAndAxialCoordinatesString(card):

    if card.Get('AxialCoordinate').lower() == 'x':
        return 'Y','Z','X'
    elif card.Get('AxialCoordinate').lower() == 'y': 
        return 'Z','X','Y'
    elif card.Get('AxialCoordinate').lower() == 'z': 
        return 'X','Y','Z'
    else:
        print 'Axial coordinate must be defined as x, y or z!'
        sys.exit()

def GetFilesInfo(card, options = []):

    optionsAvailable = ['jobs','g4','cluster','recon']

    if len(options) == 0:
        print 'You must specify what part of the process to get information about files!'
        print 'Available options =', optionsAvailable
        sys.exit()

    for option in options:
        if not option in optionsAvailable:
            print 'You are requesting an unavailable option', option
            print 'Available options =', optionsAvailable
            sys.exit()

    if not option in options:
        print 'You must specify what part of the process to get information about files!'
        print 'Options =', options


    resultList = []

    initSeed = int(card.Get('StartRandomSeed'))
    for job in range(int(card.Get('NumberOfJobs'))):
        seed = job + initSeed
        for pos in card.Get('LocationList').split(','):
            for isotope, ratio in zip(card.Get('DecayChainList').split(','), card.Get('BranchRatioList').split(',')):
                location = pos[pos.rfind('/')+1:]

                resultInfo = {}

                resultInfo['FullLocationName'] = pos
                resultInfo['ShortLocationName'] = location
                resultInfo['Isotope'] = isotope
                resultInfo['NumberOfEvents'] = int(card.Get('TotalNumberOfEvents')) // int(card.Get('NumberOfJobs'))
                resultInfo['Seed'] = int(seed)
                resultInfo['BranchRatio'] = float(ratio)
                
                fileNamePattern = card.Get('FileNamePattern')
                fileNamePattern = fileNamePattern.replace('[ParentName]',card.Get('ParentName'))
                fileNamePattern = fileNamePattern.replace('[Isotope]',isotope)
                fileNamePattern = fileNamePattern.replace('[Location]',location)
                fileNamePattern = fileNamePattern.replace('[Seed]',str(seed))                

                hasFileTopDir = card.Has('FileTopDir')

                if 'jobs' in options:        
                    jobNamePattern = fileNamePattern.replace('[OutputNamePrefix]',card.Get('JobsOutputNamePrefix'))
                    jobFullPath = card.Get('JobsFullOutputPath')
                    if hasFileTopDir: jobFullPath = jobFullPath.replace('[FileTopDir]',card.Get('FileTopDir'))
                    resultInfo['JobFileName'] = '%s/%s.sh'%(jobFullPath,jobNamePattern)

                if 'g4' in options:
                    g4NamePattern = fileNamePattern.replace('[OutputNamePrefix]',card.Get('G4OutputNamePrefix'))
                    
                    g4FullPath = card.Get('G4FullOutputPath')
                    if hasFileTopDir: g4FullPath = g4FullPath.replace('[FileTopDir]',card.Get('FileTopDir'))
                    resultInfo['G4FileName'] = '%s/%s.%s.root'%(g4FullPath,g4NamePattern,card.Get('G4TreeName'))

                    if card.Has('MacrosFullOutputPath'):  # For Backward compatibility
                        macroFullPath = card.Get('MacrosFullOutputPath')
                        if hasFileTopDir: macroFullPath = macroFullPath.replace('[FileTopDir]',card.Get('FileTopDir'))
                        resultInfo['MacroFileName'] = '%s/%s.mac'%(macroFullPath,g4NamePattern)
                    else:                        
                        resultInfo['MacroFileName'] = '%s/%s.mac'%(g4FullPath,g4NamePattern)

                if 'cluster' in options:
                    clusterNamePattern = fileNamePattern.replace('[OutputNamePrefix]',card.Get('ClusterOutputNamePrefix'))
                    clusterFullPath = card.Get('ClusterFullOutputPath')
                    if hasFileTopDir: clusterFullPath = clusterFullPath.replace('[FileTopDir]',card.Get('FileTopDir'))
                    resultInfo['ClusterFileName'] = '%s/%s.%s.root'%(clusterFullPath,clusterNamePattern,card.Get('ClusterTreeName'))

                if 'recon' in options:
                    reconNamePattern = fileNamePattern.replace('[OutputNamePrefix]',card.Get('ReconOutputNamePrefix'))
                    reconFullPath = card.Get('ReconFullOutputPath')
                    if hasFileTopDir: reconFullPath = reconFullPath.replace('[FileTopDir]',card.Get('FileTopDir'))
                    resultInfo['ReconFileName'] = '%s/%s.%s.root'%(reconFullPath,reconNamePattern,card.Get('ReconTreeName'))
                                    
                # Scratch directory
                if card.Has('UseScratchDir') and card.Get('UseScratchDir').lower() != 'no':
                    scratchDir = card.Get('UseScratchDir')
                    scratchDir = scratchDir.replace('[ParentName]',card.Get('ParentName'))
                    scratchDir = scratchDir.replace('[Isotope]',isotope)
                    scratchDir = scratchDir.replace('[Location]',location)
                    scratchDir = scratchDir.replace('[Seed]',str(seed))                
                    resultInfo['ScratchDir'] = scratchDir

                resultList.append(resultInfo)

    return resultList    

## Function to smear energy of 1D histogram
## This function is slow, consider large bins of smeared histogram (14 keV or 20 keV)
## Input energy histogram should ideally start at 0 keV for a valid smeared histogram of any energy range
def SmearHisto1D(histoIn, resol, rebin = 1):
    ## histoIn = 1D histogram with unsmeared energy, preferably fine bins (1 keV)
    ## resol = resolution function (TF1) as a function of energy
    ## rebin = energy rebinning of output (smeared) 1D histogram

    binIn = histoIn.GetXaxis()
    elow = binIn.GetBinLowEdge(1)

    #histoOut = copy.copy(histoIn)
    histoOut = histoIn.Rebin(rebin,histoIn.GetName()+'_GausSmear')
    histoOut.Reset("ICESM")
    binOut = histoOut.GetXaxis()

    for bi in range(1,binIn.GetNbins()+1):
        if bi % 100 == 0:
            print bi, binIn.GetNbins()
        emc = binIn.GetBinCenter(bi)
        res = resol.Eval(emc)
        constres = 0.7071067811865474 / res
        erflow = math.erf(constres * ( elow - emc ) )
        weight = histoIn.GetBinContent(bi)
        for bo in range(1,binOut.GetNbins()+1):
            erfup = math.erf(constres * (binOut.GetBinUpEdge(bo) - emc) )
            histoOut.SetBinContent(bo,histoOut.GetBinContent(bo) + (erfup - erflow) * weight)
            erflow = erfup
    
    histoOut.Scale(0.5)
    
    return histoOut

## Function to smear energy of 2D histogram
## Call SmearHisto1D, see corresponding comments of speed, histogram bins and range
## This function is slow, consider large bins in both dimensions
def SmearHisto2D(histoIn, resol, rebin = 1):
    ## histoIn = 2D histogram with unsmeared energy, preferably fine bins (1 keV), in x-axis and addition variable in y-axis
    ## resol = resolution function (TF1) as a function of energy
    ## rebin = energy rebinning of output (smeared) 1D histogram
    ## Rebinning of the variable in y-axis should happen before and outside this function call
    
    binInY = histoIn.GetYaxis()

    histoOut = histoIn.Rebin2D(rebin,1,histoIn.GetName()+'_GausSmear')
    histoOut.Reset("ICESM")

    for by in range(1,binInY.GetNbins()+1):
        print by, binInY.GetNbins()
        histoIn1D = histoIn.ProjectionX(histoIn.GetName()+'_1d', by, by)
        histoOut1D = SmearHisto1D(histoIn1D, resol, rebin)
        binOutX = histoOut1D.GetXaxis()

        for bx in range(1,binOutX.GetNbins()+1):
            histoOut.SetBinContent(bx,by,histoOut1D.GetBinContent(bx))

    return histoOut  
