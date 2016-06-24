
import ROOT
import math, sys, os
from array import array
from Card import Card
import SimUtils
from optparse import OptionParser

# global variables
globalArgs = {}

# cluster tree variables
clusterVars = { \
'EventNumber': (array('i',[0]),'EventNumber/I'), \
'EventVertexX': (array('d',[0]),'EventVertexX/D'), \
'EventVertexY': (array('d',[0]),'EventVertexY/D'), \
'EventVertexZ': (array('d',[0]),'EventVertexZ/D'), \
'TotalEventEnergy': (array('d',[0]),'TotalEventEnergy/D'), \
'NumClusters': (array('i',[0]),'NumClusters/I'), \
'EnergyDeposit': ('vector<float>', ROOT.std.vector(float)()), \
'Xpos': ('vector<float>', ROOT.std.vector(float)()), \
'Ypos': ('vector<float>', ROOT.std.vector(float)()), \
'Zpos': ('vector<float>', ROOT.std.vector(float)()), \
}

# order of variables in cluster tree
clusterVarOrder = ['EventNumber', 'EventVertexX', 'EventVertexY', 'EventVertexZ', 'TotalEventEnergy',  'NumClusters', 'EnergyDeposit', 'Xpos', 'Ypos', 'Zpos']

def CatchGlobalArguments(args):

    global globalArgs

    globalArgs['InputRootFileName'] = options.inFileName #str(sys.argv[1])
    globalArgs['OutputRootFileName'] = options.outFileName #str(sys.argv[2])
    globalArgs['CardName'] = options.cardName #str(sys.argv[3])
    globalArgs['Card'] = Card(globalArgs['CardName'])

    card = globalArgs['Card']

def MakeOutputFile(card):

    outFile = ROOT.TFile.Open(globalArgs['OutputRootFileName'],'recreate')

    inFile = ROOT.TFile.Open(globalArgs['InputRootFileName'],'read')
    inKeys = inFile.GetListOfKeys()
    inObjs = {}
    for k in range(inKeys.GetSize()):
        inKey = inKeys.At(k)
        inName = inKey.GetName()
        className = inKey.GetClassName()
        tClass = ROOT.gROOT.GetClass(className)
        #print className, tClass
        if not tClass:
            continue
        elif tClass.InheritsFrom(ROOT.TDirectory.Class()):
            continue
        elif inName == card.Get('G4TreeName'):
            continue
        else:
            if className == 'TTimeStamp':
                inTime = ROOT.TTimeStamp()
                inFile.GetObject(inName,inTime)
                inObjs[inTime] = {}
                inObjs[inTime]['name'] = inName
                inObjs[inTime]['time'] = True
                
            else:
                inFile.cd()
                inObj = inKey.ReadObj()
                inObjs[inObj] = {}
                inObjs[inObj]['name'] = inName
                inObjs[inObj]['time'] = False

    for inObj in inObjs:
        print 'Writing', inObj, 'into', outFile
        outFile.cd()
        if inObjs[inObj]['time']: ROOT.gDirectory.WriteObject(inObj,inObjs[inTime]['name'])
        else: inObj.Write()

    inFile.Close()

    return outFile

def CreateClusterTree(treeName, treeTitle = 'Tree of Clustered Energy Deposits in LXe'):

    global clusterVars

    clusterTree = ROOT.TTree(treeName,treeTitle)
    for var in clusterVarOrder:
        print 'Adding branch', clusterVars[var][1]
        clusterTree.Branch(var, clusterVars[var][0], clusterVars[var][1])
    
    return clusterTree

def ReadInputTree(treeName):

    t = ROOT.TChain(treeName)
    print 'Adding MC file', globalArgs['InputRootFileName']
    t.Add(globalArgs['InputRootFileName'])

    return t

def ClearClusterVars():

    global clusterVars

    clusterVars['EventNumber'][0][0] = 0
    clusterVars['EventVertexX'][0][0] = 0.
    clusterVars['EventVertexY'][0][0] = 0.
    clusterVars['EventVertexZ'][0][0] = 0.
    clusterVars['TotalEventEnergy'][0][0] = 0.
    clusterVars['NumClusters'][0][0] = 0
    clusterVars['EnergyDeposit'][1].clear()
    clusterVars['Xpos'][1].clear()
    clusterVars['Ypos'][1].clear()
    clusterVars['Zpos'][1].clear()

def GetNentries(n,card):

    if card.Has('ClusterNevents'):
        optN = card.Get('ClusterNevents')
        if optN.lower() == 'all': optN = n
        else: optN = int(optN)
        n = min(n,optN)
    
    return n        

def InActiveLXe(x,y,z):

    card = globalArgs['Card']
    lXeCenterX, lXeCenterY, lXeCenterZ = float(card.Get('LXeCenterX')), float(card.Get('LXeCenterY')), float(card.Get('LXeCenterZ'))

    x -= lXeCenterX
    y -= lXeCenterY
    z -= lXeCenterZ

    if not bool(int(card.Get('UseActiveLXe'))):
        return True

    radial1, radial2, axial = SimUtils.GetRadialAndAxialCoordinates(x,y,z, card)

    if (radial1**2 + radial2**2)**0.5 < float(card.Get('ActiveLXeRadius')) and float(card.Get('ActiveLXeMinAxial')) < axial and axial < float(card.Get('ActiveLXeMaxAxial')):
        return True

    return False

def ClusterDeposits(Deps):

    global clusterVars
    card = globalArgs['Card']

    while len(Deps):
        x1 = Deps[0][0]
        y1 = Deps[0][1]
        z1 = Deps[0][2]
        picked = False

        for element in range(1,len(Deps)):
            x2 = Deps[element][0]
            y2 = Deps[element][1]
            z2 = Deps[element][2]
            d = math.sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)+(z1-z2)*(z1-z2))
            if d < float(card.Get('ClusterDiameter')):
                picked = True
                e1 = Deps[0][3]
                e2 = Deps[element][3]
                eNew = e1 + e2
                xNew = (e1*x1 + e2*x2)/eNew
                yNew = (e1*y1 + e2*y2)/eNew
                zNew = (e1*z1 + e2*z2)/eNew
                NewDep = [xNew, yNew, zNew, eNew]

                Deps.pop(element)
                Deps.pop(0)
                Deps.insert(0,NewDep)
                break

        if not picked:
            PopArray = Deps.pop(0)
            clusterVars['Xpos'][1].push_back(PopArray[0])
            clusterVars['Ypos'][1].push_back(PopArray[1])
            clusterVars['Zpos'][1].push_back(PopArray[2])
            clusterVars['EnergyDeposit'][1].push_back(PopArray[3])
            clusterVars['TotalEventEnergy'][0][0] += PopArray[3]
            clusterVars['NumClusters'][0][0] += 1

    return           

def ApplyClustering(tree,clusterTree):

    global clusterVars
    card = globalArgs['Card']

    n = GetNentries(tree.GetEntries(),card)
    print 'Clustering n events =', n

    for i in range(0, n):
        if i%1000 == 0:
            print 'cluster i = ', i
        tree.GetEntry(i)

        ClearClusterVars()

        clusterVars['EventVertexX'][0][0] = tree.GenX
        clusterVars['EventVertexY'][0][0] = tree.GenY
        clusterVars['EventVertexZ'][0][0] = tree.GenZ
        clusterVars['EventNumber'][0][0] = tree.EventNumber

        Deps = []      
        for j in range(0,tree.NumDeposits):
            Dep = [tree.Xpos[j], tree.Ypos[j], tree.Zpos[j], tree.EnergyDeposit[j]]
            if InActiveLXe(Dep[0],Dep[1],Dep[2]):
                Deps.append(Dep)

        ClusterDeposits(Deps)

        clusterTree.Fill()

if __name__ == "__main__":

    usage = "usage: python Clustering.py -c CardName -i InputFileName -o OutputFileName"
    parser = OptionParser(usage)
    
    parser.add_option("-c","--card", dest="cardName", nargs=1)
    parser.add_option("-i","--input", dest="inFileName", nargs=1)
    parser.add_option("-o","--output", dest="outFileName", nargs=1)

    options,args = parser.parse_args()   
    print 'Using options:', options

    if not (options.cardName and options.inFileName and options.outFileName):
        print 'Missing options...', usage
        sys.exit()

    CatchGlobalArguments(options)
    card = globalArgs['Card']

    OutputRootFile = MakeOutputFile(card)

    ClusterTree = CreateClusterTree(card.Get('ClusterTreeName'))
    InputTree = ReadInputTree(card.Get('G4TreeName'))

    ApplyClustering(InputTree,ClusterTree)

    print 'Saving clustered tree to file:', OutputRootFile.GetName()
    OutputRootFile.cd()
    ClusterTree.Write()

    OutputRootFile.Close()


