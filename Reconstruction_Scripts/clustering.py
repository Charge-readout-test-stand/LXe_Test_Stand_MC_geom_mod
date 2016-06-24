
import ROOT
import math, sys
from array import array

globalArgs = {'InputRootFileName':'', 'OutputRootFileName':'', 'IsotopeList':[], 'ClusterDiam':0., 'HoldingDir':'', 'Region':'', 'JobIndex':0}

clusterVars = { \
'EventNumber': (array('i',[0]),'EventNumber/I'), \
'TotalEventEnergy': (array('d',[0]),'TotalEventEnergy/D'), \
'NumClusters': (array('i',[0]),'NumClusters/I'), \
'EnergyDeposit': (array('d',[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]),'EnergyDeposit[40]/D'), \
'Xpos': (array('d',[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]),'Xpos[40]/D'), \
'Ypos': (array('d',[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]),'Ypos[40]/D'), \
'Zpos': (array('d',[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]),'Zpos[40]/D'), \
}

clusterVarOrder = ['EventNumber', 'TotalEventEnergy', 'NumClusters', 'EnergyDeposit', 'Xpos', 'Ypos', 'Zpos']

def CatchArguments(args):

    global globalArgs

    globalArgs['InputRootFileName'] = str(sys.argv[1])
    globalArgs['OutputRootFileName'] = str(sys.argv[2])
    globalArgs['IsotopeList'] = sys.argv[3].split()
    globalArgs['ClusterDiam'] = float(sys.argv[4])
    globalArgs['HoldingDir'] = str(sys.argv[5])
    globalArgs['Region'] = str(sys.argv[6])
    globalArgs['JobIndex'] = int(sys.argv[7])

    globalArgs['UseFV'] = False

    if len(args) > 8:
        fv = sys.argv[8]
        if fv == 'FV off':
            globalArgs['UseFV'] = False
        else:
            fv = sys.argv[8].split()
            globalArgs['FidVolRadius'] = float(fv[0])
            globalArgs['FidVolMinZ'] = float(fv[1])
            globalArgs['FidVolMaxZ'] = float(fv[2])
            globalArgs['UseFV'] = True

    globalArgs['FullOutputRootFileName'] = '%s/%s_%s_%d.root' % (globalArgs['HoldingDir'], globalArgs['OutputRootFileName'], globalArgs['Region'], globalArgs['JobIndex'])
    globalArgs['FullInputRootFileName'] = '%s/%s_%s_%s_%d.root' % (globalArgs['HoldingDir'], globalArgs['InputRootFileName'], '%s',globalArgs['Region'], globalArgs['JobIndex'])

def CreateClusterTree(treeName = 'clusterTree', treeTitle = 'Tree of Clustered Energy Deposits in LXe'):

    global clusterVars

    clusterTree = ROOT.TTree(treeName,treeTitle)
    for var in clusterVarOrder:
        print 'Adding branch', clusterVars[var][1]
        clusterTree.Branch(var, clusterVars[var][0], clusterVars[var][1])
    
    return clusterTree

def ReadInputTree(treeName = 'tree'):

    t = ROOT.TChain(treeName)
    for isotope in globalArgs['IsotopeList']:
        print 'Adding MC file', globalArgs['FullInputRootFileName']%(isotope)
        t.Add(globalArgs['FullInputRootFileName']%(isotope))
    return t

def InFV(x,y,z):

    if not globalArgs['UseFV']:
        return True

    if (x*x + y*y)**0.5 < globalArgs['FidVolRadius'] and globalArgs['FidVolMinZ'] < z and z < globalArgs['FidVolMaxZ']:
        return True

    return False

def PerformClusteringOnList(Deps, numClust, clusterTree):
    while len(Deps):
        x1 = Deps[0][0]
        y1 = Deps[0][1]
        z1 = Deps[0][2]
        pic = 0

        for element in range(1,len(Deps)):
            x2 = Deps[element][0]
            y2 = Deps[element][1]
            z2 = Deps[element][2]
            d = math.sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)+(z1-z2)*(z1-z2))
            if d < globalArgs['ClusterDiam']:
                pic = 1
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

        if not pic:
            PopArray = Deps.pop(0)
            clusterVars['Xpos'][0][numClust] = PopArray[0]
            clusterVars['Ypos'][0][numClust] = PopArray[1]
            clusterVars['Zpos'][0][numClust] = PopArray[2]
            clusterVars['EnergyDeposit'][0][numClust] = PopArray[3]
            numClust += 1
            clusterVars['NumClusters'][0][0] = numClust

    clusterTree.Fill()
    return

def ApplyClustering(t,clusterTree):

    n = t.GetEntries()
    print 'nfinal = ', n

    for i in range(0, n):
        if i%100 == 0:
            print 'i = ', i
        t.GetEntry(i)
        arrayNum = t.NumDeposits

        clusterVars['EventNumber'][0][0] = 0
        clusterVars['TotalEventEnergy'][0][0] = 0.
        clusterVars['NumClusters'][0][0] = 0
        for j in range(0,40):
            clusterVars['EnergyDeposit'][0][j] = 0.
            clusterVars['Xpos'][0][j] = 0.
            clusterVars['Ypos'][0][j] = 0.
            clusterVars['Zpos'][0][j] = 0.


        clusterVars['EventNumber'][0][0] = t.EventNumber
        numClust = 0

        Deps = []
        
        for j in range(0,arrayNum):
            if not globalArgs['UseFV'] or InFV(t.Xpos[j], t.Ypos[j], t.Zpos[j]):
                clusterVars['TotalEventEnergy'][0][0] += t.EnergyDeposit[j]
                Dep = [t.Xpos[j], t.Ypos[j], t.Zpos[j], t.EnergyDeposit[j]]
                Deps.append(Dep)

        PerformClusteringOnList(Deps, 0, clusterTree)

if __name__ == "__main__":

    CatchArguments(sys.argv)

    OutputRootFile = ROOT.TFile(globalArgs['FullOutputRootFileName'], 'RECREATE')
    ClusterTree = CreateClusterTree()

    InputTree = ReadInputTree()

    ApplyClustering(InputTree,ClusterTree)

    print 'Saving clustered tree to file:', OutputRootFile.GetName()
    OutputRootFile.cd()
    ClusterTree.Write()
    OutputRootFile.Close()


