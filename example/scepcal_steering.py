from DDSim.DD4hepSimulation import DD4hepSimulation
from g4units import mm, GeV, MeV, keV, eV
from math import pi, atan2

def setupCerenkovScint(kernel):
     from DDG4 import PhysicsList
     seq = kernel.physicsList()

     scint = PhysicsList(kernel, 'Geant4ScintillationPhysics/ScintillationPhys')
     scint.VerboseLevel = 0
     scint.TrackSecondariesFirst = True
     scint.enableUI()
     seq.adopt(scint)

     cerenkov = PhysicsList(kernel, 'Geant4CerenkovPhysics/CerenkovPhys')
     cerenkov.VerboseLevel = 0
     cerenkov.MaxNumPhotonsPerStep = 10
     cerenkov.MaxBetaChangePerStep = 10.0
     cerenkov.TrackSecondariesFirst = True
     cerenkov.enableUI()
     seq.adopt(cerenkov)

     ph = PhysicsList(kernel, 'Geant4OpticalPhotonPhysics/OpticalGammaPhys')
     ph.addParticleConstructor('G4OpticalPhoton')
     ph.VerboseLevel = 0
     ph.enableUI()
     seq.adopt(ph)

     return None

def setupCerenkovOnly(kernel):
     from DDG4 import PhysicsList
     seq = kernel.physicsList()

     scint = PhysicsList(kernel, 'Geant4ScintillationPhysics/ScintillationPhys')
     scint.VerboseLevel = 0
     scint.TrackSecondariesFirst = True
     scint.enableUI()
     seq.adopt(scint)

     cerenkov = PhysicsList(kernel, 'Geant4CerenkovPhysics/CerenkovPhys')
     cerenkov.VerboseLevel = 0
     cerenkov.MaxNumPhotonsPerStep = 10
     cerenkov.MaxBetaChangePerStep = 10.0
     cerenkov.TrackSecondariesFirst = True
     cerenkov.enableUI()
     seq.adopt(cerenkov)

     ph = PhysicsList(kernel, 'Geant4OpticalPhotonPhysics/OpticalGammaPhys')
     ph.addParticleConstructor('G4OpticalPhoton')
     ph.VerboseLevel = 0
     ph.enableUI()
     seq.adopt(ph)

     return None

# See DD4hep/DDG4/python/DDSim/DD4hepSimulation.py
SIM = DD4hepSimulation()
SIM.runType = "batch"

# (1) VERBOSE, DEBUG, INFO, WARNING, ERROR, FATAL, ALWAYS (7)
SIM.printLevel = 3
SIM.output.geometry = 2
SIM.output.inputStage = 3
SIM.output.kernel = 3
SIM.output.part = 3
SIM.output.random = 6

settings = {
     'opticalPhysics': setupCerenkovScint, # or setupCerenkovOnly
     'edep' : 0*keV,

     'N'        : 1,
     
     'MC'       : False,
     'MC_file'  : '/your/mc/file',

     'gun'      : True,
     'particle' : 'e-',
     'momentum' : 10*GeV,
     'plusminus': 0.05*10*GeV,
     'theta'    : [(10)*pi/180.0,(170)*pi/180.0],
     'phi'      : [(0)*pi/180.0, (360)*pi/180.0],

     'compactFile': '../compact/SCEPCal.xml'
}

#~~~~~~~~~~~~~~ Settings ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

SIM.compactFile = settings['compactFile']
SIM.numberOfEvents = settings['N']
SIM.skipNEvents = 0

prefix = f"{settings['particle']}_{settings['momentum']/GeV:1.0f}GeV_N{settings['N']}"

SIM.outputFile =f"{prefix}.root"

print(f"\nOutput file: {SIM.outputFile}\n")

#~~~~~~~~~~~~~~ Particle Gun ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
if settings['gun']==True:
     SIM.enableGun        = True
else:
     SIM.enableGun        = False
SIM.gun.multiplicity = 1
SIM.gun.distribution = 'uniform'
SIM.gun.particle     = settings['particle']
SIM.gun.position     = (0, 0, 0)
SIM.gun.momentumMin  = settings['momentum']-settings['plusminus']
SIM.gun.momentumMax  = settings['momentum']+settings['plusminus']
SIM.gun.phiMin       = settings['phi'][0]
SIM.gun.phiMax       = settings['phi'][1]
SIM.gun.thetaMin     = settings['theta'][0]
SIM.gun.thetaMax     = settings['theta'][1]

#~~~~~~~~~~~~~~ MC ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
if settings['MC']==True:
     SIM.inputFiles = settings['MC_file']
else:
     SIM.inputFiles = []
     
#~~~~~~~~~~~~~~ Vertex ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SIM.crossingAngleBoost = 0.0
SIM.vertexOffset = [0.0, 0.0, 0.0, 0.0]
SIM.vertexSigma  = [0.0, 0.0, 0.0, 0.0]

#~~~~~~~~~~~~~~ Filters ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SIM.filter.filters = {
     'geantino':{'name':     'GeantinoRejectFilter/GeantinoRejector',
                 'parameter':{ }},
     'edep':    {'name':     'EnergyDepositMinimumCut/wvmax',
                 'parameter':{'Cut':settings['edep']}}
}

#~~~~~~~~~~~~~~ Calorimeter ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SIM.action.calorimeterSDTypes = ['calorimeter']
SIM.action.mapActions["SCEPCal_MainLayer"]     = "SCEPCal_MainSDAction"
SIM.action.mapActions["SCEPCal_TimingLayer"]   = "SCEPCal_TimingSDAction"
SIM.filter.mapDetFilter['SCEPCal_MainLayer']   = 'edep'
SIM.filter.mapDetFilter['SCEPCal_TimingLayer'] = 'edep'

#~~~~~~~~~~~~~~ Particles ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SIM.part.keepAllParticles                  = False
SIM.part.minimalKineticEnergy              = 1*GeV
SIM.part.minDistToParentVertex             = 2.2e-14
SIM.part.enableDetailedHitsAndParticleInfo = False
SIM.part.printEndTracking                  = False
SIM.part.printStartTracking                = False
SIM.part.saveProcesses                     = ['Decay']
SIM.part.userParticleHandler               = ''

#~~~~~~~~~~~~~~ Physics ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SIM.physics.decays     = False
SIM.physics.list       = "FTFP_BERT"
SIM.physics.pdgfile    = None
SIM.physics.rangecut   = None
SIM.physics.rejectPDGs = {1, 2, 3, 4, 5, 6,
                          3201, 3203, 4101, 4103,
                          21, 23, 24, 25, 
                          5401, 2203, 5403,
                          3101, 3103, 4403,
                          2101, 5301, 2103, 5303,
                          4301, 1103, 4303, 5201, 
                          5203, 3303, 4201, 4203, 
                          5101, 5103, 5503}
SIM.physics.zeroTimePDGs = {17, 11, 13, 15}
SIM.physics.setupUserPhysics(settings['opticalPhysics'])

#~~~~~~~~~~~~~~ Random Generator ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SIM.random.enableEventSeed = False
SIM.random.file            = None
SIM.random.luxury          = 1
SIM.random.replace_gRandom = True
SIM.random.seed            = None
SIM.random.type            = None