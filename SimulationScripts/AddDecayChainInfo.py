
import ROOT
import math, sys, os, copy
from array import array
from Card import Card
import SimUtils
from optparse import OptionParser
import time

#fullPath = os.path.abspath(__file__)
#fullDir = fullPath[:fullPath.rfind('/')]
#ROOT.gSystem.Load('%s/AddDecayChainInfoFast_C.so'%(fullDir))

# global variables
globalArgs = {}

# merge tree variables
decayVars = { \
'ParentZ': (array('i',[0]),'ParentZ/I'), \
'ParentA': (array('i',[0]),'ParentA/I'), \
'NuclideZ': (array('i',[0]),'NuclideZ/I'), \
'NuclideA': (array('i',[0]),'NuclideA/I'), \
'BranchFraction': (array('d',[0]),'BranchFraction/D'), \
'Weight': (array('d',[0]),'Weight/D'), \
}

# order of variables in merged tree
decayVarOrder = ['ParentZ','ParentA','NuclideZ','NuclideA','BranchFraction','Weight']

def ClearMergeVars():

    global decayVars

    decayVars['ParentZ'][0][0] = 0
    decayVars['ParentA'][0][0] = 0
    decayVars['NuclideZ'][0][0] = 0
    decayVars['NuclideA'][0][0] = 0
    decayVars['BranchFraction'][0][0] = 0.
    decayVars['Weight'][0][0] = 0.


def CatchGlobalArguments(options):

    global globalArgs

    globalArgs['CardName'] = options.cardName #str(sys.argv[1])
    globalArgs['Process'] = options.processName 
    globalArgs['OutputDir'] = options.outDir 
    globalArgs['SubmitTogether'] = int(options.submitTogether)
    globalArgs['TimeBetween'] = int(options.timeBetween)

    globalArgs['Card'] = Card(globalArgs['CardName'])

def GetChainsForLocation(location):

    card = globalArgs['Card']

    processName = globalArgs['Process']
    processTitle = processName.title()

    filesInfo = SimUtils.GetFilesInfo(card,['g4',processName])

    chainList = {}
    for fileInfo in filesInfo:
        if fileInfo['ShortLocationName'] != location:
            continue

        isotope = fileInfo['Isotope']
        if not isotope in chainList:
            chainList[isotope] = {}
            chainList[isotope]['chain'] = ROOT.TChain(card.Get('%sTreeName'%(processTitle)))
            chainList[isotope]['fraction'] = fileInfo['BranchRatio']
        chain = chainList[isotope]['chain']
        if not 'nevents' in chainList[isotope]:
            chainList[isotope]['nevents'] = 0

        nMCevents = 0

        fileName = fileInfo['%sFileName'%(processTitle)]
        if os.path.isfile(fileName):# and os.path.isfile(fileInfo['G4FileName']):
            rootFile = ROOT.TFile.Open(fileName)
            nMCevents = rootFile.Get('NumberOfEvents_Run0')
            if nMCevents: 
                chainList[isotope]['nevents'] += nMCevents.GetVal()
            rootFile.Close()
        elif os.path.isfile(fileInfo['G4FileName']):
            rootFile = ROOT.TFile.Open(fileInfo['G4FileName'])
            nMCevents = rootFile.Get('NumberOfEvents_Run0')
            if nMCevents: 
                chainList[isotope]['nevents'] += nMCevents.GetVal()
            rootFile.Close()
                
        chain.Add(fileName)

    return chainList
    
def AddBranchesToFile(inFileName,outFileName,inTreeName,outTreeName,parentZ,parentA,nuclideZ,nuclideA,branchRatio,weight):

    cmd = 'cp %s %s' % (inFileName, outFileName)
    #print cmd
    os.system(cmd)

    rootFile = ROOT.TFile.Open(outFileName,'update')
    if not rootFile or rootFile.IsZombie():
        return 
    tree = rootFile.Get(inTreeName)
    tree.SetName(outTreeName)
    
    ClearMergeVars()
    
    decayVars['ParentZ'][0][0] = parentZ
    decayVars['ParentA'][0][0] = parentA
    decayVars['NuclideZ'][0][0] = nuclideZ
    decayVars['NuclideA'][0][0] = nuclideA
    decayVars['BranchFraction'][0][0] = branchRatio
    decayVars['Weight'][0][0] = weight
    #print decayVars['ParentZ'][0][0], decayVars['ParentA'][0][0], decayVars['NuclideZ'][0][0], decayVars['NuclideA'][0][0], decayVars['BranchFraction'][0][0], decayVars['Weight'][0][0]
        
    branches = []
    for var in decayVarOrder:
        #print 'Adding branch', decayVars[var][1], 'to tree in', fileName
        branches.append(tree.Branch(var, decayVars[var][0], decayVars[var][1]))
        #branches[-1].Fill()
    for i in range(tree.GetEntries()):
        tree.GetEntry(i)
        for branch in branches:
            branch.Fill()

    rootFile.cd()
    tree.Write()
    rootFile.Close()


def AddDecayInfoBranches(location,chainList):

    card = globalArgs['Card']

    parent = card.Get('ParentName')
    parentZ = SimUtils.GetAtomicNumber(parent)
    parentA = SimUtils.GetAtomicMass(parent)

    minTotal = []
    for isotope in chainList:
        if 'nevents' in chainList[isotope]:
            minTotal.append(chainList[isotope]['nevents'])
        else:
            minTotal.append(chainList[isotope]['chain'].GetEntries())
    minTotal = min(minTotal)

    filesInfoList = SimUtils.GetFilesInfo(card,['jobs',globalArgs['Process']])
    fileInfo = filesInfoList[0]

    jobDir = '/'.join(fileInfo['JobFileName'].split('/')[:-1])
    for isotope in chainList:
        
        ratio = chainList[isotope]['fraction']
        chain = chainList[isotope]['chain']
        if 'nevents' in chainList[isotope] and chainList[isotope]['nevents'] > 0:
            total = chainList[isotope]['nevents']
        else:
            total = chain.GetEntries()

        fileList = chain.GetListOfFiles()
        nFiles = fileList.GetEntries()
        print isotope, ratio, total, nFiles

        parentZ,parentA,nuclideZ,nuclideA,branchRatio,weight = parentZ,parentA,SimUtils.GetAtomicNumber(isotope),SimUtils.GetAtomicMass(isotope),ratio,minTotal*1./total * ratio

        next = ROOT.TIter(fileList)
        
        submitJobs = False
        if globalArgs['SubmitTogether'] > 0: 
            submitJobs = True
            includedJobs = 0

        for tn in range(nFiles):
            fileName = next().GetTitle()

            treeName = card.Get('%sTreeName'%(globalArgs['Process'].title()))
            treeDecayName = treeName+'Decay'
    
            outFileName = fileName.replace('.%s.root'%(treeName),'.%s.root'%(treeDecayName))
            if globalArgs['OutputDir']:
                outFileName = globalArgs['OutputDir'] + outFileName[outFileName.rfind('/'):]

            #ROOT.AddDecayChainInfoFast(outFileName,treeName,treeDecayName,parentZ,parentA,SimUtils.GetAtomicNumber(isotope),SimUtils.GetAtomicMass(isotope),ratio,minTotal*1./total * ratio)
            if not submitJobs:
                if tn % 10 == 0: print tn, 'of', nFiles
                AddBranchesToFile(fileName,outFileName,treeName,treeDecayName,parentZ,parentA,nuclideZ,nuclideA,branchRatio,weight)
            else:
                if includedJobs == 0:
                    #jobName = '%s/%s_%s_%s_%s_AddDecayInfo%s.sh'%(card.Get('JobsFullOutputPath'),card.Get('JobsOutputNamePrefix'),card.Get('ParentName'),isotope,location,tn)
                    jobName = '%s/%s_%s_%s_%s_AddDecayInfo%s.sh'%(jobDir,card.Get('JobsOutputNamePrefix'),card.Get('ParentName'),isotope,location,tn)
                    jobFile = open(jobName,'w')
                    if card.Has('JobHeader'):
                        jobFile.write(card.Get('JobHeader'))
                    jobFile.write('source %s \n\n'%(card.Get('SetupFile')))

                    fullPath = os.path.abspath(__file__)
                    fullDir = fullPath[:fullPath.rfind('/')]

                    jobFile.write('cd %s \n\n'%(fullDir))

                fullModule = os.path.abspath(__file__)
                module = fullModule[fullModule.rfind('/')+1:fullModule.rfind('.py')]
                jobFile.write("""python -c "import %s; %s.AddBranchesToFile('%s','%s','%s','%s',%d,%d,%d,%d,%f,%f) " \n""" % (module,module,os.path.realpath(fileName),os.path.realpath(outFileName),treeName,treeDecayName,parentZ,parentA,nuclideZ,nuclideA,branchRatio,weight))                   
                includedJobs += 1
                    
                if includedJobs == globalArgs['SubmitTogether'] or tn == nFiles-1:
                    jobFile.close()
                    includedJobs = 0
                    cmd = "%s %s" % (card.Get('BatchFarmCall'),jobName)
                    print cmd
                    os.system(cmd)
                    time.sleep(globalArgs['TimeBetween'])

                
def LoopAllLocations():

    card = globalArgs['Card']

    for pos in card.Get('LocationList').split(','):
        location = pos[pos.rfind('/')+1:]
        chainList = GetChainsForLocation(location)
        AddDecayInfoBranches(location,chainList)
    
               
if __name__ == "__main__":

    usage = "usage: python AddDecayChainInfo.py -c CardName -p ProcessToAddDecayInfo"
    parser = OptionParser(usage)
    
    parser.add_option("-c","--card", dest="cardName", nargs=1)
    parser.add_option("-p","--process", dest="processName", nargs=1)
    parser.add_option("-d","--output-dir", dest="outDir", nargs=1)
    parser.add_option("-s","--submit-together", dest="submitTogether", default=0, nargs=1)
    parser.add_option("-t","--time-between", dest="timeBetween", nargs=1, default=0)

    options,args = parser.parse_args()   
    print 'Using options:', options

    if not (options.cardName and options.processName):
        print 'Missing options...', usage
        sys.exit()

    CatchGlobalArguments(options)

    LoopAllLocations()
