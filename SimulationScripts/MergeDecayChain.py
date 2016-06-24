
import ROOT
import math, sys, os
from array import array
from Card import Card
import SubmitSimulation, SimUtils

# global variables
globalArgs = {}

# merge tree variables
mergeVars = { \
'ParentZ': (array('i',[0]),'ParentZ/I'), \
'ParentA': (array('i',[0]),'ParentA/I'), \
'NuclideZ': (array('i',[0]),'NuclideZ/I'), \
'NuclideA': (array('i',[0]),'NuclideA/I'), \
'BranchFraction': (array('d',[0]),'BranchFraction/D'), \
'Weight': (array('d',[0]),'Weight/D'), \
}

# order of variables in merged tree
mergeVarOrder = ['ParentZ','ParentA','NuclideZ','NuclideA','BranchFraction','Weight']

def ClearMergeVars():

    global mergeVars

    mergeVars['ParentZ'][0][0] = 0
    mergeVars['ParentA'][0][0] = 0
    mergeVars['NuclideZ'][0][0] = 0
    mergeVars['NuclideA'][0][0] = 0
    mergeVars['BranchFraction'][0][0] = 0.
    mergeVars['Weight'][0][0] = 0.


def CatchGlobalArguments(args):

    global globalArgs

    globalArgs['CardName'] = str(sys.argv[1])
    globalArgs['Card'] = Card(globalArgs['CardName'])

def GetExtensionFor(tree):
    return '.%s.root' % (tree)

def MergedTreesForLocation(location):

    card = globalArgs['Card']

    macroList = SubmitSimulation.CreateMacros(card,False)
    fileList = []
    for macro in macroList:
        macroName = macro[0]
        fileName = macroName.replace('.mac',GetExtensionFor(card.Get('TreeNameToMerge')))
        if os.path.isfile(fileName) and fileName.find(location) >= 0:
            fileList.append(fileName)

    resultList = {}
    for isotope, ratio in zip(card.Get('DecayChainList').split(','), card.Get('BranchRatioList').split(',')):
        outName = '%s/%s_%s_%s.%s.root'%(card.Get('FullOutputPath'),card.Get('MergedOutputNamePrefix'),card.Get('ParentName'),location,isotope)
        cmd = 'hadd -v 0 -f %s' % (outName)
        count = 0
        for fileName in fileList:
            if os.path.isfile(fileName) and fileName.find(isotope) >= 0:
                cmd += ' %s' % (fileName)
                count += 1
        os.system(cmd)
        resultList[outName] = {'isotope':isotope,'fraction': float(ratio)}
        print 'Merged tree created temporarily in file:', outName
        if count > 0:
            chain = ROOT.TChain(card.Get('TreeNameToMerge'))
            chain.Add(outName)
            resultList[outName]['events'] = chain.GetEntries()
        else:
            resultList[outName]['events'] = 0

    return resultList
    

def AddMergeBranches(fileList):

    card = globalArgs['Card']

    parent = card.Get('ParentName')
    parentZ = SimUtils.GetAtomicNumber(parent)
    parentA = SimUtils.GetAtomicMass(parent)

    minTotal = []
    for fileName in fileList:
        minTotal.append(fileList[fileName]['events'])
    minTotal = min(minTotal)

    for fileName in fileList:
        
        isotope = fileList[fileName]['isotope']
        ratio = fileList[fileName]['fraction']
        total = fileList[fileName]['events']

        print isotope, ratio, total, fileName

        rootFile = ROOT.TFile(fileName,'update')
        tree = rootFile.Get(card.Get('TreeNameToMerge'))
        
        branches = []
        for var in mergeVarOrder:
            print 'Adding branch', mergeVars[var][1], 'to tree in', fileName
            branches.append(tree.Branch(var, mergeVars[var][0], mergeVars[var][1]))
            
        ClearMergeVars()

        mergeVars['ParentZ'][0][0] = parentZ
        mergeVars['ParentA'][0][0] = parentA
        mergeVars['NuclideZ'][0][0] = SimUtils.GetAtomicNumber(isotope)
        mergeVars['NuclideA'][0][0] = SimUtils.GetAtomicMass(isotope)
        mergeVars['BranchFraction'][0][0] = ratio
        mergeVars['Weight'][0][0] = minTotal*1./total * ratio
        print mergeVars['ParentZ'][0][0], mergeVars['ParentA'][0][0], mergeVars['NuclideZ'][0][0], mergeVars['NuclideA'][0][0], mergeVars['BranchFraction'][0][0], mergeVars['Weight'][0][0]
        
        for i in range(total):
            tree.GetEntry(i)

        for branch in branches:
            branch.Fill()

        rootFile.cd()
        tree.SetName('mergedTree')
        tree.Write()
        ROOT.gDirectory.Delete('%s;*'%(card.Get('TreeNameToMerge')))
        rootFile.Close()
        
def MergeDecayChainTree(fileList):

    cmdRm = 'rm -rf'
    cmdHadd = 'hadd -O -v 0 -f %s' 
    for fileName in fileList:
        isotope = fileList[fileName]['isotope']
        mergedName = fileName.replace('.%s.root'%(isotope),'.root')
        cmdRm += ' %s' % (fileName)
        cmdHadd += ' %s ' % (fileName)

    cmdHadd = cmdHadd % (mergedName)
    print cmdHadd
    os.system(cmdHadd)
    print cmdRm       
    #os.system(cmdRm)

    print 'Merged written to:', mergedName
                
def MergeAllLocations():

    card = globalArgs['Card']

    for pos in card.Get('LocationList').split(','):
        location = pos[pos.rfind('/')+1:]
        fileList = MergedTreesForLocation(location)
        AddMergeBranches(fileList)
        MergeDecayChainTree(fileList)
    
               
if __name__ == "__main__":

    CatchGlobalArguments(sys.argv)

    MergeAllLocations()
