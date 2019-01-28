// -*- C++ -*-
//
// Package:    ME0analyzer/DeltaGlobalPhiAnalyzer
// Class:      DeltaGlobalPhiAnalyzer
// 
/**\class DeltaGlobalPhiAnalyzer DeltaGlobalPhiAnalyzer.cc ME0analyzer/DeltaGlobalPhiAnalyzer/plugins/DeltaGlobalPhiAnalyzer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Martina Ressegotti
//         Created:  Wed, 15 Aug 2018 15:25:03 GMT
//
//


// system include files
#include <memory>

#include <iterator>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <map>
#include <bitset>
#include <algorithm>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/Utilities/interface/EDGetToken.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "Geometry/GEMGeometry/interface/ME0Geometry.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/GEMDigi/interface/ME0Digi.h"
#include "DataFormats/GEMRecHit/interface/ME0SegmentCollection.h"
#include "DataFormats/GEMRecHit/interface/ME0Segment.h"
#include "DataFormats/MuonData/interface/MuonDigiCollection.h"
#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "Geometry/GEMGeometry/interface/ME0EtaPartitionSpecs.h"
#include "Geometry/GEMGeometry/interface/ME0EtaPartition.h"
#include "Geometry/CommonTopologies/interface/StripTopology.h"
#include "DataFormats/GEMRecHit/interface/ME0RecHit.h"
#include "DataFormats/GEMRecHit/interface/ME0RecHitCollection.h"
#include "DataFormats/TrackingRecHit/interface/TrackingRecHit.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/GEMDigi/interface/ME0PadDigi.h"
#include "DataFormats/GEMDigi/interface/ME0PadDigiCluster.h"
#include "DataFormats/GEMDigi/interface/ME0PadDigiClusterCollection.h"
//#include "RecoLocalMuon/GEMSegment/plugins/GEMSegmentAlgorithm.h"
#include "SimDataFormats/TrackingHit/interface/PSimHit.h"
#include "SimDataFormats/Track/interface/SimTrackContainer.h"

//#include "typedefs.h"

#include <TTree.h>
#include <TMath.h>
#include <stdlib.h>
#include <TGraphErrors.h>
#include <TCanvas.h>
#include <TPaveText.h>
#include <TStyle.h>
#include <TLegend.h>
#include <TLatex.h>
#include <TColor.h>
#include <TGaxis.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TH3F.h>

//
// class declaration
//

using namespace edm;
using namespace std;
using namespace reco;

Int_t trans_white = TColor::GetColorTransparent(0, 0);

//canvas dimensions
  int W = 800;
  int H = 600;
  int H_ref = 600; 
  int W_ref = 800; 

  // references for T, B, L, R
  float T = 0.12*H_ref;
  float B = 0.17*H_ref; 
  float L = 0.15*W_ref;
  float R = 0.12*W_ref;

TPaveText* cmsPrel()
{

TPaveText *t1=new TPaveText(0.145,0.88,0.37,0.94,"brNDC");
  t1->AddText("CMS Preliminary");
  t1->SetTextAlign(12);
  t1->SetShadowColor(trans_white);
  t1->SetFillColor(trans_white);
  t1->SetLineColor(trans_white);
  t1->SetLineColor(trans_white);
  //t1->AddText("more text");

  return t1;
}



// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<> and also remove the line from
// constructor "usesResource("TFileService");"
// This will improve performance in multithreaded jobs.

class DeltaGlobalPhiAnalyzer : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit DeltaGlobalPhiAnalyzer(const edm::ParameterSet&);
      ~DeltaGlobalPhiAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      TCanvas * c_quality;
      TCanvas * c_eveto;
      TCanvas * c_singlemu;
      TCanvas * c_doublemu;
      TCanvas * c_triplemu;

      // ----------member data ---------------------------
      edm::EDGetTokenT<ME0SegmentCollection> ME0SegmentToken_;
      edm::EDGetTokenT<GenParticleCollection> genToken_;
      edm::EDGetTokenT<MuonDigiCollection<ME0DetId,ME0Digi>> ME0DigiToken_;
      edm::EDGetTokenT<MuonDigiCollection<ME0DetId,ME0PadDigi>> ME0PadDigiToken_;
      edm::EDGetTokenT<MuonDigiCollection<ME0DetId,ME0PadDigiCluster>> ME0PadDigiClusterToken_;
      edm::EDGetTokenT<vector<PSimHit>> ME0SimHitToken_;
      edm::EDGetTokenT<vector<SimTrack>> simTrackToken_;

      bool v = 1 ; //verbose initialization
      bool reco = 1; //reco initialization (to digi based)
      bool mutype = 1; //mutype initialization (to prompt mu)
      bool signal = 1; //signal initialization (to run on signal simulation)

      //histograms GenParticles
      TH2F * h_PtVsEta	;
      TH2F * h_Eta2D_max;
      TH2F * h_Eta2D_min;
      TH3F * h_Eta3D	;
      TH2F * h_Pt2D_max	;
      TH2F * h_Pt2D_min	;
      TH3F * h_Pt3D	;
      TH2F * h_xy_strange;
      TH2F * h_xy_strangeEP1;
      TH2F * h_xy_strangeEP2;
      TH2F * h_xy_strangeEP3;
      TH2F * h_xy_strangeEP4;
      TH2F * h_xy_strangeEP5;
      TH2F * h_xy_strangeEP6;
      TH2F * h_xy_strangeEP7;
      TH2F * h_xy_strangeEP8;
      TH1F * h_z_strange;
      TH1F * h_z_strangeEP1;
      TH1F * h_z_strangeEP2;
      TH1F * h_z_strangeEP3;
      TH1F * h_z_strangeEP4;
      TH1F * h_z_strangeEP5;
      TH1F * h_z_strangeEP6;
      TH1F * h_z_strangeEP7;
      TH1F * h_z_strangeEP8;

   std::map<ME0DetId,vector<float>> deltaPhiMap;
   std::map<ME0DetId,vector<float>> alphaMap;
   std::map<ME0DetId,vector<float>> thetaMap;
   vector<uint32_t> * me0List = new std::vector<uint32_t>(200);
   std::vector<float> * deltaPhiList = new std::vector<float>(200) ;
   std::vector<float> * xOriginList = new std::vector<float>(200) ;
   std::vector<float> * yOriginList = new std::vector<float>(200) ;
   std::vector<float> * zOriginList = new std::vector<float>(200) ;
   std::vector<float> * globalPhiOriginList = new std::vector<float>(200) ;
   std::vector<float> * globalEtaOriginList = new std::vector<float>(200) ;
   std::vector<float> * globalPhiLayer1List = new std::vector<float>(200) ;
   std::vector<float> * globalEtaLayer1List = new std::vector<float>(200) ;
   std::vector<float> * globalPhiLayer2List = new std::vector<float>(200) ;
   std::vector<float> * globalEtaLayer2List = new std::vector<float>(200) ;
   std::vector<float> * deltaGlobalPhiLayer21List = new std::vector<float>(200) ;
   std::vector<float> * averageTofList = new std::vector<float>(200) ;
   std::vector<Int_t> * etaPartList = new std::vector<Int_t>(200) ;
   std::vector<Int_t> * nSegmRecHitList = new std::vector<Int_t>(200) ;
   std::vector<Int_t> * qualityList = new std::vector<Int_t>(200) ;
   std::vector<float> * alphaList    = new std::vector<float>(200) ;
   std::vector<float> * thetaList    = new std::vector<float>(200) ;
//   std::vector<float> * muPdgId    = new std::vector<float>(1000) ;
//   std::vector<float> * muPt    = new std::vector<float>(1000) ;
//   std::vector<float> * muEta    = new std::vector<float>(1000) ;
//   std::vector<float> * muPx    = new std::vector<float>(1000) ;
//   std::vector<float> * muPy    = new std::vector<float>(1000) ;
//   std::vector<float> * muPz    = new std::vector<float>(1000) ;
   Int_t nSegments = 0;
   Int_t singleME0count[5];
   Int_t singleME0Quality0count[5];
   Int_t singleME0Quality1count[5];
   Int_t singleME0Quality2count[5];
   Int_t singleME0Veto0count[5];
   Int_t singleME0Veto1count[5];
   Int_t singleME0Veto2count[5];
   Int_t singleME0Veto3count[5];
   Int_t singleME0Quality0Veto0count[5];
   Int_t singleME0Quality0Veto1count[5];
   Int_t singleME0Quality0Veto2count[5];
   Int_t singleME0Quality0Veto3count[5];
   Int_t singleME0Quality1Veto0count[5];
   Int_t singleME0Quality1Veto1count[5];
   Int_t singleME0Quality1Veto2count[5];
   Int_t singleME0Quality1Veto3count[5];
   Int_t singleME0Quality2Veto0count[5];
   Int_t singleME0Quality2Veto1count[5];
   Int_t singleME0Quality2Veto2count[5];
   Int_t singleME0Quality2Veto3count[5];
   Int_t doubleME0count[15], tripleME0count[35];
   Int_t doubleME0nearcount[15], tripleME0nearcount[35];
   Int_t doubleME0nearQuality0count[15], tripleME0nearQuality0count[35];
   Int_t doubleME0nearQuality1count[15], tripleME0nearQuality1count[35];
   Int_t doubleME0nearQuality2count[15], tripleME0nearQuality2count[35];
   Int_t doubleME0nearVeto0count[15], tripleME0nearVeto0count[35];
   Int_t doubleME0nearVeto1count[15], tripleME0nearVeto1count[35];
   Int_t doubleME0nearVeto2count[15], tripleME0nearVeto2count[35];
   Int_t doubleME0nearVeto3count[15], tripleME0nearVeto3count[35];
   Int_t doubleME0nearQuality0Veto0count[15], tripleME0nearQuality0Veto0count[35];
   Int_t doubleME0nearQuality0Veto1count[15], tripleME0nearQuality0Veto1count[35];
   Int_t doubleME0nearQuality0Veto2count[15], tripleME0nearQuality0Veto2count[35];
   Int_t doubleME0nearQuality0Veto3count[15], tripleME0nearQuality0Veto3count[35];
   Int_t doubleME0nearQuality1Veto0count[15], tripleME0nearQuality1Veto0count[35];
   Int_t doubleME0nearQuality1Veto1count[15], tripleME0nearQuality1Veto1count[35];
   Int_t doubleME0nearQuality1Veto2count[15], tripleME0nearQuality1Veto2count[35];
   Int_t doubleME0nearQuality1Veto3count[15], tripleME0nearQuality1Veto3count[35];
   Int_t doubleME0nearQuality2Veto0count[15], tripleME0nearQuality2Veto0count[35];
   Int_t doubleME0nearQuality2Veto1count[15], tripleME0nearQuality2Veto1count[35];
   Int_t doubleME0nearQuality2Veto2count[15], tripleME0nearQuality2Veto2count[35];
   Int_t doubleME0nearQuality2Veto3count[15], tripleME0nearQuality2Veto3count[35];
   Int_t doubleME0closecount[15], tripleME0closecount[35];
   Int_t multiplicity[5];
   Float_t singleME0rate[5];
   Float_t singleME0Quality0rate[5];
   Float_t singleME0Quality1rate[5];
   Float_t singleME0Quality2rate[5];
   Float_t singleME0Veto0rate[5];
   Float_t singleME0Veto1rate[5];
   Float_t singleME0Veto2rate[5];
   Float_t singleME0Veto3rate[5];
   Float_t singleME0Quality0Veto0rate[5];
   Float_t singleME0Quality0Veto1rate[5];
   Float_t singleME0Quality0Veto2rate[5];
   Float_t singleME0Quality0Veto3rate[5];
   Float_t singleME0Quality1Veto0rate[5];
   Float_t singleME0Quality1Veto1rate[5];
   Float_t singleME0Quality1Veto2rate[5];
   Float_t singleME0Quality1Veto3rate[5];
   Float_t singleME0Quality2Veto0rate[5];
   Float_t singleME0Quality2Veto1rate[5];
   Float_t singleME0Quality2Veto2rate[5];
   Float_t singleME0Quality2Veto3rate[5];
   Float_t doubleME0rate[15], tripleME0rate[35];
   Float_t doubleME0nearrate[15], tripleME0nearrate[35];
   Float_t doubleME0nearQuality0rate[15], tripleME0nearQuality0rate[35];
   Float_t doubleME0nearQuality1rate[15], tripleME0nearQuality1rate[35];
   Float_t doubleME0nearQuality2rate[15], tripleME0nearQuality2rate[35];
   Float_t doubleME0nearVeto0rate[15], tripleME0nearVeto0rate[35];
   Float_t doubleME0nearVeto1rate[15], tripleME0nearVeto1rate[35];
   Float_t doubleME0nearVeto2rate[15], tripleME0nearVeto2rate[35];
   Float_t doubleME0nearVeto3rate[15], tripleME0nearVeto3rate[35];
   Float_t doubleME0nearQuality0Veto0rate[15], tripleME0nearQuality0Veto0rate[35];
   Float_t doubleME0nearQuality0Veto1rate[15], tripleME0nearQuality0Veto1rate[35];
   Float_t doubleME0nearQuality0Veto2rate[15], tripleME0nearQuality0Veto2rate[35];
   Float_t doubleME0nearQuality0Veto3rate[15], tripleME0nearQuality0Veto3rate[35];
   Float_t doubleME0nearQuality1Veto0rate[15], tripleME0nearQuality1Veto0rate[35];
   Float_t doubleME0nearQuality1Veto1rate[15], tripleME0nearQuality1Veto1rate[35];
   Float_t doubleME0nearQuality1Veto2rate[15], tripleME0nearQuality1Veto2rate[35];
   Float_t doubleME0nearQuality1Veto3rate[15], tripleME0nearQuality1Veto3rate[35];
   Float_t doubleME0nearQuality2Veto0rate[15], tripleME0nearQuality2Veto0rate[35];
   Float_t doubleME0nearQuality2Veto1rate[15], tripleME0nearQuality2Veto1rate[35];
   Float_t doubleME0nearQuality2Veto2rate[15], tripleME0nearQuality2Veto2rate[35];
   Float_t doubleME0nearQuality2Veto3rate[15], tripleME0nearQuality2Veto3rate[35];
   Float_t doubleME0closerate[15], tripleME0closerate[35];
   Float_t singleME0rateError[5];
   Float_t singleME0Quality0rateError[5];
   Float_t singleME0Quality1rateError[5];
   Float_t singleME0Quality2rateError[5];
   Float_t singleME0Veto0rateError[5];
   Float_t singleME0Veto1rateError[5];
   Float_t singleME0Veto2rateError[5];
   Float_t singleME0Veto3rateError[5];
   Float_t singleME0Quality0Veto0rateError[5];
   Float_t singleME0Quality0Veto1rateError[5];
   Float_t singleME0Quality0Veto2rateError[5];
   Float_t singleME0Quality0Veto3rateError[5];
   Float_t singleME0Quality1Veto0rateError[5];
   Float_t singleME0Quality1Veto1rateError[5];
   Float_t singleME0Quality1Veto2rateError[5];
   Float_t singleME0Quality1Veto3rateError[5];
   Float_t singleME0Quality2Veto0rateError[5];
   Float_t singleME0Quality2Veto1rateError[5];
   Float_t singleME0Quality2Veto2rateError[5];
   Float_t singleME0Quality2Veto3rateError[5];
   Float_t doubleME0rateError[15], tripleME0rateError[35];
   Float_t doubleME0nearrateError[15], tripleME0nearrateError[35];
   Float_t doubleME0nearQuality0rateError[15], tripleME0nearQuality0rateError[35];
   Float_t doubleME0nearQuality1rateError[15], tripleME0nearQuality1rateError[35];
   Float_t doubleME0nearQuality2rateError[15], tripleME0nearQuality2rateError[35];
   Float_t doubleME0nearVeto0rateError[15], tripleME0nearVeto0rateError[35];
   Float_t doubleME0nearVeto1rateError[15], tripleME0nearVeto1rateError[35];
   Float_t doubleME0nearVeto2rateError[15], tripleME0nearVeto2rateError[35];
   Float_t doubleME0nearVeto3rateError[15], tripleME0nearVeto3rateError[35];
   Float_t doubleME0nearQuality0Veto0rateError[15], tripleME0nearQuality0Veto0rateError[35];
   Float_t doubleME0nearQuality0Veto1rateError[15], tripleME0nearQuality0Veto1rateError[35];
   Float_t doubleME0nearQuality0Veto2rateError[15], tripleME0nearQuality0Veto2rateError[35];
   Float_t doubleME0nearQuality0Veto3rateError[15], tripleME0nearQuality0Veto3rateError[35];
   Float_t doubleME0nearQuality1Veto0rateError[15], tripleME0nearQuality1Veto0rateError[35];
   Float_t doubleME0nearQuality1Veto1rateError[15], tripleME0nearQuality1Veto1rateError[35];
   Float_t doubleME0nearQuality1Veto2rateError[15], tripleME0nearQuality1Veto2rateError[35];
   Float_t doubleME0nearQuality1Veto3rateError[15], tripleME0nearQuality1Veto3rateError[35];
   Float_t doubleME0nearQuality2Veto0rateError[15], tripleME0nearQuality2Veto0rateError[35];
   Float_t doubleME0nearQuality2Veto1rateError[15], tripleME0nearQuality2Veto1rateError[35];
   Float_t doubleME0nearQuality2Veto2rateError[15], tripleME0nearQuality2Veto2rateError[35];
   Float_t doubleME0nearQuality2Veto3rateError[15], tripleME0nearQuality2Veto3rateError[35];
   Float_t doubleME0closerateError[15], tripleME0closerateError[35];
   Float_t singleME0efficiency[5];
   Float_t singleME0Quality0efficiency[5];
   Float_t singleME0Quality1efficiency[5];
   Float_t singleME0Quality2efficiency[5];
   Float_t singleME0Veto0efficiency[5];
   Float_t singleME0Veto1efficiency[5];
   Float_t singleME0Veto2efficiency[5];
   Float_t singleME0Veto3efficiency[5];
   Float_t singleME0Quality0Veto0efficiency[5];
   Float_t singleME0Quality0Veto1efficiency[5];
   Float_t singleME0Quality0Veto2efficiency[5];
   Float_t singleME0Quality0Veto3efficiency[5];
   Float_t singleME0Quality1Veto0efficiency[5];
   Float_t singleME0Quality1Veto1efficiency[5];
   Float_t singleME0Quality1Veto2efficiency[5];
   Float_t singleME0Quality1Veto3efficiency[5];
   Float_t singleME0Quality2Veto0efficiency[5];
   Float_t singleME0Quality2Veto1efficiency[5];
   Float_t singleME0Quality2Veto2efficiency[5];
   Float_t singleME0Quality2Veto3efficiency[5];
   Float_t doubleME0efficiency[15], tripleME0efficiency[35];
   Float_t doubleME0nearefficiency[15], tripleME0nearefficiency[35];
   Float_t doubleME0nearQuality0efficiency[15], tripleME0nearQuality0efficiency[35];
   Float_t doubleME0nearQuality1efficiency[15], tripleME0nearQuality1efficiency[35];
   Float_t doubleME0nearQuality2efficiency[15], tripleME0nearQuality2efficiency[35];
   Float_t doubleME0nearVeto0efficiency[15], tripleME0nearVeto0efficiency[35];
   Float_t doubleME0nearVeto1efficiency[15], tripleME0nearVeto1efficiency[35];
   Float_t doubleME0nearVeto2efficiency[15], tripleME0nearVeto2efficiency[35];
   Float_t doubleME0nearVeto3efficiency[15], tripleME0nearVeto3efficiency[35];
   Float_t doubleME0nearQuality0Veto0efficiency[15], tripleME0nearQuality0Veto0efficiency[35];
   Float_t doubleME0nearQuality0Veto1efficiency[15], tripleME0nearQuality0Veto1efficiency[35];
   Float_t doubleME0nearQuality0Veto2efficiency[15], tripleME0nearQuality0Veto2efficiency[35];
   Float_t doubleME0nearQuality0Veto3efficiency[15], tripleME0nearQuality0Veto3efficiency[35];
   Float_t doubleME0nearQuality1Veto0efficiency[15], tripleME0nearQuality1Veto0efficiency[35];
   Float_t doubleME0nearQuality1Veto1efficiency[15], tripleME0nearQuality1Veto1efficiency[35];
   Float_t doubleME0nearQuality1Veto2efficiency[15], tripleME0nearQuality1Veto2efficiency[35];
   Float_t doubleME0nearQuality1Veto3efficiency[15], tripleME0nearQuality1Veto3efficiency[35];
   Float_t doubleME0nearQuality2Veto0efficiency[15], tripleME0nearQuality2Veto0efficiency[35];
   Float_t doubleME0nearQuality2Veto1efficiency[15], tripleME0nearQuality2Veto1efficiency[35];
   Float_t doubleME0nearQuality2Veto2efficiency[15], tripleME0nearQuality2Veto2efficiency[35];
   Float_t doubleME0nearQuality2Veto3efficiency[15], tripleME0nearQuality2Veto3efficiency[35];
   Float_t doubleME0closeefficiency[15], tripleME0closeefficiency[35];

   Float_t singleME0efficiencyErr[5];
   Float_t singleME0Quality0efficiencyErr[5];
   Float_t singleME0Quality1efficiencyErr[5];
   Float_t singleME0Quality2efficiencyErr[5];
   Float_t singleME0Veto0efficiencyErr[5];
   Float_t singleME0Veto1efficiencyErr[5];
   Float_t singleME0Veto2efficiencyErr[5];
   Float_t singleME0Veto3efficiencyErr[5];
   Float_t singleME0Quality0Veto0efficiencyErr[5];
   Float_t singleME0Quality0Veto1efficiencyErr[5];
   Float_t singleME0Quality0Veto2efficiencyErr[5];
   Float_t singleME0Quality0Veto3efficiencyErr[5];
   Float_t singleME0Quality1Veto0efficiencyErr[5];
   Float_t singleME0Quality1Veto1efficiencyErr[5];
   Float_t singleME0Quality1Veto2efficiencyErr[5];
   Float_t singleME0Quality1Veto3efficiencyErr[5];
   Float_t singleME0Quality2Veto0efficiencyErr[5];
   Float_t singleME0Quality2Veto1efficiencyErr[5];
   Float_t singleME0Quality2Veto2efficiencyErr[5];
   Float_t singleME0Quality2Veto3efficiencyErr[5];
   Float_t doubleME0efficiencyErr[15], tripleME0efficiencyErr[35];
   Float_t doubleME0nearefficiencyErr[15], tripleME0nearefficiencyErr[35];
   Float_t doubleME0nearQuality0efficiencyErr[15], tripleME0nearQuality0efficiencyErr[35];
   Float_t doubleME0nearQuality1efficiencyErr[15], tripleME0nearQuality1efficiencyErr[35];
   Float_t doubleME0nearQuality2efficiencyErr[15], tripleME0nearQuality2efficiencyErr[35];
   Float_t doubleME0nearVeto0efficiencyErr[15], tripleME0nearVeto0efficiencyErr[35];
   Float_t doubleME0nearVeto1efficiencyErr[15], tripleME0nearVeto1efficiencyErr[35];
   Float_t doubleME0nearVeto2efficiencyErr[15], tripleME0nearVeto2efficiencyErr[35];
   Float_t doubleME0nearVeto3efficiencyErr[15], tripleME0nearVeto3efficiencyErr[35];
   Float_t doubleME0nearQuality0Veto0efficiencyErr[15], tripleME0nearQuality0Veto0efficiencyErr[35];
   Float_t doubleME0nearQuality0Veto1efficiencyErr[15], tripleME0nearQuality0Veto1efficiencyErr[35];
   Float_t doubleME0nearQuality0Veto2efficiencyErr[15], tripleME0nearQuality0Veto2efficiencyErr[35];
   Float_t doubleME0nearQuality0Veto3efficiencyErr[15], tripleME0nearQuality0Veto3efficiencyErr[35];
   Float_t doubleME0nearQuality1Veto0efficiencyErr[15], tripleME0nearQuality1Veto0efficiencyErr[35];
   Float_t doubleME0nearQuality1Veto1efficiencyErr[15], tripleME0nearQuality1Veto1efficiencyErr[35];
   Float_t doubleME0nearQuality1Veto2efficiencyErr[15], tripleME0nearQuality1Veto2efficiencyErr[35];
   Float_t doubleME0nearQuality1Veto3efficiencyErr[15], tripleME0nearQuality1Veto3efficiencyErr[35];
   Float_t doubleME0nearQuality2Veto0efficiencyErr[15], tripleME0nearQuality2Veto0efficiencyErr[35];
   Float_t doubleME0nearQuality2Veto1efficiencyErr[15], tripleME0nearQuality2Veto1efficiencyErr[35];
   Float_t doubleME0nearQuality2Veto2efficiencyErr[15], tripleME0nearQuality2Veto2efficiencyErr[35];
   Float_t doubleME0nearQuality2Veto3efficiencyErr[15], tripleME0nearQuality2Veto3efficiencyErr[35];
   Float_t doubleME0closeefficiencyErr[15], tripleME0closeefficiencyErr[35];


   Bool_t is_singleME0[5];
   Bool_t is_singleME0Quality0[5];
   Bool_t is_singleME0Quality1[5];
   Bool_t is_singleME0Quality2[5];
   Bool_t is_singleME0Veto0[5];
   Bool_t is_singleME0Veto1[5];
   Bool_t is_singleME0Veto2[5];
   Bool_t is_singleME0Veto3[5];
   Bool_t is_singleME0Quality0Veto0[5];
   Bool_t is_singleME0Quality0Veto1[5];
   Bool_t is_singleME0Quality0Veto2[5];
   Bool_t is_singleME0Quality0Veto3[5];
   Bool_t is_singleME0Quality1Veto0[5];
   Bool_t is_singleME0Quality1Veto1[5];
   Bool_t is_singleME0Quality1Veto2[5];
   Bool_t is_singleME0Quality1Veto3[5];
   Bool_t is_singleME0Quality2Veto0[5];
   Bool_t is_singleME0Quality2Veto1[5];
   Bool_t is_singleME0Quality2Veto2[5];
   Bool_t is_singleME0Quality2Veto3[5];
   Bool_t is_doubleME0[15], is_tripleME0[35];
   Bool_t is_doubleME0near[15], is_tripleME0near[35];
   Bool_t is_doubleME0nearQuality0[15], is_tripleME0nearQuality0[35];
   Bool_t is_doubleME0nearQuality1[15], is_tripleME0nearQuality1[35];
   Bool_t is_doubleME0nearQuality2[15], is_tripleME0nearQuality2[35];
   Bool_t is_doubleME0nearVeto0[15], is_tripleME0nearVeto0[35];
   Bool_t is_doubleME0nearVeto1[15], is_tripleME0nearVeto1[35];
   Bool_t is_doubleME0nearVeto2[15], is_tripleME0nearVeto2[35];
   Bool_t is_doubleME0nearVeto3[15], is_tripleME0nearVeto3[35];
   Bool_t is_doubleME0nearQuality0Veto0[15], is_tripleME0nearQuality0Veto0[35];
   Bool_t is_doubleME0nearQuality0Veto1[15], is_tripleME0nearQuality0Veto1[35];
   Bool_t is_doubleME0nearQuality0Veto2[15], is_tripleME0nearQuality0Veto2[35];
   Bool_t is_doubleME0nearQuality0Veto3[15], is_tripleME0nearQuality0Veto3[35];
   Bool_t is_doubleME0nearQuality1Veto0[15], is_tripleME0nearQuality1Veto0[35];
   Bool_t is_doubleME0nearQuality1Veto1[15], is_tripleME0nearQuality1Veto1[35];
   Bool_t is_doubleME0nearQuality1Veto2[15], is_tripleME0nearQuality1Veto2[35];
   Bool_t is_doubleME0nearQuality1Veto3[15], is_tripleME0nearQuality1Veto3[35];
   Bool_t is_doubleME0nearQuality2Veto0[15], is_tripleME0nearQuality2Veto0[35];
   Bool_t is_doubleME0nearQuality2Veto1[15], is_tripleME0nearQuality2Veto1[35];
   Bool_t is_doubleME0nearQuality2Veto2[15], is_tripleME0nearQuality2Veto2[35];
   Bool_t is_doubleME0nearQuality2Veto3[15], is_tripleME0nearQuality2Veto3[35];
   Bool_t is_doubleME0close[15], is_tripleME0close[35];
   Int_t nEvent = 0;
   Int_t nEventSel = 0;
   Int_t lastEvent = 0;
   Int_t lastEventSel = 0;
   Int_t estimatedNmu = 0;
   TTree *tr;
   TTree *trSum;
   TTree *tr2;
   TTree *tr2near;
   TTree *tr2nearQuality0;
   TTree *tr2nearQuality1;
   TTree *tr2nearQuality2;
   TTree *tr2nearVeto0;
   TTree *tr2nearVeto1;
   TTree *tr2nearVeto2;
   TTree *tr2nearVeto3;
   TTree *tr2nearQuality0Veto0;
   TTree *tr2nearQuality0Veto1;
   TTree *tr2nearQuality0Veto2;
   TTree *tr2nearQuality0Veto3;
   TTree *tr2nearQuality1Veto0;
   TTree *tr2nearQuality1Veto1;
   TTree *tr2nearQuality1Veto2;
   TTree *tr2nearQuality1Veto3;
   TTree *tr2nearQuality2Veto0;
   TTree *tr2nearQuality2Veto1;
   TTree *tr2nearQuality2Veto2;
   TTree *tr2nearQuality2Veto3;
   TTree *tr2close;


      int nTau = 0;
      int nDs  = 0;
      int nB0  = 0;
      int nTau3Mu = 0; //=N_myLFVdecay
      int nB0Tau3Mu = 0; //=N_MyB0Chain
      int nDsTau3Mu = 0; //=N_myDsChain
      int nB0Tau = 0;    //=N_myB0decay
      int nDsTau = 0;    //=N_myDsdecay

      int N_myLFVdecay = 0;
      int N_myB0decay = 0;
      int N_myB0Chain = 0;
      int N_myDsdecay = 0;
      int N_myDsChain = 0;

      float muPt[3]	;
      float muEta[3]	;
      float muP[3]	;
      int muIdx[3];
      int muTrackId[3];
      float muVxDeath[3]	;
      float muVyDeath[3]	;
      float muVzDeath[3]	;
      float muVxBirth[3]	;
      float muVyBirth[3]	;
      float muVzBirth[3]	;
      float muPx[3]	;
      float muPy[3]	;
      float muPz[3]	;
      bool isMuVisible[3]; //whether each muon is visibile by ME0 (i.e. has produced ME0SimHits)
      int nVisibleMu;    //number of muons visible by ME0 (from 0 to 3)
      int muStatus[3]	;
      float tauPt = 0	;
      float tauEta = 0	;
      float tauP = 0;
      float tauPx = 0;
      float tauPy = 0;
      float tauPz = 0;
      float tauVxBirth = 0;
      float tauVyBirth = 0;
      float tauVzBirth = 0;
      float tauVxDeath = 0;
      float tauVyDeath = 0;
      float tauVzDeath = 0;
      float mesonPt = 0	;
      float mesonEta = 0	;
      float mesonP = 0;
      float mesonPx = 0;
      float mesonPy = 0;
      float mesonPz = 0;
      float mesonVxBirth = 0;
      float mesonVyBirth = 0;
      float mesonVzBirth = 0;
      float mesonVxDeath = 0;
      float mesonVyDeath = 0;
      float mesonVzDeath = 0;
      
      Bool_t mu_isME0[3]		;	//true if mu in 1.8 < |eta| < 3 
      Bool_t mu_isME0pos[3]	;       //true if mu in +1.8 < eta < +3
      Bool_t mu_isME0neg[3]	;       //true if mu in -1.8 < eta < -3
      int muPdgId[3]		;
      int mesonPdgId = 0;
      int tauPdgId =0   ;
      int nME0 = 0;	//number of mu in 1.8 < |eta| < 3
      int nME0_pos = 0;	//number of mu in +1.8 < eta < +3
      int nME0_neg = 0;	//number of mu in -1.8 < eta < -3
      float diffEta_32 = 0;  //muEta[2]-muEta[1] eta difference betweem mu with highest pt and second highest pt
      float diffEta_31 = 0;  //muEta[2]-muEta[0] eta difference between mu with highest and smallest pt
      float diffEta_21 = 0;  //muEta[1]-muEta[0] eta difference between mu with medium and smallest pt
      float diffEta_maxmed = 0;  //muEta[max]-muEta[med] eta difference between biggest and medium eta
      float diffEta_maxmin = 0;  //muEta[max]-muEta[min] eta difference between biggest and smallest eta
      float diffEta_medmin = 0;  //muEta[med]-muEta[min] eta difference between medium and smallest eta

      std::vector<Float_t> * deltaChamber = new std::vector<Float_t>(200) ;

     int mismatchCount = 0;
     int totalmismatchCount = 0;
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
DeltaGlobalPhiAnalyzer::DeltaGlobalPhiAnalyzer(const edm::ParameterSet& iConfig):
    ME0SegmentToken_(consumes<ME0SegmentCollection>(iConfig.getParameter<InputTag>("me0Segments"))),
    genToken_(consumes<GenParticleCollection>(iConfig.getParameter<InputTag>("genParticles"))),
    ME0SimHitToken_(consumes<vector<PSimHit>>(iConfig.getParameter<InputTag>("me0SimHits"))),
    simTrackToken_(consumes<vector<SimTrack>>(iConfig.getParameter<InputTag>("simTracks")))
{
   //now do what ever initialization is needed
   usesResource("TFileService");
   Service<TFileService> fs;

   v = iConfig.getParameter<bool>("verbose");
   reco = iConfig.getParameter<bool>("reco");
   mutype = iConfig.getParameter<bool>("mutype");
   signal = iConfig.getParameter<bool>("signal");

   //h_Pt3D	= fs->make<TH3F>("h_Pt3D","h_Pt3D", 50, -50, 50, 50, -50, 50, 50, 50, 50);
   h_PtVsEta	= fs->make<TH2F>("h_PtVsEta","h_PtVsEta", 100, 0, 5, 100, 0, 50);         
   h_Eta2D_max	= fs->make<TH2F>("h_Eta2D_max","h_Eta2D_max", 100, 0, 5, 100, 0, 5);
   h_Eta2D_min	= fs->make<TH2F>("h_Eta2D_min","h_Eta2D_min", 100, 0, 5, 100, 0, 5);
   h_Eta3D	= fs->make<TH3F>("h_Eta3D","h_Eta3D", 100, 0, 5, 100, 0, 5, 100, 0, 5);
   h_Pt2D_max	= fs->make<TH2F>("h_Pt2D_max","h_Pt2D_max", 100, 0, 50, 100, 0, 50);
   h_Pt2D_min	= fs->make<TH2F>("h_Pt2D_min","h_Pt2D_min", 100, 0, 50, 100, 0, 50);
   h_Pt3D	= fs->make<TH3F>("h_Pt3D","h_Pt3D", 100, 0, 50, 100, 0, 50, 100, 0, 50);
   h_xy_strange	= fs->make<TH2F>("h_xy_strange","h_xy_strange", 3000, -30, 30, 3000, -30, +30);         
   h_xy_strangeEP1	= fs->make<TH2F>("h_xy_strangeEP1","h_xy_strangeEP1", 3000, -30, 30, 3000, -30, +30);         
   h_xy_strangeEP2	= fs->make<TH2F>("h_xy_strangeEP2","h_xy_strangeEP2", 3000, -30, 30, 3000, -30, +30);         
   h_xy_strangeEP3	= fs->make<TH2F>("h_xy_strangeEP3","h_xy_strangeEP3", 3000, -30, 30, 3000, -30, +30);         
   h_xy_strangeEP4	= fs->make<TH2F>("h_xy_strangeEP4","h_xy_strangeEP4", 3000, -30, 30, 3000, -30, +30);         
   h_xy_strangeEP5	= fs->make<TH2F>("h_xy_strangeEP5","h_xy_strangeEP5", 3000, -30, 30, 3000, -30, +30);         
   h_xy_strangeEP6	= fs->make<TH2F>("h_xy_strangeEP6","h_xy_strangeEP6", 3000, -30, 30, 3000, -30, +30);         
   h_xy_strangeEP7	= fs->make<TH2F>("h_xy_strangeEP7","h_xy_strangeEP7", 3000, -30, 30, 3000, -30, +30);         
   h_xy_strangeEP8	= fs->make<TH2F>("h_xy_strangeEP8","h_xy_strangeEP8", 3000, -30, 30, 3000, -30, +30);         
   h_z_strange	= fs->make<TH1F>("h_z_strange","h_z_strange", 1000, -1, +1);         
   h_z_strangeEP1	= fs->make<TH1F>("h_z_strangeEP1","h_z_strangeEP1", 1000, -1, +1);         
   h_z_strangeEP2	= fs->make<TH1F>("h_z_strangeEP2","h_z_strangeEP2", 1000, -1, +1);         
   h_z_strangeEP3	= fs->make<TH1F>("h_z_strangeEP3","h_z_strangeEP3", 1000, -1, +1);         
   h_z_strangeEP4	= fs->make<TH1F>("h_z_strangeEP4","h_z_strangeEP4", 1000, -1, +1);         
   h_z_strangeEP5	= fs->make<TH1F>("h_z_strangeEP5","h_z_strangeEP5", 1000, -1, +1);         
   h_z_strangeEP6	= fs->make<TH1F>("h_z_strangeEP6","h_z_strangeEP6", 1000, -1, +1);         
   h_z_strangeEP7	= fs->make<TH1F>("h_z_strangeEP7","h_z_strangeEP7", 1000, -1, +1);         
   h_z_strangeEP8	= fs->make<TH1F>("h_z_strangeEP8","h_z_strangeEP8", 1000, -1, +1);         

   tr = fs->make<TTree>("Event", "");
   trSum = fs->make<TTree>("EventSummary", "");

   for ( int i=0; i<5; i++)	{ 
   				singleME0count[i] = 0; singleME0rate[i] = 0; singleME0efficiency[i]=0; 
   				singleME0Quality0count[i] = 0; singleME0Quality0rate[i] = 0; singleME0Quality0efficiency[i]=0; 
   				singleME0Quality1count[i] = 0; singleME0Quality1rate[i] = 0; singleME0Quality1efficiency[i]=0; 
   				singleME0Quality2count[i] = 0; singleME0Quality2rate[i] = 0; singleME0Quality2efficiency[i]=0; 
   				singleME0Veto0count[i] = 0; singleME0Veto0rate[i] = 0; singleME0Veto0efficiency[i]=0; 
   				singleME0Veto1count[i] = 0; singleME0Veto1rate[i] = 0; singleME0Veto1efficiency[i]=0; 
   				singleME0Veto2count[i] = 0; singleME0Veto2rate[i] = 0; singleME0Veto2efficiency[i]=0; 
   				singleME0Veto3count[i] = 0; singleME0Veto3rate[i] = 0; singleME0Veto3efficiency[i]=0; 
   				singleME0Quality0Veto0count[i] = 0; singleME0Quality0Veto0rate[i] = 0; singleME0Quality0Veto0efficiency[i]=0; 
   				singleME0Quality0Veto1count[i] = 0; singleME0Quality0Veto1rate[i] = 0; singleME0Quality0Veto1efficiency[i]=0; 
   				singleME0Quality0Veto2count[i] = 0; singleME0Quality0Veto2rate[i] = 0; singleME0Quality0Veto2efficiency[i]=0; 
   				singleME0Quality0Veto3count[i] = 0; singleME0Quality0Veto3rate[i] = 0; singleME0Quality0Veto3efficiency[i]=0; 
   				singleME0Quality1Veto0count[i] = 0; singleME0Quality1Veto0rate[i] = 0; singleME0Quality1Veto0efficiency[i]=0; 
   				singleME0Quality1Veto1count[i] = 0; singleME0Quality1Veto1rate[i] = 0; singleME0Quality1Veto1efficiency[i]=0; 
   				singleME0Quality1Veto2count[i] = 0; singleME0Quality1Veto2rate[i] = 0; singleME0Quality1Veto2efficiency[i]=0; 
   				singleME0Quality1Veto3count[i] = 0; singleME0Quality1Veto3rate[i] = 0; singleME0Quality1Veto3efficiency[i]=0; 
   				singleME0Quality2Veto0count[i] = 0; singleME0Quality2Veto0rate[i] = 0; singleME0Quality2Veto0efficiency[i]=0; 
   				singleME0Quality2Veto1count[i] = 0; singleME0Quality2Veto1rate[i] = 0; singleME0Quality2Veto1efficiency[i]=0; 
   				singleME0Quality2Veto2count[i] = 0; singleME0Quality2Veto2rate[i] = 0; singleME0Quality2Veto2efficiency[i]=0; 
   				singleME0Quality2Veto3count[i] = 0; singleME0Quality2Veto3rate[i] = 0; singleME0Quality2Veto3efficiency[i]=0; 

   				singleME0efficiencyErr[i]=0; 
   				singleME0Quality0efficiencyErr[i]=0; 
   				singleME0Quality1efficiencyErr[i]=0; 
   				singleME0Quality2efficiencyErr[i]=0; 
   				singleME0Veto0efficiencyErr[i]=0; 
   				singleME0Veto1efficiencyErr[i]=0; 
   				singleME0Veto2efficiencyErr[i]=0; 
   				singleME0Veto3efficiencyErr[i]=0; 
   				singleME0Quality0Veto0efficiencyErr[i]=0; 
   				singleME0Quality0Veto1efficiencyErr[i]=0; 
   				singleME0Quality0Veto2efficiencyErr[i]=0; 
   				singleME0Quality0Veto3efficiencyErr[i]=0; 
   				singleME0Quality1Veto0efficiencyErr[i]=0; 
   				singleME0Quality1Veto1efficiencyErr[i]=0; 
   				singleME0Quality1Veto2efficiencyErr[i]=0; 
   				singleME0Quality1Veto3efficiencyErr[i]=0; 
   				singleME0Quality2Veto0efficiencyErr[i]=0; 
   				singleME0Quality2Veto1efficiencyErr[i]=0; 
   				singleME0Quality2Veto2efficiencyErr[i]=0; 
   				singleME0Quality2Veto3efficiencyErr[i]=0; 

				}
   for ( int i=0; i<15; i++)	{ 
                                doubleME0count[i] = 0; doubleME0rate[i] = 0; doubleME0efficiency[i]=0;
                                doubleME0nearcount[i] = 0; doubleME0nearrate[i] = 0; doubleME0nearefficiency[i]=0;
                                doubleME0nearQuality0count[i] = 0; doubleME0nearQuality0rate[i] = 0; doubleME0nearQuality0efficiency[i]=0;
                                doubleME0nearQuality1count[i] = 0; doubleME0nearQuality1rate[i] = 0; doubleME0nearQuality1efficiency[i]=0;
                                doubleME0nearQuality2count[i] = 0; doubleME0nearQuality2rate[i] = 0; doubleME0nearQuality2efficiency[i]=0;
                                doubleME0nearVeto0count[i] = 0; doubleME0nearVeto0rate[i] = 0; doubleME0nearVeto0efficiency[i]=0;
                                doubleME0nearVeto1count[i] = 0; doubleME0nearVeto1rate[i] = 0; doubleME0nearVeto1efficiency[i]=0;
                                doubleME0nearVeto2count[i] = 0; doubleME0nearVeto2rate[i] = 0; doubleME0nearVeto2efficiency[i]=0;
                                doubleME0nearVeto3count[i] = 0; doubleME0nearVeto3rate[i] = 0; doubleME0nearVeto3efficiency[i]=0;
                                doubleME0nearQuality0Veto0count[i] = 0; doubleME0nearQuality0Veto0rate[i] = 0; doubleME0nearQuality0Veto0efficiency[i]=0;
                                doubleME0nearQuality0Veto1count[i] = 0; doubleME0nearQuality0Veto1rate[i] = 0; doubleME0nearQuality0Veto1efficiency[i]=0;
                                doubleME0nearQuality0Veto2count[i] = 0; doubleME0nearQuality0Veto2rate[i] = 0; doubleME0nearQuality0Veto2efficiency[i]=0;
                                doubleME0nearQuality0Veto3count[i] = 0; doubleME0nearQuality0Veto3rate[i] = 0; doubleME0nearQuality0Veto3efficiency[i]=0;
                                doubleME0nearQuality1Veto0count[i] = 0; doubleME0nearQuality1Veto0rate[i] = 0; doubleME0nearQuality1Veto0efficiency[i]=0;
                                doubleME0nearQuality1Veto1count[i] = 0; doubleME0nearQuality1Veto1rate[i] = 0; doubleME0nearQuality1Veto1efficiency[i]=0;
                                doubleME0nearQuality1Veto2count[i] = 0; doubleME0nearQuality1Veto2rate[i] = 0; doubleME0nearQuality1Veto2efficiency[i]=0;
                                doubleME0nearQuality1Veto3count[i] = 0; doubleME0nearQuality1Veto3rate[i] = 0; doubleME0nearQuality1Veto3efficiency[i]=0;
                                doubleME0nearQuality2Veto0count[i] = 0; doubleME0nearQuality2Veto0rate[i] = 0; doubleME0nearQuality2Veto0efficiency[i]=0;
                                doubleME0nearQuality2Veto1count[i] = 0; doubleME0nearQuality2Veto1rate[i] = 0; doubleME0nearQuality2Veto1efficiency[i]=0;
                                doubleME0nearQuality2Veto2count[i] = 0; doubleME0nearQuality2Veto2rate[i] = 0; doubleME0nearQuality2Veto2efficiency[i]=0;
                                doubleME0nearQuality2Veto3count[i] = 0; doubleME0nearQuality2Veto3rate[i] = 0; doubleME0nearQuality2Veto3efficiency[i]=0;

                                doubleME0closecount[i] = 0; doubleME0closerate[i] = 0; doubleME0closeefficiency[i]=0;


                                doubleME0efficiencyErr[i]=0;
                                doubleME0nearefficiencyErr[i]=0;
                                doubleME0nearQuality0efficiencyErr[i]=0;
                                doubleME0nearQuality1efficiencyErr[i]=0;
                                doubleME0nearQuality2efficiencyErr[i]=0;
                                doubleME0nearVeto0efficiencyErr[i]=0;
                                doubleME0nearVeto1efficiencyErr[i]=0;
                                doubleME0nearVeto2efficiencyErr[i]=0;
                                doubleME0nearVeto3efficiencyErr[i]=0;
                                doubleME0nearQuality0Veto0efficiencyErr[i]=0;
                                doubleME0nearQuality0Veto1efficiencyErr[i]=0;
                                doubleME0nearQuality0Veto2efficiencyErr[i]=0;
                                doubleME0nearQuality0Veto3efficiencyErr[i]=0;
                                doubleME0nearQuality1Veto0efficiencyErr[i]=0;
                                doubleME0nearQuality1Veto1efficiencyErr[i]=0;
                                doubleME0nearQuality1Veto2efficiencyErr[i]=0;
                                doubleME0nearQuality1Veto3efficiencyErr[i]=0;
                                doubleME0nearQuality2Veto0efficiencyErr[i]=0;
                                doubleME0nearQuality2Veto1efficiencyErr[i]=0;
                                doubleME0nearQuality2Veto2efficiencyErr[i]=0;
                                doubleME0nearQuality2Veto3efficiencyErr[i]=0;

                                doubleME0closeefficiencyErr[i]=0;

				}
   for ( int i=0; i<35; i++)	{
                                tripleME0count[i] = 0; tripleME0rate[i] = 0; tripleME0efficiency[i]=0; 
                                tripleME0nearcount[i] = 0; tripleME0nearrate[i] = 0; tripleME0nearefficiency[i]=0; 
                                tripleME0nearQuality0count[i] = 0; tripleME0nearQuality0rate[i] = 0; tripleME0nearQuality0efficiency[i]=0; 
                                tripleME0nearQuality1count[i] = 0; tripleME0nearQuality1rate[i] = 0; tripleME0nearQuality1efficiency[i]=0; 
                                tripleME0nearQuality2count[i] = 0; tripleME0nearQuality2rate[i] = 0; tripleME0nearQuality2efficiency[i]=0; 
                                tripleME0nearVeto0count[i] = 0; tripleME0nearVeto0rate[i] = 0; tripleME0nearVeto0efficiency[i]=0; 
                                tripleME0nearVeto1count[i] = 0; tripleME0nearVeto1rate[i] = 0; tripleME0nearVeto1efficiency[i]=0; 
                                tripleME0nearVeto2count[i] = 0; tripleME0nearVeto2rate[i] = 0; tripleME0nearVeto2efficiency[i]=0; 
                                tripleME0nearVeto3count[i] = 0; tripleME0nearVeto3rate[i] = 0; tripleME0nearVeto3efficiency[i]=0; 
                                tripleME0nearQuality0Veto0count[i] = 0; tripleME0nearQuality0Veto0rate[i] = 0; tripleME0nearQuality0Veto0efficiency[i]=0; 
                                tripleME0nearQuality0Veto1count[i] = 0; tripleME0nearQuality0Veto1rate[i] = 0; tripleME0nearQuality0Veto1efficiency[i]=0; 
                                tripleME0nearQuality0Veto2count[i] = 0; tripleME0nearQuality0Veto2rate[i] = 0; tripleME0nearQuality0Veto2efficiency[i]=0; 
                                tripleME0nearQuality0Veto3count[i] = 0; tripleME0nearQuality0Veto3rate[i] = 0; tripleME0nearQuality0Veto3efficiency[i]=0; 
                                tripleME0nearQuality1Veto0count[i] = 0; tripleME0nearQuality1Veto0rate[i] = 0; tripleME0nearQuality1Veto0efficiency[i]=0; 
                                tripleME0nearQuality1Veto1count[i] = 0; tripleME0nearQuality1Veto1rate[i] = 0; tripleME0nearQuality1Veto1efficiency[i]=0; 
                                tripleME0nearQuality1Veto2count[i] = 0; tripleME0nearQuality1Veto2rate[i] = 0; tripleME0nearQuality1Veto2efficiency[i]=0; 
                                tripleME0nearQuality1Veto3count[i] = 0; tripleME0nearQuality1Veto3rate[i] = 0; tripleME0nearQuality1Veto3efficiency[i]=0; 
                                tripleME0nearQuality2Veto0count[i] = 0; tripleME0nearQuality2Veto0rate[i] = 0; tripleME0nearQuality2Veto0efficiency[i]=0; 
                                tripleME0nearQuality2Veto1count[i] = 0; tripleME0nearQuality2Veto1rate[i] = 0; tripleME0nearQuality2Veto1efficiency[i]=0; 
                                tripleME0nearQuality2Veto2count[i] = 0; tripleME0nearQuality2Veto2rate[i] = 0; tripleME0nearQuality2Veto2efficiency[i]=0; 
                                tripleME0nearQuality2Veto3count[i] = 0; tripleME0nearQuality2Veto3rate[i] = 0; tripleME0nearQuality2Veto3efficiency[i]=0; 
                                tripleME0closecount[i] = 0; tripleME0closerate[i] = 0; tripleME0closeefficiency[i]=0; 

                                tripleME0efficiencyErr[i]=0; 
                                tripleME0nearefficiencyErr[i]=0; 
                                tripleME0nearQuality0efficiencyErr[i]=0; 
                                tripleME0nearQuality1efficiencyErr[i]=0; 
                                tripleME0nearQuality2efficiencyErr[i]=0; 
                                tripleME0nearVeto0efficiencyErr[i]=0; 
                                tripleME0nearVeto1efficiencyErr[i]=0; 
                                tripleME0nearVeto2efficiencyErr[i]=0; 
                                tripleME0nearVeto3efficiencyErr[i]=0; 
                                tripleME0nearQuality0Veto0efficiencyErr[i]=0; 
                                tripleME0nearQuality0Veto1efficiencyErr[i]=0; 
                                tripleME0nearQuality0Veto2efficiencyErr[i]=0; 
                                tripleME0nearQuality0Veto3efficiencyErr[i]=0; 
                                tripleME0nearQuality1Veto0efficiencyErr[i]=0; 
                                tripleME0nearQuality1Veto1efficiencyErr[i]=0; 
                                tripleME0nearQuality1Veto2efficiencyErr[i]=0; 
                                tripleME0nearQuality1Veto3efficiencyErr[i]=0; 
                                tripleME0nearQuality2Veto0efficiencyErr[i]=0; 
                                tripleME0nearQuality2Veto1efficiencyErr[i]=0; 
                                tripleME0nearQuality2Veto2efficiencyErr[i]=0; 
                                tripleME0nearQuality2Veto3efficiencyErr[i]=0; 
                                tripleME0closeefficiencyErr[i]=0; 

				}

   TString tripleStringInt = "";
   TString tripleStringBool = "";
   TString tripleStringFloat = "";
   TString pt[5] = {"3","5","10","20","50"};
   for (int i1=0 ; i1<5 ; i1++)
     for (int i2=i1 ; i2<5; i2++)
       for (int i3=i2 ; i3<5; i3++)
         {
	 tripleStringInt   = tripleStringInt   +pt[i1]+"_"+pt[i2]+"_"+pt[i3]+"GeV/I";
	 tripleStringBool  = tripleStringBool  +pt[i1]+"_"+pt[i2]+"_"+pt[i3]+"GeV/O";
	 tripleStringFloat = tripleStringFloat +pt[i1]+"_"+pt[i2]+"_"+pt[i3]+"GeV/F";
	 if ( !(i1==4 && i2==4 && i3==4) ) 
	   {
	   tripleStringInt   =tripleStringInt   +":";
	   tripleStringBool  =tripleStringBool  +":";
	   tripleStringFloat =tripleStringFloat +":";
	   }
	 }


   //tr->Branch("nEvent"     ,	&nEvent	  , 	"nEvent/I"     	);
   tr->Branch("nTau"       ,	&nTau	  ,	"nTau/I"	);
   tr->Branch("nDs"        ,	&nDs      ,     "nDs/I"		);
   tr->Branch("nB0"        ,	&nB0      ,     "nB0/I"		);
   tr->Branch("nTau3Mu"    ,	&nTau3Mu  ,	"nTau3Mu/I"	);
   tr->Branch("nB0Tau3Mu"  ,	&nB0Tau3Mu,	"nB0Tau3Mu/I"	);
   tr->Branch("nDsTau3Mu"  ,	&nDsTau3Mu,	"nDsTau3Mu/I"	);
   tr->Branch("nB0Tau"  ,	&nB0Tau,	"nB0Tau/I"	);
   tr->Branch("nDsTau"  ,	&nDsTau,	"nDsTau/I"	);

   tr->Branch("tauPt"  ,	&tauPt,	"tauPt"	);
   tr->Branch("tauEta"  ,	&tauEta,	"tauEta"	);
   tr->Branch("tauP"  ,	&tauP,	"tauP"	);
   tr->Branch("tauPx"  ,	&tauPx,	"tauPx"	);
   tr->Branch("tauPy"  ,	&tauPy,	"tauPy"	);
   tr->Branch("tauPz"  ,	&tauPz,	"tauPz"	);
   tr->Branch("tauVxBirth"  ,	&tauVxBirth,	"tauVxBirth"	);
   tr->Branch("tauVyBirth"  ,	&tauVyBirth,	"tauVyBirth"	);
   tr->Branch("tauVzBirth"  ,	&tauVzBirth,	"tauVzBirth"	);
   tr->Branch("tauVxDeath"  ,	&tauVxDeath,	"tauVxDeath"	);
   tr->Branch("tauVyDeath"  ,	&tauVyDeath,	"tauVyDeath"	);
   tr->Branch("tauVzDeath"  ,	&tauVzDeath,	"tauVzDeath"	);
   tr->Branch("mesonPt"  ,	&mesonPt,	"mesonPt"	);
   tr->Branch("mesonEta"  ,	&mesonEta,	"mesonEta"	);
   tr->Branch("mesonP"  ,	&mesonP,	"mesonP"	);
   tr->Branch("mesonPx"  ,	&mesonPx,	"mesonPx"	);
   tr->Branch("mesonPy"  ,	&mesonPy,	"mesonPy"	);
   tr->Branch("mesonPz"  ,	&mesonPz,	"mesonPz"	);
   tr->Branch("mesonVxBirth"  ,	&mesonVxBirth,	"mesonVxBirth"	);
   tr->Branch("mesonVyBirth"  ,	&mesonVyBirth,	"mesonVyBirth"	);
   tr->Branch("mesonVzBirth"  ,	&mesonVzBirth,	"mesonVzBirth"	);
   tr->Branch("mesonVxDeath"  ,	&mesonVxDeath,	"mesonVxDeath"	);
   tr->Branch("mesonVyDeath"  ,	&mesonVyDeath,	"mesonVyDeath"	);
   tr->Branch("mesonVzDeath"  ,	&mesonVzDeath,	"mesonVzDeath"	);
   tr->Branch("muPt"     ,	&muPt[0]     , 	"mu1_Pt:mu2_Pt:mu3_Pt"     );
   tr->Branch("muEta"     ,	&muEta[0]     , 	"mu1_Eta:mu2_Eta:mu3_Eta"     );
   tr->Branch("muIdx"     ,	&muIdx[0]     , 	"mu1_Idx/I:mu2_Idx/I:mu3_Idx/I"     );
   tr->Branch("muTrackId"     ,	&muTrackId[0]     , 	"mu1_Idx/I:mu2_Idx/I:mu3_Idx/I"     );
   tr->Branch("muP"     ,	&muP[0]     , 	"mu1_P:mu2_P:mu3_P"     );
   tr->Branch("muPx"     ,	&muPx[0]     , 	"mu1_Px:mu2_Px:mu3_Px"     );
   tr->Branch("muPy"     ,	&muPy[0]     , 	"mu1_Py:mu2_Py:mu3_Py"     );
   tr->Branch("muPz"     ,	&muPz[0]     , 	"mu1_Pz:mu2_Pz:mu3_Pz"     );
   tr->Branch("isMuVisible"     ,	&isMuVisible[0]     , 	"mu1_isVisible/O:mu2_isVisible/O:mu3_isVisible/O"     );
   tr->Branch("nVisibleMu"  ,	&nVisibleMu,	"nVisibleMu/I"	);
   tr->Branch("muVxBirth"     ,	&muVxBirth[0]     , 	"mu1_VxBirth:mu2_VxBirth:mu3_VxBirth"     );
   tr->Branch("muVyBirth"     ,	&muVyBirth[0]     , 	"mu1_VyBirth:mu2_VyBirth:mu3_VyBirth"     );
   tr->Branch("muVzBirth"     ,	&muVzBirth[0]     , 	"mu1_VzBirth:mu2_VzBirth:mu3_VzBirth"     );
   tr->Branch("muVxDeath"     ,	&muVxDeath[0]     , 	"mu1_VxDeath:mu2_VxDeath:mu3_VxDeath"     );
   tr->Branch("muVyDeath"     ,	&muVyDeath[0]     , 	"mu1_VyDeath:mu2_VyDeath:mu3_VyDeath"     );
   tr->Branch("muVzDeath"     ,	&muVzDeath[0]     , 	"mu1_VzDeath:mu2_VzDeath:mu3_VzDeath"     );
   tr->Branch("muStatus"     ,	&muStatus[0]     , 	"mu1_Status/I:mu2_Status/I:mu3_Status/I"     );

   tr->Branch("muPdgId"     ,	&muPdgId[0]     , 	"mu1_PdgId/I:mu2_PdgId/I:mu3_PdgId/I"     );
   tr->Branch("mu_isME0"     ,	&mu_isME0[0]     , 	"mu1_isME0/O:mu2_isME0/O:mu3_isME0/O"     );
   tr->Branch("mu_isME0pos"     ,	&mu_isME0pos[0]     , 	"mu1_isME0pos/O:mu2_isME0pos/O:mu3_isME0pos/O"     );
   tr->Branch("mu_isME0neg"     ,	&mu_isME0neg[0]     , 	"mu1_isME0neg/O:mu2_isME0neg/O:mu3_isME0neg/O"     );

   tr->Branch("mesonPdgId"  ,	&mesonPdgId,	"mesonPdgId/I"	);
   tr->Branch("tauPdgId"  ,	&tauPdgId,	"tauPdgId/I"	);
   tr->Branch("nME0"  ,	&nME0,	"nME0/I"	);
   tr->Branch("nME0_pos"  ,	&nME0_pos,	"nME0_pos/I"	);
   tr->Branch("nME0_neg"  ,	&nME0_neg,	"nME0_neg/I"	);

   tr->Branch("diffEta_32"  ,	&diffEta_32,	"diffEta_32"	);
   tr->Branch("diffEta_31"  ,	&diffEta_31,	"diffEta_31"	);
   tr->Branch("diffEta_21"  ,	&diffEta_21,	"diffEta_21"	);
   tr->Branch("diffEta_maxmed"  ,	&diffEta_maxmed,	"diffEta_maxmed"	);
   tr->Branch("diffEta_maxmin"  ,	&diffEta_maxmin,	"diffEta_maxmin"	);
   tr->Branch("diffEta_medmin"  ,	&diffEta_medmin,	"diffEta_medmin"	);
   tr->Branch("deltaChamber"     ,	&deltaChamber ); //	, "deltaChamber/I"    );



   //tr->Branch("deltaPhi"     ,	&deltaPhiMap     , 	"deltaPhiMap"     );
   tr->Branch("ME0RawId"     ,	&me0List      );  //, 	"ME0DetId/i"     );
   tr->Branch("deltaPhi"     ,	&deltaPhiList );  //, 	"deltaPhi"     );
   tr->Branch("alpha"     ,	&alphaList    );  //, 	"alpha"     );
   tr->Branch("theta"     ,	&thetaList    );  //, 	"theta"     );
   tr->Branch("segmOriginX"     ,	&xOriginList    );  //, 	"theta"     );
   tr->Branch("segmOriginY"     ,	&yOriginList    );  //, 	"theta"     );
   tr->Branch("segmOriginZ"     ,	&zOriginList    );  //, 	"theta"     );
   tr->Branch("segmOriginGlobalPhi"     ,	&globalPhiOriginList    );  //, 	"theta"     );
   tr->Branch("segmOriginGlobalEta"     ,	&globalEtaOriginList    );  //, 	"theta"     );
   tr->Branch("segmentOriginEtaPartition"     ,	&etaPartList ); //	, "etaPartList/I"    );  //, 	"theta"     );
   tr->Branch("segmLayer1GlobalPhi"     ,	&globalPhiLayer1List    );  //, 	"theta"     );
   tr->Branch("segmLayer1GlobalEta"     ,	&globalEtaLayer1List    );  //, 	"theta"     );
   tr->Branch("segmLayer2GlobalPhi"     ,	&globalPhiLayer2List    );  //, 	"theta"     );
   tr->Branch("segmLayer2GlobalEta"     ,	&globalEtaLayer2List    );  //, 	"theta"     );
   tr->Branch("segmDeltaGlobalPhi"     ,	&deltaGlobalPhiLayer21List    );  //, 	"theta"     );
   tr->Branch("segmRecHitsAverageTof"     ,	&averageTofList    );  //, 	"theta"     );
   //tr->Branch("muPdgId"     ,	&muPdgId    );
   //tr->Branch("muPt"     ,	&muPt    );
   //tr->Branch("muEta"     ,	&muEta    );
   //tr->Branch("muPx"     ,	&muPx    );
   //tr->Branch("muPy"     ,	&muPy    );
   //tr->Branch("muPz"     ,	&muPz    );
   tr->Branch("nEvent"     ,	&nEvent	, "nEvent/I"    );  //, 	"theta"     );
   tr->Branch("nEventSel"     ,	&nEventSel	, "nEventSel/I"    );  //, 	"theta"     );
   trSum->Branch("lastEvent"     ,	&lastEvent	, "lastEvent/I"    );  //, 	"theta"     );
   trSum->Branch("lastEventSel"     ,	&lastEventSel	, "lastEventSel/I"    );  //, 	"theta"     );
   tr->Branch("nSegments"     ,	&nSegments	, "nSegments/I"    );  //, 	"theta"     );
   tr->Branch("segmentNrecHits"     ,	&nSegmRecHitList ); //	, "etaPartList/I"    );  //, 	"theta"     );
   tr->Branch("segmentQuality"     ,	&qualityList ); // q=0 if nRH=4, q=1 if nRH=5, q=2 if nRH=6
   tr->Branch("estimatedNmu"     ,	&estimatedNmu	, "estimatedNmu/I"    );  //, 	"theta"     );
   tr->Branch("is_singleME0", &is_singleME0[0] , "3GeV/O:5GeV/O:10GeV/O:20GeV/O:50GeV/O");
   tr->Branch("is_singleME0Quality0", &is_singleME0Quality0[0] , "3GeV/O:5GeV/O:10GeV/O:20GeV/O:50GeV/O");
   tr->Branch("is_singleME0Quality1", &is_singleME0Quality1[0] , "3GeV/O:5GeV/O:10GeV/O:20GeV/O:50GeV/O");
   tr->Branch("is_singleME0Quality2", &is_singleME0Quality2[0] , "3GeV/O:5GeV/O:10GeV/O:20GeV/O:50GeV/O");
   tr->Branch("is_singleME0Veto0", &is_singleME0Veto0[0] , "3GeV/O:5GeV/O:10GeV/O:20GeV/O:50GeV/O");
   tr->Branch("is_singleME0Veto1", &is_singleME0Veto1[0] , "3GeV/O:5GeV/O:10GeV/O:20GeV/O:50GeV/O");
   tr->Branch("is_singleME0Veto2", &is_singleME0Veto2[0] , "3GeV/O:5GeV/O:10GeV/O:20GeV/O:50GeV/O");
   tr->Branch("is_singleME0Veto3", &is_singleME0Veto3[0] , "3GeV/O:5GeV/O:10GeV/O:30GeV/O:50GeV/O");
   tr->Branch("is_singleME0Quality0Veto0", &is_singleME0Quality0Veto0[0] , "3GeV/O:5GeV/O:10GeV/O:20GeV/O:50GeV/O");
   tr->Branch("is_singleME0Quality0Veto1", &is_singleME0Quality0Veto1[0] , "3GeV/O:5GeV/O:10GeV/O:20GeV/O:50GeV/O");
   tr->Branch("is_singleME0Quality0Veto2", &is_singleME0Quality0Veto2[0] , "3GeV/O:5GeV/O:10GeV/O:20GeV/O:50GeV/O");
   tr->Branch("is_singleME0Quality0Veto3", &is_singleME0Quality0Veto3[0] , "3GeV/O:5GeV/O:10GeV/O:30GeV/O:50GeV/O");
   tr->Branch("is_singleME0Quality1Veto0", &is_singleME0Quality1Veto0[0] , "3GeV/O:5GeV/O:10GeV/O:20GeV/O:50GeV/O");
   tr->Branch("is_singleME0Quality1Veto1", &is_singleME0Quality1Veto1[0] , "3GeV/O:5GeV/O:10GeV/O:20GeV/O:50GeV/O");
   tr->Branch("is_singleME0Quality1Veto2", &is_singleME0Quality1Veto2[0] , "3GeV/O:5GeV/O:10GeV/O:20GeV/O:50GeV/O");
   tr->Branch("is_singleME0Quality1Veto3", &is_singleME0Quality1Veto3[0] , "3GeV/O:5GeV/O:10GeV/O:30GeV/O:50GeV/O");
   tr->Branch("is_singleME0Quality2Veto0", &is_singleME0Quality2Veto0[0] , "3GeV/O:5GeV/O:10GeV/O:20GeV/O:50GeV/O");
   tr->Branch("is_singleME0Quality2Veto1", &is_singleME0Quality2Veto1[0] , "3GeV/O:5GeV/O:10GeV/O:20GeV/O:50GeV/O");
   tr->Branch("is_singleME0Quality2Veto2", &is_singleME0Quality2Veto2[0] , "3GeV/O:5GeV/O:10GeV/O:20GeV/O:50GeV/O");
   tr->Branch("is_singleME0Quality2Veto3", &is_singleME0Quality2Veto3[0] , "3GeV/O:5GeV/O:10GeV/O:30GeV/O:50GeV/O");
   tr->Branch("is_doubleME0", &is_doubleME0[0] , "3_3GeV/O:3_5GeV/O:3_10GeV/O:3_20GeV/O:3_50GeV/O:5_5GeV/O:5_10GeV/O:5_20GeV/O:5_50GeV/O:10_10GeV/O:10_20GeV/O:10_50GeV/O:20_20GeV/O:20_50GeV/O:50_50GeV/O");
   tr->Branch("is_doubleME0near", &is_doubleME0near[0] , "3_3GeV/O:3_5GeV/O:3_10GeV/O:3_20GeV/O:3_50GeV/O:5_5GeV/O:5_10GeV/O:5_20GeV/O:5_50GeV/O:10_10GeV/O:10_20GeV/O:10_50GeV/O:20_20GeV/O:20_50GeV/O:50_50GeV/O");
   tr->Branch("is_doubleME0nearQuality0", &is_doubleME0nearQuality0[0] , "3_3GeV/O:3_5GeV/O:3_10GeV/O:3_20GeV/O:3_50GeV/O:5_5GeV/O:5_10GeV/O:5_20GeV/O:5_50GeV/O:10_10GeV/O:10_20GeV/O:10_50GeV/O:20_20GeV/O:20_50GeV/O:50_50GeV/O");
   tr->Branch("is_doubleME0nearQuality1", &is_doubleME0nearQuality1[0] , "3_3GeV/O:3_5GeV/O:3_10GeV/O:3_20GeV/O:3_50GeV/O:5_5GeV/O:5_10GeV/O:5_20GeV/O:5_50GeV/O:10_10GeV/O:10_20GeV/O:10_50GeV/O:20_20GeV/O:20_50GeV/O:50_50GeV/O");
   tr->Branch("is_doubleME0nearQuality2", &is_doubleME0nearQuality2[0] , "3_3GeV/O:3_5GeV/O:3_10GeV/O:3_20GeV/O:3_50GeV/O:5_5GeV/O:5_10GeV/O:5_20GeV/O:5_50GeV/O:10_10GeV/O:10_20GeV/O:10_50GeV/O:20_20GeV/O:20_50GeV/O:50_50GeV/O");
   tr->Branch("is_doubleME0nearVeto0", &is_doubleME0nearVeto0[0] , "3_3GeV/O:3_5GeV/O:3_10GeV/O:3_20GeV/O:3_50GeV/O:5_5GeV/O:5_10GeV/O:5_20GeV/O:5_50GeV/O:10_10GeV/O:10_20GeV/O:10_50GeV/O:20_20GeV/O:20_50GeV/O:50_50GeV/O");
   tr->Branch("is_doubleME0nearVeto1", &is_doubleME0nearVeto1[0] , "3_3GeV/O:3_5GeV/O:3_10GeV/O:3_20GeV/O:3_50GeV/O:5_5GeV/O:5_10GeV/O:5_20GeV/O:5_50GeV/O:10_10GeV/O:10_20GeV/O:10_50GeV/O:20_20GeV/O:20_50GeV/O:50_50GeV/O");
   tr->Branch("is_doubleME0nearVeto2", &is_doubleME0nearVeto2[0] , "3_3GeV/O:3_5GeV/O:3_10GeV/O:3_20GeV/O:3_50GeV/O:5_5GeV/O:5_10GeV/O:5_20GeV/O:5_50GeV/O:10_10GeV/O:10_20GeV/O:10_50GeV/O:20_20GeV/O:20_50GeV/O:50_50GeV/O");
   tr->Branch("is_doubleME0nearVeto3", &is_doubleME0nearVeto3[0] , "3_3GeV/O:3_5GeV/O:3_10GeV/O:3_30GeV/O:3_50GeV/O:5_5GeV/O:5_10GeV/O:5_30GeV/O:5_50GeV/O:10_10GeV/O:10_30GeV/O:10_50GeV/O:30_30GeV/O:30_50GeV/O:50_50GeV/O");
   tr->Branch("is_doubleME0nearQuality0Veto0", &is_doubleME0nearQuality0Veto0[0] , "3_3GeV/O:3_5GeV/O:3_10GeV/O:3_20GeV/O:3_50GeV/O:5_5GeV/O:5_10GeV/O:5_20GeV/O:5_50GeV/O:10_10GeV/O:10_20GeV/O:10_50GeV/O:20_20GeV/O:20_50GeV/O:50_50GeV/O");
   tr->Branch("is_doubleME0nearQuality0Veto1", &is_doubleME0nearQuality0Veto1[0] , "3_3GeV/O:3_5GeV/O:3_10GeV/O:3_20GeV/O:3_50GeV/O:5_5GeV/O:5_10GeV/O:5_20GeV/O:5_50GeV/O:10_10GeV/O:10_20GeV/O:10_50GeV/O:20_20GeV/O:20_50GeV/O:50_50GeV/O");
   tr->Branch("is_doubleME0nearQuality0Veto2", &is_doubleME0nearQuality0Veto2[0] , "3_3GeV/O:3_5GeV/O:3_10GeV/O:3_20GeV/O:3_50GeV/O:5_5GeV/O:5_10GeV/O:5_20GeV/O:5_50GeV/O:10_10GeV/O:10_20GeV/O:10_50GeV/O:20_20GeV/O:20_50GeV/O:50_50GeV/O");
   tr->Branch("is_doubleME0nearQuality0Veto3", &is_doubleME0nearQuality0Veto3[0] , "3_3GeV/O:3_5GeV/O:3_10GeV/O:3_30GeV/O:3_50GeV/O:5_5GeV/O:5_10GeV/O:5_30GeV/O:5_50GeV/O:10_10GeV/O:10_30GeV/O:10_50GeV/O:30_30GeV/O:30_50GeV/O:50_50GeV/O");
   tr->Branch("is_doubleME0nearQuality1Veto0", &is_doubleME0nearQuality1Veto0[0] , "3_3GeV/O:3_5GeV/O:3_10GeV/O:3_20GeV/O:3_50GeV/O:5_5GeV/O:5_10GeV/O:5_20GeV/O:5_50GeV/O:10_10GeV/O:10_20GeV/O:10_50GeV/O:20_20GeV/O:20_50GeV/O:50_50GeV/O");
   tr->Branch("is_doubleME0nearQuality1Veto1", &is_doubleME0nearQuality1Veto1[0] , "3_3GeV/O:3_5GeV/O:3_10GeV/O:3_20GeV/O:3_50GeV/O:5_5GeV/O:5_10GeV/O:5_20GeV/O:5_50GeV/O:10_10GeV/O:10_20GeV/O:10_50GeV/O:20_20GeV/O:20_50GeV/O:50_50GeV/O");
   tr->Branch("is_doubleME0nearQuality1Veto2", &is_doubleME0nearQuality1Veto2[0] , "3_3GeV/O:3_5GeV/O:3_10GeV/O:3_20GeV/O:3_50GeV/O:5_5GeV/O:5_10GeV/O:5_20GeV/O:5_50GeV/O:10_10GeV/O:10_20GeV/O:10_50GeV/O:20_20GeV/O:20_50GeV/O:50_50GeV/O");
   tr->Branch("is_doubleME0nearQuality1Veto3", &is_doubleME0nearQuality1Veto3[0] , "3_3GeV/O:3_5GeV/O:3_10GeV/O:3_30GeV/O:3_50GeV/O:5_5GeV/O:5_10GeV/O:5_30GeV/O:5_50GeV/O:10_10GeV/O:10_30GeV/O:10_50GeV/O:30_30GeV/O:30_50GeV/O:50_50GeV/O");
   tr->Branch("is_doubleME0nearQuality2Veto0", &is_doubleME0nearQuality2Veto0[0] , "3_3GeV/O:3_5GeV/O:3_10GeV/O:3_20GeV/O:3_50GeV/O:5_5GeV/O:5_10GeV/O:5_20GeV/O:5_50GeV/O:10_10GeV/O:10_20GeV/O:10_50GeV/O:20_20GeV/O:20_50GeV/O:50_50GeV/O");
   tr->Branch("is_doubleME0nearQuality2Veto1", &is_doubleME0nearQuality2Veto1[0] , "3_3GeV/O:3_5GeV/O:3_10GeV/O:3_20GeV/O:3_50GeV/O:5_5GeV/O:5_10GeV/O:5_20GeV/O:5_50GeV/O:10_10GeV/O:10_20GeV/O:10_50GeV/O:20_20GeV/O:20_50GeV/O:50_50GeV/O");
   tr->Branch("is_doubleME0nearQuality2Veto2", &is_doubleME0nearQuality2Veto2[0] , "3_3GeV/O:3_5GeV/O:3_10GeV/O:3_20GeV/O:3_50GeV/O:5_5GeV/O:5_10GeV/O:5_20GeV/O:5_50GeV/O:10_10GeV/O:10_20GeV/O:10_50GeV/O:20_20GeV/O:20_50GeV/O:50_50GeV/O");
   tr->Branch("is_doubleME0nearQuality2Veto3", &is_doubleME0nearQuality2Veto3[0] , "3_3GeV/O:3_5GeV/O:3_10GeV/O:3_30GeV/O:3_50GeV/O:5_5GeV/O:5_10GeV/O:5_30GeV/O:5_50GeV/O:10_10GeV/O:10_30GeV/O:10_50GeV/O:30_30GeV/O:30_50GeV/O:50_50GeV/O");
   tr->Branch("is_doubleME0close", &is_doubleME0close[0] , "3_3GeV/O:3_5GeV/O:3_10GeV/O:3_20GeV/O:3_50GeV/O:5_5GeV/O:5_10GeV/O:5_20GeV/O:5_50GeV/O:10_10GeV/O:10_20GeV/O:10_50GeV/O:20_20GeV/O:20_50GeV/O:50_50GeV/O");
   tr->Branch("is_tripleME0", &is_tripleME0[0] , tripleStringBool);
   tr->Branch("is_tripleME0near", &is_tripleME0near[0] , tripleStringBool);
   tr->Branch("is_tripleME0nearQuality0", &is_tripleME0nearQuality0[0] , tripleStringBool);
   tr->Branch("is_tripleME0nearQuality1", &is_tripleME0nearQuality1[0] , tripleStringBool);
   tr->Branch("is_tripleME0nearQuality2", &is_tripleME0nearQuality2[0] , tripleStringBool);
   tr->Branch("is_tripleME0nearVeto0", &is_tripleME0nearVeto0[0] , tripleStringBool);
   tr->Branch("is_tripleME0nearVeto1", &is_tripleME0nearVeto1[0] , tripleStringBool);
   tr->Branch("is_tripleME0nearVeto2", &is_tripleME0nearVeto2[0] , tripleStringBool);
   tr->Branch("is_tripleME0nearVeto3", &is_tripleME0nearVeto3[0] , tripleStringBool);
   tr->Branch("is_tripleME0nearQuality0Veto0", &is_tripleME0nearQuality0Veto0[0] , tripleStringBool);
   tr->Branch("is_tripleME0nearQuality0Veto1", &is_tripleME0nearQuality0Veto1[0] , tripleStringBool);
   tr->Branch("is_tripleME0nearQuality0Veto2", &is_tripleME0nearQuality0Veto2[0] , tripleStringBool);
   tr->Branch("is_tripleME0nearQuality0Veto3", &is_tripleME0nearQuality0Veto3[0] , tripleStringBool);
   tr->Branch("is_tripleME0nearQuality1Veto0", &is_tripleME0nearQuality1Veto0[0] , tripleStringBool);
   tr->Branch("is_tripleME0nearQuality1Veto1", &is_tripleME0nearQuality1Veto1[0] , tripleStringBool);
   tr->Branch("is_tripleME0nearQuality1Veto2", &is_tripleME0nearQuality1Veto2[0] , tripleStringBool);
   tr->Branch("is_tripleME0nearQuality1Veto3", &is_tripleME0nearQuality1Veto3[0] , tripleStringBool);
   tr->Branch("is_tripleME0nearQuality2Veto0", &is_tripleME0nearQuality2Veto0[0] , tripleStringBool);
   tr->Branch("is_tripleME0nearQuality2Veto1", &is_tripleME0nearQuality2Veto1[0] , tripleStringBool);
   tr->Branch("is_tripleME0nearQuality2Veto2", &is_tripleME0nearQuality2Veto2[0] , tripleStringBool);
   tr->Branch("is_tripleME0nearQuality2Veto3", &is_tripleME0nearQuality2Veto3[0] , tripleStringBool);
   tr->Branch("is_tripleME0close", &is_tripleME0close[0] , tripleStringBool);
   tr->Branch("singleME0_multiplicity", &multiplicity[0] , "3GeV/I:5GeV/I:10GeV/I:20GeV/I:50GeV/I");

   tr2 = fs->make<TTree>("Trigger", "");
   //trigger counts
   tr2->Branch("singleME0count", &singleME0count[0] , "3GeV/I:5GeV/I:10GeV/I:20GeV/I:50GeV/I");
   tr2->Branch("doubleME0count", &doubleME0count[0] , 	"3_3GeV/I:3_5GeV/I:3_10GeV/I:3_20GeV/I:3_50GeV/I:5_5GeV/I:5_10GeV/I:5_20GeV/I:5_50GeV/I:10_10GeV/I:10_20GeV/I:10_50GeV/I:20_20GeV/I:20_50GeV/I:50_50GeV/I");
   tr2->Branch("tripleME0count", &tripleME0count[0] , 	tripleStringInt);
   //trigger rates
   tr2->Branch("singleME0rate", &singleME0rate[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2->Branch("doubleME0rate", &doubleME0rate[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2->Branch("tripleME0rate", &tripleME0rate[0] , tripleStringFloat);
   //trigger rate errors
   tr2->Branch("singleME0rateError", &singleME0rateError[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2->Branch("doubleME0rateError", &doubleME0rateError[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2->Branch("tripleME0rateError", &tripleME0rateError[0] , tripleStringFloat);
   //trigger efficiencies
   tr2->Branch("singleME0efficiency", &singleME0efficiency[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2->Branch("doubleME0efficiency", &doubleME0efficiency[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2->Branch("tripleME0efficiency", &tripleME0efficiency[0] , tripleStringFloat);
   //trigger efficiency errors
   tr2->Branch("singleME0efficiencyErr", &singleME0efficiencyErr[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2->Branch("doubleME0efficiencyErr", &doubleME0efficiencyErr[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2->Branch("tripleME0efficiencyErr", &tripleME0efficiencyErr[0] , tripleStringFloat);

   tr2near = fs->make<TTree>("Trigger_ChamberVicinity", "");
   //trigger counts
   tr2near->Branch("doubleME0count", &doubleME0nearcount[0] , 	"3_3GeV/I:3_5GeV/I:3_10GeV/I:3_20GeV/I:3_50GeV/I:5_5GeV/I:5_10GeV/I:5_20GeV/I:5_50GeV/I:10_10GeV/I:10_20GeV/I:10_50GeV/I:20_20GeV/I:20_50GeV/I:50_50GeV/I");
   tr2near->Branch("tripleME0count", &tripleME0nearcount[0] , 	tripleStringInt);
   //trigger rates
   tr2near->Branch("doubleME0rate", &doubleME0nearrate[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2near->Branch("tripleME0rate", &tripleME0nearrate[0] , tripleStringFloat);
   //trigger rate errors
   tr2near->Branch("doubleME0rateError", &doubleME0nearrateError[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2near->Branch("tripleME0rateError", &tripleME0nearrateError[0] , tripleStringFloat);
   //trigger efficiencies
   tr2near->Branch("doubleME0efficiency", &doubleME0nearefficiency[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2near->Branch("tripleME0efficiency", &tripleME0nearefficiency[0] , tripleStringFloat);
   //trigger efficiency error
   tr2near->Branch("doubleME0efficiencyErr", &doubleME0nearefficiencyErr[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2near->Branch("tripleME0efficiencyErr", &tripleME0nearefficiencyErr[0] , tripleStringFloat);

   tr2close = fs->make<TTree>("Trigger_ChamberEtaVicinity", "");
   //trigger counts
   tr2close->Branch("doubleME0count", &doubleME0closecount[0] , 	"3_3GeV/I:3_5GeV/I:3_10GeV/I:3_20GeV/I:3_50GeV/I:5_5GeV/I:5_10GeV/I:5_20GeV/I:5_50GeV/I:10_10GeV/I:10_20GeV/I:10_50GeV/I:20_20GeV/I:20_50GeV/I:50_50GeV/I");
   tr2close->Branch("tripleME0count", &tripleME0closecount[0] , 	tripleStringInt);
   //trigger rates
   tr2close->Branch("doubleME0rate", &doubleME0closerate[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2close->Branch("tripleME0rate", &tripleME0closerate[0] , tripleStringFloat);
   //trigger rate errors
   tr2close->Branch("doubleME0rateError", &doubleME0closerateError[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2close->Branch("tripleME0rateError", &tripleME0closerateError[0] , tripleStringFloat);
   //trigger efficiencies
   tr2close->Branch("doubleME0efficiency", &doubleME0closeefficiency[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2close->Branch("tripleME0efficiency", &tripleME0closeefficiency[0] , tripleStringFloat);
   //trigger efficiency errors
   tr2close->Branch("doubleME0efficiencyErr", &doubleME0closeefficiencyErr[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2close->Branch("tripleME0efficiencyErr", &tripleME0closeefficiencyErr[0] , tripleStringFloat);

   tr2nearQuality0 = fs->make<TTree>("Trigger_ChamberVicinity_quality0", "");
   //trigger counts
   tr2nearQuality0->Branch("singleME0count", &singleME0Quality0count[0] , "3GeV/I:5GeV/I:10GeV/I:20GeV/I:50GeV/I");
   tr2nearQuality0->Branch("doubleME0count", &doubleME0nearQuality0count[0] , 	"3_3GeV/I:3_5GeV/I:3_10GeV/I:3_20GeV/I:3_50GeV/I:5_5GeV/I:5_10GeV/I:5_20GeV/I:5_50GeV/I:10_10GeV/I:10_20GeV/I:10_50GeV/I:20_20GeV/I:20_50GeV/I:50_50GeV/I");
   tr2nearQuality0->Branch("tripleME0count", &tripleME0nearQuality0count[0] , 	tripleStringInt);
   //trigger rates
   tr2nearQuality0->Branch("singleME0rate", &singleME0Quality0rate[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2nearQuality0->Branch("doubleME0rate", &doubleME0nearQuality0rate[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2nearQuality0->Branch("tripleME0rate", &tripleME0nearQuality0rate[0] , tripleStringFloat);
   //trigger rate errors
   tr2nearQuality0->Branch("singleME0rateError", &singleME0Quality0rateError[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2nearQuality0->Branch("doubleME0rateError", &doubleME0nearQuality0rateError[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2nearQuality0->Branch("tripleME0rateError", &tripleME0nearQuality0rateError[0] , tripleStringFloat);
   //trigger efficiencies
   tr2nearQuality0->Branch("singleME0efficiency", &singleME0Quality0efficiency[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2nearQuality0->Branch("doubleME0efficiency", &doubleME0nearQuality0efficiency[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2nearQuality0->Branch("tripleME0efficiency", &tripleME0nearQuality0efficiency[0] , tripleStringFloat);
   //trigger efficiency errors
   tr2nearQuality0->Branch("singleME0efficiencyErr", &singleME0Quality0efficiencyErr[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2nearQuality0->Branch("doubleME0efficiencyErr", &doubleME0nearQuality0efficiencyErr[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2nearQuality0->Branch("tripleME0efficiencyErr", &tripleME0nearQuality0efficiencyErr[0] , tripleStringFloat);

   tr2nearQuality1 = fs->make<TTree>("Trigger_ChamberVicinity_quality1", "");
   //trigger counts
   tr2nearQuality1->Branch("singleME0count", &singleME0Quality1count[0] , "3GeV/I:5GeV/I:10GeV/I:20GeV/I:50GeV/I");
   tr2nearQuality1->Branch("doubleME0count", &doubleME0nearQuality1count[0] , 	"3_3GeV/i:3_5GeV/i:3_10GeV/i:3_20GeV/i:3_50GeV/i:5_5GeV/i:5_10GeV/i:5_20GeV/i:5_50GeV/i:10_10GeV/i:10_20GeV/i:10_50GeV/i:20_20GeV/i:20_50GeV/i:50_50GeV/i");
   tr2nearQuality1->Branch("tripleME0count", &tripleME0nearQuality1count[0] , 	tripleStringInt);
   //trigger rates
   tr2nearQuality1->Branch("singleME0rate", &singleME0Quality1rate[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2nearQuality1->Branch("doubleME0rate", &doubleME0nearQuality1rate[0] , 	"3_3GeV/f:3_5GeV/f:3_10GeV/f:3_20GeV/f:3_50GeV/f:5_5GeV/f:5_10GeV/f:5_20GeV/f:5_50GeV/f:10_10GeV/f:10_20GeV/f:10_50GeV/f:20_20GeV/f:20_50GeV/f:50_50GeV/f");
   tr2nearQuality1->Branch("tripleME0rate", &tripleME0nearQuality1rate[0] , tripleStringFloat);
   //trigger rate errors
   tr2nearQuality1->Branch("singleME0rateError", &singleME0Quality1rateError[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2nearQuality1->Branch("doubleME0rateError", &doubleME0nearQuality1rateError[0] , 	"3_3GeV/f:3_5GeV/f:3_10GeV/f:3_20GeV/f:3_50GeV/f:5_5GeV/f:5_10GeV/f:5_20GeV/f:5_50GeV/f:10_10GeV/f:10_20GeV/f:10_50GeV/f:20_20GeV/f:20_50GeV/f:50_50GeV/f");
   tr2nearQuality1->Branch("tripleME0rateError", &tripleME0nearQuality1rateError[0] , tripleStringFloat);
   //trigger efficiencies
   tr2nearQuality1->Branch("singleME0efficiency", &singleME0Quality1efficiency[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2nearQuality1->Branch("doubleME0efficiency", &doubleME0nearQuality1efficiency[0] , 	"3_3GeV/f:3_5GeV/f:3_10GeV/f:3_20GeV/f:3_50GeV/f:5_5GeV/f:5_10GeV/f:5_20GeV/f:5_50GeV/f:10_10GeV/f:10_20GeV/f:10_50GeV/f:20_20GeV/f:20_50GeV/f:50_50GeV/f");
   tr2nearQuality1->Branch("tripleME0efficiency", &tripleME0nearQuality1efficiency[0] , tripleStringFloat);
   //trigger efficiency errors
   tr2nearQuality1->Branch("singleME0efficiencyErr", &singleME0Quality1efficiencyErr[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2nearQuality1->Branch("doubleME0efficiencyErr", &doubleME0nearQuality1efficiencyErr[0] , 	"3_3GeV/f:3_5GeV/f:3_10GeV/f:3_20GeV/f:3_50GeV/f:5_5GeV/f:5_10GeV/f:5_20GeV/f:5_50GeV/f:10_10GeV/f:10_20GeV/f:10_50GeV/f:20_20GeV/f:20_50GeV/f:50_50GeV/f");
   tr2nearQuality1->Branch("tripleME0efficiencyErr", &tripleME0nearQuality1efficiencyErr[0] , tripleStringFloat);

   tr2nearQuality2 = fs->make<TTree>("Trigger_ChamberVicinity_quality2", "");
   //trigger counts
   tr2nearQuality2->Branch("singleME0count", &singleME0Quality2count[0] , "3GeV/I:5GeV/I:10GeV/I:20GeV/I:50GeV/I");
   tr2nearQuality2->Branch("doubleME0count", &doubleME0nearQuality2count[0] , 	"3_3GeV/I:3_5GeV/I:3_10GeV/I:3_20GeV/I:3_50GeV/I:5_5GeV/I:5_10GeV/I:5_20GeV/I:5_50GeV/I:10_10GeV/I:10_20GeV/I:10_50GeV/I:20_20GeV/I:20_50GeV/I:50_50GeV/I");
   tr2nearQuality2->Branch("tripleME0count", &tripleME0nearQuality2count[0] , 	tripleStringInt);
   //trigger rates
   tr2nearQuality2->Branch("singleME0rate", &singleME0Quality2rate[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2nearQuality2->Branch("doubleME0rate", &doubleME0nearQuality2rate[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2nearQuality2->Branch("tripleME0rate", &tripleME0nearQuality2rate[0] , tripleStringFloat);
   //trigger rate errors
   tr2nearQuality2->Branch("singleME0rateError", &singleME0Quality2rateError[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2nearQuality2->Branch("doubleME0rateError", &doubleME0nearQuality2rateError[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2nearQuality2->Branch("tripleME0rateError", &tripleME0nearQuality2rateError[0] , tripleStringFloat);
   //trigger efficiencies
   tr2nearQuality2->Branch("singleME0efficiency", &singleME0Quality2efficiency[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2nearQuality2->Branch("doubleME0efficiency", &doubleME0nearQuality2efficiency[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2nearQuality2->Branch("tripleME0efficiency", &tripleME0nearQuality2efficiency[0] , tripleStringFloat);
   //trigger efficiency errors
   tr2nearQuality2->Branch("singleME0efficiencyErr", &singleME0Quality2efficiencyErr[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2nearQuality2->Branch("doubleME0efficiencyErr", &doubleME0nearQuality2efficiencyErr[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2nearQuality2->Branch("tripleME0efficiencyErr", &tripleME0nearQuality2efficiencyErr[0] , tripleStringFloat);

   tr2nearVeto0 = fs->make<TTree>("Trigger_ChamberVicinity_veto0", "");
   //trigger counts
   tr2nearVeto0->Branch("singleME0count", &singleME0Veto0count[0] , "3GeV/I:5GeV/I:10GeV/I:20GeV/I:50GeV/I");
   tr2nearVeto0->Branch("doubleME0count", &doubleME0nearVeto0count[0] , 	"3_3GeV/I:3_5GeV/I:3_10GeV/I:3_20GeV/I:3_50GeV/I:5_5GeV/I:5_10GeV/I:5_20GeV/I:5_50GeV/I:10_10GeV/I:10_20GeV/I:10_50GeV/I:20_20GeV/I:20_50GeV/I:50_50GeV/I");
   tr2nearVeto0->Branch("tripleME0count", &tripleME0nearVeto0count[0] , 	tripleStringInt);
   //trigger rates
   tr2nearVeto0->Branch("singleME0rate", &singleME0Veto0rate[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2nearVeto0->Branch("doubleME0rate", &doubleME0nearVeto0rate[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2nearVeto0->Branch("tripleME0rate", &tripleME0nearVeto0rate[0] , tripleStringFloat);
   //trigger rate errors
   tr2nearVeto0->Branch("singleME0rateError", &singleME0Veto0rateError[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2nearVeto0->Branch("doubleME0rateError", &doubleME0nearVeto0rateError[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2nearVeto0->Branch("tripleME0rateError", &tripleME0nearVeto0rateError[0] , tripleStringFloat);
   //trigger efficiencies
   tr2nearVeto0->Branch("singleME0efficiency", &singleME0Veto0efficiency[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2nearVeto0->Branch("doubleME0efficiency", &doubleME0nearVeto0efficiency[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2nearVeto0->Branch("tripleME0efficiency", &tripleME0nearVeto0efficiency[0] , tripleStringFloat);
   //trigger efficiency errors
   tr2nearVeto0->Branch("singleME0efficiencyErr", &singleME0Veto0efficiencyErr[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2nearVeto0->Branch("doubleME0efficiencyErr", &doubleME0nearVeto0efficiencyErr[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2nearVeto0->Branch("tripleME0efficiencyErr", &tripleME0nearVeto0efficiencyErr[0] , tripleStringFloat);

   tr2nearVeto1 = fs->make<TTree>("Trigger_ChamberVicinity_veto1", "");
   //trigger counts
   tr2nearVeto1->Branch("singleME0count", &singleME0Veto1count[0] , "3GeV/I:5GeV/I:10GeV/I:20GeV/I:50GeV/I");
   tr2nearVeto1->Branch("doubleME0count", &doubleME0nearVeto1count[0] , 	"3_3GeV/I:3_5GeV/I:3_10GeV/I:3_20GeV/I:3_50GeV/I:5_5GeV/I:5_10GeV/I:5_20GeV/I:5_50GeV/I:10_10GeV/I:10_20GeV/I:10_50GeV/I:20_20GeV/I:20_50GeV/I:50_50GeV/I");
   tr2nearVeto1->Branch("tripleME0count", &tripleME0nearVeto1count[0] , 	tripleStringInt);
   //trigger rates
   tr2nearVeto1->Branch("singleME0rate", &singleME0Veto1rate[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2nearVeto1->Branch("doubleME0rate", &doubleME0nearVeto1rate[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2nearVeto1->Branch("tripleME0rate", &tripleME0nearVeto1rate[0] , tripleStringFloat);
   //trigger rate errors
   tr2nearVeto1->Branch("singleME0rateError", &singleME0Veto1rateError[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2nearVeto1->Branch("doubleME0rateError", &doubleME0nearVeto1rateError[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2nearVeto1->Branch("tripleME0rateError", &tripleME0nearVeto1rateError[0] , tripleStringFloat);
   //trigger efficiencies
   tr2nearVeto1->Branch("singleME0efficiency", &singleME0Veto1efficiency[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2nearVeto1->Branch("doubleME0efficiency", &doubleME0nearVeto1efficiency[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2nearVeto1->Branch("tripleME0efficiency", &tripleME0nearVeto1efficiency[0] , tripleStringFloat);
   //trigger efficiency errors
   tr2nearVeto1->Branch("singleME0efficiencyErr", &singleME0Veto1efficiencyErr[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2nearVeto1->Branch("doubleME0efficiencyErr", &doubleME0nearVeto1efficiencyErr[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2nearVeto1->Branch("tripleME0efficiencyErr", &tripleME0nearVeto1efficiencyErr[0] , tripleStringFloat);

   tr2nearVeto2 = fs->make<TTree>("Trigger_ChamberVicinity_veto2", "");
   //trigger counts
   tr2nearVeto2->Branch("singleME0count", &singleME0Veto2count[0] , "3GeV/I:5GeV/I:10GeV/I:20GeV/I:50GeV/I");
   tr2nearVeto2->Branch("doubleME0count", &doubleME0nearVeto2count[0] , 	"3_3GeV/I:3_5GeV/I:3_10GeV/I:3_20GeV/I:3_50GeV/I:5_5GeV/I:5_10GeV/I:5_20GeV/I:5_50GeV/I:10_10GeV/I:10_20GeV/I:10_50GeV/I:20_20GeV/I:20_50GeV/I:50_50GeV/I");
   tr2nearVeto2->Branch("tripleME0count", &tripleME0nearVeto2count[0] , 	tripleStringInt);
   //trigger rates
   tr2nearVeto2->Branch("singleME0rate", &singleME0Veto2rate[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2nearVeto2->Branch("doubleME0rate", &doubleME0nearVeto2rate[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2nearVeto2->Branch("tripleME0rate", &tripleME0nearVeto2rate[0] , tripleStringFloat);
   //trigger rate errors
   tr2nearVeto2->Branch("singleME0rateError", &singleME0Veto2rateError[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2nearVeto2->Branch("doubleME0rateError", &doubleME0nearVeto2rateError[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2nearVeto2->Branch("tripleME0rateError", &tripleME0nearVeto2rateError[0] , tripleStringFloat);
   //trigger efficiencies
   tr2nearVeto2->Branch("singleME0efficiency", &singleME0Veto2efficiency[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2nearVeto2->Branch("doubleME0efficiency", &doubleME0nearVeto2efficiency[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2nearVeto2->Branch("tripleME0efficiency", &tripleME0nearVeto2efficiency[0] , tripleStringFloat);
   //trigger efficiency errors
   tr2nearVeto2->Branch("singleME0efficiencyErr", &singleME0Veto2efficiencyErr[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2nearVeto2->Branch("doubleME0efficiencyErr", &doubleME0nearVeto2efficiencyErr[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2nearVeto2->Branch("tripleME0efficiencyErr", &tripleME0nearVeto2efficiencyErr[0] , tripleStringFloat);

   tr2nearVeto3 = fs->make<TTree>("Trigger_ChamberVicinity_veto3", "");
   //trigger counts
   tr2nearVeto3->Branch("singleME0count", &singleME0Veto3count[0] , "3GeV/I:5GeV/I:10GeV/I:20GeV/I:50GeV/I");
   tr2nearVeto3->Branch("doubleME0count", &doubleME0nearVeto3count[0] , 	"3_3GeV/I:3_5GeV/I:3_10GeV/I:3_20GeV/I:3_50GeV/I:5_5GeV/I:5_10GeV/I:5_20GeV/I:5_50GeV/I:10_10GeV/I:10_20GeV/I:10_50GeV/I:20_20GeV/I:20_50GeV/I:50_50GeV/I");
   tr2nearVeto3->Branch("tripleME0count", &tripleME0nearVeto3count[0] , 	tripleStringInt);
   //trigger rates
   tr2nearVeto3->Branch("singleME0rate", &singleME0Veto3rate[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2nearVeto3->Branch("doubleME0rate", &doubleME0nearVeto3rate[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2nearVeto3->Branch("tripleME0rate", &tripleME0nearVeto3rate[0] , tripleStringFloat);
   //trigger rate errors
   tr2nearVeto3->Branch("singleME0rateError", &singleME0Veto3rateError[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2nearVeto3->Branch("doubleME0rateError", &doubleME0nearVeto3rateError[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2nearVeto3->Branch("tripleME0rateError", &tripleME0nearVeto3rateError[0] , tripleStringFloat);
   //trigger efficiencies
   tr2nearVeto3->Branch("singleME0efficiency", &singleME0Veto3efficiency[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2nearVeto3->Branch("doubleME0efficiency", &doubleME0nearVeto3efficiency[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2nearVeto3->Branch("tripleME0efficiency", &tripleME0nearVeto3efficiency[0] , tripleStringFloat);
   //trigger efficiency errors
   tr2nearVeto3->Branch("singleME0efficiencyErr", &singleME0Veto3efficiencyErr[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2nearVeto3->Branch("doubleME0efficiencyErr", &doubleME0nearVeto3efficiencyErr[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2nearVeto3->Branch("tripleME0efficiencyErr", &tripleME0nearVeto3efficiencyErr[0] , tripleStringFloat);

   tr2nearQuality0Veto0 = fs->make<TTree>("Trigger_ChamberVicinity_quality0veto0", "");
   //trigger counts
   tr2nearQuality0Veto0->Branch("singleME0count", &singleME0Quality0Veto0count[0] , "3GeV/I:5GeV/I:10GeV/I:20GeV/I:50GeV/I");
   tr2nearQuality0Veto0->Branch("doubleME0count", &doubleME0nearQuality0Veto0count[0] , 	"3_3GeV/I:3_5GeV/I:3_10GeV/I:3_20GeV/I:3_50GeV/I:5_5GeV/I:5_10GeV/I:5_20GeV/I:5_50GeV/I:10_10GeV/I:10_20GeV/I:10_50GeV/I:20_20GeV/I:20_50GeV/I:50_50GeV/I");
   tr2nearQuality0Veto0->Branch("tripleME0count", &tripleME0nearQuality0Veto0count[0] , 	tripleStringInt);
   //trigger rates
   tr2nearQuality0Veto0->Branch("singleME0rate", &singleME0Quality0Veto0rate[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2nearQuality0Veto0->Branch("doubleME0rate", &doubleME0nearQuality0Veto0rate[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2nearQuality0Veto0->Branch("tripleME0rate", &tripleME0nearQuality0Veto0rate[0] , tripleStringFloat);
   //trigger rate errors
   tr2nearQuality0Veto0->Branch("singleME0rateError", &singleME0Quality0Veto0rateError[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2nearQuality0Veto0->Branch("doubleME0rateError", &doubleME0nearQuality0Veto0rateError[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2nearQuality0Veto0->Branch("tripleME0rateError", &tripleME0nearQuality0Veto0rateError[0] , tripleStringFloat);
   //trigger efficiencies
   tr2nearQuality0Veto0->Branch("singleME0efficiency", &singleME0Quality0Veto0efficiency[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2nearQuality0Veto0->Branch("doubleME0efficiency", &doubleME0nearQuality0Veto0efficiency[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2nearQuality0Veto0->Branch("tripleME0efficiency", &tripleME0nearQuality0Veto0efficiency[0] , tripleStringFloat);
   //trigger efficiency errors
   tr2nearQuality0Veto0->Branch("singleME0efficiencyErr", &singleME0Quality0Veto0efficiencyErr[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2nearQuality0Veto0->Branch("doubleME0efficiencyErr", &doubleME0nearQuality0Veto0efficiencyErr[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2nearQuality0Veto0->Branch("tripleME0efficiencyErr", &tripleME0nearQuality0Veto0efficiencyErr[0] , tripleStringFloat);

   tr2nearQuality0Veto1 = fs->make<TTree>("Trigger_ChamberVicinity_quality0veto1", "");
   //trigger counts
   tr2nearQuality0Veto1->Branch("singleME0count", &singleME0Quality0Veto1count[0] , "3GeV/I:5GeV/I:10GeV/I:20GeV/I:50GeV/I");
   tr2nearQuality0Veto1->Branch("doubleME0count", &doubleME0nearQuality0Veto1count[0] , 	"3_3GeV/I:3_5GeV/I:3_10GeV/I:3_20GeV/I:3_50GeV/I:5_5GeV/I:5_10GeV/I:5_20GeV/I:5_50GeV/I:10_10GeV/I:10_20GeV/I:10_50GeV/I:20_20GeV/I:20_50GeV/I:50_50GeV/I");
   tr2nearQuality0Veto1->Branch("tripleME0count", &tripleME0nearQuality0Veto1count[0] , 	tripleStringInt);
   //trigger rates
   tr2nearQuality0Veto1->Branch("singleME0rate", &singleME0Quality0Veto1rate[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2nearQuality0Veto1->Branch("doubleME0rate", &doubleME0nearQuality0Veto1rate[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2nearQuality0Veto1->Branch("tripleME0rate", &tripleME0nearQuality0Veto1rate[0] , tripleStringFloat);
   //trigger rate errors
   tr2nearQuality0Veto1->Branch("singleME0rateError", &singleME0Quality0Veto1rateError[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2nearQuality0Veto1->Branch("doubleME0rateError", &doubleME0nearQuality0Veto1rateError[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2nearQuality0Veto1->Branch("tripleME0rateError", &tripleME0nearQuality0Veto1rateError[0] , tripleStringFloat);
   //trigger efficiencies
   tr2nearQuality0Veto1->Branch("singleME0efficiency", &singleME0Quality0Veto1efficiency[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2nearQuality0Veto1->Branch("doubleME0efficiency", &doubleME0nearQuality0Veto1efficiency[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2nearQuality0Veto1->Branch("tripleME0efficiency", &tripleME0nearQuality0Veto1efficiency[0] , tripleStringFloat);
   //trigger efficiency errors
   tr2nearQuality0Veto1->Branch("singleME0efficiencyErr", &singleME0Quality0Veto1efficiencyErr[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2nearQuality0Veto1->Branch("doubleME0efficiencyErr", &doubleME0nearQuality0Veto1efficiencyErr[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2nearQuality0Veto1->Branch("tripleME0efficiencyErr", &tripleME0nearQuality0Veto1efficiencyErr[0] , tripleStringFloat);

   tr2nearQuality0Veto2 = fs->make<TTree>("Trigger_ChamberVicinity_quality0veto2", "");
   //trigger counts
   tr2nearQuality0Veto2->Branch("singleME0count", &singleME0Quality0Veto2count[0] , "3GeV/I:5GeV/I:10GeV/I:20GeV/I:50GeV/I");
   tr2nearQuality0Veto2->Branch("doubleME0count", &doubleME0nearQuality0Veto2count[0] , 	"3_3GeV/I:3_5GeV/I:3_10GeV/I:3_20GeV/I:3_50GeV/I:5_5GeV/I:5_10GeV/I:5_20GeV/I:5_50GeV/I:10_10GeV/I:10_20GeV/I:10_50GeV/I:20_20GeV/I:20_50GeV/I:50_50GeV/I");
   tr2nearQuality0Veto2->Branch("tripleME0count", &tripleME0nearQuality0Veto2count[0] , 	tripleStringInt);
   //trigger rates
   tr2nearQuality0Veto2->Branch("singleME0rate", &singleME0Quality0Veto2rate[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2nearQuality0Veto2->Branch("doubleME0rate", &doubleME0nearQuality0Veto2rate[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2nearQuality0Veto2->Branch("tripleME0rate", &tripleME0nearQuality0Veto2rate[0] , tripleStringFloat);
   //trigger rate errors
   tr2nearQuality0Veto2->Branch("singleME0rateError", &singleME0Quality0Veto2rateError[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2nearQuality0Veto2->Branch("doubleME0rateError", &doubleME0nearQuality0Veto2rateError[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2nearQuality0Veto2->Branch("tripleME0rateError", &tripleME0nearQuality0Veto2rateError[0] , tripleStringFloat);
   //trigger efficiencies
   tr2nearQuality0Veto2->Branch("singleME0efficiency", &singleME0Quality0Veto2efficiency[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2nearQuality0Veto2->Branch("doubleME0efficiency", &doubleME0nearQuality0Veto2efficiency[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2nearQuality0Veto2->Branch("tripleME0efficiency", &tripleME0nearQuality0Veto2efficiency[0] , tripleStringFloat);
   //trigger efficiency errors
   tr2nearQuality0Veto2->Branch("singleME0efficiencyErr", &singleME0Quality0Veto2efficiencyErr[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2nearQuality0Veto2->Branch("doubleME0efficiencyErr", &doubleME0nearQuality0Veto2efficiencyErr[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2nearQuality0Veto2->Branch("tripleME0efficiencyErr", &tripleME0nearQuality0Veto2efficiencyErr[0] , tripleStringFloat);

   tr2nearQuality0Veto3 = fs->make<TTree>("Trigger_ChamberVicinity_quality0veto3", "");
   //trigger counts
   tr2nearQuality0Veto3->Branch("singleME0count", &singleME0Quality0Veto3count[0] , "3GeV/I:5GeV/I:10GeV/I:20GeV/I:50GeV/I");
   tr2nearQuality0Veto3->Branch("doubleME0count", &doubleME0nearQuality0Veto3count[0] , 	"3_3GeV/I:3_5GeV/I:3_10GeV/I:3_20GeV/I:3_50GeV/I:5_5GeV/I:5_10GeV/I:5_20GeV/I:5_50GeV/I:10_10GeV/I:10_20GeV/I:10_50GeV/I:20_20GeV/I:20_50GeV/I:50_50GeV/I");
   tr2nearQuality0Veto3->Branch("tripleME0count", &tripleME0nearQuality0Veto3count[0] , 	tripleStringInt);
   //trigger rates
   tr2nearQuality0Veto3->Branch("singleME0rate", &singleME0Quality0Veto3rate[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2nearQuality0Veto3->Branch("doubleME0rate", &doubleME0nearQuality0Veto3rate[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2nearQuality0Veto3->Branch("tripleME0rate", &tripleME0nearQuality0Veto3rate[0] , tripleStringFloat);
   //trigger rate errors
   tr2nearQuality0Veto3->Branch("singleME0rateError", &singleME0Quality0Veto3rateError[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2nearQuality0Veto3->Branch("doubleME0rateError", &doubleME0nearQuality0Veto3rateError[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2nearQuality0Veto3->Branch("tripleME0rateError", &tripleME0nearQuality0Veto3rateError[0] , tripleStringFloat);
   //trigger efficiencies
   tr2nearQuality0Veto3->Branch("singleME0efficiency", &singleME0Quality0Veto3efficiency[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2nearQuality0Veto3->Branch("doubleME0efficiency", &doubleME0nearQuality0Veto3efficiency[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2nearQuality0Veto3->Branch("tripleME0efficiency", &tripleME0nearQuality0Veto3efficiency[0] , tripleStringFloat);
   //trigger efficiency errors
   tr2nearQuality0Veto3->Branch("singleME0efficiencyErr", &singleME0Quality0Veto3efficiencyErr[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2nearQuality0Veto3->Branch("doubleME0efficiencyErr", &doubleME0nearQuality0Veto3efficiencyErr[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2nearQuality0Veto3->Branch("tripleME0efficiencyErr", &tripleME0nearQuality0Veto3efficiencyErr[0] , tripleStringFloat);

   tr2nearQuality1Veto0 = fs->make<TTree>("Trigger_ChamberVicinity_quality1veto0", "");
   //trigger counts
   tr2nearQuality1Veto0->Branch("singleME0count", &singleME0Quality1Veto0count[0] , "3GeV/I:5GeV/I:10GeV/I:20GeV/I:50GeV/I");
   tr2nearQuality1Veto0->Branch("doubleME0count", &doubleME0nearQuality1Veto0count[0] , 	"3_3GeV/I:3_5GeV/I:3_10GeV/I:3_20GeV/I:3_50GeV/I:5_5GeV/I:5_10GeV/I:5_20GeV/I:5_50GeV/I:10_10GeV/I:10_20GeV/I:10_50GeV/I:20_20GeV/I:20_50GeV/I:50_50GeV/I");
   tr2nearQuality1Veto0->Branch("tripleME0count", &tripleME0nearQuality1Veto0count[0] , 	tripleStringInt);
   //trigger rates
   tr2nearQuality1Veto0->Branch("singleME0rate", &singleME0Quality1Veto0rate[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2nearQuality1Veto0->Branch("doubleME0rate", &doubleME0nearQuality1Veto0rate[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2nearQuality1Veto0->Branch("tripleME0rate", &tripleME0nearQuality1Veto0rate[0] , tripleStringFloat);
   //trigger rate errors
   tr2nearQuality1Veto0->Branch("singleME0rateError", &singleME0Quality1Veto0rateError[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2nearQuality1Veto0->Branch("doubleME0rateError", &doubleME0nearQuality1Veto0rateError[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2nearQuality1Veto0->Branch("tripleME0rateError", &tripleME0nearQuality1Veto0rateError[0] , tripleStringFloat);
   //trigger efficiencies
   tr2nearQuality1Veto0->Branch("singleME0efficiency", &singleME0Quality1Veto0efficiency[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2nearQuality1Veto0->Branch("doubleME0efficiency", &doubleME0nearQuality1Veto0efficiency[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2nearQuality1Veto0->Branch("tripleME0efficiency", &tripleME0nearQuality1Veto0efficiency[0] , tripleStringFloat);
   //trigger efficiency errors
   tr2nearQuality1Veto0->Branch("singleME0efficiencyErr", &singleME0Quality1Veto0efficiencyErr[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2nearQuality1Veto0->Branch("doubleME0efficiencyErr", &doubleME0nearQuality1Veto0efficiencyErr[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2nearQuality1Veto0->Branch("tripleME0efficiencyErr", &tripleME0nearQuality1Veto0efficiencyErr[0] , tripleStringFloat);

   tr2nearQuality1Veto1 = fs->make<TTree>("Trigger_ChamberVicinity_quality1veto1", "");
   //trigger counts
   tr2nearQuality1Veto1->Branch("singleME0count", &singleME0Quality1Veto1count[0] , "3GeV/I:5GeV/I:10GeV/I:20GeV/I:50GeV/I");
   tr2nearQuality1Veto1->Branch("doubleME0count", &doubleME0nearQuality1Veto1count[0] , 	"3_3GeV/I:3_5GeV/I:3_10GeV/I:3_20GeV/I:3_50GeV/I:5_5GeV/I:5_10GeV/I:5_20GeV/I:5_50GeV/I:10_10GeV/I:10_20GeV/I:10_50GeV/I:20_20GeV/I:20_50GeV/I:50_50GeV/I");
   tr2nearQuality1Veto1->Branch("tripleME0count", &tripleME0nearQuality1Veto1count[0] , 	tripleStringInt);
   //trigger rates
   tr2nearQuality1Veto1->Branch("singleME0rate", &singleME0Quality1Veto1rate[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2nearQuality1Veto1->Branch("doubleME0rate", &doubleME0nearQuality1Veto1rate[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2nearQuality1Veto1->Branch("tripleME0rate", &tripleME0nearQuality1Veto1rate[0] , tripleStringFloat);
   //trigger rate errors
   tr2nearQuality1Veto1->Branch("singleME0rateError", &singleME0Quality1Veto1rateError[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2nearQuality1Veto1->Branch("doubleME0rateError", &doubleME0nearQuality1Veto1rateError[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2nearQuality1Veto1->Branch("tripleME0rateError", &tripleME0nearQuality1Veto1rateError[0] , tripleStringFloat);
   //trigger efficiencies
   tr2nearQuality1Veto1->Branch("singleME0efficiency", &singleME0Quality1Veto1efficiency[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2nearQuality1Veto1->Branch("doubleME0efficiency", &doubleME0nearQuality1Veto1efficiency[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2nearQuality1Veto1->Branch("tripleME0efficiency", &tripleME0nearQuality1Veto1efficiency[0] , tripleStringFloat);
   //trigger efficiency errors
   tr2nearQuality1Veto1->Branch("singleME0efficiencyErr", &singleME0Quality1Veto1efficiencyErr[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2nearQuality1Veto1->Branch("doubleME0efficiencyErr", &doubleME0nearQuality1Veto1efficiencyErr[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2nearQuality1Veto1->Branch("tripleME0efficiencyErr", &tripleME0nearQuality1Veto1efficiencyErr[0] , tripleStringFloat);

   tr2nearQuality1Veto2 = fs->make<TTree>("Trigger_ChamberVicinity_quality1veto2", "");
   //trigger counts
   tr2nearQuality1Veto2->Branch("singleME0count", &singleME0Quality1Veto2count[0] , "3GeV/I:5GeV/I:10GeV/I:20GeV/I:50GeV/I");
   tr2nearQuality1Veto2->Branch("doubleME0count", &doubleME0nearQuality1Veto2count[0] , 	"3_3GeV/I:3_5GeV/I:3_10GeV/I:3_20GeV/I:3_50GeV/I:5_5GeV/I:5_10GeV/I:5_20GeV/I:5_50GeV/I:10_10GeV/I:10_20GeV/I:10_50GeV/I:20_20GeV/I:20_50GeV/I:50_50GeV/I");
   tr2nearQuality1Veto2->Branch("tripleME0count", &tripleME0nearQuality1Veto2count[0] , 	tripleStringInt);
   //trigger rates
   tr2nearQuality1Veto2->Branch("singleME0rate", &singleME0Quality1Veto2rate[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2nearQuality1Veto2->Branch("doubleME0rate", &doubleME0nearQuality1Veto2rate[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2nearQuality1Veto2->Branch("tripleME0rate", &tripleME0nearQuality1Veto2rate[0] , tripleStringFloat);
   //trigger rate errors
   tr2nearQuality1Veto2->Branch("singleME0rateError", &singleME0Quality1Veto2rateError[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2nearQuality1Veto2->Branch("doubleME0rateError", &doubleME0nearQuality1Veto2rateError[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2nearQuality1Veto2->Branch("tripleME0rateError", &tripleME0nearQuality1Veto2rateError[0] , tripleStringFloat);
   //trigger efficiencies
   tr2nearQuality1Veto2->Branch("singleME0efficiency", &singleME0Quality1Veto2efficiency[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2nearQuality1Veto2->Branch("doubleME0efficiency", &doubleME0nearQuality1Veto2efficiency[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2nearQuality1Veto2->Branch("tripleME0efficiency", &tripleME0nearQuality1Veto2efficiency[0] , tripleStringFloat);
   //trigger efficiency errors
   tr2nearQuality1Veto2->Branch("singleME0efficiencyErr", &singleME0Quality1Veto2efficiencyErr[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2nearQuality1Veto2->Branch("doubleME0efficiencyErr", &doubleME0nearQuality1Veto2efficiencyErr[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2nearQuality1Veto2->Branch("tripleME0efficiencyErr", &tripleME0nearQuality1Veto2efficiencyErr[0] , tripleStringFloat);


   tr2nearQuality1Veto3 = fs->make<TTree>("Trigger_ChamberVicinity_quality1veto3", "");
   //trigger counts
   tr2nearQuality1Veto3->Branch("singleME0count", &singleME0Quality1Veto3count[0] , "3GeV/I:5GeV/I:10GeV/I:20GeV/I:50GeV/I");
   tr2nearQuality1Veto3->Branch("doubleME0count", &doubleME0nearQuality1Veto3count[0] , 	"3_3GeV/I:3_5GeV/I:3_10GeV/I:3_20GeV/I:3_50GeV/I:5_5GeV/I:5_10GeV/I:5_20GeV/I:5_50GeV/I:10_10GeV/I:10_20GeV/I:10_50GeV/I:20_20GeV/I:20_50GeV/I:50_50GeV/I");
   tr2nearQuality1Veto3->Branch("tripleME0count", &tripleME0nearQuality1Veto3count[0] , 	tripleStringInt);
   //trigger rates
   tr2nearQuality1Veto3->Branch("singleME0rate", &singleME0Quality1Veto3rate[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2nearQuality1Veto3->Branch("doubleME0rate", &doubleME0nearQuality1Veto3rate[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2nearQuality1Veto3->Branch("tripleME0rate", &tripleME0nearQuality1Veto3rate[0] , tripleStringFloat);
   //trigger rate errors
   tr2nearQuality1Veto3->Branch("singleME0rateError", &singleME0Quality1Veto3rateError[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2nearQuality1Veto3->Branch("doubleME0rateError", &doubleME0nearQuality1Veto3rateError[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2nearQuality1Veto3->Branch("tripleME0rateError", &tripleME0nearQuality1Veto3rateError[0] , tripleStringFloat);
   //trigger efficiencies
   tr2nearQuality1Veto3->Branch("singleME0efficiency", &singleME0Quality1Veto3efficiency[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2nearQuality1Veto3->Branch("doubleME0efficiency", &doubleME0nearQuality1Veto3efficiency[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2nearQuality1Veto3->Branch("tripleME0efficiency", &tripleME0nearQuality1Veto3efficiency[0] , tripleStringFloat);
   //trigger efficiency errors
   tr2nearQuality1Veto3->Branch("singleME0efficiencyErr", &singleME0Quality1Veto3efficiencyErr[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2nearQuality1Veto3->Branch("doubleME0efficiencyErr", &doubleME0nearQuality1Veto3efficiencyErr[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2nearQuality1Veto3->Branch("tripleME0efficiencyErr", &tripleME0nearQuality1Veto3efficiencyErr[0] , tripleStringFloat);

   tr2nearQuality2Veto0 = fs->make<TTree>("Trigger_ChamberVicinity_quality2veto0", "");
   //trigger counts
   tr2nearQuality2Veto0->Branch("singleME0count", &singleME0Quality2Veto0count[0] , "3GeV/I:5GeV/I:10GeV/I:20GeV/I:50GeV/I");
   tr2nearQuality2Veto0->Branch("doubleME0count", &doubleME0nearQuality2Veto0count[0] , 	"3_3GeV/I:3_5GeV/I:3_10GeV/I:3_20GeV/I:3_50GeV/I:5_5GeV/I:5_10GeV/I:5_20GeV/I:5_50GeV/I:10_10GeV/I:10_20GeV/I:10_50GeV/I:20_20GeV/I:20_50GeV/I:50_50GeV/I");
   tr2nearQuality2Veto0->Branch("tripleME0count", &tripleME0nearQuality2Veto0count[0] , 	tripleStringInt);
   //trigger rates
   tr2nearQuality2Veto0->Branch("singleME0rate", &singleME0Quality2Veto0rate[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2nearQuality2Veto0->Branch("doubleME0rate", &doubleME0nearQuality2Veto0rate[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2nearQuality2Veto0->Branch("tripleME0rate", &tripleME0nearQuality2Veto0rate[0] , tripleStringFloat);
   //trigger rate errors
   tr2nearQuality2Veto0->Branch("singleME0rateError", &singleME0Quality2Veto0rateError[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2nearQuality2Veto0->Branch("doubleME0rateError", &doubleME0nearQuality2Veto0rateError[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2nearQuality2Veto0->Branch("tripleME0rateError", &tripleME0nearQuality2Veto0rateError[0] , tripleStringFloat);
   //trigger efficiencies
   tr2nearQuality2Veto0->Branch("singleME0efficiency", &singleME0Quality2Veto0efficiency[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2nearQuality2Veto0->Branch("doubleME0efficiency", &doubleME0nearQuality2Veto0efficiency[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2nearQuality2Veto0->Branch("tripleME0efficiency", &tripleME0nearQuality2Veto0efficiency[0] , tripleStringFloat);
   //trigger efficiency errors
   tr2nearQuality2Veto0->Branch("singleME0efficiencyErr", &singleME0Quality2Veto0efficiencyErr[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2nearQuality2Veto0->Branch("doubleME0efficiencyErr", &doubleME0nearQuality2Veto0efficiencyErr[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2nearQuality2Veto0->Branch("tripleME0efficiencyErr", &tripleME0nearQuality2Veto0efficiencyErr[0] , tripleStringFloat);

   tr2nearQuality2Veto1 = fs->make<TTree>("Trigger_ChamberVicinity_quality2veto1", "");
   //trigger counts
   tr2nearQuality2Veto1->Branch("singleME0count", &singleME0Quality2Veto1count[0] , "3GeV/I:5GeV/I:10GeV/I:20GeV/I:50GeV/I");
   tr2nearQuality2Veto1->Branch("doubleME0count", &doubleME0nearQuality2Veto1count[0] , 	"3_3GeV/I:3_5GeV/I:3_10GeV/I:3_20GeV/I:3_50GeV/I:5_5GeV/I:5_10GeV/I:5_20GeV/I:5_50GeV/I:10_10GeV/I:10_20GeV/I:10_50GeV/I:20_20GeV/I:20_50GeV/I:50_50GeV/I");
   tr2nearQuality2Veto1->Branch("tripleME0count", &tripleME0nearQuality2Veto1count[0] , 	tripleStringInt);
   //trigger rates
   tr2nearQuality2Veto1->Branch("singleME0rate", &singleME0Quality2Veto1rate[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2nearQuality2Veto1->Branch("doubleME0rate", &doubleME0nearQuality2Veto1rate[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2nearQuality2Veto1->Branch("tripleME0rate", &tripleME0nearQuality2Veto1rate[0] , tripleStringFloat);
   //trigger rate errors
   tr2nearQuality2Veto1->Branch("singleME0rateError", &singleME0Quality2Veto1rateError[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2nearQuality2Veto1->Branch("doubleME0rateError", &doubleME0nearQuality2Veto1rateError[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2nearQuality2Veto1->Branch("tripleME0rateError", &tripleME0nearQuality2Veto1rateError[0] , tripleStringFloat);
   //trigger efficiencies
   tr2nearQuality2Veto1->Branch("singleME0efficiency", &singleME0Quality2Veto1efficiency[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2nearQuality2Veto1->Branch("doubleME0efficiency", &doubleME0nearQuality2Veto1efficiency[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2nearQuality2Veto1->Branch("tripleME0efficiency", &tripleME0nearQuality2Veto1efficiency[0] , tripleStringFloat);
   //trigger efficiency errors
   tr2nearQuality2Veto1->Branch("singleME0efficiencyErr", &singleME0Quality2Veto1efficiencyErr[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2nearQuality2Veto1->Branch("doubleME0efficiencyErr", &doubleME0nearQuality2Veto1efficiencyErr[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2nearQuality2Veto1->Branch("tripleME0efficiencyErr", &tripleME0nearQuality2Veto1efficiencyErr[0] , tripleStringFloat);

   tr2nearQuality2Veto2 = fs->make<TTree>("Trigger_ChamberVicinity_quality2veto2", "");
   //trigger counts
   tr2nearQuality2Veto2->Branch("singleME0count", &singleME0Quality2Veto2count[0] , "3GeV/I:5GeV/I:10GeV/I:20GeV/I:50GeV/I");
   tr2nearQuality2Veto2->Branch("doubleME0count", &doubleME0nearQuality2Veto2count[0] , 	"3_3GeV/I:3_5GeV/I:3_10GeV/I:3_20GeV/I:3_50GeV/I:5_5GeV/I:5_10GeV/I:5_20GeV/I:5_50GeV/I:10_10GeV/I:10_20GeV/I:10_50GeV/I:20_20GeV/I:20_50GeV/I:50_50GeV/I");
   tr2nearQuality2Veto2->Branch("tripleME0count", &tripleME0nearQuality2Veto2count[0] , 	tripleStringInt);
   //trigger rates
   tr2nearQuality2Veto2->Branch("singleME0rate", &singleME0Quality2Veto2rate[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2nearQuality2Veto2->Branch("doubleME0rate", &doubleME0nearQuality2Veto2rate[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2nearQuality2Veto2->Branch("tripleME0rate", &tripleME0nearQuality2Veto2rate[0] , tripleStringFloat);
   //trigger rate errors
   tr2nearQuality2Veto2->Branch("singleME0rateError", &singleME0Quality2Veto2rateError[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2nearQuality2Veto2->Branch("doubleME0rateError", &doubleME0nearQuality2Veto2rateError[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2nearQuality2Veto2->Branch("tripleME0rateError", &tripleME0nearQuality2Veto2rateError[0] , tripleStringFloat);
   //trigger efficiencies
   tr2nearQuality2Veto2->Branch("singleME0efficiency", &singleME0Quality2Veto2efficiency[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2nearQuality2Veto2->Branch("doubleME0efficiency", &doubleME0nearQuality2Veto2efficiency[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2nearQuality2Veto2->Branch("tripleME0efficiency", &tripleME0nearQuality2Veto2efficiency[0] , tripleStringFloat);
   //trigger efficiency errors
   tr2nearQuality2Veto2->Branch("singleME0efficiencyErr", &singleME0Quality2Veto2efficiencyErr[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2nearQuality2Veto2->Branch("doubleME0efficiencyErr", &doubleME0nearQuality2Veto2efficiencyErr[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2nearQuality2Veto2->Branch("tripleME0efficiencyErr", &tripleME0nearQuality2Veto2efficiencyErr[0] , tripleStringFloat);


   tr2nearQuality2Veto3 = fs->make<TTree>("Trigger_ChamberVicinity_quality2veto3", "");
   //trigger counts
   tr2nearQuality2Veto3->Branch("singleME0count", &singleME0Quality2Veto3count[0] , "3GeV/I:5GeV/I:10GeV/I:20GeV/I:50GeV/I");
   tr2nearQuality2Veto3->Branch("doubleME0count", &doubleME0nearQuality2Veto3count[0] , 	"3_3GeV/I:3_5GeV/I:3_10GeV/I:3_20GeV/I:3_50GeV/I:5_5GeV/I:5_10GeV/I:5_20GeV/I:5_50GeV/I:10_10GeV/I:10_20GeV/I:10_50GeV/I:20_20GeV/I:20_50GeV/I:50_50GeV/I");
   tr2nearQuality2Veto3->Branch("tripleME0count", &tripleME0nearQuality2Veto3count[0] , 	tripleStringInt);
   //trigger rates
   tr2nearQuality2Veto3->Branch("singleME0rate", &singleME0Quality2Veto3rate[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2nearQuality2Veto3->Branch("doubleME0rate", &doubleME0nearQuality2Veto3rate[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2nearQuality2Veto3->Branch("tripleME0rate", &tripleME0nearQuality2Veto3rate[0] , tripleStringFloat);
   //trigger rate errors
   tr2nearQuality2Veto3->Branch("singleME0rateError", &singleME0Quality2Veto3rateError[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2nearQuality2Veto3->Branch("doubleME0rateError", &doubleME0nearQuality2Veto3rateError[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2nearQuality2Veto3->Branch("tripleME0rateError", &tripleME0nearQuality2Veto3rateError[0] , tripleStringFloat);
   //trigger efficiencies
   tr2nearQuality2Veto3->Branch("singleME0efficiency", &singleME0Quality2Veto3efficiency[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2nearQuality2Veto3->Branch("doubleME0efficiency", &doubleME0nearQuality2Veto3efficiency[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2nearQuality2Veto3->Branch("tripleME0efficiency", &tripleME0nearQuality2Veto3efficiency[0] , tripleStringFloat);
   //trigger efficiency errors
   tr2nearQuality2Veto3->Branch("singleME0efficiencyErr", &singleME0Quality2Veto3efficiencyErr[0] , "3GeV/F:5GeV/F:10GeV/F:20GeV/F:50GeV/F");
   tr2nearQuality2Veto3->Branch("doubleME0efficiencyErr", &doubleME0nearQuality2Veto3efficiencyErr[0] , 	"3_3GeV/F:3_5GeV/F:3_10GeV/F:3_20GeV/F:3_50GeV/F:5_5GeV/F:5_10GeV/F:5_20GeV/F:5_50GeV/F:10_10GeV/F:10_20GeV/F:10_50GeV/F:20_20GeV/F:20_50GeV/F:50_50GeV/F");
   tr2nearQuality2Veto3->Branch("tripleME0efficiencyErr", &tripleME0nearQuality2Veto3efficiencyErr[0] , tripleStringFloat);

   c_quality	= fs->make<TCanvas>("RateVsQuality", "RateVsQuality", 50,50,W,H); 
   c_eveto	= fs->make<TCanvas>("RateVsEtaveto", "RateVsEtaveto", 50,50,W,H); 
   c_singlemu	= fs->make<TCanvas>("singleMu_RateVsSel", "singleMu_RateVsSel", 50,50,W,H); 
   c_doublemu	= fs->make<TCanvas>("doubleMu_RateVsSel", "doubleMu_RateVsSel", 50,50,W,H); 
   c_triplemu	= fs->make<TCanvas>("tripleMu_RateVsSel", "tripleMu_RateVsSel", 50,50,W,H); 

}


DeltaGlobalPhiAnalyzer::~DeltaGlobalPhiAnalyzer()
{
  cout << "nr. of tau -> 3mu found: " << N_myLFVdecay << "\n" << endl;
  cout << "nr. of B0 -> tau found: " << N_myB0decay << endl;
  cout << "nr. of B0 -> tau -> 3mu found: "<< N_myB0Chain  << "\n" << endl;
  cout << "nr. of Ds -> tau found: " << N_myDsdecay << endl;
  cout << "nr. of Ds -> tau -> 3mu found: " << N_myDsChain << "\n" << endl;
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
DeltaGlobalPhiAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   using namespace std;


gStyle->SetOptTitle(0);

#ifdef THIS_IS_AN_EVENT_EXAMPLE
   Handle<ExampleData> pIn;
   iEvent.getByLabel("example",pIn);
#endif
   
#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
   ESHandle<SetupData> pSetup;
   iSetup.get<SetupRecord>().get(pSetup);
#endif


      (*deltaChamber).clear();
   //   nEvent++ ;
      nB0	 = 0 ;
      nDs	 = 0 ;
      nTau	 = 0 ;
      nTau3Mu    = 0 ;
      nB0Tau3Mu  = 0 ;
      nDsTau3Mu  = 0 ;
      nB0Tau     = 0 ;
      nDsTau     = 0 ;
      for (int ii=0; ii<3; ii++) 
        {
	mu_isME0[ii]=0;	       
        mu_isME0pos[ii]=0;       
        mu_isME0neg[ii]=0;       
        muPdgId[ii]=0;
	}
      mesonPdgId = 0;
      tauPdgId =0   ;
      nME0 = 0;	//number of mu in 1.8 < |eta| < 3
      nME0_pos = 0;	//number of mu in +1.8 < eta < +3
      nME0_neg = 0;	//number of mu in -1.8 < eta < -3
      diffEta_32 = 9999;  //muEta[3]-muEta[2]
      diffEta_31 = 9999;  //muEta[3]-muEta[1]
      diffEta_21 = 9999;  //muEta[2]-muEta[1]
      diffEta_maxmed = 9999;  //muEta[3]-muEta[2]
      diffEta_maxmin = 9999;  //muEta[3]-muEta[1]
      diffEta_medmin = 9999;  //muEta[2]-muEta[1]
//------------------ GEN PARTICLES ------------------------------
     Handle<GenParticleCollection> genParticles;
     iEvent.getByToken(genToken_, genParticles);

     mesonPt  = 0;
     mesonEta = 0;
     mesonP   = 0;
     mesonPx  = 0;
     mesonPy  = 0;
     mesonPz  = 0;
     mesonVxBirth = 0;
     mesonVyBirth = 0;
     mesonVzBirth = 0;
     mesonVxDeath = 0;
     mesonVyDeath = 0;
     mesonVzDeath = 0;
     tauPt  =  0;
     tauEta =  0;
     tauP   =  0;
     tauPx  =  0;
     tauPy  =  0;
     tauPz  =  0;
     tauVxBirth = 0;
     tauVyBirth = 0;
     tauVzBirth = 0;
     tauVxDeath = 0;
     tauVyDeath = 0;
     tauVzDeath = 0;
     nVisibleMu = 0;

     for (int i=0; i<3; i++)
       {
       muPdgId[i]	= 0;
       muPt[i] 		= 0; 
       muP[i] 		= 0; 
       muPx[i] 		= 0; 
       muPy[i] 		= 0; 
       muPz[i] 		= 0; 
       isMuVisible[i] 	= 0; 
       muVxBirth[i] 	= 0; 
       muVyBirth[i] 	= 0; 
       muVzBirth[i] 	= 0; 
       muVxDeath[i] 	= 0; 
       muVyDeath[i] 	= 0; 
       muVzDeath[i] 	= 0; 
       muEta[i] 	= 0; 
       muIdx[i]		= -1;
       muTrackId[i]	= -1;
       muStatus[i] 	= 0; 
       }

cout << "START LOOP on GEN PARTICLES" << endl;
       cout << "Nr. of genParticles: " << genParticles->size()+1 << endl;
cout << "Size of GenParticles: " << genParticles->size() << endl;
     for(size_t i = 0; i < genParticles->size(); ++ i) 
     {
       //if (v) cout << "GenParticle index: " << i << endl;
       int pidx = i;
       const GenParticle & p = (*genParticles)[i];
       //const Candidate * mom =( p.mother());

       if ( (fabs(p.pdgId()) != 511) && (fabs(p.pdgId()) != 15) && (fabs(p.pdgId()) != 431) ) continue;  //15=tau, 511=B0
       //selects only tau and B0
       if (v) 
         {
         cout  <<"id: "<<p.pdgId()<<"\tstatus: "<<p.status()
         <<"\npt: "<<p.pt()<<"\teta: "<<p.eta()
         <<"\npx: "<<p.px()<<"\tpy: "<<p.py()<<"\tpz: "<<p.pz()
         <<"\nphi: "<<p.phi()<<"\tmass: "<<p.mass()
         <<"\nvx: "<<p.vx()<<"\tvy "<<p.vy()
         <<"\tvz: "<<p.vz() << "\n" << endl;
         }

      if ( fabs(p.pdgId()) == 511 ||  fabs(p.pdgId()) == 431 )	//511=B0, 431=Ds+
      // set myB0decay=TRUE if a decay B0->tau + X is found
        {
	bool B0found = false;
	bool Dsfound = false;
	if ( fabs(p.pdgId()) == 511 )	{ B0found = true; nB0++; }
	if ( fabs(p.pdgId()) == 431 )	{ Dsfound = true; nDs++; }
        int n = p.numberOfDaughters();
	int dauPdgId[n];
        for(unsigned int j = 0; j < p.numberOfDaughters(); ++j) 
	  {
          const Candidate * d = p.daughter( j );
	  dauPdgId[j] = d->pdgId();
	  cout << "Daughter " << j << "is " << dauPdgId[j] << endl;
	  }
	bool myB0decay = false;
	bool myDsdecay = false;
	int n_tau = 0;
        for(unsigned int j = 0; j < p.numberOfDaughters(); ++ j) 
	  {
	  if (fabs(dauPdgId[j]) == 15 ) 	n_tau++;
	  }
	if (n_tau > 0 && B0found)		myB0decay = true;
	if (n_tau > 0 && Dsfound)		myDsdecay = true;
	if (myB0decay)		
	  {
	  N_myB0decay++;
	  nB0Tau++;
	  cout << "B0 -> tau FOUND!" << endl;
	  }
	if (myDsdecay)		
	  {
	  N_myDsdecay++;
	  nDsTau++;
	  cout << "Ds -> tau FOUND!" << endl;
	  }
        }


      if ( fabs(p.pdgId()) == 15 )  //15=tau
      // set myTauDecay = TRUE if a decay tau->3mu + X is found
        {
	nTau++;
	tauPdgId = p.pdgId();
        int n = p.numberOfDaughters();
	int dauPdgId[n];
	int dauStatus[n];
        for(unsigned int j = 0; j < p.numberOfDaughters(); ++j) 
	  {
          const Candidate * d = p.daughter( j );
	  dauPdgId[j] = d->pdgId();
	  dauStatus[j] = d->status();
	  cout << "Daughter " << j << "is " << dauPdgId[j] << endl;
	  cout << "status: " << dauStatus[j]  << endl;
	  }
	bool myTauDecay = false;
	//bool myChain	= false;
	int n_mu = 0;
        for(unsigned int j = 0; j < p.numberOfDaughters(); ++ j) 
	  {
	  if (fabs(dauPdgId[j]) == 13 ) 	n_mu++;		//13=mu
	  }
	if (n_mu == 3)		myTauDecay = true;
	if (myTauDecay)		
	  {
	  N_myLFVdecay++;
	  nTau3Mu++;
	  cout << "tau -> 3mu FOUND!" << endl;
	  }
        
	const Candidate * mom =( p.mother());
	if (myTauDecay && (fabs(mom->pdgId()) == 511) )	
	  {
	  //myChain = true;
	  N_myB0Chain++;
	  nB0Tau3Mu++;
	  cout << "B0 -> tau -> 3mu FOUND!" << endl;
	  mesonPt = mom->pt();
	  mesonEta = mom->eta();
	  mesonP = mom->p();
	  mesonPx = mom->px();
	  mesonPy = mom->py();
	  mesonPz = mom->pz();
	  mesonVxBirth = mom->vx();
	  mesonVyBirth = mom->vy();
	  mesonVzBirth = mom->vz();
	  mesonVxDeath = p.vx();
	  mesonVyDeath = p.vy();
	  mesonVzDeath = p.vz();
	  tauPt = p.pt();
	  tauEta = p.eta();
	  tauP = p.p();
	  tauPx = p.px();
	  tauPy = p.py();
	  tauPz = p.pz();
	  tauVxBirth = p.vx();
	  tauVyBirth = p.vy();
	  tauVzBirth = p.vz();
	      const Candidate *gdaugh = p.daughter( 0 );
	  tauVxDeath = gdaugh->vx();
	  tauVyDeath = gdaugh->vy();
	  tauVzDeath = gdaugh->vz();
	  }
	if (myTauDecay && (fabs(mom->pdgId()) == 431) )	
	  {
	  //myChain = true;
	  N_myDsChain++;
	  nDsTau3Mu++;
	  cout << "Ds -> tau -> 3mu FOUND!" << endl;
	  mesonPt = mom->pt();
	  mesonEta = mom->eta();
	  mesonP = mom->p();
	  mesonPx = mom->px();
	  mesonPy = mom->py();
	  mesonPz = mom->pz();
	  mesonVxBirth = mom->vx();
	  mesonVyBirth = mom->vy();
	  mesonVzBirth = mom->vz();
	  mesonVxDeath = p.vx();
	  mesonVyDeath = p.vy();
	  mesonVzDeath = p.vz();
	  tauPt = p.pt();
	  tauEta = p.eta();
	  tauP = p.p();
	  tauPx = p.px();
	  tauPy = p.py();
	  tauPz = p.pz();
	  tauVxBirth = p.vx();
	  tauVyBirth = p.vy();
	  tauVzBirth = p.vz();
	      const Candidate *gdaugh = p.daughter( 0 );
	  tauVxDeath = gdaugh->vx();
	  tauVyDeath = gdaugh->vy();
	  tauVzDeath = gdaugh->vz();
	  }

	//Fill histograms
	if ( (myTauDecay && (fabs(mom->pdgId()) == 511)) || (myTauDecay && (fabs(mom->pdgId()) == 431))  )
	  {
	  //float muEta[3];
	  //float muPt[3];o
	  mesonPdgId = mom->pdgId();
	  //int muPdgId[3];
	  unsigned int i=0;
          for(unsigned int j = 0; j < p.numberOfDaughters(); ++j) 
	    {
            const Candidate * d = p.daughter( j );
	    if (fabs(d->pdgId())!=13)	continue;
	    muPdgId[i] = d->pdgId();
	    muPt[i] = d->pt();
	    muP[i] = d->p();
	    muPx[i] = d->px();
	    muPy[i] = d->py();
	    muPz[i] = d->pz();
	    muVxBirth[i] = d->vx();
	    muVyBirth[i] = d->vy();
	    muVzBirth[i] = d->vz();
	    muEta[i] = fabs(d->eta());
	    muStatus[i] = d->status();
	    //muGenPind[i] = d->index();
	    //cout << "	!!! MU PARTICLE INDEX !!!   " <<  (d->track()).size() << endl;
            //if (muStatus[i]==1)
	    //  {
	    //  muVxDeath[i] = 0;
	    //  muVyDeath[i] = 0;
	    //  muVzDeath[i] = 0;
	    //  }

	    if (muStatus[i]!=1)
	      {
	      const Candidate *gdaugh = d->daughter( 0 );
	      muVxDeath[i] = gdaugh->vx();
	      muVyDeath[i] = gdaugh->vy();
	      muVzDeath[i] = gdaugh->vz();


	    //canDeleteFromHere
	      for( size_t i = 0; i < d->numberOfDaughters(); ++ i ) 
	        {
	        const Candidate * gdaugh = d->daughter( i );
		cout << "VERTEX: " << gdaugh->vx()  << ", " << gdaugh->vy() << ", " << gdaugh->vz() << endl;
	        }
	    //canDeleteToHere
	      }
	    i++;
	    }
	    
            //cout << "\nTRY to compare particle taken from index with tau children" << endl;
	    //cout << "current mom i = " << pidx << endl;
	    //cout << "full chain: " << (*genParticles)[pidx-1].pdgId() << " -> " << (*genParticles)[pidx].pdgId() << " -> 3mu " << endl;
	    //for ( int back = 1 ; back<pidx ; back ++)  
	    //   {
	    //   cout << "particle -" << back << "from tau:\t" << (*genParticles)[pidx-back].pdgId() << endl;
	    //   if ( fabs( (*genParticles)[pidx-back].pdgId() ) == 511 )/*||  fabs( (*genParticles)[pidx-back].pdgId() ) == 431 )*/ break;
	    //   }
            //const GenParticle & p1 = (*genParticles)[pidx+1];
	    //cout << "p1 eta= " << p1.eta() << "   child eta= " << muEta[0] << endl;
            //const GenParticle & p2 = (*genParticles)[pidx+2];
	    //cout << "p2 eta= " << p2.eta() << "   child eta= " << muEta[1] << endl;
            //const GenParticle & p3 = (*genParticles)[pidx+3];
	    //cout << "p2 eta= " << p3.eta() << "   child eta= " << muEta[2] << "\n" << endl;
	    //muIdx[0] = pidx+1;
	    //muIdx[1] = pidx+2;
	    //muIdx[2] = pidx+3;
	    //cout << "MOM vertex: " << p.vertex() << endl;
	    //cout << "MUON1 vertex: " << p1.vertex() << endl;
	    //cout << "MUON2 vertex: " << p2.vertex() << endl;
	    //cout << "MUON3 vertex: " << p3.vertex() << endl;

	  for (unsigned int i = 0; i < 3; i++)	h_PtVsEta->Fill( muEta[i], muPt[i] );
          //std::sort(muPt, muPt+3);
          //std::sort(muEta, muEta+3);

	  // sort arrays according to increasing Pt
	  int imax = distance(muPt, max_element(muPt, muPt + 3));
	  std::swap(muPt[2], muPt[imax]);
	  std::swap(muEta[2], muEta[imax]);
	  std::swap(muStatus[2], muStatus[imax]);
	  std::swap(muPdgId[2], muPdgId[imax]);
	  std::swap(muP[2], muP[imax]);
	  std::swap(muPx[2], muPx[imax]);
	  std::swap(muPy[2], muPy[imax]);
	  std::swap(muPz[2], muPz[imax]);
	  std::swap(muVxBirth[2], muVxBirth[imax]);
	  std::swap(muVyBirth[2], muVyBirth[imax]);
	  std::swap(muVzBirth[2], muVzBirth[imax]);
	  std::swap(muVxDeath[2], muVxDeath[imax]);
	  std::swap(muVyDeath[2], muVyDeath[imax]);
	  std::swap(muVzDeath[2], muVzDeath[imax]);
	  //std::swap(muIdx[2], muIdx[imax]);
	  int imin = distance(muPt, min_element(muPt, muPt + 3));
	  std::swap(muPt[0], muPt[imin]);
	  std::swap(muEta[0], muEta[imin]);
	  std::swap(muStatus[0], muStatus[imin]);
	  std::swap(muPdgId[0], muPdgId[imin]);
	  std::swap(muP[0], muP[imin]);
	  std::swap(muPx[0], muPx[imin]);
	  std::swap(muPy[0], muPy[imin]);
	  std::swap(muPz[0], muPz[imin]);
	  std::swap(muVxBirth[0], muVxBirth[imin]);
	  std::swap(muVyBirth[0], muVyBirth[imin]);
	  std::swap(muVzBirth[0], muVzBirth[imin]);
	  std::swap(muVxDeath[0], muVxDeath[imin]);
	  std::swap(muVyDeath[0], muVyDeath[imin]);
	  std::swap(muVzDeath[0], muVzDeath[imin]);
	  //std::swap(muIdx[0], muIdx[imin]);
	  // end sort arrays together

	  h_Eta2D_max->Fill(muEta[2], muEta[1]) ; 		//(highest-pt eta, second highest-pt eta)
	  h_Eta2D_min->Fill(muEta[0], muEta[1]) ; 		//(smallest-pt eta, second highest-pt eta)
	  h_Eta3D->Fill(muEta[2], muEta[1], muEta[0]) ;		//(highest-pt eta, second highest-pt eta, smallest-pt eta)
	  h_Pt2D_max->Fill(muPt[2], muPt[1]) ; 			//(highest pt, second highest pt)
	  h_Pt2D_min->Fill(muPt[0], muPt[1]) ; 			//(smallest pt, second highest pt)
	  h_Pt3D->Fill(muPt[2], muPt[1], muPt[0]) ; 		//(highest pt, second highest pt, smallest pt)
          for (auto i=0; i<3; i++)  cout << muPt[i] << "\t" << muEta[i] << "\t" << muPdgId[i] << endl;	

	  for (auto i=0; i<3; i++)
	    {
	    if ( fabs(muEta[i])>1.8 && fabs(muEta[i])<3.0 )
	      {
	      mu_isME0[i] = 1;
	      nME0++;
	      if (muEta[i]>0)	{ mu_isME0pos[i] = 1; 	nME0_pos++; }
	      else		{ mu_isME0neg[i] = 1;	nME0_neg++; }
	      }
	    }
	  diffEta_32 = muEta[2]-muEta[1];
	  diffEta_31 = muEta[2]-muEta[0];
	  diffEta_21 = muEta[1]-muEta[0];
	  int jmax = distance(muEta, max_element(muEta, muEta + 3));
	  int jmin = distance(muEta, min_element(muEta, muEta + 3));
	  int jmed = 3-jmax-jmin;
	  diffEta_maxmed=muEta[jmax]-muEta[jmed];
	  diffEta_maxmin=muEta[jmax]-muEta[jmin];
	  diffEta_medmin=muEta[jmed]-muEta[jmin];
	  }
        }
      // << "\nmother: "<<p.mother()<<"\tMOTHER id: "<<mom->pdgId()
      // <<"\tMOTHER pt:"<<mom->pt()<<endl<<endl;

     // if ( fabs(p.eta()) < 2.0 || fabs(p.eta()) > 2.8 ) return;
     // if ( fabs(p.pt()) < 0.5 ) return;

     }


     float muPtlocal[3] = { 0,0,0 };
     int nMu = 0;
     for(size_t i = 0; i < genParticles->size(); ++ i) 
     {
       //if (v) cout << "GenParticle index: " << i << endl;
       const GenParticle & p = (*genParticles)[i];
       if ( fabs( p.pdgId() ) != 13 ) continue;
       bool found = 0;
       bool foundlast = 0;
       const Candidate * momtemp =( p.mother());
       Candidate * mom = momtemp->clone();
       // run back the mothers until a non-muon is found
       while ( fabs(mom->pdgId()) == 13 )
         {
	 const Candidate * momtemp1 = mom->mother();
         mom = momtemp1->clone();
	 }
	 	
       // check if the mother is a tau, and its mother is a B0 or Ds	--> if yes "found" is set to "true"
       const Candidate * momtemp1 = mom->mother();
       Candidate * mom2 = momtemp1->clone();
       if ( fabs(mom->pdgId())==15 && ( fabs(mom2->pdgId())==511 || fabs(mom2->pdgId())==431 ) ) 
	  {
          cout << "meson -> tau -> 3mu found" << endl;
          found = 1;
	  }

       // verify that p is the last muon in the chain  --> if yes "foundlast" is set to "true"
       if (p.numberOfDaughters()==0)    //easy case: the muon has no daughters
       {
       foundlast=1;       
       cout << "This muon has no daughters" << endl;
       }
       else	            //other case: the muon has daughters. Need NONE of them being a muon.
       {
       cout << "This muon has daugters" << endl;
       foundlast = 1;
       for(unsigned int j = 0; j < p.numberOfDaughters(); ++j)
         {
         const Candidate * d = p.daughter( j );
         if ( fabs(d->pdgId()) == 13 ) foundlast = 0;
         }
       }
       if (found && foundlast) //this is my muon if "found" and "foundlast" are verified. I store the particle index
         {
         muIdx[nMu] = i;
	 muPtlocal[nMu] = p.pt();
         nMu++;
         }
       if (nMu==3) break;
      }
     cout << "Muon indexes: " << muIdx[0] << "  " << muIdx[1] << "  " << muIdx[2] << endl;
     cout << "Muon pt: " << muPtlocal[0] << "  " << muPtlocal[1] << "  " << muPtlocal[2] << endl;
	  // sort arrays according to increasing Pt
	  int imax = distance(muPtlocal, max_element(muPtlocal, muPtlocal + 3));
	  std::swap(muIdx[2], muIdx[imax]);
	  std::swap(muPtlocal[2], muPtlocal[imax]);
	  int imin = distance(muPtlocal, min_element(muPtlocal, muPtlocal + 3));
	  cout << "imin=" << imin << "  imax=" << imax << endl;
	  std::swap(muIdx[0], muIdx[imin]);
	  std::swap(muPtlocal[0], muPtlocal[imin]);
     //cout << "Ordered muon indexes: " << muIdx[0] << "  " << muIdx[1] << "  " << muIdx[2] << endl;
     //cout << "Ordered muon pt: " << muPtlocal[0] << "  " << muPtlocal[1] << "  " << muPtlocal[2] << endl;

//h_PtVsEta	->Draw("COLZ");
//h_Eta2D_max	->Draw("COLZ");
//h_Eta2D_min	->Draw("COLZ");
//h_Eta3D		->Draw("COLZ");
//h_Pt2D_max	->Draw("COLZ");
//h_Pt2D_min	->Draw("COLZ");
//h_Pt3D		->Draw("COLZ");

     Handle<vector<SimTrack>> simTracksH;
     iEvent.getByToken(simTrackToken_, simTracksH);
     int count = 0;
     for ( auto itk = simTracksH->begin(); itk != simTracksH->end(); ++itk )
      {
      int trackGenPIdx = (*itk).genpartIndex();
      int trackId = (*itk).trackId();
      //cout << "current track.genpartIndex = " << trackGenPIdx << endl;
      if       (trackGenPIdx == muIdx[0]+1)
        {
	cout << "Track of 1st muon found" << endl;
	muTrackId[0] = trackId;
	}
      else if ( trackGenPIdx == muIdx[1]+1 )
        {
	cout << "Track of 2nd muon found" << endl;
	muTrackId[1] = trackId;
	}
      else if ( trackGenPIdx == muIdx[2]+1 ) 
        {
	cout << "Track of 3rd muon found" << endl;
	muTrackId[2] = trackId;
	}
      else   continue;

      //adding also a check on the muType
      if ( fabs((*itk).type()) != 13 ) 
        {
        cout << "ERROR, the track's particle type is NOT a muon!" << endl;
        return;
        }
      }
   cout << "Trackids of SimTracks matched to muons: " << muTrackId[0] << "  " << muTrackId[1] << "  " << muTrackId[2] << endl;

   Handle<vector<PSimHit>> me0simHitH;
   iEvent.getByToken(ME0SimHitToken_, me0simHitH);

   //loop on simihits to determine the farthest simhit in ME0 for each of the three muons
   double muMax_z[3] = {-1000, -1000, -1000};
   //bool atLeastOne;   

   for ( auto it = me0simHitH->begin(); it != me0simHitH->end(); ++it )
   { 
     for ( int i=0; i<3; i++ )
     {
       //consider only hits of muons
       if ( fabs( (*it).particleType() ) != 13 )	continue; 
       
       int idtrack = (*it).trackId();
       
       //verify there is at least one correspondence, 
       //if not the muon has not arrived in ME0
       //atLeastOne = false;
       
       if ( idtrack == muTrackId[i]  )
       { 
       isMuVisible[i] = 1;
       ME0DetId * shMe0id = new ME0DetId( (*it).detUnitId() );
       if (  (*it).entryPoint().z() > -0.298 )    // ||  (*it).exitPoint().z() < 0.29  )
         {
	 //float ytmp = (*it).entryPoint().y()*3
	   h_z_strange->Fill( (*it).entryPoint().z() );
	   if ( shMe0id->roll()==1 )			h_z_strangeEP1->Fill( (*it).entryPoint().z() );
	   if ( shMe0id->roll()==2 )			h_z_strangeEP2->Fill( (*it).entryPoint().z() );
	   if ( shMe0id->roll()==3 )			h_z_strangeEP3->Fill( (*it).entryPoint().z() );
	   if ( shMe0id->roll()==4 )			h_z_strangeEP4->Fill( (*it).entryPoint().z() );
	   if ( shMe0id->roll()==5 )			h_z_strangeEP5->Fill( (*it).entryPoint().z() );
	   if ( shMe0id->roll()==6 )			h_z_strangeEP6->Fill( (*it).entryPoint().z() );
	   if ( shMe0id->roll()==7 )			h_z_strangeEP7->Fill( (*it).entryPoint().z() );
	   if ( shMe0id->roll()==8 )			h_z_strangeEP8->Fill( (*it).entryPoint().z() );

           h_xy_strange->Fill( (*it).entryPoint().x(), (*it).entryPoint().y() );
	   if ( shMe0id->roll()==1 )			h_xy_strangeEP1->Fill( (*it).entryPoint().x(), (*it).entryPoint().y()  );
	   if ( shMe0id->roll()==2 )			h_xy_strangeEP2->Fill( (*it).entryPoint().x(), (*it).entryPoint().y()  );
	   if ( shMe0id->roll()==3 )			h_xy_strangeEP3->Fill( (*it).entryPoint().x(), (*it).entryPoint().y()  );
	   if ( shMe0id->roll()==4 )			h_xy_strangeEP4->Fill( (*it).entryPoint().x(), (*it).entryPoint().y()  );
	   if ( shMe0id->roll()==5 )			h_xy_strangeEP5->Fill( (*it).entryPoint().x(), (*it).entryPoint().y()  );
	   if ( shMe0id->roll()==6 )			h_xy_strangeEP6->Fill( (*it).entryPoint().x(), (*it).entryPoint().y()  );
	   if ( shMe0id->roll()==7 )			h_xy_strangeEP7->Fill( (*it).entryPoint().x(), (*it).entryPoint().y()  );
	   if ( shMe0id->roll()==8 )			h_xy_strangeEP8->Fill( (*it).entryPoint().x(), (*it).entryPoint().y()  );
	   }
       if (  (*it).exitPoint().z() < 0.29 )         
	 {
	   h_z_strange->Fill( (*it).exitPoint().z() );
	   if ( shMe0id->roll()==1 )			h_z_strangeEP1->Fill( (*it).exitPoint().z() );
	   if ( shMe0id->roll()==2 )			h_z_strangeEP2->Fill( (*it).exitPoint().z() );
	   if ( shMe0id->roll()==3 )			h_z_strangeEP3->Fill( (*it).exitPoint().z() );
	   if ( shMe0id->roll()==4 )			h_z_strangeEP4->Fill( (*it).exitPoint().z() );
	   if ( shMe0id->roll()==5 )			h_z_strangeEP5->Fill( (*it).exitPoint().z() );
	   if ( shMe0id->roll()==6 )			h_z_strangeEP6->Fill( (*it).exitPoint().z() );
	   if ( shMe0id->roll()==7 )			h_z_strangeEP7->Fill( (*it).exitPoint().z() );
	   if ( shMe0id->roll()==8 )			h_z_strangeEP8->Fill( (*it).exitPoint().z() );
	   
           h_xy_strange->Fill( (*it).exitPoint().x(), (*it).exitPoint().y() );
	   if ( shMe0id->roll()==1 )			h_xy_strangeEP1->Fill( (*it).exitPoint().x(), (*it).exitPoint().y()  );
	   if ( shMe0id->roll()==2 )			h_xy_strangeEP2->Fill( (*it).exitPoint().x(), (*it).exitPoint().y()  );
	   if ( shMe0id->roll()==3 )			h_xy_strangeEP3->Fill( (*it).exitPoint().x(), (*it).exitPoint().y()  );
	   if ( shMe0id->roll()==4 )			h_xy_strangeEP4->Fill( (*it).exitPoint().x(), (*it).exitPoint().y()  );
	   if ( shMe0id->roll()==5 )			h_xy_strangeEP5->Fill( (*it).exitPoint().x(), (*it).exitPoint().y()  );
	   if ( shMe0id->roll()==6 )			h_xy_strangeEP6->Fill( (*it).exitPoint().x(), (*it).exitPoint().y()  );
	   if ( shMe0id->roll()==7 )			h_xy_strangeEP7->Fill( (*it).exitPoint().x(), (*it).exitPoint().y()  );
	   if ( shMe0id->roll()==8 )			h_xy_strangeEP8->Fill( (*it).exitPoint().x(), (*it).exitPoint().y()  );
	 }

       cout << "\ni=" << i << "\tcurrent entryPoint z= " << (*it).entryPoint().z() << "\tx= " << (*it).entryPoint().x() << "\ty= " << (*it).entryPoint().y() << endl;
       cout 	           << "\tcurrent exitPoint z= " << (*it).exitPoint().z()   << "\tx= " << (*it).exitPoint().x()  << "\ty= " << (*it).exitPoint().y()  << endl;
       cout                << "\tand muMax_z= " << muMax_z[i] << endl; //MR
       cout << "ME0DetId: endcap= " <<  shMe0id->region()  << " chamber= " << shMe0id->chamber() << " layer= " << shMe0id->layer() << endl;
         //atLeastOne = true;
         //identify the farthest hit for the current muon
         if ( muMax_z[i] < (*it).entryPoint().z() )
         {
           muMax_z[i] = (*it).entryPoint().z();  
           cout << "maxz changed" << muMax_z[i] << " i=" << i << endl;
         }
       
       }
        
     //cout << atLeastOne << endl;  
     } 
   }      
   h_xy_strange->SaveAs("h_xy_strange.root");
   h_z_strange->SaveAs("h_z_strange.root");
   
   for (int i=0; i<3; i++)
   {
     if ( muMax_z[i] == -1000 )
     {
     cout << "The " << i << "-th mu didn't arrive in ME0!" << endl; 
     }
     else 
     {
     cout << "Max_z of " << i << "-th mu in ME0:" << muMax_z[i] << endl;  
     }
     if (isMuVisible[i])  nVisibleMu++;
   }
   
   for ( auto it = me0simHitH->begin(); it != me0simHitH->end(); ++it )
   {
   if ( fabs( (*it).particleType() ) != 13 )	continue;
//   Local3DPoint locPosSH = (*it).localPosition();
//   Local3DPoint entryPoint =   (*it).entryPoint();
//   Local3DPoint exitPoint  =   (*it).exitPoint();
   ME0DetId me0id = ME0DetId::DetId( (*it).detUnitId() );
   (*deltaChamber).push_back(me0id.chamber());
   }

   //calculate average chamber
   Int_t ent=0;
   Float_t av=0.00;
   for ( auto chit=(*deltaChamber).begin() ; chit!=(*deltaChamber).end() ; ++chit)
     {
     ent++;
     av=av+(*chit);
     cout << "Chamber before shift: " << (*chit) << endl;
     }
   if (ent!=0) av=av/ent;
   cout << "Average chamber: " << av << endl;
   //shift chambers to be centered in the average
   for ( auto chit=(*deltaChamber).begin() ; chit!=(*deltaChamber).end() ; ++chit)
     {
     (*chit)=(*chit)-av;
     cout << "Chamber after shift: " << (*chit) << endl;
     }


   ESHandle<ME0Geometry> me0Geom;
   iSetup.get<MuonGeometryRecord>().get(me0Geom);
   Handle<ME0SegmentCollection> me0segmentH;
   iEvent.getByToken(ME0SegmentToken_, me0segmentH);

   deltaPhiMap.clear();
   alphaMap.clear();
   thetaMap.clear();
   (*me0List).clear();
   (*etaPartList).clear();
   (*nSegmRecHitList).clear();
   (*qualityList).clear();
   (*deltaPhiList).clear();
   (*xOriginList).clear();
   (*yOriginList).clear();
   (*zOriginList).clear();
   (*globalPhiOriginList).clear();
   (*globalEtaOriginList).clear();
   (*globalPhiLayer1List).clear();
   (*globalEtaLayer1List).clear();
   (*globalPhiLayer2List).clear();
   (*globalEtaLayer2List).clear();
   (*deltaGlobalPhiLayer21List).clear();
   (*averageTofList).clear();
   (*alphaList).clear();
   (*thetaList).clear();
   //(*muPdgId).clear();
   //(*muPt).clear();
   //(*muEta).clear();
   //(*muPx).clear();
   //(*muPy).clear();
   //(*muPz).clear();
   nSegments = 0;

   nEvent++;
   if (nME0>0) nEventSel++;

   //bool is_singleME0[5], is_doubleME0[15];
   for ( int i=0; i<5; i++)	
     {
     is_singleME0[i] = false;
     is_singleME0Quality0[i] = false;
     is_singleME0Quality1[i] = false;
     is_singleME0Quality2[i] = false;
     is_singleME0Veto0[i] = false;
     is_singleME0Veto1[i] = false;
     is_singleME0Veto2[i] = false;
     is_singleME0Veto3[i] = false;
     is_singleME0Quality0Veto0[i] = false;
     is_singleME0Quality0Veto1[i] = false;
     is_singleME0Quality0Veto2[i] = false;
     is_singleME0Quality0Veto3[i] = false;
     is_singleME0Quality1Veto0[i] = false;
     is_singleME0Quality1Veto1[i] = false;
     is_singleME0Quality1Veto2[i] = false;
     is_singleME0Quality1Veto3[i] = false;
     is_singleME0Quality2Veto0[i] = false;
     is_singleME0Quality2Veto1[i] = false;
     is_singleME0Quality2Veto2[i] = false;
     is_singleME0Quality2Veto3[i] = false;
     multiplicity[i] = 0;
     }
   for ( int i=0; i<15; i++)	{
     				is_doubleME0[i] = false;
     				is_doubleME0near[i] = false;
     				is_doubleME0nearQuality0[i] = false;
     				is_doubleME0nearQuality1[i] = false;
     				is_doubleME0nearQuality2[i] = false;
     				is_doubleME0nearVeto0[i] = false;
     				is_doubleME0nearVeto1[i] = false;
     				is_doubleME0nearVeto2[i] = false;
     				is_doubleME0nearVeto3[i] = false;
     				is_doubleME0nearQuality0Veto0[i] = false;
     				is_doubleME0nearQuality0Veto1[i] = false;
     				is_doubleME0nearQuality0Veto2[i] = false;
     				is_doubleME0nearQuality0Veto3[i] = false;
     				is_doubleME0nearQuality1Veto0[i] = false;
     				is_doubleME0nearQuality1Veto1[i] = false;
     				is_doubleME0nearQuality1Veto2[i] = false;
     				is_doubleME0nearQuality1Veto3[i] = false;
     				is_doubleME0nearQuality2Veto0[i] = false;
     				is_doubleME0nearQuality2Veto1[i] = false;
     				is_doubleME0nearQuality2Veto2[i] = false;
     				is_doubleME0nearQuality2Veto3[i] = false;
     				is_doubleME0close[i] = false;
				}
   for ( int i=0; i<35; i++)	{
   				is_tripleME0[i] = false;
   				is_tripleME0near[i] = false;
   				is_tripleME0nearQuality0[i] = false;
   				is_tripleME0nearQuality1[i] = false;
   				is_tripleME0nearQuality2[i] = false;
   				is_tripleME0nearVeto0[i] = false;
   				is_tripleME0nearVeto1[i] = false;
   				is_tripleME0nearVeto2[i] = false;
   				is_tripleME0nearVeto3[i] = false;
   				is_tripleME0nearQuality0Veto0[i] = false;
   				is_tripleME0nearQuality0Veto1[i] = false;
   				is_tripleME0nearQuality0Veto2[i] = false;
   				is_tripleME0nearQuality0Veto3[i] = false;
   				is_tripleME0nearQuality1Veto0[i] = false;
   				is_tripleME0nearQuality1Veto1[i] = false;
   				is_tripleME0nearQuality1Veto2[i] = false;
   				is_tripleME0nearQuality1Veto3[i] = false;
   				is_tripleME0nearQuality2Veto0[i] = false;
   				is_tripleME0nearQuality2Veto1[i] = false;
   				is_tripleME0nearQuality2Veto2[i] = false;
   				is_tripleME0nearQuality2Veto3[i] = false;
   				is_tripleME0close[i] = false;
				}

   float thr[5]={-1,-1,-1,-1,-1};
   //thresholds
   if (reco && mutype) //prompt mu, digi based segment
     {
     float thrT[5] = { 0.01264 , 0.00734 , 0.00366 , 0.00206 , 0.00144 } ;
     for (int tr=0; tr<5; tr++) thr[tr]=thrT[tr];
     }
   
   else if (!reco && mutype) //prompt mu, pad based segment
     {
     float thrT[5] = { 0.0128 , 0.00732 , 0.00398 , 0.00526 , 0.00206 };
     for (int tr=0; tr<5; tr++) thr[tr]=thrT[tr];
     }

   else if (reco && !mutype) //disp mu, digi based segment
     {
     float thrT[5] = { 0.0145 , 0.00952 , 0.0064 , 0.00542 , 0.00452 };
     for (int tr=0; tr<5; tr++) thr[tr]=thrT[tr];
     }

   else if (!reco && !mutype) //disp mu, pad based segment
     {
     float thrT[5] = { 0.01442 , 0.00932 , 0.0066 , 0.00526 , 0.00472 };
     for (int tr=0; tr<5; tr++) thr[tr]=thrT[tr];
     }
   else 
     {
     cout << "ERROR! Check thresholds configuration." << endl;
     return;
     }


   //eta partitions - local y limits
   float etaPart[9] = { 0 , 14.7606 , 14.7606*2 , 14.7606+14.7606+11.6926 , 14.7606+14.7606+11.6926+11.6926 ,
   		  14.7606+14.7606+11.6926+11.6926+9.3132 , 14.7606+14.7606+11.6926+11.6926+9.3132+9.3132 ,
		  14.7606+14.7606+11.6926+11.6926+9.3132+9.3132+7.6426 , 14.7606+14.7606+11.6926+11.6926+9.3132+9.3132+7.6426+7.6426 };
   for (int ieta=0; ieta<9; ieta++)	etaPart[ieta] = etaPart[ieta]-(etaPart[8]/2) ;

//========== INSERTED FROM HERE ========================
//------------------- SEGMENT --------------------------------------  
     ME0SegmentCollection::const_iterator sg_it;
     estimatedNmu = 0;
     nSegments = me0segmentH->size();

if ( (nME0>0 && signal) || !signal ) {//run trigger analysis only if there is at least one mu from the tau decay in the eta range 1.8<|eta!|<3 

     for ( sg_it = me0segmentH->begin(); sg_it !=me0segmentH->end(); ++sg_it)
        {
	ME0DetId segmMe0Id = (*sg_it).me0DetId();
	int endcap = segmMe0Id.region();
        int segm_chamber = segmMe0Id.chamber();

        const std::vector<ME0RecHit>& segmRHits = (*sg_it).specificRecHits();
        (*nSegmRecHitList).push_back(segmRHits.size());
	Int_t qtmp = -10;
	if (segmRHits.size() == 6 )	qtmp=2;
	if (segmRHits.size() == 5 )	qtmp=1;
	if (segmRHits.size() == 4 )	qtmp=0;
	(*qualityList).push_back(qtmp);
	float avTofTmp = 0.0;
	float entr = 0.0;
	cout << "TOF of segment rechits:" << endl;
	for ( auto srh=segmRHits.begin(); srh!=segmRHits.end(); ++srh ) //loop over this segment rechits
	  {
	  cout << (*srh).tof() << endl;
	  avTofTmp = avTofTmp+(*srh).tof();
	  entr = entr+1.0;
	  }
	avTofTmp=avTofTmp/entr;
	(*averageTofList).push_back(avTofTmp);

	bool foundRH = false;

	for ( auto prev_it=me0segmentH->begin(); prev_it!=sg_it; ++prev_it) //loop over previous segments
	{
        const std::vector<ME0RecHit>& prevSegRH = (*prev_it).specificRecHits();
	for ( auto srh=segmRHits.begin(); srh!=segmRHits.end(); ++srh ) //loop over this segment rechits
	  {
	  for (auto prev = prevSegRH.begin() ; prev!=prevSegRH.end(); ++prev) //loop over rechits of a previous segment
	    {
            if (  ((*prev).me0Id() 			== (*srh).me0Id()) 
	       && ((*prev).localPosition() 		== (*srh).localPosition()) 
	      // && ((*prev).localPositionError() 	== (*srh).localPositionError())
	       && ((*prev).tof() 			== (*srh).tof())        
	       )	foundRH = true;
	    }
	  }
	}
	if (!foundRH)	estimatedNmu++;
	

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

        (*xOriginList).push_back(par[2]);
        (*yOriginList).push_back(par[3]);
        (*zOriginList).push_back(theOrigin.z());
	cout << "yOrigin = " << par[3] << endl;
	int etaParTmp = 1;
	for (int ieta=7; ieta>=0; ieta--)
	  {
	  cout << "limits: " << etaPart[ieta] << " - " << etaPart[ieta+1] << endl;
          if ( par[3] > etaPart[ieta] && par[3] < etaPart[ieta+1] )
	    {
            (*etaPartList).push_back(etaParTmp);
	    cout << "Origin eta partition: " << etaParTmp << endl;
	    break;
	    }
	  else if (ieta==0)	(*etaPartList).push_back(-10);  //assign eta partition -10 (not existing) if eta partition is not found
	  etaParTmp++;
	  }
	cout << "Inserted item: " << *((*etaPartList).end()-1) << endl;

        cout << "PARAMETERS: " << par[0] << "  " << par[1] << "  " << par[2] << "  " << par[3] << endl;
        cout << "ORIGIN:  phi " << theOrigin.phi() << endl;
        cout << "ORIGIN:  z " << theOrigin.z() << endl;

        //Get all coordinates
	ME0DetId layer1( ((*sg_it).me0DetId()).region(), 1, ((*sg_it).me0DetId()).chamber(), 4 ) ; //use only for x,z coordinates!
        const ME0EtaPartition * rollLayer1 = me0Geom->etaPartition(layer1);	
	// cout << "Segment ME0ID: " << (*sg_it).me0DetId()  << endl;
	const ME0Chamber * chamber = me0Geom->chamber( (*sg_it).me0DetId() );
	Local3DPoint OriginLP( par[2], par[3], theOrigin.z() );
	Local3DPoint centerLP( 0,0,0 ); //center of chamber
	GlobalPoint  centerGP = chamber->toGlobal( centerLP );
	Local3DPoint centerFromL1 = rollLayer1->toLocal( centerGP ); //use only for x,z coordinates!
	Local3DPoint theEntryLP (par[2] - (centerFromL1.z()*par[0]) , par[3] - (centerFromL1.z()*par[1]) , -centerFromL1.z());//in chamber frame
	Local3DPoint theEnd     (par[2] + (centerFromL1.z()*par[0]) , par[3] + (centerFromL1.z()*par[1]) ,  centerFromL1.z());//in chamber frame
	GlobalPoint  IpGP( 0,0,0 );
        Local3DPoint IpLP(chamber->toLocal(IpGP) );
	GlobalPoint  theEntryGP = chamber->toGlobal( theEntryLP );
	GlobalPoint  theEndGP = chamber->toGlobal( theEnd );
	GlobalPoint  OriginGP = chamber->toGlobal( OriginLP );
        (*globalPhiOriginList).push_back(OriginGP.phi());
        (*globalEtaOriginList).push_back(OriginGP.eta());
        (*globalPhiLayer1List).push_back(theEntryGP.phi());
        (*globalEtaLayer1List).push_back(theEntryGP.eta());
        (*globalPhiLayer2List).push_back(theEndGP.phi());
        (*globalEtaLayer2List).push_back(theEndGP.eta());
        (*deltaGlobalPhiLayer21List).push_back(theEndGP.phi()-theEntryGP.phi());
	float sgDeltaGlobalPhi = theEndGP.phi()-theEntryGP.phi();

	//Calculation in LOCAL coordinates
	float vectorOriginX = theEntryLP.x()- IpLP.x();
	float vectorOriginZ = theEntryLP.z()- IpLP.z();
	float segmentDeltaX = theEnd.x()    - theEntryLP.x();
	float segmentDeltaY = theEnd.y()    - theEntryLP.y();
	float segmentDeltaZ = theEnd.z()    - theEntryLP.z();
	cout << "IP local coord:  " << IpLP.x() << ", " << IpLP.y() << ", " << IpLP.z() << endl;
	cout << "theEntry local coord:  " << theEntryLP.x() << ", " << theEntryLP.y() << ", " << theEntryLP.z() << endl;
	cout << "segmentDeltaX, segmentDeltaY(local coord):  " << segmentDeltaX << ", " << segmentDeltaY << ", " << segmentDeltaZ << endl;
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

 	//Saving results
        deltaPhiMap[segmMe0Id].push_back(sgDeltaPhiLocal3);
	alphaMap[segmMe0Id].push_back(alpha3);
	thetaMap[segmMe0Id].push_back(theta3);

	(*me0List).push_back(segmMe0Id.rawId());
	(*deltaPhiList).push_back(sgDeltaPhiLocal3);
	(*alphaList).push_back(alpha3);
	(*thetaList).push_back(theta3);

	//Printing results:
	cout << "Segment:\nME0Id:"<< segmMe0Id << "\trawId: " << segmMe0Id.rawId() << "\tdeltaPhi: " 
	     << sgDeltaPhiLocal3 << "\talpha:" << alpha3 << "\ttheta:" << theta3 << "\n" << endl;

        for ( int pt_ind = 0; pt_ind < 5 ; pt_ind++)
          {
	  if ( fabs(sgDeltaGlobalPhi) < thr[pt_ind] )	
	    {
	    is_singleME0[pt_ind] = true;
	    multiplicity[pt_ind]++;
	    if ( qtmp>= 0 )  is_singleME0Quality0[pt_ind] = true; 
	    if ( qtmp>= 1 )  is_singleME0Quality1[pt_ind] = true; 
	    if ( qtmp>= 2 )  is_singleME0Quality2[pt_ind] = true; 
	    /* if ( etaParTmp <= 8 ) */  is_singleME0Veto0[pt_ind] = true; 
	    if ( etaParTmp <  8 )  is_singleME0Veto1[pt_ind] = true; 
	    if ( etaParTmp <  7 )  is_singleME0Veto2[pt_ind] = true; 
	    if ( etaParTmp <  6 )  is_singleME0Veto3[pt_ind] = true; 
	    //combinations
	    if ( qtmp>= 0 )
	      {
              			     is_singleME0Quality0Veto0[pt_ind] = true;
              if ( etaParTmp <  8 )  is_singleME0Quality0Veto1[pt_ind] = true;
              if ( etaParTmp <  7 )  is_singleME0Quality0Veto2[pt_ind] = true;
              if ( etaParTmp <  6 )  is_singleME0Quality0Veto3[pt_ind] = true;
	      }
	    if ( qtmp>= 1 )
	      {
              			     is_singleME0Quality1Veto0[pt_ind] = true;
              if ( etaParTmp <  8 )  is_singleME0Quality1Veto1[pt_ind] = true;
              if ( etaParTmp <  7 )  is_singleME0Quality1Veto2[pt_ind] = true;
              if ( etaParTmp <  6 )  is_singleME0Quality1Veto3[pt_ind] = true;
	      }
	    if ( qtmp>= 2 )
	      {
              			     is_singleME0Quality2Veto0[pt_ind] = true;
              if ( etaParTmp <  8 )  is_singleME0Quality2Veto1[pt_ind] = true;
              if ( etaParTmp <  7 )  is_singleME0Quality2Veto2[pt_ind] = true;
              if ( etaParTmp <  6 )  is_singleME0Quality2Veto3[pt_ind] = true;
	      }
	    }
	  }

        }

      cout << "Starting to evaluate double and triple triggers without vicinity request" << endl;

      bool found_double1 = false;
      bool found_double2 = false;
      bool found_triple1 = false;
      bool found_triple2 = false;
      bool found_triple3 = false;


      //Trigger without vicinity request
      int mixed_ind = 0;
      int triple_ind = 0;
      for ( int pt1_ind = 0; pt1_ind < 5 ; pt1_ind++)
        {
	cout << "pt1_ind " << pt1_ind;
        for ( int pt2_ind = pt1_ind; pt2_ind < 5 ; pt2_ind++)
	  {
	  cout << "pt2_ind " << pt2_ind;
          found_double1 = false;
	  found_double2 = false;
          for ( auto dp=(*deltaGlobalPhiLayer21List).begin(); dp!=(*deltaGlobalPhiLayer21List).end(); ++dp)
	    {
            if 	    ( (fabs(*dp) < thr[pt2_ind]) && !found_double2 )	
	      {
	      found_double2 = true;
	      }
            else if (  fabs(*dp) < thr[pt1_ind] )			
	      {
	      found_double1 = true;
	      }
	    //cout << "deltaPhi = " << *dp << "   found_double1 = " << found_double1 << "   found_double2 = " << found_double2 << endl;
	    }
          if ( found_double1 && found_double2 )			is_doubleME0[mixed_ind] = true;
	  //cout << "result: is_doubleME0 = " << is_doubleME0[mixed_ind] << endl;

	  cout << "mixed_ind =" << mixed_ind << endl;
	  mixed_ind++;
          
	  for ( int pt3_ind = pt2_ind; pt3_ind < 5 ; pt3_ind++)
            {
	    cout << "pt3_ind " << pt2_ind;
	    found_triple1 = false;
	    found_triple2 = false;
	    found_triple3 = false;
            for ( auto dp=(*deltaGlobalPhiLayer21List).begin(); dp!=(*deltaGlobalPhiLayer21List).end(); ++dp)
  	      {
              if    	( (fabs(*dp) < thr[pt3_ind]) && !found_triple3  )	
	        { 
		found_triple3 = true;
		}
              else if 	( (fabs(*dp) < thr[pt2_ind]) && !found_triple2  )	
	        { 
		found_triple2 = true;  
		}
              else if	( (fabs(*dp) < thr[pt1_ind]) && !found_triple1  )	
	        { 
	        found_triple1 = true;  
	        }
	      //cout << "deltaPhi = " << *dp << "   found_triple3 = " << found_triple3 << "   found_triple2 = " << found_triple2 << "   found_triple1 = " << found_triple1 << endl;
  	      }
            if ( found_triple1 && found_triple2 && found_triple3 )	is_tripleME0[triple_ind] = true;
  	    //cout << "result: is_tripleME0 = " << is_tripleME0[triple_ind] << endl;
  
	    cout << "triple_ind =" << triple_ind << endl;
	    triple_ind++;
	    }
	  }
	}

      cout << "Starting to evaluate double triggers with vicinity request" << endl;

      cout << "deltaGlobalPhiLayer21List size = " << (*deltaGlobalPhiLayer21List).size() << endl;
      cout << "me0List size = " << (*me0List).size() << endl;

      //Trigger WITH chamber vicinity request (doubles)

      if ( (*deltaGlobalPhiLayer21List).size() > 1 )
      {
      mixed_ind = 0;
      for ( int pt1_ind = 0; pt1_ind < 5 ; pt1_ind++)
        {
	cout << "pt1_ind = " << pt1_ind << endl;
        for ( int pt2_ind = pt1_ind; pt2_ind < 5 ; pt2_ind++)
	  {
	  cout << "pt2_ind = " << pt2_ind << endl;
	    bool good = false;
            for ( unsigned int dp1_ind=0 ; dp1_ind<(*deltaGlobalPhiLayer21List).size()-1 ; dp1_ind++ )
              {
	      cout << "dp1_ind = " << dp1_ind << endl;
	      for ( unsigned int dp2_ind=dp1_ind+1 ; dp2_ind<(*deltaGlobalPhiLayer21List).size() ; dp2_ind++ )
                {
	          cout << "dp2_ind = " << dp2_ind << endl;
	       	  ME0DetId me01( (*me0List)[dp1_ind] );
	       	  ME0DetId me02( (*me0List)[dp2_ind] );
		  int end1 = me01.region(); 
		  int end2 = me02.region(); 
		  int ch1  = me01.chamber();
		  int ch2  = me02.chamber();
		  if ( end1!=end2 ) continue; //two segments must be in the same endcap
  		  if ( fabs(ch1-ch2)>1 ) continue; //three segments must be in the same chamber or in adjacent chambers
		  //order deltaPhi by absolute value
		  float dPhiFabs[2] = { fabs((*deltaGlobalPhiLayer21List)[dp1_ind]) , fabs((*deltaGlobalPhiLayer21List)[dp2_ind]) };
		  float maxDP = *std::max_element(dPhiFabs,dPhiFabs+2);
		  float minDP = *std::min_element(dPhiFabs,dPhiFabs+2);
                  if ( (minDP<thr[pt2_ind]) && (maxDP<thr[pt1_ind]) )	good=true;
	        }
	      } //end loop over 3 deltaPhi combinations
	    if (good) is_doubleME0near[mixed_ind] = true;
  
	  mixed_ind++;
	  cout << "mixed_ind = " << mixed_ind << endl;
	  } //loop over pt2_ind (2nd thr value)
	} //loop over pt1_ind (1st thr value)
      } //if there are at least 2 segments


      cout << "Starting to evaluate triple triggers with vicinity request" << endl;

      //Trigger WITH chamber vicinity request (triples)
      if ( (*deltaGlobalPhiLayer21List).size() > 2 )
      {
      triple_ind = 0;
      for ( int pt1_ind = 0; pt1_ind < 5 ; pt1_ind++)
        {
	cout << "pt1_ind = " << pt1_ind << endl;
        for ( int pt2_ind = pt1_ind; pt2_ind < 5 ; pt2_ind++)
	  {
	  cout << "pt2_ind = " << pt2_ind << endl;
	  for ( int pt3_ind = pt2_ind; pt3_ind < 5 ; pt3_ind++)
            {
	    cout << "pt3_ind = " << pt3_ind << endl;
	    bool good = false;
            for ( unsigned int dp1_ind=0 ; dp1_ind<(*deltaGlobalPhiLayer21List).size()-2 ; dp1_ind++ )
              {
	      cout << "dp1_ind = " << dp1_ind << endl;
	      for ( unsigned int dp2_ind=dp1_ind+1 ; dp2_ind<(*deltaGlobalPhiLayer21List).size()-1 ; dp2_ind++ )
                {
	        cout << "dp2_ind = " << dp2_ind << endl;
	        for ( unsigned int dp3_ind=dp2_ind+1 ; dp3_ind<(*deltaGlobalPhiLayer21List).size() ; dp3_ind++ )
	          {
	          cout << "dp3_ind = " << dp3_ind << endl;
	       	  ME0DetId me01( (*me0List)[dp1_ind] );
	       	  ME0DetId me02( (*me0List)[dp2_ind] );
	       	  ME0DetId me03( (*me0List)[dp3_ind] );
		  int end1 = me01.region(); 
		  int end2 = me02.region(); 
		  int end3 = me03.region(); 
		  int ch1  = me01.chamber();
		  int ch2  = me02.chamber();
		  int ch3  = me03.chamber();
		  if ( !(end1==end2 && end1==end3) ) continue; //three segments must be in the same endcap
		  //three segments must be in the same chamber or in adjacent chambers
		  if ( fabs(ch1-ch2)>1 ) continue;
		  if ( fabs(ch1-ch3)>1 ) continue;
		  if ( fabs(ch2-ch3)>1 ) continue;
		  //order deltaPhi by absolute value
		  float dPhiFabs[3] = { fabs((*deltaGlobalPhiLayer21List)[dp1_ind]) , fabs((*deltaGlobalPhiLayer21List)[dp2_ind]) , fabs((*deltaGlobalPhiLayer21List)[dp3_ind]) };
		  float maxDP = *std::max_element(dPhiFabs,dPhiFabs+3);
		  float minDP = *std::min_element(dPhiFabs,dPhiFabs+3);
		  float medDP = dPhiFabs[0]+dPhiFabs[1]+dPhiFabs[2]-maxDP-minDP;
                  if ( (minDP<thr[pt3_ind]) && (maxDP<thr[pt1_ind]) && (medDP<thr[pt2_ind]) )	good=true;
		  }
	        }
	      } //end loop over 3 deltaPhi combinations
	    if (good) is_tripleME0near[triple_ind] = true;
            cout << "triple_ind = " << triple_ind << endl; 
	    triple_ind++;
	    } // loop over pt3_ind (3rd thr value)
	  } //loop over pt2_ind (2nd thr value)
	} //loop over pt1_ind (1st thr value)
       } // if there are at least 3 segments
       

      //Trigger WITH eta partition vicinity request (doubles)

      if ( (*deltaGlobalPhiLayer21List).size() > 1 )
      {
      mixed_ind = 0;
      for ( int pt1_ind = 0; pt1_ind < 5 ; pt1_ind++)
        {
	cout << "pt1_ind = " << pt1_ind << endl;
        for ( int pt2_ind = pt1_ind; pt2_ind < 5 ; pt2_ind++)
	  {
	  cout << "pt2_ind = " << pt2_ind << endl;
	    bool good = false;
            for ( unsigned int dp1_ind=0 ; dp1_ind<(*deltaGlobalPhiLayer21List).size()-1 ; dp1_ind++ )
              {
	      cout << "dp1_ind = " << dp1_ind << endl;
	      for ( unsigned int dp2_ind=dp1_ind+1 ; dp2_ind<(*deltaGlobalPhiLayer21List).size() ; dp2_ind++ )
                {
	          cout << "dp2_ind = " << dp2_ind << endl;
	       	  ME0DetId me01( (*me0List)[dp1_ind] );
	       	  ME0DetId me02( (*me0List)[dp2_ind] );
		  int end1 = me01.region(); 	//endcap
		  int end2 = me02.region(); 	//endcap
		  int ch1  = me01.chamber();	//chamber
		  int ch2  = me02.chamber();	//chamber
		  int etap1 = (*etaPartList)[dp1_ind]; //etaPartition
		  int etap2 = (*etaPartList)[dp1_ind]; //etaPartition
		  if ( end1!=end2 ) continue; //two segments must be in the same endcap
  		  if ( fabs(ch1-ch2)>1 ) continue; //two segments must be in the same chamber or in adjacent chambers
		  if ( fabs(etap1-etap2)>1 ) continue; //two segments must be in nearby eta partition (limit to be defined)
		  //order deltaPhi by absolute value
		  float dPhiFabs[2] = { fabs((*deltaGlobalPhiLayer21List)[dp1_ind]) , fabs((*deltaGlobalPhiLayer21List)[dp2_ind]) };
		  float maxDP = *std::max_element(dPhiFabs,dPhiFabs+2);
		  float minDP = *std::min_element(dPhiFabs,dPhiFabs+2);
                  if ( (minDP<thr[pt2_ind]) && (maxDP<thr[pt1_ind]) )	good=true;
	        }
	      } //end loop over 3 deltaPhi combinations
	    if (good) is_doubleME0close[mixed_ind] = true;
  
	  mixed_ind++;
	  cout << "mixed_ind = " << mixed_ind << endl;
	  } //loop over pt2_ind (2nd thr value)
	} //loop over pt1_ind (1st thr value)
      } //if there are at least 2 segments


      cout << "Starting to evaluate triple triggers with vicinity request" << endl;

      //Trigger WITH eta partition vicinity request (triples)
      if ( (*deltaGlobalPhiLayer21List).size() > 2 )
      {
      triple_ind = 0;
      for ( int pt1_ind = 0; pt1_ind < 5 ; pt1_ind++)
        {
	cout << "pt1_ind = " << pt1_ind << endl;
        for ( int pt2_ind = pt1_ind; pt2_ind < 5 ; pt2_ind++)
	  {
	  cout << "pt2_ind = " << pt2_ind << endl;
	  for ( int pt3_ind = pt2_ind; pt3_ind < 5 ; pt3_ind++)
            {
	    cout << "pt3_ind = " << pt3_ind << endl;
	    bool good = false;
            for ( unsigned int dp1_ind=0 ; dp1_ind<(*deltaGlobalPhiLayer21List).size()-2 ; dp1_ind++ )
              {
	      cout << "dp1_ind = " << dp1_ind << endl;
	      for ( unsigned int dp2_ind=dp1_ind+1 ; dp2_ind<(*deltaGlobalPhiLayer21List).size()-1 ; dp2_ind++ )
                {
	        cout << "dp2_ind = " << dp2_ind << endl;
	        for ( unsigned int dp3_ind=dp2_ind+1 ; dp3_ind<(*deltaGlobalPhiLayer21List).size() ; dp3_ind++ )
	          {
	          cout << "dp3_ind = " << dp3_ind << endl;
	       	  ME0DetId me01( (*me0List)[dp1_ind] );
	       	  ME0DetId me02( (*me0List)[dp2_ind] );
	       	  ME0DetId me03( (*me0List)[dp3_ind] );
		  int end1 = me01.region(); 
		  int end2 = me02.region(); 
		  int end3 = me03.region(); 
		  int ch1  = me01.chamber();
		  int ch2  = me02.chamber();
		  int ch3  = me03.chamber();
		  int etap1= (*etaPartList)[dp1_ind];
		  int etap2= (*etaPartList)[dp2_ind];
		  int etap3= (*etaPartList)[dp3_ind];
		  if ( fabs(etap1-etap2)>1 ) continue;
		  if ( fabs(etap1-etap3)>1 ) continue;
		  if ( fabs(etap2-etap3)>1 ) continue; //two segments must be in nearby eta partition (limit to be defined)
		  if ( !(end1==end2 && end1==end3) ) continue; //three segments must be in the same endcap
		  //three segments must be in the same chamber or in adjacent chambers
		  if ( fabs(ch1-ch2)>1 ) continue;
		  if ( fabs(ch1-ch3)>1 ) continue;
		  if ( fabs(ch2-ch3)>1 ) continue;
		  //order deltaPhi by absolute value
		  float dPhiFabs[3] = { fabs((*deltaGlobalPhiLayer21List)[dp1_ind]) , fabs((*deltaGlobalPhiLayer21List)[dp2_ind]) , fabs((*deltaGlobalPhiLayer21List)[dp3_ind]) };
		  float maxDP = *std::max_element(dPhiFabs,dPhiFabs+3);
		  float minDP = *std::min_element(dPhiFabs,dPhiFabs+3);
		  float medDP = dPhiFabs[0]+dPhiFabs[1]+dPhiFabs[2]-maxDP-minDP;
                  if ( (minDP<thr[pt3_ind]) && (maxDP<thr[pt1_ind]) && (medDP<thr[pt2_ind]) )	good=true;
		  }
	        }
	      } //end loop over 3 deltaPhi combinations
	    if (good) is_tripleME0close[triple_ind] = true;
            cout << "triple_ind = " << triple_ind << endl; 
	    triple_ind++;
	    } // loop over pt3_ind (3rd thr value)
	  } //loop over pt2_ind (2nd thr value)
	} //loop over pt1_ind (1st thr value)
       } // if there are at least 3 segments




      //Trigger WITH chamber vicinity and Quality request (doubles)

      if ( (*deltaGlobalPhiLayer21List).size() > 1 )
      {
      mixed_ind = 0;
      for ( int pt1_ind = 0; pt1_ind < 5 ; pt1_ind++)
        {
	cout << "pt1_ind = " << pt1_ind << endl;
        for ( int pt2_ind = pt1_ind; pt2_ind < 5 ; pt2_ind++)
	  {
	  cout << "pt2_ind = " << pt2_ind << endl;
	    bool goodQ0 = false;
	    bool goodQ1 = false;
	    bool goodQ2 = false;
	    bool eveto0 = false;
	    bool eveto1 = false;
	    bool eveto2 = false;
	    bool eveto3 = false;
            bool goodQ0eveto0 = false;
            bool goodQ0eveto1 = false;
            bool goodQ0eveto2 = false;
            bool goodQ0eveto3 = false;
            bool goodQ1eveto0 = false;
            bool goodQ1eveto1 = false;
            bool goodQ1eveto2 = false;
            bool goodQ1eveto3 = false;
            bool goodQ2eveto0 = false;
            bool goodQ2eveto1 = false;
            bool goodQ2eveto2 = false;
            bool goodQ2eveto3 = false;
            for ( unsigned int dp1_ind=0 ; dp1_ind<(*deltaGlobalPhiLayer21List).size()-1 ; dp1_ind++ )
              {
	      cout << "dp1_ind = " << dp1_ind << endl;
	      for ( unsigned int dp2_ind=dp1_ind+1 ; dp2_ind<(*deltaGlobalPhiLayer21List).size() ; dp2_ind++ )
                {
	          cout << "dp2_ind = " << dp2_ind << endl;
	       	  ME0DetId me01( (*me0List)[dp1_ind] );
	       	  ME0DetId me02( (*me0List)[dp2_ind] );
		  int end1 = me01.region(); 
		  int end2 = me02.region(); 
		  int ch1  = me01.chamber();
		  int ch2  = me02.chamber();
		  if ( end1!=end2 ) continue; //two segments must be in the same endcap
  		  if ( fabs(ch1-ch2)>1 ) continue; //three segments must be in the same chamber or in adjacent chambers
		  //order deltaPhi by absolute value
		  float dPhiFabs[2] = { fabs((*deltaGlobalPhiLayer21List)[dp1_ind]) , fabs((*deltaGlobalPhiLayer21List)[dp2_ind]) };
		  float maxDP = *std::max_element(dPhiFabs,dPhiFabs+2);
		  float minDP = *std::min_element(dPhiFabs,dPhiFabs+2);
                  if ( ! ((minDP<thr[pt2_ind]) && (maxDP<thr[pt1_ind])) ) continue;
		  if ( ((*qualityList)[dp1_ind]>=0) && ((*qualityList)[dp2_ind]>=0) ) goodQ0 = true;
		  if ( ((*qualityList)[dp1_ind]>=1) && ((*qualityList)[dp2_ind]>=1) ) goodQ1 = true;
		  if ( ((*qualityList)[dp1_ind]>=2) && ((*qualityList)[dp2_ind]>=2) ) goodQ2 = true;
		  eveto0 = true;
		  if ( ((*etaPartList)[dp1_ind]<8)  && ((*etaPartList)[dp2_ind]<8) )  eveto1  = true;
		  if ( ((*etaPartList)[dp1_ind]<7)  && ((*etaPartList)[dp2_ind]<7) )  eveto2  = true;
		  if ( ((*etaPartList)[dp1_ind]<6)  && ((*etaPartList)[dp2_ind]<6) )  eveto3  = true;
		  //combinations
		  if (goodQ0)
		    { 
		    if (eveto0)	goodQ0eveto0 = true;
		    if (eveto1)	goodQ0eveto1 = true;
		    if (eveto2)	goodQ0eveto2 = true;
		    if (eveto3)	goodQ0eveto3 = true;
		    }
		  if (goodQ1)
		    { 
		    if (eveto0)	goodQ1eveto0 = true;
		    if (eveto1)	goodQ1eveto1 = true;
		    if (eveto2)	goodQ1eveto2 = true;
		    if (eveto3)	goodQ1eveto3 = true;
		    }
		  if (goodQ2)
		    { 
		    if (eveto0)	goodQ2eveto0 = true;
		    if (eveto1)	goodQ2eveto1 = true;
		    if (eveto2)	goodQ2eveto2 = true;
		    if (eveto3)	goodQ2eveto3 = true;
		    }
	        }
	      } //end loop over 3 deltaPhi combinations
	    if (goodQ0) is_doubleME0nearQuality0[mixed_ind] = true;
	    if (goodQ1) is_doubleME0nearQuality1[mixed_ind] = true;
	    if (goodQ2) is_doubleME0nearQuality2[mixed_ind] = true;
	    if (eveto0) is_doubleME0nearVeto0[mixed_ind] = true;
	    if (eveto1) is_doubleME0nearVeto1[mixed_ind] = true;
	    if (eveto2) is_doubleME0nearVeto2[mixed_ind] = true;
	    if (eveto3) is_doubleME0nearVeto3[mixed_ind] = true;
            if (goodQ0eveto0) is_doubleME0nearQuality0Veto0[mixed_ind] = true;
            if (goodQ0eveto1) is_doubleME0nearQuality0Veto1[mixed_ind] = true;
            if (goodQ0eveto2) is_doubleME0nearQuality0Veto2[mixed_ind] = true;
            if (goodQ0eveto3) is_doubleME0nearQuality0Veto3[mixed_ind] = true;
            if (goodQ1eveto0) is_doubleME0nearQuality1Veto0[mixed_ind] = true;
            if (goodQ1eveto1) is_doubleME0nearQuality1Veto1[mixed_ind] = true;
            if (goodQ1eveto2) is_doubleME0nearQuality1Veto2[mixed_ind] = true;
            if (goodQ1eveto3) is_doubleME0nearQuality1Veto3[mixed_ind] = true;
            if (goodQ2eveto0) is_doubleME0nearQuality2Veto0[mixed_ind] = true;
            if (goodQ2eveto1) is_doubleME0nearQuality2Veto1[mixed_ind] = true;
            if (goodQ2eveto2) is_doubleME0nearQuality2Veto2[mixed_ind] = true;
            if (goodQ2eveto3) is_doubleME0nearQuality2Veto3[mixed_ind] = true;
  
	  mixed_ind++;
	  cout << "mixed_ind = " << mixed_ind << endl;
	  } //loop over pt2_ind (2nd thr value)
	} //loop over pt1_ind (1st thr value)
      } //if there are at least 2 segments


      cout << "Starting to evaluate triple triggers with vicinity and quality request" << endl;

      //Trigger WITH chamber vicinity and Quality request (triples)
      if ( (*deltaGlobalPhiLayer21List).size() > 2 )
      {
      triple_ind = 0;
      for ( int pt1_ind = 0; pt1_ind < 5 ; pt1_ind++)
        {
	cout << "pt1_ind = " << pt1_ind << endl;
        for ( int pt2_ind = pt1_ind; pt2_ind < 5 ; pt2_ind++)
	  {
	  cout << "pt2_ind = " << pt2_ind << endl;
	  for ( int pt3_ind = pt2_ind; pt3_ind < 5 ; pt3_ind++)
            {
	    cout << "pt3_ind = " << pt3_ind << endl;
	    bool goodQ0 = false;
	    bool goodQ1 = false;
	    bool goodQ2 = false;
	    bool eveto0 = false;
	    bool eveto1 = false;
	    bool eveto2 = false;
	    bool eveto3 = false;
            bool goodQ0eveto0 = false;
            bool goodQ0eveto1 = false;
            bool goodQ0eveto2 = false;
            bool goodQ0eveto3 = false;
            bool goodQ1eveto0 = false;
            bool goodQ1eveto1 = false;
            bool goodQ1eveto2 = false;
            bool goodQ1eveto3 = false;
            bool goodQ2eveto0 = false;
            bool goodQ2eveto1 = false;
            bool goodQ2eveto2 = false;
            bool goodQ2eveto3 = false;
            for ( unsigned int dp1_ind=0 ; dp1_ind<(*deltaGlobalPhiLayer21List).size()-2 ; dp1_ind++ )
              {
	      cout << "dp1_ind = " << dp1_ind << endl;
	      for ( unsigned int dp2_ind=dp1_ind+1 ; dp2_ind<(*deltaGlobalPhiLayer21List).size()-1 ; dp2_ind++ )
                {
	        cout << "dp2_ind = " << dp2_ind << endl;
	        for ( unsigned int dp3_ind=dp2_ind+1 ; dp3_ind<(*deltaGlobalPhiLayer21List).size() ; dp3_ind++ )
	          {
	          cout << "dp3_ind = " << dp3_ind << endl;
	       	  ME0DetId me01( (*me0List)[dp1_ind] );
	       	  ME0DetId me02( (*me0List)[dp2_ind] );
	       	  ME0DetId me03( (*me0List)[dp3_ind] );
		  int end1 = me01.region(); 
		  int end2 = me02.region(); 
		  int end3 = me03.region(); 
		  int ch1  = me01.chamber();
		  int ch2  = me02.chamber();
		  int ch3  = me03.chamber();
		  if ( !(end1==end2 && end1==end3) ) continue; //three segments must be in the same endcap
		  //three segments must be in the same chamber or in adjacent chambers
		  if ( fabs(ch1-ch2)>1 ) continue;
		  if ( fabs(ch1-ch3)>1 ) continue;
		  if ( fabs(ch2-ch3)>1 ) continue;
		  //order deltaPhi by absolute value
		  float dPhiFabs[3] = { fabs((*deltaGlobalPhiLayer21List)[dp1_ind]) , fabs((*deltaGlobalPhiLayer21List)[dp2_ind]) , fabs((*deltaGlobalPhiLayer21List)[dp3_ind]) };
		  float maxDP = *std::max_element(dPhiFabs,dPhiFabs+3);
		  float minDP = *std::min_element(dPhiFabs,dPhiFabs+3);
		  float medDP = dPhiFabs[0]+dPhiFabs[1]+dPhiFabs[2]-maxDP-minDP;
                  if ( !((minDP<thr[pt3_ind]) && (maxDP<thr[pt1_ind]) && (medDP<thr[pt2_ind])) )   continue;
		  if ( ((*qualityList)[dp1_ind]>=0) && ((*qualityList)[dp2_ind]>=0) &&((*qualityList)[dp3_ind]>=0) )  goodQ0 = true;
		  if ( ((*qualityList)[dp1_ind]>=1) && ((*qualityList)[dp2_ind]>=1) &&((*qualityList)[dp3_ind]>=1) )  goodQ1 = true;
		  if ( ((*qualityList)[dp1_ind]>=2) && ((*qualityList)[dp2_ind]>=2) &&((*qualityList)[dp3_ind]>=2) )  goodQ2 = true;
		  eveto0 = true;
		  if ( ((*etaPartList)[dp1_ind]<8)  && ((*etaPartList)[dp2_ind]<8)  &&((*etaPartList)[dp3_ind]<8) )  eveto1 = true;
		  if ( ((*etaPartList)[dp1_ind]<7)  && ((*etaPartList)[dp2_ind]<7)  &&((*etaPartList)[dp3_ind]<7) )  eveto2 = true;
		  if ( ((*etaPartList)[dp1_ind]<6)  && ((*etaPartList)[dp2_ind]<6)  &&((*etaPartList)[dp3_ind]<6) )  eveto3 = true;
		  //combinations
		  if (goodQ0)
		    { 
		    if (eveto0)	goodQ0eveto0 = true;
		    if (eveto1)	goodQ0eveto1 = true;
		    if (eveto2)	goodQ0eveto2 = true;
		    if (eveto3)	goodQ0eveto3 = true;
		    }
		  if (goodQ1)
		    { 
		    if (eveto0)	goodQ1eveto0 = true;
		    if (eveto1)	goodQ1eveto1 = true;
		    if (eveto2)	goodQ1eveto2 = true;
		    if (eveto3)	goodQ1eveto3 = true;
		    }
		  if (goodQ2)
		    { 
		    if (eveto0)	goodQ2eveto0 = true;
		    if (eveto1)	goodQ2eveto1 = true;
		    if (eveto2)	goodQ2eveto2 = true;
		    if (eveto3)	goodQ2eveto3 = true;
		    }

		  }
	        }
	      } //end loop over 3 deltaPhi combinations
	    if (goodQ0) is_tripleME0nearQuality0[triple_ind] = true;
	    if (goodQ1) is_tripleME0nearQuality1[triple_ind] = true;
	    if (goodQ2) is_tripleME0nearQuality2[triple_ind] = true;
	    if (eveto0) is_tripleME0nearVeto0[triple_ind] = true;
	    if (eveto1) is_tripleME0nearVeto1[triple_ind] = true;
	    if (eveto2) is_tripleME0nearVeto2[triple_ind] = true;
            if (goodQ0eveto0) is_tripleME0nearQuality0Veto0[triple_ind] = true;
            if (goodQ0eveto1) is_tripleME0nearQuality0Veto1[triple_ind] = true;
            if (goodQ0eveto2) is_tripleME0nearQuality0Veto2[triple_ind] = true;
            if (goodQ0eveto3) is_tripleME0nearQuality0Veto3[triple_ind] = true;
            if (goodQ1eveto0) is_tripleME0nearQuality1Veto0[triple_ind] = true;
            if (goodQ1eveto1) is_tripleME0nearQuality1Veto1[triple_ind] = true;
            if (goodQ1eveto2) is_tripleME0nearQuality1Veto2[triple_ind] = true;
            if (goodQ1eveto3) is_tripleME0nearQuality1Veto3[triple_ind] = true;
            if (goodQ2eveto0) is_tripleME0nearQuality2Veto0[triple_ind] = true;
            if (goodQ2eveto1) is_tripleME0nearQuality2Veto1[triple_ind] = true;
            if (goodQ2eveto2) is_tripleME0nearQuality2Veto2[triple_ind] = true;
            if (goodQ2eveto3) is_tripleME0nearQuality2Veto3[triple_ind] = true;
            cout << "triple_ind = " << triple_ind << endl; 
	    triple_ind++;
	    } // loop over pt3_ind (3rd thr value)
	  } //loop over pt2_ind (2nd thr value)
	} //loop over pt1_ind (1st thr value)
       } // if there are at least 3 segments



     
     cout << "Incrementing counters..." << endl;


     for (int kk=0; kk<5; kk++)
       if ( is_singleME0[kk] )	singleME0count[kk]++;
     for (int kk=0; kk<15; kk++)
       if ( is_doubleME0[kk] )	doubleME0count[kk]++;
     for (int kk=0; kk<35; kk++)
       if ( is_tripleME0[kk] )	tripleME0count[kk]++;

     for (int kk=0; kk<15; kk++)
       if ( is_doubleME0near[kk] )	doubleME0nearcount[kk]++;
     for (int kk=0; kk<35; kk++)
       if ( is_tripleME0near[kk] )	tripleME0nearcount[kk]++;

     for (int kk=0; kk<15; kk++)
       if ( is_doubleME0close[kk] )	doubleME0closecount[kk]++;
     for (int kk=0; kk<35; kk++)
       if ( is_tripleME0close[kk] )	tripleME0closecount[kk]++;

     for (int kk=0; kk<5; kk++)
       {
       if ( is_singleME0Quality0[kk] )	singleME0Quality0count[kk]++;
       if ( is_singleME0Quality1[kk] )	singleME0Quality1count[kk]++;
       if ( is_singleME0Quality2[kk] )	singleME0Quality2count[kk]++;
       if ( is_singleME0Veto0[kk] )	singleME0Veto0count[kk]++;
       if ( is_singleME0Veto1[kk] )	singleME0Veto1count[kk]++;
       if ( is_singleME0Veto2[kk] )	singleME0Veto2count[kk]++;
       if ( is_singleME0Veto3[kk] )	singleME0Veto3count[kk]++;
       if ( is_singleME0Quality0Veto0[kk] )	singleME0Quality0Veto0count[kk]++;
       if ( is_singleME0Quality0Veto1[kk] )	singleME0Quality0Veto1count[kk]++;
       if ( is_singleME0Quality0Veto2[kk] )	singleME0Quality0Veto2count[kk]++;
       if ( is_singleME0Quality0Veto3[kk] )	singleME0Quality0Veto3count[kk]++;
       if ( is_singleME0Quality1Veto0[kk] )	singleME0Quality1Veto0count[kk]++;
       if ( is_singleME0Quality1Veto1[kk] )	singleME0Quality1Veto1count[kk]++;
       if ( is_singleME0Quality1Veto2[kk] )	singleME0Quality1Veto2count[kk]++;
       if ( is_singleME0Quality1Veto3[kk] )	singleME0Quality1Veto3count[kk]++;
       if ( is_singleME0Quality2Veto0[kk] )	singleME0Quality2Veto0count[kk]++;
       if ( is_singleME0Quality2Veto1[kk] )	singleME0Quality2Veto1count[kk]++;
       if ( is_singleME0Quality2Veto2[kk] )	singleME0Quality2Veto2count[kk]++;
       if ( is_singleME0Quality2Veto3[kk] )	singleME0Quality2Veto3count[kk]++;
       }
     for (int kk=0; kk<15; kk++)
       {
       if ( is_doubleME0nearQuality0[kk] )	doubleME0nearQuality0count[kk]++;
       if ( is_doubleME0nearQuality1[kk] )	doubleME0nearQuality1count[kk]++;
       if ( is_doubleME0nearQuality2[kk] )	doubleME0nearQuality2count[kk]++;
       if ( is_doubleME0nearVeto0[kk] )	doubleME0nearVeto0count[kk]++;
       if ( is_doubleME0nearVeto1[kk] )	doubleME0nearVeto1count[kk]++;
       if ( is_doubleME0nearVeto2[kk] )	doubleME0nearVeto2count[kk]++;
       if ( is_doubleME0nearVeto3[kk] )	doubleME0nearVeto3count[kk]++;
       if ( is_doubleME0nearQuality0Veto0[kk] )	doubleME0nearQuality0Veto0count[kk]++;
       if ( is_doubleME0nearQuality0Veto1[kk] )	doubleME0nearQuality0Veto1count[kk]++;
       if ( is_doubleME0nearQuality0Veto2[kk] )	doubleME0nearQuality0Veto2count[kk]++;
       if ( is_doubleME0nearQuality0Veto3[kk] )	doubleME0nearQuality0Veto3count[kk]++;
       if ( is_doubleME0nearQuality1Veto0[kk] )	doubleME0nearQuality1Veto0count[kk]++;
       if ( is_doubleME0nearQuality1Veto1[kk] )	doubleME0nearQuality1Veto1count[kk]++;
       if ( is_doubleME0nearQuality1Veto2[kk] )	doubleME0nearQuality1Veto2count[kk]++;
       if ( is_doubleME0nearQuality1Veto3[kk] )	doubleME0nearQuality1Veto3count[kk]++;
       if ( is_doubleME0nearQuality2Veto0[kk] )	doubleME0nearQuality2Veto0count[kk]++;
       if ( is_doubleME0nearQuality2Veto1[kk] )	doubleME0nearQuality2Veto1count[kk]++;
       if ( is_doubleME0nearQuality2Veto2[kk] )	doubleME0nearQuality2Veto2count[kk]++;
       if ( is_doubleME0nearQuality2Veto3[kk] )	doubleME0nearQuality2Veto3count[kk]++;
       }

     for (int kk=0; kk<35; kk++)
       {
       if ( is_tripleME0nearQuality0[kk] )	tripleME0nearQuality0count[kk]++;
       if ( is_tripleME0nearQuality1[kk] )	tripleME0nearQuality1count[kk]++;
       if ( is_tripleME0nearQuality2[kk] )	tripleME0nearQuality2count[kk]++;
       if ( is_tripleME0nearVeto0[kk] )	tripleME0nearVeto0count[kk]++;
       if ( is_tripleME0nearVeto1[kk] )	tripleME0nearVeto1count[kk]++;
       if ( is_tripleME0nearVeto2[kk] )	tripleME0nearVeto2count[kk]++;
       if ( is_tripleME0nearVeto3[kk] )	tripleME0nearVeto3count[kk]++;
       if ( is_tripleME0nearQuality0Veto0[kk] )	tripleME0nearQuality0Veto0count[kk]++;
       if ( is_tripleME0nearQuality0Veto1[kk] )	tripleME0nearQuality0Veto1count[kk]++;
       if ( is_tripleME0nearQuality0Veto2[kk] )	tripleME0nearQuality0Veto2count[kk]++;
       if ( is_tripleME0nearQuality0Veto3[kk] )	tripleME0nearQuality0Veto3count[kk]++;
       if ( is_tripleME0nearQuality1Veto0[kk] )	tripleME0nearQuality1Veto0count[kk]++;
       if ( is_tripleME0nearQuality1Veto1[kk] )	tripleME0nearQuality1Veto1count[kk]++;
       if ( is_tripleME0nearQuality1Veto2[kk] )	tripleME0nearQuality1Veto2count[kk]++;
       if ( is_tripleME0nearQuality1Veto3[kk] )	tripleME0nearQuality1Veto3count[kk]++;
       if ( is_tripleME0nearQuality2Veto0[kk] )	tripleME0nearQuality2Veto0count[kk]++;
       if ( is_tripleME0nearQuality2Veto1[kk] )	tripleME0nearQuality2Veto1count[kk]++;
       if ( is_tripleME0nearQuality2Veto2[kk] )	tripleME0nearQuality2Veto2count[kk]++;
       if ( is_tripleME0nearQuality2Veto3[kk] )	tripleME0nearQuality2Veto3count[kk]++;
       }

     cout << "...Done." << endl;

}

//============= INSERTED TO HERE ========================
 

   //auto_ptr<std::map<ME0DetId,vector<float>>> deltaPhi_ptr ( &deltaPhiMap);
   //auto_ptr<std::map<ME0DetId,vector<float>>> alpha_ptr    ( &alphaMap   );
   //auto_ptr<std::map<ME0DetId,vector<float>>> theta_ptr    ( &thetaMap   );

   //iEvent.put( deltaPhi_ptr, "ME0SegmentDeltaPhi" );
   //iEvent.put( alpha_ptr, "ME0SegmentAlpha" );
   //iEvent.put( theta_ptr, "ME0SegmentTheta" );

  // iEvent.put(std::make_unique<std::map<ME0DetId,vector<float>>>(deltaPhiMap));
  // iEvent.put(std::make_unique<std::map<ME0DetId,vector<float>>>(alphaMap));
  // iEvent.put(std::make_unique<std::map<ME0DetId,vector<float>>>(thetaMap));
  
//=================GEN PARATICLES===========================

 //Handle<GenParticleCollection> genParticles;
 iEvent.getByToken(genToken_, genParticles);

 for(size_t i = 0; i < genParticles->size(); ++ i) 
  {
  //if (v) cout << "GenParticle index: " << i << endl;
  const GenParticle & p = (*genParticles)[i];
  //const Candidate * mom =( p.mother());

  if (fabs(p.pdgId()) != 13)	continue; 
  //(*muPdgId).push_back(p.pdgId());
  //(*muPt).push_back(p.pt());
  //(*muEta).push_back(p.eta());
  //(*muPx).push_back(p.px());
  //(*muPy).push_back(p.py());
  //(*muPz).push_back(p.pz());
  }

 tr->Fill();
 cout << "etaPartList size = " << (*etaPartList).size() << endl;
 cout << "etaPrtList items:" << endl;
 for (unsigned int ind=0; ind<(*etaPartList).size() ; ind++)	cout << (*etaPartList)[ind] << endl;

}


// ------------ method called once each job just before starting event loop  ------------
void 
DeltaGlobalPhiAnalyzer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
DeltaGlobalPhiAnalyzer::endJob() 
{
 cout << "EndJob started:" << endl;
 lastEvent=nEvent;
 lastEventSel=nEventSel;
 Int_t denom=-1;
 if (nME0>0 && signal)  denom=lastEventSel;
 if ( signal ) denom=lastEvent;//run trigger analysis only if there is at least one mu from the tau decay in the eta range 1.8<|eta!|<3 

 cout << "Calculating rates and efficiencies (triggers without vicinity)" << endl;
 for (int kk=0; kk<5; kk++)	singleME0rate[kk] = singleME0count[kk]/(denom*25.0*1e-9); //Hz
 for (int kk=0; kk<15; kk++)   	doubleME0rate[kk] = doubleME0count[kk]/(denom*25.0*1e-9);  //Hz
 for (int kk=0; kk<35; kk++)   	tripleME0rate[kk] = tripleME0count[kk]/(denom*25.0*1e-9);  //Hz

 for (int kk=0; kk<5; kk++)	singleME0efficiency[kk] = singleME0count[kk]*1.0/(denom); //Hz
 for (int kk=0; kk<15; kk++)   	doubleME0efficiency[kk] = doubleME0count[kk]*1.0/(denom);  //Hz
 for (int kk=0; kk<35; kk++)   	tripleME0efficiency[kk] = tripleME0count[kk]*1.0/(denom);  //Hz

 for (int kk=0; kk<5; kk++)	singleME0efficiencyErr[kk] = TMath::Sqrt(singleME0efficiency[kk]*(1.0-singleME0efficiency[kk])/(denom*1.0)); //Hz
 for (int kk=0; kk<15; kk++)   	doubleME0efficiencyErr[kk] = TMath::Sqrt(doubleME0efficiency[kk]*(1.0-doubleME0efficiency[kk])/(denom*1.0));  //Hz
 for (int kk=0; kk<35; kk++)   	tripleME0efficiencyErr[kk] = TMath::Sqrt(tripleME0efficiency[kk]*(1.0-tripleME0efficiency[kk])/(denom*1.0));  //Hz

 for (int kk=0; kk<5; kk++)	singleME0rateError[kk] = singleME0efficiencyErr[kk]/(25.0*1e-9); //Hz
 for (int kk=0; kk<15; kk++)   	doubleME0rateError[kk] = doubleME0efficiencyErr[kk]/(25.0*1e-9);  //Hz
 for (int kk=0; kk<35; kk++)   	tripleME0rateError[kk] = tripleME0efficiencyErr[kk]/(25.0*1e-9);  //Hz

 cout << "Calculating rates and efficiencies (triggers with chamber vicinity)" << endl;
 for (int kk=0; kk<15; kk++)   	doubleME0nearrate[kk] = doubleME0nearcount[kk]/(denom*25.0*1e-9);  //Hz
 for (int kk=0; kk<35; kk++)   	tripleME0nearrate[kk] = tripleME0nearcount[kk]/(denom*25.0*1e-9);  //Hz

 for (int kk=0; kk<15; kk++)   	doubleME0nearefficiency[kk] = doubleME0nearcount[kk]*1.0/(denom);  //Hz
 for (int kk=0; kk<35; kk++)   	tripleME0nearefficiency[kk] = tripleME0nearcount[kk]*1.0/(denom);  //Hz

 for (int kk=0; kk<15; kk++)   	doubleME0nearefficiencyErr[kk] = TMath::Sqrt(doubleME0nearefficiency[kk]*(1.0-doubleME0nearefficiency[kk])/(denom*1.0));  //Hz
 for (int kk=0; kk<35; kk++)   	tripleME0nearefficiencyErr[kk] = TMath::Sqrt(tripleME0nearefficiency[kk]*(1.0-tripleME0nearefficiency[kk])/(denom*1.0));  //Hz

 for (int kk=0; kk<15; kk++)   	doubleME0nearrateError[kk] = doubleME0nearefficiencyErr[kk]/(25.0*1e-9);  //Hz
 for (int kk=0; kk<35; kk++)   	tripleME0nearrateError[kk] = tripleME0nearefficiencyErr[kk]/(25.0*1e-9);  //Hz

 cout << "Calculating rates and efficiencies (triggers with chamber and eta vicinity)" << endl;
 for (int kk=0; kk<15; kk++)   	doubleME0closerate[kk] = doubleME0closecount[kk]/(denom*25.0*1e-9);  //Hz
 for (int kk=0; kk<35; kk++)   	tripleME0closerate[kk] = tripleME0closecount[kk]/(denom*25.0*1e-9);  //Hz

 for (int kk=0; kk<15; kk++)   	doubleME0closeefficiency[kk] = doubleME0closecount[kk]*1.0/(denom);  //Hz
 for (int kk=0; kk<35; kk++)   	tripleME0closeefficiency[kk] = tripleME0closecount[kk]*1.0/(denom);  //Hz

 for (int kk=0; kk<15; kk++)   	doubleME0closeefficiencyErr[kk] = TMath::Sqrt(doubleME0closeefficiency[kk]*(1.0-doubleME0closeefficiency[kk])/(1.0*denom));  //Hz
 for (int kk=0; kk<35; kk++)   	tripleME0closeefficiencyErr[kk] = TMath::Sqrt(tripleME0closeefficiency[kk]*(1.0-tripleME0closeefficiency[kk])/(1.0*denom));  //Hz

 for (int kk=0; kk<15; kk++)   	doubleME0closerateError[kk] = doubleME0closeefficiencyErr[kk]/(25.0*1e-9);  //Hz
 for (int kk=0; kk<35; kk++)   	tripleME0closerateError[kk] = tripleME0closeefficiencyErr[kk]/(25.0*1e-9);  //Hz

 cout << "Calculating rates and efficiencies (triggers with chamber vicinity and segment quality request)" << endl;
 for (int kk=0; kk<5; kk++)	
   {
   singleME0Quality0rate[kk] = singleME0Quality0count[kk]/(denom*25.0*1e-9); //Hz
   singleME0Quality1rate[kk] = singleME0Quality1count[kk]/(denom*25.0*1e-9); //Hz
   singleME0Quality2rate[kk] = singleME0Quality2count[kk]/(denom*25.0*1e-9); //Hz
   singleME0Veto0rate[kk] = singleME0Veto0count[kk]/(denom*25.0*1e-9); //Hz
   singleME0Veto1rate[kk] = singleME0Veto1count[kk]/(denom*25.0*1e-9); //Hz
   singleME0Veto2rate[kk] = singleME0Veto2count[kk]/(denom*25.0*1e-9); //Hz
   singleME0Veto3rate[kk] = singleME0Veto3count[kk]/(denom*25.0*1e-9); //Hz
   singleME0Quality0Veto0rate[kk] = singleME0Quality0Veto0count[kk]/(denom*25.0*1e-9); //Hz
   singleME0Quality0Veto1rate[kk] = singleME0Quality0Veto1count[kk]/(denom*25.0*1e-9); //Hz
   singleME0Quality0Veto2rate[kk] = singleME0Quality0Veto2count[kk]/(denom*25.0*1e-9); //Hz
   singleME0Quality0Veto3rate[kk] = singleME0Quality0Veto3count[kk]/(denom*25.0*1e-9); //Hz
   singleME0Quality1Veto0rate[kk] = singleME0Quality1Veto0count[kk]/(denom*25.0*1e-9); //Hz
   singleME0Quality1Veto1rate[kk] = singleME0Quality1Veto1count[kk]/(denom*25.0*1e-9); //Hz
   singleME0Quality1Veto2rate[kk] = singleME0Quality1Veto2count[kk]/(denom*25.0*1e-9); //Hz
   singleME0Quality1Veto3rate[kk] = singleME0Quality1Veto3count[kk]/(denom*25.0*1e-9); //Hz
   singleME0Quality2Veto0rate[kk] = singleME0Quality2Veto0count[kk]/(denom*25.0*1e-9); //Hz
   singleME0Quality2Veto1rate[kk] = singleME0Quality2Veto1count[kk]/(denom*25.0*1e-9); //Hz
   singleME0Quality2Veto2rate[kk] = singleME0Quality2Veto2count[kk]/(denom*25.0*1e-9); //Hz
   singleME0Quality2Veto3rate[kk] = singleME0Quality2Veto3count[kk]/(denom*25.0*1e-9); //Hz
   }
 for (int kk=0; kk<15; kk++)   	
   {
   doubleME0nearQuality0rate[kk] = doubleME0nearQuality0count[kk]/(denom*25.0*1e-9);  //Hz
   doubleME0nearQuality1rate[kk] = doubleME0nearQuality1count[kk]/(denom*25.0*1e-9);  //Hz
   doubleME0nearQuality2rate[kk] = doubleME0nearQuality2count[kk]/(denom*25.0*1e-9);  //Hz
   doubleME0nearVeto0rate[kk] = doubleME0nearVeto0count[kk]/(denom*25.0*1e-9);  //Hz
   doubleME0nearVeto1rate[kk] = doubleME0nearVeto1count[kk]/(denom*25.0*1e-9);  //Hz
   doubleME0nearVeto2rate[kk] = doubleME0nearVeto2count[kk]/(denom*25.0*1e-9);  //Hz
   doubleME0nearVeto3rate[kk] = doubleME0nearVeto3count[kk]/(denom*25.0*1e-9);  //Hz
   doubleME0nearQuality0Veto0rate[kk] = doubleME0nearQuality0Veto0count[kk]/(denom*25.0*1e-9);  //Hz
   doubleME0nearQuality0Veto1rate[kk] = doubleME0nearQuality0Veto1count[kk]/(denom*25.0*1e-9);  //Hz
   doubleME0nearQuality0Veto2rate[kk] = doubleME0nearQuality0Veto2count[kk]/(denom*25.0*1e-9);  //Hz
   doubleME0nearQuality0Veto3rate[kk] = doubleME0nearQuality0Veto3count[kk]/(denom*25.0*1e-9);  //Hz
   doubleME0nearQuality1Veto0rate[kk] = doubleME0nearQuality1Veto0count[kk]/(denom*25.0*1e-9);  //Hz
   doubleME0nearQuality1Veto1rate[kk] = doubleME0nearQuality1Veto1count[kk]/(denom*25.0*1e-9);  //Hz
   doubleME0nearQuality1Veto2rate[kk] = doubleME0nearQuality1Veto2count[kk]/(denom*25.0*1e-9);  //Hz
   doubleME0nearQuality1Veto3rate[kk] = doubleME0nearQuality1Veto3count[kk]/(denom*25.0*1e-9);  //Hz
   doubleME0nearQuality2Veto0rate[kk] = doubleME0nearQuality2Veto0count[kk]/(denom*25.0*1e-9);  //Hz
   doubleME0nearQuality2Veto1rate[kk] = doubleME0nearQuality2Veto1count[kk]/(denom*25.0*1e-9);  //Hz
   doubleME0nearQuality2Veto2rate[kk] = doubleME0nearQuality2Veto2count[kk]/(denom*25.0*1e-9);  //Hz
   doubleME0nearQuality2Veto3rate[kk] = doubleME0nearQuality2Veto3count[kk]/(denom*25.0*1e-9);  //Hz
   }
 for (int kk=0; kk<35; kk++)   	
   {
   tripleME0nearQuality0rate[kk] = tripleME0nearQuality0count[kk]/(denom*25.0*1e-9);  //Hz
   tripleME0nearQuality1rate[kk] = tripleME0nearQuality1count[kk]/(denom*25.0*1e-9);  //Hz
   tripleME0nearQuality2rate[kk] = tripleME0nearQuality2count[kk]/(denom*25.0*1e-9);  //Hz
   tripleME0nearVeto0rate[kk] = tripleME0nearVeto0count[kk]/(denom*25.0*1e-9);  //Hz
   tripleME0nearVeto1rate[kk] = tripleME0nearVeto1count[kk]/(denom*25.0*1e-9);  //Hz
   tripleME0nearVeto2rate[kk] = tripleME0nearVeto2count[kk]/(denom*25.0*1e-9);  //Hz
   tripleME0nearVeto3rate[kk] = tripleME0nearVeto3count[kk]/(denom*25.0*1e-9);  //Hz
   tripleME0nearQuality0Veto0rate[kk] = tripleME0nearQuality0Veto0count[kk]/(denom*25.0*1e-9);  //Hz
   tripleME0nearQuality0Veto1rate[kk] = tripleME0nearQuality0Veto1count[kk]/(denom*25.0*1e-9);  //Hz
   tripleME0nearQuality0Veto2rate[kk] = tripleME0nearQuality0Veto2count[kk]/(denom*25.0*1e-9);  //Hz
   tripleME0nearQuality0Veto3rate[kk] = tripleME0nearQuality0Veto3count[kk]/(denom*25.0*1e-9);  //Hz
   tripleME0nearQuality1Veto0rate[kk] = tripleME0nearQuality1Veto0count[kk]/(denom*25.0*1e-9);  //Hz
   tripleME0nearQuality1Veto1rate[kk] = tripleME0nearQuality1Veto1count[kk]/(denom*25.0*1e-9);  //Hz
   tripleME0nearQuality1Veto2rate[kk] = tripleME0nearQuality1Veto2count[kk]/(denom*25.0*1e-9);  //Hz
   tripleME0nearQuality1Veto3rate[kk] = tripleME0nearQuality1Veto3count[kk]/(denom*25.0*1e-9);  //Hz
   tripleME0nearQuality2Veto0rate[kk] = tripleME0nearQuality2Veto0count[kk]/(denom*25.0*1e-9);  //Hz
   tripleME0nearQuality2Veto1rate[kk] = tripleME0nearQuality2Veto1count[kk]/(denom*25.0*1e-9);  //Hz
   tripleME0nearQuality2Veto2rate[kk] = tripleME0nearQuality2Veto2count[kk]/(denom*25.0*1e-9);  //Hz
   tripleME0nearQuality2Veto3rate[kk] = tripleME0nearQuality2Veto3count[kk]/(denom*25.0*1e-9);  //Hz
   }

 for (int kk=0; kk<5; kk++)	
   {
   singleME0Quality0efficiency[kk] = singleME0Quality0count[kk]*1.0/(denom); //Hz
   singleME0Quality1efficiency[kk] = singleME0Quality1count[kk]*1.0/(denom); //Hz
   singleME0Quality2efficiency[kk] = singleME0Quality2count[kk]*1.0/(denom); //Hz

   singleME0Quality0efficiencyErr[kk] = TMath::Sqrt(singleME0Quality0efficiency[kk]*(1.0-singleME0Quality0efficiency[kk])/(1.0*denom)); //Hz
   singleME0Quality1efficiencyErr[kk] = TMath::Sqrt(singleME0Quality1efficiency[kk]*(1.0-singleME0Quality1efficiency[kk])/(1.0*denom)); //Hz
   singleME0Quality2efficiencyErr[kk] = TMath::Sqrt(singleME0Quality2efficiency[kk]*(1.0-singleME0Quality2efficiency[kk])/(1.0*denom)); //Hz

   singleME0Veto0efficiency[kk] = singleME0Veto0count[kk]*1.0/(denom); //Hz
   singleME0Veto1efficiency[kk] = singleME0Veto1count[kk]*1.0/(denom); //Hz
   singleME0Veto2efficiency[kk] = singleME0Veto2count[kk]*1.0/(denom); //Hz
   singleME0Veto3efficiency[kk] = singleME0Veto3count[kk]*1.0/(denom); //Hz

   singleME0Veto0efficiencyErr[kk] = TMath::Sqrt(singleME0Veto0efficiency[kk]*(1.0-singleME0Veto0efficiency[kk])/(1.0*denom)); //Hz
   singleME0Veto1efficiencyErr[kk] = TMath::Sqrt(singleME0Veto1efficiency[kk]*(1.0-singleME0Veto1efficiency[kk])/(1.0*denom)); //Hz
   singleME0Veto2efficiencyErr[kk] = TMath::Sqrt(singleME0Veto2efficiency[kk]*(1.0-singleME0Veto2efficiency[kk])/(1.0*denom)); //Hz
   singleME0Veto3efficiencyErr[kk] = TMath::Sqrt(singleME0Veto3efficiency[kk]*(1.0-singleME0Veto3efficiency[kk])/(1.0*denom)); //Hz

   singleME0Quality0Veto0efficiency[kk] = singleME0Quality0Veto0count[kk]*1.0/(denom); //Hz
   singleME0Quality0Veto1efficiency[kk] = singleME0Quality0Veto1count[kk]*1.0/(denom); //Hz
   singleME0Quality0Veto2efficiency[kk] = singleME0Quality0Veto2count[kk]*1.0/(denom); //Hz
   singleME0Quality0Veto3efficiency[kk] = singleME0Quality0Veto3count[kk]*1.0/(denom); //Hz
   singleME0Quality1Veto0efficiency[kk] = singleME0Quality1Veto0count[kk]*1.0/(denom); //Hz
   singleME0Quality1Veto1efficiency[kk] = singleME0Quality1Veto1count[kk]*1.0/(denom); //Hz
   singleME0Quality1Veto2efficiency[kk] = singleME0Quality1Veto2count[kk]*1.0/(denom); //Hz
   singleME0Quality1Veto3efficiency[kk] = singleME0Quality1Veto3count[kk]*1.0/(denom); //Hz
   singleME0Quality2Veto0efficiency[kk] = singleME0Quality2Veto0count[kk]*1.0/(denom); //Hz
   singleME0Quality2Veto1efficiency[kk] = singleME0Quality2Veto1count[kk]*1.0/(denom); //Hz
   singleME0Quality2Veto2efficiency[kk] = singleME0Quality2Veto2count[kk]*1.0/(denom); //Hz
   singleME0Quality2Veto3efficiency[kk] = singleME0Quality2Veto3count[kk]*1.0/(denom); //Hz

   singleME0Quality0Veto0efficiencyErr[kk] = TMath::Sqrt(singleME0Quality0Veto0efficiency[kk]*(1.0-singleME0Quality0Veto0efficiency[kk])/(1.0*denom)); //Hz
   singleME0Quality0Veto1efficiencyErr[kk] = TMath::Sqrt(singleME0Quality0Veto1efficiency[kk]*(1.0-singleME0Quality0Veto1efficiency[kk])/(1.0*denom)); //Hz
   singleME0Quality0Veto2efficiencyErr[kk] = TMath::Sqrt(singleME0Quality0Veto2efficiency[kk]*(1.0-singleME0Quality0Veto2efficiency[kk])/(1.0*denom)); //Hz
   singleME0Quality0Veto3efficiencyErr[kk] = TMath::Sqrt(singleME0Quality0Veto3efficiency[kk]*(1.0-singleME0Quality0Veto3efficiency[kk])/(1.0*denom)); //Hz
   singleME0Quality1Veto0efficiencyErr[kk] = TMath::Sqrt(singleME0Quality1Veto0efficiency[kk]*(1.0-singleME0Quality1Veto0efficiency[kk])/(1.0*denom)); //Hz
   singleME0Quality1Veto1efficiencyErr[kk] = TMath::Sqrt(singleME0Quality1Veto1efficiency[kk]*(1.0-singleME0Quality1Veto1efficiency[kk])/(1.0*denom)); //Hz
   singleME0Quality1Veto2efficiencyErr[kk] = TMath::Sqrt(singleME0Quality1Veto2efficiency[kk]*(1.0-singleME0Quality1Veto2efficiency[kk])/(1.0*denom)); //Hz
   singleME0Quality1Veto3efficiencyErr[kk] = TMath::Sqrt(singleME0Quality1Veto3efficiency[kk]*(1.0-singleME0Quality1Veto3efficiency[kk])/(1.0*denom)); //Hz
   singleME0Quality2Veto0efficiencyErr[kk] = TMath::Sqrt(singleME0Quality2Veto0efficiency[kk]*(1.0-singleME0Quality2Veto0efficiency[kk])/(1.0*denom)); //Hz
   singleME0Quality2Veto1efficiencyErr[kk] = TMath::Sqrt(singleME0Quality2Veto1efficiency[kk]*(1.0-singleME0Quality2Veto1efficiency[kk])/(1.0*denom)); //Hz
   singleME0Quality2Veto2efficiencyErr[kk] = TMath::Sqrt(singleME0Quality2Veto2efficiency[kk]*(1.0-singleME0Quality2Veto2efficiency[kk])/(1.0*denom)); //Hz
   singleME0Quality2Veto3efficiencyErr[kk] = TMath::Sqrt(singleME0Quality2Veto3efficiency[kk]*(1.0-singleME0Quality2Veto3efficiency[kk])/(1.0*denom)); //Hz
   }
 for (int kk=0; kk<15; kk++)   	
   {
   doubleME0nearQuality0efficiency[kk] = doubleME0nearQuality0count[kk]*1.0/(denom);  //Hz
   doubleME0nearQuality1efficiency[kk] = doubleME0nearQuality1count[kk]*1.0/(denom);  //Hz
   doubleME0nearQuality2efficiency[kk] = doubleME0nearQuality2count[kk]*1.0/(denom);  //Hz

   doubleME0nearQuality0efficiencyErr[kk] = TMath::Sqrt(doubleME0nearQuality0efficiency[kk]*(1.0-doubleME0nearQuality0efficiency[kk])/(1.0*denom));  //Hz
   doubleME0nearQuality1efficiencyErr[kk] = TMath::Sqrt(doubleME0nearQuality1efficiency[kk]*(1.0-doubleME0nearQuality1efficiency[kk])/(1.0*denom));  //Hz
   doubleME0nearQuality2efficiencyErr[kk] = TMath::Sqrt(doubleME0nearQuality2efficiency[kk]*(1.0-doubleME0nearQuality2efficiency[kk])/(1.0*denom));  //Hz

   doubleME0nearVeto0efficiency[kk] = doubleME0nearVeto0count[kk]*1.0/(denom);  //Hz
   doubleME0nearVeto1efficiency[kk] = doubleME0nearVeto1count[kk]*1.0/(denom);  //Hz
   doubleME0nearVeto2efficiency[kk] = doubleME0nearVeto2count[kk]*1.0/(denom);  //Hz
   doubleME0nearVeto3efficiency[kk] = doubleME0nearVeto3count[kk]*1.0/(denom);  //Hz

   doubleME0nearVeto0efficiencyErr[kk] = TMath::Sqrt(doubleME0nearVeto0efficiency[kk]*(1.0-doubleME0nearVeto0efficiency[kk])/(1.0*denom));  //Hz
   doubleME0nearVeto1efficiencyErr[kk] = TMath::Sqrt(doubleME0nearVeto1efficiency[kk]*(1.0-doubleME0nearVeto1efficiency[kk])/(1.0*denom));  //Hz
   doubleME0nearVeto2efficiencyErr[kk] = TMath::Sqrt(doubleME0nearVeto2efficiency[kk]*(1.0-doubleME0nearVeto2efficiency[kk])/(1.0*denom));  //Hz
   doubleME0nearVeto3efficiencyErr[kk] = TMath::Sqrt(doubleME0nearVeto3efficiency[kk]*(1.0-doubleME0nearVeto3efficiency[kk])/(1.0*denom));  //Hz

   doubleME0nearQuality0Veto0efficiency[kk] = doubleME0nearQuality0Veto0count[kk]*1.0/(denom);  //Hz
   doubleME0nearQuality0Veto1efficiency[kk] = doubleME0nearQuality0Veto1count[kk]*1.0/(denom);  //Hz
   doubleME0nearQuality0Veto2efficiency[kk] = doubleME0nearQuality0Veto2count[kk]*1.0/(denom);  //Hz
   doubleME0nearQuality0Veto3efficiency[kk] = doubleME0nearQuality0Veto3count[kk]*1.0/(denom);  //Hz
   doubleME0nearQuality1Veto0efficiency[kk] = doubleME0nearQuality1Veto0count[kk]*1.0/(denom);  //Hz
   doubleME0nearQuality1Veto1efficiency[kk] = doubleME0nearQuality1Veto1count[kk]*1.0/(denom);  //Hz
   doubleME0nearQuality1Veto2efficiency[kk] = doubleME0nearQuality1Veto2count[kk]*1.0/(denom);  //Hz
   doubleME0nearQuality1Veto3efficiency[kk] = doubleME0nearQuality1Veto3count[kk]*1.0/(denom);  //Hz
   doubleME0nearQuality2Veto0efficiency[kk] = doubleME0nearQuality2Veto0count[kk]*1.0/(denom);  //Hz
   doubleME0nearQuality2Veto1efficiency[kk] = doubleME0nearQuality2Veto1count[kk]*1.0/(denom);  //Hz
   doubleME0nearQuality2Veto2efficiency[kk] = doubleME0nearQuality2Veto2count[kk]*1.0/(denom);  //Hz
   doubleME0nearQuality2Veto3efficiency[kk] = doubleME0nearQuality2Veto3count[kk]*1.0/(denom);  //Hz

   doubleME0nearQuality0Veto0efficiencyErr[kk] = TMath::Sqrt(doubleME0nearQuality0Veto0efficiency[kk]*(1.0-doubleME0nearQuality0Veto0efficiency[kk])/(1.0*denom));  //Hz
   doubleME0nearQuality0Veto1efficiencyErr[kk] = TMath::Sqrt(doubleME0nearQuality0Veto1efficiency[kk]*(1.0-doubleME0nearQuality0Veto1efficiency[kk])/(1.0*denom));  //Hz
   doubleME0nearQuality0Veto2efficiencyErr[kk] = TMath::Sqrt(doubleME0nearQuality0Veto2efficiency[kk]*(1.0-doubleME0nearQuality0Veto2efficiency[kk])/(1.0*denom));  //Hz
   doubleME0nearQuality0Veto3efficiencyErr[kk] = TMath::Sqrt(doubleME0nearQuality0Veto3efficiency[kk]*(1.0-doubleME0nearQuality0Veto3efficiency[kk])/(1.0*denom));  //Hz
   doubleME0nearQuality1Veto0efficiencyErr[kk] = TMath::Sqrt(doubleME0nearQuality1Veto0efficiency[kk]*(1.0-doubleME0nearQuality1Veto0efficiency[kk])/(1.0*denom));  //Hz
   doubleME0nearQuality1Veto1efficiencyErr[kk] = TMath::Sqrt(doubleME0nearQuality1Veto1efficiency[kk]*(1.0-doubleME0nearQuality1Veto1efficiency[kk])/(1.0*denom));  //Hz
   doubleME0nearQuality1Veto2efficiencyErr[kk] = TMath::Sqrt(doubleME0nearQuality1Veto2efficiency[kk]*(1.0-doubleME0nearQuality1Veto2efficiency[kk])/(1.0*denom));  //Hz
   doubleME0nearQuality1Veto3efficiencyErr[kk] = TMath::Sqrt(doubleME0nearQuality1Veto3efficiency[kk]*(1.0-doubleME0nearQuality1Veto3efficiency[kk])/(1.0*denom));  //Hz
   doubleME0nearQuality2Veto0efficiencyErr[kk] = TMath::Sqrt(doubleME0nearQuality2Veto0efficiency[kk]*(1.0-doubleME0nearQuality2Veto0efficiency[kk])/(1.0*denom));  //Hz
   doubleME0nearQuality2Veto1efficiencyErr[kk] = TMath::Sqrt(doubleME0nearQuality2Veto1efficiency[kk]*(1.0-doubleME0nearQuality2Veto1efficiency[kk])/(1.0*denom));  //Hz
   doubleME0nearQuality2Veto2efficiencyErr[kk] = TMath::Sqrt(doubleME0nearQuality2Veto2efficiency[kk]*(1.0-doubleME0nearQuality2Veto2efficiency[kk])/(1.0*denom));  //Hz
   doubleME0nearQuality2Veto3efficiencyErr[kk] = TMath::Sqrt(doubleME0nearQuality2Veto3efficiency[kk]*(1.0-doubleME0nearQuality2Veto3efficiency[kk])/(1.0*denom));  //Hz
   }
 for (int kk=0; kk<35; kk++)   	
   {
   tripleME0nearQuality0efficiency[kk] = tripleME0nearQuality0count[kk]*1.0/(denom);  //Hz
   tripleME0nearQuality1efficiency[kk] = tripleME0nearQuality1count[kk]*1.0/(denom);  //Hz
   tripleME0nearQuality2efficiency[kk] = tripleME0nearQuality2count[kk]*1.0/(denom);  //Hz

   tripleME0nearQuality0efficiencyErr[kk] = TMath::Sqrt(tripleME0nearQuality0efficiency[kk]*(1.0-tripleME0nearQuality0efficiency[kk])/(1.0*denom));  //Hz
   tripleME0nearQuality1efficiencyErr[kk] = TMath::Sqrt(tripleME0nearQuality1efficiency[kk]*(1.0-tripleME0nearQuality1efficiency[kk])/(1.0*denom));  //Hz
   tripleME0nearQuality2efficiencyErr[kk] = TMath::Sqrt(tripleME0nearQuality2efficiency[kk]*(1.0-tripleME0nearQuality2efficiency[kk])/(1.0*denom));  //Hz

   tripleME0nearVeto0efficiency[kk] = tripleME0nearVeto0count[kk]*1.0/(denom);  //Hz
   tripleME0nearVeto1efficiency[kk] = tripleME0nearVeto1count[kk]*1.0/(denom);  //Hz
   tripleME0nearVeto2efficiency[kk] = tripleME0nearVeto2count[kk]*1.0/(denom);  //Hz
   tripleME0nearVeto3efficiency[kk] = tripleME0nearVeto3count[kk]*1.0/(denom);  //Hz

   tripleME0nearVeto0efficiencyErr[kk] = TMath::Sqrt(tripleME0nearVeto0efficiency[kk]*(1.0-tripleME0nearVeto0efficiency[kk])/(1.0*denom));  //Hz
   tripleME0nearVeto1efficiencyErr[kk] = TMath::Sqrt(tripleME0nearVeto1efficiency[kk]*(1.0-tripleME0nearVeto1efficiency[kk])/(1.0*denom));  //Hz
   tripleME0nearVeto2efficiencyErr[kk] = TMath::Sqrt(tripleME0nearVeto2efficiency[kk]*(1.0-tripleME0nearVeto2efficiency[kk])/(1.0*denom));  //Hz
   tripleME0nearVeto3efficiencyErr[kk] = TMath::Sqrt(tripleME0nearVeto3efficiency[kk]*(1.0-tripleME0nearVeto3efficiency[kk])/(1.0*denom));  //Hz

   tripleME0nearQuality0Veto0efficiency[kk] = tripleME0nearQuality0Veto0count[kk]*1.0/(denom);  //Hz
   tripleME0nearQuality0Veto1efficiency[kk] = tripleME0nearQuality0Veto1count[kk]*1.0/(denom);  //Hz
   tripleME0nearQuality0Veto2efficiency[kk] = tripleME0nearQuality0Veto2count[kk]*1.0/(denom);  //Hz
   tripleME0nearQuality0Veto3efficiency[kk] = tripleME0nearQuality0Veto3count[kk]*1.0/(denom);  //Hz
   tripleME0nearQuality1Veto0efficiency[kk] = tripleME0nearQuality1Veto0count[kk]*1.0/(denom);  //Hz
   tripleME0nearQuality1Veto1efficiency[kk] = tripleME0nearQuality1Veto1count[kk]*1.0/(denom);  //Hz
   tripleME0nearQuality1Veto2efficiency[kk] = tripleME0nearQuality1Veto2count[kk]*1.0/(denom);  //Hz
   tripleME0nearQuality1Veto3efficiency[kk] = tripleME0nearQuality1Veto3count[kk]*1.0/(denom);  //Hz
   tripleME0nearQuality2Veto0efficiency[kk] = tripleME0nearQuality2Veto0count[kk]*1.0/(denom);  //Hz
   tripleME0nearQuality2Veto1efficiency[kk] = tripleME0nearQuality2Veto1count[kk]*1.0/(denom);  //Hz
   tripleME0nearQuality2Veto2efficiency[kk] = tripleME0nearQuality2Veto2count[kk]*1.0/(denom);  //Hz
   tripleME0nearQuality2Veto3efficiency[kk] = tripleME0nearQuality2Veto3count[kk]*1.0/(denom);  //Hz

   tripleME0nearQuality0Veto0efficiencyErr[kk] = TMath::Sqrt(tripleME0nearQuality0Veto0efficiency[kk]*(1.0-tripleME0nearQuality0Veto0efficiency[kk])/(1.0*denom));  //Hz
   tripleME0nearQuality0Veto1efficiencyErr[kk] = TMath::Sqrt(tripleME0nearQuality0Veto1efficiency[kk]*(1.0-tripleME0nearQuality0Veto1efficiency[kk])/(1.0*denom));  //Hz
   tripleME0nearQuality0Veto2efficiencyErr[kk] = TMath::Sqrt(tripleME0nearQuality0Veto2efficiency[kk]*(1.0-tripleME0nearQuality0Veto2efficiency[kk])/(1.0*denom));  //Hz
   tripleME0nearQuality0Veto3efficiencyErr[kk] = TMath::Sqrt(tripleME0nearQuality0Veto3efficiency[kk]*(1.0-tripleME0nearQuality0Veto3efficiency[kk])/(1.0*denom));  //Hz
   tripleME0nearQuality1Veto0efficiencyErr[kk] = TMath::Sqrt(tripleME0nearQuality1Veto0efficiency[kk]*(1.0-tripleME0nearQuality1Veto0efficiency[kk])/(1.0*denom));  //Hz
   tripleME0nearQuality1Veto1efficiencyErr[kk] = TMath::Sqrt(tripleME0nearQuality1Veto1efficiency[kk]*(1.0-tripleME0nearQuality1Veto1efficiency[kk])/(1.0*denom));  //Hz
   tripleME0nearQuality1Veto2efficiencyErr[kk] = TMath::Sqrt(tripleME0nearQuality1Veto2efficiency[kk]*(1.0-tripleME0nearQuality1Veto2efficiency[kk])/(1.0*denom));  //Hz
   tripleME0nearQuality1Veto3efficiencyErr[kk] = TMath::Sqrt(tripleME0nearQuality1Veto3efficiency[kk]*(1.0-tripleME0nearQuality1Veto3efficiency[kk])/(1.0*denom));  //Hz
   tripleME0nearQuality2Veto0efficiencyErr[kk] = TMath::Sqrt(tripleME0nearQuality2Veto0efficiency[kk]*(1.0-tripleME0nearQuality2Veto0efficiency[kk])/(1.0*denom));  //Hz
   tripleME0nearQuality2Veto1efficiencyErr[kk] = TMath::Sqrt(tripleME0nearQuality2Veto1efficiency[kk]*(1.0-tripleME0nearQuality2Veto1efficiency[kk])/(1.0*denom));  //Hz
   tripleME0nearQuality2Veto2efficiencyErr[kk] = TMath::Sqrt(tripleME0nearQuality2Veto2efficiency[kk]*(1.0-tripleME0nearQuality2Veto2efficiency[kk])/(1.0*denom));  //Hz
   tripleME0nearQuality2Veto3efficiencyErr[kk] = TMath::Sqrt(tripleME0nearQuality2Veto3efficiency[kk]*(1.0-tripleME0nearQuality2Veto3efficiency[kk])/(1.0*denom));  //Hz

   }

 for (int kk=0; kk<5; kk++)	
   {
   singleME0Quality0rateError[kk] = singleME0Quality0efficiencyErr[kk]/(25.0*1e-9); //Hz
   singleME0Quality1rateError[kk] = singleME0Quality1efficiencyErr[kk]/(25.0*1e-9); //Hz
   singleME0Quality2rateError[kk] = singleME0Quality2efficiencyErr[kk]/(25.0*1e-9); //Hz
   singleME0Veto0rateError[kk] = singleME0Veto0efficiencyErr[kk]/(25.0*1e-9); //Hz
   singleME0Veto1rateError[kk] = singleME0Veto1efficiencyErr[kk]/(25.0*1e-9); //Hz
   singleME0Veto2rateError[kk] = singleME0Veto2efficiencyErr[kk]/(25.0*1e-9); //Hz
   singleME0Veto3rateError[kk] = singleME0Veto3efficiencyErr[kk]/(25.0*1e-9); //Hz
   singleME0Quality0Veto0rateError[kk] = singleME0Quality0Veto0efficiencyErr[kk]/(25.0*1e-9); //Hz
   singleME0Quality0Veto1rateError[kk] = singleME0Quality0Veto1efficiencyErr[kk]/(25.0*1e-9); //Hz
   singleME0Quality0Veto2rateError[kk] = singleME0Quality0Veto2efficiencyErr[kk]/(25.0*1e-9); //Hz
   singleME0Quality0Veto3rateError[kk] = singleME0Quality0Veto3efficiencyErr[kk]/(25.0*1e-9); //Hz
   singleME0Quality1Veto0rateError[kk] = singleME0Quality1Veto0efficiencyErr[kk]/(25.0*1e-9); //Hz
   singleME0Quality1Veto1rateError[kk] = singleME0Quality1Veto1efficiencyErr[kk]/(25.0*1e-9); //Hz
   singleME0Quality1Veto2rateError[kk] = singleME0Quality1Veto2efficiencyErr[kk]/(25.0*1e-9); //Hz
   singleME0Quality1Veto3rateError[kk] = singleME0Quality1Veto3efficiencyErr[kk]/(25.0*1e-9); //Hz
   singleME0Quality2Veto0rateError[kk] = singleME0Quality2Veto0efficiencyErr[kk]/(25.0*1e-9); //Hz
   singleME0Quality2Veto1rateError[kk] = singleME0Quality2Veto1efficiencyErr[kk]/(25.0*1e-9); //Hz
   singleME0Quality2Veto2rateError[kk] = singleME0Quality2Veto2efficiencyErr[kk]/(25.0*1e-9); //Hz
   singleME0Quality2Veto3rateError[kk] = singleME0Quality2Veto3efficiencyErr[kk]/(25.0*1e-9); //Hz
   }
 for (int kk=0; kk<15; kk++)   	
   {
   doubleME0nearQuality0rateError[kk] = doubleME0nearQuality0efficiencyErr[kk]/(25.0*1e-9);  //Hz
   doubleME0nearQuality1rateError[kk] = doubleME0nearQuality1efficiencyErr[kk]/(25.0*1e-9);  //Hz
   doubleME0nearQuality2rateError[kk] = doubleME0nearQuality2efficiencyErr[kk]/(25.0*1e-9);  //Hz
   doubleME0nearVeto0rateError[kk] = doubleME0nearVeto0efficiencyErr[kk]/(25.0*1e-9);  //Hz
   doubleME0nearVeto1rateError[kk] = doubleME0nearVeto1efficiencyErr[kk]/(25.0*1e-9);  //Hz
   doubleME0nearVeto2rateError[kk] = doubleME0nearVeto2efficiencyErr[kk]/(25.0*1e-9);  //Hz
   doubleME0nearVeto3rateError[kk] = doubleME0nearVeto3efficiencyErr[kk]/(25.0*1e-9);  //Hz
   doubleME0nearQuality0Veto0rateError[kk] = doubleME0nearQuality0Veto0efficiencyErr[kk]/(25.0*1e-9);  //Hz
   doubleME0nearQuality0Veto1rateError[kk] = doubleME0nearQuality0Veto1efficiencyErr[kk]/(25.0*1e-9);  //Hz
   doubleME0nearQuality0Veto2rateError[kk] = doubleME0nearQuality0Veto2efficiencyErr[kk]/(25.0*1e-9);  //Hz
   doubleME0nearQuality0Veto3rateError[kk] = doubleME0nearQuality0Veto3efficiencyErr[kk]/(25.0*1e-9);  //Hz
   doubleME0nearQuality1Veto0rateError[kk] = doubleME0nearQuality1Veto0efficiencyErr[kk]/(25.0*1e-9);  //Hz
   doubleME0nearQuality1Veto1rateError[kk] = doubleME0nearQuality1Veto1efficiencyErr[kk]/(25.0*1e-9);  //Hz
   doubleME0nearQuality1Veto2rateError[kk] = doubleME0nearQuality1Veto2efficiencyErr[kk]/(25.0*1e-9);  //Hz
   doubleME0nearQuality1Veto3rateError[kk] = doubleME0nearQuality1Veto3efficiencyErr[kk]/(25.0*1e-9);  //Hz
   doubleME0nearQuality2Veto0rateError[kk] = doubleME0nearQuality2Veto0efficiencyErr[kk]/(25.0*1e-9);  //Hz
   doubleME0nearQuality2Veto1rateError[kk] = doubleME0nearQuality2Veto1efficiencyErr[kk]/(25.0*1e-9);  //Hz
   doubleME0nearQuality2Veto2rateError[kk] = doubleME0nearQuality2Veto2efficiencyErr[kk]/(25.0*1e-9);  //Hz
   doubleME0nearQuality2Veto3rateError[kk] = doubleME0nearQuality2Veto3efficiencyErr[kk]/(25.0*1e-9);  //Hz
   }
 for (int kk=0; kk<35; kk++)   	
   {
   tripleME0nearQuality0rateError[kk] = tripleME0nearQuality0efficiencyErr[kk]/(25.0*1e-9);  //Hz
   tripleME0nearQuality1rateError[kk] = tripleME0nearQuality1efficiencyErr[kk]/(25.0*1e-9);  //Hz
   tripleME0nearQuality2rateError[kk] = tripleME0nearQuality2efficiencyErr[kk]/(25.0*1e-9);  //Hz
   tripleME0nearVeto0rateError[kk] = tripleME0nearVeto0efficiencyErr[kk]/(25.0*1e-9);  //Hz
   tripleME0nearVeto1rateError[kk] = tripleME0nearVeto1efficiencyErr[kk]/(25.0*1e-9);  //Hz
   tripleME0nearVeto2rateError[kk] = tripleME0nearVeto2efficiencyErr[kk]/(25.0*1e-9);  //Hz
   tripleME0nearVeto3rateError[kk] = tripleME0nearVeto3efficiencyErr[kk]/(25.0*1e-9);  //Hz
   tripleME0nearQuality0Veto0rateError[kk] = tripleME0nearQuality0Veto0efficiencyErr[kk]/(25.0*1e-9);  //Hz
   tripleME0nearQuality0Veto1rateError[kk] = tripleME0nearQuality0Veto1efficiencyErr[kk]/(25.0*1e-9);  //Hz
   tripleME0nearQuality0Veto2rateError[kk] = tripleME0nearQuality0Veto2efficiencyErr[kk]/(25.0*1e-9);  //Hz
   tripleME0nearQuality0Veto3rateError[kk] = tripleME0nearQuality0Veto3efficiencyErr[kk]/(25.0*1e-9);  //Hz
   tripleME0nearQuality1Veto0rateError[kk] = tripleME0nearQuality1Veto0efficiencyErr[kk]/(25.0*1e-9);  //Hz
   tripleME0nearQuality1Veto1rateError[kk] = tripleME0nearQuality1Veto1efficiencyErr[kk]/(25.0*1e-9);  //Hz
   tripleME0nearQuality1Veto2rateError[kk] = tripleME0nearQuality1Veto2efficiencyErr[kk]/(25.0*1e-9);  //Hz
   tripleME0nearQuality1Veto3rateError[kk] = tripleME0nearQuality1Veto3efficiencyErr[kk]/(25.0*1e-9);  //Hz
   tripleME0nearQuality2Veto0rateError[kk] = tripleME0nearQuality2Veto0efficiencyErr[kk]/(25.0*1e-9);  //Hz
   tripleME0nearQuality2Veto1rateError[kk] = tripleME0nearQuality2Veto1efficiencyErr[kk]/(25.0*1e-9);  //Hz
   tripleME0nearQuality2Veto2rateError[kk] = tripleME0nearQuality2Veto2efficiencyErr[kk]/(25.0*1e-9);  //Hz
   tripleME0nearQuality2Veto3rateError[kk] = tripleME0nearQuality2Veto3efficiencyErr[kk]/(25.0*1e-9);  //Hz
   }

 //for (int kk=0; kk<5; kk++)	cout << "singleME0rate[kk] = " << singleME0rate[kk] << endl;
 //for (int kk=0; kk<15; kk++)	cout << "doubleME0rate[kk] = " << doubleME0rate[kk] << endl;
 //for (int kk=0; kk<35; kk++)	cout << "tripleME0rate[kk] = " << tripleME0rate[kk] << endl;

 cout << "Filling Trigger trees..." << endl;
 trSum->Fill();
 tr2->Fill();
 tr2near->Fill();
 tr2nearQuality0->Fill();
 tr2nearQuality1->Fill();
 tr2nearQuality2->Fill();
 tr2nearVeto0->Fill();
 tr2nearVeto1->Fill();
 tr2nearVeto2->Fill();
 tr2nearVeto3->Fill();
 tr2nearQuality0Veto0->Fill();
 tr2nearQuality0Veto1->Fill();
 tr2nearQuality0Veto2->Fill();
 tr2nearQuality0Veto3->Fill();
 tr2nearQuality1Veto0->Fill();
 tr2nearQuality1Veto1->Fill();
 tr2nearQuality1Veto2->Fill();
 tr2nearQuality1Veto3->Fill();
 tr2nearQuality2Veto0->Fill();
 tr2nearQuality2Veto1->Fill();
 tr2nearQuality2Veto2->Fill();
 tr2nearQuality2Veto3->Fill();
 tr2close->Fill();
 cout << "...Done." << endl;

//=========== Draw Graph rate vs quality =======================
TPaveText * prel = cmsPrel();
c_quality->cd();

//cms aesthetics
c_quality->SetFillColor(0);
c_quality->SetBorderMode(0);
c_quality->SetFrameFillStyle(0);
c_quality->SetFrameBorderMode(0);
c_quality->SetLeftMargin( L/W );
c_quality->SetRightMargin( R/W );
c_quality->SetTopMargin( T/H );
c_quality->SetBottomMargin( B/H );
c_quality->SetTickx(0);
c_quality->SetTicky(0);
//end cms aesthetics

c_quality->SetGridy();
c_quality->SetGridx();
c_quality->SetLogy();
//c_quality->SetGridx(5);
c_quality->Update();
c_quality->cd();
gStyle->SetOptTitle(0);

Double_t x_q[3]={0,1,2};
Double_t errx[3]={0,0,0}     ;
Double_t y_qsingle[3];
Double_t y_qdouble[3];
Double_t y_qtriple[3];
///*Double_t*/ x_q[3] 		= { 0,1,2 };
///*Double_t*/ errx[3] 	= { 0, 0, 0};
/*Double_t*/ y_qsingle[0] 	= singleME0Quality0rate[2]/1e3; //threshold 10 GeV
/*Double_t*/ y_qsingle[1] 	= singleME0Quality1rate[2]/1e3; //threshold 10 GeV
/*Double_t*/ y_qsingle[2] 	= singleME0Quality2rate[2]/1e3; //threshold 10 GeV
/*Double_t*/ y_qdouble[0] 	= doubleME0nearQuality0rate[9]/1e3; //thresholds 10 GeV
/*Double_t*/ y_qdouble[1] 	= doubleME0nearQuality1rate[9]/1e3; //thresholds 10 GeV
/*Double_t*/ y_qdouble[2] 	= doubleME0nearQuality2rate[9]/1e3; //thresholds 10 GeV
/*Double_t*/ y_qtriple[0] 	= tripleME0nearQuality0rate[25]/1e3; //thresholds 10 GeV
/*Double_t*/ y_qtriple[1] 	= tripleME0nearQuality1rate[25]/1e3; //thresholds 10 GeV
/*Double_t*/ y_qtriple[2] 	= tripleME0nearQuality2rate[25]/1e3; //thresholds 10 GeV
Double_t erry_qsingle[3] = { singleME0Quality0rateError[2]/1e3, singleME0Quality1rateError[2]/1e3, singleME0Quality2rateError[2]/1e3 };
Double_t erry_qdouble[3] = { doubleME0nearQuality0rateError[9]/1e3, doubleME0nearQuality1rateError[9]/1e3, doubleME0nearQuality2rateError[9]/1e3 };
Double_t erry_qtriple[3] = { tripleME0nearQuality0rateError[25]/1e3, tripleME0nearQuality1rateError[25]/1e3, tripleME0nearQuality2rateError[25]/1e3 };
//Double_t erry_qsingle[3] = { float(TMath::Sqrt(1.0*singleME0Quality0count[2]))/(denom*25.0*1e-9), 	 float(TMath::Sqrt(1.0*singleME0Quality1rate[2]))/(denom*25.0*1e-9),     float(TMath::Sqrt(1.0*singleME0Quality2rate[2]))/(denom*25.0*1e-9) }; //threshold 10 GeV
//Double_t erry_qdouble[3] = { float(TMath::Sqrt(1.0*doubleME0nearQuality0count[2]))/(denom*25.0*1e-9), float(TMath::Sqrt(1.0*doubleME0nearQuality1rate[2]))/(denom*25.0*1e-9), float(TMath::Sqrt(1.0*doubleME0nearQuality2rate[2]))/(denom*25.0*1e-9) }; //thresholds 10 GeV
//Double_t erry_qtriple[3] = { float(TMath::Sqrt(1.0*tripleME0nearQuality0count[2]))/(denom*25.0*1e-9), float(TMath::Sqrt(1.0*tripleME0nearQuality1rate[2]))/(denom*25.0*1e-9), float(TMath::Sqrt(1.0*tripleME0nearQuality2rate[2]))/(denom*25.0*1e-9) }; //thresholds 10 GeV
//
TGraphErrors * g_qsingle = new TGraphErrors (3, x_q, y_qsingle, errx, erry_qsingle); //erry_qsingle);
TGraphErrors * g_qdouble = new TGraphErrors (3, x_q, y_qdouble, errx, erry_qdouble); //erry_qdouble);
TGraphErrors * g_qtriple = new TGraphErrors (3, x_q, y_qtriple, errx, erry_qtriple); //erry_qtriple);


        g_qsingle->GetXaxis()->SetTitleFont(42);
        g_qsingle->GetXaxis()->SetTitleSize(0.06);
        g_qsingle->GetXaxis()->SetTitleOffset(0.9);
        g_qsingle->GetXaxis()->SetLabelOffset(0.007);
        g_qsingle->GetXaxis()->SetLabelSize(0.05);
        //g_qsingle->GetXaxis()->SetDecimals(kTRUE);
        g_qsingle->GetXaxis()->SetTickLength(0.03);
        //g_qsingle->GetYaxis()->SetTitleColor(kRed);
        g_qsingle->GetYaxis()->SetTitleFont(42);
        g_qsingle->GetYaxis()->SetTitleSize(0.06);
        g_qsingle->GetYaxis()->SetTitleOffset(1.2);
        g_qsingle->GetYaxis()->SetLabelOffset(0.007);
        g_qsingle->GetYaxis()->SetLabelSize(0.05);
        TGaxis::SetMaxDigits(1);
        //g_qsingle->GetYaxis()->SetLabelColor(kRed);
        g_qsingle->GetYaxis()->SetDecimals(kTRUE);
        g_qsingle->GetYaxis()->SetTickLength(0.03);
        g_qsingle->GetXaxis()->SetTitle("Segment quality");
        g_qsingle->GetYaxis()->SetTitle("Rate [kHZ]");
        //g_qsingle->GetYaxis()->SetRangeUser(0,1);
        //g_qsingle->GetYaxis()->SetLimits(0,1);
        //g_qsingle->GetXaxis()->SetLimits(1025/div,1600/div);
        // g_qsingle->Write(grTitle);


	g_qsingle->SetMarkerStyle(20);
	g_qsingle->SetMarkerSize(1.2);
        g_qsingle->SetLineColor(kGreen);     
        g_qsingle->SetMarkerColor(kGreen);     

	g_qdouble->SetMarkerStyle(21);
	g_qdouble->SetMarkerSize(1.2);
        g_qdouble->SetLineColor(kBlue);     
        g_qdouble->SetMarkerColor(kBlue);     

	g_qtriple->SetMarkerStyle(22);
        g_qtriple->SetMarkerSize(1.2);     
        g_qtriple->SetMarkerColor(kRed);     

	g_qsingle->Draw("AP");
	g_qsingle->SetTitle("");
	g_qdouble->Draw("SAMEP");
	g_qtriple->Draw("SAMEP");

      prel->Draw("NB");
      TLegend *leg = new TLegend(0.55,0.72,0.92,0.92);
      leg->SetTextSize(0.04);
      leg->SetTextFont(42);
      leg->AddEntry(g_qsingle, "Single #mu p_{t}>10 GeV", "p");
      leg->AddEntry(g_qdouble, "Double #mu p_{t}>10 GeV", "p");
      leg->AddEntry(g_qtriple, "Triple #mu p_{t}>10 GeV", "p");
      leg->AddEntry(g_qtriple, "PU200", "");
      leg->Draw();

	g_qsingle->GetYaxis()->SetLimits(-1000,1e5);
	g_qsingle->GetYaxis()->SetRangeUser(-1000,1e5);
	g_qsingle->GetXaxis()->SetRangeUser(-0.4,2.4);
	g_qsingle->GetXaxis()->SetLimits(-0.5,2.5);
        //g_qsingle->GetXaxis()->TGaxis::SetExponentOffset(-0.05, 0, "y");
        //g_qsingle->GetXaxis()->SetNoExponent(kFALSE);
        g_qsingle->GetXaxis()->SetNdivisions(5);
        g_qsingle->GetYaxis()->SetNdivisions(510);

      c_quality->Update();
      //write PU200 and TMath::Sqrt(s)=13TeV

//=========== Draw Graph rate vs eta veto  =======================
//TPaveText * prel = cmsPrel();

c_eveto->cd();
//cms aesthetics
c_eveto->SetFillColor(0);
c_eveto->SetBorderMode(0);
c_eveto->SetFrameFillStyle(0);
c_eveto->SetFrameBorderMode(0);
c_eveto->SetLeftMargin( L/W );
c_eveto->SetRightMargin( R/W );
c_eveto->SetTopMargin( T/H );
c_eveto->SetBottomMargin( B/H );
c_eveto->SetTickx(0);
c_eveto->SetTicky(0);
//end cms aesthetics

c_eveto->SetGridy();
c_eveto->SetGridx();
c_eveto->SetLogy();
//c_eveto->SetGridx(5);
c_eveto->Update();
c_eveto->cd();
gStyle->SetOptTitle(0);
///*Double_t*/ x_ev[3] 		= { 0,1,2 };
///*Double_t*/ errx[3] 	= { 0, 0, 0};
Double_t y_evsingle[4], y_evdouble[4], y_evtriple[4];
/*Double_t*/ y_evsingle[0] 	= singleME0Veto0rate[2]/1e3; //threshold 10 GeV
/*Double_t*/ y_evsingle[1] 	= singleME0Veto1rate[2]/1e3; //threshold 10 GeV
/*Double_t*/ y_evsingle[2] 	= singleME0Veto2rate[2]/1e3; //threshold 10 GeV
/*Double_t*/ y_evsingle[3] 	= singleME0Veto3rate[2]/1e3; //threshold 10 GeV
/*Double_t*/ y_evdouble[0] 	= doubleME0nearVeto0rate[9]/1e3; //thresholds 10 GeV
/*Double_t*/ y_evdouble[1] 	= doubleME0nearVeto1rate[9]/1e3; //thresholds 10 GeV
/*Double_t*/ y_evdouble[2] 	= doubleME0nearVeto2rate[9]/1e3; //thresholds 10 GeV
/*Double_t*/ y_evdouble[3] 	= doubleME0nearVeto3rate[9]/1e3; //thresholds 10 GeV
/*Double_t*/ y_evtriple[0] 	= tripleME0nearVeto0rate[25]/1e3; //thresholds 10 GeV
/*Double_t*/ y_evtriple[1] 	= tripleME0nearVeto1rate[25]/1e3; //thresholds 10 GeV
/*Double_t*/ y_evtriple[2] 	= tripleME0nearVeto2rate[25]/1e3; //thresholds 10 GeV
/*Double_t*/ y_evtriple[3] 	= tripleME0nearVeto3rate[25]/1e3; //thresholds 10 GeV
Double_t erry_evsingle[4] = { singleME0Veto0rateError[2]/1e3, singleME0Veto1rateError[2]/1e3, singleME0Veto2rateError[2]/1e3, singleME0Veto3rateError[2]/1e3 };
Double_t erry_evdouble[4] = { doubleME0nearVeto0rateError[9]/1e3, doubleME0nearVeto1rateError[9]/1e3, doubleME0nearVeto2rateError[9]/1e3, doubleME0nearVeto3rateError[9]/1e3 };
Double_t erry_evtriple[4] = { tripleME0nearVeto0rateError[25]/1e3, tripleME0nearVeto1rateError[25]/1e3, tripleME0nearVeto2rateError[25]/1e3, tripleME0nearVeto3rateError[25]/1e3 };
Double_t x_ev[4] = {0,1,2,3};
Double_t errx4[4] = {0,0,0,0};

//Double_t erry_evsingle[3] = { float(TMath::Sqrt(1.0*singleME0Veto0count[2]))/(denom*25.0*1e-9), 	 float(TMath::Sqrt(1.0*singleME0Veto1rate[2]))/(denom*25.0*1e-9),     float(TMath::Sqrt(1.0*singleME0Veto2rate[2]))/(denom*25.0*1e-9) }; //threshold 10 GeV
//Double_t erry_evdouble[3] = { float(TMath::Sqrt(1.0*doubleME0nearVeto0count[2]))/(denom*25.0*1e-9), float(TMath::Sqrt(1.0*doubleME0nearVeto1rate[2]))/(denom*25.0*1e-9), float(TMath::Sqrt(1.0*doubleME0nearVeto2rate[2]))/(denom*25.0*1e-9) }; //thresholds 10 GeV
//Double_t erry_evtriple[3] = { float(TMath::Sqrt(1.0*tripleME0nearVeto0count[2]))/(denom*25.0*1e-9), float(TMath::Sqrt(1.0*tripleME0nearVeto1rate[2]))/(denom*25.0*1e-9), float(TMath::Sqrt(1.0*tripleME0nearVeto2rate[2]))/(denom*25.0*1e-9) }; //thresholds 10 GeV
//
TGraphErrors * g_evsingle = new TGraphErrors (4, x_ev, y_evsingle, errx4, erry_evsingle); //erry_evsingle);
TGraphErrors * g_evdouble = new TGraphErrors (4, x_ev, y_evdouble, errx4, erry_evdouble); //erry_evdouble);
TGraphErrors * g_evtriple = new TGraphErrors (4, x_ev, y_evtriple, errx4, erry_evtriple); //erry_evtriple);


        g_evtriple->GetXaxis()->SetTitleFont(42);
        g_evtriple->GetXaxis()->SetTitleSize(0.06);
        g_evtriple->GetXaxis()->SetTitleOffset(0.9);
        g_evtriple->GetXaxis()->SetLabelOffset(0.007);
        g_evtriple->GetXaxis()->SetLabelSize(0.05);
        //g_evtriple->GetXaxis()->SetDecimals(kTRUE);
        g_evtriple->GetXaxis()->SetTickLength(0.03);
        //g_evtriple->GetYaxis()->SetTitleColor(kRed);
        g_evtriple->GetYaxis()->SetTitleFont(42);
        g_evtriple->GetYaxis()->SetTitleSize(0.06);
        g_evtriple->GetYaxis()->SetTitleOffset(1.2);
        g_evtriple->GetYaxis()->SetLabelOffset(0.007);
        g_evtriple->GetYaxis()->SetLabelSize(0.05);
        TGaxis::SetMaxDigits(1);
        //g_evtriple->GetYaxis()->SetLabelColor(kRed);
        g_evtriple->GetYaxis()->SetDecimals(kTRUE);
        g_evtriple->GetYaxis()->SetTickLength(0.03);
        g_evtriple->GetXaxis()->SetTitle("#eta veto");
        g_evtriple->GetYaxis()->SetTitle("Rate [kHZ]");
        //g_evtriple->GetYaxis()->SetRangeUser(0,1);
        //g_evtriple->GetYaxis()->SetLimits(0,1);
        //g_evtriple->GetXaxis()->SetLimits(1025/div,1600/div);
        // g_evtriple->Write(grTitle);


	g_evsingle->SetMarkerStyle(20);
	g_evsingle->SetMarkerSize(1.2);
        g_evsingle->SetLineColor(kGreen);     
        g_evsingle->SetMarkerColor(kGreen);     

	g_evdouble->SetMarkerStyle(21);
	g_evdouble->SetMarkerSize(1.2);
        g_evdouble->SetLineColor(kBlue);     
        g_evdouble->SetMarkerColor(kBlue);     

	g_evtriple->SetMarkerStyle(22);
	g_evtriple->SetMarkerSize(1.2);
        g_evtriple->SetLineColor(kRed);     
        g_evtriple->SetMarkerColor(kRed);     

	g_evtriple->SetTitle("");
	g_evtriple->Draw("AP");
	g_evdouble->Draw("SAMEP");
	g_evsingle->Draw("SAMEP");

      prel->Draw("NB");
      TLegend *leg2 = new TLegend(0.55,0.72,0.92,0.92);
      leg2->SetTextSize(0.04);
      leg2->SetTextFont(42);
      leg2->AddEntry(g_evsingle, "Single #mu p_{t}>10 GeV", "p");
      leg2->AddEntry(g_evdouble, "Double #mu p_{t}>10 GeV", "p");
      leg2->AddEntry(g_evtriple, "Triple #mu p_{t}>10 GeV", "p");
      leg2->AddEntry(g_evtriple, "PU200", "");
      leg2->Draw();

	g_evtriple->GetYaxis()->SetLimits(-1000,1e5);
	g_evtriple->GetYaxis()->SetRangeUser(-1000,1e5);
	g_evtriple->GetXaxis()->SetRangeUser(-0.5,3.5);
	g_evtriple->GetXaxis()->SetLimits(-0.5,3.5);
        //g_evtriple->GetXaxis()->TGaxis::SetExponentOffset(-0.05, 0, "y");
        //g_evtriple->GetXaxis()->SetNoExponent(kFALSE);
        g_evtriple->GetXaxis()->SetNdivisions(5);
        g_evtriple->GetYaxis()->SetNdivisions(510);

      c_eveto->Update();

//===================== More TGraphs =============================================

Double_t y_evsingleQuality0[4], y_evdoubleQuality0[4], y_evtripleQuality0[4];
y_evsingleQuality0[0] 	= singleME0Quality0Veto0rate[2]/1e3; //threshold 10 GeV
y_evsingleQuality0[1] 	= singleME0Quality0Veto1rate[2]/1e3; //threshold 10 GeV
y_evsingleQuality0[2] 	= singleME0Quality0Veto2rate[2]/1e3; //threshold 10 GeV
y_evsingleQuality0[3] 	= singleME0Quality0Veto3rate[2]/1e3; //threshold 10 GeV
y_evdoubleQuality0[0] 	= doubleME0nearQuality0Veto0rate[9]/1e3; //thresholds 10 GeV
y_evdoubleQuality0[1] 	= doubleME0nearQuality0Veto1rate[9]/1e3; //thresholds 10 GeV
y_evdoubleQuality0[2] 	= doubleME0nearQuality0Veto2rate[9]/1e3; //thresholds 10 GeV
y_evdoubleQuality0[3] 	= doubleME0nearQuality0Veto3rate[9]/1e3; //thresholds 10 GeV
y_evtripleQuality0[0] 	= tripleME0nearQuality0Veto0rate[25]/1e3; //thresholds 10 GeV
y_evtripleQuality0[1] 	= tripleME0nearQuality0Veto1rate[25]/1e3; //thresholds 10 GeV
y_evtripleQuality0[2] 	= tripleME0nearQuality0Veto2rate[25]/1e3; //thresholds 10 GeV
y_evtripleQuality0[3] 	= tripleME0nearQuality0Veto3rate[25]/1e3; //thresholds 10 GeV
Double_t erry_evsingleQuality0[4] = { singleME0Quality0Veto0rateError[2]/1e3, singleME0Quality0Veto1rateError[2]/1e3, singleME0Quality0Veto2rateError[2]/1e3, singleME0Quality0Veto3rateError[2]/1e3 };
Double_t erry_evdoubleQuality0[4] = { doubleME0nearQuality0Veto0rateError[9]/1e3, doubleME0nearQuality0Veto1rateError[9]/1e3, doubleME0nearQuality0Veto2rateError[9]/1e3, doubleME0nearQuality0Veto3rateError[9]/1e3 };
Double_t erry_evtripleQuality0[4] = { tripleME0nearQuality0Veto0rateError[25]/1e3, tripleME0nearQuality0Veto1rateError[25]/1e3, tripleME0nearQuality0Veto2rateError[25]/1e3, tripleME0nearQuality0Veto3rateError[25]/1e3 };

TGraphErrors * g_evsingleQuality0 = new TGraphErrors (4, x_ev, y_evsingleQuality0, errx4, erry_evsingleQuality0); //erry_evsingle);
TGraphErrors * g_evdoubleQuality0 = new TGraphErrors (4, x_ev, y_evdoubleQuality0, errx4, erry_evdoubleQuality0); //erry_evdouble);
TGraphErrors * g_evtripleQuality0 = new TGraphErrors (4, x_ev, y_evtripleQuality0, errx4, erry_evtripleQuality0); //erry_evtriple);



Double_t y_evsingleQuality1[4], y_evdoubleQuality1[4], y_evtripleQuality1[4];
y_evsingleQuality1[0] 	= singleME0Quality1Veto0rate[2]/1e3; //threshold 10 GeV
y_evsingleQuality1[1] 	= singleME0Quality1Veto1rate[2]/1e3; //threshold 10 GeV
y_evsingleQuality1[2] 	= singleME0Quality1Veto2rate[2]/1e3; //threshold 10 GeV
y_evsingleQuality1[3] 	= singleME0Quality1Veto3rate[2]/1e3; //threshold 10 GeV
y_evdoubleQuality1[0] 	= doubleME0nearQuality1Veto0rate[9]/1e3; //thresholds 10 GeV
y_evdoubleQuality1[1] 	= doubleME0nearQuality1Veto1rate[9]/1e3; //thresholds 10 GeV
y_evdoubleQuality1[2] 	= doubleME0nearQuality1Veto2rate[9]/1e3; //thresholds 10 GeV
y_evdoubleQuality1[3] 	= doubleME0nearQuality1Veto3rate[9]/1e3; //thresholds 10 GeV
y_evtripleQuality1[0] 	= tripleME0nearQuality1Veto0rate[25]/1e3; //thresholds 10 GeV
y_evtripleQuality1[1] 	= tripleME0nearQuality1Veto1rate[25]/1e3; //thresholds 10 GeV
y_evtripleQuality1[2] 	= tripleME0nearQuality1Veto2rate[25]/1e3; //thresholds 10 GeV
y_evtripleQuality1[3] 	= tripleME0nearQuality1Veto3rate[25]/1e3; //thresholds 10 GeV
Double_t erry_evsingleQuality1[4] = { singleME0Quality1Veto0rateError[2]/1e3, singleME0Quality1Veto1rateError[2]/1e3, singleME0Quality1Veto2rateError[2]/1e3, singleME0Quality1Veto3rateError[2]/1e3 };
Double_t erry_evdoubleQuality1[4] = { doubleME0nearQuality1Veto0rateError[9]/1e3, doubleME0nearQuality1Veto1rateError[9]/1e3, doubleME0nearQuality1Veto2rateError[9]/1e3, doubleME0nearQuality1Veto3rateError[9]/1e3 };
Double_t erry_evtripleQuality1[4] = { tripleME0nearQuality1Veto0rateError[25]/1e3, tripleME0nearQuality1Veto1rateError[25]/1e3, tripleME0nearQuality1Veto2rateError[25]/1e3, tripleME0nearQuality1Veto3rateError[25]/1e3 };

TGraphErrors * g_evsingleQuality1 = new TGraphErrors (4, x_ev, y_evsingleQuality1, errx4, erry_evsingleQuality1); //erry_evsingle);
TGraphErrors * g_evdoubleQuality1 = new TGraphErrors (4, x_ev, y_evdoubleQuality1, errx4, erry_evdoubleQuality1); //erry_evdouble);
TGraphErrors * g_evtripleQuality1 = new TGraphErrors (4, x_ev, y_evtripleQuality1, errx4, erry_evtripleQuality1); //erry_evtriple);



Double_t y_evsingleQuality2[4], y_evdoubleQuality2[4], y_evtripleQuality2[4];
y_evsingleQuality2[0] 	= singleME0Quality2Veto0rate[2]/1e3; //threshold 10 GeV
y_evsingleQuality2[1] 	= singleME0Quality2Veto1rate[2]/1e3; //threshold 10 GeV
y_evsingleQuality2[2] 	= singleME0Quality2Veto2rate[2]/1e3; //threshold 10 GeV
y_evsingleQuality2[3] 	= singleME0Quality2Veto3rate[2]/1e3; //threshold 10 GeV
y_evdoubleQuality2[0] 	= doubleME0nearQuality2Veto0rate[9]/1e3; //thresholds 10 GeV
y_evdoubleQuality2[1] 	= doubleME0nearQuality2Veto1rate[9]/1e3; //thresholds 10 GeV
y_evdoubleQuality2[2] 	= doubleME0nearQuality2Veto2rate[9]/1e3; //thresholds 10 GeV
y_evdoubleQuality2[3] 	= doubleME0nearQuality2Veto3rate[9]/1e3; //thresholds 10 GeV
y_evtripleQuality2[0] 	= tripleME0nearQuality2Veto0rate[25]/1e3; //thresholds 10 GeV
y_evtripleQuality2[1] 	= tripleME0nearQuality2Veto1rate[25]/1e3; //thresholds 10 GeV
y_evtripleQuality2[2] 	= tripleME0nearQuality2Veto2rate[25]/1e3; //thresholds 10 GeV
y_evtripleQuality2[3] 	= tripleME0nearQuality2Veto3rate[25]/1e3; //thresholds 10 GeV
Double_t erry_evsingleQuality2[4] = { singleME0Quality2Veto0rateError[2]/1e3, singleME0Quality2Veto1rateError[2]/1e3, singleME0Quality2Veto2rateError[2]/1e3, singleME0Quality2Veto3rateError[2]/1e3 };
Double_t erry_evdoubleQuality2[4] = { doubleME0nearQuality2Veto0rateError[9]/1e3, doubleME0nearQuality2Veto1rateError[9]/1e3, doubleME0nearQuality2Veto2rateError[9]/1e3, doubleME0nearQuality2Veto3rateError[9]/1e3 };
Double_t erry_evtripleQuality2[4] = { tripleME0nearQuality2Veto0rateError[25]/1e3, tripleME0nearQuality2Veto1rateError[25]/1e3, tripleME0nearQuality2Veto2rateError[25]/1e3, tripleME0nearQuality2Veto3rateError[25]/1e3 };

TGraphErrors * g_evsingleQuality2 = new TGraphErrors (4, x_ev, y_evsingleQuality2, errx4, erry_evsingleQuality2); //erry_evsingle);
TGraphErrors * g_evdoubleQuality2 = new TGraphErrors (4, x_ev, y_evdoubleQuality2, errx4, erry_evdoubleQuality2); //erry_evdouble);
TGraphErrors * g_evtripleQuality2 = new TGraphErrors (4, x_ev, y_evtripleQuality2, errx4, erry_evtripleQuality2); //erry_evtriple);

//=======================elements coordinates==================================

Float_t l1=0.2;
Float_t l2=0.2;
Float_t l3=0.62; //0.85
Float_t l4=0.43;
Float_t p1=0.65;//0.6
Float_t p2=0.88;
Float_t p3=0.88; //0.855
Float_t p4=0.94;

//=========== Draw Graph rate vs selection SINGLE mu  =======================
//TPaveText * prel = cmsPrel();

c_singlemu->cd();
//cms aesthetics
c_singlemu->SetFillColor(0);
c_singlemu->SetBorderMode(0);
c_singlemu->SetFrameFillStyle(0);
c_singlemu->SetFrameBorderMode(0);
c_singlemu->SetLeftMargin( L/W );
c_singlemu->SetRightMargin( R/W );
c_singlemu->SetTopMargin( T/H );
c_singlemu->SetBottomMargin( B/H );
c_singlemu->SetTickx(0);
c_singlemu->SetTicky(0);
//end cms aesthetics

c_singlemu->SetGridy();
c_singlemu->SetGridx();
c_singlemu->SetLogy();
//c_singlemu->SetGridx(5);
c_singlemu->Update();
c_singlemu->cd();
gStyle->SetOptTitle(0);

        g_evsingleQuality0->GetXaxis()->SetTitleFont(42);
        g_evsingleQuality0->GetXaxis()->SetTitleSize(0.06);
        g_evsingleQuality0->GetXaxis()->SetTitleOffset(0.9);
        g_evsingleQuality0->GetXaxis()->SetLabelOffset(0.007);
        g_evsingleQuality0->GetXaxis()->SetLabelSize(0.05);
        //g_evsingleQuality0->GetXaxis()->SetDecimals(kTRUE);
        g_evsingleQuality0->GetXaxis()->SetTickLength(0.03);
        //g_evsingleQuality0->GetYaxis()->SetTitleColor(kRed);
        g_evsingleQuality0->GetYaxis()->SetTitleFont(42);
        g_evsingleQuality0->GetYaxis()->SetTitleSize(0.06);
        g_evsingleQuality0->GetYaxis()->SetTitleOffset(1.2);
        g_evsingleQuality0->GetYaxis()->SetLabelOffset(0.007);
        g_evsingleQuality0->GetYaxis()->SetLabelSize(0.05);
        TGaxis::SetMaxDigits(1);
        //g_evsingleQuality0->GetYaxis()->SetLabelColor(kRed);
        g_evsingleQuality0->GetYaxis()->SetDecimals(kTRUE);
        g_evsingleQuality0->GetYaxis()->SetTickLength(0.03);
        g_evsingleQuality0->GetXaxis()->SetTitle("#eta veto");
        g_evsingleQuality0->GetYaxis()->SetTitle("Rate [kHZ]");
        //g_evsingleQuality0->GetYaxis()->SetRangeUser(0,1);
        //g_evsingleQuality0->GetYaxis()->SetLimits(0,1);
        //g_evsingleQuality0->GetXaxis()->SetLimits(1025/div,1600/div);
        // g_evsingleQuality0->Write(grTitle);


	g_evsingleQuality0->SetMarkerStyle(20);
	g_evsingleQuality0->SetMarkerSize(1.2);
        g_evsingleQuality0->SetLineColor(kBlue);     
        g_evsingleQuality0->SetMarkerColor(kBlue);     

	g_evsingleQuality1->SetMarkerStyle(21);
	g_evsingleQuality1->SetMarkerSize(1.2);
        g_evsingleQuality1->SetLineColor(kRed);     
        g_evsingleQuality1->SetMarkerColor(kRed);     

	g_evsingleQuality2->SetMarkerStyle(22);
	g_evsingleQuality2->SetMarkerSize(1.3);
        g_evsingleQuality2->SetLineColor(kGreen);     
        g_evsingleQuality2->SetMarkerColor(kGreen);     

	g_evsingleQuality0->SetTitle("");
	g_evsingleQuality0->Draw("AP");
	g_evsingleQuality1->Draw("SAMEP");
	g_evsingleQuality2->Draw("SAMEP");

      prel->Draw("NB");
      //TLegend *leg3 = new TLegend(0.2,0.2,0.85,0.4);
      TLegend *leg3 = new TLegend(l1,l2,l3,l4);
      leg3->SetTextSize(0.04);
      leg3->SetTextFont(42);
      leg3->AddEntry(g_evsingleQuality0, "Single #mu p_{t}>10 GeV, q=0", "p");
      leg3->AddEntry(g_evsingleQuality1, "Single #mu p_{t}>10 GeV, q=1", "p");
      leg3->AddEntry(g_evsingleQuality2, "Single #mu p_{t}>10 GeV, q=2", "p");
      //leg3->AddEntry(g_evtriple, "PU200", "");
      leg3->Draw();

	g_qsingle->GetYaxis()->SetLimits(-1000,1e5);
	g_qsingle->GetYaxis()->SetRangeUser(-1000,1e5);
	g_qsingle->GetXaxis()->SetRangeUser(-0.5,3.5);
	g_qsingle->GetXaxis()->SetLimits(-0.5,3.5);
        //g_qsingle->GetXaxis()->TGaxis::SetExponentOffset(-0.05, 0, "y");
        //g_qsingle->GetXaxis()->SetNoExponent(kFALSE);
        g_qsingle->GetXaxis()->SetNdivisions(4);
        g_qsingle->GetYaxis()->SetNdivisions(510);

//TPaveText *t1=new TPaveText(0.145,0.88,0.37,0.94,"brNDC");
TPaveText *tsingle=new TPaveText(p1,p2,p3,p4,"brNDC");
  tsingle->AddText("Single #mu   PU200");
  tsingle->SetTextAlign(12);
  tsingle->SetShadowColor(trans_white);
  tsingle->SetFillColor(trans_white);
  tsingle->SetLineColor(trans_white);
  tsingle->SetLineColor(trans_white);
  tsingle->Draw("NB");
      c_singlemu->Update();
TLatex texsingle;
texsingle.SetTextSize(0.028);
texsingle.DrawLatexNDC(0.61,0.82, "#splitline{Single muon}{#splitline{Threshold: muon p_{t}>10 GeV}{PU200}}");


//=========== Draw Graph rate vs selection DOUBLE mu  =======================
//TPaveText * prel = cmsPrel();

c_doublemu->cd();
//cms aesthetics
c_doublemu->SetFillColor(0);
c_doublemu->SetBorderMode(0);
c_doublemu->SetFrameFillStyle(0);
c_doublemu->SetFrameBorderMode(0);
c_doublemu->SetLeftMargin( L/W );
c_doublemu->SetRightMargin( R/W );
c_doublemu->SetTopMargin( T/H );
c_doublemu->SetBottomMargin( B/H );
c_doublemu->SetTickx(0);
c_doublemu->SetTicky(0);
//end cms aesthetics

c_doublemu->SetGridy();
c_doublemu->SetGridx();
c_doublemu->SetLogy();
//c_doublemu->SetGridx(5);
c_doublemu->Update();
c_doublemu->cd();
gStyle->SetOptTitle(0);

        g_evdoubleQuality0->GetXaxis()->SetTitleFont(42);
        g_evdoubleQuality0->GetXaxis()->SetTitleSize(0.06);
        g_evdoubleQuality0->GetXaxis()->SetTitleOffset(0.9);
        g_evdoubleQuality0->GetXaxis()->SetLabelOffset(0.007);
        g_evdoubleQuality0->GetXaxis()->SetLabelSize(0.05);
        //g_evdoubleQuality0->GetXaxis()->SetDecimals(kTRUE);
        g_evdoubleQuality0->GetXaxis()->SetTickLength(0.03);
        //g_evdoubleQuality0->GetYaxis()->SetTitleColor(kRed);
        g_evdoubleQuality0->GetYaxis()->SetTitleFont(42);
        g_evdoubleQuality0->GetYaxis()->SetTitleSize(0.06);
        g_evdoubleQuality0->GetYaxis()->SetTitleOffset(1.2);
        g_evdoubleQuality0->GetYaxis()->SetLabelOffset(0.007);
        g_evdoubleQuality0->GetYaxis()->SetLabelSize(0.05);
        TGaxis::SetMaxDigits(1);
        //g_evdoubleQuality0->GetYaxis()->SetLabelColor(kRed);
        g_evdoubleQuality0->GetYaxis()->SetDecimals(kTRUE);
        g_evdoubleQuality0->GetYaxis()->SetTickLength(0.03);
        g_evdoubleQuality0->GetXaxis()->SetTitle("#eta veto");
        g_evdoubleQuality0->GetYaxis()->SetTitle("Rate [kHZ]");
        //g_evdoubleQuality0->GetYaxis()->SetRangeUser(0,1);
        //g_evdoubleQuality0->GetYaxis()->SetLimits(0,1);
        //g_evdoubleQuality0->GetXaxis()->SetLimits(1025/div,1600/div);
        // g_evdoubleQuality0->Write(grTitle);


	g_evdoubleQuality0->SetMarkerStyle(20);
	g_evdoubleQuality0->SetMarkerSize(1.2);
        g_evdoubleQuality0->SetLineColor(kBlue);     
        g_evdoubleQuality0->SetMarkerColor(kBlue);     

	g_evdoubleQuality1->SetMarkerStyle(21);
	g_evdoubleQuality1->SetMarkerSize(1.2);
        g_evdoubleQuality1->SetLineColor(kRed);     
        g_evdoubleQuality1->SetMarkerColor(kRed);     

	g_evdoubleQuality2->SetMarkerStyle(22);
	g_evdoubleQuality2->SetMarkerSize(1.3);
        g_evdoubleQuality2->SetLineColor(kGreen);     
        g_evdoubleQuality2->SetMarkerColor(kGreen);     

	g_evdoubleQuality0->SetTitle("");
	g_evdoubleQuality0->Draw("AP");
	g_evdoubleQuality1->Draw("SAMEP");
	g_evdoubleQuality2->Draw("SAMEP");

      prel->Draw("NB");
      //TLegend *leg4 = new TLegend(0.2,0.2,0.85,0.4);
      TLegend *leg4 = new TLegend(l1,l2,l3,l4);
      leg4->SetTextSize(0.04);
      leg4->SetTextFont(42);
      leg4->AddEntry(g_evdoubleQuality0, "Double #mu p_{t}>10 GeV, q=0", "p");
      leg4->AddEntry(g_evdoubleQuality1, "Double #mu p_{t}>10 GeV, q=1", "p");
      leg4->AddEntry(g_evdoubleQuality2, "Double #mu p_{t}>10 GeV, q=2", "p");
      //leg4->AddEntry(g_evtriple, "PU200", "");
      leg4->Draw();

	g_qdouble->GetYaxis()->SetLimits(-1000,1e5);
	g_qdouble->GetYaxis()->SetRangeUser(-1000,1e5);
	g_qdouble->GetXaxis()->SetRangeUser(-0.5,3.5);
	g_qdouble->GetXaxis()->SetLimits(-0.5,3.5);
        //g_qdouble->GetXaxis()->TGaxis::SetExponentOffset(-0.05, 0, "y");
        //g_qdouble->GetXaxis()->SetNoExponent(kFALSE);
        g_qdouble->GetXaxis()->SetNdivisions(4);
        g_qdouble->GetYaxis()->SetNdivisions(510);



//TPaveText *t1=new TPaveText(0.145,0.88,0.37,0.94,"brNDC");
TPaveText *tdouble=new TPaveText(p1,p2,p3,p4,"brNDC");
  tdouble->AddText("Double #mu   PU200");
  tdouble->SetTextAlign(12);
  tdouble->SetShadowColor(trans_white);
  tdouble->SetFillColor(trans_white);
  tdouble->SetLineColor(trans_white);
  tdouble->SetLineColor(trans_white);
  tdouble->Draw("NB");
      c_doublemu->Update();

TLatex texdouble;
texdouble.SetTextSize(0.028);
texdouble.DrawLatexNDC(0.61,0.82, "#splitline{Double muon}{#splitline{Threshold: muon p_{t}>10 GeV}{PU200}}");

//=========== Draw Graph rate vs selection TRIPLE mu  =======================
//TPaveText * prel = cmsPrel();

c_triplemu->cd();
//cms aesthetics
c_triplemu->SetFillColor(0);
c_triplemu->SetBorderMode(0);
c_triplemu->SetFrameFillStyle(0);
c_triplemu->SetFrameBorderMode(0);
c_triplemu->SetLeftMargin( L/W );
c_triplemu->SetRightMargin( R/W );
c_triplemu->SetTopMargin( T/H );
c_triplemu->SetBottomMargin( B/H );
c_triplemu->SetTickx(0);
c_triplemu->SetTicky(0);
//end cms aesthetics

c_triplemu->SetGridy();
c_triplemu->SetGridx();
c_triplemu->SetLogy();
//c_triplemu->SetGridx(5);
c_triplemu->Update();
c_triplemu->cd();
gStyle->SetOptTitle(0);

        g_evtripleQuality0->GetXaxis()->SetTitleFont(42);
        g_evtripleQuality0->GetXaxis()->SetTitleSize(0.06);
        g_evtripleQuality0->GetXaxis()->SetTitleOffset(0.9);
        g_evtripleQuality0->GetXaxis()->SetLabelOffset(0.007);
        g_evtripleQuality0->GetXaxis()->SetLabelSize(0.05);
        //g_evtripleQuality0->GetXaxis()->SetDecimals(kTRUE);
        g_evtripleQuality0->GetXaxis()->SetTickLength(0.03);
        //g_evtripleQuality0->GetYaxis()->SetTitleColor(kRed);
        g_evtripleQuality0->GetYaxis()->SetTitleFont(42);
        g_evtripleQuality0->GetYaxis()->SetTitleSize(0.06);
        g_evtripleQuality0->GetYaxis()->SetTitleOffset(1.2);
        g_evtripleQuality0->GetYaxis()->SetLabelOffset(0.007);
        g_evtripleQuality0->GetYaxis()->SetLabelSize(0.05);
        TGaxis::SetMaxDigits(1);
        //g_evtripleQuality0->GetYaxis()->SetLabelColor(kRed);
        g_evtripleQuality0->GetYaxis()->SetDecimals(kTRUE);
        g_evtripleQuality0->GetYaxis()->SetTickLength(0.03);
        g_evtripleQuality0->GetXaxis()->SetTitle("#eta veto");
        g_evtripleQuality0->GetYaxis()->SetTitle("Rate [kHZ]");
        //g_evtripleQuality0->GetYaxis()->SetRangeUser(0,1);
        //g_evtripleQuality0->GetYaxis()->SetLimits(0,1);
        //g_evtripleQuality0->GetXaxis()->SetLimits(1025/div,1600/div);
        // g_evtripleQuality0->Write(grTitle);


	g_evtripleQuality0->SetMarkerStyle(20);
	g_evtripleQuality0->SetMarkerSize(1.2);
        g_evtripleQuality0->SetLineColor(kBlue);     
        g_evtripleQuality0->SetMarkerColor(kBlue);     

	g_evtripleQuality1->SetMarkerStyle(21);
	g_evtripleQuality1->SetMarkerSize(1.2);
        g_evtripleQuality1->SetLineColor(kRed);     
        g_evtripleQuality1->SetMarkerColor(kRed);     

	g_evtripleQuality2->SetMarkerStyle(22);
	g_evtripleQuality2->SetMarkerSize(1.3);
        g_evtripleQuality2->SetLineColor(kGreen);     
        g_evtripleQuality2->SetMarkerColor(kGreen);     

	g_evtripleQuality0->SetTitle("");
	g_evtripleQuality0->Draw("AP");
	g_evtripleQuality1->Draw("SAMEP");
	g_evtripleQuality2->Draw("SAMEP");

      prel->Draw("NB");
      //TLegend *leg5 = new TLegend(0.2,0.2,0.85,0.4);
      TLegend *leg5 = new TLegend(l1,l2,l3,l4);
      leg5->SetTextSize(0.04);
      leg5->SetTextFont(42);
      leg5->AddEntry(g_evtripleQuality0, "Triple #mu p_{t}>10 GeV, q=0", "p");
      leg5->AddEntry(g_evtripleQuality1, "Triple #mu p_{t}>10 GeV, q=1", "p");
      leg5->AddEntry(g_evtripleQuality2, "Triple #mu p_{t}>10 GeV, q=2", "p");
      //leg5->AddEntry(g_evtriple, "PU200", "");
      leg5->Draw();

	g_qtriple->GetYaxis()->SetLimits(-1000,1e5);
	g_qtriple->GetYaxis()->SetRangeUser(-1000,1e5);
	g_qtriple->GetXaxis()->SetRangeUser(-0.5,3.5);
	g_qtriple->GetXaxis()->SetLimits(-0.5,3.5);
        //g_qtriple->GetXaxis()->TGaxis::SetExponentOffset(-0.05, 0, "y");
        //g_qtriple->GetXaxis()->SetNoExponent(kFALSE);
        g_qtriple->GetXaxis()->SetNdivisions(4);
        g_qtriple->GetYaxis()->SetNdivisions(510);


//TPaveText *t1=new TPaveText(0.145,0.88,0.37,0.94,"brNDC");
TPaveText *ttriple=new TPaveText(p1,p2,p3,p4,"brNDC");
  ttriple->AddText("Triple #mu   PU200");
  ttriple->SetTextAlign(12);
  ttriple->SetShadowColor(trans_white);
  ttriple->SetFillColor(trans_white);
  ttriple->SetLineColor(trans_white);
  ttriple->SetLineColor(trans_white);
  ttriple->Draw("NB");
      c_triplemu->Update();

TLatex textriple;
textriple.SetTextSize(0.028);
textriple.DrawLatexNDC(0.61,0.82, "#splitline{Triple muon}{#splitline{Threshold: muon p_{t}>10 GeV}{PU200}}");

      cout << "\nTotal mismatchCount = " << mismatchCount << endl;
      cout << "over considered totalmismatchCount = " << totalmismatchCount << endl;

}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
DeltaGlobalPhiAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(DeltaGlobalPhiAnalyzer);

//errore sull'efficienza è:
//TMath::Sqrt( e*(1-e)/T )
//dove e = efficienza
//     T = conteggi di trigger
