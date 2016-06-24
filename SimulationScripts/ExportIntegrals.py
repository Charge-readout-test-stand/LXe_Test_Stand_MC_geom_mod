import sys
import os
from optparse import OptionParser
import tempfile
import zipfile

import ROOT
import shutil

from Card import Card
import SimUtils

globalArgs = {}

class SimplePlot:
    def __init__(self, name, expression, cut, option = '', destination = 'histo', specifications = '', entrylist = None):
        self.name = name
        self.expression = expression
        self.cut = cut
        self.weight = 'Weight'
        self.option = '%s goff' % option
        self.destination = destination
        self.specifications = specifications
        self.entrylist = entrylist

def CatchGlobalArguments(options):

    global globalArgs

    globalArgs['CardName'] = options.cardName #str(sys.argv[1])
    globalArgs['InputDir'] = options.inDir
    globalArgs['OutputFileName'] = options.outFileName
    globalArgs['AllIsotopes'] = bool(options.allIsotopes)
    globalArgs['PrintInfo'] = options.printInfo
    globalArgs['ExcelTemplate'] = options.excelTemplate

    globalArgs['Card'] = Card(globalArgs['CardName'])

def GetChainsForLocation(location):

    card = globalArgs['Card']

    processName = 'recon'#globalArgs['Process']
    processTitle = processName.title()

    treeName = card.Get('%sTreeName'%(processTitle))
    treeDecayName = treeName+'Decay'

    filesInfo = SimUtils.GetFilesInfo(card,[processName])

    decayChainName = card.Get('ParentName')+'DecayChain'

    chainList = {}
    chainList[decayChainName] = ROOT.TChain(treeDecayName)
    for fileInfo in filesInfo:
        if fileInfo['ShortLocationName'] != location:
            continue

        isotope = fileInfo['Isotope']
        if globalArgs['AllIsotopes']:
            if not isotope in chainList:
                chainList[isotope] = {}
                chainList[isotope] = ROOT.TChain(treeDecayName)
            chain = chainList[isotope]

        fileName = fileInfo['%sFileName'%(processTitle)]
        fileName = fileName.replace(treeName,treeDecayName)
        if globalArgs['InputDir']:
            fileName = globalArgs['InputDir'] + fileName[fileName.rfind('/'):]
        
        if os.path.isfile(fileName):
            if globalArgs['AllIsotopes']: chain.Add(fileName)
            chainList[decayChainName].Add(fileName)

    return chainList

def GetPlots():

    energyCuts = {}
    energyCuts['1sigma'] = [2.433, 2.483]
    energyCuts['2sigma'] = [2.408, 2.507]
    energyCuts['3sigma'] = [2.384, 2.532]
    energyCuts['FWHM'] = [2.428, 2.488]
    energyCuts['> 700 keV'] = [0.7, 3.5]
    energyCuts['> 0'] = [0., 3.5]

    sdCuts = {}
    sdCuts['000;036;779.96'] = [0,36]
    sdCuts['036;090;1000'] = [36,90]
    sdCuts['090;122;500'] = [90,122]
    sdCuts['122;159;500'] = [122,159]
    sdCuts['159;202;500'] = [159,202]
    sdCuts['202;256;500'] = [202,256]
    sdCuts['256;333;500'] = [256,333]
    sdCuts['333;631;500'] = [333,631]
    sdCuts['Total;;4779.96'] = [0,631]

    plots = []

    for energyCut in energyCuts:
        energyCutStr = '%.3f < SmearedEnergy && SmearedEnergy < %.3f' % (energyCuts[energyCut][0],energyCuts[energyCut][1])
        for sdCut in sdCuts:
            sdCutStr = '%d < StandoffDistance && StandoffDistance < %d' % (sdCuts[sdCut][0],sdCuts[sdCut][1])
            cutStr  = 'ReconFlag == 2 && %s && %s' % (energyCutStr,sdCutStr)
            cutStrSS = cutStr + ' && NumReconClusters == 1'
            plots.append(SimplePlot(name = 'SS;%s;%s' %(energyCut,sdCut) , expression = 'SmearedEnergy', specifications = '(500,0,5)', cut = cutStrSS))
            cutStrMS = cutStr + ' && NumReconClusters > 1'
            plots.append(SimplePlot(name = 'MS;%s;%s' %(energyCut,sdCut) , expression = 'SmearedEnergy', specifications = '(500,0,5)', cut = cutStrMS))


    return plots

def PerformCuts(location, chainList, plots):

    cutList = []
    for plot in plots:
        if plot.cut != '':
            cutList.append(plot.cut)
    cutSet = set(cutList)
    cutDict = {}
    print 'Selecting events...'
    for c,cut in enumerate(sorted(cutSet)):
        print 'cut', c, 'of', len(cutSet), ':', cut
        cutDict[cut] = {}
        for i,isotope in enumerate(chainList):
            chain = chainList[isotope]
            listName = 'elist_%d_%d' % (c,i)
            #print 'Selecting events for %s in %s with cut "%s" ...'%(isotope, location, cut)
            chain.Draw('>>%s'%(listName),cut,'entrylist')
            cutDict[cut][isotope] = ROOT.gDirectory.Get(listName)
            #print '... selected', cutDict[cut][isotope].GetN(), 'events!'

    for plot in plots:
        if plot.cut in cutDict:
            plot.entrylist = cutDict[plot.cut]


def ExportToExcel(location, integrals, relerrs):
    '''
    Exports results into the excel template given as input

    NB: Some of the code here depends on the details of the template
    and might have to change if the template is modified
    '''

    if not globalArgs['ExcelTemplate']:
        return -1

    # Lazy import so that one can use the code w/o excel exporting feature
    # w/o having to install these modules
    import lxml         # these is not used directly but needed by BeautifulSoup to parse xml
    from bs4 import BeautifulSoup as bs

    #
    xls_template = globalArgs['ExcelTemplate']
    xls_out = os.path.splitext(xls_template)[-2] + "-" + location + os.path.splitext(xls_template)[-1]

    # if the output file name already exists for the given location,
    # it is assumed to be the one that needs to be updated. Makes a backup just in case
    if os.path.isfile(xls_out):
        xls_template = xls_out
        shutil.copy(xls_out, xls_out.replace(".xlsx", ".backup.xlsx"))

    # generate a temp file
    tmpfd, tmpname = tempfile.mkstemp(dir=os.path.dirname(xls_template))
    os.close(tmpfd)

    # create a temp copy of the archive without 'sheet1.xml'
    # and load 'sheet1.xml' in memory
    with zipfile.ZipFile(xls_template, 'r') as zin:
        with zipfile.ZipFile(tmpname, 'w') as zout:
            for item in zin.infolist():
                if item.filename == "xl/worksheets/sheet1.xml":
                    # read 'sheet1.xml'
                    sheet1 = bs(zin.read(item.filename), 'xml')
                elif item.filename == "xl/workbook.xml":
                    workbook = bs(zin.read(item.filename), 'xml')
                else:
                    # rewrite everything else as is
                    zout.writestr(item, zin.read(item.filename))

    # replace the temp file name with the output name
    os.rename(tmpname, xls_out)

    # First update the workbook.xml in memory
    cal = workbook.find("calcPr")
    # remove any existing attributes
    attribs =  [a for a in cal.attrs]
    for attr in attribs:
        del cal[attr]
    # force recalculation of all formulas upon load
    cal['fullCalcOnLoad'] = 1

    # Now update the sheet1.xml in memory

    # Positions are relative to here
    # if templates change, this might have to change
    row_index = 8
    col_index = 6

    # column depends on the decay chain
    # if templates change, this might have to change
    card = globalArgs['Card']
    decayChainName = card.Get('ParentName')
    if decayChainName == "Th232":
        col_index += 0
    elif decayChainName == "U238":
        col_index += 2
    elif decayChainName == "K40":
        col_index += 4
    elif decayChainName == "Co60":
        col_index += 6

    # this is were the cell content is changed
    for name in reversed(sorted(integrals)):
        cell = sheet1.find(r=chr(65 + col_index)+str(row_index))
        cell.v.string = str(integrals[name])
        cell = sheet1.find(r=chr(65 + col_index + 1)+str(row_index))
        cell.v.string = str(relerrs[name]*integrals[name])
        row_index += 1

    # add sheet1 & workbook back into xlsx file
    with zipfile.ZipFile(xls_out, mode='a', compression=zipfile.ZIP_DEFLATED) as zf:
        zf.writestr('xl/worksheets/sheet1.xml', str(sheet1))
        zf.writestr('xl/workbook.xml',str(workbook))

    return 0

def EvaluateIntegrals(location, chainList, plots):

    card = globalArgs['Card']
    totalSim = int(card.Get('TotalNumberOfEvents'))

    PerformCuts(location,chainList,plots)
    
    integrals = {}
    relerrs = {}
    effs = {}
    vars = {}
    dir = ROOT.gDirectory
    for plot in plots:
        for isotope in chainList:
            chain = chainList[isotope]

            if plot.entrylist:
                chain.SetEntryList(plot.entrylist[isotope])
            chain.Draw("%s>>%s%s"%(plot.expression,plot.destination,plot.specifications),plot.weight,plot.option)

            destination = dir.Get(plot.destination)
            integrals[plot.name] = destination.Integral()
            entries = destination.GetEntries()
            if entries > 0:
                relerrs[plot.name] = entries**0.5 / entries
            else:
                relerrs[plot.name] = 0
            effs[plot.name] = (integrals[plot.name]+1)*1./(totalSim + 2)
            vars[plot.name] = effs[plot.name]*((integrals[plot.name]+2)*1./(totalSim + 3) - effs[plot.name])
            

            print 'Integral of %s for %s in %s = %f' % (plot.name,isotope,location,destination.Integral())

    print '---------------------------------------------------'
    if globalArgs['OutputFileName']:
        outName = globalArgs['OutputFileName']
        dotPos = outName.rfind('.')
        outName = outName[:dotPos] + location + outName[dotPos:]
        fout = open(outName,'w')

    for name in reversed(sorted(integrals)):
        print name, 'counts =', integrals[name], '+-', relerrs[name]*integrals[name], '(%.1f'%(relerrs[name]*100), '%)', 'eff =', effs[name], '+-', vars[name]**0.5, '(%.1f'%((vars[name]**0.5/effs[name])*100), '%)'
        if globalArgs['OutputFileName']:
            if globalArgs['PrintInfo'] == 'counts':
                fout.write('%.2f;%.2f\n'%(integrals[name],relerrs[name]*integrals[name]))
            else:
                fout.write('%s;%.2f;%.2f;%s;%s\n'%(name,integrals[name],relerrs[name]*integrals[name],str(effs[name]),str(vars[name]**0.5)))
    if globalArgs['OutputFileName']:
        fout.close()

    if globalArgs['ExcelTemplate']:
        ExportToExcel(location, integrals, relerrs)

    print '---------------------------------------------------'

def LoopAllLocations():

    card = globalArgs['Card']

    #if os.path.isfile(globalArgs['OutputFileName']):
    #    os.system('rm %s'%(globalArgs['OutputFileName']))

    for pos in card.Get('LocationList').split(','):
        location = pos[pos.rfind('/')+1:]
        chainList = GetChainsForLocation(location)

        plots = GetPlots()

        if len(plots) > 0: 
            EvaluateIntegrals(location, chainList, plots)
        

if __name__ == "__main__":

    usage = "usage: python ExportIntegrals.py -c CardName -i InputDir"
    parser = OptionParser(usage)
    
    parser.add_option("-c","--card", dest="cardName", nargs=1)
    parser.add_option("-i","--input-dir", dest="inDir", nargs=1)
    parser.add_option("-o","--output", dest="outFileName", nargs=1)
    parser.add_option("-a","--all-isotopes", dest="allIsotopes", action="store_true",default=False)
    parser.add_option("-p","--print-info", dest="printInfo", nargs=1)
    parser.add_option("-e", "--excel", dest="excelTemplate", nargs=1)

    options,args = parser.parse_args()   
    print 'Using options:', options

    if not (options.cardName and options.inDir):
        print 'Missing options...', usage
        sys.exit()

    CatchGlobalArguments(options)
    
    LoopAllLocations()
