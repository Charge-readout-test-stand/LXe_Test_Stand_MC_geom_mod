
import ROOT
import sys, os, copy
from Card import Card
import SimUtils

from optparse import OptionParser
import tempfile
import zipfile
import shutil

globalArgs = {}

class SimplePlot:
    def __init__(self, name, expression, cut = '', option = '', draw = '', destination = 'histo', specifications = '', xTitle = '', yTitle = '', zTitle = '', entrylist = None):
        self.name = name
        self.expression = expression
        self.weight = 'Weight'
        self.cut = cut
        self.option = '%s goff' % option
        self.destination = destination
        self.specifications = specifications
        self.entrylist = entrylist
        self.draw = draw

        self.xTitle = xTitle
        self.yTitle = yTitle
        self.zTitle = zTitle

def CatchGlobalArguments(options):

    global globalArgs

    globalArgs['CardName'] = options.cardName #str(sys.argv[1])
    globalArgs['Process'] = options.processName 
    globalArgs['InputDir'] = options.inDir
    globalArgs['OutputFileName'] = options.outFileName
    if not globalArgs['OutputFileName'].endswith('.root'):
        globalArgs['OutputFileName'] += '.root'
        print 'Adding extension .root to output file name:', globalArgs['OutputFileName']
    globalArgs['AllIsotopes'] = bool(options.allIsotopes)

    globalArgs['WritePdf'] = bool(options.writePdf)
    globalArgs['OutputPdfFileName'] = None
    if globalArgs['WritePdf']:
        globalArgs['OutputPdfFileName'] = globalArgs['OutputFileName'].replace('.root','.pdf')

    globalArgs['Card'] = Card(globalArgs['CardName'])

    globalArgs['Vertex'] = options.vertex
    if globalArgs['Vertex']:
        globalArgs['VertexRadius'], globalArgs['VertexMinAxial'], globalArgs['VertexMaxAxial'] = options.vertex
    globalArgs['VertexOut'] = options.vertexOut

    globalArgs['TableName'] = options.tableTxtName
    globalArgs['ExcelTemplate'] = options.excelTemplate

def GetChainsForLocation(location):

    card = globalArgs['Card']

    processName = globalArgs['Process']
    if globalArgs['Process'] in ['sens','table']:
        processName = 'recon'
    processTitle = processName.title()

    treeName = card.Get('%sTreeName'%processTitle)#(globalArgs['Process'].title()))
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

def GetRadiusAndAxial(r1,r2,axial,pre,pos):

    return 'sqrt(%s%s%s**2 + %s%s%s**2)' % (pre,r1,pos,pre,r2,pos), '%s%s%s' % (pre,axial,pos)

def GetRadius2AndAxial(r1,r2,axial,pre,pos):

    return '%s%s%s**2 + %s%s%s**2' % (pre,r1,pos,pre,r2,pos), '%s%s%s' % (pre,axial,pos)

def GetG4Plots():

    plots = []
    plots.append(SimplePlot(name = 'GenY_vs_GenX', expression = 'GenY:GenX', xTitle = 'X (mm)', yTitle = 'Y (mm)', draw = 'colz'))
    plots.append(SimplePlot(name = 'GenX_vs_GenZ', expression = 'GenX:GenZ', xTitle = 'Z (mm)', yTitle = 'X (mm)', draw = 'colz'))
    plots.append(SimplePlot(name = 'GenY_vs_GenZ', expression = 'GenY:GenZ', xTitle = 'Z (mm)', yTitle = 'Y (mm)', draw = 'colz'))
    plots.append(SimplePlot(name = 'NuclideZ', expression = 'NuclideZ', specifications = '(200,0,200)', xTitle = 'Isotope Atomic Number Z'))
    plots.append(SimplePlot(name = 'TotalEventEnergy', expression = 'TotalEventEnergy', specifications = '(250,0,5)', xTitle = 'Energy (MeV)', cut = 'TotalEventEnergy > 0'))
    plots.append(SimplePlot(name = 'NumDeposits', expression = 'NumDeposits', xTitle = 'Number of G4 Deposits', cut = 'TotalEventEnergy > 0'))
    plots.append(SimplePlot(name = 'NumDeposits_vs_TotalEventEnergy', expression = 'NumDeposits:TotalEventEnergy', xTitle = 'Total Energy (MeV)', yTitle = 'Number of G4 Deposits', cut = 'TotalEventEnergy > 0', draw = 'colz'))
    plots.append(SimplePlot(name = 'EnergyDeposit', expression = 'EnergyDeposit', xTitle = 'Total Energy (MeV)', cut = 'EnergyDeposit > 0'))
    plots.append(SimplePlot(name = 'Ypos_vs_Xpos', expression = 'Ypos:Xpos', xTitle = 'Xpos (mm)', yTitle = 'Ypos (mm)', cut = 'EnergyDeposit > 0', draw = 'colz'))
    plots.append(SimplePlot(name = 'Xpos_vs_Zpos', expression = 'Xpos:Zpos', xTitle = 'Zpos (mm)', yTitle = 'Xpos (mm)', cut = 'EnergyDeposit > 0', draw = 'colz'))
    plots.append(SimplePlot(name = 'Ypos_vs_Zpos', expression = 'Ypos:Zpos', xTitle = 'Zpos (mm)', yTitle = 'Ypos (mm)', cut = 'EnergyDeposit > 0', draw = 'colz'))

    r1, r2, axial = SimUtils.GetRadialAndAxialCoordinatesString(globalArgs['Card'])
    radius, axial = GetRadius2AndAxial(r1,r2,axial,'','pos')

    plots.append(SimplePlot(name = 'Radius2_vs_Axial', expression = '%s:%s'%(radius,axial), xTitle = 'Axial = %s (mm)' % (axial), yTitle = 'Radius2 = %s (mm)' % (radius), cut = 'EnergyDeposit > 0', draw = 'colz'))
    plots.append(SimplePlot(name = 'NumDeposits_vs_Radius2', expression = 'NumDeposits:%s'%(radius), xTitle = 'Radius2 = %s (mm)' % (radius), yTitle = 'Number of G4 Deposits', cut = 'EnergyDeposit > 0', draw = 'colz'))
    plots.append(SimplePlot(name = 'NumDeposits_vs_Axial', expression = 'NumDeposits:%s'%(axial), xTitle = 'Axial = %s (mm)' % (axial), yTitle = 'Number of G4 Deposits', cut = 'EnergyDeposit > 0', draw = 'colz'))
    

    return plots

def GetClusterPlots():

    plots = []
    plots.append(SimplePlot(name = 'TotalEventEnergy', expression = 'TotalEventEnergy', specifications = '(250,0,5)', xTitle = 'Total Energy (MeV)', cut = 'TotalEventEnergy > 0'))
    plots.append(SimplePlot(name = 'NumClusters', expression = 'NumClusters', xTitle = 'Number of Clusters', cut = 'TotalEventEnergy > 0'))
    plots.append(SimplePlot(name = 'NumClusters_vs_TotalEventEnergy', expression = 'NumClusters:TotalEventEnergy', specifications = '(250,0,5,50,0,50)', xTitle = 'Total Energy (MeV)', yTitle = 'Number of Clusters', cut = 'TotalEventEnergy > 0', draw = 'colz'))
    plots.append(SimplePlot(name = 'Ypos_vs_Xpos', expression = 'Ypos:Xpos', xTitle = 'Xpos (mm)', yTitle = 'Ypos (mm)', cut = 'EnergyDeposit > 0', draw = 'colz'))
    plots.append(SimplePlot(name = 'Xpos_vs_Zpos', expression = 'Xpos:Zpos', xTitle = 'Zpos (mm)', yTitle = 'Xpos (mm)', cut = 'EnergyDeposit > 0', draw = 'colz'))
    plots.append(SimplePlot(name = 'Ypos_vs_Zpos', expression = 'Ypos:Zpos', xTitle = 'Zpos (mm)', yTitle = 'Ypos (mm)', cut = 'EnergyDeposit > 0', draw = 'colz'))

    r1, r2, axial = SimUtils.GetRadialAndAxialCoordinatesString(globalArgs['Card'])
    radius, axial = GetRadius2AndAxial(r1,r2,axial,'','pos')

    plots.append(SimplePlot(name = 'Radius2_vs_Axial', expression = '%s:%s'%(radius,axial), xTitle = 'Axial = %s (mm)' % (axial), yTitle = 'Radius2 = %s (mm)' % (radius), cut = 'EnergyDeposit > 0', draw = 'colz'))
    plots.append(SimplePlot(name = 'NumClusters_vs_Radius2', expression = 'NumClusters:%s'%(radius), xTitle = 'Radius2 = %s (mm)' % (radius), yTitle = 'Number of Clusters', cut = 'EnergyDeposit > 0', draw = 'colz'))
    plots.append(SimplePlot(name = 'NumClusters_vs_Axial', expression = 'NumClusters:%s'%(axial), xTitle = 'Axial = %s (mm)' % (axial), yTitle = 'Number of Clusters', cut = 'EnergyDeposit > 0', draw = 'colz'))

    return plots

def GetReconPlots():

    plots = []
    plots.append(SimplePlot(name = 'ReconFlag', expression = 'ReconFlag', specifications = '(5,-2,3)', xTitle = 'Reconstruction Flag'))
    plots.append(SimplePlot(name = 'TotalReconEnergy', expression = 'TotalReconEnergy', specifications = '(250,0,5)', xTitle = 'Reconstructed Energy (MeV)', cut = 'ReconFlag == 2'))
    plots.append(SimplePlot(name = 'SmearedEnergy', expression = 'SmearedEnergy', specifications = '(250,0,5)', xTitle = 'Smeared Energy (MeV)', cut = 'ReconFlag == 2'))
    plots.append(SimplePlot(name = 'StandoffDistance', expression = 'StandoffDistance', specifications = '(1000,0,1000)', xTitle = 'Standoff Distance (mm)', cut = 'ReconFlag == 2'))
    plots.append(SimplePlot(name = 'NumReconClusters', expression = 'NumReconClusters', xTitle = 'Number of Reconstructed Clusters', cut = 'ReconFlag == 2'))
    plots.append(SimplePlot(name = 'NumReconClusters_vs_TotalEventEnergy', expression = 'NumReconClusters:TotalEventEnergy', specifications = '(250,0,5,20,0,20)', xTitle = 'Total Energy (MeV)', yTitle = 'Number of Reconstructed Clusters', cut = 'ReconFlag == 2', draw = 'colz'))
    plots.append(SimplePlot(name = 'NumReconClusters_vs_TotalReconEnergy', expression = 'NumReconClusters:TotalReconEnergy', specifications = '(250,0,5,20,0,20)', xTitle = 'Reconstructed Energy (MeV)', yTitle = 'Number of Reconstructed Clusters', cut = 'ReconFlag == 2', draw = 'colz'))

    plots.append(SimplePlot(name = 'TotalReconEnergy_ss', expression = 'TotalReconEnergy', specifications = '(250,0,5)', xTitle = 'SS Reconstructed Energy (MeV)', cut = 'ReconFlag == 2 && NumReconClusters == 1'))
    plots.append(SimplePlot(name = 'SmearedEnergy_ss', expression = 'SmearedEnergy', specifications = '(250,0,5)', xTitle = 'SS Smeared Energy (MeV)', cut = 'ReconFlag == 2 && NumReconClusters == 1'))
    plots.append(SimplePlot(name = 'StandoffDistance_ss', expression = 'StandoffDistance', specifications = '(1000,0,1000)', xTitle = 'Standoff Distance (mm)', cut = 'ReconFlag == 2 && NumReconClusters == 1'))
    plots.append(SimplePlot(name = 'TotalReconEnergy_ms', expression = 'TotalReconEnergy', specifications = '(250,0,5)', xTitle = 'MS Reconstructed Energy (MeV)', cut = 'ReconFlag == 2 && NumReconClusters > 1'))
    plots.append(SimplePlot(name = 'SmearedEnergy_ms', expression = 'SmearedEnergy', specifications = '(250,0,5)', xTitle = 'MS Smeared Energy (MeV)', cut = 'ReconFlag == 2 && NumReconClusters > 1'))
    plots.append(SimplePlot(name = 'StandoffDistance_ms', expression = 'StandoffDistance', specifications = '(1000,0,1000)', xTitle = 'Standoff Distance (mm)', cut = 'ReconFlag == 2 && NumReconClusters > 1'))

    return plots

def GetSensitivityPlots():

    plots = []
    plots.append(SimplePlot(name = 'h_SSEnergy', expression = 'TotalReconEnergy*1000.', specifications = '(2800,700,3500)', xTitle = 'SS Reconstructed Energy (keV)', cut = 'ReconFlag == 2 && NumReconClusters == 1'))
    plots.append(SimplePlot(name = 'h_MSEnergy', expression = 'TotalReconEnergy*1000.', specifications = '(2800,700,3500)', xTitle = 'MS Reconstructed Energy (keV)', cut = 'ReconFlag == 2 && NumReconClusters > 1'))
    plots.append(SimplePlot(name = 'h_SSEnergy_Smear', expression = 'SmearedEnergy*1000.', specifications = '(2800,700,3500)', xTitle = 'SS Smeared Energy (keV)', cut = 'ReconFlag == 2 && NumReconClusters == 1'))
    plots.append(SimplePlot(name = 'h_MSEnergy_Smear', expression = 'SmearedEnergy*1000.', specifications = '(2800,700,3500)', xTitle = 'MS Smeared Energy (keV)', cut = 'ReconFlag == 2 && NumReconClusters > 1'))
    plots.append(SimplePlot(name = 'h_StandoffDistSS', expression = 'StandoffDistance', specifications = '(650,0,650)', xTitle = 'SS Standoff Distance (mm)', cut = 'ReconFlag == 2 && NumReconClusters == 1'))
    plots.append(SimplePlot(name = 'h_StandoffDistMS', expression = 'StandoffDistance', specifications = '(650,0,650)', xTitle = 'MS Standoff Distance (mm)', cut = 'ReconFlag == 2 && NumReconClusters > 1'))
    plots.append(SimplePlot(name = 'h_StandoffVsEnergySS', expression = 'StandoffDistance:TotalReconEnergy*1000.', specifications = '(2800,700,3500,650,0,650)', xTitle = 'SS Reconstructed Energy (keV)', yTitle = 'SS Standoff Distance (mm)', cut = 'ReconFlag == 2 && NumReconClusters == 1', draw = 'colz'))
    plots.append(SimplePlot(name = 'h_StandoffVsEnergyMS', expression = 'StandoffDistance:TotalReconEnergy*1000.', specifications = '(2800,700,3500,650,0,650)', xTitle = 'MS Reconstructed Energy (keV)', yTitle = 'MS Standoff Distance (mm)', cut = 'ReconFlag == 2 && NumReconClusters > 1', draw = 'colz'))
    plots.append(SimplePlot(name = 'h_StandoffVsEnergySS_Smear', expression = 'StandoffDistance:SmearedEnergy*1000.', specifications = '(2800,700,3500,650,0,650)', xTitle = 'SS Smeared Energy (keV)', yTitle = 'SS Standoff Distance (mm)', cut = 'ReconFlag == 2 && NumReconClusters == 1', draw = 'colz'))
    plots.append(SimplePlot(name = 'h_StandoffVsEnergyMS_Smear', expression = 'StandoffDistance:SmearedEnergy*1000.', specifications = '(2800,700,3500,650,0,650)', xTitle = 'MS Smeared Energy (keV)', yTitle = 'MS Standoff Distance (mm)', cut = 'ReconFlag == 2 && NumReconClusters > 1', draw = 'colz'))

    return plots

def GetTablePlots():

    plots = []
    plots.append(SimplePlot(name = 'h_StandoffVsEnergySS_Smear', expression = 'StandoffDistance:SmearedEnergy*1000.', specifications = '(2800,700,3500,650,0,650)', xTitle = 'SS Smeared Energy (keV)', yTitle = 'SS Standoff Distance (mm)', cut = 'ReconFlag == 2 && NumReconClusters == 1', draw = 'colz'))
    plots.append(SimplePlot(name = 'h_StandoffVsEnergyMS_Smear', expression = 'StandoffDistance:SmearedEnergy*1000.', specifications = '(2800,700,3500,650,0,650)', xTitle = 'MS Smeared Energy (keV)', yTitle = 'MS Standoff Distance (mm)', cut = 'ReconFlag == 2 && NumReconClusters > 1', draw = 'colz'))

    return plots    

def ExportToExcel(location, integrals, errors):
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
        cell.v.string = str(errors[name])
        row_index += 1

    # add sheet1 & workbook back into xlsx file
    with zipfile.ZipFile(xls_out, mode='a', compression=zipfile.ZIP_DEFLATED) as zf:
        zf.writestr('xl/worksheets/sheet1.xml', str(sheet1))
        zf.writestr('xl/workbook.xml',str(workbook))

    return 0

def EvaluateIntegrals(histo,location,isotope):

    energyCuts = {}
    energyCuts['1sigma'] = [2433, 2483]
    energyCuts['2sigma'] = [2408, 2507]
    energyCuts['3sigma'] = [2384, 2532]
    energyCuts['FWHM'] = [2428, 2488]
    energyCuts['> 700 keV'] = [700, 3500]
    energyCuts['> 0'] = [0, 3500]

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

    histName = histo.GetName()
    multiplicity = ('SS','MS')[histName.find('MS')>=0]

    error = ROOT.Double()
    integrals = {}
    errors = {}
    for energyCut in energyCuts:
        energyLimit = energyCuts[energyCut]
        xl = histo.GetXaxis().FindBin(energyLimit[0])
        xu = histo.GetXaxis().FindBin(energyLimit[1])-1
        for sdCut in sdCuts:
            name = '%s;%s;%s' % (multiplicity,energyCut,sdCut)
            sdLimit = sdCuts[sdCut]
            yl = histo.GetYaxis().FindBin(sdLimit[0])
            yu = histo.GetYaxis().FindBin(sdLimit[1])-1

            integrals[name] = histo.IntegralAndError(xl,xu,yl,yu,error)
            errors[name] = float(error)
            #print energyCut, sdCut, integral, error

    return integrals, errors
    
def MakePlots(location,chainList,plots,outFile, canvas = None):

    ROOT.TH1.SetDefaultSumw2(True)

    card = globalArgs['Card']

    cutList = []
    for plot in plots:
        if plot.cut != '':
            cutList.append(plot.cut)
    cutSet = set(cutList)

    cutVertex = ''
    if globalArgs['Vertex']:
        print 'Will select only event vertices within (False) or without (True) =',  globalArgs['VertexOut'], 'this cylinder (radius, min axial, max axial) =', globalArgs['Vertex']
        
        lXeCenterX, lXeCenterY, lXeCenterZ = float(card.Get('LXeCenterX')), float(card.Get('LXeCenterY')), float(card.Get('LXeCenterZ'))
        lXeCenterR1, lXeCenterR2, lXeCenterAxial = SimUtils.GetRadialAndAxialCoordinates(lXeCenterX, lXeCenterY, lXeCenterZ, card)
        r1,r2,axial = SimUtils.GetRadialAndAxialCoordinatesString(card)
        sr1 = ('-','+')[lXeCenterR1 < 0]
        sr2 = ('-','+')[lXeCenterR2 < 0]
        sa = ('-','+')[lXeCenterAxial < 0]
        
        cutVertex = '(sqrt(pow(EventVertex%s %s %f,2) + pow(EventVertex%s %s %f,2)) < %f && %f < (EventVertex%s %s %f) && (EventVertex%s %s %f) < %f)' % (r1,sr1,abs(lXeCenterR1),r2,sr2,abs(lXeCenterR2),globalArgs['VertexRadius'],globalArgs['VertexMinAxial'],axial,sa,abs(lXeCenterAxial),axial,sa,abs(lXeCenterAxial),globalArgs['VertexMaxAxial'])
        if globalArgs['VertexOut']:
            cutVertex = cutVertex.replace('&&','||')
            cutVertex = cutVertex.replace('<','>')
        cutVertex = ' && ' + cutVertex

    cutDict = {}
    for c,cut in enumerate(cutSet):
        #cut += cutVertex
        cutDict[cut] = {}
        for i,isotope in enumerate(chainList):
            chain = chainList[isotope]
            entries = chain.GetEntries()
            listName = 'elist_%d_%d' % (c,i)
            print 'Selecting events for %s in %s with cut "%s" ...'%(isotope, location, cut+cutVertex)
            chain.Draw('>>%s'%(listName),cut+cutVertex,'entrylist')
            cutDict[cut][isotope] = ROOT.gDirectory.Get(listName)
            print '... selected', cutDict[cut][isotope].GetN(), 'events out of a total ', entries, '!'
            #del cutDict[cut][isotope]
    for plot in plots:
        if plot.cut in cutDict:
            plot.entrylist = cutDict[plot.cut]

    #dir = ROOT.gDirectory

    if not globalArgs['Process'] in ['sens','table']:
        locationDir = outFile.mkdir(location)
        locationDir.cd()
        isotopeDirs = {}
    else:
        locationFile = ROOT.TFile.Open(globalArgs['OutputFileName'].replace('.root','_%s.root'%(location)),'recreate')                    

    decayChainDir = None
    integrals = None

    for plot in plots:
        for isotope in chainList:
            if not globalArgs['Process'] in ['sens','table']:
                locationDir.cd()
                if not isotope in isotopeDirs:
                    isotopeDirs[isotope] = locationDir.mkdir(isotope)
                isotopeDir = isotopeDirs[isotope]
                isotopeDir.cd()
                curDir = isotopeDir
            else:
                locationFile.cd()#outFile.cd()
                curDir = ROOT.gDirectory

            chain = chainList[isotope]
#            isotopeDir.cd()

            print 'Making %s for %s in %s ...' % (plot.name,isotope,location)
                        
            if plot.entrylist:
                chain.SetEntryList(plot.entrylist[isotope])
            chain.Draw("%s>>%s%s"%(plot.expression,plot.destination,plot.specifications),plot.weight,plot.option)

            destination = curDir.Get(plot.destination)#isotopeDir.Get(plot.destination)
            destination.GetXaxis().SetTitle(plot.xTitle)
            destination.GetYaxis().SetTitle(plot.yTitle)
            destination.GetZaxis().SetTitle(plot.zTitle)
            destination.SetTitle('%s, %s'%(isotope,location))
            destination.SetName(plot.name)
            destination.Write()

            if globalArgs['WritePdf']:
                canvas.cd()
                destination.Draw(plot.draw)
                canvas.Print(globalArgs['OutputPdfFileName'])

            if globalArgs['Process'] == 'table':
                if not integrals:
                    integrals, errors = EvaluateIntegrals(destination, location, isotope)
                else:
                    newIntegrals, newErrors = EvaluateIntegrals(destination, location, isotope)
                    integrals.update(newIntegrals)
                    errors.update(newErrors)

    if globalArgs['Process'] in ['sens','table']:
        locationFile.Close()

    if globalArgs['Process'] == 'table':
        print '---------------------------------------------------'
        if globalArgs['TableName']:
            outName = globalArgs['TableName']
            dotPos = outName.rfind('.')
            outName = '%s_%s_%s%s' % (outName[:dotPos],isotope,location,outName[dotPos:])
            fout = open(outName,'w')

        for name in reversed(sorted(integrals)):
            strOut = '%s;%.2f;%.2f'%(name,integrals[name],errors[name])
            print strOut
            if globalArgs['TableName']:
                #if globalArgs['PrintInfo'] == 'counts':
                #    fout.write('%.2f;%.2f\n'%(integrals[name],relerrs[name]*integrals[name]))
                #else:
                fout.write(strOut+'\n')
        if globalArgs['TableName']:
            fout.close()

        if globalArgs['ExcelTemplate']:
            ExportToExcel(location, integrals, errors)

        print '---------------------------------------------------'
        

def LoopAllLocations():

    card = globalArgs['Card']

    outFile = ROOT.TFile.Open(globalArgs['OutputFileName'],'recreate')

    canvas = ROOT.TCanvas()
    if globalArgs['WritePdf']:
        canvas.Print('%s['%(globalArgs['OutputPdfFileName']))

    for pos in card.Get('LocationList').split(','):
        location = pos[pos.rfind('/')+1:]
        chainList = GetChainsForLocation(location)

        plots = None
        if globalArgs['Process'].lower() == 'g4': 
            plots = GetG4Plots()
        if globalArgs['Process'].lower() == 'cluster': 
            plots = GetClusterPlots()
        if globalArgs['Process'].lower() == 'recon': 
            plots = GetClusterPlots() + GetReconPlots()
        if globalArgs['Process'].lower() == 'sens': 
            plots = GetSensitivityPlots()
        if globalArgs['Process'].lower() == 'table': 
            plots = GetTablePlots()


        if len(plots) > 0: 
            MakePlots(location, chainList, plots, outFile, canvas)

    if globalArgs['WritePdf']:
        canvas.Print('%s]'%(globalArgs['OutputPdfFileName']))
    outFile.Close()    

if __name__ == "__main__":

    usage = "usage: python MakeSimplePlotsDecayChain.py -c CardName -p Process -i InputDir -o OutputName"
    parser = OptionParser(usage)
    
    parser.add_option("-c","--card", dest="cardName", nargs=1)
    parser.add_option("-p","--process", dest="processName", nargs=1)
    parser.add_option("-i","--input-dir", dest="inDir", nargs=1)
    parser.add_option("-o","--output", dest="outFileName", nargs=1)
    parser.add_option("-a","--all-isotopes", dest="allIsotopes", action="store_true",default=False)
    parser.add_option("-w","--write-pdf", dest="writePdf", action="store_true",default=False)

    parser.add_option("--vertex", dest="vertex", type = "float", nargs=3)
    parser.add_option("--vertex-out", dest="vertexOut", action="store_true",default=False)   

    parser.add_option("-t","--table",dest="tableTxtName",nargs=1)
    parser.add_option("-e","--excel", dest="excelTemplate", nargs=1)
    

    options,args = parser.parse_args()   
    print 'Using options:', options

    if not (options.cardName and options.processName and options.outFileName):
        print 'Missing options...', usage
        sys.exit()

    CatchGlobalArguments(options)
    
    LoopAllLocations()

    
