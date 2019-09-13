void ReaderME0_padbased()
{
   using namespace std;
  
   TFile *rfile = new TFile("out_padbased_both.root");
   //TFile *rfile = new TFile("out_DeltaGlobalPhiAnalyzer_1.root");
 
   //string dirName = "demo";
   //TDirectory demo = TDirectory(dirName.c_str(), dirName.c_str());

   TTree *tree_event_summary = (TTree*)rfile->Get("demo/EventSummary");

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

   Int_t nentries = (Int_t)tree_event_summary -> GetEntries();
  
   for ( Int_t i=0; i < nentries; i++ ) 
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
   tripleMuTopology->SetTitle("Tau->3 Mu Topology in CMS (Padbased)");

   for(Int_t k=0;k<numchains;k++){
   tripleMuTopology->GetHistogram()->GetXaxis()->SetBinLabel(k+1,names[k].c_str());   
   } 

   tripleMuTopology->SetMarkerStyle(21);
   tripleMuTopology->SetMarkerSize(0.8);
   tripleMuTopology->Draw("");

   //only events D R C G M (see all the three mu)
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
    tripleMuTopology_allThreeSeen->SetTitle("Tau->3 Mu Topology in CMS (all 3 muons seen)(Padbased)");
                                                                                                                    
    for(Int_t k=0;k<numchains_2;k++){
    tripleMuTopology_allThreeSeen->GetHistogram()->GetXaxis()->SetBinLabel(k+1,names_2[k].c_str());   
    } 
                                                                                                                    
    tripleMuTopology_allThreeSeen->SetMarkerStyle(21);
    tripleMuTopology_allThreeSeen->SetMarkerSize(0.8);
    tripleMuTopology_allThreeSeen->Draw("");


   c1->SaveAs("topology_padbased_all.png");
   c2->SaveAs("topology_padbased_onlyVisible.png");




}
