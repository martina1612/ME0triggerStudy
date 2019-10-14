// 
//
// Package:    ME0analyzer/ME0SegmentAnalyzer
// Class:      ME0SegmentAnalyzer
// 
/**\class ME0SegmentAnalyzer ME0SegmentAnalyzer.cc ME0analyzer/ME0SegmentAnalyzer/plugins/ME0SegmentAnalyzer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Martina Ressegotti
//         Created:  Fri, 16 Feb 2018 10:28:44 GMT
//
//


// system include files
#include <memory>

#include <stdlib.h>
#include <vector>
#include <map>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/Utilities/interface/EDGetToken.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/GEMDigi/interface/ME0TriggerDigi.h"
#include "Geometry/GEMGeometry/interface/ME0Geometry.h"
#include "DataFormats/MuonData/interface/MuonDigiCollection.h"
#include "DataFormats/GEMDigi/interface/ME0Digi.h"
#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "Geometry/GEMGeometry/interface/ME0EtaPartitionSpecs.h"
#include "Geometry/GEMGeometry/interface/ME0EtaPartition.h"
#include "Geometry/CommonTopologies/interface/StripTopology.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/GEMDigi/interface/ME0PadDigi.h"
#include "DataFormats/GEMDigi/interface/ME0PadDigiCluster.h"
#include "DataFormats/GEMDigi/interface/ME0PadDigiClusterCollection.h"
#include "DataFormats/GEMRecHit/interface/ME0RecHit.h"
#include "DataFormats/GEMRecHit/interface/ME0RecHitCollection.h"
#include "SimDataFormats/TrackingHit/interface/PSimHit.h"
#include "DataFormats/GEMRecHit/interface/ME0SegmentCollection.h"
#include "DataFormats/GEMRecHit/interface/ME0Segment.h"
#include "DataFormats/GeometryVector/interface/Phi.h"
#include "DataFormats/TrackingRecHit/interface/TrackingRecHit.h"

#include <TTree.h>
#include <TH1F.h>
#include <TGraphErrors.h>
#include <TMath.h>
//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<> and also remove the line from
// constructor "usesResource("TFileService");"
// This will improve performance in multithreaded jobs.


using namespace edm;
using namespace std;
using namespace reco;

class ME0SegmentAnalyzer : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit ME0SegmentAnalyzer(const edm::ParameterSet&);
      ~ME0SegmentAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      // ----------member data ---------------------------
      edm::EDGetTokenT<MuonDigiCollection<ME0DetId,ME0Digi>> ME0DigiToken_;
      edm::EDGetTokenT<MuonDigiCollection<ME0DetId,ME0PadDigi>> ME0PadDigiToken_;
      edm::EDGetTokenT<MuonDigiCollection<ME0DetId,ME0PadDigiCluster>> ME0PadDigiClusterToken_;
      edm::EDGetTokenT<GenParticleCollection> genToken_;
      edm::EDGetTokenT<ME0RecHitCollection> ME0RecHitToken_;
      edm::EDGetTokenT<vector<PSimHit>> ME0SimHitToken_;
      edm::EDGetTokenT<ME0SegmentCollection> ME0SegmentToken_;

      bool v = 1 ; //verbose initialization

      TH1F * h_StripX	         ;
      TH1F * h_StripY	         ;
      TH1F * h_StripZ	         ;

      TH1F * h_RecHitX	         ;
      TH1F * h_RecHitY           ;
      TH1F * h_RecHitZ	         ;
      TH1F * h_SimHitX	         ;
      TH1F * h_SimHitY           ;
      TH1F * h_SimHitZ	         ;
      TH1F * h_RecoDigiX         ;
      TH1F * h_RecoDigiY         ;
      TH1F * h_RecoDigiZ         ;
      TH1F * h_RecHitErrX        ;
      TH1F * h_RecHitErrY        ;
      TH1F * h_RecoDigiX_fracErr ;
      TH1F * h_RecoDigiY_fracErr ;

      TH1F * h_RecHitSimHitX     ;
      TH1F * h_RecHitSimHitY     ;
      TH1F * h_RecHitSimHitZ     ;
      TH1F * h_RecHitSimHitX_fracErr     ;
      TH1F * h_RecHitSimHitY_fracErr     ;
      TH1F * h_nMuSimHit	;

      TH1F * h_RecHitSize_all		;
      TH1F * h_RecHitFirstStrip_all	;
      TH1F * h_RecHitSize_sel		;
      TH1F * h_RecHitFirstStrip_sel	;

      TH1F * h_SimHitDeltaPhi_posMu	;

      TH1F * h_SegmentDeltaPhiLocal3_posMu_posEnd	;
      TH1F * h_SegmentDeltaPhiLocal3_posMu_negEnd	;
      TH1F * h_SegmentDeltaPhiLocal3_negMu_posEnd	;
      TH1F * h_SegmentDeltaPhiLocal3_negMu_negEnd	;

      TH1F * h_SimHitDeltaPhiLocal3_posMu_posEnd	;
      TH1F * h_SimHitDeltaPhiLocal3_posMu_negEnd	;
      TH1F * h_SimHitDeltaPhiLocal3_negMu_posEnd	;
      TH1F * h_SimHitDeltaPhiLocal3_negMu_negEnd	;

      TH1F * h_SegmentDeltaZ_posMu	;

      TH1F * h_SegmentDeltaZ_negMu	;

      TH1F * h_deltaLocalX_pos	;
      TH1F * h_deltaLocalX_neg	;

      TH1F * h_wDistX_recHitSegment_posEnd ;
      TH1F * h_wDistY_recHitSegment_posEnd ;

      TTree *tr;
      TTree *tr2;
      float nEvent;
      float nSeg_posMu_posEnd;
      float nSeg_posMu_negEnd;
      float nSeg_negMu_posEnd;
      float nSeg_negMu_negEnd;
      float nSegmentRecHits_posMu_posEnd; 
      float nSegmentRecHits_posMu_negEnd; 
      float nSegmentRecHits_negMu_posEnd; 
      float nSegmentRecHits_negMu_negEnd; 
      float p_pos;
      float p_neg;
      float pt_pos;
      float pt_neg;
      float eta_pos;
      float eta_neg;
      float segmentPars_posMu_posEnd[10];
      float segmentPars_negMu_posEnd[10];
      float segmentPars_posMu_negEnd[10];
      float segmentPars_negMu_negEnd[10];

      float arr_NsimHit_posMu[6];
      float arr_NsimHit_negMu[6];
      float segment_thetaLocal3_posMu_negEnd, segment_thetaLocal3_posMu_posEnd, segment_thetaLocal3_negMu_posEnd, segment_thetaLocal3_negMu_negEnd,
      	    segment_alphaLocal3_posMu_negEnd, segment_alphaLocal3_posMu_posEnd, segment_alphaLocal3_negMu_posEnd, segment_alphaLocal3_negMu_negEnd;
      float simHit_thetaLocal3_posMu_negEnd, simHit_thetaLocal3_posMu_posEnd, simHit_thetaLocal3_negMu_posEnd, simHit_thetaLocal3_negMu_negEnd,
      	    simHit_alphaLocal3_posMu_negEnd, simHit_alphaLocal3_posMu_posEnd, simHit_alphaLocal3_negMu_posEnd, simHit_alphaLocal3_negMu_negEnd;
      float segmentDeltaPhiLocal3_posMu_negEnd, segmentDeltaPhiLocal3_posMu_posEnd, 
      	    segmentDeltaPhiLocal3_negMu_negEnd, segmentDeltaPhiLocal3_negMu_posEnd;
      float simHitDeltaPhiLocal3_posMu_negEnd, simHitDeltaPhiLocal3_posMu_posEnd, 
      	    simHitDeltaPhiLocal3_negMu_negEnd, simHitDeltaPhiLocal3_negMu_posEnd;
      float segmentDeltaGlobalPhi, segmentLayer1GlobalPhi, segmentLayer6GlobalPhi;
      float segmentDeltaGlobalPhi_posMu_posEnd, segmentLayer1GlobalPhi_posMu_posEnd, segmentLayer6GlobalPhi_posMu_posEnd;

      //float ratio_posMu_posEnd, ratio_posMu_negEnd, ratio_negMu_posEnd, ratio_negMu_negEnd;

      int nearbyChamber_posMu_posEnd;
      int nearbyChamber_negMu_posEnd;
      int nearbyChamber_posMu_negEnd;
      int nearbyChamber_negMu_negEnd;

      int consistent_signVec   = 0;
      int inconsistent_signVec = 0;

};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
ME0SegmentAnalyzer::ME0SegmentAnalyzer(const edm::ParameterSet& iConfig):
    ME0DigiToken_(consumes<MuonDigiCollection<ME0DetId,ME0Digi>>(iConfig.getParameter<edm::InputTag>("me0DigiToken"))),
    ME0PadDigiToken_(consumes<MuonDigiCollection<ME0DetId,ME0PadDigi>>(iConfig.getParameter<edm::InputTag>("me0PadDigiToken"))),
    ME0PadDigiClusterToken_(consumes<MuonDigiCollection<ME0DetId,ME0PadDigiCluster>>(iConfig.getParameter<edm::InputTag>("me0PadDigiClusterToken"))),
    genToken_(consumes<GenParticleCollection>(iConfig.getParameter<InputTag>("genParticles"))),
    ME0RecHitToken_(consumes<ME0RecHitCollection>(iConfig.getParameter<InputTag>("me0RecHits"))),
    ME0SimHitToken_(consumes<vector<PSimHit>>(iConfig.getParameter<InputTag>("me0SimHits"))),
    ME0SegmentToken_(consumes<ME0SegmentCollection>(iConfig.getParameter<InputTag>("me0Segments")))

{
   //now do what ever initialization is needed
   usesResource("TFileService");
   Service<TFileService> fs;
   tr = fs->make<TTree>("Run", "");

   v = iConfig.getParameter<bool>("verbose");

   tr->Branch("consistent_signVec"     ,	&consistent_signVec     , 	"consistent_signVec/I"     );
   tr->Branch("inconsistent_signVec"     ,	&inconsistent_signVec     , 	"inconsistent_signVec/I"     );

   h_StripX 	= fs->make<TH1F>("h_StripX","h_StripX", 1000, -100, 100);
   h_StripY 	= fs->make<TH1F>("h_StripY","h_StripY", 1000, -100, 100);
   h_StripZ 	= fs->make<TH1F>("h_StripZ","h_StripZ", 1000, -100, 100);

   h_RecHitX 	= fs->make<TH1F>("h_RecHitX","h_RecHitX", 1000, -100, 100);
   h_RecHitY 	= fs->make<TH1F>("h_RecHitY","h_RecHitY", 1000, -100, 100);
   h_RecHitZ 	= fs->make<TH1F>("h_RecHitZ","h_RecHitZ", 1000, -100, 100);

   h_RecHitErrX	= fs->make<TH1F>("h_RecHitErrX","h_RecHitErrX", 1000, -100, 100);
   h_RecHitErrY	= fs->make<TH1F>("h_RecHitErrY","h_RecHitErrY", 1000, -100, 100);

   h_RecoDigiX 	= fs->make<TH1F>("h_RecoDigiX","h_RecoDigiX", 1000, -100, 100);
   h_RecoDigiY 	= fs->make<TH1F>("h_RecoDigiY","h_RecoDigiY", 1000, -100, 100);
   h_RecoDigiZ 	= fs->make<TH1F>("h_RecoDigiZ","h_RecoDigiZ", 1000, -100, 100);

   h_SimHitX 	= fs->make<TH1F>("h_SimHitX","h_SimHitX", 1000, -100, 100);         
   h_SimHitY   	= fs->make<TH1F>("h_SimHitY","h_SimHitY", 1000, -100, 100);
   h_SimHitZ   	= fs->make<TH1F>("h_SimHitZ","h_SimHitZ", 1000, -100, 100);
   h_RecoDigiX_fracErr  = fs->make<TH1F>("h_RecoDigiX_fracErr","h_RecoDigiX_fracErr", 1000, -100, 100);
   h_RecoDigiY_fracErr  = fs->make<TH1F>("h_RecoDigiY_fracErr","h_RecoDigiY_fracErr", 1000, -100, 100);
                      
   h_RecHitSimHitX     	= fs->make<TH1F>("h_RecHitSimHitX","h_RecHitSimHitX", 1000, -100, 100);
   h_RecHitSimHitY     	= fs->make<TH1F>("h_RecHitSimHitY","h_RecHitSimHitY", 1000, -100, 100);
   h_RecHitSimHitZ     	= fs->make<TH1F>("h_RecHitSimHitZ","h_RecHitSimHitZ", 1000, -100, 100);
   h_RecHitSimHitX_fracErr     	= fs->make<TH1F>("h_RecHitSimHitX_fracErr","h_RecHitSimHitX_fracErr", 1000, -100, 100);
   h_RecHitSimHitY_fracErr     	= fs->make<TH1F>("h_RecHitSimHitY_fracErr","h_RecHitSimHitY_fracErr", 1000, -100, 100);

   h_nMuSimHit 	= fs->make<TH1F>("h_nMuSimHit","h_nMuSimHit", 21, -10, 10);

   h_RecHitSize_all      	= fs->make<TH1F>("h_RecHitSize_all","h_RecHitSize_all", 301, 0, 300);
   h_RecHitFirstStrip_all	= fs->make<TH1F>("h_RecHitFirstStrip_all","h_RecHitFirstStrip_all", 301, 0, 300);
   h_RecHitSize_sel      	= fs->make<TH1F>("h_RecHitSize_sel","h_RecHitSize_sel", 301, 0, 300);
   h_RecHitFirstStrip_sel	= fs->make<TH1F>("h_RecHitFirstStrip_sel","h_RecHitFirstStrip_sel", 301, 0, 300);


   h_SegmentDeltaPhiLocal3_posMu_posEnd
   		= fs->make<TH1F>("h_SegmentDeltaPhiLocal3_posMu_posEnd","h_SegmentDeltaPhiLocal3_posMu_posEnd", 2600000, -6.5, 6.5);
   h_SegmentDeltaPhiLocal3_posMu_negEnd	        
   		= fs->make<TH1F>("h_SegmentDeltaPhiLocal3_posMu_negEnd","h_SegmentDeltaPhiLocal3_posMu_negEnd", 2600000, -6.5, 6.5);
   h_SegmentDeltaPhiLocal3_negMu_posEnd
   		= fs->make<TH1F>("h_SegmentDeltaPhiLocal3_negMu_posEnd","h_SegmentDeltaPhiLocal3_negMu_posEnd", 2600000, -6.5, 6.5);
   h_SegmentDeltaPhiLocal3_negMu_negEnd	        
   		= fs->make<TH1F>("h_SegmentDeltaPhiLocal3_negMu_negEnd","h_SegmentDeltaPhiLocal3_negMu_negEnd", 2600000, -6.5, 6.5);

   h_SimHitDeltaPhiLocal3_posMu_posEnd
   		= fs->make<TH1F>("h_SimHitDeltaPhiLocal3_posMu_posEnd","h_SimHitDeltaPhiLocal3_posMu_posEnd", 2600000, -6.5, 6.5);
   h_SimHitDeltaPhiLocal3_posMu_negEnd	        
   		= fs->make<TH1F>("h_SimHitDeltaPhiLocal3_posMu_negEnd","h_SimHitDeltaPhiLocal3_posMu_negEnd", 2600000, -6.5, 6.5);
   h_SimHitDeltaPhiLocal3_negMu_posEnd
   		= fs->make<TH1F>("h_SimHitDeltaPhiLocal3_negMu_posEnd","h_SimHitDeltaPhiLocal3_negMu_posEnd", 2600000, -6.5, 6.5);
   h_SimHitDeltaPhiLocal3_negMu_negEnd	        
   		= fs->make<TH1F>("h_SimHitDeltaPhiLocal3_negMu_negEnd","h_SimHitDeltaPhiLocal3_negMu_negEnd", 2600000, -6.5, 6.5);

   h_deltaLocalX_pos		= fs->make<TH1F>("h_deltaLocalX_pos","h_deltaLocalX_pos", 1000, -100, 100);
   h_deltaLocalX_neg            = fs->make<TH1F>("h_deltaLocalX_neg","h_deltaLocalX_neg", 1000, -100, 100);
   
   h_wDistX_recHitSegment_posEnd            = fs->make<TH1F>("h_wDistX_recHitSegment_pos","h_wDistX_recHitSegment_pos", 10000, -500, 500);
   h_wDistY_recHitSegment_posEnd            = fs->make<TH1F>("h_wDistY_recHitSegment_pos","h_wDistY_recHitSegment_pos", 10000, -500, 500);

   tr2 = fs->make<TTree>("Event", "");
   
   tr2->Branch("nEvent"     ,	&nEvent     , 	"nEvent"     );
   tr2->Branch("nSeg_posMu_posEnd"     ,	&nSeg_posMu_posEnd     , 	"nSeg_posMu_posEnd"     );
   tr2->Branch("nSeg_posMu_negEnd"     ,	&nSeg_posMu_negEnd     , 	"nSeg_posMu_negEnd"     );
   tr2->Branch("nSeg_negMu_posEnd"     ,	&nSeg_negMu_posEnd     , 	"nSeg_negMu_posEnd"     );
   tr2->Branch("nSeg_negMu_negEnd"     ,	&nSeg_negMu_negEnd     , 	"nSeg_negMu_negEnd"     );
   tr2->Branch("nSegmentRecHits_posMu_posEnd"     ,	&nSegmentRecHits_posMu_posEnd     , 	"nSegmentRecHits_posMu_posEnd"     );
   tr2->Branch("nSegmentRecHits_posMu_negEnd"     ,	&nSegmentRecHits_posMu_negEnd     , 	"nSegmentRecHits_posMu_negEnd"     );
   tr2->Branch("nSegmentRecHits_negMu_posEnd"     ,	&nSegmentRecHits_negMu_posEnd     , 	"nSegmentRecHits_negMu_posEnd"     );
   tr2->Branch("nSegmentRecHits_negMu_negEnd"     ,	&nSegmentRecHits_negMu_negEnd     , 	"nSegmentRecHits_negMu_negEnd"     );
   
   tr2->Branch("nearbyChamber_posMu_posEnd"     ,	&nearbyChamber_posMu_posEnd     , 	"nearbyChamber_posMu_posEnd/I"     );
   tr2->Branch("nearbyChamber_posMu_negEnd"     ,	&nearbyChamber_posMu_negEnd     , 	"nearbyChamber_posMu_negEnd/I"     );
   tr2->Branch("nearbyChamber_negMu_negEnd"     ,	&nearbyChamber_negMu_negEnd     , 	"nearbyChamber_negMu_negEnd/I"     );
   tr2->Branch("nearbyChamber_negMu_posEnd"     ,	&nearbyChamber_negMu_posEnd     , 	"nearbyChamber_negMu_posEnd/I"     );

   tr2->Branch("SegmentParameters_posMu_posEnd"     ,	&segmentPars_posMu_posEnd[0]     , 	"slopeX:slopeY:X0:Y0:chisquare:DoF:redChisquare:localDirectionX:localDirectionY:localDirectionZ"     );
   tr2->Branch("SegmentParameters_negMu_posEnd"     ,	&segmentPars_negMu_posEnd[0]     , 	"slopeX:slopeY:X0:Y0:chisquare:DoF:redChisquare:localDirectionX:localDirectionY:localDirectionZ"     );
   tr2->Branch("SegmentParameters_posMu_negEnd"     ,	&segmentPars_posMu_negEnd[0]     , 	"slopeX:slopeY:X0:Y0:chisquare:DoF:redChisquare:localDirectionX:localDirectionY:localDirectionZ"     );
   tr2->Branch("SegmentParameters_negMu_negEnd"     ,	&segmentPars_negMu_negEnd[0]     , 	"slopeX:slopeY:X0:Y0:chisquare:DoF:redChisquare:localDirectionX:localDirectionY:localDirectionZ"     );

   tr2->Branch("arr_NsimHit_posMu"     ,	&arr_NsimHit_posMu[0]     , 	"layer1:layer2:layer3:layer4:layer5:layer6"     );
   tr2->Branch("arr_NsimHit_negMu"     ,	&arr_NsimHit_negMu[0]     , 	"layer1:layer2:layer3:layer4:layer5:layer6"     );

   tr2->Branch("eta_pos"     ,	&eta_pos     , 	"eta_pos"     );
   tr2->Branch("eta_neg"     ,	&eta_neg     , 	"eta_neg"     );
   tr2->Branch("p_pos"     ,	&p_pos     , 	"p_pos"     );
   tr2->Branch("p_neg"     ,	&p_neg     , 	"p_neg"     );
   tr2->Branch("pt_pos"     ,	&pt_pos     , 	"pt_pos"     );
   tr2->Branch("pt_neg"     ,	&pt_neg     , 	"pt_neg"     );

   tr2->Branch("simHit_thetaLocal3_posMu_posEnd"     ,	&simHit_thetaLocal3_posMu_posEnd     , 	"simHit_thetaLocal3_posMu_posEnd"     );
   tr2->Branch("simHit_thetaLocal3_posMu_negEnd"     ,	&simHit_thetaLocal3_posMu_negEnd     , 	"simHit_thetaLocal3_posMu_negEnd"     );
   tr2->Branch("simHit_thetaLocal3_negMu_posEnd"     ,	&simHit_thetaLocal3_negMu_posEnd     , 	"simHit_thetaLocal3_negMu_posEnd"     );
   tr2->Branch("simHit_thetaLocal3_negMu_negEnd"     ,	&simHit_thetaLocal3_negMu_negEnd     , 	"simHit_thetaLocal3_negMu_negEnd"     );
   tr2->Branch("simHit_alphaLocal3_posMu_posEnd"     ,	&simHit_alphaLocal3_posMu_posEnd     , 	"simHit_alphaLocal3_posMu_posEnd"     );
   tr2->Branch("simHit_alphaLocal3_posMu_negEnd"     ,	&simHit_alphaLocal3_posMu_negEnd     , 	"simHit_alphaLocal3_posMu_negEnd"     );
   tr2->Branch("simHit_alphaLocal3_negMu_posEnd"     ,	&simHit_alphaLocal3_negMu_posEnd     , 	"simHit_alphaLocal3_negMu_posEnd"     );
   tr2->Branch("simHit_alphaLocal3_negMu_negEnd"     ,	&simHit_alphaLocal3_negMu_negEnd     , 	"simHit_alphaLocal3_negMu_negEnd"     );
   tr2->Branch("simHitDeltaPhiLocal3_posMu_negEnd"	, &simHitDeltaPhiLocal3_posMu_negEnd	, "simHitDeltaPhiLocal3_posMu_negEnd"	);
   tr2->Branch("simHitDeltaPhiLocal3_posMu_posEnd"	, &simHitDeltaPhiLocal3_posMu_posEnd	, "simHitDeltaPhiLocal3_posMu_posEnd"	);
   tr2->Branch("simHitDeltaPhiLocal3_negMu_negEnd"	, &simHitDeltaPhiLocal3_negMu_negEnd	, "simHitDeltaPhiLocal3_negMu_negEnd"	);
   tr2->Branch("simHitDeltaPhiLocal3_negMu_posEnd"	, &simHitDeltaPhiLocal3_negMu_posEnd	, "simHitDeltaPhiLocal3_negMu_posEnd"	);
   tr2->Branch("segment_thetaLocal3_posMu_posEnd"     ,	&segment_thetaLocal3_posMu_posEnd     , 	"segment_thetaLocal3_posMu_posEnd"     );
   tr2->Branch("segment_thetaLocal3_posMu_negEnd"     ,	&segment_thetaLocal3_posMu_negEnd     , 	"segment_thetaLocal3_posMu_negEnd"     );
   tr2->Branch("segment_thetaLocal3_negMu_posEnd"     ,	&segment_thetaLocal3_negMu_posEnd     , 	"segment_thetaLocal3_negMu_posEnd"     );
   tr2->Branch("segment_thetaLocal3_negMu_negEnd"     ,	&segment_thetaLocal3_negMu_negEnd     , 	"segment_thetaLocal3_negMu_negEnd"     );
   tr2->Branch("segment_alphaLocal3_posMu_posEnd"     ,	&segment_alphaLocal3_posMu_posEnd     , 	"segment_alphaLocal3_posMu_posEnd"     );
   tr2->Branch("segment_alphaLocal3_posMu_negEnd"     ,	&segment_alphaLocal3_posMu_negEnd     , 	"segment_alphaLocal3_posMu_negEnd"     );
   tr2->Branch("segment_alphaLocal3_negMu_posEnd"     ,	&segment_alphaLocal3_negMu_posEnd     , 	"segment_alphaLocal3_negMu_posEnd"     );
   tr2->Branch("segment_alphaLocal3_negMu_negEnd"     ,	&segment_alphaLocal3_negMu_negEnd     , 	"segment_alphaLocal3_negMu_negEnd"     );
   tr2->Branch("segmentDeltaPhiLocal3_posMu_negEnd"	, &segmentDeltaPhiLocal3_posMu_negEnd	, "segmentDeltaPhiLocal3_posMu_negEnd"	);
   tr2->Branch("segmentDeltaPhiLocal3_posMu_posEnd"	, &segmentDeltaPhiLocal3_posMu_posEnd	, "segmentDeltaPhiLocal3_posMu_posEnd"	);
   tr2->Branch("segmentDeltaPhiLocal3_negMu_negEnd"	, &segmentDeltaPhiLocal3_negMu_negEnd	, "segmentDeltaPhiLocal3_negMu_negEnd"	);
   tr2->Branch("segmentDeltaPhiLocal3_negMu_posEnd"	, &segmentDeltaPhiLocal3_negMu_posEnd	, "segmentDeltaPhiLocal3_negMu_posEnd"	);
   tr2->Branch("segmentDeltaGlobalPhi"	, &segmentDeltaGlobalPhi	, "segmentDeltaGlobalPhi"	);
   tr2->Branch("segmentLayer1GlobalPhi"	, &segmentLayer1GlobalPhi	, "segmentLayer1GlobalPhi"	);
   tr2->Branch("segmentLayer6GlobalPhi"	, &segmentLayer6GlobalPhi	, "segmentLayer6GlobalPhi"	);
   tr2->Branch("segmentDeltaGlobalPhi_posMu_posEnd"	, &segmentDeltaGlobalPhi_posMu_posEnd	, "segmentDeltaGlobalPhi_posMu_posEnd"	);
   tr2->Branch("segmentLayer1GlobalPhi_posMu_posEnd"	, &segmentLayer1GlobalPhi_posMu_posEnd	, "segmentLayer1GlobalPhi_posMu_posEnd"	);
   tr2->Branch("segmentLayer6GlobalPhi_posMu_posEnd"	, &segmentLayer6GlobalPhi_posMu_posEnd	, "segmentLayer6GlobalPhi_posMu_posEnd"	);

   //tr2->Branch("ratio_posMu_posEnd"     ,	&ratio_posMu_posEnd     , 	"ratio_posMu_posEnd"     );
   //tr2->Branch("ratio_posMu_negEnd"     ,	&ratio_posMu_negEnd     , 	"ratio_posMu_negEnd"     );
   //tr2->Branch("ratio_negMu_posEnd"     ,	&ratio_negMu_posEnd     , 	"ratio_negMu_posEnd"     );
   //tr2->Branch("ratio_negMu_negEnd"     ,	&ratio_negMu_negEnd     , 	"ratio_negMu_negEnd"     );
}





ME0SegmentAnalyzer::~ME0SegmentAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
ME0SegmentAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   using namespace std;

   nEvent = iEvent.id().event();

   ESHandle<ME0Geometry> me0Geom;
   iSetup.get<MuonGeometryRecord>().get(me0Geom);


   Handle<MuonDigiCollection<ME0DetId,ME0Digi>> me0digiH;
   Handle<MuonDigiCollection<ME0DetId,ME0PadDigi>> me0padDigiH;
   Handle<MuonDigiCollection<ME0DetId,ME0PadDigiCluster>> me0padDigiClusterH;
   Handle<ME0RecHitCollection> me0recHitH;
   Handle<vector<PSimHit>> me0simHitH;
   Handle<ME0SegmentCollection> me0segmentH;
   iEvent.getByToken(ME0DigiToken_, me0digiH);
   iEvent.getByToken(ME0PadDigiToken_, me0padDigiH);
   iEvent.getByToken(ME0PadDigiClusterToken_, me0padDigiClusterH);
   iEvent.getByToken(ME0RecHitToken_, me0recHitH);
   iEvent.getByToken(ME0SimHitToken_, me0simHitH);
   iEvent.getByToken(ME0SegmentToken_, me0segmentH);


   map<ME0DetId,vector<LocalPoint>> 	digiMap;
   map<ME0DetId,vector<LocalPoint>>	recHitMap;
   map<ME0DetId,vector<LocalError>>	recHitErrMap;
   map<ME0DetId,vector<int>>		recHitFirstStripMap;
   map<ME0DetId,vector<int>>		recHitClusterSizeMap;
   map<ME0DetId,vector<Local3DPoint>>	simHitMap;
   map<ME0DetId,vector<bool>>		simHitSignMap;
   map<ME0DetId,vector<Local3DPoint>>	simHitEntryMap_posMu;
   map<ME0DetId,vector<Local3DPoint>>	simHitExitMap_posMu;
   map<ME0DetId,vector<Local3DPoint>>	simHitEntryMap_negMu;
   map<ME0DetId,vector<Local3DPoint>>	simHitExitMap_negMu;
   map<ME0DetId,bool>			muSignMap;

   digiMap.clear();
   recHitMap.clear();
   recHitErrMap.clear();
   recHitFirstStripMap.clear();
   recHitClusterSizeMap.clear();
   simHitMap.clear();
   simHitSignMap.clear();
   simHitEntryMap_posMu.clear();
   simHitExitMap_posMu.clear();
   simHitEntryMap_negMu.clear();
   simHitExitMap_negMu.clear();
   muSignMap.clear();

   //reset varbiables for tree

    p_pos = -1;
    p_neg = -1;
    pt_pos = -1;
    pt_neg = -1;
    eta_pos = -1;
    eta_neg = -1;
    std::fill(segmentPars_posMu_posEnd,segmentPars_posMu_posEnd+10,-20);
    std::fill(segmentPars_negMu_posEnd,segmentPars_negMu_posEnd+10,-20);
    std::fill(segmentPars_posMu_negEnd,segmentPars_posMu_negEnd+10,-20);
    std::fill(segmentPars_negMu_negEnd,segmentPars_negMu_negEnd+10,-20);

    std::fill(arr_NsimHit_posMu,arr_NsimHit_posMu+6,0);
    std::fill(arr_NsimHit_negMu,arr_NsimHit_negMu+6,0);

    simHit_thetaLocal3_posMu_posEnd = -4;
    simHit_thetaLocal3_posMu_negEnd = -4;
    simHit_thetaLocal3_negMu_posEnd = -4;
    simHit_thetaLocal3_negMu_negEnd = -4;
    simHit_alphaLocal3_posMu_posEnd = -4;
    simHit_alphaLocal3_posMu_negEnd = -4;
    simHit_alphaLocal3_negMu_posEnd = -4;
    simHit_alphaLocal3_negMu_negEnd = -4;
    simHitDeltaPhiLocal3_posMu_posEnd = -7;
    simHitDeltaPhiLocal3_posMu_negEnd = -7;
    simHitDeltaPhiLocal3_negMu_posEnd = -7;
    simHitDeltaPhiLocal3_negMu_negEnd = -7;
    segment_thetaLocal3_posMu_posEnd = -4;
    segment_thetaLocal3_posMu_negEnd = -4;
    segment_thetaLocal3_negMu_posEnd = -4;
    segment_thetaLocal3_negMu_negEnd = -4;
    segment_alphaLocal3_posMu_posEnd = -4;
    segment_alphaLocal3_posMu_negEnd = -4;
    segment_alphaLocal3_negMu_posEnd = -4;
    segment_alphaLocal3_negMu_negEnd = -4;
    segmentDeltaPhiLocal3_posMu_posEnd = -7;
    segmentDeltaPhiLocal3_posMu_negEnd = -7;
    segmentDeltaPhiLocal3_negMu_posEnd = -7;
    segmentDeltaPhiLocal3_negMu_negEnd = -7;
    segmentDeltaGlobalPhi = -7;
    segmentLayer1GlobalPhi = -7;
    segmentLayer6GlobalPhi = -7;
    segmentDeltaGlobalPhi_posMu_posEnd = -7;
    segmentLayer1GlobalPhi_posMu_posEnd = -7;
    segmentLayer6GlobalPhi_posMu_posEnd = -7;

    //ratio_posMu_posEnd = -1;
    //ratio_posMu_negEnd = -1;
    //ratio_negMu_posEnd = -1;
    //ratio_negMu_negEnd = -1;


//------------------ GEN PARTICLES ------------------------------
     Handle<GenParticleCollection> genParticles;
     iEvent.getByToken(genToken_, genParticles);

     for(size_t i = 0; i < genParticles->size(); ++ i) 
     {
       const GenParticle & p = (*genParticles)[i];
       if (fabs (p.pdgId()) != 13) continue;
       if ( p.pdgId() < 0 ) //mu+
         {
         pt_pos = p.pt();
	 p_pos  = p.p();
	 eta_pos = p.eta();
	 }
       if ( p.pdgId() > 0 ) //mu-
         {
         pt_neg = p.pt();
	 p_neg  = p.p();
	 eta_neg = p.eta();
	 }
     }

//------------------- DIGI --------------------------------------  
   for ( DigiContainerIterator<ME0DetId,ME0Digi> it = me0digiH->begin(); it != me0digiH->end(); ++it )
   {
    ME0DetId me0id = (*it).first;
   if ( me0id.region() == -1 ) continue;
    MuonDigiCollection<ME0DetId,ME0Digi>::const_iterator itr;
    for ( itr =((*it).second).first; itr!= ((*it).second).second; itr++ )
     {
      int strip = (*itr).strip();
      //int bx  = (*itr).bx();
      const ME0EtaPartition* roll = me0Geom->etaPartition(me0id);
      LocalPoint stripcentre = (roll->centreOfStrip(strip));
      
      digiMap[me0id].push_back(stripcentre);
      
      if(v) cout << "Strip: " << stripcentre.x() << " " << stripcentre.y() << " " << stripcentre.z() << endl;

      h_StripX->Fill(stripcentre.x());	
      h_StripY->Fill(stripcentre.y());  
      h_StripZ->Fill(stripcentre.z());	
     }
    
   }

//------------------- PAD DIGI -------------------------------------- 
if (v) cout << "** PAD **" << endl;
   for ( DigiContainerIterator<ME0DetId,ME0PadDigi> it = me0padDigiH->begin(); it != me0padDigiH->end(); ++it )
   {
    ME0DetId me0id = (*it).first;
    if ( me0id.region() == -1 ) continue;
    if (v)	cout << me0id << endl;
    MuonDigiCollection<ME0DetId,ME0PadDigi>::const_iterator itr;
    for ( itr =((*it).second).first; itr!= ((*it).second).second; itr++ )
     {
      int pad = (*itr).pad();
      int bx    = (*itr).bx();
      //pads[me0id].push_back(pad);
      if (v)	cout << "pad: " << pad << "  bx: " << bx << endl;
     }
    //me0detid_v2.push_back(me0id);
   }
/*
//------------------- PAD DIGI CLUSTER ------------------------------  
if (v) cout << "** CLUSTER **" << endl;
   for ( DigiContainerIterator<ME0DetId,ME0PadDigiCluster> it = me0padDigiClusterH->begin(); it != me0padDigiClusterH->end(); ++it )
   {
    ME0DetId me0id = (*it).first;
    if ( me0id.region() == -1 ) continue;
    if (v)	cout << me0id << endl;
    MuonDigiCollection<ME0DetId,ME0PadDigiCluster>::const_iterator itr;
    for ( itr =((*it).second).first; itr!= ((*it).second).second; itr++ )
     {
      vector<uint16_t> pads = (*itr).pads();
      //int bx    = (*itr).bx();
      //clusters[me0id].push_back(pads);
      if (v)	cout << "pads: " ;
      for ( unsigned int i=0; i<pads.size(); i++)  cout << pads[i] << " " ;
     }
   }
*/


//------------------- SIM HIT --------------------------------------  
   vector<ME0DetId> sh_me0id;
   bool muSign;
   for ( auto it = me0simHitH->begin(); it != me0simHitH->end(); ++it )
   {
   //if (v) cout << "Process Type: " << (*it).theProcessType << endl;
   if ( fabs( (*it).particleType() ) != 13 )	continue;
   if ( (*it).particleType() > 0 ) muSign = false ; else muSign = true ;
   Local3DPoint locPosSH = (*it).localPosition();
   Local3DPoint entryPoint =   (*it).entryPoint();
   Local3DPoint exitPoint  =   (*it).exitPoint();
   ME0DetId me0id = ME0DetId::DetId( (*it).detUnitId() );
   //if ( me0id.region() == -1 ) continue;
   if (v) cout << me0id << endl;
   if (v) cout << " SimHit local position:  x = " << locPosSH.x() << "  y = " << locPosSH.y() << "  z = " << locPosSH.z() << endl;
   simHitMap[me0id].push_back(locPosSH);
   simHitSignMap[me0id].push_back(muSign);
   sh_me0id.push_back( me0id );
   muSignMap[me0id] = muSign;
   ME0DetId chamber( me0id.region(), 0, me0id.chamber(), 0 ) ;//mapping also the chamber to the muSign
   muSignMap[chamber] = muSign;
   
   if ( muSign ) 
     {
     if (v) cout << "Computing SimHit DeltaPhi for POS MU" << endl;
     simHitEntryMap_posMu[me0id].push_back( entryPoint );
     simHitExitMap_posMu[me0id].push_back(  exitPoint  );
     arr_NsimHit_posMu[me0id.layer()-1] = arr_NsimHit_posMu[me0id.layer()-1]+1; 
     }
   else
     {
     if (v) cout << "Computing SimHit DeltaPhi for NEG MU" << endl;
     simHitEntryMap_negMu[me0id].push_back( entryPoint );
     simHitExitMap_negMu[me0id].push_back(  exitPoint  );
     arr_NsimHit_negMu[me0id.layer()-1] = arr_NsimHit_negMu[me0id.layer()-1]+1; 
     }
   

   if (v) cout << "Particle type: " << (*it).particleType() << endl;
   h_SimHitX->Fill(locPosSH.x());
   h_SimHitY->Fill(locPosSH.y());   
   h_SimHitZ->Fill(locPosSH.z());
   }
  

   bool regionPlayer1 = false;
   bool regionNlayer1 = false;
   ME0DetId IdregionPlayer1;
   ME0DetId IdregionNlayer1;

   if (v) cout << "Start loop over muons with one simHit on layer1" << endl;
   for ( auto itt = simHitMap.begin() ; itt != simHitMap.end() ; ++itt)
     {
     regionPlayer1 = false;
     regionNlayer1 = false;

     int l = (itt->first).layer();
     int r = (itt->first).region();

     if ( l == 1 && r ==  1 &&  (itt->second).size()==1 )	{ regionPlayer1 = true;  IdregionPlayer1 = itt->first;}
     if ( l == 1 && r == -1 &&  (itt->second).size()==1 )	{ regionNlayer1 = true;  IdregionNlayer1 = itt->first;}
    
     if (v) cout << "\nBoolean set:\nregionPlayer1=" << regionPlayer1 << "\tregionNlayer1=" << regionNlayer1 
     		 << "\nIdregionPlayer1=" << IdregionPlayer1 << "\nIdreionNlayer1" << IdregionNlayer1 << endl;
    

     if (regionPlayer1)
       {
       if (v) cout << "if (regionPlayer1)" << endl;
       const ME0EtaPartition * roll = me0Geom->etaPartition(IdregionPlayer1);	
       if (v) cout << "ME0Id = " << IdregionPlayer1 << endl;
       GlobalPoint IpGP( 0,0,0 );
       Local3DPoint IpLP(roll->toLocal(IpGP) );
       if ( simHitSignMap[IdregionPlayer1][0] )
         {
	 //for calculation using local coordinates
	 float thetaLocal3 = 0;
	 float alphaLocal3 = 0;
	 float vectorOriginX = simHitEntryMap_posMu[IdregionPlayer1][0].x()-IpLP.x();
	 float vectorOriginZ = simHitEntryMap_posMu[IdregionPlayer1][0].z()-IpLP.z();
	 if ( vectorOriginZ != 0 )	thetaLocal3 = atan( vectorOriginX / vectorOriginZ );
	 else				thetaLocal3 = ( vectorOriginX/fabs(vectorOriginX) ) * TMath::Pi()/2;
         simHit_thetaLocal3_posMu_posEnd = thetaLocal3;
         if (v) cout << "variables for SimHit local coordinates initialized." << endl;

	 float divider = 0;

	 for ( auto itt2 = simHitMap.begin() ; itt2 != simHitMap.end() ; ++itt2)
	   {
	   if ( (itt2->first).region() !=  IdregionPlayer1.region() ) continue;
	   for( unsigned int nSH = 0; nSH < (itt2->second).size() ; nSH++ )
     	     {
	     if ( simHitSignMap[itt2->first][nSH] !=  simHitSignMap[IdregionPlayer1][0] ) continue;
	     if (v) cout << "nSH = " << nSH << endl;
	     divider = divider+1.0;
	     //using LOCAL COORDINATES
	     float deltaX = (simHitExitMap_posMu[itt2->first][nSH].x()-simHitEntryMap_posMu[itt2->first][nSH].x()); 
	     float deltaZ = (simHitExitMap_posMu[itt2->first][nSH].z()-simHitEntryMap_posMu[itt2->first][nSH].z());
	     float tmp;
	     if ( deltaZ != 0) 	tmp = atan( deltaX/deltaZ );
	     else		tmp = ( deltaX/fabs(deltaX) ) * TMath::Pi()/2 ;

	     if ( deltaZ<0 )
	       {
	       if ( deltaX < 0)	
	         {
		 tmp = tmp - TMath::Pi() ;
		 }
	       else 			
	         {
		 tmp = tmp + TMath::Pi() ;
		 }
	       }

	     alphaLocal3 = alphaLocal3 + tmp ;

	     }
	   }
	 if (v) cout << "divider = " << divider << endl;
	 alphaLocal3 = alphaLocal3/divider; // divider can't be zero because there's at least one Hit on layer1


	 //Fill Histogram with alpha-theta
	 h_SimHitDeltaPhiLocal3_posMu_posEnd->Fill(alphaLocal3-thetaLocal3);
	 simHitDeltaPhiLocal3_posMu_posEnd = alphaLocal3-thetaLocal3;
	 simHit_alphaLocal3_posMu_posEnd = alphaLocal3;
	 simHit_thetaLocal3_posMu_posEnd = thetaLocal3;

	 if(v) cout << "Histograms and Tree filled." << endl;
	 }
       else
         {
	 //for calculation using local coordinates
	 float thetaLocal3 = 0;
	 float alphaLocal3 = 0;
	 float vectorOriginX = simHitEntryMap_negMu[IdregionPlayer1][0].x()-IpLP.x();
	 float vectorOriginZ = simHitEntryMap_negMu[IdregionPlayer1][0].z()-IpLP.z();
	 if ( vectorOriginZ != 0 )	thetaLocal3 = atan( vectorOriginX / vectorOriginZ );
	 else				thetaLocal3 = ( vectorOriginX/fabs(vectorOriginX) ) * TMath::Pi()/2;
         simHit_thetaLocal3_negMu_posEnd = thetaLocal3;
         if (v) cout << "variables for SimHit local coordinates initialized." << endl;

	 float divider = 0;

	 for ( auto itt2 = simHitMap.begin() ; itt2 != simHitMap.end() ; ++itt2)
	   {
	   if ( (itt2->first).region() !=  IdregionPlayer1.region() ) continue;
	   for( unsigned int nSH = 0; nSH < (itt2->second).size() ; nSH++ )
     	     {
	     if ( simHitSignMap[itt2->first][nSH] !=  simHitSignMap[IdregionPlayer1][0] ) continue;
	     if (v) cout << "nSH = " << nSH << endl;
	     divider = divider+1.0;
	     //using LOCAL COORDINATES
	     float deltaX = (simHitExitMap_negMu[itt2->first][nSH].x()-simHitEntryMap_negMu[itt2->first][nSH].x()); 
	     float deltaZ = (simHitExitMap_negMu[itt2->first][nSH].z()-simHitEntryMap_negMu[itt2->first][nSH].z());
	     float tmp;
	     if ( deltaZ != 0) 	tmp = atan( deltaX/deltaZ );
	     else		tmp = ( deltaX/fabs(deltaX) ) * TMath::Pi()/2 ;

	     if ( deltaZ<0 )
	       {
	       if ( deltaX < 0)	
	         {
		 tmp = tmp - TMath::Pi() ;
		 }
	       else 			
	         {
		 tmp = tmp + TMath::Pi() ;
		 }
	       }

	     alphaLocal3 = alphaLocal3 + tmp ;

	     }
	   }
	 if (v) cout << "divider = " << divider << endl;
	 alphaLocal3 = alphaLocal3/divider; // divider can't be zero because there's at least one Hit on layer1


	 //Fill Histogram with alpha-theta
	 h_SimHitDeltaPhiLocal3_negMu_posEnd->Fill(alphaLocal3-thetaLocal3);
	 simHitDeltaPhiLocal3_negMu_posEnd = alphaLocal3-thetaLocal3;
	 simHit_alphaLocal3_negMu_posEnd = alphaLocal3;
	 simHit_thetaLocal3_negMu_posEnd = thetaLocal3;

	 if(v) cout << "Histograms and Tree filled." << endl;
	 }
       } //regionPlayer1


     if (regionNlayer1)
       {
       if (v) cout << "if (regionNlayer1)" << endl;
       const ME0EtaPartition * roll = me0Geom->etaPartition(IdregionNlayer1);	
       if (v) cout << "ME0Id = " << IdregionNlayer1 << endl;
       GlobalPoint IpGP( 0,0,0 );
       Local3DPoint IpLP(roll->toLocal(IpGP) );
       if ( simHitSignMap[IdregionNlayer1][0] )
         {
	 //for calculation using local coordinates
	 float thetaLocal3 = 0;
	 float alphaLocal3 = 0;
	 float vectorOriginX = simHitEntryMap_posMu[IdregionNlayer1][0].x()-IpLP.x();
	 float vectorOriginZ = simHitEntryMap_posMu[IdregionNlayer1][0].z()-IpLP.z();
	 if ( vectorOriginZ != 0 )	thetaLocal3 = atan( vectorOriginX / vectorOriginZ );
	 else				thetaLocal3 = ( vectorOriginX/fabs(vectorOriginX) ) * TMath::Pi()/2;
         simHit_thetaLocal3_posMu_negEnd = thetaLocal3;
         if (v) cout << "variables for SimHit local coordinates initialized." << endl;

	 float divider = 0;

	 for ( auto itt2 = simHitMap.begin() ; itt2 != simHitMap.end() ; ++itt2)
	   {
	   if ( (itt2->first).region() !=  IdregionNlayer1.region() ) continue;
	   for( unsigned int nSH = 0; nSH < (itt2->second).size() ; nSH++ )
     	     {
	     if ( simHitSignMap[itt2->first][nSH] !=  simHitSignMap[IdregionNlayer1][0] ) continue;
	     if (v) cout << "nSH = " << nSH << endl;
	     divider = divider+1.0;
	     //using LOCAL COORDINATES
	     float deltaX = (simHitExitMap_posMu[itt2->first][nSH].x()-simHitEntryMap_posMu[itt2->first][nSH].x()); 
	     float deltaZ = (simHitExitMap_posMu[itt2->first][nSH].z()-simHitEntryMap_posMu[itt2->first][nSH].z());
	     float tmp;
	     if ( deltaZ != 0) 	tmp = atan( deltaX/deltaZ );
	     else		tmp = ( deltaX/fabs(deltaX) ) * TMath::Pi()/2 ;

	     if ( deltaZ<0 )
	       {
	       if ( deltaX < 0)	
	         {
		 tmp = tmp - TMath::Pi() ;
		 }
	       else 			
	         {
		 tmp = tmp + TMath::Pi() ;
		 }
	       }

	     alphaLocal3 = alphaLocal3 + tmp ;

	     }
	   }
	 if (v) cout << "divider = " << divider << endl;
	 alphaLocal3 = alphaLocal3/divider; // divider can't be zero because there's at least one Hit on layer1


	 //Fill Histogram with alpha-theta
	 h_SimHitDeltaPhiLocal3_posMu_negEnd->Fill(alphaLocal3-thetaLocal3);
	 simHitDeltaPhiLocal3_posMu_negEnd = alphaLocal3-thetaLocal3;
	 simHit_alphaLocal3_posMu_negEnd = alphaLocal3;
	 simHit_thetaLocal3_posMu_negEnd = thetaLocal3;

	 if(v) cout << "Histograms and Tree filled." << endl;
	 }
       else
         {
	 //for calculation using local coordinates
	 float thetaLocal3 = 0;
	 float alphaLocal3 = 0;
	 float vectorOriginX = simHitEntryMap_negMu[IdregionNlayer1][0].x()-IpLP.x();
	 float vectorOriginZ = simHitEntryMap_negMu[IdregionNlayer1][0].z()-IpLP.z();
	 if ( vectorOriginZ != 0 )	thetaLocal3 = atan( vectorOriginX / vectorOriginZ );
	 else				thetaLocal3 = ( vectorOriginX/fabs(vectorOriginX) ) * TMath::Pi()/2;
         simHit_thetaLocal3_negMu_negEnd = thetaLocal3;
         if (v) cout << "variables for SimHit local coordinates initialized." << endl;

	 float divider = 0;

	 for ( auto itt2 = simHitMap.begin() ; itt2 != simHitMap.end() ; ++itt2)
	   {
	   if ( (itt2->first).region() !=  IdregionNlayer1.region() ) continue;
	   for( unsigned int nSH = 0; nSH < (itt2->second).size() ; nSH++ )
     	     {
	     if ( simHitSignMap[itt2->first][nSH] !=  simHitSignMap[IdregionNlayer1][0] ) continue;
	     if (v) cout << "nSH = " << nSH << endl;
	     divider = divider+1.0;
	     //using LOCAL COORDINATES
	     float deltaX = (simHitExitMap_negMu[itt2->first][nSH].x()-simHitEntryMap_negMu[itt2->first][nSH].x()); 
	     float deltaZ = (simHitExitMap_negMu[itt2->first][nSH].z()-simHitEntryMap_negMu[itt2->first][nSH].z());
	     float tmp;
	     if ( deltaZ != 0) 	tmp = atan( deltaX/deltaZ );
	     else		tmp = ( deltaX/fabs(deltaX) ) * TMath::Pi()/2 ;

	     if ( deltaZ<0 )
	       {
	       if ( deltaX < 0)	
	         {
		 tmp = tmp - TMath::Pi() ;
		 }
	       else 			
	         {
		 tmp = tmp + TMath::Pi() ;
		 }
	       }

	     alphaLocal3 = alphaLocal3 + tmp ;

	     }
	   }
	 if (v) cout << "divider = " << divider << endl;
	 alphaLocal3 = alphaLocal3/divider; // divider can't be zero because there's at least one Hit on layer1


	 //Fill Histogram with alpha-theta
	 h_SimHitDeltaPhiLocal3_negMu_negEnd->Fill(alphaLocal3-thetaLocal3);
	 simHitDeltaPhiLocal3_negMu_negEnd = alphaLocal3-thetaLocal3;
	 simHit_alphaLocal3_negMu_negEnd = alphaLocal3;
	 simHit_thetaLocal3_negMu_negEnd = thetaLocal3;

	 if(v) cout << "Histograms and Tree filled." << endl;
	 }
       } //regionNlayer1

     }//loop over simHitMap

/*
   //find first and last layer with simhit
   int firstlayer = 999;
   int lastlayer = -999;
   ME0DetId firstMe0id, lastMe0id;
   
   if ( sh_me0id.size() == 0 ) return;
   for (unsigned int ind=0; ind<sh_me0id.size(); ++ind)
     {
     if ( (sh_me0id[ind]).layer() < firstlayer )	
       {
       firstlayer = (sh_me0id[ind]).layer() ;
       firstMe0id = sh_me0id[ind] ;
       }
     if ( (sh_me0id[ind]).layer() > lastlayer  )	
       {
       lastlayer  = (sh_me0id[ind]).layer() ;
       lastMe0id  = sh_me0id[ind] ;
       }
     }
   if (v) cout << "First Layer: " << firstlayer << "\tLast Layer: " << lastlayer << endl;
   float simhDeltaPhi = atan( ( simHitExitMap[lastMe0id][0].x() - simHitEntryMap[firstMe0id][0].x() ) / ( simHitExitMap[lastMe0id][0].z() - simHitEntryMap[firstMe0id][0].z()  ) );
   float SegmentDeltaZ = ( simHitExitMap[lastMe0id][0].z() - simHitEntryMap[firstMe0id][0].z()  ) ;
   h_SimHitDeltaPhi->Fill( simhDeltaPhi );
   h_SegmentDeltaZ->Fill( SegmentDeltaZ );

   if (v) cout << "Sim Hit DeltaPhi:  " << simhDeltaPhi << "   (uses Delta z = " << SegmentDeltaZ << " ) " << endl;
*/
   for (map<ME0DetId,vector<Local3DPoint>>::iterator map_it = simHitMap.begin(); map_it != simHitMap.end(); ++map_it)
      {
      h_nMuSimHit->Fill((map_it->second).size()); 
      if ( (map_it->second).size() > 1 )
      cout << "ERROR! " << map_it->first << " has nr. of simHits: " << (map_it->second).size() << endl;
      //for (vector<Local3DPoint>::iterator hit_it = (simHitMap[map_it->first]).begin();
      //     hit_it != (simHitMap[map_it->first]).end(); ++hit_it)
      //   {  }
      }

//------------------- REC HIT --------------------------------------  
   if (v) std::cout << "***REC HIT***" << std::endl;
     ME0RecHitCollection::const_iterator rh_it;
     map<ME0DetId,int> nRecHits;
     for ( rh_it = me0recHitH->begin(); rh_it !=me0recHitH->end(); ++rh_it)
        {
        if ( ((*rh_it).me0Id()).region() == -1 ) continue;
        LocalPoint rechit = (*rh_it).localPosition();
	LocalError rechiterr = (*rh_it).localPositionError();

        recHitMap[(*rh_it).me0Id()].push_back(rechit);
        recHitErrMap[(*rh_it).me0Id()].push_back(rechiterr);
	recHitClusterSizeMap[(*rh_it).me0Id()].push_back((*rh_it).clusterSize());
	recHitFirstStripMap[(*rh_it).me0Id()].push_back((*rh_it).firstClusterStrip());

	h_RecHitX->Fill(rechit.x());		h_RecHitErrX->Fill(rechiterr.xx());
	h_RecHitY->Fill(rechit.y());            h_RecHitErrY->Fill(rechiterr.yy());
	h_RecHitZ->Fill(rechit.z());		//rechiterr.zz() doesn't exist
        h_RecHitSize_all->Fill( (*rh_it).clusterSize() );	
        h_RecHitFirstStrip_all->Fill( (*rh_it).firstClusterStrip() );	

        if (v) cout << "RecHit: " << rechit.x() << " " << rechit.y() << " " << rechit.z() << endl;
        if (v) cout << "RecHitErr: " << rechiterr.xx() << " " << rechiterr.yy() << endl;
	if (v) cout << "ClusterSize: " << (*rh_it).clusterSize() << " First Strip: " << (*rh_it).firstClusterStrip() ;

	if (nRecHits.count((*rh_it).me0Id()) < 1) 	nRecHits[(*rh_it).me0Id()] = 1;
	else	nRecHits[(*rh_it).me0Id()]++;
	}

     for ( map<ME0DetId,vector<LocalPoint>>::iterator it_rh =recHitMap.begin(); it_rh !=recHitMap.end(); ++it_rh)
       {
        if ( simHitMap.count( it_rh->first ) > 0 )
	  {
	  // look for RecHit whose X is closest to SimHit
	  float distX = 999;
	  int index = 999;
	  int indexLoop = 0;
	  for (vector<LocalPoint>::iterator it_v = (it_rh->second).begin(); it_v != (it_rh->second).end(); ++it_v)
	    { 
	    float distTemp = fabs( (*it_v).x() - simHitMap[it_rh->first][0].x() );
	    if (  distTemp < distX ) 
	      {
	      distX = distTemp;
	      index = indexLoop;
	      }
	    indexLoop++;
	    }
	  
	  //Filling distances RecHit from SimHit
  	  h_RecHitSimHitX->Fill( (it_rh->second)[index].x() - simHitMap[it_rh->first][0].x() ); 
  	  h_RecHitSimHitY->Fill( (it_rh->second)[index].y() - simHitMap[it_rh->first][0].y() );
  	  h_RecHitSimHitZ->Fill( (it_rh->second)[index].z() - simHitMap[it_rh->first][0].z() );
  	  //if ( rechiterr.xx() < 0.2 )
  	  h_RecHitSimHitX_fracErr->Fill( ( (it_rh->second)[index].x() - simHitMap[it_rh->first][0].x() ) / sqrt(recHitErrMap[it_rh->first][index].xx()) ); 
  	  //if ( rechiterr.yy() < 0.2 )
  	  h_RecHitSimHitY_fracErr->Fill( ( (it_rh->second)[index].y() - simHitMap[it_rh->first][0].y() ) / sqrt(recHitErrMap[it_rh->first][index].yy()) ); 
	  
	  //Fill ClusterSize and FirstStrip
          h_RecHitSize_sel->Fill( recHitClusterSizeMap[it_rh->first][index] );	
          h_RecHitFirstStrip_sel->Fill( recHitFirstStripMap[it_rh->first][index] );	
	  }
	}
     
     if (v) cout << "nRecHits found:" << endl;
     for ( rh_it = me0recHitH->begin(); rh_it !=me0recHitH->end(); ++rh_it)
        {
        if (v) cout << (*rh_it).me0Id() << "\t" << nRecHits[(*rh_it).me0Id()] << endl;
        }

   if (v)
      {
      cout << "COUNTS:" << endl;
      cout << digiMap.size() << " " << recHitMap.size() << "  " << recHitErrMap.size() << endl;
      }

   if (!( digiMap.size() == recHitMap.size() && digiMap.size() == recHitErrMap.size() ))
      {
      cout << "ERROR!" << endl;
      return;
      }

   if (v) cout << "CALCULATE MIN DISTANCE: " << endl;
   for (map<ME0DetId,vector<LocalPoint>>::iterator map_it = digiMap.begin(); map_it != digiMap.end(); ++map_it)
      {
      if (v) cout << map_it->first << endl;
      for (vector<LocalPoint>::iterator digi_it = (digiMap[map_it->first]).begin(); 
           digi_it != (digiMap[map_it->first]).end(); ++digi_it)
         {
	 if (v) cout << "Digi: " << (*digi_it).x() << " " << (*digi_it).y() << " " << (*digi_it).z() << endl;
         float valueX = 99999;
         float valueY = 99999;
         float valueZ = 99999;
         float valueXfrac = 99999;
         float valueYfrac = 99999;
	 float valueXtmp = 99999;
	 float valueYtmp = 99999;
	 float valueZtmp = 99999;

         vector<LocalError>::iterator err_it = (recHitErrMap[map_it->first]).begin();
         for (vector<LocalPoint>::iterator recHit_it = (recHitMap[map_it->first]).begin(); 
	      recHit_it != (recHitMap[map_it->first]).end(); ++recHit_it, ++err_it)
	   {
	   if (v) cout << "RecHit size: " << recHitMap[map_it->first].size() << endl;
	   // x axis
	   valueXtmp = ( (*recHit_it).x()- (*digi_it).x() ) / sqrt( (*err_it).xx() );
	   cout << "valueXtmp: " << valueXtmp << endl;
           if (fabs(valueXtmp) < fabs(valueX))	
	     {
	     valueXfrac = valueXtmp;
	     valueX = (*recHit_it).x()- (*digi_it).x() ;
	     }

  	   // y axis
	   valueYtmp = ( (*recHit_it).y()- (*digi_it).y() ) / sqrt ( (*err_it).yy() );
	   cout << "valueYtmp: " << valueYtmp << endl;
           if (fabs(valueYtmp) < fabs(valueY))	
	     {
	     valueYfrac = valueYtmp;
	     valueY = (*recHit_it).y()- (*digi_it).y() ;
	     }
	   
	   // z axis
	   valueZtmp = ( (*recHit_it).z()- (*digi_it).z() ); 
	   cout << "valueZtmp: " << valueZtmp << endl;
           if (fabs(valueZtmp) < fabs(valueZ))	valueZ = valueZtmp;
        
	   if (v) cout << "RecHit: " << (*recHit_it).x() << " " << (*recHit_it).y() << (*recHit_it).z() << endl;
	   if (v) cout << "RecHitErr: " << (*err_it).xx() << " " << (*err_it).yy() << endl;
	   }

	h_RecoDigiX_fracErr->Fill(valueXfrac);
        h_RecoDigiY_fracErr->Fill(valueYfrac);
        h_RecoDigiX->Fill(valueX);
        h_RecoDigiY->Fill(valueY);
        h_RecoDigiZ->Fill(valueZ);
      }
   }


//------------------- SEGMENT --------------------------------------  
     if (v) std::cout << "***SEGMENT***" << std::endl;
     ME0SegmentCollection::const_iterator sg_it;

     nSeg_posMu_posEnd = 0;
     nSeg_posMu_negEnd = 0;
     nSeg_negMu_posEnd = 0;
     nSeg_negMu_negEnd = 0;
     nSegmentRecHits_posMu_posEnd = 0; 
     nSegmentRecHits_posMu_negEnd = 0; 
     nSegmentRecHits_negMu_posEnd = 0; 
     nSegmentRecHits_negMu_negEnd = 0; 
     nearbyChamber_posMu_posEnd = 0; 
     nearbyChamber_posMu_negEnd = 0; 
     nearbyChamber_negMu_posEnd = 0; 
     nearbyChamber_negMu_negEnd = 0; 
     for ( sg_it = me0segmentH->begin(); sg_it !=me0segmentH->end(); ++sg_it) //loop only to count number of segments in the pos/neg endcap
       {
       ME0DetId segmMe0Id = (*sg_it).me0DetId();
       int endcap = segmMe0Id.region();
       int segm_chamber = segmMe0Id.chamber();
       ME0DetId  mapId ;

       bool skipSegment = true;
       for ( auto signIt=simHitSignMap.begin() ; signIt!=simHitSignMap.end() ; ++signIt)
         {
         int map_endcap = (signIt->first).region();
         int map_chamber = (signIt->first).chamber();
	 if ( (map_endcap == endcap) && (map_chamber==segm_chamber) )	
	   { 
	   mapId = signIt->first; 
	   skipSegment = false;
	   break; 
	   }
	 }

       if (skipSegment)  continue;
       muSign = simHitSignMap[mapId][0];
       if ( endcap>0 &&  muSign )  nSeg_posMu_posEnd = nSeg_posMu_posEnd+1;
       if ( endcap>0 && !muSign )  nSeg_negMu_posEnd = nSeg_negMu_posEnd+1;
       if ( endcap<0 &&  muSign )  nSeg_posMu_negEnd = nSeg_posMu_negEnd+1;
       if ( endcap<0 && !muSign )  nSeg_negMu_negEnd = nSeg_negMu_negEnd+1;
       }

    // map<ME0DetId,float> nRecHits;
     for ( sg_it = me0segmentH->begin(); sg_it !=me0segmentH->end(); ++sg_it)
        {
	ME0DetId segmMe0Id = (*sg_it).me0DetId();
	int endcap = segmMe0Id.region();
        int segm_chamber = segmMe0Id.chamber();
        ME0DetId  mapId ;
        bool skipSegment = true;

        for ( auto signIt=simHitSignMap.begin() ; signIt!=simHitSignMap.end() ; ++signIt)
          {
          int map_endcap = (signIt->first).region();
          int map_chamber = (signIt->first).chamber();
 	  if ( (map_endcap == endcap) && (map_chamber==segm_chamber) )
	    { 
	    mapId = signIt->first; 
	    skipSegment = false;
	    break; 
	    }
 	  }
        if (skipSegment)  continue;
	muSign = simHitSignMap[ mapId ][0];
	//if ( endcap>0 && nSeg_posEnd>1 ) continue; //consider only events with 1 segment
	//if ( endcap<0 && nSeg_negEnd>1 ) continue; //consider only events with 1 segment

        const std::vector<ME0RecHit>& segmRHits = (*sg_it).specificRecHits();

	int nearbyChamber = 0;
	for ( auto srh=segmRHits.begin(); srh!=segmRHits.end(); ++srh )
	  {
	  ME0DetId srh_detId = (*srh).me0Id();
	  if ( srh_detId.chamber() != ((*sg_it).me0DetId()).chamber() )	nearbyChamber++;
	  if (v) cout << "looking for nearby chambers: " << srh_detId.chamber() << "\n" << ((*sg_it).me0DetId()).chamber() << "\nnearbyChamber=" << nearbyChamber << endl;
	  }

	if ( endcap>0 &&  muSign )	{ nSegmentRecHits_posMu_posEnd = segmRHits.size() ; nearbyChamber_posMu_posEnd = nearbyChamber; }
	if ( endcap>0 && !muSign )	{ nSegmentRecHits_negMu_posEnd = segmRHits.size() ; nearbyChamber_negMu_posEnd = nearbyChamber; }
	if ( endcap<0 &&  muSign )	{ nSegmentRecHits_posMu_negEnd = segmRHits.size() ; nearbyChamber_posMu_negEnd = nearbyChamber; }
	if ( endcap<0 && !muSign )	{ nSegmentRecHits_negMu_negEnd = segmRHits.size() ; nearbyChamber_negMu_negEnd = nearbyChamber; }

        //if ( ((*sg_it).me0DetId()).region() == -1 ) continue;
	LocalVector theLocalDirection = (*sg_it).localDirection();
	LocalPoint theOrigin = (*sg_it).localPosition();

	AlgebraicVector par(4);

        if(theLocalDirection.z() != 0)
         {
         par[0] = theLocalDirection.x()/theLocalDirection.z();
   	 par[1] = theLocalDirection.y()/theLocalDirection.z();    
	 }
	 par[2] = theOrigin.x();
	 par[3] = theOrigin.y();


        if (v) cout << "PARAMETERS: " << par[0] << "  " << par[1] << "  " << par[2] << "  " << par[3] << endl;
        if (v) cout << "ORIGIN:  phi " << theOrigin.phi() << endl;
        if (v) cout << "ORIGIN:  z " << theOrigin.z() << endl;

        //Get all coordinates
	ME0DetId layer1( ((*sg_it).me0DetId()).region(), 1, ((*sg_it).me0DetId()).chamber(), 4 ) ; //use only for x,z coordinates!
        const ME0EtaPartition * rollLayer1 = me0Geom->etaPartition(layer1);	
	// if (v)  cout << "Segment ME0ID: " << (*sg_it).me0DetId()  << endl;
	const ME0Chamber * chamber = me0Geom->chamber( (*sg_it).me0DetId() );
	Local3DPoint OriginLP( par[2], par[3], theOrigin.z() );
	Local3DPoint centerLP( 0,0,0 ); //center of chamber
	GlobalPoint  centerGP = chamber->toGlobal( centerLP );
	Local3DPoint centerFromL1 = rollLayer1->toLocal( centerGP ); //use only for x,z coordinates!
	Local3DPoint theEntryLP (par[2] - (centerFromL1.z()*par[0]) , par[3] - (centerFromL1.z()*par[1]) , -centerFromL1.z());//in chamber frame
	Local3DPoint theEnd     (par[2] + (centerFromL1.z()*par[0]) , par[3] + (centerFromL1.z()*par[1]) ,  centerFromL1.z());//in chamber frame
	GlobalPoint theEntryGP	= chamber->toGlobal(theEntryLP);
	GlobalPoint theEndGP	= chamber->toGlobal(theEnd);

	GlobalPoint  IpGP( 0,0,0 );
        Local3DPoint IpLP(chamber->toLocal(IpGP) );

	//Calculation in LOCAL coordinates
	float vectorOriginX = theEntryLP.x()- IpLP.x();
	float vectorOriginZ = theEntryLP.z()- IpLP.z();
	float segmentDeltaX = theEnd.x()    - theEntryLP.x();
	float segmentDeltaY = theEnd.y()    - theEntryLP.y();
	float segmentDeltaZ = theEnd.z()    - theEntryLP.z();
	if (v) cout << "IP local coord:  " << IpLP.x() << ", " << IpLP.y() << ", " << IpLP.z() << endl;
	if (v) cout << "theEntry local coord:  " << theEntryLP.x() << ", " << theEntryLP.y() << ", " << theEntryLP.z() << endl;
	if (v) cout << "segmentDeltaX, segmentDeltaY(local coord):  " << segmentDeltaX << ", " << segmentDeltaY << ", " << segmentDeltaZ << endl;
        //float theta = atan( vectorOriginX / vectorOriginZ  ) ;  //angle of vectorOrigin w.r.t X axis
	//float alpha = atan( par[0] / 1  );  //angle of segment direction w.r.t. X axis

	//Calculation in LOCAL coordinates v3
        float theta3;
        float alpha3;
        if ( vectorOriginZ != 0 )	theta3 = atan( vectorOriginX / vectorOriginZ  ) ;
	else				theta3 = ( vectorOriginX/fabs(vectorOriginX) ) * TMath::Pi()/2 ;
	if (segmentDeltaZ != 0)		alpha3 = atan( segmentDeltaX / segmentDeltaZ  );
	else				alpha3 = ( segmentDeltaX/fabs(segmentDeltaX) ) * TMath::Pi()/2 ;
	if ( segmentDeltaZ<0 )
	  {
	  if (segmentDeltaX < 0)	
	    {
	    alpha3 = alpha3 - TMath::Pi() ;
	    }
	  else 			
	    {
	    alpha3 = alpha3 + TMath::Pi() ;
	    }
	  }
	float sgDeltaPhiLocal3 = alpha3-theta3;

        segmentDeltaGlobalPhi = theEndGP.phi() - theEntryGP.phi();
	segmentLayer1GlobalPhi = theEntryGP.phi();
	segmentLayer6GlobalPhi = theEndGP.phi();

 	//Saving results
	if ( muSign )
	 {
	 if ( segmMe0Id.region() > 0 )
	   {
	   if ( nSeg_posMu_posEnd==1 ) 	h_SegmentDeltaPhiLocal3_posMu_posEnd->Fill( sgDeltaPhiLocal3 ); 
	   segment_alphaLocal3_posMu_posEnd = alpha3;
	   segment_thetaLocal3_posMu_posEnd = theta3;
	   segmentDeltaPhiLocal3_posMu_posEnd = sgDeltaPhiLocal3;
           segmentDeltaGlobalPhi_posMu_posEnd = theEndGP.phi() - theEntryGP.phi();
	   segmentLayer1GlobalPhi_posMu_posEnd = theEntryGP.phi();
	   segmentLayer6GlobalPhi_posMu_posEnd = theEndGP.phi();
	   }
	 if ( segmMe0Id.region() < 0 )
	   {
	   if ( nSeg_posMu_negEnd==1 ) 	h_SegmentDeltaPhiLocal3_posMu_negEnd->Fill( sgDeltaPhiLocal3 );
	   segment_alphaLocal3_posMu_negEnd = alpha3;
	   segment_thetaLocal3_posMu_negEnd = theta3;
	   segmentDeltaPhiLocal3_posMu_negEnd = sgDeltaPhiLocal3;
	   }
         }
	else
	 {
	 if ( segmMe0Id.region() > 0 )
	   {
	   if ( nSeg_negMu_posEnd==1 ) 	h_SegmentDeltaPhiLocal3_negMu_posEnd->Fill( sgDeltaPhiLocal3 ); 
	   segment_alphaLocal3_negMu_posEnd = alpha3;
	   segment_thetaLocal3_negMu_posEnd = theta3;
	   segmentDeltaPhiLocal3_negMu_posEnd = sgDeltaPhiLocal3;
	   }
	 if ( segmMe0Id.region() < 0 )
	   {
	   if ( nSeg_negMu_negEnd==1 ) 	h_SegmentDeltaPhiLocal3_negMu_negEnd->Fill( sgDeltaPhiLocal3 );
	   segment_alphaLocal3_negMu_negEnd = alpha3;
	   segment_thetaLocal3_negMu_negEnd = theta3;
	   segmentDeltaPhiLocal3_negMu_negEnd = sgDeltaPhiLocal3;
	   }
         }



	 //Saving segment parameters
	 if (endcap > 0)
	   {
	   if ( muSign )
	     {
	     for (int i=0; i<4; i++)  segmentPars_posMu_posEnd[i] = par[i];
	     segmentPars_posMu_posEnd[4] = (*sg_it).chi2();
	     segmentPars_posMu_posEnd[5] = (*sg_it).degreesOfFreedom();
	     segmentPars_posMu_posEnd[6] = segmentPars_posMu_posEnd[4] / segmentPars_posMu_posEnd[5] ;
	     segmentPars_posMu_posEnd[7] = theLocalDirection.x();
	     segmentPars_posMu_posEnd[8] = theLocalDirection.y();
	     segmentPars_posMu_posEnd[9] = theLocalDirection.z();
	     }
	   else
	     {
	     for (int i=0; i<4; i++)  segmentPars_negMu_posEnd[i] = par[i];
	     segmentPars_negMu_posEnd[4] = (*sg_it).chi2();
	     segmentPars_negMu_posEnd[5] = (*sg_it).degreesOfFreedom();
	     segmentPars_negMu_posEnd[6] = segmentPars_negMu_posEnd[4] / segmentPars_negMu_posEnd[5] ;
	     segmentPars_negMu_posEnd[7] = theLocalDirection.x();
	     segmentPars_negMu_posEnd[8] = theLocalDirection.y();
	     segmentPars_negMu_posEnd[9] = theLocalDirection.z();
	     }
	   }
	 if (endcap < 0)
	   {
	   if ( muSign )
	     {
	     for (int i=0; i<4; i++)  segmentPars_posMu_negEnd[i] = par[i];
	     segmentPars_posMu_negEnd[4] = (*sg_it).chi2();
	     segmentPars_posMu_negEnd[5] = (*sg_it).degreesOfFreedom();
	     segmentPars_posMu_negEnd[6] = segmentPars_posMu_negEnd[4] / segmentPars_posMu_negEnd[5] ;
	     segmentPars_posMu_negEnd[7] = theLocalDirection.x();
	     segmentPars_posMu_negEnd[8] = theLocalDirection.y();
	     segmentPars_posMu_negEnd[9] = theLocalDirection.z();
	     }
	   else
	     {
	     for (int i=0; i<4; i++)  segmentPars_negMu_negEnd[i] = par[i];
	     segmentPars_negMu_negEnd[4] = (*sg_it).chi2();
	     segmentPars_negMu_negEnd[5] = (*sg_it).degreesOfFreedom();
	     segmentPars_negMu_negEnd[6] = segmentPars_negMu_negEnd[4] / segmentPars_negMu_negEnd[5] ;
	     segmentPars_negMu_negEnd[7] = theLocalDirection.x();
	     segmentPars_negMu_negEnd[8] = theLocalDirection.y();
	     segmentPars_negMu_negEnd[9] = theLocalDirection.z();
	     }
	   }

	// RELATION between results using LOCAL and GLOBAL variables
	// say Theta1 and Theta2 are in local and global coordinates respectively.
	// a is the length of the segment,
	// b,c are the sides of triangle that has the segment as one side and one vertex in the origin of local coordinates,
	// B,C are the sides of triangle that has the segment as one side and one vertex in the origin of global coordinates,
	// Then we expect that: (from Carnot theorem)
	//
	// cos(Theta1) / cos(Theta2)  =  ( (b^2 + c^2 - a^2) BC ) / ( (B^2 + C^2 - a^2) bc )
	//
/*
	float a2 =  (segmentDeltaX*segmentDeltaX) 	+ (segmentDeltaY*segmentDeltaY)		 ;
	float b2 =  (theEndLP1.x()*theEndLP1.x())      	+ (theEndLP1.y()*theEndLP1.y())	      	 ;
	float c2 =  (theEntryLP1.x()*theEntryLP1.x())	+ (theEntryLP1.y()*theEntryLP1.y())	 ;
	float A2 =  ( (theEndGP.x()-theEntryGP.x())*(theEndGP.x()-theEntryGP.x()) ) +
			( (theEndGP.y()-theEntryGP.y())*(theEndGP.y()-theEntryGP.y()) ) ;
	float B2 =  (theEndGP.x()*theEndGP.x())	+ (theEndGP.y()*theEndGP.y())	       	 ;
	float C2 =  (theEntryGP.x()*theEntryGP.x())	+ (theEntryGP.y()*theEntryGP.y())	 ;

	if (v)
	  {
	  cout << "a,b,c = " << sqrt(a2) << ", " << sqrt(b2) << ", " << sqrt(c2) << endl;
	  cout << "A,B,C = " << sqrt(A2) << ", " << sqrt(B2) << ", " << sqrt(C2) << endl;
	  }
	
	float ratio = ( ( b2 + c2 - a2 ) * sqrt(B2) * sqrt(C2) )  /  ( ( B2 + C2 - A2 ) * sqrt(b2) * sqrt(c2) )	;

	if ( muSign )
	 {
	 if ( segmMe0Id.region() > 0 )	ratio_posMu_posEnd = ratio;
	 else				ratio_posMu_negEnd = ratio;
	 }
 	else
	 {
	 if ( segmMe0Id.region() > 0 )	ratio_negMu_posEnd = ratio;
	 else				ratio_negMu_negEnd = ratio;
	 }
	  
	// Distance of RecHits from segment
	// using coordinates in the chamber frame
	if (segmMe0Id.region() == 1)
	  {
	  if (v) cout << "\nComputing (xHit-xSegm)/errX and (yHit-ySegm)/errY.\nx\ty\terrX\terrY\t\t(distX/errX)\t(distY/errY)" << endl;
	  //for ( auto rhitIt = recHitMap.begin(); rhitIt != recHitMap.end(); ++rhitIt )
	  const std::vector<ME0RecHit>& segmRHits = (*sg_it).specificRecHits();
	  if (v) cout << "This segment has " << segmRHits.size() << "RecHits." << endl;
	  for ( auto rhitIt = segmRHits.begin(); rhitIt != segmRHits.end(); ++rhitIt )
	    {
	    ME0DetId detId = (*rhitIt).me0Id();
            const ME0EtaPartition* roll = me0Geom->etaPartition(detId);
	    LocalPoint hitLP = (*rhitIt).localPosition();
	    LocalError hitError = (*rhitIt).localPositionError();

	      Global3DPoint gp = roll->toGlobal(hitLP);
	      Local3DPoint  lpCh = chamber->toLocal(gp);

	      float z = lpCh.z();    	//z position of rechit in chamber frame

	      float xHit = lpCh.x(); 	//x position of rechit in chamber frame
	      float xSegm = par[2] + (z*par[0]); //x coordinate of segment at this z
	      float errX = sqrt( hitError.xx() );

	      float yHit = lpCh.y(); 	//y position of rechit in chamber frame
	      float ySegm = par[3] + (z*par[1]); //x coordinate of segment at this z
	      float errY = sqrt( hitError.yy() );

	      float wDistX = (xHit-xSegm)/errX;
	      h_wDistX_recHitSegment_posEnd->Fill(wDistX);

	      float wDistY = (yHit-ySegm)/errY;
	      h_wDistY_recHitSegment_posEnd->Fill(wDistY);

	      if (v) cout << "in Chamber frame: x="<<xHit << "\ty=" << yHit << "\terrX=" << errX << "\terrY" << errY << "\twDistX" << wDistX << "\twDistY" << wDistY << endl;

	    }

          }
*/ 
	}




      // DEBUG
      if (v) cout << "\n\n\n===== GEOMETRY DEBUG =====\n" << endl;
      if (v) cout << "event " << iEvent.id().event() << endl;
      bool differentEtaPart = false;
      int thisEtaPart = 1;
      int nextEtaPart = 1;

      for ( auto rhitIt = recHitMap.begin(); rhitIt != recHitMap.end(); ++rhitIt )
      //note that this recHitMap and recHitErrMap contain only rec hits in the positive endcap
        {
        ME0DetId RHdetId = rhitIt->first;
	thisEtaPart = RHdetId.roll();
	auto minnie = recHitMap.end();
	minnie--;
	if (rhitIt != minnie )	
	  {
	  auto pippo = rhitIt;
	  pippo++;
	  nextEtaPart =  pippo->first.roll();
	  }
	if ( thisEtaPart != nextEtaPart )	differentEtaPart=true;
	
        for ( DigiContainerIterator<ME0DetId,ME0Digi> it = me0digiH->begin(); it != me0digiH->end(); ++it )
          {
          ME0DetId DIGIdetId = (*it).first;
	  if ( DIGIdetId != RHdetId )	continue;

          ME0SegmentCollection::const_iterator sg_it;
          for ( sg_it = me0segmentH->begin(); sg_it !=me0segmentH->end(); ++sg_it)
            {
            //if ( ((*sg_it).me0DetId()).region() == -1 ) continue;
    	    ME0DetId segmMe0Id = (*sg_it).me0DetId();
	    if ( segmMe0Id.chamber() != RHdetId.chamber() )	continue;
    	    LocalVector theLocalDirection = (*sg_it).localDirection();
    	    LocalPoint theOrigin = (*sg_it).localPosition();
    
    	    AlgebraicVector par(4);
    
            if(theLocalDirection.z() != 0)
             {
             par[0] = theLocalDirection.x()/theLocalDirection.z();
       	     par[1] = theLocalDirection.y()/theLocalDirection.z();    
    	     }
    	     par[2] = theOrigin.x();
    	     par[3] = theOrigin.y();
    
    
             const ME0EtaPartition* roll = me0Geom->etaPartition(RHdetId);
             if (v) cout << "\nDetId =\t"<< DIGIdetId << endl;
	     if (v) cout << "-- DIGIis" << endl;
	     for (unsigned int i=0; i<digiMap[DIGIdetId].size(); ++i)
	         {
		 if (v) cout << i << "in ME0DetId coord.\tx=" << digiMap[DIGIdetId][i].x() << "  y=" << digiMap[DIGIdetId][i].y() << "  z=" << digiMap[DIGIdetId][i].z() << endl;
		 Global3DPoint digiGP = roll->toGlobal(digiMap[DIGIdetId][i]);
		 if (v) cout << i << "in global coord.:\tx=" << digiGP.x() << "  y=" << digiGP.y() << "  z=" << digiGP.z() << endl;
		 }
             if (v) cout << "-- REC HITs" << endl;
             for (unsigned int i=0; i<recHitMap[RHdetId].size(); ++i)
	         {
		 if (v) cout << i << "in ME0DetId coord.\tx=" << recHitMap[RHdetId][i].x() << "  y=" << recHitMap[RHdetId][i].y() << "  z=" << recHitMap[RHdetId][i].z() << "\n"
		 	<< "\t\t\terrx=" << sqrt( recHitErrMap[RHdetId][i].xx() )  << "  erry=" << sqrt( recHitErrMap[RHdetId][i].yy() ) << endl;
		 Global3DPoint rhGP = roll->toGlobal(recHitMap[RHdetId][i]);
		 if (v) cout << i << "in global coord.:\tx=" << rhGP.x() << "  y=" << rhGP.y() << "  z=" << rhGP.z() << endl;
		 }
	     if (v) cout << "-- SEGMENT PARAMETERS" << endl;
	     if (v) cout << "dx/dz=" << par[0] << "  dy/dz=" << par[1] << "  x0=" << par[2] << "  y0" << par[3] << endl;
	     if (v) cout << "-- COORDINATES predicted by SEGMENT" << endl;
    
             vector<Local3DPoint> hitLP = rhitIt->second;
             vector<LocalError>   hitError = recHitErrMap[RHdetId];
             vector<Local3DPoint> hitLPCh; 
      
	     const ME0Chamber * chamber = me0Geom->chamber( segmMe0Id );
             for ( unsigned int i = 0; i < hitLP.size(); ++i )
               {
               Global3DPoint gp = roll->toGlobal(hitLP[i]);
               Local3DPoint  lpCh = chamber->toLocal(gp);
               hitLPCh.push_back(lpCh);
      
               float z = lpCh.z();    	//z position of rechit in chamber frame
      
               float xHit = lpCh.x(); 	//x position of rechit in chamber frame
               float xSegm = par[2] + (z*par[0]); //x coordinate of segment at this z
               float errX = sqrt( recHitErrMap[RHdetId][i].xx() );
      
               float yHit = lpCh.y(); 	//x position of rechit in chamber frame
               float ySegm = par[3] + (z*par[1]); //x coordinate of segment at this z
               float errY = sqrt( recHitErrMap[RHdetId][i].yy() );
	       
	       Local3DPoint segmLPch(xSegm, ySegm, z);
	       Global3DPoint segmGP = chamber->toGlobal(segmLPch);
	       Local3DPoint segmLP = roll->toLocal(segmGP);
	       float zGP = segmGP.z();
	       float zLP = segmLP.z(); //in ME0DetId of RH frame
	       float xSegmGP = segmGP.x();
	       float xSegmLP = segmLP.x(); //in ME0DetId of RH frame
	       float ySegmGP = segmGP.y();
	       float ySegmLP = segmLP.y(); //in ME0DetId of RH frame
     
               float wDistX = (xHit-xSegm)/errX;
               h_wDistX_recHitSegment_posEnd->Fill(wDistX);
      
               float wDistY = (yHit-ySegm)/errY;
               h_wDistY_recHitSegment_posEnd->Fill(wDistY);
      	       
	       if (v) cout << i << " in Chamber frame:\t" << "z=" << z << "  x=" << xSegm << "  y=" << ySegm << endl;
	       if (v) cout << i << " in ME0DetIt frame:\t" << "z=" << zLP << "  x=" << xSegmLP << "  y=" << ySegmLP << endl;
               if (v) cout << i << " in Global coord.:\t" << "z=" << zGP << "  x=" << xSegmGP << "  y=" << ySegmGP<< endl;
	       if (v) cout << "deltaX/errX=" << wDistX << "  deltaY/errY=" << wDistY << endl;

	       if ( v && ( fabs(wDistX) > 20) )	cout << "High deltaX/errX" << endl;
	       if ( v && ( fabs(wDistY) > 20) )	cout << "High deltaY/errY" << endl;
               }
	     }
	   }
        }
	if (differentEtaPart && v) cout << "\nEvent with different eta partitions." << endl;
	if (v) cout << "\n===== END geometry debug =====\n" << endl;
        

     //DEBUG for signMap
     for ( auto itSign = simHitSignMap.begin() ; itSign != simHitSignMap.end() ; ++itSign )
       {
       int pos_cnt = 0;
       int neg_cnt = 0;
       for ( unsigned int el=0; el<(itSign->second).size(); el++)
         {
         if ( (itSign->second)[el] )	pos_cnt++;
         if ( !(itSign->second)[el] )	neg_cnt++;
	 }
       if ( pos_cnt==0 || neg_cnt==0)	consistent_signVec++;
       else				inconsistent_signVec++;
       }

   tr2->Fill();
}


// ------------ method called once each job just before starting event loop  ------------
void 
ME0SegmentAnalyzer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
ME0SegmentAnalyzer::endJob() 
{
   tr->Fill();
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
ME0SegmentAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(ME0SegmentAnalyzer);
