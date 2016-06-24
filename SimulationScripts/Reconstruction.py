
import ROOT
import math, sys, os
from array import array
from Card import Card
import SimUtils
from optparse import OptionParser

# global variables
globalArgs = {}

# recon tree variables
reconVars = { \
'TotalReconEnergy': (array('d',[0]),'TotalReconEnergy/D'), \
'SmearedEnergy': (array('d',[0]),'SmearedEnergy/D'), \
'StandoffDistance': (array('d',[0]),'StandoffDistance/D'), \
'NumReconClusters': (array('i',[0]),'NumReconClusters/I'), \
'ReconFlag': (array('i',[0]),'ReconFlag/I'), \
}

#recon tree branches
reconBranches = []

# order of variables in recon tree
reconVarOrder = ['TotalReconEnergy', 'SmearedEnergy', 'StandoffDistance', 'NumReconClusters', 'ReconFlag']

def CatchGlobalArguments(options):

    global globalArgs

    globalArgs['InputRootFileName'] = options.inFileName #str(sys.argv[1])
    globalArgs['OutputRootFileName'] = options.outFileName #str(sys.argv[2])
    globalArgs['CardName'] = options.cardName #str(sys.argv[3])
    globalArgs['Seed'] = int(options.seedNumber) #str(sys.argv[4])
    globalArgs['Card'] = Card(globalArgs['CardName'])

    card = globalArgs['Card']
    globalArgs['ResolSS'] = ROOT.TF1('resol_ss',card.Get('ResolSS'),0,100000)
    globalArgs['ResolSS'].SetParameters(array('d',[float(i) for i in card.Get('ResolPss').split(',')]))
    globalArgs['ResolMS'] = ROOT.TF1('resol_ms',card.Get('ResolMS'),0,100000)
    globalArgs['ResolMS'].SetParameters(array('d',[float(i) for i in card.Get('ResolPms').split(',')]))
    globalArgs['ResolScale'] = float(card.Get('ResolScale'))
    
    globalArgs['Random'] = ROOT.TRandom3(int(globalArgs['Seed']))

def GetNentries(n,card):

    if card.Has('ReconstructNevents'):
        optN = card.Get('ReconstructNevents')
        if optN.lower() == 'all':
            optN = n
        else:
            optN = int(optN)
        n = min(n,optN)
    
    return n        

def ReadInputTree(treeName):

    t = ROOT.TChain(treeName)
    print 'Adding cluster file', globalArgs['InputRootFileName']
    t.Add(globalArgs['InputRootFileName'])

    return t

def CreateReconTree(outFile):

    global reconVars

    tree = outFile.Get(card.Get('ClusterTreeName'))
    tree.SetName(card.Get('ReconTreeName'))

    for var in reconVarOrder:
        print 'Adding branch', var, reconVars[var][0], reconVars[var][1]
        reconBranches.append(tree.Branch(var, reconVars[var][0], reconVars[var][1]))
    
    return tree

def MakeOutputFile(card):

    fileName = globalArgs['InputRootFileName']
    outFileName = globalArgs['OutputRootFileName']

    cmd = 'cp %s %s' % (fileName, outFileName)
    print cmd
    os.system(cmd)

    rootFile = ROOT.TFile.Open(outFileName,'update')
    if not rootFile or rootFile.IsZombie():
        return False

    return rootFile

def ClearReconVars():

    global reconVars

    reconVars['TotalReconEnergy'][0][0] = 0.
    reconVars['SmearedEnergy'][0][0] = 0.
    reconVars['StandoffDistance'][0][0] = 0.
    reconVars['NumReconClusters'][0][0] = 0
    reconVars['ReconFlag'][0][0] = 0


def InFV(x,y,z):

    card = globalArgs['Card']
    lXeCenterX, lXeCenterY, lXeCenterZ = float(card.Get('LXeCenterX')), float(card.Get('LXeCenterY')), float(card.Get('LXeCenterZ'))

    x -= lXeCenterX
    y -= lXeCenterY
    z -= lXeCenterZ

    if not bool(int(card.Get('UseFV'))):
        return True

    radial1, radial2, axial = SimUtils.GetRadialAndAxialCoordinates(x,y,z, card)

    if (radial1**2 + radial2**2)**0.5 < float(card.Get('FidVolRadius')) and float(card.Get('FidVolMinAxial')) < axial and axial < float(card.Get('FidVolMaxAxial')):
        return True

    return False

def GetStandoffDistance(x,y,z):

    card = globalArgs['Card']
    lXeCenterX, lXeCenterY, lXeCenterZ = float(card.Get('LXeCenterX')), float(card.Get('LXeCenterY')), float(card.Get('LXeCenterZ'))
    x -= lXeCenterX
    y -= lXeCenterY
    z -= lXeCenterZ   

    radial1, radial2, axial = SimUtils.GetRadialAndAxialCoordinates(x,y,z, card)
    rad = (radial1**2 + radial2**2)**0.5

    #lXeRadius, lXeHalfLength = float(card.Get('LXeRadius')), float(card.Get('LXeHalfLength'))
    #sda = lXeHalfLength - abs(axial)
    #sdr = lXeRadius - rad
    
    lXeSDdim = False
    if card.Has('UseLXeSD'):
        lXeSDdim = bool(int(card.Get('UseLXeSD')))
        
    if lXeSDdim:
        lXeRadius, lXeMinAx, lXeMaxAx = float(card.Get('LXeSDRadius')), float(card.Get('LXeSDMinAxial')), float(card.Get('LXeSDMaxAxial'))
    else:
        lXeRadius, lXeMinAx, lXeMaxAx = float(card.Get('ActiveLXeRadius')), float(card.Get('ActiveLXeMinAxial')), float(card.Get('ActiveLXeMaxAxial'))

    sdr = abs(lXeRadius - rad)
    sda = min(abs(lXeMinAx - axial),abs(lXeMaxAx - axial))

    return min(sda,sdr)

   
def ApplyReconstruction(tree):

    global reconVars
    card = globalArgs['Card']
    oldCode = True
    if card.Has('UseOldAlgorithm'):
        oldCode = bool(int(card.Get('UseOldAlgorithm')))
    print 'Using old algorithm...', oldCode
    lXeSDdim = False
    if card.Has('UseLXeSD'):
        lXeSDdim = bool(int(card.Get('UseLXeSD')))
    print 'Using LXe SD...', lXeSDdim

    n = GetNentries(tree.GetEntries(),card)
    print 'Reconstructing n events =', n

    for i in range(n):
        if i%1000 == 0:
            print 'recon i = ', i
        tree.GetEntry(i)

        ClearReconVars()

        for j in range(tree.NumClusters):
            x, y, z = tree.Xpos[j], tree.Ypos[j], tree.Zpos[j]
            energy = tree.EnergyDeposit[j]
            
            if energy*1000 < float(card.Get('Uthreshold')):
                continue

            if not InFV(x,y,z):
                reconVars['ReconFlag'][0][0] = -1
                break

            if energy*1000 < float(card.Get('Vthreshold')):
                if reconVars['ReconFlag'][0][0] <= 0 or oldCode:
                    reconVars['ReconFlag'][0][0] = 1
                if oldCode:
                    break
                continue

            reconVars['ReconFlag'][0][0] = 2
            reconVars['NumReconClusters'][0][0] += 1
            reconVars['TotalReconEnergy'][0][0] += energy

            sd = GetStandoffDistance(x,y,z)
            if reconVars['StandoffDistance'][0][0] == 0 or sd < reconVars['StandoffDistance'][0][0]:
                reconVars['StandoffDistance'][0][0] = sd


        multInt = reconVars['NumReconClusters'][0][0]
        if multInt > 0 :
            multStr = ('SS','MS')[multInt > 1]
            reconEnergy = reconVars['TotalReconEnergy'][0][0]
            sigma = globalArgs['ResolScale']*globalArgs['Resol%s'%(multStr)].Eval(reconEnergy*1000.)/1000.
            reconVars['SmearedEnergy'][0][0] = reconEnergy + sigma*globalArgs['Random'].Gaus(0,1)

        for reconBranch in reconBranches:
            reconBranch.Fill()


if __name__ == "__main__":

    usage = "usage: python Reconstruction.py -i InputFileName -o OutputFileName -c CardName -s Seed"
    parser = OptionParser(usage)
    
    parser.add_option("-c","--card", dest="cardName", nargs=1)
    parser.add_option("-i","--input", dest="inFileName", nargs=1)
    parser.add_option("-o","--output", dest="outFileName", nargs=1)
    parser.add_option("-s","--seed", dest="seedNumber", nargs=1)

    options,args = parser.parse_args()   
    print 'Using options:', options

    if not (options.cardName and options.inFileName and options.outFileName and options.seedNumber):
        print 'Missing options...', usage
        sys.exit()

    CatchGlobalArguments(options)
    card = globalArgs['Card']

    OutputRootFile = MakeOutputFile(card)
    ReconTree = CreateReconTree(OutputRootFile)
    ApplyReconstruction(ReconTree)

    print 'Saving reconstructed tree to file:', OutputRootFile.GetName()
    OutputRootFile.cd()
    ReconTree.Write()

    OutputRootFile.Close()

