void ReaderME0_digibased()
{
   using namespace std;
  
   TFile *rfile = new TFile("out_digibased_both.root");
   //TFile *rfile = new TFile("out_DeltaGlobalPhiAnalyzer_1.root");

   TFile *outfile = new TFile("outAnalyzerReaderDigibased.root", "RECREATE");
 
   //string dirName = "demo";
   //TDirectory demo = TDirectory(dirName.c_str(), dirName.c_str());

   TTree *tree_event_summary = (TTree*)rfile->Get("demo/EventSummary");
   TTree *tree_event         = (TTree*)rfile->Get("demo/Event");

   //TTree of the output file of this macro

   TTree *tree_DDD = new TTree("DDDEvents", "DDDEvents");
   TTree *tree_DDR = new TTree("DDREvents", "DDREvents");
   TTree *tree_DDC = new TTree("DDCEvents", "DDCEvents");
   TTree *tree_DDG = new TTree("DDGEvents", "DDGEvents");
   TTree *tree_DDM = new TTree("DDMEvents", "DDMEvents");
   TTree *tree_DRR = new TTree("DRREvents", "DRREvents");
   TTree *tree_DRC = new TTree("DRCEvents", "DRCEvents");
   TTree *tree_DRG = new TTree("DRGEvents", "DRGEvents");
   TTree *tree_DRM = new TTree("DRMEvents", "DRMEvents");
   TTree *tree_DCC = new TTree("DCCEvents", "DCCEvents");
   TTree *tree_DCG = new TTree("DCGEvents", "DCGEvents");
   TTree *tree_DCM = new TTree("DCMEvents", "DCMEvents");
   TTree *tree_DGG = new TTree("DGGEvents", "DGGEvents");
   TTree *tree_DGM = new TTree("DGMEvents", "DGMEvents");
   TTree *tree_DMM = new TTree("DMMEvents", "DMMEvents");
 
   TTree *tree_RRR = new TTree("RRREvents", "RRREvents");
   TTree *tree_RRC = new TTree("RRCEvents", "RRCEvents");
   TTree *tree_RRG = new TTree("RRGEvents", "RRGEvents");
   TTree *tree_RRM = new TTree("RRMEvents", "RRMEvents");
   TTree *tree_RCC = new TTree("RCCEvents", "RCCEvents");
   TTree *tree_RCG = new TTree("RCGEvents", "RCGEvents");
   TTree *tree_RCM = new TTree("RCMEvents", "RCMEvents");
   TTree *tree_RGG = new TTree("RGGEvents", "RGGEvents");
   TTree *tree_RGM = new TTree("RGMEvents", "RGMEvents");
   TTree *tree_RMM = new TTree("RMMEvents", "RMMEvents");
   TTree *tree_CCC = new TTree("CCCEvents", "CCCEvents");
   TTree *tree_CCG = new TTree("CCGEvents", "CCGEvents");
   TTree *tree_CCM = new TTree("CCMEvents", "CCMEvents");
   TTree *tree_CGG = new TTree("CGGEvents", "CGGEvents");
   TTree *tree_CGM = new TTree("CGMEvents", "CGMEvents");
   TTree *tree_CMM = new TTree("CMMEvents", "CMMEvents");
   TTree *tree_GGG = new TTree("GGGEvents", "GGGEvents");
   TTree *tree_GGM = new TTree("GGMEvents", "GGMEvents");
   TTree *tree_GMM = new TTree("GMMEvents", "GMMEvents");
   TTree *tree_MMM = new TTree("MMMEvents", "MMMEvents");

   TTree *tree_summary = new TTree("EventSummary", "EventSummary");

   //declarations and initialisations
   Int_t lastEvent = 0;
   Int_t lastEvent_total = 0;

   Int_t nMuNNN = 0;	 Int_t nMuDDD = 0;	Int_t nMuRRR = 0;	Int_t nMuCCC = 0;	Int_t nMuGGG = 0;	Int_t nMuMMM = 0; 
   Int_t nMuNND = 0;     Int_t nMuDDR = 0;      Int_t nMuRRC = 0;       Int_t nMuCCG = 0;       Int_t nMuGGM = 0;
   Int_t nMuNNR = 0;     Int_t nMuDDC = 0;      Int_t nMuRRG = 0;       Int_t nMuCCM = 0;       Int_t nMuGMM = 0;
   Int_t nMuNNC = 0;     Int_t nMuDDG = 0;      Int_t nMuRRM = 0;       Int_t nMuCGG = 0;
   Int_t nMuNNG = 0;     Int_t nMuDDM = 0;      Int_t nMuRCC = 0;       Int_t nMuCGM = 0;
   Int_t nMuNNM = 0;     Int_t nMuDRR = 0;      Int_t nMuRCG = 0;       Int_t nMuCMM = 0;
   Int_t nMuNDD = 0;     Int_t nMuDRC = 0;      Int_t nMuRCM = 0;
   Int_t nMuNDR = 0;     Int_t nMuDRG = 0;      Int_t nMuRGG = 0;
   Int_t nMuNDC = 0;     Int_t nMuDRM = 0;      Int_t nMuRGM = 0;
   Int_t nMuNDG = 0;     Int_t nMuDCC = 0;      Int_t nMuRMM = 0;
   Int_t nMuNDM = 0;     Int_t nMuDCG = 0;      
   Int_t nMuNRR = 0;     Int_t nMuDCM = 0;      
   Int_t nMuNRC = 0;     Int_t nMuDGG = 0;      
   Int_t nMuNRG = 0;     Int_t nMuDGM = 0;      
   Int_t nMuNRM = 0;     Int_t nMuDMM = 0;      
   Int_t nMuNCC = 0;
   Int_t nMuNCG = 0;
   Int_t nMuNCM = 0;
   Int_t nMuNGG = 0;
   Int_t nMuNGM = 0;
   Int_t nMuNMM = 0;

   Int_t nMuN = 0; 

   Int_t nMuNNN_total = 0;	Int_t nMuDDD_total = 0;		Int_t nMuRRR_total = 0;	
   Int_t nMuNND_total = 0;     	Int_t nMuDDR_total = 0;      	Int_t nMuRRC_total = 0;       
   Int_t nMuNNR_total = 0;     	Int_t nMuDDC_total = 0;      	Int_t nMuRRG_total = 0;       
   Int_t nMuNNC_total = 0;     	Int_t nMuDDG_total = 0;      	Int_t nMuRRM_total = 0;       
   Int_t nMuNNG_total = 0;     	Int_t nMuDDM_total = 0;      	Int_t nMuRCC_total = 0;       
   Int_t nMuNNM_total = 0;     	Int_t nMuDRR_total = 0;      	Int_t nMuRCG_total = 0;       
   Int_t nMuNDD_total = 0;     	Int_t nMuDRC_total = 0;      	Int_t nMuRCM_total = 0;
   Int_t nMuNDR_total = 0;     	Int_t nMuDRG_total = 0;      	Int_t nMuRGG_total = 0;
   Int_t nMuNDC_total = 0;     	Int_t nMuDRM_total = 0;      	Int_t nMuRGM_total = 0;
   Int_t nMuNDG_total = 0;     	Int_t nMuDCC_total = 0;      	Int_t nMuRMM_total = 0;
   Int_t nMuNDM_total = 0;     	Int_t nMuDCG_total = 0;      
   Int_t nMuNRR_total = 0;     	Int_t nMuDCM_total = 0;      
   Int_t nMuNRC_total = 0;     	Int_t nMuDGG_total = 0;      
   Int_t nMuNRG_total = 0;     	Int_t nMuDGM_total = 0;      
   Int_t nMuNRM_total = 0;     	Int_t nMuDMM_total = 0;      
   Int_t nMuNCC_total = 0;
   Int_t nMuNCG_total = 0;
   Int_t nMuNCM_total = 0;
   Int_t nMuNGG_total = 0;
   Int_t nMuNGM_total = 0;
   Int_t nMuNMM_total = 0;

   Int_t nMuCCC_total = 0;	Int_t nMuGGG_total = 0;		Int_t nMuMMM_total = 0; 
   Int_t nMuCCG_total = 0;      Int_t nMuGGM_total = 0;
   Int_t nMuCCM_total = 0;      Int_t nMuGMM_total = 0;
   Int_t nMuCGG_total = 0;
   Int_t nMuCGM_total = 0;
   Int_t nMuCMM_total = 0;

   Int_t nMuN_total = 0;

   Bool_t isMuRRR = 0;  Bool_t isMuDDD = 0; 
   Bool_t isMuRRC = 0;  Bool_t isMuDDR = 0;
   Bool_t isMuRRG = 0;  Bool_t isMuDDC = 0;
   Bool_t isMuRRM = 0;  Bool_t isMuDDG = 0;
   Bool_t isMuRCC = 0;  Bool_t isMuDDM = 0;
   Bool_t isMuRCG = 0;  Bool_t isMuDRR = 0;
   Bool_t isMuRCM = 0;  Bool_t isMuDRC = 0;
   Bool_t isMuRGG = 0;  Bool_t isMuDRG = 0;
   Bool_t isMuRGM = 0;  Bool_t isMuDRM = 0;
   Bool_t isMuRMM = 0;  Bool_t isMuDCC = 0;
   Bool_t isMuCCC = 0;  Bool_t isMuDCG = 0;
   Bool_t isMuCCG = 0;  Bool_t isMuDCM = 0;
   Bool_t isMuCCM = 0;  Bool_t isMuDGG = 0;
   Bool_t isMuCGG = 0;  Bool_t isMuDGM = 0;
   Bool_t isMuCGM = 0;  Bool_t isMuDMM = 0;
   Bool_t isMuCMM = 0;
   Bool_t isMuGGG = 0;
   Bool_t isMuGGM = 0;
   Bool_t isMuGMM = 0;
   Bool_t isMuMMM = 0;

   Float_t muEta[3] = {0, 0, 0};
   Float_t muPt[3]  = {0, 0, 0};   

   float DDDminEta = 0;  float DDRminEta = 0;  float DDCminEta = 0;  float DDGminEta = 0; 
   float DDDmaxEta = 0;  float DDRmaxEta = 0;  float DDCmaxEta = 0;  float DDGmaxEta = 0;
   float DDDminPt  = 0;  float DDRminPt  = 0;  float DDCminPt  = 0;  float DDGminPt  = 0;
   float DDDmaxPt  = 0;  float DDRmaxPt  = 0;  float DDCmaxPt  = 0;  float DDGmaxPt  = 0;

   float DDMminEta = 0;  float DRRminEta = 0;  float DRCminEta = 0;  float DRGminEta = 0; 
   float DDMmaxEta = 0;  float DRRmaxEta = 0;  float DRCmaxEta = 0;  float DRGmaxEta = 0;
   float DDMminPt  = 0;  float DRRminPt  = 0;  float DRCminPt  = 0;  float DRGminPt  = 0;
   float DDMmaxPt  = 0;  float DRRmaxPt  = 0;  float DRCmaxPt  = 0;  float DRGmaxPt  = 0;

   float DRMminEta = 0;  float DCCminEta = 0;  float DCGminEta = 0;  float DCMminEta = 0; 
   float DRMmaxEta = 0;  float DCCmaxEta = 0;  float DCGmaxEta = 0;  float DCMmaxEta = 0;
   float DRMminPt  = 0;  float DCCminPt  = 0;  float DCGminPt  = 0;  float DCMminPt  = 0;
   float DRMmaxPt  = 0;  float DCCmaxPt  = 0;  float DCGmaxPt  = 0;  float DCMmaxPt  = 0;

   float DGGminEta = 0;  float DGMminEta = 0;  float DMMminEta = 0;   
   float DGGmaxEta = 0;  float DGMmaxEta = 0;  float DMMmaxEta = 0;  
   float DGGminPt  = 0;  float DGMminPt  = 0;  float DMMminPt  = 0;  
   float DGGmaxPt  = 0;  float DGMmaxPt  = 0;  float DMMmaxPt  = 0;  

   float RRRminEta = 0;  float RRCminEta = 0;  float RRGminEta = 0;  float RMMminEta = 0; 
   float RRRmaxEta = 0;  float RRCmaxEta = 0;  float RRGmaxEta = 0;  float RMMmaxEta = 0;
   float RRRminPt  = 0;  float RRCminPt  = 0;  float RRGminPt  = 0;  float RMMminPt  = 0;
   float RRRmaxPt  = 0;  float RRCmaxPt  = 0;  float RRGmaxPt  = 0;  float RMMmaxPt  = 0;

   float RRMminEta = 0;  float RCCminEta = 0;  float RCGminEta = 0;  float RCMminEta = 0; 
   float RRMmaxEta = 0;  float RCCmaxEta = 0;  float RCGmaxEta = 0;  float RCMmaxEta = 0;
   float RRMminPt  = 0;  float RCCminPt  = 0;  float RCGminPt  = 0;  float RCMminPt  = 0;
   float RRMmaxPt  = 0;  float RCCmaxPt  = 0;  float RCGmaxPt  = 0;  float RCMmaxPt  = 0;

   float RGGminEta = 0;  float RGMminEta = 0;  float CCCminEta = 0;  float CCGminEta = 0; 
   float RGGmaxEta = 0;  float RGMmaxEta = 0;  float CCCmaxEta = 0;  float CCGmaxEta = 0;
   float RGGminPt  = 0;  float RGMminPt  = 0;  float CCCminPt  = 0;  float CCGminPt  = 0;
   float RGGmaxPt  = 0;  float RGMmaxPt  = 0;  float CCCmaxPt  = 0;  float CCGmaxPt  = 0;

   float CCMminEta = 0;  float CGGminEta = 0;  float CGMminEta = 0;  float CMMminEta = 0; 
   float CCMmaxEta = 0;  float CGGmaxEta = 0;  float CGMmaxEta = 0;  float CMMmaxEta = 0;
   float CCMminPt  = 0;  float CGGminPt  = 0;  float CGMminPt  = 0;  float CMMminPt  = 0;
   float CCMmaxPt  = 0;  float CGGmaxPt  = 0;  float CGMmaxPt  = 0;  float CMMmaxPt  = 0;

   float GGGminEta = 0;  float GGMminEta = 0;  float GMMminEta = 0;  float MMMminEta = 0; 
   float GGGmaxEta = 0;  float GGMmaxEta = 0;  float GMMmaxEta = 0;  float MMMmaxEta = 0;
   float GGGminPt  = 0;  float GGMminPt  = 0;  float GMMminPt  = 0;  float MMMminPt  = 0;
   float GGGmaxPt  = 0;  float GGMmaxPt  = 0;  float GMMmaxPt  = 0;  float MMMmaxPt  = 0;

   float RRRrate = -1;   float DDDrate = -1; 
   float RRCrate = -1;   float DDRrate = -1;
   float RRGrate = -1;   float DDCrate = -1;
   float RRMrate = -1;   float DDGrate = -1;
   float RCCrate = -1;   float DDMrate = -1;
   float RCGrate = -1;   float DRRrate = -1;
   float RCMrate = -1;   float DRCrate = -1;
   float RGGrate = -1;   float DRGrate = -1;
   float RGMrate = -1;   float DRMrate = -1;
   float RMMrate = -1;   float DCCrate = -1;
   float CCCrate = -1;   float DCGrate = -1;
   float CCGrate = -1;   float DCMrate = -1;
   float CCMrate = -1;   float DGGrate = -1;
   float CGGrate = -1;   float DGMrate = -1;
   float CGMrate = -1;   float DMMrate = -1;
   float CMMrate = -1;
   float GGGrate = -1;
   float GGMrate = -1;
   float GMMrate = -1;
   float MMMrate = -1;
   

   //-------------------EVENT-----------------------------------
   tree_event-> SetBranchAddress("isMuDDD", &isMuDDD);
   tree_event-> SetBranchAddress("isMuDDR", &isMuDDR);
   tree_event-> SetBranchAddress("isMuDDC", &isMuDDC);
   tree_event-> SetBranchAddress("isMuDDG", &isMuDDG);
   tree_event-> SetBranchAddress("isMuDDM", &isMuDDM);
   tree_event-> SetBranchAddress("isMuDRR", &isMuDRR);
   tree_event-> SetBranchAddress("isMuDRC", &isMuDRC);
   tree_event-> SetBranchAddress("isMuDRG", &isMuDRG);
   tree_event-> SetBranchAddress("isMuDRM", &isMuDRM);
   tree_event-> SetBranchAddress("isMuDCC", &isMuDCC);
   tree_event-> SetBranchAddress("isMuDCG", &isMuDCG);
   tree_event-> SetBranchAddress("isMuDCM", &isMuDCM);
   tree_event-> SetBranchAddress("isMuDGG", &isMuDGG);
   tree_event-> SetBranchAddress("isMuDGM", &isMuDGM);
   tree_event-> SetBranchAddress("isMuDMM", &isMuDMM);
   
   tree_event-> SetBranchAddress("isMuRRR", &isMuRRR);
   tree_event-> SetBranchAddress("isMuRRC", &isMuRRC);
   tree_event-> SetBranchAddress("isMuRRG", &isMuRRG);
   tree_event-> SetBranchAddress("isMuRRM", &isMuRRM);
   tree_event-> SetBranchAddress("isMuRCC", &isMuRCC);
   tree_event-> SetBranchAddress("isMuRCG", &isMuRCG);
   tree_event-> SetBranchAddress("isMuRCM", &isMuRCM);
   tree_event-> SetBranchAddress("isMuRGG", &isMuRGG);
   tree_event-> SetBranchAddress("isMuRGM", &isMuRGM);
   tree_event-> SetBranchAddress("isMuRMM", &isMuRMM);
   tree_event-> SetBranchAddress("isMuCCC", &isMuCCC);
   tree_event-> SetBranchAddress("isMuCCG", &isMuCCG);
   tree_event-> SetBranchAddress("isMuCCM", &isMuCCM);
   tree_event-> SetBranchAddress("isMuCGG", &isMuCGG);
   tree_event-> SetBranchAddress("isMuCGM", &isMuCGM);
   tree_event-> SetBranchAddress("isMuCMM", &isMuCMM);
   tree_event-> SetBranchAddress("isMuGGG", &isMuGGG);
   tree_event-> SetBranchAddress("isMuGGM", &isMuGGM);
   tree_event-> SetBranchAddress("isMuGMM", &isMuGMM);
   tree_event-> SetBranchAddress("isMuMMM", &isMuMMM);
   
   tree_event-> SetBranchAddress("muEta", &muEta[0]);
   tree_event-> SetBranchAddress("muPt", &muPt[0]);




   //-----------------EVENT SUMMARY------------------------------ 
   //tree_event_summary->Print();
 
   tree_event_summary->SetBranchAddress("lastEvent",&lastEvent);

   tree_event_summary->SetBranchAddress("nMuNNN",&nMuNNN);
   tree_event_summary->SetBranchAddress("nMuNND",&nMuNND);
   tree_event_summary->SetBranchAddress("nMuNNR",&nMuNNR);
   tree_event_summary->SetBranchAddress("nMuNNC",&nMuNNC);
   tree_event_summary->SetBranchAddress("nMuNNG",&nMuNNG);
   tree_event_summary->SetBranchAddress("nMuNNM",&nMuNNM);
   tree_event_summary->SetBranchAddress("nMuNDD",&nMuNDD);
   tree_event_summary->SetBranchAddress("nMuNDR",&nMuNDR);
   tree_event_summary->SetBranchAddress("nMuNDC",&nMuNDC);
   tree_event_summary->SetBranchAddress("nMuNDG",&nMuNDG);
   tree_event_summary->SetBranchAddress("nMuNDM",&nMuNDM);
   tree_event_summary->SetBranchAddress("nMuNRR",&nMuNRR);
   tree_event_summary->SetBranchAddress("nMuNRC",&nMuNRC);
   tree_event_summary->SetBranchAddress("nMuNRG",&nMuNRG);
   tree_event_summary->SetBranchAddress("nMuNRM",&nMuNRM);
   tree_event_summary->SetBranchAddress("nMuNCC",&nMuNCC);
   tree_event_summary->SetBranchAddress("nMuNCG",&nMuNCG);
   tree_event_summary->SetBranchAddress("nMuNCM",&nMuNCM);
   tree_event_summary->SetBranchAddress("nMuNGG",&nMuNGG);
   tree_event_summary->SetBranchAddress("nMuNGM",&nMuNGM);
   tree_event_summary->SetBranchAddress("nMuNMM",&nMuNMM);

   tree_event_summary->SetBranchAddress("nMuDDD",&nMuDDD);
   tree_event_summary->SetBranchAddress("nMuDDR",&nMuDDR);
   tree_event_summary->SetBranchAddress("nMuDDC",&nMuDDC);
   tree_event_summary->SetBranchAddress("nMuDDG",&nMuDDG);
   tree_event_summary->SetBranchAddress("nMuDDM",&nMuDDM);
   tree_event_summary->SetBranchAddress("nMuDRR",&nMuDRR);
   tree_event_summary->SetBranchAddress("nMuDRC",&nMuDRC);
   tree_event_summary->SetBranchAddress("nMuDRG",&nMuDRG);
   tree_event_summary->SetBranchAddress("nMuDRM",&nMuDRM);
   tree_event_summary->SetBranchAddress("nMuDCC",&nMuDCC);
   tree_event_summary->SetBranchAddress("nMuDCG",&nMuDCG);
   tree_event_summary->SetBranchAddress("nMuDCM",&nMuDCM);
   tree_event_summary->SetBranchAddress("nMuDGG",&nMuDGG);
   tree_event_summary->SetBranchAddress("nMuDGM",&nMuDGM);
   tree_event_summary->SetBranchAddress("nMuDMM",&nMuDMM);

   tree_event_summary->SetBranchAddress("nMuRRR",&nMuRRR);
   tree_event_summary->SetBranchAddress("nMuRRC",&nMuRRC);
   tree_event_summary->SetBranchAddress("nMuRRG",&nMuRRG);
   tree_event_summary->SetBranchAddress("nMuRRM",&nMuRRM);
   tree_event_summary->SetBranchAddress("nMuRCC",&nMuRCC);
   tree_event_summary->SetBranchAddress("nMuRCG",&nMuRCG);
   tree_event_summary->SetBranchAddress("nMuRCM",&nMuRCM);
   tree_event_summary->SetBranchAddress("nMuRGG",&nMuRGG);
   tree_event_summary->SetBranchAddress("nMuRGM",&nMuRGM);
   tree_event_summary->SetBranchAddress("nMuRMM",&nMuRMM);

   tree_event_summary->SetBranchAddress("nMuCCC",&nMuCCC);
   tree_event_summary->SetBranchAddress("nMuCCG",&nMuCCG);
   tree_event_summary->SetBranchAddress("nMuCCM",&nMuCCM);
   tree_event_summary->SetBranchAddress("nMuCGG",&nMuCGG);
   tree_event_summary->SetBranchAddress("nMuCGM",&nMuCGM);
   tree_event_summary->SetBranchAddress("nMuCMM",&nMuCMM);

   tree_event_summary->SetBranchAddress("nMuGGG",&nMuGGG);
   tree_event_summary->SetBranchAddress("nMuGGM",&nMuGGM);
   tree_event_summary->SetBranchAddress("nMuGMM",&nMuGMM);

   tree_event_summary->SetBranchAddress("nMuMMM",&nMuMMM);
   
   tree_event_summary->SetBranchAddress("nMuN",&nMuN);

   //Branches of trees produced by this macro 
   tree_DDD -> Branch( "DDDminEta", &DDDminEta );
   tree_DDD -> Branch( "DDDmaxEta", &DDDmaxEta );
   tree_DDD -> Branch( "DDDminPt" , &DDDminPt  );
   tree_DDD -> Branch( "DDDmaxPt" , &DDDmaxPt  );
                                        
   tree_DDR -> Branch( "DDRminEta", &DDRminEta );
   tree_DDR -> Branch( "DDRmaxEta", &DDRmaxEta );
   tree_DDR -> Branch( "DDRminPt" , &DDRminPt  );
   tree_DDR -> Branch( "DDRmaxPt" , &DDRmaxPt  );
                                        
   tree_DDC -> Branch( "DDCminEta", &DDCminEta );
   tree_DDC -> Branch( "DDCmaxEta", &DDCmaxEta );
   tree_DDC -> Branch( "DDCminPt" , &DDCminPt  );
   tree_DDC -> Branch( "DDCmaxPt" , &DDCmaxPt  );
                                        
   tree_DDG -> Branch( "DDGminEta", &DDGminEta );
   tree_DDG -> Branch( "DDGmaxEta", &DDGmaxEta );
   tree_DDG -> Branch( "DDGminPt" , &DDGminPt  );
   tree_DDG -> Branch( "DDGmaxPt" , &DDGmaxPt  );
                                        
   tree_DDM -> Branch( "DDMminEta", &DDMminEta );
   tree_DDM -> Branch( "DDMmaxEta", &DDMmaxEta );
   tree_DDM -> Branch( "DDMminPt" , &DDMminPt  );
   tree_DDM -> Branch( "DDMmaxPt" , &DDMmaxPt  );
                                        
   tree_DRR -> Branch( "DRRminEta", &DRRminEta );
   tree_DRR -> Branch( "DRRmaxEta", &DRRmaxEta );
   tree_DRR -> Branch( "DRRminPt" , &DRRminPt  );
   tree_DRR -> Branch( "DRRmaxPt" , &DRRmaxPt  );
                                        
   tree_DRC -> Branch( "DRCminEta", &DRCminEta );
   tree_DRC -> Branch( "DRCmaxEta", &DRCmaxEta );
   tree_DRC -> Branch( "DRCminPt" , &DRCminPt  );
   tree_DRC -> Branch( "DRCmaxPt" , &DRCmaxPt  );
                                        
   tree_DRG -> Branch( "DRGminEta", &DRGminEta );
   tree_DRG -> Branch( "DRGmaxEta", &DRGmaxEta );
   tree_DRG -> Branch( "DRGminPt" , &DRGminPt  );
   tree_DRG -> Branch( "DRGmaxPt" , &DRGmaxPt  );
                                        
   tree_DRM -> Branch( "DRMminEta", &DRMminEta );
   tree_DRM -> Branch( "DRMmaxEta", &DRMmaxEta );
   tree_DRM -> Branch( "DRMminPt" , &DRMminPt  );
   tree_DRM -> Branch( "DRMmaxPt" , &DRMmaxPt  );
                                        
   tree_DCC -> Branch( "DCCminEta", &DCCminEta );
   tree_DCC -> Branch( "DCCmaxEta", &DCCmaxEta );
   tree_DCC -> Branch( "DCCminPt" , &DCCminPt  );
   tree_DCC -> Branch( "DCCmaxPt" , &DCCmaxPt  );
                                        
   tree_DCG -> Branch( "DCGminEta", &DCGminEta );
   tree_DCG -> Branch( "DCGmaxEta", &DCGmaxEta );
   tree_DCG -> Branch( "DCGminPt" , &DCGminPt  );
   tree_DCG -> Branch( "DCGmaxPt" , &DCGmaxPt  );
                                        
   tree_DCM -> Branch( "DCMminEta", &DCMminEta );
   tree_DCM -> Branch( "DCMmaxEta", &DCMmaxEta );
   tree_DCM -> Branch( "DCMminPt" , &DCMminPt  );
   tree_DCM -> Branch( "DCMmaxPt" , &DCMmaxPt  );
                                        
   tree_DGG -> Branch( "DGGminEta", &DGGminEta );
   tree_DGG -> Branch( "DGGmaxEta", &DGGmaxEta );
   tree_DGG -> Branch( "DGGminPt" , &DGGminPt  );
   tree_DGG -> Branch( "DGGmaxPt" , &DGGmaxPt  );
                                        
   tree_DGM -> Branch( "DGMminEta", &DGMminEta );
   tree_DGM -> Branch( "DGMmaxEta", &DGMmaxEta );
   tree_DGM -> Branch( "DGMminPt" , &DGMminPt  );
   tree_DGM -> Branch( "DGMmaxPt" , &DGMmaxPt  );
                                        
   tree_DMM -> Branch( "DMMminEta", &DMMminEta );
   tree_DMM -> Branch( "DMMmaxEta", &DMMmaxEta );
   tree_DMM -> Branch( "DMMminPt" , &DMMminPt  );
   tree_DMM -> Branch( "DMMmaxPt" , &DMMmaxPt  );

   tree_RRR -> Branch( "RRRminEta", &RRRminEta );
   tree_RRR -> Branch( "RRRmaxEta", &RRRmaxEta );
   tree_RRR -> Branch( "RRRminPt" , &RRRminPt  );
   tree_RRR -> Branch( "RRRmaxPt" , &RRRmaxPt  );

   tree_RRC -> Branch( "RRCminEta", &RRCminEta );
   tree_RRC -> Branch( "RRCmaxEta", &RRCmaxEta );
   tree_RRC -> Branch( "RRCminPt" , &RRCminPt  );
   tree_RRC -> Branch( "RRCmaxPt" , &RRCmaxPt  );

   tree_RRG -> Branch( "RRGminEta", &RRGminEta );
   tree_RRG -> Branch( "RRGmaxEta", &RRGmaxEta );
   tree_RRG -> Branch( "RRGminPt" , &RRGminPt  );
   tree_RRG -> Branch( "RRGmaxPt" , &RRGmaxPt  );

   tree_RRM -> Branch( "RRMminEta", &RRMminEta );
   tree_RRM -> Branch( "RRMmaxEta", &RRMmaxEta );
   tree_RRM -> Branch( "RRMminPt" , &RRMminPt  );
   tree_RRM -> Branch( "RRMmaxPt" , &RRMmaxPt  );

   tree_RCC -> Branch( "RCCminEta", &RCCminEta );
   tree_RCC -> Branch( "RCCmaxEta", &RCCmaxEta );
   tree_RCC -> Branch( "RCCminPt" , &RCCminPt  );
   tree_RCC -> Branch( "RCCmaxPt" , &RCCmaxPt  );

   tree_RCG -> Branch( "RCGminEta", &RCGminEta );
   tree_RCG -> Branch( "RCGmaxEta", &RCGmaxEta );
   tree_RCG -> Branch( "RCGminPt" , &RCGminPt  );
   tree_RCG -> Branch( "RCGmaxPt" , &RCGmaxPt  );

   tree_RCM -> Branch( "RCMminEta", &RCMminEta );
   tree_RCM -> Branch( "RCMmaxEta", &RCMmaxEta );
   tree_RCM -> Branch( "RCMminPt" , &RCMminPt  );
   tree_RCM -> Branch( "RCMmaxPt" , &RCMmaxPt  );

   tree_RGG -> Branch( "RGGminEta", &RGGminEta );
   tree_RGG -> Branch( "RGGmaxEta", &RGGmaxEta );
   tree_RGG -> Branch( "RGGminPt" , &RGGminPt  );
   tree_RGG -> Branch( "RGGmaxPt" , &RGGmaxPt  );

   tree_RGM -> Branch( "RGMminEta", &RGMminEta );
   tree_RGM -> Branch( "RGMmaxEta", &RGMmaxEta );
   tree_RGM -> Branch( "RGMminPt" , &RGMminPt  );
   tree_RGM -> Branch( "RGMmaxPt" , &RGMmaxPt  );

   tree_RMM -> Branch( "RMMminEta", &RMMminEta );
   tree_RMM -> Branch( "RMMmaxEta", &RMMmaxEta );
   tree_RMM -> Branch( "RMMminPt" , &RMMminPt  );
   tree_RMM -> Branch( "RMMmaxPt" , &RMMmaxPt  );

   tree_CCC -> Branch( "CCCminEta", &CCCminEta );
   tree_CCC -> Branch( "CCCmaxEta", &CCCmaxEta );
   tree_CCC -> Branch( "CCCminPt" , &CCCminPt  );
   tree_CCC -> Branch( "CCCmaxPt" , &CCCmaxPt  );

   tree_CCG -> Branch( "CCGminEta", &CCGminEta );   
   tree_CCG -> Branch( "CCGmaxEta", &CCGmaxEta );
   tree_CCG -> Branch( "CCGminPt" , &CCGminPt  );   
   tree_CCG -> Branch( "CCGmaxPt" , &CCGmaxPt  );

   tree_CCM -> Branch( "CCMminEta", &CCMminEta );
   tree_CCM -> Branch( "CCMmaxEta", &CCMmaxEta );
   tree_CCM -> Branch( "CCMminPt" , &CCMminPt  );
   tree_CCM -> Branch( "CCMmaxPt" , &CCMmaxPt  );

   tree_CGG -> Branch( "CGGminEta", &CGGminEta );
   tree_CGG -> Branch( "CGGmaxEta", &CGGmaxEta );
   tree_CGG -> Branch( "CGGminPt" , &CGGminPt  );
   tree_CGG -> Branch( "CGGmaxPt" , &CGGmaxPt  );

   tree_CGM -> Branch( "CGMminEta", &CGMminEta );
   tree_CGM -> Branch( "CGMmaxEta", &CGMmaxEta );
   tree_CGM -> Branch( "CGMminPt" , &CGMminPt  );
   tree_CGM -> Branch( "CGMmaxPt" , &CGMmaxPt  );

   tree_CMM -> Branch( "CMMminEta", &CMMminEta );
   tree_CMM -> Branch( "CMMmaxEta", &CMMmaxEta );
   tree_CMM -> Branch( "CMMminPt" , &CMMminPt  );
   tree_CMM -> Branch( "CMMmaxPt" , &CMMmaxPt  );

   tree_GGG -> Branch( "GGGminEta", &GGGminEta );
   tree_GGG -> Branch( "GGGmaxEta", &GGGmaxEta );
   tree_GGG -> Branch( "GGGminPt" , &GGGminPt  );
   tree_GGG -> Branch( "GGGmaxPt" , &GGGmaxPt  );

   tree_GGM -> Branch( "GGMminEta", &GGMminEta );
   tree_GGM -> Branch( "GGMmaxEta", &GGMmaxEta );
   tree_GGM -> Branch( "GGMminPt" , &GGMminPt  );
   tree_GGM -> Branch( "GGMmaxPt" , &GGMmaxPt  );

   tree_GMM -> Branch( "GMMminEta", &GMMminEta );
   tree_GMM -> Branch( "GMMmaxEta", &GMMmaxEta );
   tree_GMM -> Branch( "GMMminPt" , &GMMminPt  );
   tree_GMM -> Branch( "GMMmaxPt" , &GMMmaxPt  );

   tree_MMM -> Branch( "MMMminEta", &MMMminEta );
   tree_MMM -> Branch( "MMMmaxEta", &MMMmaxEta );
   tree_MMM -> Branch( "MMMminPt" , &MMMminPt  );
   tree_MMM -> Branch( "MMMmaxPt" , &MMMmaxPt  );


   tree_summary -> Branch("lastEvent_total", &lastEvent_total);


   tree_summary -> Branch("nMu_totalDDD", &nMuDDD_total);
   tree_summary -> Branch("nMu_totalDDR", &nMuDDR_total);
   tree_summary -> Branch("nMu_totalDDC", &nMuDDC_total);
   tree_summary -> Branch("nMu_totalDDG", &nMuDDG_total);
   tree_summary -> Branch("nMu_totalDDM", &nMuDDM_total);
   tree_summary -> Branch("nMu_totalDRR", &nMuDRR_total);
   tree_summary -> Branch("nMu_totalDRC", &nMuDRC_total);
   tree_summary -> Branch("nMu_totalDRG", &nMuDRG_total);
   tree_summary -> Branch("nMu_totalDRM", &nMuDRM_total);
   tree_summary -> Branch("nMu_totalDCC", &nMuDCC_total);
   tree_summary -> Branch("nMu_totalDCG", &nMuDCG_total);
   tree_summary -> Branch("nMu_totalDCM", &nMuDCM_total);
   tree_summary -> Branch("nMu_totalDGG", &nMuDGG_total);
   tree_summary -> Branch("nMu_totalDGM", &nMuDGM_total);
   tree_summary -> Branch("nMu_totalDMM", &nMuDMM_total);

   tree_summary -> Branch("nMuRRR_total", &nMuRRR_total);
   tree_summary -> Branch("nMuRRC_total", &nMuRRC_total);
   tree_summary -> Branch("nMuRRG_total", &nMuRRG_total);
   tree_summary -> Branch("nMuRRM_total", &nMuRRM_total);
   tree_summary -> Branch("nMuRCC_total", &nMuRCC_total);
   tree_summary -> Branch("nMuRCG_total", &nMuRCG_total);
   tree_summary -> Branch("nMuRCM_total", &nMuRCM_total);
   tree_summary -> Branch("nMuRGG_total", &nMuRGG_total);
   tree_summary -> Branch("nMuRGM_total", &nMuRGM_total);
   tree_summary -> Branch("nMuRMM_total", &nMuRMM_total);
   tree_summary -> Branch("nMuCCC_total", &nMuCCC_total);
   tree_summary -> Branch("nMuCCG_total", &nMuCCG_total);
   tree_summary -> Branch("nMuCCM_total", &nMuCCM_total);
   tree_summary -> Branch("nMuCGG_total", &nMuCGG_total);
   tree_summary -> Branch("nMuCGM_total", &nMuCGM_total);
   tree_summary -> Branch("nMuCMM_total", &nMuCMM_total);
   tree_summary -> Branch("nMuGGG_total", &nMuGGG_total);
   tree_summary -> Branch("nMuGGM_total", &nMuGGM_total);
   tree_summary -> Branch("nMuGMM_total", &nMuGMM_total);
   tree_summary -> Branch("nMuMMM_total", &nMuMMM_total);


   tree_summary -> Branch("DDDrate", &DDDrate);
   tree_summary -> Branch("DDRrate", &DDRrate);
   tree_summary -> Branch("DDCrate", &DDCrate);
   tree_summary -> Branch("DDGrate", &DDGrate);
   tree_summary -> Branch("DDMrate", &DDMrate);
   tree_summary -> Branch("DRRrate", &DRRrate);
   tree_summary -> Branch("DRCrate", &DRCrate);
   tree_summary -> Branch("DRGrate", &DRGrate);
   tree_summary -> Branch("DRMrate", &DRMrate);
   tree_summary -> Branch("DCCrate", &DCCrate);
   tree_summary -> Branch("DCGrate", &DCGrate);
   tree_summary -> Branch("DCMrate", &DCMrate);
   tree_summary -> Branch("DGGrate", &DGGrate);
   tree_summary -> Branch("DGMrate", &DGMrate);
   tree_summary -> Branch("DMMrate", &DMMrate);

   tree_summary -> Branch("RRRrate", &RRRrate);
   tree_summary -> Branch("RRCrate", &RRCrate);
   tree_summary -> Branch("RRGrate", &RRGrate);
   tree_summary -> Branch("RRMrate", &RRMrate);
   tree_summary -> Branch("RCCrate", &RCCrate);
   tree_summary -> Branch("RCGrate", &RCGrate);
   tree_summary -> Branch("RCMrate", &RCMrate);
   tree_summary -> Branch("RGGrate", &RGGrate);
   tree_summary -> Branch("RGMrate", &RGMrate);
   tree_summary -> Branch("RMMrate", &RMMrate);
   tree_summary -> Branch("CCCrate", &CCCrate);
   tree_summary -> Branch("CCGrate", &CCGrate);
   tree_summary -> Branch("CCMrate", &CCMrate);
   tree_summary -> Branch("CGGrate", &CGGrate);
   tree_summary -> Branch("CGMrate", &CGMrate);
   tree_summary -> Branch("CMMrate", &CMMrate);
   tree_summary -> Branch("GGGrate", &GGGrate);
   tree_summary -> Branch("GGMrate", &GGMrate);
   tree_summary -> Branch("GMMrate", &GMMrate);
   tree_summary -> Branch("MMMrate", &MMMrate);
   

   

   //------------------------LET'S GO -------------------------------



   Int_t summaryEntries = (Int_t)tree_event_summary -> GetEntries();
   Int_t eventEntries = (Int_t)tree_event -> GetEntries();
  
   for ( Int_t i=0; i < summaryEntries; i++ ) 
   {
      tree_event_summary->GetEntry(i);
      //cout << i << "lastEvent:"<< lastEvent << endl;
      lastEvent_total = lastEvent_total + lastEvent;

      nMuNNN_total = nMuNNN_total + nMuNNN;	nMuDDD_total = nMuDDD_total + nMuDDD;	nMuRRR_total = nMuRRR_total + nMuRRR;
      nMuNND_total = nMuNND_total + nMuNND;     nMuDDR_total = nMuDDR_total + nMuDDR;   nMuRRC_total = nMuRRC_total + nMuRRC;
      nMuNNR_total = nMuNNR_total + nMuNNR;     nMuDDC_total = nMuDDC_total + nMuDDC;   nMuRRG_total = nMuRRG_total + nMuRRG;
      nMuNNC_total = nMuNNC_total + nMuNNC;     nMuDDG_total = nMuDDG_total + nMuDDG;   nMuRRM_total = nMuRRM_total + nMuRRM;
      nMuNNG_total = nMuNNG_total + nMuNNG;     nMuDDM_total = nMuDDM_total + nMuDDM;   nMuRCC_total = nMuRCC_total + nMuRCC;
      nMuNNM_total = nMuNNM_total + nMuNNM;     nMuDRR_total = nMuDRR_total + nMuDRR;   nMuRCG_total = nMuRCG_total + nMuRCG;
      nMuNDD_total = nMuNDD_total + nMuNDD;     nMuDRC_total = nMuDRC_total + nMuDRC;   nMuRCM_total = nMuRCM_total + nMuRCM;
      nMuNDR_total = nMuNDR_total + nMuNDR;     nMuDRG_total = nMuDRG_total + nMuDRG;   nMuRGG_total = nMuRGG_total + nMuRGG;
      nMuNDC_total = nMuNDC_total + nMuNDC;     nMuDRM_total = nMuDRM_total + nMuDRM;   nMuRGM_total = nMuRGM_total + nMuRGM;
      nMuNDG_total = nMuNDG_total + nMuNDG;     nMuDCC_total = nMuDCC_total + nMuDCC;   nMuRMM_total = nMuRMM_total + nMuRMM;
      nMuNDM_total = nMuNDM_total + nMuNDM;     nMuDCG_total = nMuDCG_total + nMuDCG;
      nMuNRR_total = nMuNRR_total + nMuNRR;     nMuDCM_total = nMuDCM_total + nMuDCM;
      nMuNRC_total = nMuNRC_total + nMuNRC;     nMuDGG_total = nMuDGG_total + nMuDGG;
      nMuNRG_total = nMuNRG_total + nMuNRG;     nMuDGM_total = nMuDGM_total + nMuDGM;
      nMuNRM_total = nMuNRM_total + nMuNRM;     nMuDMM_total = nMuDMM_total + nMuDMM;
      nMuNCC_total = nMuNCC_total + nMuNCC;
      nMuNCG_total = nMuNCG_total + nMuNCG;
      nMuNCM_total = nMuNCM_total + nMuNCM;
      nMuNGG_total = nMuNGG_total + nMuNGG;
      nMuNGM_total = nMuNGM_total + nMuNGM;
      nMuNMM_total = nMuNMM_total + nMuNMM;

      nMuCCC_total = nMuCCC_total + nMuCCC;	nMuGGG_total = nMuGGG_total + nMuGGG;	nMuMMM_total = nMuMMM_total + nMuMMM;
      nMuCCG_total = nMuCCG_total + nMuCCG;	nMuGGM_total = nMuGGM_total + nMuGGM;
      nMuCCM_total = nMuCCM_total + nMuCCM;	nMuGMM_total = nMuGMM_total + nMuGMM;
      nMuCGG_total = nMuCGG_total + nMuCGG;
      nMuCGM_total = nMuCGM_total + nMuCGM;
      nMuCMM_total = nMuCMM_total + nMuCMM;

      nMuN_total = nMuN_total + nMuN;


   }

   cout << "lastEvent_total:" << lastEvent_total << endl;

   cout << "nMuNNN_total: " << nMuNNN_total << " nMuDDD_total: " << nMuDDD_total << "nMuRRR_total: " << nMuRRR_total << endl; 
   cout << "nMuNND_total: " << nMuNND_total << " nMuDDR_total: " << nMuDDR_total << "nMuRRC_total: " << nMuRRC_total << endl;      
   cout << "nMuNNR_total: " << nMuNNR_total << " nMuDDC_total: " << nMuDDC_total << "nMuRRG_total: " << nMuRRG_total << endl;       
   cout << "nMuNNC_total: " << nMuNNC_total << " nMuDDG_total: " << nMuDDG_total << "nMuRRM_total: " << nMuRRM_total << endl;       
   cout << "nMuNNG_total: " << nMuNNG_total << " nMuDDM_total: " << nMuDDM_total << "nMuRCC_total: " << nMuRCC_total << endl;       
   cout << "nMuNNM_total: " << nMuNNM_total << " nMuDRR_total: " << nMuDRR_total << "nMuRCG_total: " << nMuRCG_total << endl;       
   cout << "nMuNDD_total: " << nMuNDD_total << " nMuDRC_total: " << nMuDRC_total << "nMuRCM_total: " << nMuRCM_total << endl;
   cout << "nMuNDR_total: " << nMuNDR_total << " nMuDRG_total: " << nMuDRG_total << "nMuRGG_total: " << nMuRGG_total << endl;
   cout << "nMuNDC_total: " << nMuNDC_total << " nMuDRM_total: " << nMuDRM_total << "nMuRGM_total: " << nMuRGM_total << endl;
   cout << "nMuNDG_total: " << nMuNDG_total << " nMuDCC_total: " << nMuDCC_total << "nMuRMM_total: " << nMuRMM_total << endl;
   cout << "nMuNDM_total: " << nMuNDM_total << " nMuDCG_total: " << nMuDCG_total << endl; 
   cout << "nMuNRR_total: " << nMuNRR_total << " nMuDCM_total: " << nMuDCM_total << endl;                 
   cout << "nMuNRC_total: " << nMuNRC_total << " nMuDGG_total: " << nMuDGG_total << endl;                 
   cout << "nMuNRG_total: " << nMuNRG_total << " nMuDGM_total: " << nMuDGM_total << endl;                
   cout << "nMuNRM_total: " << nMuNRM_total << " nMuDMM_total: " << nMuDMM_total << endl;               
   cout << "nMuNCC_total: " << nMuNCC_total << endl;             
   cout << "nMuNCG_total: " << nMuNCG_total << endl;             
   cout << "nMuNCM_total: " << nMuNCM_total << endl;             
   cout << "nMuNGG_total: " << nMuNGG_total << endl;             
   cout << "nMuNGM_total: " << nMuNGM_total << endl;             
   cout << "nMuNMM_total: " << nMuNMM_total << endl;             

   cout << "nMuCCC_total: " << nMuCCC_total << " nMuGGG_total: " << nMuGGG_total << " nMuMMM_total: " << nMuMMM_total << endl;
   cout << "nMuCCG_total: " << nMuCCG_total << " nMuGGM_total: " << nMuGGM_total << endl;
   cout << "nMuCCM_total: " << nMuCCM_total << " nMuGMM_total: " << nMuGMM_total << endl;
   cout << "nMuCGG_total: " << nMuCGG_total << endl;
   cout << "nMuCGM_total: " << nMuCGM_total << endl;
   cout << "nMuCMM_total: " << nMuCMM_total << endl;

   cout << "nMuN_total:" << nMuN_total << endl;


   cout << "nMuNNN_total%: " << (Float_t)nMuNNN_total/lastEvent_total*100. << " nMuDDD_total%: " << (Float_t)nMuDDD_total/lastEvent_total*100. << " nMuRRR_total%: " << (Float_t)nMuRRR_total/lastEvent_total*100. << endl; 
   cout << "nMuNND_total%: " << (Float_t)nMuNND_total/lastEvent_total*100. << " nMuDDR_total%: " << (Float_t)nMuDDR_total/lastEvent_total*100. << " nMuRRC_total%: " << (Float_t)nMuRRC_total/lastEvent_total*100. << endl;      
   cout << "nMuNNR_total%: " << (Float_t)nMuNNR_total/lastEvent_total*100. << " nMuDDC_total%: " << (Float_t)nMuDDC_total/lastEvent_total*100. << " nMuRRG_total%: " << (Float_t)nMuRRG_total/lastEvent_total*100. << endl;       
   cout << "nMuNNC_total%: " << (Float_t)nMuNNC_total/lastEvent_total*100. << " nMuDDG_total%: " << (Float_t)nMuDDG_total/lastEvent_total*100. << " nMuRRM_total%: " << (Float_t)nMuRRM_total/lastEvent_total*100. << endl;       
   cout << "nMuNNG_total%: " << (Float_t)nMuNNG_total/lastEvent_total*100. << " nMuDDM_total%: " << (Float_t)nMuDDM_total/lastEvent_total*100. << " nMuRCC_total%: " << (Float_t)nMuRCC_total/lastEvent_total*100. << endl;       
   cout << "nMuNNM_total%: " << (Float_t)nMuNNM_total/lastEvent_total*100. << " nMuDRR_total%: " << (Float_t)nMuDRR_total/lastEvent_total*100. << " nMuRCG_total%: " << (Float_t)nMuRCG_total/lastEvent_total*100. << endl;       
   cout << "nMuNDD_total%: " << (Float_t)nMuNDD_total/lastEvent_total*100. << " nMuDRC_total%: " << (Float_t)nMuDRC_total/lastEvent_total*100. << " nMuRCM_total%: " << (Float_t)nMuRCM_total/lastEvent_total*100. << endl;
   cout << "nMuNDR_total%: " << (Float_t)nMuNDR_total/lastEvent_total*100. << " nMuDRG_total%: " << (Float_t)nMuDRG_total/lastEvent_total*100. << " nMuRGG_total%: " << (Float_t)nMuRGG_total/lastEvent_total*100. << endl;
   cout << "nMuNDC_total%: " << (Float_t)nMuNDC_total/lastEvent_total*100. << " nMuDRM_total%: " << (Float_t)nMuDRM_total/lastEvent_total*100. << " nMuRGM_total%: " << (Float_t)nMuRGM_total/lastEvent_total*100. << endl;
   cout << "nMuNDG_total%: " << (Float_t)nMuNDG_total/lastEvent_total*100. << " nMuDCC_total%: " << (Float_t)nMuDCC_total/lastEvent_total*100. << " nMuRMM_total%: " << (Float_t)nMuRMM_total/lastEvent_total*100. << endl;
   cout << "nMuNDM_total%: " << (Float_t)nMuNDM_total/lastEvent_total*100. << " nMuDCG_total%: " << (Float_t)nMuDCG_total/lastEvent_total*100. << endl; 
   cout << "nMuNRR_total%: " << (Float_t)nMuNRR_total/lastEvent_total*100. << " nMuDCM_total%: " << (Float_t)nMuDCM_total/lastEvent_total*100. << endl;                 
   cout << "nMuNRC_total%: " << (Float_t)nMuNRC_total/lastEvent_total*100. << " nMuDGG_total%: " << (Float_t)nMuDGG_total/lastEvent_total*100. << endl;                 
   cout << "nMuNRG_total%: " << (Float_t)nMuNRG_total/lastEvent_total*100. << " nMuDGM_total%: " << (Float_t)nMuDGM_total/lastEvent_total*100. << endl;                
   cout << "nMuNRM_total%: " << (Float_t)nMuNRM_total/lastEvent_total*100. << " nMuDMM_total%: " << (Float_t)nMuDMM_total/lastEvent_total*100. << endl;               
   cout << "nMuNCC_total%: " << (Float_t)nMuNCC_total/lastEvent_total*100. << endl;             
   cout << "nMuNCG_total%: " << (Float_t)nMuNCG_total/lastEvent_total*100. << endl;             
   cout << "nMuNCM_total%: " << (Float_t)nMuNCM_total/lastEvent_total*100. << endl;             
   cout << "nMuNGG_total%: " << (Float_t)nMuNGG_total/lastEvent_total*100. << endl;             
   cout << "nMuNGM_total%: " << (Float_t)nMuNGM_total/lastEvent_total*100. << endl;             
   cout << "nMuNMM_total%: " << (Float_t)nMuNMM_total/lastEvent_total*100. << endl;             
   
   cout << "nMuCCC_total%: " << (Float_t)nMuCCC_total/lastEvent_total*100. << " nMuGGG_total%: " << (Float_t)nMuGGG_total/lastEvent_total*100. << " nMuMMM_total%: " << (Float_t)nMuMMM_total/lastEvent_total*100. << endl;
   cout << "nMuCCG_total%: " << (Float_t)nMuCCG_total/lastEvent_total*100. << " nMuGGM_total%: " << (Float_t)nMuGGM_total/lastEvent_total*100. << endl;
   cout << "nMuCCM_total%: " << (Float_t)nMuCCM_total/lastEvent_total*100. << " nMuGMM_total%: " << (Float_t)nMuGMM_total/lastEvent_total*100. << endl;
   cout << "nMuCGG_total%: " << (Float_t)nMuCGG_total/lastEvent_total*100. << endl;
   cout << "nMuCGM_total%: " << (Float_t)nMuCGM_total/lastEvent_total*100. << endl;
   cout << "nMuCMM_total%: " << (Float_t)nMuCMM_total/lastEvent_total*100. << endl;




   TCanvas *c1 = new TCanvas("c1", "c1",15,49,1051,500);   

   Int_t numchains=56;

   Float_t chain[numchains];
   Float_t chainerr[numchains];

   for (Int_t i=0;i<numchains;i++)
   {
      chain[i]=i+1;
      chainerr[i]=0.;
   }


   Float_t counts_percent[56] = { (Float_t)nMuNNN_total, (Float_t)nMuNND_total, (Float_t)nMuNNR_total, (Float_t)nMuNNC_total, (Float_t)nMuNNG_total, (Float_t)nMuNNM_total,
                                 (Float_t)nMuNDD_total, (Float_t)nMuNDR_total, (Float_t)nMuNDC_total, (Float_t)nMuNDG_total, (Float_t)nMuNDM_total,
                                 (Float_t)nMuNRR_total, (Float_t)nMuNRC_total, (Float_t)nMuNRG_total, (Float_t)nMuNRM_total,
                                 (Float_t)nMuNCC_total, (Float_t)nMuNCG_total, (Float_t)nMuNCM_total,
                                 (Float_t)nMuNGG_total, (Float_t)nMuNGM_total,
                                 (Float_t)nMuNMM_total,
                                 (Float_t)nMuDDD_total, (Float_t)nMuDDR_total, (Float_t)nMuDDC_total, (Float_t)nMuDDG_total, (Float_t)nMuDDM_total,
                                 (Float_t)nMuDRR_total, (Float_t)nMuDRC_total, (Float_t)nMuDRG_total, (Float_t)nMuDRM_total,
                                 (Float_t)nMuDCC_total, (Float_t)nMuDCG_total, (Float_t)nMuDCM_total,
                                 (Float_t)nMuDGG_total, (Float_t)nMuDGM_total, 
                                 (Float_t)nMuDMM_total,
                                 (Float_t)nMuRRR_total, (Float_t)nMuRRC_total, (Float_t)nMuRRG_total, (Float_t)nMuRRM_total,
                                 (Float_t)nMuRCC_total, (Float_t)nMuRCG_total, (Float_t)nMuRCM_total,
                                 (Float_t)nMuRGG_total, (Float_t)nMuRGM_total,
                                 (Float_t)nMuRMM_total,
                                 (Float_t)nMuCCC_total, (Float_t)nMuCCG_total, (Float_t)nMuCCM_total,
                                 (Float_t)nMuCGG_total, (Float_t)nMuCGM_total,
                                 (Float_t)nMuCMM_total,
                                 (Float_t)nMuGGG_total, (Float_t)nMuGGM_total,
                                 (Float_t)nMuGMM_total,
                                 (Float_t)nMuMMM_total };

   Float_t err_counts_percent[numchains];

   for ( int i=0; i<numchains; i++ )
   {
      err_counts_percent[i] = sqrt( counts_percent[i] )*100./lastEvent_total;
      counts_percent[i] = counts_percent[i]/lastEvent_total*100.;
   }

   string names[56] = { "NNN", "NND", "NNR", "NNC", "NNG", "NNM", "NDD", "NDR", "NDC", "NDG", "NDM", "NRR", "NRC", "NRG", "NRM", 
                        "NCC", "NCG", "NCM", "NGG", "NGM", "NMM", 
                        "DDD", "DDR", "DDC", "DDG", "DDM", "DRR", "DRC", "DRG", "DRM", 
                        "DCC", "DCG", "DCM", "DGG", "DGM", "DMM", 
                        "RRR", "RRC", "RRG", "RRM", "RCC", "RCG", "RCM", "RGG", "RGM", "RMM", 
                        "CCC", "CCG", "CCM", "CGG", "CGM", "CMM", 
                        "GGG", "GGM", "GMM", 
                        "MMM" };

   TGraphErrors *tripleMuTopology = new TGraphErrors(numchains,chain,counts_percent,chainerr,err_counts_percent); 
   TAxis *ax = tripleMuTopology->GetHistogram()->GetXaxis();
   Double_t x1 = ax->GetBinLowEdge(1); 
   //h messoil -8 perchèse no i bin erano sfasati dai label
   //i label son0o 56, ma i bin sono 100
   Double_t x2 = ax->GetBinUpEdge(ax->GetNbins()-8);
   //cout << ax->GetNbins() << endl;
   tripleMuTopology->GetHistogram()->GetXaxis()->Set(56,x1,x2);
   tripleMuTopology->GetHistogram()->GetYaxis()->SetTitle("Tau->3 Mu [%]");
   tripleMuTopology->GetHistogram()->GetXaxis()->SetLabelSize(0.04);
   tripleMuTopology->SetTitle("Tau->3 Mu Topology in CMS");

   for(Int_t k=0;k<numchains;k++){
   tripleMuTopology->GetHistogram()->GetXaxis()->SetBinLabel(k+1,names[k].c_str());   
   } 

   tripleMuTopology->SetMarkerStyle(21);
   tripleMuTopology->SetMarkerSize(0.8);
   tripleMuTopology->Draw("");

   //only events D R C G M (see all the three mu)
   cout << "\nOnly with all three muons visible"<< endl;
   Int_t lastEvent_allThreeSeen = lastEvent_total - nMuN_total; 
    cout << "nMuDDD_total%: " << (Float_t)nMuDDD_total/lastEvent_allThreeSeen*100. << " nMuRRR_total%: " << (Float_t)nMuRRR_total/lastEvent_allThreeSeen*100. << endl; 
    cout << "nMuDDR_total%: " << (Float_t)nMuDDR_total/lastEvent_allThreeSeen*100. << " nMuRRC_total%: " << (Float_t)nMuRRC_total/lastEvent_allThreeSeen*100. << endl;      
    cout << "nMuDDC_total%: " << (Float_t)nMuDDC_total/lastEvent_allThreeSeen*100. << " nMuRRG_total%: " << (Float_t)nMuRRG_total/lastEvent_allThreeSeen*100. << endl;       
    cout << "nMuDDG_total%: " << (Float_t)nMuDDG_total/lastEvent_allThreeSeen*100. << " nMuRRM_total%: " << (Float_t)nMuRRM_total/lastEvent_allThreeSeen*100. << endl;       
    cout << "nMuDDM_total%: " << (Float_t)nMuDDM_total/lastEvent_allThreeSeen*100. << " nMuRCC_total%: " << (Float_t)nMuRCC_total/lastEvent_allThreeSeen*100. << endl;       
    cout << "nMuDRR_total%: " << (Float_t)nMuDRR_total/lastEvent_allThreeSeen*100. << " nMuRCG_total%: " << (Float_t)nMuRCG_total/lastEvent_allThreeSeen*100. << endl;       
    cout << "nMuDRC_total%: " << (Float_t)nMuDRC_total/lastEvent_allThreeSeen*100. << " nMuRCM_total%: " << (Float_t)nMuRCM_total/lastEvent_allThreeSeen*100. << endl;
    cout << "nMuDRG_total%: " << (Float_t)nMuDRG_total/lastEvent_allThreeSeen*100. << " nMuRGG_total%: " << (Float_t)nMuRGG_total/lastEvent_allThreeSeen*100. << endl;
    cout << "nMuDRM_total%: " << (Float_t)nMuDRM_total/lastEvent_allThreeSeen*100. << " nMuRGM_total%: " << (Float_t)nMuRGM_total/lastEvent_allThreeSeen*100. << endl;
    cout << "nMuDCC_total%: " << (Float_t)nMuDCC_total/lastEvent_allThreeSeen*100. << " nMuRMM_total%: " << (Float_t)nMuRMM_total/lastEvent_allThreeSeen*100. << endl;
    cout << "nMuDCG_total%: " << (Float_t)nMuDCG_total/lastEvent_allThreeSeen*100. << endl; 
    cout << "nMuDCM_total%: " << (Float_t)nMuDCM_total/lastEvent_allThreeSeen*100. << endl;                 
    cout << "nMuDGG_total%: " << (Float_t)nMuDGG_total/lastEvent_allThreeSeen*100. << endl;                 
    cout << "nMuDGM_total%: " << (Float_t)nMuDGM_total/lastEvent_allThreeSeen*100. << endl;                
    cout << "nMuDMM_total%: " << (Float_t)nMuDMM_total/lastEvent_allThreeSeen*100. << endl;               
    
    cout << "nMuCCC_total%: " << (Float_t)nMuCCC_total/lastEvent_allThreeSeen*100. << " nMuGGG_total%: " << (Float_t)nMuGGG_total/lastEvent_allThreeSeen*100. << " nMuMMM_total%: " << (Float_t)nMuMMM_total/lastEvent_allThreeSeen*100. << endl;
    cout << "nMuCCG_total%: " << (Float_t)nMuCCG_total/lastEvent_allThreeSeen*100. << " nMuGGM_total%: " << (Float_t)nMuGGM_total/lastEvent_allThreeSeen*100. << endl;
    cout << "nMuCCM_total%: " << (Float_t)nMuCCM_total/lastEvent_allThreeSeen*100. << " nMuGMM_total%: " << (Float_t)nMuGMM_total/lastEvent_allThreeSeen*100. << endl;
    cout << "nMuCGG_total%: " << (Float_t)nMuCGG_total/lastEvent_allThreeSeen*100. << endl;
    cout << "nMuCGM_total%: " << (Float_t)nMuCGM_total/lastEvent_allThreeSeen*100. << endl;
    cout << "nMuCMM_total%: " << (Float_t)nMuCMM_total/lastEvent_allThreeSeen*100. << endl;

    TCanvas *c2 = new TCanvas("c2", "c2",15,49,1051,500);                                                                                                                      
 
    Int_t numchains_2=35;
 
    Float_t chain_2[numchains_2];
    Float_t chainerr_2[numchains_2];
 
    for (Int_t i=0;i<numchains_2;i++)
    {
       chain_2[i]=i+1;
       chainerr_2[i]=0.;
    }
 
 
    Float_t counts_percent_2[35] = { (Float_t)nMuDDD_total, (Float_t)nMuDDR_total, (Float_t)nMuDDC_total, (Float_t)nMuDDG_total, (Float_t)nMuDDM_total,
                                  (Float_t)nMuDRR_total, (Float_t)nMuDRC_total, (Float_t)nMuDRG_total, (Float_t)nMuDRM_total,
                                  (Float_t)nMuDCC_total, (Float_t)nMuDCG_total, (Float_t)nMuDCM_total,
                                  (Float_t)nMuDGG_total, (Float_t)nMuDGM_total, 
                                  (Float_t)nMuDMM_total,
                                  (Float_t)nMuRRR_total, (Float_t)nMuRRC_total, (Float_t)nMuRRG_total, (Float_t)nMuRRM_total,
                                  (Float_t)nMuRCC_total, (Float_t)nMuRCG_total, (Float_t)nMuRCM_total,
                                  (Float_t)nMuRGG_total, (Float_t)nMuRGM_total,
                                  (Float_t)nMuRMM_total,
                                  (Float_t)nMuCCC_total, (Float_t)nMuCCG_total, (Float_t)nMuCCM_total,
                                  (Float_t)nMuCGG_total, (Float_t)nMuCGM_total,
                                  (Float_t)nMuCMM_total,
                                  (Float_t)nMuGGG_total, (Float_t)nMuGGM_total,
                                  (Float_t)nMuGMM_total,
                                  (Float_t)nMuMMM_total };
 
    Float_t err_counts_percent_2[numchains_2];
 
    for ( int i=0; i<numchains_2; i++ )
    {
       err_counts_percent_2[i] = sqrt( counts_percent_2[i] )*100./lastEvent_allThreeSeen;
       counts_percent_2[i] = counts_percent_2[i]/lastEvent_allThreeSeen*100.;
    }
 
    string names_2[35] = { "DDD", "DDR", "DDC", "DDG", "DDM", "DRR", "DRC", "DRG", "DRM", 
                         "DCC", "DCG", "DCM", "DGG", "DGM", "DMM", 
                         "RRR", "RRC", "RRG", "RRM", "RCC", "RCG", "RCM", "RGG", "RGM", "RMM", 
                         "CCC", "CCG", "CCM", "CGG", "CGM", "CMM", 
                         "GGG", "GGM", "GMM", 
                         "MMM" };


    TGraphErrors *tripleMuTopology_allThreeSeen = new TGraphErrors(35,chain_2,counts_percent_2,chainerr_2,err_counts_percent_2); 
    TAxis *ax_2 = tripleMuTopology_allThreeSeen->GetHistogram()->GetXaxis();
    Double_t x3 = ax_2->GetBinLowEdge(1); 
    //h messoil -8 perchèse no i bin erano sfasati dai label
    //i label son0o 56, ma i bin sono 100
    Double_t x4 = ax_2->GetBinUpEdge(ax_2->GetNbins()-7);
    //cout << ax->GetNbins() << endl;
    tripleMuTopology_allThreeSeen->GetHistogram()->GetXaxis()->Set(35,x3,x4);
    tripleMuTopology_allThreeSeen->GetHistogram()->GetYaxis()->SetTitle("Tau->3 Mu [%]");
    tripleMuTopology_allThreeSeen->GetHistogram()->GetXaxis()->SetLabelSize(0.04);
    tripleMuTopology_allThreeSeen->SetTitle("Tau->3 Mu Topology in CMS (all 3 muons seen)");
                                                                                                                    
    for(Int_t k=0;k<numchains_2;k++){
    tripleMuTopology_allThreeSeen->GetHistogram()->GetXaxis()->SetBinLabel(k+1,names_2[k].c_str());   
    } 
                                                                                                                    
    tripleMuTopology_allThreeSeen->SetMarkerStyle(21);
    tripleMuTopology_allThreeSeen->SetMarkerSize(0.8);
    tripleMuTopology_allThreeSeen->Draw("");

    //check the Delta Eta and Delta pt of muons
    //
    cout << "eventEntries:" << eventEntries << endl;

    for ( Int_t i=0; i < eventEntries; i++ ) 
    {
       cout << "Entry#" << i << endl;
       tree_event->GetEntry(i);
  
        if ( isMuDDD )                                                                              
        {                                                                                           
           cout << i << " isMuDDD" << endl; 
           //calculate the minimum eta
           DDDminEta = fabs(muEta[0]);
           DDDmaxEta = fabs(muEta[0]);
           DDDminPt  = fabs(muPt[0]);
           DDDmaxPt  = fabs(muPt[0]);
           //cout << fabs(muEta[0]) << " " << fabs(muEta[1]) << " " << fabs(muEta[2]) << endl;
           //cout << muPt[0] << " " << muPt[1] << " " << muPt[2] << endl;
           for (int j=1; j<3; j++)
           {
              if ( fabs(muEta[j]) < DDDminEta ) DDDminEta = fabs(muEta[j]);
              if ( fabs(muEta[j]) > DDDmaxEta ) DDDmaxEta = fabs(muEta[j]);
              if ( fabs(muPt[j])  < DDDminPt  ) DDDminPt  = fabs(muPt[j]);
              if ( fabs(muPt[j])  > DDDmaxPt  ) DDDmaxPt  = fabs(muPt[j]);
           }
           //cout << DDDminEta << " " << DDDmaxEta << endl;
           //cout << DDDminPt  << " " << DDDmaxPt  << endl;
           
           tree_DDD -> Fill();
 
        }

       if ( isMuDDR )  
       { 
          cout << i << " isMuDDR" << endl; 
          //calculate the minimum eta
          DDRminEta = fabs(muEta[0]);
          DDRmaxEta = fabs(muEta[0]);
          DDRminPt  = fabs(muPt[0]);
          DDRmaxPt  = fabs(muPt[0]);
          //cout << fabs(muEta[0]) << " " << fabs(muEta[1]) << " " << fabs(muEta[2]) << endl;
          //cout << muPt[0] << " " << muPt[1] << " " << muPt[2] << endl;
          for (int j=1; j<3; j++)
          {
             if ( fabs(muEta[j]) < DDRminEta ) DDRminEta = fabs(muEta[j]);
             if ( fabs(muEta[j]) > DDRmaxEta ) DDRmaxEta = fabs(muEta[j]);
             if ( fabs(muPt[j])  < DDRminPt  ) DDRminPt  = fabs(muPt[j]);
             if ( fabs(muPt[j])  > DDRmaxPt  ) DDRmaxPt  = fabs(muPt[j]);
          }
          //cout << DDRminEta << " " << DDRmaxEta << endl;
          //cout << DDRminPt  << " " << DDRmaxPt  << endl;
          
          tree_DDR -> Fill();

       }

       if ( isMuDDC )  
       { 
          cout << i << " isMuDDC" << endl; 
          //calculate the minimum eta
          DDCminEta = fabs(muEta[0]);
          DDCmaxEta = fabs(muEta[0]);
          DDCminPt  = fabs(muPt[0]);
          DDCmaxPt  = fabs(muPt[0]);
          //cout << fabs(muEta[0]) << " " << fabs(muEta[1]) << " " << fabs(muEta[2]) << endl;
          //cout << muPt[0] << " " << muPt[1] << " " << muPt[2] << endl;
        
  for (int j=1; j<3; j++)
          {
             if ( fabs(muEta[j]) < DDCminEta ) DDCminEta = fabs(muEta[j]);
             if ( fabs(muEta[j]) > DDCmaxEta ) DDCmaxEta = fabs(muEta[j]);
             if ( fabs(muPt[j])  < DDCminPt  ) DDCminPt  = fabs(muPt[j]);
             if ( fabs(muPt[j])  > DDCmaxPt  ) DDCmaxPt  = fabs(muPt[j]);
          }
          //cout << DDCminEta << " " << DDCmaxEta << endl;
          //cout << DDCminPt  << " " << DDCmaxPt  << endl;
          
          tree_DDC -> Fill();

       }

       if ( isMuDDG )  
       { 
          cout << i << " isMuDDG" << endl; 
          //calculate the minimum eta
          DDGminEta = fabs(muEta[0]);
          DDGmaxEta = fabs(muEta[0]);
          DDGminPt  = fabs(muPt[0]);
          DDGmaxPt  = fabs(muPt[0]);
          //cout << fabs(muEta[0]) << " " << fabs(muEta[1]) << " " << fabs(muEta[2]) << endl;
          //cout << muPt[0] << " " << muPt[1] << " " << muPt[2] << endl;
          for (int j=1; j<3; j++)
          {
             if ( fabs(muEta[j]) < DDGminEta ) DDGminEta = fabs(muEta[j]);
             if ( fabs(muEta[j]) > DDGmaxEta ) DDGmaxEta = fabs(muEta[j]);
             if ( fabs(muPt[j])  < DDGminPt  ) DDGminPt  = fabs(muPt[j]);
             if ( fabs(muPt[j])  > DDGmaxPt  ) DDGmaxPt  = fabs(muPt[j]);
          }
          //cout << DDGminEta << " " << DDGmaxEta << endl;
          //cout << DDGminPt  << " " << DDGmaxPt  << endl;
          
          tree_DDG -> Fill();

       }

       if ( isMuDDM )  
       { 
          cout << i << " isMuDDM" << endl; 
          //calculate the minimum eta
          DDMminEta = fabs(muEta[0]);
          DDMmaxEta = fabs(muEta[0]);
          DDMminPt  = fabs(muPt[0]);
          DDMmaxPt  = fabs(muPt[0]);
          //cout << fabs(muEta[0]) << " " << fabs(muEta[1]) << " " << fabs(muEta[2]) << endl;
          //cout << muPt[0] << " " << muPt[1] << " " << muPt[2] << endl;
          for (int j=1; j<3; j++)
          {
             if ( fabs(muEta[j]) < DDMminEta ) DDMminEta = fabs(muEta[j]);
             if ( fabs(muEta[j]) > DDMmaxEta ) DDMmaxEta = fabs(muEta[j]);
             if ( fabs(muPt[j])  < DDMminPt  ) DDMminPt  = fabs(muPt[j]);
             if ( fabs(muPt[j])  > DDMmaxPt  ) DDMmaxPt  = fabs(muPt[j]);
          }
          //cout << DDMminEta << " " << DDMmaxEta << endl;
          //cout << DDMminPt  << " " << DDMmaxPt  << endl;
          
          tree_DDM -> Fill();

       }

       if ( isMuDRR )  
       { 
          cout << i << " isMuDRR" << endl; 
          //calculate the minimum eta
          DRRminEta = fabs(muEta[0]);
          DRRmaxEta = fabs(muEta[0]);
          DRRminPt  = fabs(muPt[0]);
          DRRmaxPt  = fabs(muPt[0]);
          //cout << fabs(muEta[0]) << " " << fabs(muEta[1]) << " " << fabs(muEta[2]) << endl;
          //cout << muPt[0] << " " << muPt[1] << " " << muPt[2] << endl;
          for (int j=1; j<3; j++)
          {
             if ( fabs(muEta[j]) < DRRminEta ) DRRminEta = fabs(muEta[j]);
             if ( fabs(muEta[j]) > DRRmaxEta ) DRRmaxEta = fabs(muEta[j]);
             if ( fabs(muPt[j])  < DRRminPt  ) DRRminPt  = fabs(muPt[j]);
             if ( fabs(muPt[j])  > DRRmaxPt  ) DRRmaxPt  = fabs(muPt[j]);
          }
          //cout << DRRminEta << " " << DRRmaxEta << endl;
          //cout << DRRminPt  << " " << DRRmaxPt  << endl;
          
          tree_DRR -> Fill();

       }

       if ( isMuDRC )  
       { 
          cout << i << " isMuDRC" << endl; 
          //calculate the minimum eta
          DRCminEta = fabs(muEta[0]);
          DRCmaxEta = fabs(muEta[0]);
          DRCminPt  = fabs(muPt[0]);
          DRCmaxPt  = fabs(muPt[0]);
          //cout << fabs(muEta[0]) << " " << fabs(muEta[1]) << " " << fabs(muEta[2]) << endl;
          //cout << muPt[0] << " " << muPt[1] << " " << muPt[2] << endl;
          for (int j=1; j<3; j++)
          {
             if ( fabs(muEta[j]) < DRCminEta ) DRCminEta = fabs(muEta[j]);
             if ( fabs(muEta[j]) > DRCmaxEta ) DRCmaxEta = fabs(muEta[j]);
             if ( fabs(muPt[j])  < DRCminPt  ) DRCminPt  = fabs(muPt[j]);
             if ( fabs(muPt[j])  > DRCmaxPt  ) DRCmaxPt  = fabs(muPt[j]);
          }
          //cout << DRCminEta << " " << DRCmaxEta << endl;
          //cout << DRCminPt  << " " << DRCmaxPt  << endl;
          
          tree_DRC -> Fill();

       }

       if ( isMuDRG )  
       { 
          cout << i << " isMuDRG" << endl; 
          //calculate the minimum eta
          DRGminEta = fabs(muEta[0]);
          DRGmaxEta = fabs(muEta[0]);
          DRGminPt  = fabs(muPt[0]);
          DRGmaxPt  = fabs(muPt[0]);
          //cout << fabs(muEta[0]) << " " << fabs(muEta[1]) << " " << fabs(muEta[2]) << endl;
          //cout << muPt[0] << " " << muPt[1] << " " << muPt[2] << endl;
          for (int j=1; j<3; j++)
          {
             if ( fabs(muEta[j]) < DRGminEta ) DRGminEta = fabs(muEta[j]);
             if ( fabs(muEta[j]) > DRGmaxEta ) DRGmaxEta = fabs(muEta[j]);
             if ( fabs(muPt[j])  < DRGminPt  ) DRGminPt  = fabs(muPt[j]);
             if ( fabs(muPt[j])  > DRGmaxPt  ) DRGmaxPt  = fabs(muPt[j]);
          }
          //cout << DRGminEta << " " << DRGmaxEta << endl;
          //cout << DRGminPt  << " " << DRGmaxPt  << endl;
          
          tree_DRG -> Fill();

       }

       if ( isMuDRM )  
       { 
          cout << i << " isMuDRM" << endl; 
          //calculate the minimum eta
          DRMminEta = fabs(muEta[0]);
          DRMmaxEta = fabs(muEta[0]);
          DRMminPt  = fabs(muPt[0]);
          DRMmaxPt  = fabs(muPt[0]);
          //cout << fabs(muEta[0]) << " " << fabs(muEta[1]) << " " << fabs(muEta[2]) << endl;
          //cout << muPt[0] << " " << muPt[1] << " " << muPt[2] << endl;
          for (int j=1; j<3; j++)
          {
             if ( fabs(muEta[j]) < DRMminEta ) DRMminEta = fabs(muEta[j]);
             if ( fabs(muEta[j]) > DRMmaxEta ) DRMmaxEta = fabs(muEta[j]);
             if ( fabs(muPt[j])  < DRMminPt  ) DRMminPt  = fabs(muPt[j]);
             if ( fabs(muPt[j])  > DRMmaxPt  ) DRMmaxPt  = fabs(muPt[j]);
          }
          //cout << DRMminEta << " " << DRMmaxEta << endl;
          //cout << DRMminPt  << " " << DRMmaxPt  << endl;
          
          tree_DRM -> Fill();

       }

       if ( isMuDCC )  
       { 
          cout << i << " isMuDCC" << endl; 
          //calculate the minimum eta
          DCCminEta = fabs(muEta[0]);
          DCCmaxEta = fabs(muEta[0]);
          DCCminPt  = fabs(muPt[0]);
          DCCmaxPt  = fabs(muPt[0]);
          //cout << fabs(muEta[0]) << " " << fabs(muEta[1]) << " " << fabs(muEta[2]) << endl;
          //cout << muPt[0] << " " << muPt[1] << " " << muPt[2] << endl;
          for (int j=1; j<3; j++)
          {
             if ( fabs(muEta[j]) < DCCminEta ) DCCminEta = fabs(muEta[j]);
             if ( fabs(muEta[j]) > DCCmaxEta ) DCCmaxEta = fabs(muEta[j]);
             if ( fabs(muPt[j])  < DCCminPt  ) DCCminPt  = fabs(muPt[j]);
             if ( fabs(muPt[j])  > DCCmaxPt  ) DCCmaxPt  = fabs(muPt[j]);
          }
          //cout << DCCminEta << " " << DCCmaxEta << endl;
          //cout << DCCminPt  << " " << DCCmaxPt  << endl;
          
          tree_DCC -> Fill();

       }

       if ( isMuDCG )  
       { 
          cout << i << " isMuDCG" << endl; 
          //calculate the minimum eta
          DCGminEta = fabs(muEta[0]);
          DCGmaxEta = fabs(muEta[0]);
          DCGminPt  = fabs(muPt[0]);
          DCGmaxPt  = fabs(muPt[0]);
          //cout << fabs(muEta[0]) << " " << fabs(muEta[1]) << " " << fabs(muEta[2]) << endl;
          //cout << muPt[0] << " " << muPt[1] << " " << muPt[2] << endl;
          for (int j=1; j<3; j++)
          {
             if ( fabs(muEta[j]) < DCGminEta ) DCGminEta = fabs(muEta[j]);
             if ( fabs(muEta[j]) > DCGmaxEta ) DCGmaxEta = fabs(muEta[j]);
             if ( fabs(muPt[j])  < DCGminPt  ) DCGminPt  = fabs(muPt[j]);
             if ( fabs(muPt[j])  > DCGmaxPt  ) DCGmaxPt  = fabs(muPt[j]);
          }
          //cout << DCGminEta << " " << DCGmaxEta << endl;
          //cout << DCGminPt  << " " << DCGmaxPt  << endl;
          
          tree_DCG -> Fill();

       }

       if ( isMuDCM )  
       { 
          cout << i << " isMuDCM" << endl; 
          //calculate the minimum eta
          DCMminEta = fabs(muEta[0]);
          DCMmaxEta = fabs(muEta[0]);
          DCMminPt  = fabs(muPt[0]);
          DCMmaxPt  = fabs(muPt[0]);
          //cout << fabs(muEta[0]) << " " << fabs(muEta[1]) << " " << fabs(muEta[2]) << endl;
          //cout << muPt[0] << " " << muPt[1] << " " << muPt[2] << endl;
          for (int j=1; j<3; j++)
          {
             if ( fabs(muEta[j]) < DCMminEta ) DCMminEta = fabs(muEta[j]);
             if ( fabs(muEta[j]) > DCMmaxEta ) DCMmaxEta = fabs(muEta[j]);
             if ( fabs(muPt[j])  < DCMminPt  ) DCMminPt  = fabs(muPt[j]);
             if ( fabs(muPt[j])  > DCMmaxPt  ) DCMmaxPt  = fabs(muPt[j]);
          }
          //cout << DCMminEta << " " << DCMmaxEta << endl;
          //cout << DCMminPt  << " " << DCMmaxPt  << endl;
          
          tree_DCM -> Fill();

       }

       if ( isMuDGG )  
       { 
          cout << i << " isMuDGG" << endl; 
          //calculate the minimum eta
          DGGminEta = fabs(muEta[0]);
          DGGmaxEta = fabs(muEta[0]);
          DGGminPt  = fabs(muPt[0]);
          DGGmaxPt  = fabs(muPt[0]);
          //cout << fabs(muEta[0]) << " " << fabs(muEta[1]) << " " << fabs(muEta[2]) << endl;
          //cout << muPt[0] << " " << muPt[1] << " " << muPt[2] << endl;
          for (int j=1; j<3; j++)
          {
             if ( fabs(muEta[j]) < DGGminEta ) DGGminEta = fabs(muEta[j]);
             if ( fabs(muEta[j]) > DGGmaxEta ) DGGmaxEta = fabs(muEta[j]);
             if ( fabs(muPt[j])  < DGGminPt  ) DGGminPt  = fabs(muPt[j]);
             if ( fabs(muPt[j])  > DGGmaxPt  ) DGGmaxPt  = fabs(muPt[j]);
          }
          //cout << DGGminEta << " " << DGGmaxEta << endl;
          //cout << DGGminPt  << " " << DGGmaxPt  << endl;
          
          tree_DGG -> Fill();

       }

       if ( isMuDGM )  
       { 
          cout << i << " isMuDGM" << endl; 
          //calculate the minimum eta
          DGMminEta = fabs(muEta[0]);
          DGMmaxEta = fabs(muEta[0]);
          DGMminPt  = fabs(muPt[0]);
          DGMmaxPt  = fabs(muPt[0]);
          //cout << fabs(muEta[0]) << " " << fabs(muEta[1]) << " " << fabs(muEta[2]) << endl;
          //cout << muPt[0] << " " << muPt[1] << " " << muPt[2] << endl;
          for (int j=1; j<3; j++)
          {
             if ( fabs(muEta[j]) < DGMminEta ) DGMminEta = fabs(muEta[j]);
             if ( fabs(muEta[j]) > DGMmaxEta ) DGMmaxEta = fabs(muEta[j]);
             if ( fabs(muPt[j])  < DGMminPt  ) DGMminPt  = fabs(muPt[j]);
             if ( fabs(muPt[j])  > DGMmaxPt  ) DGMmaxPt  = fabs(muPt[j]);
          }
          //cout << DGMminEta << " " << DGMmaxEta << endl;
          //cout << DGMminPt  << " " << DGMmaxPt  << endl;
          
          tree_DGM -> Fill();

       }

       if ( isMuDMM )  
       { 
          cout << i << " isMuDMM" << endl; 
          //calculate the minimum eta
          DMMminEta = fabs(muEta[0]);
          DMMmaxEta = fabs(muEta[0]);
          DMMminPt  = fabs(muPt[0]);
          DMMmaxPt  = fabs(muPt[0]);
          //cout << fabs(muEta[0]) << " " << fabs(muEta[1]) << " " << fabs(muEta[2]) << endl;
          //cout << muPt[0] << " " << muPt[1] << " " << muPt[2] << endl;
          for (int j=1; j<3; j++)
          {
             if ( fabs(muEta[j]) < DMMminEta ) DMMminEta = fabs(muEta[j]);
             if ( fabs(muEta[j]) > DMMmaxEta ) DMMmaxEta = fabs(muEta[j]);
             if ( fabs(muPt[j])  < DMMminPt  ) DMMminPt  = fabs(muPt[j]);
             if ( fabs(muPt[j])  > DMMmaxPt  ) DMMmaxPt  = fabs(muPt[j]);
          }
          //cout << DMMminEta << " " << DMMmaxEta << endl;
          //cout << DMMminPt  << " " << DMMmaxPt  << endl;
          
          tree_DMM -> Fill();

       }


       if ( isMuRRR )  
       { 
          cout << i << " isMuRRR" << endl; 
          //calculate the minimum eta
          RRRminEta = fabs(muEta[0]);
          RRRmaxEta = fabs(muEta[0]);
          RRRminPt  = fabs(muPt[0]);
          RRRmaxPt  = fabs(muPt[0]);
          //cout << fabs(muEta[0]) << " " << fabs(muEta[1]) << " " << fabs(muEta[2]) << endl;
          //cout << muPt[0] << " " << muPt[1] << " " << muPt[2] << endl;
          for (int j=1; j<3; j++)
          {
             if ( fabs(muEta[j]) < RRRminEta ) RRRminEta = fabs(muEta[j]);
             if ( fabs(muEta[j]) > RRRmaxEta ) RRRmaxEta = fabs(muEta[j]);
             if ( fabs(muPt[j])  < RRRminPt  ) RRRminPt  = fabs(muPt[j]);
             if ( fabs(muPt[j])  > RRRmaxPt  ) RRRmaxPt  = fabs(muPt[j]);
          }
          //cout << RRRminEta << " " << RRRmaxEta << endl;
          //cout << RRRminPt  << " " << RRRmaxPt  << endl;
          
          tree_RRR -> Fill();

       }

       if ( isMuRRC )  
       { 
          cout << i << " isMuRRC" << endl; 
          //calculate the minimum eta
          RRCminEta = fabs(muEta[0]);
          RRCmaxEta = fabs(muEta[0]);
          RRCminPt  = fabs(muPt[0]);
          RRCmaxPt  = fabs(muPt[0]);
          //cout << fabs(muEta[0]) << " " << fabs(muEta[1]) << " " << fabs(muEta[2]) << endl;
          //cout << muPt[0] << " " << muPt[1] << " " << muPt[2] << endl;
          for (int j=1; j<3; j++)
          {
             if ( fabs(muEta[j]) < RRCminEta ) RRCminEta = fabs(muEta[j]);
             if ( fabs(muEta[j]) > RRCmaxEta ) RRCmaxEta = fabs(muEta[j]);
             if ( fabs(muPt[j])  < RRCminPt  ) RRCminPt  = fabs(muPt[j]);
             if ( fabs(muPt[j])  > RRCmaxPt  ) RRCmaxPt  = fabs(muPt[j]);
          }
          //cout << RRCminEta << " " << RRCmaxEta << endl;
          //cout << RRCminPt  << " " << RRCmaxPt  << endl;
          
          tree_RRC -> Fill();
                                                                                              
       }

       if ( isMuRRG )                                                                              
       { 
          cout << i << " isMuRRG" << endl; 
          //calculate the minimum eta
          RRGminEta = fabs(muEta[0]);
          RRGmaxEta = fabs(muEta[0]);
          RRGminPt  = fabs(muPt[0]);
          RRGmaxPt  = fabs(muPt[0]);
          //cout << fabs(muEta[0]) << " " << fabs(muEta[1]) << " " << fabs(muEta[2]) << endl;
          //cout << muPt[0] << " " << muPt[1] << " " << muPt[2] << endl;
          for (int j=1; j<3; j++)
          {
             if ( fabs(muEta[j]) < RRGminEta ) RRGminEta = fabs(muEta[j]);
             if ( fabs(muEta[j]) > RRGmaxEta ) RRGmaxEta = fabs(muEta[j]);
             if ( fabs(muPt[j])  < RRGminPt  ) RRGminPt  = fabs(muPt[j]);
             if ( fabs(muPt[j])  > RRGmaxPt  ) RRGmaxPt  = fabs(muPt[j]);
          }
          //cout << RRGminEta << " " << RRGmaxEta << endl;
          //cout << RRGminPt  << " " << RRGmaxPt  << endl;
          
          tree_RRG -> Fill();
                                                                                              
       }

       if ( isMuRRM )  
       { 
          cout << i << " isMuRRM" << endl; 
          //calculate the minimum eta
          RRMminEta = fabs(muEta[0]);
          RRMmaxEta = fabs(muEta[0]);
          RRMminPt  = fabs(muPt[0]);
          RRMmaxPt  = fabs(muPt[0]);
          //cout << fabs(muEta[0]) << " " << fabs(muEta[1]) << " " << fabs(muEta[2]) << endl;
          //cout << muPt[0] << " " << muPt[1] << " " << muPt[2] << endl;
          for (int j=1; j<3; j++)
          {
             if ( fabs(muEta[j]) < RRMminEta ) RRMminEta = fabs(muEta[j]);
             if ( fabs(muEta[j]) > RRMmaxEta ) RRMmaxEta = fabs(muEta[j]);
             if ( fabs(muPt[j])  < RRMminPt  ) RRMminPt  = fabs(muPt[j]);
             if ( fabs(muPt[j])  > RRMmaxPt  ) RRMmaxPt  = fabs(muPt[j]);
          }
          //cout << RRMminEta << " " << RRMmaxEta << endl;
          //cout << RRMminPt  << " " << RRMmaxPt  << endl;
          
          tree_RRM -> Fill();
                                                                                              
       }

       if ( isMuRCC )  
       { 
          cout << i << " isMuRCC" << endl; 
          //calculate the minimum eta
          RCCminEta = fabs(muEta[0]);
          RCCmaxEta = fabs(muEta[0]);
          RCCminPt  = fabs(muPt[0]);
          RCCmaxPt  = fabs(muPt[0]);
          //cout << fabs(muEta[0]) << " " << fabs(muEta[1]) << " " << fabs(muEta[2]) << endl;
          //cout << muPt[0] << " " << muPt[1] << " " << muPt[2] << endl;
          for (int j=1; j<3; j++)
          {
             if ( fabs(muEta[j]) < RCCminEta ) RCCminEta = fabs(muEta[j]);
             if ( fabs(muEta[j]) > RCCmaxEta ) RCCmaxEta = fabs(muEta[j]);
             if ( fabs(muPt[j])  < RCCminPt  ) RCCminPt  = fabs(muPt[j]);
             if ( fabs(muPt[j])  > RCCmaxPt  ) RCCmaxPt  = fabs(muPt[j]);
          }
          //cout << RCCminEta << " " << RCCmaxEta << endl;
          //cout << RCCminPt  << " " << RCCmaxPt  << endl;
          
          tree_RCC -> Fill();
                                                                                              
       }

       if ( isMuRCG )  
       { 
          cout << i << " isMuRCG" << endl; 
          //calculate the minimum eta
          RCGminEta = fabs(muEta[0]);
          RCGmaxEta = fabs(muEta[0]);
          RCGminPt  = fabs(muPt[0]);
          RCGmaxPt  = fabs(muPt[0]);
          //cout << fabs(muEta[0]) << " " << fabs(muEta[1]) << " " << fabs(muEta[2]) << endl;
          //cout << muPt[0] << " " << muPt[1] << " " << muPt[2] << endl;
          for (int j=1; j<3; j++)
          {
             if ( fabs(muEta[j]) < RCGminEta ) RCGminEta = fabs(muEta[j]);
             if ( fabs(muEta[j]) > RCGmaxEta ) RCGmaxEta = fabs(muEta[j]);
             if ( fabs(muPt[j])  < RCGminPt  ) RCGminPt  = fabs(muPt[j]);
             if ( fabs(muPt[j])  > RCGmaxPt  ) RCGmaxPt  = fabs(muPt[j]);
          }
          //cout << RCGminEta << " " << RCGmaxEta << endl;
          //cout << RCGminPt  << " " << RCGmaxPt  << endl;
          
          tree_RCG -> Fill();
                                                                                              
       }

       if ( isMuRCM )  
       { 
          cout << i << " isMuRCM" << endl;                                                     
          //calculate the minimum eta
          RCMminEta = fabs(muEta[0]);
          RCMmaxEta = fabs(muEta[0]);
          RCMminPt  = fabs(muPt[0]);
          RCMmaxPt  = fabs(muPt[0]);
          //cout << fabs(muEta[0]) << " " << fabs(muEta[1]) << " " << fabs(muEta[2]) << endl;
          //cout << muPt[0] << " " << muPt[1] << " " << muPt[2] << endl;
          for (int j=1; j<3; j++)
          {
             if ( fabs(muEta[j]) < RCMminEta ) RCMminEta = fabs(muEta[j]);
             if ( fabs(muEta[j]) > RCMmaxEta ) RCMmaxEta = fabs(muEta[j]);
             if ( fabs(muPt[j])  < RCMminPt  ) RCMminPt  = fabs(muPt[j]);
             if ( fabs(muPt[j])  > RCMmaxPt  ) RCMmaxPt  = fabs(muPt[j]);
          }
          //cout << RCMminEta << " " << RCMmaxEta << endl;
          //cout << RCMminPt  << " " << RCMmaxPt  << endl;
          
          tree_RCM -> Fill();
                                                                                              
       }

       if ( isMuRGG )  
       { 
          cout << i << " isMuRGG" << endl; 
          //calculate the minimum eta
          RGGminEta = fabs(muEta[0]);
          RGGmaxEta = fabs(muEta[0]);
          RGGminPt  = fabs(muPt[0]);
          RGGmaxPt  = fabs(muPt[0]);
          //cout << fabs(muEta[0]) << " " << fabs(muEta[1]) << " " << fabs(muEta[2]) << endl;
          //cout << muPt[0] << " " << muPt[1] << " " << muPt[2] << endl;
        
  for (int j=1; j<3; j++)
          {
             if ( fabs(muEta[j]) < RGGminEta ) RGGminEta = fabs(muEta[j]);
             if ( fabs(muEta[j]) > RGGmaxEta ) RGGmaxEta = fabs(muEta[j]);
             if ( fabs(muPt[j])  < RGGminPt  ) RGGminPt  = fabs(muPt[j]);
             if ( fabs(muPt[j])  > RGGmaxPt  ) RGGmaxPt  = fabs(muPt[j]);
          }
          //cout << RGGminEta << " " << RGGmaxEta << endl;
          //cout << RGGminPt  << " " << RGGmaxPt  << endl;
          
          tree_RGG -> Fill();
                                                                                              
       }

       if ( isMuRGM )  
       { 
          cout << i << " isMuRGM" << endl; 
          //calculate the minimum eta
          RGMminEta = fabs(muEta[0]);
          RGMmaxEta = fabs(muEta[0]);
          RGMminPt  = fabs(muPt[0]);
          RGMmaxPt  = fabs(muPt[0]);
          //cout << fabs(muEta[0]) << " " << fabs(muEta[1]) << " " << fabs(muEta[2]) << endl;
          //cout << muPt[0] << " " << muPt[1] << " " << muPt[2] << endl;
          for (int j=1; j<3; j++)
          {
             if ( fabs(muEta[j]) < RGMminEta ) RGMminEta = fabs(muEta[j]);
             if ( fabs(muEta[j]) > RGMmaxEta ) RGMmaxEta = fabs(muEta[j]);
             if ( fabs(muPt[j])  < RGMminPt  ) RGMminPt  = fabs(muPt[j]);
             if ( fabs(muPt[j])  > RGMmaxPt  ) RGMmaxPt  = fabs(muPt[j]);
          }
          //cout << RGMminEta << " " << RGMmaxEta << endl;
          //cout << RGMminPt  << " " << RGMmaxPt  << endl;
          
          tree_RGM -> Fill();
                                                                                              
       }

       if ( isMuRMM )  
       { 
          cout << i << " isMuRMM" << endl; 
          //calculate the minimum eta
          RMMminEta = fabs(muEta[0]);
          RMMmaxEta = fabs(muEta[0]);
          RMMminPt  = fabs(muPt[0]);
          RMMmaxPt  = fabs(muPt[0]);
          //cout << fabs(muEta[0]) << " " << fabs(muEta[1]) << " " << fabs(muEta[2]) << endl;
          //cout << muPt[0] << " " << muPt[1] << " " << muPt[2] << endl;
          for (int j=1; j<3; j++)
          {
             if ( fabs(muEta[j]) < RMMminEta ) RMMminEta = fabs(muEta[j]);
             if ( fabs(muEta[j]) > RMMmaxEta ) RMMmaxEta = fabs(muEta[j]);
             if ( fabs(muPt[j])  < RMMminPt  ) RMMminPt  = fabs(muPt[j]);
             if ( fabs(muPt[j])  > RMMmaxPt  ) RMMmaxPt  = fabs(muPt[j]);
          }
          //cout << RMMminEta << " " << RMMmaxEta << endl;
          //cout << RMMminPt  << " " << RMMmaxPt  << endl;
          
          tree_RMM -> Fill();
                                                                                              
       }

       if ( isMuCCC )  
       { 
          cout << i << " isMuCCC" << endl; 
          //calculate the minimum eta
          CCCminEta = fabs(muEta[0]);
          CCCmaxEta = fabs(muEta[0]);
          CCCminPt  = fabs(muPt[0]);
          CCCmaxPt  = fabs(muPt[0]);
          //cout << fabs(muEta[0]) << " " << fabs(muEta[1]) << " " << fabs(muEta[2]) << endl;
          //cout << muPt[0] << " " << muPt[1] << " " << muPt[2] << endl;
          for (int j=1; j<3; j++)
          {
             if ( fabs(muEta[j]) < CCCminEta ) CCCminEta = fabs(muEta[j]);
             if ( fabs(muEta[j]) > CCCmaxEta ) CCCmaxEta = fabs(muEta[j]);
             if ( fabs(muPt[j])  < CCCminPt  ) CCCminPt  = fabs(muPt[j]);
             if ( fabs(muPt[j])  > CCCmaxPt  ) CCCmaxPt  = fabs(muPt[j]);
          }
          //cout << CCCminEta << " " << CCCmaxEta << endl;
          //cout << CCCminPt  << " " << CCCmaxPt  << endl;
          
          tree_CCC -> Fill();
                                                                                              
       }

       if ( isMuCCG )  
       { 
          cout << i << " isMuCCG" << endl; 
          //calculate the minimum eta
          CCGminEta = fabs(muEta[0]);
          CCGmaxEta = fabs(muEta[0]);
          CCGminPt  = fabs(muPt[0]);
          CCGmaxPt  = fabs(muPt[0]);
          //cout << fabs(muEta[0]) << " " << fabs(muEta[1]) << " " << fabs(muEta[2]) << endl;
          //cout << muPt[0] << " " << muPt[1] << " " << muPt[2] << endl;
          for (int j=1; j<3; j++)
          {
             if ( fabs(muEta[j]) < CCGminEta ) CCGminEta = fabs(muEta[j]);
             if ( fabs(muEta[j]) > CCGmaxEta ) CCGmaxEta = fabs(muEta[j]);
             if ( fabs(muPt[j])  < CCGminPt  ) CCGminPt  = fabs(muPt[j]);
             if ( fabs(muPt[j])  > CCGmaxPt  ) CCGmaxPt  = fabs(muPt[j]);
          }
          //cout << CCGminEta << " " << CCGmaxEta << endl;
          //cout << CCGminPt  << " " << CCGmaxPt  << endl;
          
          tree_CCG -> Fill();
                                                                                              
       }

       if ( isMuCCM )  
       { 
          cout << i << " isMuCCM" << endl; 
          //calculate the minimum eta
          CCMminEta = fabs(muEta[0]);
          CCMmaxEta = fabs(muEta[0]);
          CCMminPt  = fabs(muPt[0]);
          CCMmaxPt  = fabs(muPt[0]);
          //cout << fabs(muEta[0]) << " " << fabs(muEta[1]) << " " << fabs(muEta[2]) << endl;
          //cout << muPt[0] << " " << muPt[1] << " " << muPt[2] << endl;
          for (int j=1; j<3; j++)
          {
             if ( fabs(muEta[j]) < CCMminEta ) CCMminEta = fabs(muEta[j]);
             if ( fabs(muEta[j]) > CCMmaxEta ) CCMmaxEta = fabs(muEta[j]);
             if ( fabs(muPt[j])  < CCMminPt  ) CCMminPt  = fabs(muPt[j]);
             if ( fabs(muPt[j])  > CCMmaxPt  ) CCMmaxPt  = fabs(muPt[j]);
          }
          //cout << CCMminEta << " " << CCMmaxEta << endl;
          //cout << CCMminPt  << " " << CCMmaxPt  << endl;
          
          tree_CCM -> Fill();
                                                                                              
       }

       if ( isMuCGG )  
       { 
          cout << i << " isMuCGG" << endl; 
          //calculate the minimum eta
          CGGminEta = fabs(muEta[0]);
          CGGmaxEta = fabs(muEta[0]);
          CGGminPt  = fabs(muPt[0]);
          CGGmaxPt  = fabs(muPt[0]);
          //cout << fabs(muEta[0]) << " " << fabs(muEta[1]) << " " << fabs(muEta[2]) << endl;
          //cout << muPt[0] << " " << muPt[1] << " " << muPt[2] << endl;
          for (int j=1; j<3; j++)
          {
             if ( fabs(muEta[j]) < CGGminEta ) CGGminEta = fabs(muEta[j]);
             if ( fabs(muEta[j]) > CGGmaxEta ) CGGmaxEta = fabs(muEta[j]);
             if ( fabs(muPt[j])  < CGGminPt  ) CGGminPt  = fabs(muPt[j]);
             if ( fabs(muPt[j])  > CGGmaxPt  ) CGGmaxPt  = fabs(muPt[j]);
          }
          //cout << CGGminEta << " " << CGGmaxEta << endl;
          //cout << CGGminPt  << " " << CGGmaxPt  << endl;
          
          tree_CGG -> Fill();
                                                                                              
       }

       if ( isMuCGM )  
       { 
          cout << i << " isMuCGM" << endl; 
          //calculate the minimum eta
          CGMminEta = fabs(muEta[0]);
          CGMmaxEta = fabs(muEta[0]);
          CGMminPt  = fabs(muPt[0]);
          CGMmaxPt  = fabs(muPt[0]);
          //cout << fabs(muEta[0]) << " " << fabs(muEta[1]) << " " << fabs(muEta[2]) << endl;
          //cout << muPt[0] << " " << muPt[1] << " " << muPt[2] << endl;
          for (int j=1; j<3; j++)
          {
             if ( fabs(muEta[j]) < CGMminEta ) CGMminEta = fabs(muEta[j]);
             if ( fabs(muEta[j]) > CGMmaxEta ) CGMmaxEta = fabs(muEta[j]);
             if ( fabs(muPt[j])  < CGMminPt  ) CGMminPt  = fabs(muPt[j]);
             if ( fabs(muPt[j])  > CGMmaxPt  ) CGMmaxPt  = fabs(muPt[j]);
          }
          //cout << CGMminEta << " " << CGMmaxEta << endl;
          //cout << CGMminPt  << " " << CGMmaxPt  << endl;
          
          tree_CGM -> Fill();
                                                                                              
       }

       if ( isMuCMM )  
       { 
          cout << i << " isMuCMM" << endl; 
          //calculate the minimum eta
          CMMminEta = fabs(muEta[0]);
          CMMmaxEta = fabs(muEta[0]);
          CMMminPt  = fabs(muPt[0]);
          CMMmaxPt  = fabs(muPt[0]);
          //cout << fabs(muEta[0]) << " " << fabs(muEta[1]) << " " << fabs(muEta[2]) << endl;
          //cout << muPt[0] << " " << muPt[1] << " " << muPt[2] << endl;
          for (int j=1; j<3; j++)
          {
             if ( fabs(muEta[j]) < CMMminEta ) CMMminEta = fabs(muEta[j]);
             if ( fabs(muEta[j]) > CMMmaxEta ) CMMmaxEta = fabs(muEta[j]);
             if ( fabs(muPt[j])  < CMMminPt  ) CMMminPt  = fabs(muPt[j]);
             if ( fabs(muPt[j])  > CMMmaxPt  ) CMMmaxPt  = fabs(muPt[j]);
          }
          //cout << CMMminEta << " " << CMMmaxEta << endl;
          //cout << CMMminPt  << " " << CMMmaxPt  << endl;
          
          tree_CMM -> Fill();
                                                                                              
       }

       if ( isMuGGG )  
       { 
          cout << i << " isMuGGG" << endl; 
          //calculate the minimum eta
          GGGminEta = fabs(muEta[0]);
          GGGmaxEta = fabs(muEta[0]);
          GGGminPt  = fabs(muPt[0]);
          GGGmaxPt  = fabs(muPt[0]);
          //cout << fabs(muEta[0]) << " " << fabs(muEta[1]) << " " << fabs(muEta[2]) << endl;
          //cout << muPt[0] << " " << muPt[1] << " " << muPt[2] << endl;
          for (int j=1; j<3; j++)
          {
             if ( fabs(muEta[j]) < GGGminEta ) GGGminEta = fabs(muEta[j]);
             if ( fabs(muEta[j]) > GGGmaxEta ) GGGmaxEta = fabs(muEta[j]);
             if ( fabs(muPt[j])  < GGGminPt  ) GGGminPt  = fabs(muPt[j]);
             if ( fabs(muPt[j])  > GGGmaxPt  ) GGGmaxPt  = fabs(muPt[j]);
          }
          //cout << GGGminEta << " " << GGGmaxEta << endl;
          //cout << GGGminPt  << " " << GGGmaxPt  << endl;
          
          tree_GGG -> Fill();
                                                                                              
       }

       if ( isMuGGM )  
       { 
          cout << i << " isMuGGM" << endl; 
          //calculate the minimum eta
          GGMminEta = fabs(muEta[0]);
          GGMmaxEta = fabs(muEta[0]);
          GGMminPt  = fabs(muPt[0]);
          GGMmaxPt  = fabs(muPt[0]);
          //cout << fabs(muEta[0]) << " " << fabs(muEta[1]) << " " << fabs(muEta[2]) << endl;
          //cout << muPt[0] << " " << muPt[1] << " " << muPt[2] << endl;
          for (int j=1; j<3; j++)
          {
             if ( fabs(muEta[j]) < GGMminEta ) GGMminEta = fabs(muEta[j]);
             if ( fabs(muEta[j]) > GGMmaxEta ) GGMmaxEta = fabs(muEta[j]);
             if ( fabs(muPt[j])  < GGMminPt  ) GGMminPt  = fabs(muPt[j]);
             if ( fabs(muPt[j])  > GGMmaxPt  ) GGMmaxPt  = fabs(muPt[j]);
          }
          //cout << GGMminEta << " " << GGMmaxEta << endl;
          //cout << GGMminPt  << " " << GGMmaxPt  << endl;
          
          tree_GGM -> Fill();
                                                                                              
       }

       if ( isMuGMM )  
       { 
          cout << i << " isMuGMM" << endl; 
          //calculate the minimum eta
          GMMminEta = fabs(muEta[0]);
          GMMmaxEta = fabs(muEta[0]);
          GMMminPt  = fabs(muPt[0]);
          GMMmaxPt  = fabs(muPt[0]);
          //cout << fabs(muEta[0]) << " " << fabs(muEta[1]) << " " << fabs(muEta[2]) << endl;
          //cout << muPt[0] << " " << muPt[1] << " " << muPt[2] << endl;
          for (int j=1; j<3; j++)
          {
             if ( fabs(muEta[j]) < GMMminEta ) GMMminEta = fabs(muEta[j]);
             if ( fabs(muEta[j]) > GMMmaxEta ) GMMmaxEta = fabs(muEta[j]);
             if ( fabs(muPt[j])  < GMMminPt  ) GMMminPt  = fabs(muPt[j]);
             if ( fabs(muPt[j])  > GMMmaxPt  ) GMMmaxPt  = fabs(muPt[j]);
          }
          //cout << GMMminEta << " " << GMMmaxEta << endl;
          //cout << GMMminPt  << " " << GMMmaxPt  << endl;
          
          tree_GMM -> Fill();
                                                                                              
       }

       if ( isMuMMM )  
       { 
          cout << i << " isMuMMM" << endl; 
          //calculate the minimum eta
          MMMminEta = fabs(muEta[0]);
          MMMmaxEta = fabs(muEta[0]);
          MMMminPt  = fabs(muPt[0]);
          MMMmaxPt  = fabs(muPt[0]);
          //cout << fabs(muEta[0]) << " " << fabs(muEta[1]) << " " << fabs(muEta[2]) << endl;
          //cout << muPt[0] << " " << muPt[1] << " " << muPt[2] << endl;
          for (int j=1; j<3; j++)
          {
             if ( fabs(muEta[j]) < MMMminEta ) MMMminEta = fabs(muEta[j]);
             if ( fabs(muEta[j]) > MMMmaxEta ) MMMmaxEta = fabs(muEta[j]);
             if ( fabs(muPt[j])  < MMMminPt  ) MMMminPt  = fabs(muPt[j]);
             if ( fabs(muPt[j])  > MMMmaxPt  ) MMMmaxPt  = fabs(muPt[j]);
          }
          //cout << MMMminEta << " " << MMMmaxEta << endl;
          //cout << MMMminPt  << " " << MMMmaxPt  << endl;
          
          tree_MMM -> Fill();
                                                                                              
       }

    }


   //-------------------END LOOP ON EVENTS-------------------------------------
   //--------------------------------------------------------------------------
   //------------------START ACTIONS ON SUMMARY TREE---------------------------
 
   //calculate the rate
   DDDrate = nMuDDD_total/( lastEvent_total * 25.0 * 1e-9 ); //Hz
   DDRrate = nMuDDR_total/( lastEvent_total * 25.0 * 1e-9 ); //Hz
   DDCrate = nMuDDC_total/( lastEvent_total * 25.0 * 1e-9 ); //Hz
   DDGrate = nMuDDG_total/( lastEvent_total * 25.0 * 1e-9 ); //Hz
   DDMrate = nMuDDM_total/( lastEvent_total * 25.0 * 1e-9 ); //Hz
   DRRrate = nMuDRR_total/( lastEvent_total * 25.0 * 1e-9 ); //Hz
   DRCrate = nMuDRC_total/( lastEvent_total * 25.0 * 1e-9 ); //Hz
   DRGrate = nMuDRG_total/( lastEvent_total * 25.0 * 1e-9 ); //Hz
   DRMrate = nMuDRM_total/( lastEvent_total * 25.0 * 1e-9 ); //Hz
   DCCrate = nMuDCC_total/( lastEvent_total * 25.0 * 1e-9 ); //Hz
   DCGrate = nMuDCG_total/( lastEvent_total * 25.0 * 1e-9 ); //Hz
   DCMrate = nMuDCM_total/( lastEvent_total * 25.0 * 1e-9 ); //Hz
   DGGrate = nMuDGG_total/( lastEvent_total * 25.0 * 1e-9 ); //Hz
   DGMrate = nMuDGM_total/( lastEvent_total * 25.0 * 1e-9 ); //Hz
   DMMrate = nMuDMM_total/( lastEvent_total * 25.0 * 1e-9 ); //Hz
   

   RRRrate = nMuRRR_total/( lastEvent_total * 25.0 * 1e-9 ); //Hz
   RRCrate = nMuRRC_total/( lastEvent_total * 25.0 * 1e-9 ); //Hz
   RRGrate = nMuRRG_total/( lastEvent_total * 25.0 * 1e-9 ); //Hz
   RRMrate = nMuRRM_total/( lastEvent_total * 25.0 * 1e-9 ); //Hz
   RCCrate = nMuRCC_total/( lastEvent_total * 25.0 * 1e-9 ); //Hz
   RCGrate = nMuRCG_total/( lastEvent_total * 25.0 * 1e-9 ); //Hz
   RCMrate = nMuRCM_total/( lastEvent_total * 25.0 * 1e-9 ); //Hz
   RGGrate = nMuRGG_total/( lastEvent_total * 25.0 * 1e-9 ); //Hz
   RGMrate = nMuRGM_total/( lastEvent_total * 25.0 * 1e-9 ); //Hz
   RMMrate = nMuRMM_total/( lastEvent_total * 25.0 * 1e-9 ); //Hz
   CCCrate = nMuCCC_total/( lastEvent_total * 25.0 * 1e-9 ); //Hz
   CCGrate = nMuCCG_total/( lastEvent_total * 25.0 * 1e-9 ); //Hz
   CCMrate = nMuCCM_total/( lastEvent_total * 25.0 * 1e-9 ); //Hz
   CGGrate = nMuCGG_total/( lastEvent_total * 25.0 * 1e-9 ); //Hz
   CGMrate = nMuCGM_total/( lastEvent_total * 25.0 * 1e-9 ); //Hz
   CMMrate = nMuCMM_total/( lastEvent_total * 25.0 * 1e-9 ); //Hz
   GGGrate = nMuGGG_total/( lastEvent_total * 25.0 * 1e-9 ); //Hz
   GGMrate = nMuGGM_total/( lastEvent_total * 25.0 * 1e-9 ); //Hz
   GMMrate = nMuGMM_total/( lastEvent_total * 25.0 * 1e-9 ); //Hz
   MMMrate = nMuMMM_total/( lastEvent_total * 25.0 * 1e-9 ); //Hz


   //Fill summary tree
   tree_summary -> Fill();



    outfile->Write();   





/*   std::ofstream summaryFile ("summaryDigibased.txt");

   summaryFile << "" << std::endl;

   summaryFile.close();*/












}
