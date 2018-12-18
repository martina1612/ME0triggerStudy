# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: SingleMuPt100_pythia8_cfi.py --conditions auto:phase2_realistic -n 10 --era Phase2 --eventcontent FEVTDEBUG -s GEN,SIM --datatier GEN-SIM --beamspot HLLHC14TeV --geometry Extended2023D20 --python GEN-SIM_beamspot14TeV_.py --no_exec --fileout file:step1.root
import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras

process = cms.Process('SIM',eras.Phase2)

# import of standard configurations
#process.load('B0Tau3Mu_EvtGen_cfi')
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.Geometry.GeometryExtended2023D20Reco_cff')
process.load('Configuration.Geometry.GeometryExtended2023D20_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.Generator_cff')
process.load('IOMC.EventVertexGenerators.VtxSmearedHLLHC14TeV_cfi')
process.load('GeneratorInterface.Core.genFilterSummary_cff')
process.load('Configuration.StandardSequences.SimIdeal_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
)

# Input source
process.source = cms.Source("EmptySource",
    firstLuminosityBlock = cms.untracked.uint32(50001),
    firstEvent = cms.untracked.uint32(5000001)
)

process.options = cms.untracked.PSet(

)

#process.RandomNumberGeneratorService.generator.initialSeed = 12345
process.RandomNumberGeneratorService.generator.initialSeed = 12305

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('SingleMuPt100_pythia8_cfi.py nevts:10'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition

process.FEVTDEBUGoutput = cms.OutputModule("PoolOutputModule",
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('generation_step')
    ),
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('GEN-SIM'),
        filterName = cms.untracked.string('')
    ),
    #fileName = cms.untracked.string('file:B0_beamspot14TeV_step1.root'),
    #fileName = cms.untracked.string('file:B0_beamspot14TeV_step1-seed123-seed1234.root'),
    fileName = cms.untracked.string('file:B0_beamspot14TeV_step1-seed12305.root'),
    outputCommands = process.FEVTDEBUGEventContent.outputCommands,
    splitLevel = cms.untracked.int32(0)
)

# Additional output definition

# Other statements
process.genstepfilter.triggerConditions=cms.vstring("generation_step")
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase2_realistic', '')

# COPIED CONFIGURATION:

from Configuration.Generator.Pythia8CommonSettings_cfi import *
from Configuration.Generator.Pythia8CUEP8M1Settings_cfi import *

process.generator = cms.EDFilter("Pythia8GeneratorFilter",
    maxEventsToPrint = cms.untracked.int32(1),
    pythiaPylistVerbosity = cms.untracked.int32(1),
    filterEfficiency = cms.untracked.double(0.0011),
    pythiaHepMCVerbosity = cms.untracked.bool(False),
    comEnergy = cms.double(14000.),
    ExternalDecays = cms.PSet(
        EvtGen130 = cms.untracked.PSet(
            decay_table = cms.string('GeneratorInterface/EvtGenInterface/data/DECAY_2014_NOLONGLIFE.DEC'),
            particle_property_file = cms.FileInPath('GeneratorInterface/EvtGenInterface/data/evt_2014.pdl'),
            list_forced_decays = cms.vstring('myB0','myantiB0'),        # will force one at the time, notice just the parent
            operates_on_particles = cms.vint32(511,-511),               # we care just about our signal particles
            convertPythiaCodes = cms.untracked.bool(False),
            user_decay_embedded= cms.vstring(
"""
Alias myB0 B0
Alias myantiB0 anti-B0
Alias mytau+ tau+
Alias mytau- tau-
ChargeConj myB0 myantiB0
ChargeConj mytau+ mytau-

Decay myantiB0
0.259     D+       mytau-     nu_tau    ISGW2;
0.592     D*+      mytau-     nu_tau    ISGW2;
0.074     D_2*+    mytau-     nu_tau    ISGW2;
0.074     D'_1+    mytau-     nu_tau    ISGW2;
Enddecay
CDecay myB0

Decay mytau-
1.0 mu-    mu+    mu-             PHOTOS PHSP;
Enddecay
CDecay mytau+

End
"""
            ),
        ),
        parameterSets = cms.vstring('EvtGen130'),
    ),
    PythiaParameters = cms.PSet(
        pythia8CommonSettingsBlock,
        pythia8CUEP8M1SettingsBlock,
        processParameters = cms.vstring(
            'SoftQCD:nonDiffractive = on',
            'SoftQCD:singleDiffractive = on',
            'SoftQCD:doubleDiffractive = on',
            'PTFilter:filter = on', # this turn on the filter
            'PTFilter:quarkToFilter = 5', # PDG id of b quark
            'PTFilter:scaleToFilter = 1.0'
        ),
        parameterSets = cms.vstring('pythia8CommonSettings',
                                    'pythia8CUEP8M1Settings',
                                    'processParameters',
                                    )
    )
)

process.B0Filter = cms.EDFilter("PythiaFilter",
    ParticleID = cms.untracked.int32(511)  #B0
)

process.MuFilter = cms.EDFilter("MCParticlePairFilter",
    MinPt = cms.untracked.vdouble(0.0, 0.0),
    MaxEta = cms.untracked.vdouble(1000, 3),
    MinEta = cms.untracked.vdouble(-1000, 1.8),
    ParticleID1 = cms.untracked.vint32(13),
    ParticleID2 = cms.untracked.vint32(13)
)

process.ProductionFilterSequence = cms.Sequence(process.generator * process.B0Filter * process.MuFilter)



# END OF COPIED CONFIGURATION

# Path and EndPath definitions
process.generation_step = cms.Path(process.pgen)
#process.generation_step = cms.Path(process.ProductionFilterSequence)
process.simulation_step = cms.Path(process.psim)
process.genfiltersummary_step = cms.EndPath(process.genFilterSummary)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.FEVTDEBUGoutput_step = cms.EndPath(process.FEVTDEBUGoutput)

# Schedule definition
process.schedule = cms.Schedule(process.generation_step,process.genfiltersummary_step,process.simulation_step,process.endjob_step,process.FEVTDEBUGoutput_step)
from PhysicsTools.PatAlgos.tools.helpers import associatePatAlgosToolsTask
associatePatAlgosToolsTask(process)
# filter all path with the production filter sequence
for path in process.paths:
	getattr(process,path)._seq = process.ProductionFilterSequence * getattr(process,path)._seq 


# Customisation from command line

# Add early deletion of temporary data products to reduce peak memory need
from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
process = customiseEarlyDelete(process)
# End adding early deletion
