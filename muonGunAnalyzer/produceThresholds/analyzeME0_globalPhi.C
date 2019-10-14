#include "TApplication.h"
#include <stdlib.h>
#include <TGraphErrors.h>
#include <TH2.h>
#include <TH1D.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TFile.h>
#include "TString.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <TGraph.h>
#include <map>
#include <TTree.h>
#include <set>
#include <TMultiGraph.h>
#include <TPaveText.h>
#include <TLatex.h>
#include <TColor.h>
#include <TList.h>
#include <TMath.h>
#include <TF1.h>
#include <TPaveStats.h>


using namespace std;

Int_t trans_white = TColor::GetColorTransparent(0, 0);

TCanvas* canvasStyle(TString name, TString title)
{

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
  
  gStyle->SetOptTitle(0);

  TCanvas * c1 = new TCanvas(name, title, 50,50,W,H );    //canvas for 2 curves with tracking (one per gas)

 //cms aesthetics
  c1->SetFillColor(0);
  c1->SetBorderMode(0);
  c1->SetFrameFillStyle(0);
  c1->SetFrameBorderMode(0);
  c1->SetLeftMargin( L/W );
  c1->SetRightMargin( R/W );
  c1->SetTopMargin( T/H );
  c1->SetBottomMargin( B/H );
  c1->SetTickx(0);
  c1->SetTicky(0);
 //end cms aesthetics
  
  c1->SetGridy();
  //c1->SetGridx(5);


  return c1;
}

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

int analyzeME0_globalPhi(TString type, TString tag, TString version, TString folder)
{

// ========================================== USER OPTIONS ============================================
/* TString version = "11";
 TString type = "prompt";
 TString tag = "pad"; 
 TString energy = "3";
*/
 TString tag2 = tag;
 if (tag2 == "digi")	tag2 ="";
 //TString folder = "prova";

 if (folder=="") 
   {
   TDatime* time = new TDatime();
   TString folder = time->AsString();
   folder.ReplaceAll(" ","-");
   folder.ReplaceAll(":","");
   //string folderstr = "prova";
   //CreateDirectory(folderstr.c_str(), NULL)
   //auto dirname = fs::current_path() / __DATE__;
   //std::cout << "creating directory " << dirname << "\n";
   //  if (create_directories(dirname))	std::cout << "directory didn't exist yet\n";
   }

// ========================================= CANVAS ===================================================
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
  
  gStyle->SetOptTitle(0);

// =========================================== READ HISTS ===============================================
 gStyle->SetOptStat(111111);

 // histograms
 std::map<int,std::map<char,std::map<char,TH1F*>>> 	h_sg_deltaPhi; //h_sg_deltaPhi[energy][muCharge][endcap]
// std::map<int,std::map<char,std::map<char,TH1F*>>>	h_sg_alpha;
// std::map<int,std::map<char,std::map<char,TH1F*>>> 	h_sh_deltaPhi;
// std::map<int,std::map<char,std::map<char,TH1F*>>> 	h_sh_alpha;
 std::map<int,std::map<char,std::map<char,TH1*>>> 	h_cum_deltaPhi; //h_cum_deltaPhi[energy][muCharge][endcap]
// std::map<int,std::map<char,std::map<char,TH1*>>>	h_cum_alpha;

 // canvases
// std::map<int,std::map<char,std::map<char,TCanvas*>>>	c_compDeltaPhi;	  //h_compDeltaPhi[energy][muCharge][endcap]
// 									  //comparison of deltaPhi computed from simHits and segments
// std::map<int,std::map<char,std::map<char,TCanvas*>>>	c_compAlpha;	  //h_compAlpha[energy][muCharge][endcap]
// 									  //comparison of Alpha computed from simHits and segments
 std::map<char,std::map<char,TCanvas*>>			c_allDeltaPhi;	  //h_allAlpha[muCharge][endcap]
 									  //Alpha at all energies on same canvas computed from segments
// std::map<char,std::map<char,TCanvas*>>			c_allAlpha;	  //h_allAlpha[muCharge][endcap]
// 									  //Alpha at all energies on same canvas computed from segments
 std::map<char,std::map<char,TCanvas*>>			c_allDeltaPhiCum; //h_allAlpha[muCharge][endcap]
 									  //Alpha cumulatives at all energies on same canvas
// std::map<char,std::map<char,TCanvas*>>			c_allAlphaCum;	  //h_allAlpha[muCharge][endcap]
// 									  //Alpha cumulatives at all energies on same canvas
// std::map<int,std::map<char,std::map<char,TCanvas*>>>	c_alphaROC; 	  //h_alphaROC[energy][muCharge][endcap]
//								  //to define thresholds for 5,10,20,50 GeV compared with 3,5,10,20 GeV resp.
 std::map<int,std::map<char,std::map<char,TCanvas*>>>	c_deltaPhiROC;	  //h_deltaPhiROC[energy][muCharge][endcap]
 									  //Alpha cumulatives at all energies on same canvas
								  //to define thresholds for 5,10,20,50 GeV compared with 3,5,10,20 GeV resp.
 
 // TGraphErrors for ROC curves
 std::map<int,std::map<char,std::map<char,TGraphErrors*>>>	g_deltaPhiROC;	//g_deltaPhiROC[energy][muCharge][endcap]
// std::map<int,std::map<char,std::map<char,TGraphErrors*>>>	g_alphaROC;	//g_alphaROC[energy][muCharge][endcap]

 Double_t factor =40;

 int pt[] = {3,5,10,20,50};
 char sign[] = {'+','-'};
 for ( int i=0; i<5; i++ )
   {
   TString en;
   en.Form("%i",pt[i]);

   h_sg_deltaPhi[pt[i]]['+']['+'] = new TH1F("sg_deltaPhi_"+en+"GeV_++","sg_deltaPhi_"+en+"GeV_++",55*factor,-0.022,0.022);
//   h_sg_deltaPhi[pt[i]]['+']['-'] = new TH1F("sg_deltaPhi_"+en+"GeV_+-","sg_deltaPhi_"+en+"GeV_+-",factor*160,-0.3,0.5);
//   h_sg_deltaPhi[pt[i]]['-']['-'] = new TH1F("sg_deltaPhi_"+en+"GeV_--","sg_deltaPhi_"+en+"GeV_--",factor*160,-0.3,0.5);
//   h_sg_deltaPhi[pt[i]]['-']['+'] = new TH1F("sg_deltaPhi_"+en+"GeV_-+","sg_deltaPhi_"+en+"GeV_-+",factor*160,-0.3,0.5);
//
//   h_sg_alpha[pt[i]]['+']['+'] = new TH1F("sg_alpha_"+en+"GeV_++","sg_alpha_"+en+"GeV_++",240,-0.3,0.5);
//   h_sg_alpha[pt[i]]['+']['-'] = new TH1F("sg_alpha_"+en+"GeV_+-","sg_alpha_"+en+"GeV_+-",240,-0.3,0.5);
//   h_sg_alpha[pt[i]]['-']['-'] = new TH1F("sg_alpha_"+en+"GeV_--","sg_alpha_"+en+"GeV_--",240,-0.3,0.5);
//   h_sg_alpha[pt[i]]['-']['+'] = new TH1F("sg_alpha_"+en+"GeV_-+","sg_alpha_"+en+"GeV_-+",240,-0.3,0.5);
//
//   h_sh_deltaPhi[pt[i]]['+']['+'] = new TH1F("sh_deltaPhi_"+en+"GeV_++","sh_deltaPhi_"+en+"GeV_++",160,-0.3,0.5);
//   h_sh_deltaPhi[pt[i]]['+']['-'] = new TH1F("sh_deltaPhi_"+en+"GeV_+-","sh_deltaPhi_"+en+"GeV_+-",160,-0.3,0.5);
//   h_sh_deltaPhi[pt[i]]['-']['-'] = new TH1F("sh_deltaPhi_"+en+"GeV_--","sh_deltaPhi_"+en+"GeV_--",160,-0.3,0.5);
//   h_sh_deltaPhi[pt[i]]['-']['+'] = new TH1F("sh_deltaPhi_"+en+"GeV_-+","sh_deltaPhi_"+en+"GeV_-+",160,-0.3,0.5);
//
//   h_sh_alpha[pt[i]]['+']['+'] = new TH1F("sh_alpha_"+en+"GeV_++","sh_alpha_"+en+"GeV_++",240,-0.3,0.5);
//   h_sh_alpha[pt[i]]['+']['-'] = new TH1F("sh_alpha_"+en+"GeV_+-","sh_alpha_"+en+"GeV_+-",240,-0.3,0.5);
//   h_sh_alpha[pt[i]]['-']['-'] = new TH1F("sh_alpha_"+en+"GeV_--","sh_alpha_"+en+"GeV_--",240,-0.3,0.5);
//   h_sh_alpha[pt[i]]['-']['+'] = new TH1F("sh_alpha_"+en+"GeV_-+","sh_alpha_"+en+"GeV_-+",240,-0.3,0.5);
   }


 TString base;
 TString filename;  
 TChain *tc = new TChain("demo/Event");

 for ( int i=0; i<5; i++ )  //pt
  {
  base = "/afs/cern.ch/work/m/mressego/private/ME0trigger/CMSSW_9_3_3/src/samples/CRAB/crab_SEGM-ANA"+tag2+"/RESULTS/v"+version+"/";
  filename.Form(type+"Mu_%iGeV_segmAnalyzer_"+tag+"Based.root", pt[i]);
  tc->Add(base+filename);
  TString en;
  en.Form("%i",pt[i]);
  for ( int j=0; j<2; j++ )  //muSign
    {
    for ( int k=0; k<2; k++ )  //endcap
      {
      TString signMu = "pos";
      TString signEnd = "pos";
      if (sign[j]=='-')	{ signMu = "neg"; continue; }
      if (sign[k]=='-')	{ signEnd = "neg"; continue; }

      TString hist, command, cut;

      //segmentDeltaPhiLocal3
      hist;
      hist.Form("sg_deltaPhi_"+en+"GeV_%c%c",sign[j],sign[k]);
      command = "-segmentDeltaGlobalPhi_"+signMu+"Mu_"+signEnd+"End>>"+hist;
      cut = "segmentDeltaGlobalPhi_"+signMu+"Mu_"+signEnd+"End!=-7 && nSeg_"+signMu+"Mu_"+signEnd+"End==1 && nSegmentRecHits_"+signMu+"Mu_"+signEnd+"End==6";
      tc->Draw(command, cut);

    //  //segment_alphaLocal3
    //  hist;
    //  hist.Form("sg_alpha_"+en+"GeV_%c%c",sign[j],sign[k]);
    //  command = "segment_alphaLocal3_"+signMu+"Mu_"+signEnd+"End>>"+hist;
    //  cut = "segment_alphaLocal3_"+signMu+"Mu_"+signEnd+"End!=-4 && nSeg_"+signMu+"Mu_"+signEnd+"End==1 && nSegmentRecHits_"+signMu+"Mu_"+signEnd+"End==6";
    //  tc->Draw(command, cut);

    //  //simHitDeltaPhiLocal3
    //  hist;
    //  hist.Form("sh_deltaPhi_"+en+"GeV_%c%c",sign[j],sign[k]);
    //  command = "simHitDeltaPhiLocal3_"+signMu+"Mu_"+signEnd+"End>>"+hist;
    //  cut = "simHitDeltaPhiLocal3_"+signMu+"Mu_"+signEnd+"End!=-7";
    //  tc->Draw(command, cut);

    //  //simHit__alphaLocal3
    //  hist;
    //  hist.Form("sh_alpha_"+en+"GeV_%c%c",sign[j],sign[k]);
    //  command = "simHit_alphaLocal3_"+signMu+"Mu_"+signEnd+"End>>"+hist;
    //  cut = "simHit_alphaLocal3_"+signMu+"Mu_"+signEnd+"End!=-4";
    //  tc->Draw(command, cut);


      }
    }
  tc->Reset();
  }
 

//=============================================== NORMALIZE AND CUMULATIVE  =========================================================
Double_t deltaPhiInf[5][2][2];
Double_t deltaPhiSup[5][2][2];
Double_t binWidth[5][2][2];
Double_t fracLow95[5][2][2];
Double_t fracHigh95[5][2][2];
Double_t fracLow05[5][2][2];
Double_t fracHigh05[5][2][2];
Double_t dirt[5][2][2][5];

 for ( int i=0; i<5; i++ )  //pt
  {
  for ( int j=0; j<2; j++ )  //muSign
    {
    for ( int k=0; k<2; k++ )  //endcap
      {
      if (sign[j]=='-')	{ continue; }
      if (sign[k]=='-')	{ continue; }
      cout << "pt=" << pt[i] << "  muSign=" << sign[j] << "  endcap=" << sign[k] << endl;
      h_sg_deltaPhi[pt[i]][sign[j]][sign[k]]	->Scale( 1/h_sg_deltaPhi[pt[i]][sign[j]][sign[k]]->GetEntries() );
    //  h_sg_alpha[pt[i]][sign[j]][sign[k]]	->Scale( 1/h_sg_alpha[pt[i]][sign[j]][sign[k]]->GetEntries() );
    //  h_sh_deltaPhi[pt[i]][sign[j]][sign[k]]	->Scale( 1/h_sh_deltaPhi[pt[i]][sign[j]][sign[k]]->GetEntries() );
    //  h_sh_alpha[pt[i]][sign[j]][sign[k]]	->Scale( 1/h_sh_alpha[pt[i]][sign[j]][sign[k]]->GetEntries() );

      Bool_t forward = kTRUE;
      if (sign[j] != sign[k])	forward = kFALSE;
      h_cum_deltaPhi[pt[i]][sign[j]][sign[k]] 	= h_sg_deltaPhi[pt[i]][sign[j]][sign[k]]->GetCumulative(forward);
    //  h_cum_alpha[pt[i]][sign[j]][sign[k]] 	= h_sg_alpha[pt[i]][sign[j]][sign[k]]->GetCumulative(forward);

      h_sg_deltaPhi[pt[i]][sign[j]][sign[k]]	->SetLineWidth(3);
    //  h_sg_alpha[pt[i]][sign[j]][sign[k]]	->SetLineWidth(3);
    //  h_sh_deltaPhi[pt[i]][sign[j]][sign[k]]	->SetLineWidth(3);
    //  h_sh_alpha[pt[i]][sign[j]][sign[k]]	->SetLineWidth(3);
      h_cum_deltaPhi[pt[i]][sign[j]][sign[k]] 	->SetLineWidth(3);
    //  h_cum_alpha[pt[i]][sign[j]][sign[k]] 	->SetLineWidth(3);

      h_sg_deltaPhi[pt[i]][sign[j]][sign[k]]	->SetLineStyle(1);
    //  h_sg_alpha[pt[i]][sign[j]][sign[k]]	->SetLineStyle(1);
    //  h_sh_deltaPhi[pt[i]][sign[j]][sign[k]]	->SetLineStyle(1);
    //  h_sh_alpha[pt[i]][sign[j]][sign[k]]	->SetLineStyle(1);
      h_cum_deltaPhi[pt[i]][sign[j]][sign[k]] 	->SetLineStyle(1);
    //  h_cum_alpha[pt[i]][sign[j]][sign[k]] 	->SetLineStyle(1);
      }//endcap
    }//muSign
  }//pt

 for ( int i=0; i<5; i++ )  //pt
  {
  for ( int j=0; j<2; j++ )  //muSign
    {
    for ( int k=0; k<2; k++ )  //endcap
      {
      if (sign[j]=='-')	{ continue; }
      if (sign[k]=='-')	{ continue; }
      //GetNbinsX() returns the number of "real" bins, it doesn't count underflow and overflow bins
      binWidth[i][j][k] = h_cum_deltaPhi[pt[i]][sign[j]][sign[k]]->GetBinWidth(2);
      deltaPhiInf[i][j][k] = -100;
      deltaPhiSup[i][j][k] = -100;
      fracLow05[i][j][k] = 0;
      fracHigh05[i][j][k] = 0;
      fracLow95[i][j][k] = 0;
      fracHigh95[i][j][k] = 0;
      if ( j==k )
        {
        for ( int bin=1; bin<=h_cum_deltaPhi[pt[i]][sign[j]][sign[k]]->GetNbinsX() ; bin++ )	//fill thresholds deltaPhiInf,deltaPhiSup
          {
          Double_t val = h_cum_deltaPhi[pt[i]][sign[j]][sign[k]]->GetBinContent(bin);
          if ( val>0.05 && deltaPhiSup[i][j][k]==-100 )	
            {
            deltaPhiSup[i][j][k]= h_cum_deltaPhi[pt[i]][sign[j]][sign[k]]->GetBinLowEdge(bin);
    	    fracHigh05[i][j][k] = val;
    	    fracLow05[i][j][k] = h_cum_deltaPhi[pt[i]][sign[j]][sign[k]]->GetBinContent(bin-1);
            }
          if ( val>0.95 )				
            {
            deltaPhiInf[i][j][k] = h_cum_deltaPhi[pt[i]][sign[j]][sign[k]]->GetBinLowEdge(bin);
    	    fracHigh95[i][j][k] = val;
    	    fracLow95[i][j][k] = h_cum_deltaPhi[pt[i]][sign[j]][sign[k]]->GetBinContent(bin-1);
            for ( int l=0; l<5; l++ )  //get "dirt" from other pt values
              {
	      dirt[i][j][k][l] = h_cum_deltaPhi[pt[l]][sign[j]][sign[k]]->GetBinContent(bin-1);
	      }
            break;
            }
          }
        }//if(j==k) closed
       else
        {
        for ( int bin=h_cum_deltaPhi[pt[i]][sign[j]][sign[k]]->GetNbinsX() ; bin>0 ; bin-- )	//fill thresholds deltaPhiInf,deltaPhiSup
          {
          Double_t val = h_cum_deltaPhi[pt[i]][sign[j]][sign[k]]->GetBinContent(bin);
          if ( val>0.95 )	
            {
            deltaPhiInf[i][j][k]= (h_cum_deltaPhi[pt[i]][sign[j]][sign[k]]->GetBinLowEdge(bin)) + (h_cum_deltaPhi[pt[i]][sign[j]][sign[k]]->GetBinWidth(bin));
    	    fracHigh95[i][j][k] = val;
    	    fracLow95[i][j][k] = h_cum_deltaPhi[pt[i]][sign[j]][sign[k]]->GetBinContent(bin+1);
            for ( int l=0; l<5; l++ )  //get "dirt" from other pt values
              {
	      dirt[i][j][k][l] = h_cum_deltaPhi[pt[l]][sign[j]][sign[k]]->GetBinContent(bin+1);
	      }
    	    break;
            }
          if ( val>0.05 && deltaPhiSup[i][j][k]==-100 )				
            {
            deltaPhiSup[i][j][k] = (h_cum_deltaPhi[pt[i]][sign[j]][sign[k]]->GetBinLowEdge(bin)) + (h_cum_deltaPhi[pt[i]][sign[j]][sign[k]]->GetBinWidth(bin));
    	    fracHigh05[i][j][k] = val;
    	    fracLow05[i][j][k] = h_cum_deltaPhi[pt[i]][sign[j]][sign[k]]->GetBinContent(bin+1);
            }
          }
        }//if(j!=k) closed
      }//endcap
    }//muSign
  }//pt


  std::ofstream outfile;
  outfile.open(folder+"/Thresholds.txt");
  outfile << "Thresholds" << endl;
  outfile << "To select muons with pt>pt_inf, threshold to be applied are deltaPhi<deltaPhi_inf\t" << endl;
  outfile << "pt[GeV]\tmuSign\tendcap\tdeltaPhi_inf\tdeltaPhi_sup\tbinWidth\tfracLow95%\tfracHigh95%\tfracLow5%\tfracHigh5%" << endl;
 for ( int i=0; i<5; i++ )  //pt
  {
  for ( int j=0; j<2; j++ )  //muSign
    {
    for ( int k=0; k<2; k++ )  //endcap
      {
      if (sign[j]=='-')	{ continue; }
      if (sign[k]=='-')	{ continue; }
      outfile << pt[i] << "\t" << sign[j] << "\t" << sign[k] << "\t" << deltaPhiInf[i][j][k] << "\t" << deltaPhiSup[i][j][k] << "\t" << binWidth[i][j][k] << "\t" << fracLow95[i][j][k] << "\t" << fracHigh95[i][j][k] << "\t" << fracLow05[i][j][k] << "\t" << fracHigh05[i][j][k] << endl;
      }
    }
  }

  outfile << "\n\nFraction of muons with other pt also selected with the threshold pt_inf\n\n" << endl;
  outfile << "pt[GeV]\tmuSign\tendcap\tdirt3GeV\tdirt5GeV\tdirt10GeV\tdirt20GeV\tdirt50GeV" << endl;
 for ( int i=0; i<5; i++ )  //pt
  {
  for ( int j=0; j<2; j++ )  //muSign
    {
    for ( int k=0; k<2; k++ )  //endcap
      {
      if (sign[j]=='-')	{ continue; }
      if (sign[k]=='-')	{ continue; }
      outfile << pt[i] << "\t" << sign[j] << "\t" << sign[k] ;
      for ( int l=0; l<5; l++) outfile << "\t" << dirt[i][j][k][l] ;
      outfile << endl;
      }
    }
  }


 for ( int i=0; i<5; i++ )  //pt
  {
  for ( int j=0; j<2; j++ )  //muSign
    {
    for ( int k=0; k<2; k++ )  //endcap
      {
      if (sign[j]=='-')	{ continue; }
      if (sign[k]=='-')	{ continue; }
      h_cum_deltaPhi[pt[i]][sign[j]][sign[k]]->Rebin(factor);
      h_sg_deltaPhi[pt[i]][sign[j]][sign[k]]->Rebin(factor);
      }
    }
  }

//====================================================== Thresholds ================================================================
//use mu+ and pos endcap, i.e. j,k=0
//Double_t thr[] = deltaPhiInf[][0][0];
//Double_t frac3GeV[] = dirt[0][0][0][ ] ;
//Double_t frac5GeV[] = dirt[1][0][0][ ] ;
//Double_t frac10GeV[] = dirt[2][0][0][ ] ;
//Double_t frac20GeV[] = dirt[3][0][0][ ] ;
//Double_t frac50GeV[] = dirt[4][0][0][ ] ;

Double_t thr[5];
for ( int i=0; i<5; i++)	thr[i] = deltaPhiInf[i][0][0];
Double_t frac3GeV[5], frac5GeV[5], frac10GeV[5], frac20GeV[5], frac50GeV[5], err[5];
for ( int l=0; l<5; l++)	
  {
  frac3GeV[l] = dirt[0][0][0][l] ;
  frac5GeV[l] = dirt[1][0][0][l] ;
  frac10GeV[l] = dirt[2][0][0][l] ;
  frac20GeV[l] = dirt[3][0][0][l] ;
  frac50GeV[l] = dirt[4][0][0][l] ;
  err[l] = 0;
  }

TPaveText * prel = cmsPrel();
TCanvas * c_thr;
TString namethr = "DeltaPhiThresholds";
c_thr = canvasStyle(namethr,namethr);  

TGraphErrors * thr3GeV = new TGraphErrors( 5, thr , frac3GeV, err, err);
TGraphErrors * thr5GeV = new TGraphErrors( 5, thr , frac5GeV, err, err);
TGraphErrors * thr10GeV = new TGraphErrors( 5, thr , frac10GeV, err, err);
TGraphErrors * thr20GeV = new TGraphErrors( 5, thr , frac20GeV, err, err);
TGraphErrors * thr50GeV = new TGraphErrors( 5, thr , frac50GeV, err, err);

c_thr->cd();

	thr3GeV->GetYaxis()->SetRangeUser(0.5,1.0);

        thr3GeV->GetXaxis()->SetTitleFont(42);
        thr3GeV->GetXaxis()->SetTitleSize(0.06);
        thr3GeV->GetXaxis()->SetTitleOffset(0.9);
        thr3GeV->GetXaxis()->SetLabelOffset(0.007);
        thr3GeV->GetXaxis()->SetLabelSize(0.05);
        thr3GeV->GetXaxis()->SetDecimals(kTRUE);
        thr3GeV->GetXaxis()->SetTickLength(0.03);
        //thr3GeV->GetYaxis()->SetTitleColor(kRed);
        thr3GeV->GetYaxis()->SetTitleFont(42);
        thr3GeV->GetYaxis()->SetTitleSize(0.06);
        thr3GeV->GetYaxis()->SetTitleOffset(1.2);
        thr3GeV->GetYaxis()->SetLabelOffset(0.007);
        thr3GeV->GetYaxis()->SetLabelSize(0.05);
        //thr3GeV->GetYaxis()->SetLabelColor(kRed);
        thr3GeV->GetYaxis()->SetDecimals(kTRUE);
        thr3GeV->GetYaxis()->SetTickLength(0.03);
        thr3GeV->GetXaxis()->SetTitle("#Delta #phi_{thr}");
        thr3GeV->GetYaxis()->SetTitle("Selected fraction");
        //thr3GeV->GetYaxis()->SetRangeUser(0,1);
        //thr3GeV->GetYaxis()->SetLimits(0,1);
        //thr3GeV->GetXaxis()->SetLimits(1025/div,1600/div);
        // thr3GeV->Write(grTitle);


	thr3GeV->SetMarkerStyle(20);
        thr3GeV->SetLineColor(kOrange);     
        thr3GeV->SetMarkerColor(kOrange);     

	thr5GeV->SetMarkerStyle(21);
        thr5GeV->SetLineColor(kBlue);     
        thr5GeV->SetMarkerColor(kBlue);     

	thr10GeV->SetMarkerStyle(22);
        thr10GeV->SetLineColor(kRed);     
        thr10GeV->SetMarkerColor(kRed);     

	thr20GeV->SetMarkerStyle(23);
        thr20GeV->SetLineColor(kGreen);     
        thr20GeV->SetMarkerColor(kGreen);     

	thr50GeV->SetMarkerStyle(33);
        thr50GeV->SetLineColor(kMagenta);     
        thr50GeV->SetMarkerColor(kMagenta);     

	thr3GeV->Draw("AP");
	thr5GeV->Draw("SAMEP");
	thr10GeV->Draw("SAMEP");
	thr20GeV->Draw("SAMEP");
	thr50GeV->Draw("SAMEP");
      prel->Draw("NB");
      TLegend *legThr = new TLegend(0.20,0.235,0.42,0.485);
      legThr->SetTextSize(0.04);
      legThr->SetTextFont(42);
      legThr->AddEntry(thr3GeV, "3 GeV #mu", "p");
      legThr->AddEntry(thr5GeV, "5 GeV #mu", "p");
      legThr->AddEntry(thr10GeV, "10 GeV #mu", "p");
      legThr->AddEntry(thr20GeV, "20 GeV #mu", "p");
      legThr->AddEntry(thr50GeV, "50 GeV #mu", "p");
      legThr->Draw();

      c_thr->Update();
      c_thr->SaveAs(folder+"/"+namethr+".root");
      c_thr->SaveAs(folder+"/"+namethr+".C");
      c_thr->SaveAs(folder+"/"+namethr+".png");

//========================================================= ROC CURVES ===============================================================




//========================================= COMPARISON PLOTS segments/simhits ========================================================
/*
 for ( int i=0; i<5; i++ )  //pt
  {
  for ( int j=0; j<2; j++ )  //muSign
    {
    for ( int k=0; k<2; k++ )  //endcap
      {
      TString name;
      TPaveText * prel = cmsPrel();

      //============================== deltaPhi comparison ========================================
      name.Form("comparisonDeltaPhi_"+type+"Mu_"+tag+"Based_%iGeV_%c%c", pt[i], sign[j], sign[k]  );
      c_compDeltaPhi[pt[i]][sign[j]][sign[k]] = canvasStyle(name,name);
      
      h_sg_deltaPhi[pt[i]][sign[j]][sign[k]]->GetYaxis()->SetRangeUser(0,.25);
      h_sg_deltaPhi[pt[i]][sign[j]][sign[k]]->GetXaxis()->SetRangeUser(-0.3,0.3);
      
      h_sg_deltaPhi[pt[i]][sign[j]][sign[k]]->GetXaxis()->SetTitleFont(42);
      h_sg_deltaPhi[pt[i]][sign[j]][sign[k]]->GetXaxis()->SetTitleSize(0.06);
      h_sg_deltaPhi[pt[i]][sign[j]][sign[k]]->GetXaxis()->SetTitleOffset(0.9);
      h_sg_deltaPhi[pt[i]][sign[j]][sign[k]]->GetXaxis()->SetLabelOffset(0.007);
      h_sg_deltaPhi[pt[i]][sign[j]][sign[k]]->GetXaxis()->SetLabelSize(0.05);
      h_sg_deltaPhi[pt[i]][sign[j]][sign[k]]->GetXaxis()->SetDecimals(kTRUE);
      h_sg_deltaPhi[pt[i]][sign[j]][sign[k]]->GetXaxis()->SetTickLength(0.03);
      //h_sg_deltaPhi[pt[i]][sign[j]][sign[k]]->GetYaxis()->SetTitleColor(kRed);
      h_sg_deltaPhi[pt[i]][sign[j]][sign[k]]->GetYaxis()->SetTitleFont(42);
      h_sg_deltaPhi[pt[i]][sign[j]][sign[k]]->GetYaxis()->SetTitleSize(0.06);
      h_sg_deltaPhi[pt[i]][sign[j]][sign[k]]->GetYaxis()->SetTitleOffset(1.2);
      h_sg_deltaPhi[pt[i]][sign[j]][sign[k]]->GetYaxis()->SetLabelOffset(0.007);
      h_sg_deltaPhi[pt[i]][sign[j]][sign[k]]->GetYaxis()->SetLabelSize(0.05);
      //h_sg_deltaPhi[pt[i]][sign[j]][sign[k]]->GetYaxis()->SetLabelColor(kRed);
      h_sg_deltaPhi[pt[i]][sign[j]][sign[k]]->GetYaxis()->SetDecimals(kTRUE);
      h_sg_deltaPhi[pt[i]][sign[j]][sign[k]]->GetYaxis()->SetTickLength(0.03);
      h_sg_deltaPhi[pt[i]][sign[j]][sign[k]]->GetXaxis()->SetTitle("#Delta #Phi");
      h_sg_deltaPhi[pt[i]][sign[j]][sign[k]]->GetYaxis()->SetTitle("Entries");
      //h_sg_deltaPhi[pt[i]][sign[j]][sign[k]]->GetYaxis()->SetRangeUser(0,1);
      //h_sg_deltaPhi[pt[i]][sign[j]][sign[k]]->GetYaxis()->SetLimits(0,1);
      //h_sg_deltaPhi[pt[i]][sign[j]][sign[k]]->GetXaxis()->SetLimits(1025/div,1600/div);
     // h_sg_deltaPhi[pt[i]][sign[j]][sign[k]]->Write(grTitle);


      h_sg_deltaPhi[pt[i]][sign[j]][sign[k]]->SetLineColor(kRed);     
      h_sh_deltaPhi[pt[i]][sign[j]][sign[k]]->SetLineColor(kBlue);
      h_sg_deltaPhi[pt[i]][sign[j]][sign[k]]->Draw("HIST");     
      h_sh_deltaPhi[pt[i]][sign[j]][sign[k]]->Draw("HISTSAMES");

      c_compDeltaPhi[pt[i]][sign[j]][sign[k]]->Update(); //otherwise next commands don't work
      // default stats coordinates:
      // X1=.78  X2=.98
      // Y1=.775 Y2=0.935

      TString line1, line2;
      line1.Form("from Segment #mu^{%c} %i GeV", sign[j], pt[i]);
      line2.Form("from SimHit #mu^{%c} %i GeV", sign[j], pt[i]);
      TPaveStats *stDeltaphi = (TPaveStats*)h_sg_deltaPhi[pt[i]][sign[j]][sign[k]]->FindObject("stats");
      stDeltaphi->SetY1NDC(.775-.070); //new x start position
      stDeltaphi->SetY2NDC(.935-.070); //new x end position
      h_sg_deltaPhi[pt[i]][sign[j]][sign[k]]->SetName(line1);
      stDeltaphi = (TPaveStats*)h_sh_deltaPhi[pt[i]][sign[j]][sign[k]]->FindObject("stats");
      stDeltaphi->SetY1NDC(.755-.070); //new x start position
      stDeltaphi->SetY2NDC(.595-.070); //new x end position
      h_sh_deltaPhi[pt[i]][sign[j]][sign[k]]->SetName(line2);

      TLegend *legDeltaphi = new TLegend(0.18,0.765,0.6,0.865);
      legDeltaphi->SetTextSize(0.04);
      legDeltaphi->SetTextFont(42);
      legDeltaphi->AddEntry(h_sg_deltaPhi[pt[i]][sign[j]][sign[k]], line1, "l");
      legDeltaphi->AddEntry(h_sh_deltaPhi[pt[i]][sign[j]][sign[k]], line2, "l");
      legDeltaphi->Draw();

      prel->Draw("NB");
      c_compDeltaPhi[pt[i]][sign[j]][sign[k]]->Update();
      c_compDeltaPhi[pt[i]][sign[j]][sign[k]]->SaveAs(folder+"/"+name+".root");
      c_compDeltaPhi[pt[i]][sign[j]][sign[k]]->SaveAs(folder+"/"+name+".C");
      c_compDeltaPhi[pt[i]][sign[j]][sign[k]]->SaveAs(folder+"/"+name+".png");

      //=============================== alpha comparison ====================================================
      name.Form("comparisonAlpha_"+type+"Mu_"+tag+"Based_%iGeV_%c%c", pt[i], sign[j], sign[k]  );
      c_compAlpha[pt[i]][sign[j]][sign[k]] = canvasStyle(name,name);
      c_compAlpha[pt[i]][sign[j]][sign[k]]->cd();

      h_sg_alpha[pt[i]][sign[j]][sign[k]]->GetYaxis()->SetRangeUser(0,.25);
      h_sg_alpha[pt[i]][sign[j]][sign[k]]->GetXaxis()->SetRangeUser(-0.3,0.3);
      
      h_sg_alpha[pt[i]][sign[j]][sign[k]]->GetXaxis()->SetTitleFont(42);
      h_sg_alpha[pt[i]][sign[j]][sign[k]]->GetXaxis()->SetTitleSize(0.06);
      h_sg_alpha[pt[i]][sign[j]][sign[k]]->GetXaxis()->SetTitleOffset(0.9);
      h_sg_alpha[pt[i]][sign[j]][sign[k]]->GetXaxis()->SetLabelOffset(0.007);
      h_sg_alpha[pt[i]][sign[j]][sign[k]]->GetXaxis()->SetLabelSize(0.05);
      h_sg_alpha[pt[i]][sign[j]][sign[k]]->GetXaxis()->SetDecimals(kTRUE);
      h_sg_alpha[pt[i]][sign[j]][sign[k]]->GetXaxis()->SetTickLength(0.03);
      //h_sg_alpha[pt[i]][sign[j]][sign[k]]->GetYaxis()->SetTitleColor(kRed);
      h_sg_alpha[pt[i]][sign[j]][sign[k]]->GetYaxis()->SetTitleFont(42);
      h_sg_alpha[pt[i]][sign[j]][sign[k]]->GetYaxis()->SetTitleSize(0.06);
      h_sg_alpha[pt[i]][sign[j]][sign[k]]->GetYaxis()->SetTitleOffset(1.2);
      h_sg_alpha[pt[i]][sign[j]][sign[k]]->GetYaxis()->SetLabelOffset(0.007);
      h_sg_alpha[pt[i]][sign[j]][sign[k]]->GetYaxis()->SetLabelSize(0.05);
      //h_sg_alpha[pt[i]][sign[j]][sign[k]]->GetYaxis()->SetLabelColor(kRed);
      h_sg_alpha[pt[i]][sign[j]][sign[k]]->GetYaxis()->SetDecimals(kTRUE);
      h_sg_alpha[pt[i]][sign[j]][sign[k]]->GetYaxis()->SetTickLength(0.03);
      h_sg_alpha[pt[i]][sign[j]][sign[k]]->GetXaxis()->SetTitle("#alpha");
      h_sg_alpha[pt[i]][sign[j]][sign[k]]->GetYaxis()->SetTitle("Entries");
      //h_sg_alpha[pt[i]][sign[j]][sign[k]]->GetYaxis()->SetRangeUser(0,1);
      //h_sg_alpha[pt[i]][sign[j]][sign[k]]->GetYaxis()->SetLimits(0,1);
      //h_sg_alpha[pt[i]][sign[j]][sign[k]]->GetXaxis()->SetLimits(1025/div,1600/div);
     // h_sg_alpha[pt[i]][sign[j]][sign[k]]->Write(grTitle);


      h_sg_alpha[pt[i]][sign[j]][sign[k]]->SetLineColor(kRed);     
      h_sh_alpha[pt[i]][sign[j]][sign[k]]->SetLineColor(kBlue);
      h_sg_alpha[pt[i]][sign[j]][sign[k]]->Draw("HIST");     
      h_sh_alpha[pt[i]][sign[j]][sign[k]]->Draw("HISTSAMES");

      c_compAlpha[pt[i]][sign[j]][sign[k]]->Update(); //otherwise next commands don't work
      // default stats coordinates:
      // X1=.78  X2=.98
      // Y1=.775 Y2=0.935

      line1.Form("from Segment #mu^{%c} %i GeV", sign[j], pt[i]);
      line2.Form("from SimHit #mu^{%c} %i GeV", sign[j], pt[i]);
      TPaveStats *stAlpha = (TPaveStats*)h_sg_alpha[pt[i]][sign[j]][sign[k]]->FindObject("stats");
      stAlpha->SetY1NDC(.775-.070); //new x start position
      stAlpha->SetY2NDC(.935-.070); //new x end position
      h_sg_alpha[pt[i]][sign[j]][sign[k]]->SetName(line1);
      stAlpha = (TPaveStats*)h_sh_alpha[pt[i]][sign[j]][sign[k]]->FindObject("stats");
      stAlpha->SetY1NDC(.755-.070); //new x start position
      stAlpha->SetY2NDC(.595-.070); //new x end position
      h_sh_alpha[pt[i]][sign[j]][sign[k]]->SetName(line2);

      TLegend * legAlpha = new TLegend(0.18,0.765,0.6,0.865);
      legAlpha->SetTextSize(0.04);
      legAlpha->SetTextFont(42);
      legAlpha->AddEntry(h_sg_alpha[pt[i]][sign[j]][sign[k]], line1, "l");
      legAlpha->AddEntry(h_sh_alpha[pt[i]][sign[j]][sign[k]], line2, "l");
      legAlpha->Draw();

      prel->Draw("NB");
      c_compDeltaPhi[pt[i]][sign[j]][sign[k]]->Update();
      c_compDeltaPhi[pt[i]][sign[j]][sign[k]]->SaveAs(folder+"/"+name+".root");
      c_compDeltaPhi[pt[i]][sign[j]][sign[k]]->SaveAs(folder+"/"+name+".C");
      c_compDeltaPhi[pt[i]][sign[j]][sign[k]]->SaveAs(folder+"/"+name+".png");
      }
    }
  }
*/

//=============================== ALL plots at different energies on same canvas (alpha,deltaPhi,cumulatives) ===============================

// DELTA PHI
 for ( int j=0; j<2; j++ )  //muSign
  {
  for ( int k=0; k<2; k++ )  //endcap
    {
      if (sign[j]=='-')	{ continue; }
      if (sign[k]=='-')	{ continue; }
    TString nameDP;
    nameDP.Form("allDeltaPhi_"+type+"Mu_"+tag+"Based_%c%c", sign[j], sign[k]  );
    c_allDeltaPhi[sign[j]][sign[k]] = canvasStyle(nameDP,nameDP);

    TPaveText * prel = cmsPrel();
      
    h_sg_deltaPhi[pt[0]][sign[j]][sign[k]]->GetYaxis()->SetRangeUser(0,.30);
    h_sg_deltaPhi[pt[0]][sign[j]][sign[k]]->GetXaxis()->SetRangeUser(-0.3,0.3);
    
    h_sg_deltaPhi[pt[0]][sign[j]][sign[k]]->GetXaxis()->SetTitleFont(42);
    h_sg_deltaPhi[pt[0]][sign[j]][sign[k]]->GetXaxis()->SetTitleSize(0.06);
    h_sg_deltaPhi[pt[0]][sign[j]][sign[k]]->GetXaxis()->SetTitleOffset(0.9);
    h_sg_deltaPhi[pt[0]][sign[j]][sign[k]]->GetXaxis()->SetLabelOffset(0.007);
    h_sg_deltaPhi[pt[0]][sign[j]][sign[k]]->GetXaxis()->SetLabelSize(0.05);
    h_sg_deltaPhi[pt[0]][sign[j]][sign[k]]->GetXaxis()->SetDecimals(kTRUE);
    h_sg_deltaPhi[pt[0]][sign[j]][sign[k]]->GetXaxis()->SetTickLength(0.03);
    //h_sg_deltaPhi[pt[0]][sign[j]][sign[k]]->GetYaxis()->SetTitleColor(kRed);
    h_sg_deltaPhi[pt[0]][sign[j]][sign[k]]->GetYaxis()->SetTitleFont(42);
    h_sg_deltaPhi[pt[0]][sign[j]][sign[k]]->GetYaxis()->SetTitleSize(0.06);
    h_sg_deltaPhi[pt[0]][sign[j]][sign[k]]->GetYaxis()->SetTitleOffset(1.2);
    h_sg_deltaPhi[pt[0]][sign[j]][sign[k]]->GetYaxis()->SetLabelOffset(0.007);
    h_sg_deltaPhi[pt[0]][sign[j]][sign[k]]->GetYaxis()->SetLabelSize(0.05);
    //h_sg_deltaPhi[pt[0]][sign[j]][sign[k]]->GetYaxis()->SetLabelColor(kRed);
    h_sg_deltaPhi[pt[0]][sign[j]][sign[k]]->GetYaxis()->SetDecimals(kTRUE);
    h_sg_deltaPhi[pt[0]][sign[j]][sign[k]]->GetYaxis()->SetTickLength(0.03);
    h_sg_deltaPhi[pt[0]][sign[j]][sign[k]]->GetXaxis()->SetTitle("#Delta #Phi");
    h_sg_deltaPhi[pt[0]][sign[j]][sign[k]]->GetYaxis()->SetTitle("Entries");
    //h_sg_deltaPhi[pt[0]][sign[j]][sign[k]]->GetYaxis()->SetRangeUser(0,1);
    //h_sg_deltaPhi[pt[0]][sign[j]][sign[k]]->GetYaxis()->SetLimits(0,1);
    //h_sg_deltaPhi[pt[0]][sign[j]][sign[k]]->GetXaxis()->SetLimits(1025/div,1600/div);
    // h_sg_deltaPhi[pt[0]][sign[j]][sign[k]]->Write(grTitle);

    h_sg_deltaPhi[pt[0]][sign[j]][sign[k]]->SetLineColor(kOrange);
    h_sg_deltaPhi[pt[1]][sign[j]][sign[k]]->SetLineColor(kBlue);
    h_sg_deltaPhi[pt[2]][sign[j]][sign[k]]->SetLineColor(kRed);
    h_sg_deltaPhi[pt[3]][sign[j]][sign[k]]->SetLineColor(kGreen);
    h_sg_deltaPhi[pt[4]][sign[j]][sign[k]]->SetLineColor(kMagenta);

    h_sg_deltaPhi[pt[0]][sign[j]][sign[k]]->Draw("HIST");     

    c_allDeltaPhi[sign[j]][sign[k]]->Update(); //otherwise next commands don't work

    TString nameStats;
    nameStats.Form(type+" #mu^{%c} %i GeV", sign[j], pt[0]);
    TPaveStats *st1 = (TPaveStats*)h_sg_deltaPhi[pt[0]][sign[j]][sign[k]]->FindObject("stats");
    Float_t Y1NDC = .985; //new x start position
    Float_t Y2NDC = .825; //new x start position
    st1->SetY1NDC(Y1NDC);
    st1->SetY2NDC(Y2NDC);
    h_sg_deltaPhi[pt[0]][sign[j]][sign[k]]->SetName(nameStats);

    TLegend *leg = new TLegend(0.18,0.685,0.49,0.865);
    leg->SetTextSize(0.04);
    leg->SetTextFont(42);
    TString legEntry;
    legEntry.Form(type+" #mu^{%c} %i GeV", sign[j], pt[0]);
    leg->AddEntry(h_sg_deltaPhi[pt[0]][sign[j]][sign[k]], legEntry, "l");
    for ( int i=0; i<5; i++ )  //pt
      {
      h_sg_deltaPhi[pt[i]][sign[j]][sign[k]]->SetLineWidth(3);
      h_sg_deltaPhi[pt[i]][sign[j]][sign[k]]->SetLineStyle(1);

      if (i==0) continue;
      h_sg_deltaPhi[pt[i]][sign[j]][sign[k]]->Draw("HISTSAMES");

      c_allDeltaPhi[sign[j]][sign[k]]->Update(); //otherwise next commands don't work

      nameStats.Form(type+" #mu^{%c} %i GeV", sign[j], pt[i]);
      st1 = (TPaveStats*)h_sg_deltaPhi[pt[i]][sign[j]][sign[k]]->FindObject("stats");
      Y1NDC -= .165; //new x start position
      Y2NDC -= .165; //new y start position
      st1->SetY1NDC(Y1NDC);
      st1->SetY2NDC(Y2NDC);
      TPaveStats *st1 = (TPaveStats*)h_sg_deltaPhi[pt[i]][sign[j]][sign[k]]->FindObject("stats");
      h_sg_deltaPhi[pt[i]][sign[j]][sign[k]]->SetName(nameStats);
      legEntry.Form(type+" #mu^{%c} %i GeV", sign[j], pt[i]);
      leg->AddEntry(h_sg_deltaPhi[pt[i]][sign[j]][sign[k]], legEntry, "l");
      }
    leg->Draw();
    // default stats coordinates:
    // X1=.78  X2=.98
    // Y1=.775 Y2=0.935

    prel->Draw("NB");
    c_allDeltaPhi[sign[j]][sign[k]]->Update();
    c_allDeltaPhi[sign[j]][sign[k]]->SaveAs(folder+"/"+nameDP+".root");
    c_allDeltaPhi[sign[j]][sign[k]]->SaveAs(folder+"/"+nameDP+".C");
    c_allDeltaPhi[sign[j]][sign[k]]->SaveAs(folder+"/"+nameDP+".png");

    }
  }
  
/*
// ALPHA
 for ( int j=0; j<2; j++ )  //muSign
  {
  for ( int k=0; k<2; k++ )  //endcap
    {
    TString nameA;
    nameA.Form("allAlpha_"+type+"Mu_"+tag+"Based_%c%c", sign[j], sign[k]  );
    c_allAlpha[sign[j]][sign[k]] = canvasStyle(nameA,nameA);

    TPaveText * prel = cmsPrel();
      
    h_sg_alpha[pt[0]][sign[j]][sign[k]]->GetYaxis()->SetRangeUser(0,.65);
    h_sg_alpha[pt[0]][sign[j]][sign[k]]->GetXaxis()->SetRangeUser(-0.3,0.3);
    
    h_sg_alpha[pt[0]][sign[j]][sign[k]]->GetXaxis()->SetTitleFont(42);
    h_sg_alpha[pt[0]][sign[j]][sign[k]]->GetXaxis()->SetTitleSize(0.06);
    h_sg_alpha[pt[0]][sign[j]][sign[k]]->GetXaxis()->SetTitleOffset(0.9);
    h_sg_alpha[pt[0]][sign[j]][sign[k]]->GetXaxis()->SetLabelOffset(0.007);
    h_sg_alpha[pt[0]][sign[j]][sign[k]]->GetXaxis()->SetLabelSize(0.05);
    h_sg_alpha[pt[0]][sign[j]][sign[k]]->GetXaxis()->SetDecimals(kTRUE);
    h_sg_alpha[pt[0]][sign[j]][sign[k]]->GetXaxis()->SetTickLength(0.03);
    //h_sg_alpha[pt[0]][sign[j]][sign[k]]->GetYaxis()->SetTitleColor(kRed);
    h_sg_alpha[pt[0]][sign[j]][sign[k]]->GetYaxis()->SetTitleFont(42);
    h_sg_alpha[pt[0]][sign[j]][sign[k]]->GetYaxis()->SetTitleSize(0.06);
    h_sg_alpha[pt[0]][sign[j]][sign[k]]->GetYaxis()->SetTitleOffset(1.2);
    h_sg_alpha[pt[0]][sign[j]][sign[k]]->GetYaxis()->SetLabelOffset(0.007);
    h_sg_alpha[pt[0]][sign[j]][sign[k]]->GetYaxis()->SetLabelSize(0.05);
    //h_sg_alpha[pt[0]][sign[j]][sign[k]]->GetYaxis()->SetLabelColor(kRed);
    h_sg_alpha[pt[0]][sign[j]][sign[k]]->GetYaxis()->SetDecimals(kTRUE);
    h_sg_alpha[pt[0]][sign[j]][sign[k]]->GetYaxis()->SetTickLength(0.03);
    h_sg_alpha[pt[0]][sign[j]][sign[k]]->GetXaxis()->SetTitle("#alpha");
    h_sg_alpha[pt[0]][sign[j]][sign[k]]->GetYaxis()->SetTitle("Entries");
    //h_sg_alpha[pt[0]][sign[j]][sign[k]]->GetYaxis()->SetRangeUser(0,1);
    //h_sg_alpha[pt[0]][sign[j]][sign[k]]->GetYaxis()->SetLimits(0,1);
    //h_sg_alpha[pt[0]][sign[j]][sign[k]]->GetXaxis()->SetLimits(1025/div,1600/div);
    // h_sg_alpha[pt[0]][sign[j]][sign[k]]->Write(grTitle);

    h_sg_alpha[pt[0]][sign[j]][sign[k]]->SetLineColor(kOrange);
    h_sg_alpha[pt[1]][sign[j]][sign[k]]->SetLineColor(kBlue);
    h_sg_alpha[pt[2]][sign[j]][sign[k]]->SetLineColor(kRed);
    h_sg_alpha[pt[3]][sign[j]][sign[k]]->SetLineColor(kGreen);
    h_sg_alpha[pt[4]][sign[j]][sign[k]]->SetLineColor(kMagenta);

    h_sg_alpha[pt[0]][sign[j]][sign[k]]->Draw("HIST");     

    c_allAlpha[sign[j]][sign[k]]->Update(); //otherwise next commands don't work

    TString nameStats;
    nameStats.Form(type+" #mu^{%c} %i GeV", sign[j], pt[0]);
    TPaveStats *st1 = (TPaveStats*)h_sg_alpha[pt[0]][sign[j]][sign[k]]->FindObject("stats");
    Float_t Y1NDC = .985; //new x start position
    Float_t Y2NDC = .825; //new x start position
    st1->SetY1NDC(Y1NDC);
    st1->SetY2NDC(Y2NDC);
    h_sg_alpha[pt[0]][sign[j]][sign[k]]->SetName(nameStats);

    TLegend *leg = new TLegend(0.18,0.685,0.49,0.865);
    leg->SetTextSize(0.04);
    leg->SetTextFont(42);
    TString legEntry;
    legEntry.Form(type+" #mu^{%c} %i GeV", sign[j], pt[0]);
    leg->AddEntry(h_sg_alpha[pt[0]][sign[j]][sign[k]], legEntry, "l");
    for ( int i=0; i<5; i++ )  //pt
      {
      h_sg_alpha[pt[i]][sign[j]][sign[k]]->SetLineWidth(3);
      h_sg_alpha[pt[i]][sign[j]][sign[k]]->SetLineStyle(1);

      if (i==0) continue;
      h_sg_alpha[pt[i]][sign[j]][sign[k]]->Draw("HISTSAMES");

      c_allAlpha[sign[j]][sign[k]]->Update(); //otherwise next commands don't work

      nameStats.Form(type+" #mu^{%c} %i GeV", sign[j], pt[i]);
      st1 = (TPaveStats*)h_sg_alpha[pt[i]][sign[j]][sign[k]]->FindObject("stats");
      Y1NDC -= .165; //new x start position
      Y2NDC -= .165; //new y start position
      st1->SetY1NDC(Y1NDC);
      st1->SetY2NDC(Y2NDC);
      TPaveStats *st1 = (TPaveStats*)h_sg_alpha[pt[i]][sign[j]][sign[k]]->FindObject("stats");
      h_sg_alpha[pt[i]][sign[j]][sign[k]]->SetName(nameStats);
      legEntry.Form(type+" #mu^{%c} %i GeV", sign[j], pt[i]);
      leg->AddEntry(h_sg_alpha[pt[i]][sign[j]][sign[k]], legEntry, "l");
      }
    leg->Draw();
    // default stats coordinates:
    // X1=.78  X2=.98
    // Y1=.775 Y2=0.935

    prel->Draw("NB");
    c_allAlpha[sign[j]][sign[k]]->Update();
    c_allAlpha[sign[j]][sign[k]]->SaveAs(folder+"/"+nameA+".root");
    c_allAlpha[sign[j]][sign[k]]->SaveAs(folder+"/"+nameA+".C");
    c_allAlpha[sign[j]][sign[k]]->SaveAs(folder+"/"+nameA+".png");

    }
  }
*/

// DELTA PHI CUMULATIVE
 for ( int j=0; j<2; j++ )  //muSign
  {
  for ( int k=0; k<2; k++ )  //endcap
    {
      if (sign[j]=='-')	{ continue; }
      if (sign[k]=='-')	{ continue; }
    TString nameDPC;
    nameDPC.Form("allDeltaPhiCum_"+type+"Mu_"+tag+"Based_%c%c", sign[j], sign[k]  );
    c_allDeltaPhiCum[sign[j]][sign[k]] = canvasStyle(nameDPC,nameDPC);

    TPaveText * prel = cmsPrel();
      
    h_cum_deltaPhi[pt[0]][sign[j]][sign[k]]->GetYaxis()->SetRangeUser(0,1.05);
    h_cum_deltaPhi[pt[0]][sign[j]][sign[k]]->GetXaxis()->SetRangeUser(-0.2,0.2);
    
    h_cum_deltaPhi[pt[0]][sign[j]][sign[k]]->GetXaxis()->SetTitleFont(42);
    h_cum_deltaPhi[pt[0]][sign[j]][sign[k]]->GetXaxis()->SetTitleSize(0.06);
    h_cum_deltaPhi[pt[0]][sign[j]][sign[k]]->GetXaxis()->SetTitleOffset(0.9);
    h_cum_deltaPhi[pt[0]][sign[j]][sign[k]]->GetXaxis()->SetLabelOffset(0.007);
    h_cum_deltaPhi[pt[0]][sign[j]][sign[k]]->GetXaxis()->SetLabelSize(0.05);
    h_cum_deltaPhi[pt[0]][sign[j]][sign[k]]->GetXaxis()->SetDecimals(kTRUE);
    h_cum_deltaPhi[pt[0]][sign[j]][sign[k]]->GetXaxis()->SetTickLength(0.03);
    //h_cum_deltaPhi[pt[0]][sign[j]][sign[k]]->GetYaxis()->SetTitleColor(kRed);
    h_cum_deltaPhi[pt[0]][sign[j]][sign[k]]->GetYaxis()->SetTitleFont(42);
    h_cum_deltaPhi[pt[0]][sign[j]][sign[k]]->GetYaxis()->SetTitleSize(0.06);
    h_cum_deltaPhi[pt[0]][sign[j]][sign[k]]->GetYaxis()->SetTitleOffset(1.2);
    h_cum_deltaPhi[pt[0]][sign[j]][sign[k]]->GetYaxis()->SetLabelOffset(0.007);
    h_cum_deltaPhi[pt[0]][sign[j]][sign[k]]->GetYaxis()->SetLabelSize(0.05);
    //h_cum_deltaPhi[pt[0]][sign[j]][sign[k]]->GetYaxis()->SetLabelColor(kRed);
    h_cum_deltaPhi[pt[0]][sign[j]][sign[k]]->GetYaxis()->SetDecimals(kTRUE);
    h_cum_deltaPhi[pt[0]][sign[j]][sign[k]]->GetYaxis()->SetTickLength(0.03);
    h_cum_deltaPhi[pt[0]][sign[j]][sign[k]]->GetXaxis()->SetTitle("#Delta #Phi");
    h_cum_deltaPhi[pt[0]][sign[j]][sign[k]]->GetYaxis()->SetTitle("Entries");
    //h_cum_deltaPhi[pt[0]][sign[j]][sign[k]]->GetYaxis()->SetRangeUser(0,1);
    //h_cum_deltaPhi[pt[0]][sign[j]][sign[k]]->GetYaxis()->SetLimits(0,1);
    //h_cum_deltaPhi[pt[0]][sign[j]][sign[k]]->GetXaxis()->SetLimits(1025/div,1600/div);
    // h_cum_deltaPhi[pt[0]][sign[j]][sign[k]]->Write(grTitle);

    h_cum_deltaPhi[pt[0]][sign[j]][sign[k]]->SetLineColor(kOrange);
    h_cum_deltaPhi[pt[1]][sign[j]][sign[k]]->SetLineColor(kBlue);
    h_cum_deltaPhi[pt[2]][sign[j]][sign[k]]->SetLineColor(kRed);
    h_cum_deltaPhi[pt[3]][sign[j]][sign[k]]->SetLineColor(kGreen);
    h_cum_deltaPhi[pt[4]][sign[j]][sign[k]]->SetLineColor(kMagenta);

    h_cum_deltaPhi[pt[0]][sign[j]][sign[k]]->Draw("HIST");     

    c_allDeltaPhiCum[sign[j]][sign[k]]->Update(); //otherwise next commands don't work

    TString nameStats;
    nameStats.Form(type+" #mu^{%c} %i GeV", sign[j], pt[0]);
    TPaveStats *st1 = (TPaveStats*)h_cum_deltaPhi[pt[0]][sign[j]][sign[k]]->FindObject("stats");
    Float_t Y1NDC = .985; //new x start position
    Float_t Y2NDC = .825; //new x start position
    st1->SetY1NDC(Y1NDC);
    st1->SetY2NDC(Y2NDC);
    h_cum_deltaPhi[pt[0]][sign[j]][sign[k]]->SetName(nameStats);

    TLegend *leg = new TLegend(0.18,0.685,0.49,0.865);
    leg->SetTextSize(0.04);
    leg->SetTextFont(42);
    TString legEntry;
    legEntry.Form(type+" #mu^{%c} %i GeV", sign[j], pt[0]);
    leg->AddEntry(h_cum_deltaPhi[pt[0]][sign[j]][sign[k]], legEntry, "l");
    for ( int i=0; i<5; i++ )  //pt
      {
      h_cum_deltaPhi[pt[i]][sign[j]][sign[k]]->SetLineWidth(3);
      h_cum_deltaPhi[pt[i]][sign[j]][sign[k]]->SetLineStyle(1);

      if (i==0) continue;
      h_cum_deltaPhi[pt[i]][sign[j]][sign[k]]->Draw("HISTSAMES");

      c_allDeltaPhiCum[sign[j]][sign[k]]->Update(); //otherwise next commands don't work

      nameStats.Form(type+" #mu^{%c} %i GeV", sign[j], pt[i]);
      st1 = (TPaveStats*)h_cum_deltaPhi[pt[i]][sign[j]][sign[k]]->FindObject("stats");
      Y1NDC -= .165; //new x start position
      Y2NDC -= .165; //new y start position
      st1->SetY1NDC(Y1NDC);
      st1->SetY2NDC(Y2NDC);
      TPaveStats *st1 = (TPaveStats*)h_cum_deltaPhi[pt[i]][sign[j]][sign[k]]->FindObject("stats");
      h_cum_deltaPhi[pt[i]][sign[j]][sign[k]]->SetName(nameStats);
      legEntry.Form(type+" #mu^{%c} %i GeV", sign[j], pt[i]);
      leg->AddEntry(h_cum_deltaPhi[pt[i]][sign[j]][sign[k]], legEntry, "l");
      }
    leg->Draw();
    // default stats coordinates:
    // X1=.78  X2=.98
    // Y1=.775 Y2=0.935

    prel->Draw("NB");
    c_allDeltaPhiCum[sign[j]][sign[k]]->Update();
    c_allDeltaPhiCum[sign[j]][sign[k]]->SaveAs(folder+"/"+nameDPC+".root");
    c_allDeltaPhiCum[sign[j]][sign[k]]->SaveAs(folder+"/"+nameDPC+".C");
    c_allDeltaPhiCum[sign[j]][sign[k]]->SaveAs(folder+"/"+nameDPC+".png");

    }
  }

/*
// ALPHA CUMULATIVE
 for ( int j=0; j<2; j++ )  //muSign
  {
  for ( int k=0; k<2; k++ )  //endcap
    {
    TString nameAC;
    nameAC.Form("allAlphaCum_"+type+"Mu_"+tag+"Based_%c%c", sign[j], sign[k]  );
    c_allAlphaCum[sign[j]][sign[k]] = canvasStyle(nameAC,nameAC);

    TPaveText * prel = cmsPrel();
      
    h_cum_deltaPhi[pt[0]][sign[j]][sign[k]]->GetYaxis()->SetRangeUser(0,1.05);
    h_cum_deltaPhi[pt[0]][sign[j]][sign[k]]->GetXaxis()->SetRangeUser(-0.2,0.2);
    
    h_cum_deltaPhi[pt[0]][sign[j]][sign[k]]->GetXaxis()->SetTitleFont(42);
    h_cum_deltaPhi[pt[0]][sign[j]][sign[k]]->GetXaxis()->SetTitleSize(0.06);
    h_cum_deltaPhi[pt[0]][sign[j]][sign[k]]->GetXaxis()->SetTitleOffset(0.9);
    h_cum_deltaPhi[pt[0]][sign[j]][sign[k]]->GetXaxis()->SetLabelOffset(0.007);
    h_cum_deltaPhi[pt[0]][sign[j]][sign[k]]->GetXaxis()->SetLabelSize(0.05);
    h_cum_deltaPhi[pt[0]][sign[j]][sign[k]]->GetXaxis()->SetDecimals(kTRUE);
    h_cum_deltaPhi[pt[0]][sign[j]][sign[k]]->GetXaxis()->SetTickLength(0.03);
    //h_cum_deltaPhi[pt[0]][sign[j]][sign[k]]->GetYaxis()->SetTitleColor(kRed);
    h_cum_deltaPhi[pt[0]][sign[j]][sign[k]]->GetYaxis()->SetTitleFont(42);
    h_cum_deltaPhi[pt[0]][sign[j]][sign[k]]->GetYaxis()->SetTitleSize(0.06);
    h_cum_deltaPhi[pt[0]][sign[j]][sign[k]]->GetYaxis()->SetTitleOffset(1.2);
    h_cum_deltaPhi[pt[0]][sign[j]][sign[k]]->GetYaxis()->SetLabelOffset(0.007);
    h_cum_deltaPhi[pt[0]][sign[j]][sign[k]]->GetYaxis()->SetLabelSize(0.05);
    //h_cum_deltaPhi[pt[0]][sign[j]][sign[k]]->GetYaxis()->SetLabelColor(kRed);
    h_cum_deltaPhi[pt[0]][sign[j]][sign[k]]->GetYaxis()->SetDecimals(kTRUE);
    h_cum_deltaPhi[pt[0]][sign[j]][sign[k]]->GetYaxis()->SetTickLength(0.03);
    h_cum_deltaPhi[pt[0]][sign[j]][sign[k]]->GetXaxis()->SetTitle("#alpha");
    h_cum_deltaPhi[pt[0]][sign[j]][sign[k]]->GetYaxis()->SetTitle("Entries");
    //h_cum_deltaPhi[pt[0]][sign[j]][sign[k]]->GetYaxis()->SetRangeUser(0,1);
    //h_cum_deltaPhi[pt[0]][sign[j]][sign[k]]->GetYaxis()->SetLimits(0,1);
    //h_cum_deltaPhi[pt[0]][sign[j]][sign[k]]->GetXaxis()->SetLimits(1025/div,1600/div);
    // h_cum_deltaPhi[pt[0]][sign[j]][sign[k]]->Write(grTitle);

    h_cum_deltaPhi[pt[0]][sign[j]][sign[k]]->SetLineColor(kOrange);
    h_cum_deltaPhi[pt[1]][sign[j]][sign[k]]->SetLineColor(kBlue);
    h_cum_deltaPhi[pt[2]][sign[j]][sign[k]]->SetLineColor(kRed);
    h_cum_deltaPhi[pt[3]][sign[j]][sign[k]]->SetLineColor(kGreen);
    h_cum_deltaPhi[pt[4]][sign[j]][sign[k]]->SetLineColor(kMagenta);

    h_cum_deltaPhi[pt[0]][sign[j]][sign[k]]->Draw("HIST");     

    c_allAlphaCum[sign[j]][sign[k]]->Update(); //otherwise next commands don't work

    TString nameStats;
    nameStats.Form(type+" #mu^{%c} %i GeV", sign[j], pt[0]);
    TPaveStats *st1 = (TPaveStats*)h_cum_deltaPhi[pt[0]][sign[j]][sign[k]]->FindObject("stats");
    Float_t Y1NDC = .985; //new x start position
    Float_t Y2NDC = .825; //new x start position
    st1->SetY1NDC(Y1NDC);
    st1->SetY2NDC(Y2NDC);
    h_cum_deltaPhi[pt[0]][sign[j]][sign[k]]->SetName(nameStats);

    TLegend *leg = new TLegend(0.18,0.685,0.49,0.865);
    leg->SetTextSize(0.04);
    leg->SetTextFont(42);
    TString legEntry;
    legEntry.Form(type+" #mu^{%c} %i GeV", sign[j], pt[0]);
    leg->AddEntry(h_cum_deltaPhi[pt[0]][sign[j]][sign[k]], legEntry, "l");
    for ( int i=0; i<5; i++ )  //pt
      {
      h_cum_deltaPhi[pt[i]][sign[j]][sign[k]]->SetLineWidth(3);
      h_cum_deltaPhi[pt[i]][sign[j]][sign[k]]->SetLineStyle(1);

      if (i==0) continue;
      h_cum_deltaPhi[pt[i]][sign[j]][sign[k]]->Draw("HISTSAMES");

      c_allAlphaCum[sign[j]][sign[k]]->Update(); //otherwise next commands don't work

      nameStats.Form(type+" #mu^{%c} %i GeV", sign[j], pt[i]);
      st1 = (TPaveStats*)h_cum_deltaPhi[pt[i]][sign[j]][sign[k]]->FindObject("stats");
      Y1NDC -= .165; //new x start position
      Y2NDC -= .165; //new y start position
      st1->SetY1NDC(Y1NDC);
      st1->SetY2NDC(Y2NDC);
      TPaveStats *st1 = (TPaveStats*)h_cum_deltaPhi[pt[i]][sign[j]][sign[k]]->FindObject("stats");
      h_cum_deltaPhi[pt[i]][sign[j]][sign[k]]->SetName(nameStats);
      legEntry.Form(type+" #mu^{%c} %i GeV", sign[j], pt[i]);
      leg->AddEntry(h_cum_deltaPhi[pt[i]][sign[j]][sign[k]], legEntry, "l");
      }
    leg->Draw();
    // default stats coordinates:
    // X1=.78  X2=.98
    // Y1=.775 Y2=0.935

    prel->Draw("NB");
    c_allAlphaCum[sign[j]][sign[k]]->Update();
    c_allAlphaCum[sign[j]][sign[k]]->SaveAs(folder+"/"+nameAC+".root");
    c_allAlphaCum[sign[j]][sign[k]]->SaveAs(folder+"/"+nameAC+".C");
    c_allAlphaCum[sign[j]][sign[k]]->SaveAs(folder+"/"+nameAC+".png");

    }
  }
*/
 
 for ( int i=1; i<5; i++ )  //pt
  {
  for ( int j=0; j<2; j++ )  //muSign
    {
    for ( int k=0; k<2; k++ )  //endcap
      {
      if (sign[j]=='-')	{ continue; }
      if (sign[k]=='-')	{ continue; }
      TString name;
      TPaveText * prel = cmsPrel();

      //============================== deltaPhi ROC curves ========================================
      if ( h_cum_deltaPhi[pt[i]][sign[j]][sign[k]]->GetNbinsX() != h_cum_deltaPhi[pt[i-1]][sign[j]][sign[k]]->GetNbinsX() )
        {
        cout << "Error in producing ROC curves: pt=" << pt[i] << " muSign=" << sign[j] << " endcap=" << sign[k] << endl;
	continue;
	}
      Int_t nbin = h_cum_deltaPhi[pt[i]][sign[j]][sign[k]]->GetNbinsX() ;
      Double_t entPtLow[nbin], entPtHigh[nbin], thrLow[nbin], thrHigh[nbin], x[nbin], y[nbin], errx[nbin], erry[nbin];

	h_cum_deltaPhi[pt[i-1]][sign[j]][sign[k]]->GetLowEdge( &thrLow[0] );
	h_cum_deltaPhi[pt[i]][sign[j]][sign[k]]->GetLowEdge( &thrHigh[0] );
      
      for (int ip = 0; ip<nbin; ip++)
        if ( thrLow[ip] != thrHigh[ip] )
	  {
          cout << "Error in producing ROC curves: hists for ptLow and ptHigh have different binning." << endl;
	  cout << "i=" << i << "\tj=" << j << "\tk=" << k << endl;
	  continue;
	  }

      for (int ip = 0; ip<nbin; ip++)
        {
        if ( sign[j]!=sign[k] )	
	  {
	  thrLow[ip]  += h_cum_deltaPhi[pt[i-1]][sign[j]][sign[k]]->GetBinWidth(3);
	  thrHigh[ip] += h_cum_deltaPhi[pt[i]][sign[j]][sign[k]]->GetBinWidth(3);
          }
	entPtLow[ip]  = h_cum_deltaPhi[pt[i-1]][sign[j]][sign[k]]->GetBinContent(ip+1); //ip+1 because GetBinContent(0) is the underflow
	entPtHigh[ip] = h_cum_deltaPhi[pt[i]][sign[j]][sign[k]]->GetBinContent(ip+1);   //ip+1 because GetBinContent(0) is the underflow
        x[ip]  = 1-entPtLow[ip];
	y[ip]  =  entPtHigh[ip];
	errx[ip]=0; erry[ip]=0;
	}

        name.Form("deltaPhiROC_"+type+"Mu_"+tag+"Based_%iGeV_%c%c", pt[i], sign[j], sign[k]  );
        c_deltaPhiROC[pt[i]][sign[j]][sign[k]] = canvasStyle(name,name);
        c_deltaPhiROC[pt[i]][sign[j]][sign[k]]->cd();

	g_deltaPhiROC[pt[i]][sign[j]][sign[k]] = new TGraphErrors( nbin, x, y, errx, erry);
	g_deltaPhiROC[pt[i]][sign[j]][sign[k]]->GetYaxis()->SetRangeUser(0,1);

        g_deltaPhiROC[pt[i]][sign[j]][sign[k]]->GetXaxis()->SetTitleFont(42);
        g_deltaPhiROC[pt[i]][sign[j]][sign[k]]->GetXaxis()->SetTitleSize(0.06);
        g_deltaPhiROC[pt[i]][sign[j]][sign[k]]->GetXaxis()->SetTitleOffset(0.9);
        g_deltaPhiROC[pt[i]][sign[j]][sign[k]]->GetXaxis()->SetLabelOffset(0.007);
        g_deltaPhiROC[pt[i]][sign[j]][sign[k]]->GetXaxis()->SetLabelSize(0.05);
        g_deltaPhiROC[pt[i]][sign[j]][sign[k]]->GetXaxis()->SetDecimals(kTRUE);
        g_deltaPhiROC[pt[i]][sign[j]][sign[k]]->GetXaxis()->SetTickLength(0.03);
        //g_deltaPhiROC[pt[i]][sign[j]][sign[k]]->GetYaxis()->SetTitleColor(kRed);
        g_deltaPhiROC[pt[i]][sign[j]][sign[k]]->GetYaxis()->SetTitleFont(42);
        g_deltaPhiROC[pt[i]][sign[j]][sign[k]]->GetYaxis()->SetTitleSize(0.06);
        g_deltaPhiROC[pt[i]][sign[j]][sign[k]]->GetYaxis()->SetTitleOffset(1.2);
        g_deltaPhiROC[pt[i]][sign[j]][sign[k]]->GetYaxis()->SetLabelOffset(0.007);
        g_deltaPhiROC[pt[i]][sign[j]][sign[k]]->GetYaxis()->SetLabelSize(0.05);
        //g_deltaPhiROC[pt[i]][sign[j]][sign[k]]->GetYaxis()->SetLabelColor(kRed);
        g_deltaPhiROC[pt[i]][sign[j]][sign[k]]->GetYaxis()->SetDecimals(kTRUE);
        g_deltaPhiROC[pt[i]][sign[j]][sign[k]]->GetYaxis()->SetTickLength(0.03);
        g_deltaPhiROC[pt[i]][sign[j]][sign[k]]->GetXaxis()->SetTitle(Form("Exclusion efficiency pt<%i GeV", pt[i-1]));
        g_deltaPhiROC[pt[i]][sign[j]][sign[k]]->GetYaxis()->SetTitle(Form("Selection efficiency pt>%i GeV", pt[i]));
        //g_deltaPhiROC[pt[i]][sign[j]][sign[k]]->GetYaxis()->SetRangeUser(0,1);
        //g_deltaPhiROC[pt[i]][sign[j]][sign[k]]->GetYaxis()->SetLimits(0,1);
        //g_deltaPhiROC[pt[i]][sign[j]][sign[k]]->GetXaxis()->SetLimits(1025/div,1600/div);
        // g_deltaPhiROC[pt[i]][sign[j]][sign[k]]->Write(grTitle);


	g_deltaPhiROC[pt[i]][sign[j]][sign[k]]->SetMarkerStyle(20);
        g_deltaPhiROC[pt[i]][sign[j]][sign[k]]->SetLineColor(kBlue);     
        g_deltaPhiROC[pt[i]][sign[j]][sign[k]]->SetMarkerColor(kBlue);     

	g_deltaPhiROC[pt[i]][sign[j]][sign[k]]->Draw("AP");

        TLatex l;
        l.SetTextSize(0.028);
        l.SetTextFont(42);
        l.SetTextAlign(12);
        for (int ip=0; ip<nbin; ip++)
	  {
	  if ( y[ip]<0.2 || y[ip]>0.8 )	continue;
          l.DrawLatex(x[ip]+0.03,y[ip]-0.01,Form("#Delta#Phi_{cut}=%4.3f",thrLow[ip]));
          }
        c_deltaPhiROC[pt[i]][sign[j]][sign[k]]->Update(); 
        
        prel->Draw("NB");
        c_deltaPhiROC[pt[i]][sign[j]][sign[k]]->Update();
        c_deltaPhiROC[pt[i]][sign[j]][sign[k]]->SaveAs(folder+"/"+name+".root");
        c_deltaPhiROC[pt[i]][sign[j]][sign[k]]->SaveAs(folder+"/"+name+".C");
        c_deltaPhiROC[pt[i]][sign[j]][sign[k]]->SaveAs(folder+"/"+name+".png");


/*
      c_deltaPhiROC[pt[i]][sign[j]][sign[k]]->Update(); //otherwise next commands don't work
      // default stats coordinates:
      // X1=.78  X2=.98
      // Y1=.775 Y2=0.935

      TString line1, line2;
      line1.Form("from Segment #mu^{%c} %i GeV", sign[j], pt[i]);
      line2.Form("from SimHit #mu^{%c} %i GeV", sign[j], pt[i]);
      TPaveStats *stDeltaphi = (TPaveStats*)h_cum_deltaPhi[pt[i]][sign[j]][sign[k]]->FindObject("stats");
      stDeltaphi->SetY1NDC(.775-.070); //new x start position
      stDeltaphi->SetY2NDC(.935-.070); //new x end position
      h_cum_deltaPhi[pt[i]][sign[j]][sign[k]]->SetName(line1);
      stDeltaphi = (TPaveStats*)h_sh_deltaPhi[pt[i]][sign[j]][sign[k]]->FindObject("stats");
      stDeltaphi->SetY1NDC(.755-.070); //new x start position
      stDeltaphi->SetY2NDC(.595-.070); //new x end position
      h_cum_deltaPhi[pt[i-1]][sign[j]][sign[k]]->SetName(line2);

      TLegend *legDeltaphi = new TLegend(0.18,0.765,0.6,0.865);
      legDeltaphi->SetTextSize(0.04);
      legDeltaphi->SetTextFont(42);
      legDeltaphi->AddEntry(h_cum_deltaPhi[pt[i]][sign[j]][sign[k]], line1, "l");
      legDeltaphi->AddEntry(h_sh_deltaPhi[pt[i]][sign[j]][sign[k]], line2, "l");
      legDeltaphi->Draw();



      //============================== alpha ROC curves ========================================
      if ( h_cum_alpha[pt[i]][sign[j]][sign[k]]->GetNbinsX() != h_cum_alpha[pt[i-1]][sign[j]][sign[k]]->GetNbinsX() )
        {
        cout << "Error in producing ROC curves: pt=" << pt[i] << " muSign=" << sign[j] << " endcap=" << sign[k] << endl;
	continue;
	}
      Int_t nbin2 = h_cum_alpha[pt[i]][sign[j]][sign[k]]->GetNbinsX() ;
      Double_t entPtLow2[nbin2], entPtHigh2[nbin2], thrLow2[nbin2], thrHigh2[nbin2], x2[nbin2], y2[nbin2], errx2[nbin2], erry2[nbin2];

	h_cum_alpha[pt[i-1]][sign[j]][sign[k]]->GetLowEdge( &thrLow2[0] );
	h_cum_alpha[pt[i]][sign[j]][sign[k]]->GetLowEdge( &thrHigh2[0] );
      
      for (int ip = 0; ip<nbin2; ip++)
        if ( thrLow2[ip] != thrHigh2[ip] )
	  {
          cout << "Error in producing ROC curves: hists for ptLow and ptHigh have different binning." << endl;
	  cout << "i=" << i << "\tj=" << j << "\tk=" << k << endl;
	  continue;
	  }

      for (int ip = 0; ip<nbin2; ip++)
        {
        if ( sign[j]!=sign[k] )	
	  {
	  thrLow2[ip]  += h_cum_alpha[pt[i-1]][sign[j]][sign[k]]->GetBinWidth(3);
	  thrHigh2[ip] += h_cum_alpha[pt[i]][sign[j]][sign[k]]->GetBinWidth(3);
          }
	entPtLow2[ip]  = h_cum_alpha[pt[i-1]][sign[j]][sign[k]]->GetBinContent(ip+1); //ip+1 because GetBinContent(0) is the underflow
	entPtHigh2[ip] = h_cum_alpha[pt[i]][sign[j]][sign[k]]->GetBinContent(ip+1);   //ip+1 because GetBinContent(0) is the underflow
        x2[ip]  = 1-entPtLow2[ip];
	y2[ip]  =  entPtHigh2[ip];
	errx2[ip]=0; erry2[ip]=0;
	}

        name.Form("alphaROC_"+type+"Mu_"+tag+"Based_%iGeV_%c%c", pt[i], sign[j], sign[k]  );
        c_alphaROC[pt[i]][sign[j]][sign[k]] = canvasStyle(name,name);
        c_alphaROC[pt[i]][sign[j]][sign[k]]->cd();

	g_alphaROC[pt[i]][sign[j]][sign[k]] = new TGraphErrors( nbin2, x2, y2, errx2, erry2);
	g_alphaROC[pt[i]][sign[j]][sign[k]]->GetYaxis()->SetRangeUser(0,1);

        g_alphaROC[pt[i]][sign[j]][sign[k]]->GetXaxis()->SetTitleFont(42);
        g_alphaROC[pt[i]][sign[j]][sign[k]]->GetXaxis()->SetTitleSize(0.06);
        g_alphaROC[pt[i]][sign[j]][sign[k]]->GetXaxis()->SetTitleOffset(0.9);
        g_alphaROC[pt[i]][sign[j]][sign[k]]->GetXaxis()->SetLabelOffset(0.007);
        g_alphaROC[pt[i]][sign[j]][sign[k]]->GetXaxis()->SetLabelSize(0.05);
        g_alphaROC[pt[i]][sign[j]][sign[k]]->GetXaxis()->SetDecimals(kTRUE);
        g_alphaROC[pt[i]][sign[j]][sign[k]]->GetXaxis()->SetTickLength(0.03);
        //g_alphaROC[pt[i]][sign[j]][sign[k]]->GetYaxis()->SetTitleColor(kRed);
        g_alphaROC[pt[i]][sign[j]][sign[k]]->GetYaxis()->SetTitleFont(42);
        g_alphaROC[pt[i]][sign[j]][sign[k]]->GetYaxis()->SetTitleSize(0.06);
        g_alphaROC[pt[i]][sign[j]][sign[k]]->GetYaxis()->SetTitleOffset(1.2);
        g_alphaROC[pt[i]][sign[j]][sign[k]]->GetYaxis()->SetLabelOffset(0.007);
        g_alphaROC[pt[i]][sign[j]][sign[k]]->GetYaxis()->SetLabelSize(0.05);
        //g_alphaROC[pt[i]][sign[j]][sign[k]]->GetYaxis()->SetLabelColor(kRed);
        g_alphaROC[pt[i]][sign[j]][sign[k]]->GetYaxis()->SetDecimals(kTRUE);
        g_alphaROC[pt[i]][sign[j]][sign[k]]->GetYaxis()->SetTickLength(0.03);
        g_alphaROC[pt[i]][sign[j]][sign[k]]->GetXaxis()->SetTitle(Form("Exclusion efficiency pt<%i GeV", pt[i-1]));
        g_alphaROC[pt[i]][sign[j]][sign[k]]->GetYaxis()->SetTitle(Form("Selection efficiency pt>%i GeV", pt[i]));
        //g_alphaROC[pt[i]][sign[j]][sign[k]]->GetYaxis()->SetRangeUser(0,1);
        //g_alphaROC[pt[i]][sign[j]][sign[k]]->GetYaxis()->SetLimits(0,1);
        //g_alphaROC[pt[i]][sign[j]][sign[k]]->GetXaxis()->SetLimits(1025/div,1600/div);
        // g_alphaROC[pt[i]][sign[j]][sign[k]]->Write(grTitle);


	g_alphaROC[pt[i]][sign[j]][sign[k]]->SetMarkerStyle(20);
        g_alphaROC[pt[i]][sign[j]][sign[k]]->SetLineColor(kBlue);     
        g_alphaROC[pt[i]][sign[j]][sign[k]]->SetMarkerColor(kBlue);     

	g_alphaROC[pt[i]][sign[j]][sign[k]]->Draw("AP");

        TLatex l2;
        l2.SetTextSize(0.028);
        l2.SetTextFont(42);
        l2.SetTextAlign(12);
        for (int ip=0; ip<nbin2; ip++)
	  {
	  if ( y2[ip]<0.2 || y2[ip]>0.8 )	continue;
          l2.DrawLatex(x2[ip]+0.03,y2[ip]-0.01,Form("#alpha_{cut}=%4.3f",thrLow[ip]));
          }
        c_alphaROC[pt[i]][sign[j]][sign[k]]->Update(); 
        
        prel->Draw("NB");
        c_alphaROC[pt[i]][sign[j]][sign[k]]->Update();
        c_alphaROC[pt[i]][sign[j]][sign[k]]->SaveAs(folder+"/"+name+".root");
        c_alphaROC[pt[i]][sign[j]][sign[k]]->SaveAs(folder+"/"+name+".C");
        c_alphaROC[pt[i]][sign[j]][sign[k]]->SaveAs(folder+"/"+name+".png");
*/

/*
      c_alphaROC[pt[i]][sign[j]][sign[k]]->Update(); //otherwise next commands don't work
      // default stats coordinates:
      // X1=.78  X2=.98
      // Y1=.775 Y2=0.935

      TString line1, line2;
      line1.Form("from Segment #mu^{%c} %i GeV", sign[j], pt[i]);
      line2.Form("from SimHit #mu^{%c} %i GeV", sign[j], pt[i]);
      TPaveStats *stDeltaphi = (TPaveStats*)h_cum_alpha[pt[i]][sign[j]][sign[k]]->FindObject("stats");
      stDeltaphi->SetY1NDC(.775-.070); //new x start position
      stDeltaphi->SetY2NDC(.935-.070); //new x end position
      h_cum_alpha[pt[i]][sign[j]][sign[k]]->SetName(line1);
      stDeltaphi = (TPaveStats*)h_sh_alpha[pt[i]][sign[j]][sign[k]]->FindObject("stats");
      stDeltaphi->SetY1NDC(.755-.070); //new x start position
      stDeltaphi->SetY2NDC(.595-.070); //new x end position
      h_cum_alpha[pt[i-1]][sign[j]][sign[k]]->SetName(line2);

      TLegend *legDeltaphi = new TLegend(0.18,0.765,0.6,0.865);
      legDeltaphi->SetTextSize(0.04);
      legDeltaphi->SetTextFont(42);
      legDeltaphi->AddEntry(h_cum_alpha[pt[i]][sign[j]][sign[k]], line1, "l");
      legDeltaphi->AddEntry(h_sh_alpha[pt[i]][sign[j]][sign[k]], line2, "l");
      legDeltaphi->Draw();
*/
      }
    }
  }

//  cout << "Count: " << c_compAlpha.count(5) << endl;
//  cout << "Count: " << c_compAlpha[5].count('+') << endl;
//  cout << "Count: " << c_compAlpha[5]['+'].count('+') << endl;

/*
 TCanvas * test =  canvasStyle("comparison_3GeV","comparison_3GeV");
 for ( int i=0; i<5; i++ )  //pt
 for ( int i=0; i<1; i++ )  //pt
  {
  for ( int j=0; j<1; j++ )  //muSign
    {
    for ( int k=0; k<1; k++ )  //endcap
      {
//      h_sg_deltaPhi[pt[i]][sign[j]][sign[k]]	->Draw("HIST");
      h_sg_alpha[pt[i]][sign[j]][sign[k]]	->Draw("HIST");
//      h_sh_deltaPhi[pt[i]][sign[j]][sign[k]]	->Draw("HISTSAMES");
      h_sh_alpha[pt[i]][sign[j]][sign[k]]	->Draw("HISTSAMES");

      }
    }
  }

  TPaveText * prel = cmsPrel();
  prel->Draw();
  test->Update();
*/

/*
 base = "/afs/cern.ch/work/m/mressego/private/ME0trigger/CMSSW_9_3_3/src/samples/CRAB/crab_SEGM-ANA"+tag2+"/RESULTS/v"+version+"/";
 filename = type+"Mu_"+energy+"GeV_segmAnalyzer_"+tag+"Based.root";

 TFile *f1 = TFile::Open(base+type+"Mu_3GeV_segmAnalyzer_"+tag+"Based.root", "READ");   
 TFile *f2 = TFile::Open(base+type+"Mu_5GeV_segmAnalyzer_"+tag+"Based.root", "READ");   
 TFile *f3 = TFile::Open(base+type+"Mu_10GeV_segmAnalyzer_"+tag+"Based.root", "READ");   
 TFile *f4 = TFile::Open(base+type+"Mu_20GeV_segmAnalyzer_"+tag+"Based.root", "READ");   
 TFile *f5 = TFile::Open(base+type+"Mu_50GeV_segmAnalyzer_"+tag+"Based.root", "READ");   
//f1->ls();  cout << "\n" << endl;
// f2->ls();  cout << "\n" << endl;
// f3->ls();  cout << "\n" << endl;

 *tc = new TChain("demo/Event");
 tc->Add(base+filename); // Adds "tree1" from file.root
 TH1F *ne_pointer = new TH1F("ne", "segDeltaPhi_++",80,-0.3,0.5);
 tc->Draw("segmentDeltaPhiLocal3_posMu_posEnd>>ne","segmentDeltaPhiLocal3_posMu_posEnd!=-7 && nSeg_posMu_posEnd==1 && nSegmentRecHits_posMu_posEnd==6");
*/

// Segment mu+
/*
 TH1F * g_03GeVSegm_pos	= (TH1F*)f1->Get("demo/h_SegmentDeltaPhiCorr_posMu");
 TH1F * g_05GeVSegm_pos	= (TH1F*)f2->Get("demo/h_SegmentDeltaPhiCorr_posMu");
 TH1F * g_10GeVSegm_pos	= (TH1F*)f3->Get("demo/h_SegmentDeltaPhiCorr_posMu");    
 TH1F * g_20GeVSegm_pos	= (TH1F*)f4->Get("demo/h_SegmentDeltaPhiCorr_posMu");    
 TH1F * g_50GeVSegm_pos	= (TH1F*)f5->Get("demo/h_SegmentDeltaPhiCorr_posMu");    

 g_03GeVSegm_pos->Scale(1/g_03GeVSegm_pos->GetEntries());
 g_05GeVSegm_pos->Scale(1/g_05GeVSegm_pos->GetEntries());
 g_10GeVSegm_pos->Scale(1/g_10GeVSegm_pos->GetEntries());
 g_20GeVSegm_pos->Scale(1/g_20GeVSegm_pos->GetEntries());
 g_50GeVSegm_pos->Scale(1/g_50GeVSegm_pos->GetEntries());

 g_03GeVSegm_pos->SetLineColor(kOrange);
 g_05GeVSegm_pos->SetLineColor(kBlue);
 g_10GeVSegm_pos->SetLineColor(kRed);
 g_20GeVSegm_pos->SetLineColor(kGreen);
 g_50GeVSegm_pos->SetLineColor(kMagenta);
 
 g_03GeVSegm_pos->SetLineWidth(3);
 g_05GeVSegm_pos->SetLineWidth(3);
 g_10GeVSegm_pos->SetLineWidth(3);
 g_20GeVSegm_pos->SetLineWidth(3);
 g_50GeVSegm_pos->SetLineWidth(3);

 g_03GeVSegm_pos->SetLineStyle(1);
 g_05GeVSegm_pos->SetLineStyle(1);
 g_10GeVSegm_pos->SetLineStyle(1);
 g_20GeVSegm_pos->SetLineStyle(1);
 g_50GeVSegm_pos->SetLineStyle(1);

// Segment mu-

 TH1F * g_03GeVSegm_neg	= (TH1F*)f1->Get("demo/h_SegmentDeltaPhiCorr_negMu");
 TH1F * g_05GeVSegm_neg	= (TH1F*)f2->Get("demo/h_SegmentDeltaPhiCorr_negMu");
 TH1F * g_10GeVSegm_neg	= (TH1F*)f3->Get("demo/h_SegmentDeltaPhiCorr_negMu");    
 TH1F * g_20GeVSegm_neg	= (TH1F*)f4->Get("demo/h_SegmentDeltaPhiCorr_negMu");    
 TH1F * g_50GeVSegm_neg	= (TH1F*)f5->Get("demo/h_SegmentDeltaPhiCorr_negMu");    

 g_03GeVSegm_neg->Scale(1/g_03GeVSegm_neg->GetEntries());
 g_05GeVSegm_neg->Scale(1/g_05GeVSegm_neg->GetEntries());
 g_10GeVSegm_neg->Scale(1/g_10GeVSegm_neg->GetEntries());
 g_20GeVSegm_neg->Scale(1/g_20GeVSegm_neg->GetEntries());
 g_50GeVSegm_neg->Scale(1/g_50GeVSegm_neg->GetEntries());

 g_03GeVSegm_neg->SetLineColor(kOrange);
 g_05GeVSegm_neg->SetLineColor(kBlue);
 g_10GeVSegm_neg->SetLineColor(kRed);
 g_20GeVSegm_neg->SetLineColor(kGreen);
 g_50GeVSegm_neg->SetLineColor(kMagenta);
 
 g_03GeVSegm_neg->SetLineWidth(3);
 g_05GeVSegm_neg->SetLineWidth(3);
 g_10GeVSegm_neg->SetLineWidth(3);
 g_20GeVSegm_neg->SetLineWidth(3);
 g_50GeVSegm_neg->SetLineWidth(3);

// SimHit mu+

 TH1F * g_03GeVHit_pos	= (TH1F*)f1->Get("demo/h_SimHitDeltaPhiCorr_posMu");
 TH1F * g_05GeVHit_pos	= (TH1F*)f2->Get("demo/h_SimHitDeltaPhiCorr_posMu");
 TH1F * g_10GeVHit_pos	= (TH1F*)f3->Get("demo/h_SimHitDeltaPhiCorr_posMu");    
 TH1F * g_20GeVHit_pos	= (TH1F*)f4->Get("demo/h_SimHitDeltaPhiCorr_posMu");    
 TH1F * g_50GeVHit_pos	= (TH1F*)f5->Get("demo/h_SimHitDeltaPhiCorr_posMu");    

 g_03GeVHit_pos->Scale(1/g_03GeVHit_pos->GetEntries());
 g_05GeVHit_pos->Scale(1/g_05GeVHit_pos->GetEntries());
 g_10GeVHit_pos->Scale(1/g_10GeVHit_pos->GetEntries());
 g_20GeVHit_pos->Scale(1/g_20GeVHit_pos->GetEntries());
 g_50GeVHit_pos->Scale(1/g_50GeVHit_pos->GetEntries());

 g_03GeVHit_pos->SetLineColor(kOrange);
 g_05GeVHit_pos->SetLineColor(kBlue);
 g_10GeVHit_pos->SetLineColor(kRed);
 g_20GeVHit_pos->SetLineColor(kGreen);
 g_50GeVHit_pos->SetLineColor(kMagenta);
 
 g_03GeVHit_pos->SetLineWidth(3);
 g_05GeVHit_pos->SetLineWidth(3);
 g_10GeVHit_pos->SetLineWidth(3);
 g_20GeVHit_pos->SetLineWidth(3);
 g_50GeVHit_pos->SetLineWidth(3);

// SimHit mu-

 TH1F * g_03GeVHit_neg	= (TH1F*)f1->Get("demo/h_SimHitDeltaPhiCorr_negMu");
 TH1F * g_05GeVHit_neg	= (TH1F*)f2->Get("demo/h_SimHitDeltaPhiCorr_negMu");
 TH1F * g_10GeVHit_neg	= (TH1F*)f3->Get("demo/h_SimHitDeltaPhiCorr_negMu");    
 TH1F * g_20GeVHit_neg	= (TH1F*)f4->Get("demo/h_SimHitDeltaPhiCorr_negMu");    
 TH1F * g_50GeVHit_neg	= (TH1F*)f5->Get("demo/h_SimHitDeltaPhiCorr_negMu");    

 g_03GeVHit_neg->Scale(1/g_03GeVHit_neg->GetEntries());
 g_05GeVHit_neg->Scale(1/g_05GeVHit_neg->GetEntries());
 g_10GeVHit_neg->Scale(1/g_10GeVHit_neg->GetEntries());
 g_20GeVHit_neg->Scale(1/g_20GeVHit_neg->GetEntries());
 g_50GeVHit_neg->Scale(1/g_50GeVHit_neg->GetEntries());

 g_03GeVHit_neg->SetLineColor(kOrange);
 g_05GeVHit_neg->SetLineColor(kBlue);
 g_10GeVHit_neg->SetLineColor(kRed);
 g_20GeVHit_neg->SetLineColor(kGreen);
 g_50GeVHit_neg->SetLineColor(kMagenta);
 
 g_03GeVHit_neg->SetLineWidth(3);
 g_05GeVHit_neg->SetLineWidth(3);
 g_10GeVHit_neg->SetLineWidth(3);
 g_20GeVHit_neg->SetLineWidth(3);
 g_50GeVHit_neg->SetLineWidth(3);


//================================ ALL mu+ SEGMENT DELTA PHI  ==================================  
  
  
TCanvas *c1 = new TCanvas("c1","SegmentDeltaPhi_posMu", 50,50,W,H );    //canvas for 2 curves with tracking (one per gas)

 //cms aesthetics
  c1->SetFillColor(0);
  c1->SetBorderMode(0);
  c1->SetFrameFillStyle(0);
  c1->SetFrameBorderMode(0);
  c1->SetLeftMargin( L/W );
  c1->SetRightMargin( R/W );
  c1->SetTopMargin( T/H );
  c1->SetBottomMargin( B/H );
  c1->SetTickx(0);
  c1->SetTicky(0);
 //end cms aesthetics
  
  c1->SetGridy();
  //c1->SetGridx(5);


TPaveText *t1=new TPaveText(0.145,0.88,0.37,0.94,"brNDC");
  t1->AddText("CMS Preliminary");
  t1->SetTextAlign(12);
  Int_t trans_white = TColor::GetColorTransparent(0, 0);
  t1->SetShadowColor(trans_white);
  t1->SetFillColor(trans_white);
  t1->SetLineColor(trans_white);
  t1->SetLineColor(trans_white);
  //t1->AddText("more text");


  g_05GeVSegm_pos->GetYaxis()->SetRangeUser(0,.34);
  g_05GeVSegm_pos->GetXaxis()->SetRangeUser(-0.3,0.3);
  
  g_05GeVSegm_pos->GetXaxis()->SetTitleFont(42);
  g_05GeVSegm_pos->GetXaxis()->SetTitleSize(0.06);
  g_05GeVSegm_pos->GetXaxis()->SetTitleOffset(0.9);
  g_05GeVSegm_pos->GetXaxis()->SetLabelOffset(0.007);
  g_05GeVSegm_pos->GetXaxis()->SetLabelSize(0.05);
  g_05GeVSegm_pos->GetXaxis()->SetDecimals(kTRUE);
  g_05GeVSegm_pos->GetXaxis()->SetTickLength(0.03);
  //g_05GeVSegm_pos->GetYaxis()->SetTitleColor(kRed);
  g_05GeVSegm_pos->GetYaxis()->SetTitleFont(42);
  g_05GeVSegm_pos->GetYaxis()->SetTitleSize(0.06);
  g_05GeVSegm_pos->GetYaxis()->SetTitleOffset(1.2);
  g_05GeVSegm_pos->GetYaxis()->SetLabelOffset(0.007);
  g_05GeVSegm_pos->GetYaxis()->SetLabelSize(0.05);
  //g_05GeVSegm_pos->GetYaxis()->SetLabelColor(kRed);
  g_05GeVSegm_pos->GetYaxis()->SetDecimals(kTRUE);
  g_05GeVSegm_pos->GetYaxis()->SetTickLength(0.03);
  g_05GeVSegm_pos->GetXaxis()->SetTitle("#Delta #Phi");
  g_05GeVSegm_pos->GetYaxis()->SetTitle("Entries");
  //g_05GeVSegm_pos->GetYaxis()->SetRangeUser(0,1);
  //g_05GeVSegm_pos->GetYaxis()->SetLimits(0,1);
  //g_05GeVSegm_pos->GetXaxis()->SetLimits(1025/div,1600/div);
 // g_05GeVSegm_pos->Write(grTitle);


 g_05GeVSegm_pos->Draw("HIST");
 g_03GeVSegm_pos->Draw("HISTSAMES");
 g_10GeVSegm_pos->Draw("HISTSAMES");
 g_20GeVSegm_pos->Draw("HISTSAMES");
 g_50GeVSegm_pos->Draw("HISTSAMES");

 c1->Update(); //otherwise next commands don't work
 // default stats coordinates:
 // X1=.78  X2=.98
 // Y1=.775 Y2=0.935

 TPaveStats *st1 = (TPaveStats*)g_03GeVSegm_pos->FindObject("stats");
 st1->SetY1NDC(.935+.050); //new x start position
 st1->SetY2NDC(.775+.050); //new x start position
 g_03GeVSegm_pos->SetName("prompt #mu^{+} 3 GeV");
 st1 = (TPaveStats*)g_05GeVSegm_pos->FindObject("stats");
 st1->SetY1NDC(.770+.050); //new x start position
 st1->SetY2NDC(.610+0.05); //new x end position
 g_05GeVSegm_pos->SetName("prompt #mu^{+} 5 GeV");
 st1 = (TPaveStats*)g_10GeVSegm_pos->FindObject("stats");
 st1->SetY1NDC(.605+0.05); //new x start position
 st1->SetY2NDC(.445+0.05); //new x end position
 g_10GeVSegm_pos->SetName("prompt #mu^{+} 10 GeV");
 st1 = (TPaveStats*)g_20GeVSegm_pos->FindObject("stats");
 st1->SetY1NDC(.440+0.05); //new x start position
 st1->SetY2NDC(.280+0.05); //new x end position
 g_20GeVSegm_pos->SetName("prompt #mu^{+} 20 GeV");
 st1 = (TPaveStats*)g_50GeVSegm_pos->FindObject("stats");
 st1->SetY1NDC(.275+0.05); //new x start position
 st1->SetY2NDC(.115+0.05); //new x end position
 g_50GeVSegm_pos->SetName("prompt #mu^{+} 50 GeV");

 TLegend *leg = new TLegend(0.18,0.685,0.49,0.865);
 leg->SetTextSize(0.04);
 leg->SetTextFont(42);
 leg->AddEntry(g_03GeVSegm_pos, "prompt #mu^{+} 3 GeV", "l");
 leg->AddEntry(g_05GeVSegm_pos, "prompt #mu^{+} 5 GeV", "l");
 leg->AddEntry(g_10GeVSegm_pos, "prompt #mu^{+} 10 GeV", "l");
 leg->AddEntry(g_20GeVSegm_pos, "prompt #mu^{+} 20 GeV", "l");
 leg->AddEntry(g_50GeVSegm_pos, "prompt #mu^{+} 50 GeV", "l");
 leg->Draw();

 t1->Draw("NB");
 c1->Update();



//================================ ALL mu- SEGMENT DELTA PHI  ==================================  
  
  
TCanvas *c2 = new TCanvas("c2","SegmentDeltaPhi_negMu", 50,50,W,H );    //canvas for 2 curves with tracking (one per gas)

 //cms aesthetics
  c2->SetFillColor(0);
  c2->SetBorderMode(0);
  c2->SetFrameFillStyle(0);
  c2->SetFrameBorderMode(0);
  c2->SetLeftMargin( L/W );
  c2->SetRightMargin( R/W );
  c2->SetTopMargin( T/H );
  c2->SetBottomMargin( B/H );
  c2->SetTickx(0);
  c2->SetTicky(0);
 //end cms aesthetics
  
  c2->SetGridy();
  //c2->SetGridx(5);

  c2->cd();
TPaveText *t2=new TPaveText(0.145,0.88,0.37,0.94,"brNDC");
  t2->AddText("CMS Preliminary");
  t2->SetTextAlign(12);
  //Int_t trans_white = TColor::GetColorTransparent(0, 0);
  t2->SetShadowColor(trans_white);
  t2->SetFillColor(trans_white);
  t2->SetLineColor(trans_white);
  t2->SetLineColor(trans_white);
  //t2->AddText("more text");


  g_05GeVSegm_neg->GetYaxis()->SetRangeUser(0,.34);
  g_05GeVSegm_neg->GetXaxis()->SetRangeUser(-0.3,0.3);
  
  g_05GeVSegm_neg->GetXaxis()->SetTitleFont(42);
  g_05GeVSegm_neg->GetXaxis()->SetTitleSize(0.06);
  g_05GeVSegm_neg->GetXaxis()->SetTitleOffset(0.9);
  g_05GeVSegm_neg->GetXaxis()->SetLabelOffset(0.007);
  g_05GeVSegm_neg->GetXaxis()->SetLabelSize(0.05);
  g_05GeVSegm_neg->GetXaxis()->SetDecimals(kTRUE);
  g_05GeVSegm_neg->GetXaxis()->SetTickLength(0.03);
  //g_05GeVSegm_neg->GetYaxis()->SetTitleColor(kRed);
  g_05GeVSegm_neg->GetYaxis()->SetTitleFont(42);
  g_05GeVSegm_neg->GetYaxis()->SetTitleSize(0.06);
  g_05GeVSegm_neg->GetYaxis()->SetTitleOffset(1.2);
  g_05GeVSegm_neg->GetYaxis()->SetLabelOffset(0.007);
  g_05GeVSegm_neg->GetYaxis()->SetLabelSize(0.05);
  //g_05GeVSegm_neg->GetYaxis()->SetLabelColor(kRed);
  g_05GeVSegm_neg->GetYaxis()->SetDecimals(kTRUE);
  g_05GeVSegm_neg->GetYaxis()->SetTickLength(0.03);
  g_05GeVSegm_neg->GetXaxis()->SetTitle("#Delta #Phi");
  g_05GeVSegm_neg->GetYaxis()->SetTitle("Entries");
  //g_05GeVSegm_neg->GetYaxis()->SetRangeUser(0,1);
  //g_05GeVSegm_neg->GetYaxis()->SetLimits(0,1);
  //g_05GeVSegm_neg->GetXaxis()->SetLimits(1025/div,1600/div);
 // g_05GeVSegm_neg->Write(grTitle);


 g_05GeVSegm_neg->Draw("HIST");     
 g_03GeVSegm_neg->Draw("HISTSAMES");     
 g_10GeVSegm_neg->Draw("HISTSAMES");
 g_20GeVSegm_neg->Draw("HISTSAMES");
 g_50GeVSegm_neg->Draw("HISTSAMES");

 c2->Update(); //otherwise next commands don't work
 // default stats coordinates:
 // X1=.78  X2=.98
 // Y1=.775 Y2=0.935

 TPaveStats *st2 = (TPaveStats*)g_03GeVSegm_neg->FindObject("stats");
 st2->SetY1NDC(.935+.050); //new x start position
 st2->SetY2NDC(.775+.050); //new x start position
 g_03GeVSegm_neg->SetName("prompt #mu^{-} 3 GeV");
 st2 = (TPaveStats*)g_05GeVSegm_neg->FindObject("stats");
 st2->SetY1NDC(.770+.050); //new x start position
 st2->SetY2NDC(.610+0.05); //new x end position
 g_05GeVSegm_neg->SetName("prompt #mu^{-} 5 GeV");
 st2 = (TPaveStats*)g_10GeVSegm_neg->FindObject("stats");
 st2->SetY1NDC(.605+0.05); //new x start position
 st2->SetY2NDC(.445+0.05); //new x end position
 g_10GeVSegm_neg->SetName("prompt #mu^{-} 10 GeV");
 st2 = (TPaveStats*)g_20GeVSegm_neg->FindObject("stats");
 st2->SetY1NDC(.440+0.05); //new x start position
 st2->SetY2NDC(.280+0.05); //new x end position
 g_20GeVSegm_neg->SetName("prompt #mu^{-} 20 GeV");
 st2 = (TPaveStats*)g_50GeVSegm_neg->FindObject("stats");
 st2->SetY1NDC(.275+0.05); //new x start position
 st2->SetY2NDC(.115+0.05); //new x end position
 g_50GeVSegm_neg->SetName("prompt #mu^{-} 50 GeV");

 TLegend *leg2 = new TLegend(0.18,0.685,0.49,0.865);
 leg2->SetTextSize(0.04);
 leg2->SetTextFont(42);
 leg2->AddEntry(g_03GeVSegm_neg, "prompt #mu^{-} 3 GeV", "l");
 leg2->AddEntry(g_05GeVSegm_neg, "prompt #mu^{-} 5 GeV", "l");
 leg2->AddEntry(g_10GeVSegm_neg, "prompt #mu^{-} 10 GeV", "l");
 leg2->AddEntry(g_20GeVSegm_neg, "prompt #mu^{-} 20 GeV", "l");
 leg2->AddEntry(g_50GeVSegm_neg, "prompt #mu^{-} 50 GeV", "l");
 leg2->Draw();

 t2->Draw("NB");
 c2->Update();



//================================ 03 GeV negMu  ==================================  
  
  
TCanvas *c03neg = new TCanvas("c03neg","negMu_03GeV", 50,50,W,H );    //canvas for 2 curves with tracking (one per gas)

 //cms aesthetics
  c03neg->SetFillColor(0);
  c03neg->SetBorderMode(0);
  c03neg->SetFrameFillStyle(0);
  c03neg->SetFrameBorderMode(0);
  c03neg->SetLeftMargin( L/W );
  c03neg->SetRightMargin( R/W );
  c03neg->SetTopMargin( T/H );
  c03neg->SetBottomMargin( B/H );
  c03neg->SetTickx(0);
  c03neg->SetTicky(0);
 //end cms aesthetics
  
  c03neg->SetGridy();
  //c03neg->SetGridx(5);

  c03neg->cd();
TPaveText *t03neg=new TPaveText(0.145,0.88,0.37,0.94,"brNDC");
  t03neg->AddText("CMS Preliminary");
  t03neg->SetTextAlign(12);
  //Int_t trans_white = TColor::GetColorTransparent(0, 0);
  t03neg->SetShadowColor(trans_white);
  t03neg->SetFillColor(trans_white);
  t03neg->SetLineColor(trans_white);
  t03neg->SetLineColor(trans_white);
  //t03neg->AddText("more text");


  g_03GeVSegm_neg->GetYaxis()->SetRangeUser(0,.2);
  g_03GeVSegm_neg->GetXaxis()->SetRangeUser(-0.3,0.3);
  
  g_03GeVSegm_neg->GetXaxis()->SetTitleFont(42);
  g_03GeVSegm_neg->GetXaxis()->SetTitleSize(0.06);
  g_03GeVSegm_neg->GetXaxis()->SetTitleOffset(0.9);
  g_03GeVSegm_neg->GetXaxis()->SetLabelOffset(0.007);
  g_03GeVSegm_neg->GetXaxis()->SetLabelSize(0.05);
  g_03GeVSegm_neg->GetXaxis()->SetDecimals(kTRUE);
  g_03GeVSegm_neg->GetXaxis()->SetTickLength(0.03);
  //g_03GeVSegm_neg->GetYaxis()->SetTitleColor(kRed);
  g_03GeVSegm_neg->GetYaxis()->SetTitleFont(42);
  g_03GeVSegm_neg->GetYaxis()->SetTitleSize(0.06);
  g_03GeVSegm_neg->GetYaxis()->SetTitleOffset(1.2);
  g_03GeVSegm_neg->GetYaxis()->SetLabelOffset(0.007);
  g_03GeVSegm_neg->GetYaxis()->SetLabelSize(0.05);
  //g_03GeVSegm_neg->GetYaxis()->SetLabelColor(kRed);
  g_03GeVSegm_neg->GetYaxis()->SetDecimals(kTRUE);
  g_03GeVSegm_neg->GetYaxis()->SetTickLength(0.03);
  g_03GeVSegm_neg->GetXaxis()->SetTitle("#Delta #Phi");
  g_03GeVSegm_neg->GetYaxis()->SetTitle("Entries");
  //g_03GeVSegm_neg->GetYaxis()->SetRangeUser(0,1);
  //g_03GeVSegm_neg->GetYaxis()->SetLimits(0,1);
  //g_03GeVSegm_neg->GetXaxis()->SetLimits(1025/div,1600/div);
 // g_03GeVSegm_neg->Write(grTitle);


 g_03GeVSegm_neg->SetLineColor(kRed);     
 g_03GeVHit_neg->SetLineColor(kBlue);
 g_03GeVSegm_neg->Draw("HIST");     
 g_03GeVHit_neg->Draw("HISTSAMES");

 c03neg->Update(); //otherwise next commands don't work
 // default stats coordinates:
 // X1=.78  X2=.98
 // Y1=.775 Y2=0.935

 TPaveStats *st03neg = (TPaveStats*)g_03GeVSegm_neg->FindObject("stats");
 st03neg->SetY1NDC(.775-.070); //new x start position
 st03neg->SetY2NDC(.935-.070); //new x end position
 g_03GeVSegm_neg->SetName("from Segment #mu^{-} 3 GeV");
 st03neg = (TPaveStats*)g_03GeVHit_neg->FindObject("stats");
 st03neg->SetY1NDC(.755-.070); //new x start position
 st03neg->SetY2NDC(.595-.070); //new x end position
 g_03GeVHit_neg->SetName("from SimHit #mu^{-} 3 GeV");

 TLegend *leg03neg = new TLegend(0.18,0.765,0.6,0.865);
 leg03neg->SetTextSize(0.04);
 leg03neg->SetTextFont(42);
 leg03neg->AddEntry(g_03GeVSegm_neg, "from Segment #mu^{-} 3 GeV", "l");
 leg03neg->AddEntry(g_03GeVHit_neg, "from SimHit #mu^{-} 3 GeV", "l");
 leg03neg->Draw();

 t03neg->Draw("NB");
 c03neg->Update();

//================================ 05 GeV negMu  ==================================  
  
  
TCanvas *c05neg = new TCanvas("c05neg","negMu_05GeV", 50,50,W,H );    //canvas for 2 curves with tracking (one per gas)

 //cms aesthetics
  c05neg->SetFillColor(0);
  c05neg->SetBorderMode(0);
  c05neg->SetFrameFillStyle(0);
  c05neg->SetFrameBorderMode(0);
  c05neg->SetLeftMargin( L/W );
  c05neg->SetRightMargin( R/W );
  c05neg->SetTopMargin( T/H );
  c05neg->SetBottomMargin( B/H );
  c05neg->SetTickx(0);
  c05neg->SetTicky(0);
 //end cms aesthetics
  
  c05neg->SetGridy();
  //c05neg->SetGridx(5);

  c05neg->cd();
TPaveText *t05neg=new TPaveText(0.145,0.88,0.37,0.94,"brNDC");
  t05neg->AddText("CMS Preliminary");
  t05neg->SetTextAlign(12);
  //Int_t trans_white = TColor::GetColorTransparent(0, 0);
  t05neg->SetShadowColor(trans_white);
  t05neg->SetFillColor(trans_white);
  t05neg->SetLineColor(trans_white);
  t05neg->SetLineColor(trans_white);
  //t05neg->AddText("more text");


  g_05GeVSegm_neg->GetYaxis()->SetRangeUser(0,.2);
  g_05GeVSegm_neg->GetXaxis()->SetRangeUser(-0.3,0.3);
  
  g_05GeVSegm_neg->GetXaxis()->SetTitleFont(42);
  g_05GeVSegm_neg->GetXaxis()->SetTitleSize(0.06);
  g_05GeVSegm_neg->GetXaxis()->SetTitleOffset(0.9);
  g_05GeVSegm_neg->GetXaxis()->SetLabelOffset(0.007);
  g_05GeVSegm_neg->GetXaxis()->SetLabelSize(0.05);
  g_05GeVSegm_neg->GetXaxis()->SetDecimals(kTRUE);
  g_05GeVSegm_neg->GetXaxis()->SetTickLength(0.03);
  //g_05GeVSegm_neg->GetYaxis()->SetTitleColor(kRed);
  g_05GeVSegm_neg->GetYaxis()->SetTitleFont(42);
  g_05GeVSegm_neg->GetYaxis()->SetTitleSize(0.06);
  g_05GeVSegm_neg->GetYaxis()->SetTitleOffset(1.2);
  g_05GeVSegm_neg->GetYaxis()->SetLabelOffset(0.007);
  g_05GeVSegm_neg->GetYaxis()->SetLabelSize(0.05);
  //g_05GeVSegm_neg->GetYaxis()->SetLabelColor(kRed);
  g_05GeVSegm_neg->GetYaxis()->SetDecimals(kTRUE);
  g_05GeVSegm_neg->GetYaxis()->SetTickLength(0.03);
  g_05GeVSegm_neg->GetXaxis()->SetTitle("#Delta #Phi");
  g_05GeVSegm_neg->GetYaxis()->SetTitle("Entries");
  //g_05GeVSegm_neg->GetYaxis()->SetRangeUser(0,1);
  //g_05GeVSegm_neg->GetYaxis()->SetLimits(0,1);
  //g_05GeVSegm_neg->GetXaxis()->SetLimits(1025/div,1600/div);
 // g_05GeVSegm_neg->Write(grTitle);


 g_05GeVSegm_neg->SetLineColor(kRed);     
 g_05GeVHit_neg->SetLineColor(kBlue);
 g_05GeVSegm_neg->Draw("HIST");     
 g_05GeVHit_neg->Draw("HISTSAMES");

 c05neg->Update(); //otherwise next commands don't work
 // default stats coordinates:
 // X1=.78  X2=.98
 // Y1=.775 Y2=0.935

 TPaveStats *st05neg = (TPaveStats*)g_05GeVSegm_neg->FindObject("stats");
 st05neg->SetY1NDC(.775-.070); //new x start position
 st05neg->SetY2NDC(.935-.070); //new x end position
 g_05GeVSegm_neg->SetName("from Segment #mu^{-} 5 GeV");
 st05neg = (TPaveStats*)g_05GeVHit_neg->FindObject("stats");
 st05neg->SetY1NDC(.755-.070); //new x start position
 st05neg->SetY2NDC(.595-.070); //new x end position
 g_05GeVHit_neg->SetName("from SimHit #mu^{-} 5 GeV");

 TLegend *leg05neg = new TLegend(0.18,0.765,0.6,0.865);
 leg05neg->SetTextSize(0.04);
 leg05neg->SetTextFont(42);
 leg05neg->AddEntry(g_05GeVSegm_neg, "from Segment #mu^{-} 5 GeV", "l");
 leg05neg->AddEntry(g_05GeVHit_neg, "from SimHit #mu^{-} 5 GeV", "l");
 leg05neg->Draw();

 t05neg->Draw("NB");
 c05neg->Update();


//================================ 10 GeV negMu  ==================================  
  
  
TCanvas *c10neg = new TCanvas("c10neg","negMu_10GeV", 50,50,W,H );    //canvas for 2 curves with tracking (one per gas)

 //cms aesthetics
  c10neg->SetFillColor(0);
  c10neg->SetBorderMode(0);
  c10neg->SetFrameFillStyle(0);
  c10neg->SetFrameBorderMode(0);
  c10neg->SetLeftMargin( L/W );
  c10neg->SetRightMargin( R/W );
  c10neg->SetTopMargin( T/H );
  c10neg->SetBottomMargin( B/H );
  c10neg->SetTickx(0);
  c10neg->SetTicky(0);
 //end cms aesthetics
  
  c10neg->SetGridy();
  //c10neg->SetGridx(5);

  c10neg->cd();
TPaveText *t10neg=new TPaveText(0.145,0.88,0.37,0.94,"brNDC");
  t10neg->AddText("CMS Preliminary");
  t10neg->SetTextAlign(12);
  //Int_t trans_white = TColor::GetColorTransparent(0, 0);
  t10neg->SetShadowColor(trans_white);
  t10neg->SetFillColor(trans_white);
  t10neg->SetLineColor(trans_white);
  t10neg->SetLineColor(trans_white);
  //t10neg->AddText("more text");


  g_10GeVSegm_neg->GetYaxis()->SetRangeUser(0,.2);
  g_10GeVSegm_neg->GetXaxis()->SetRangeUser(-0.3,0.3);
  
  g_10GeVSegm_neg->GetXaxis()->SetTitleFont(42);
  g_10GeVSegm_neg->GetXaxis()->SetTitleSize(0.06);
  g_10GeVSegm_neg->GetXaxis()->SetTitleOffset(0.9);
  g_10GeVSegm_neg->GetXaxis()->SetLabelOffset(0.007);
  g_10GeVSegm_neg->GetXaxis()->SetLabelSize(0.05);
  g_10GeVSegm_neg->GetXaxis()->SetDecimals(kTRUE);
  g_10GeVSegm_neg->GetXaxis()->SetTickLength(0.03);
  //g_10GeVSegm_neg->GetYaxis()->SetTitleColor(kRed);
  g_10GeVSegm_neg->GetYaxis()->SetTitleFont(42);
  g_10GeVSegm_neg->GetYaxis()->SetTitleSize(0.06);
  g_10GeVSegm_neg->GetYaxis()->SetTitleOffset(1.2);
  g_10GeVSegm_neg->GetYaxis()->SetLabelOffset(0.007);
  g_10GeVSegm_neg->GetYaxis()->SetLabelSize(0.05);
  //g_10GeVSegm_neg->GetYaxis()->SetLabelColor(kRed);
  g_10GeVSegm_neg->GetYaxis()->SetDecimals(kTRUE);
  g_10GeVSegm_neg->GetYaxis()->SetTickLength(0.03);
  g_10GeVSegm_neg->GetXaxis()->SetTitle("#Delta #Phi");
  g_10GeVSegm_neg->GetYaxis()->SetTitle("Entries");
  //g_10GeVSegm_neg->GetYaxis()->SetRangeUser(0,1);
  //g_10GeVSegm_neg->GetYaxis()->SetLimits(0,1);
  //g_10GeVSegm_neg->GetXaxis()->SetLimits(1025/div,1600/div);
 // g_10GeVSegm_neg->Write(grTitle);


 g_10GeVSegm_neg->SetLineColor(kRed);     
 g_10GeVHit_neg->SetLineColor(kBlue);
 g_10GeVSegm_neg->Draw("HIST");     
 g_10GeVHit_neg->Draw("HISTSAMES");

 c10neg->Update(); //otherwise next commands don't work
 // default stats coordinates:
 // X1=.78  X2=.98
 // Y1=.775 Y2=0.935

 TPaveStats *st10neg = (TPaveStats*)g_10GeVSegm_neg->FindObject("stats");
 st10neg->SetY1NDC(.775-.070); //new x start position
 st10neg->SetY2NDC(.935-.070); //new x end position
 g_10GeVSegm_neg->SetName("from Segment #mu^{-} 10 GeV");
 st10neg = (TPaveStats*)g_10GeVHit_neg->FindObject("stats");
 st10neg->SetY1NDC(.755-.070); //new x start position
 st10neg->SetY2NDC(.595-.070); //new x end position
 g_10GeVHit_neg->SetName("from SimHit #mu^{-} 10 GeV");

 TLegend *leg10neg = new TLegend(0.18,0.765,0.6,0.865);
 leg10neg->SetTextSize(0.04);
 leg10neg->SetTextFont(42);
 leg10neg->AddEntry(g_10GeVSegm_neg, "from Segment #mu^{-} 10 GeV", "l");
 leg10neg->AddEntry(g_10GeVHit_neg, "from SimHit #mu^{-} 10 GeV", "l");
 leg10neg->Draw();

 t10neg->Draw("NB");
 c10neg->Update();


//================================ 20 GeV negMu  ==================================  
  
  
TCanvas *c20neg = new TCanvas("c20neg","negMu_20GeV", 50,50,W,H );    //canvas for 2 curves with tracking (one per gas)

 //cms aesthetics
  c20neg->SetFillColor(0);
  c20neg->SetBorderMode(0);
  c20neg->SetFrameFillStyle(0);
  c20neg->SetFrameBorderMode(0);
  c20neg->SetLeftMargin( L/W );
  c20neg->SetRightMargin( R/W );
  c20neg->SetTopMargin( T/H );
  c20neg->SetBottomMargin( B/H );
  c20neg->SetTickx(0);
  c20neg->SetTicky(0);
 //end cms aesthetics
  
  c20neg->SetGridy();
  //c20neg->SetGridx(5);

  c20neg->cd();
TPaveText *t20neg=new TPaveText(0.145,0.88,0.37,0.94,"brNDC");
  t20neg->AddText("CMS Preliminary");
  t20neg->SetTextAlign(12);
  //Int_t trans_white = TColor::GetColorTransparent(0, 0);
  t20neg->SetShadowColor(trans_white);
  t20neg->SetFillColor(trans_white);
  t20neg->SetLineColor(trans_white);
  t20neg->SetLineColor(trans_white);
  //t20neg->AddText("more text");


  g_20GeVSegm_neg->GetYaxis()->SetRangeUser(0,.34);
  g_20GeVSegm_neg->GetXaxis()->SetRangeUser(-0.3,0.3);
  
  g_20GeVSegm_neg->GetXaxis()->SetTitleFont(42);
  g_20GeVSegm_neg->GetXaxis()->SetTitleSize(0.06);
  g_20GeVSegm_neg->GetXaxis()->SetTitleOffset(0.9);
  g_20GeVSegm_neg->GetXaxis()->SetLabelOffset(0.007);
  g_20GeVSegm_neg->GetXaxis()->SetLabelSize(0.05);
  g_20GeVSegm_neg->GetXaxis()->SetDecimals(kTRUE);
  g_20GeVSegm_neg->GetXaxis()->SetTickLength(0.03);
  //g_20GeVSegm_neg->GetYaxis()->SetTitleColor(kRed);
  g_20GeVSegm_neg->GetYaxis()->SetTitleFont(42);
  g_20GeVSegm_neg->GetYaxis()->SetTitleSize(0.06);
  g_20GeVSegm_neg->GetYaxis()->SetTitleOffset(1.2);
  g_20GeVSegm_neg->GetYaxis()->SetLabelOffset(0.007);
  g_20GeVSegm_neg->GetYaxis()->SetLabelSize(0.05);
  //g_20GeVSegm_neg->GetYaxis()->SetLabelColor(kRed);
  g_20GeVSegm_neg->GetYaxis()->SetDecimals(kTRUE);
  g_20GeVSegm_neg->GetYaxis()->SetTickLength(0.03);
  g_20GeVSegm_neg->GetXaxis()->SetTitle("#Delta #Phi");
  g_20GeVSegm_neg->GetYaxis()->SetTitle("Entries");
  //g_20GeVSegm_neg->GetYaxis()->SetRangeUser(0,1);
  //g_20GeVSegm_neg->GetYaxis()->SetLimits(0,1);
  //g_20GeVSegm_neg->GetXaxis()->SetLimits(1025/div,1600/div);
 // g_20GeVSegm_neg->Write(grTitle);


 g_20GeVSegm_neg->SetLineColor(kRed);     
 g_20GeVHit_neg->SetLineColor(kBlue);
 g_20GeVSegm_neg->Draw("HIST");     
 g_20GeVHit_neg->Draw("HISTSAMES");

 c20neg->Update(); //otherwise next commands don't work
 // default stats coordinates:
 // X1=.78  X2=.98
 // Y1=.775 Y2=0.935

 TPaveStats *st20neg = (TPaveStats*)g_20GeVSegm_neg->FindObject("stats");
 st20neg->SetY1NDC(.775-.070); //new x start position
 st20neg->SetY2NDC(.935-.070); //new x end position
 g_20GeVSegm_neg->SetName("from Segment #mu^{-} 20 GeV");
 st20neg = (TPaveStats*)g_20GeVHit_neg->FindObject("stats");
 st20neg->SetY1NDC(.755-.070); //new x start position
 st20neg->SetY2NDC(.595-.070); //new x end position
 g_20GeVHit_neg->SetName("from SimHit #mu^{-} 20 GeV");

 TLegend *leg20neg = new TLegend(0.18,0.765,0.6,0.865);
 leg20neg->SetTextSize(0.04);
 leg20neg->SetTextFont(42);
 leg20neg->AddEntry(g_20GeVSegm_neg, "from Segment #mu^{-} 20 GeV", "l");
 leg20neg->AddEntry(g_20GeVHit_neg, "from SimHit #mu^{-} 20 GeV", "l");
 leg20neg->Draw();

 t20neg->Draw("NB");
 c20neg->Update();


//================================ 50 GeV negMu  ==================================  
  
  
TCanvas *c50neg = new TCanvas("c50neg","negMu_50GeV", 50,50,W,H );    //canvas for 2 curves with tracking (one per gas)

 //cms aesthetics
  c50neg->SetFillColor(0);
  c50neg->SetBorderMode(0);
  c50neg->SetFrameFillStyle(0);
  c50neg->SetFrameBorderMode(0);
  c50neg->SetLeftMargin( L/W );
  c50neg->SetRightMargin( R/W );
  c50neg->SetTopMargin( T/H );
  c50neg->SetBottomMargin( B/H );
  c50neg->SetTickx(0);
  c50neg->SetTicky(0);
 //end cms aesthetics
  
  c50neg->SetGridy();
  //c50neg->SetGridx(5);

  c50neg->cd();
TPaveText *t50neg=new TPaveText(0.145,0.88,0.37,0.94,"brNDC");
  t50neg->AddText("CMS Preliminary");
  t50neg->SetTextAlign(12);
  //Int_t trans_white = TColor::GetColorTransparent(0, 0);
  t50neg->SetShadowColor(trans_white);
  t50neg->SetFillColor(trans_white);
  t50neg->SetLineColor(trans_white);
  t50neg->SetLineColor(trans_white);
  //t50neg->AddText("more text");


  g_50GeVSegm_neg->GetYaxis()->SetRangeUser(0,.4);
  g_50GeVSegm_neg->GetXaxis()->SetRangeUser(-0.3,0.3);
  
  g_50GeVSegm_neg->GetXaxis()->SetTitleFont(42);
  g_50GeVSegm_neg->GetXaxis()->SetTitleSize(0.06);
  g_50GeVSegm_neg->GetXaxis()->SetTitleOffset(0.9);
  g_50GeVSegm_neg->GetXaxis()->SetLabelOffset(0.007);
  g_50GeVSegm_neg->GetXaxis()->SetLabelSize(0.05);
  g_50GeVSegm_neg->GetXaxis()->SetDecimals(kTRUE);
  g_50GeVSegm_neg->GetXaxis()->SetTickLength(0.03);
  //g_50GeVSegm_neg->GetYaxis()->SetTitleColor(kRed);
  g_50GeVSegm_neg->GetYaxis()->SetTitleFont(42);
  g_50GeVSegm_neg->GetYaxis()->SetTitleSize(0.06);
  g_50GeVSegm_neg->GetYaxis()->SetTitleOffset(1.2);
  g_50GeVSegm_neg->GetYaxis()->SetLabelOffset(0.007);
  g_50GeVSegm_neg->GetYaxis()->SetLabelSize(0.05);
  //g_50GeVSegm_neg->GetYaxis()->SetLabelColor(kRed);
  g_50GeVSegm_neg->GetYaxis()->SetDecimals(kTRUE);
  g_50GeVSegm_neg->GetYaxis()->SetTickLength(0.03);
  g_50GeVSegm_neg->GetXaxis()->SetTitle("#Delta #Phi");
  g_50GeVSegm_neg->GetYaxis()->SetTitle("Entries");
  //g_50GeVSegm_neg->GetYaxis()->SetRangeUser(0,1);
  //g_50GeVSegm_neg->GetYaxis()->SetLimits(0,1);
  //g_50GeVSegm_neg->GetXaxis()->SetLimits(1025/div,1600/div);
 // g_50GeVSegm_neg->Write(grTitle);


 g_50GeVSegm_neg->SetLineColor(kRed);     
 g_50GeVHit_neg->SetLineColor(kBlue);
 g_50GeVSegm_neg->Draw("HIST");     
 g_50GeVHit_neg->Draw("HISTSAMES");

 c50neg->Update(); //otherwise next commands don't work
 // default stats coordinates:
 // X1=.78  X2=.98
 // Y1=.775 Y2=0.935

 TPaveStats *st50neg = (TPaveStats*)g_50GeVSegm_neg->FindObject("stats");
 st50neg->SetY1NDC(.775-.070); //new x start position
 st50neg->SetY2NDC(.935-.070); //new x end position
 g_50GeVSegm_neg->SetName("from Segment #mu^{-} 50 GeV");
 st50neg = (TPaveStats*)g_50GeVHit_neg->FindObject("stats");
 st50neg->SetY1NDC(.755-.070); //new x start position
 st50neg->SetY2NDC(.595-.070); //new x end position
 g_50GeVHit_neg->SetName("from SimHit #mu^{-} 50 GeV");

 TLegend *leg50neg = new TLegend(0.18,0.765,0.6,0.865);
 leg50neg->SetTextSize(0.04);
 leg50neg->SetTextFont(42);
 leg50neg->AddEntry(g_50GeVSegm_neg, "from Segment #mu^{-} 50 GeV", "l");
 leg50neg->AddEntry(g_50GeVHit_neg, "from SimHit #mu^{-} 50 GeV", "l");
 leg50neg->Draw();

 t50neg->Draw("NB");
 c50neg->Update();





//================================ 03 GeV posMu  ==================================  
  
  
TCanvas *c03pos = new TCanvas("c03pos","posMu_03GeV", 50,50,W,H );    //canvas for 2 curves with tracking (one per gas)

 //cms aesthetics
  c03pos->SetFillColor(0);
  c03pos->SetBorderMode(0);
  c03pos->SetFrameFillStyle(0);
  c03pos->SetFrameBorderMode(0);
  c03pos->SetLeftMargin( L/W );
  c03pos->SetRightMargin( R/W );
  c03pos->SetTopMargin( T/H );
  c03pos->SetBottomMargin( B/H );
  c03pos->SetTickx(0);
  c03pos->SetTicky(0);
 //end cms aesthetics
  
  c03pos->SetGridy();
  //c03pos->SetGridx(5);

  c03pos->cd();
TPaveText *t03pos=new TPaveText(0.145,0.88,0.37,0.94,"brNDC");
  t03pos->AddText("CMS Preliminary");
  t03pos->SetTextAlign(12);
  //Int_t trans_white = TColor::GetColorTransparent(0, 0);
  t03pos->SetShadowColor(trans_white);
  t03pos->SetFillColor(trans_white);
  t03pos->SetLineColor(trans_white);
  t03pos->SetLineColor(trans_white);
  //t03pos->AddText("more text");


  g_03GeVSegm_pos->GetYaxis()->SetRangeUser(0,.2);
  g_03GeVSegm_pos->GetXaxis()->SetRangeUser(-0.3,0.3);
  
  g_03GeVSegm_pos->GetXaxis()->SetTitleFont(42);
  g_03GeVSegm_pos->GetXaxis()->SetTitleSize(0.06);
  g_03GeVSegm_pos->GetXaxis()->SetTitleOffset(0.9);
  g_03GeVSegm_pos->GetXaxis()->SetLabelOffset(0.007);
  g_03GeVSegm_pos->GetXaxis()->SetLabelSize(0.05);
  g_03GeVSegm_pos->GetXaxis()->SetDecimals(kTRUE);
  g_03GeVSegm_pos->GetXaxis()->SetTickLength(0.03);
  //g_03GeVSegm_pos->GetYaxis()->SetTitleColor(kRed);
  g_03GeVSegm_pos->GetYaxis()->SetTitleFont(42);
  g_03GeVSegm_pos->GetYaxis()->SetTitleSize(0.06);
  g_03GeVSegm_pos->GetYaxis()->SetTitleOffset(1.2);
  g_03GeVSegm_pos->GetYaxis()->SetLabelOffset(0.007);
  g_03GeVSegm_pos->GetYaxis()->SetLabelSize(0.05);
  //g_03GeVSegm_pos->GetYaxis()->SetLabelColor(kRed);
  g_03GeVSegm_pos->GetYaxis()->SetDecimals(kTRUE);
  g_03GeVSegm_pos->GetYaxis()->SetTickLength(0.03);
  g_03GeVSegm_pos->GetXaxis()->SetTitle("#Delta #Phi");
  g_03GeVSegm_pos->GetYaxis()->SetTitle("Entries");
  //g_03GeVSegm_pos->GetYaxis()->SetRangeUser(0,1);
  //g_03GeVSegm_pos->GetYaxis()->SetLimits(0,1);
  //g_03GeVSegm_pos->GetXaxis()->SetLimits(1025/div,1600/div);
 // g_03GeVSegm_pos->Write(grTitle);


 g_03GeVSegm_pos->SetLineColor(kRed);     
 g_03GeVHit_pos->SetLineColor(kBlue);
 g_03GeVSegm_pos->Draw("HIST");     
 g_03GeVHit_pos->Draw("HISTSAMES");

 c03pos->Update(); //otherwise next commands don't work
 // default stats coordinates:
 // X1=.78  X2=.98
 // Y1=.775 Y2=0.935

 TPaveStats *st03pos = (TPaveStats*)g_03GeVSegm_pos->FindObject("stats");
 st03pos->SetY1NDC(.775-.070); //new x start position
 st03pos->SetY2NDC(.935-.070); //new x end position
 g_03GeVSegm_pos->SetName("from Segment #mu^{+} 3 GeV");
 st03pos = (TPaveStats*)g_03GeVHit_pos->FindObject("stats");
 st03pos->SetY1NDC(.755-.070); //new x start position
 st03pos->SetY2NDC(.595-.070); //new x end position
 g_03GeVHit_pos->SetName("from SimHit #mu^{+} 3 GeV");

 TLegend *leg03pos = new TLegend(0.18,0.765,0.6,0.865);
 leg03pos->SetTextSize(0.04);
 leg03pos->SetTextFont(42);
 leg03pos->AddEntry(g_03GeVSegm_pos, "from Segment #mu^{+} 3 GeV", "l");
 leg03pos->AddEntry(g_03GeVHit_pos, "from SimHit #mu^{+} 3 GeV", "l");
 leg03pos->Draw();

 t03pos->Draw("NB");
 c03pos->Update();


//================================ 05 GeV posMu  ==================================  
  
  
TCanvas *c05pos = new TCanvas("c05pos","posMu_05GeV", 50,50,W,H );    //canvas for 2 curves with tracking (one per gas)

 //cms aesthetics
  c05pos->SetFillColor(0);
  c05pos->SetBorderMode(0);
  c05pos->SetFrameFillStyle(0);
  c05pos->SetFrameBorderMode(0);
  c05pos->SetLeftMargin( L/W );
  c05pos->SetRightMargin( R/W );
  c05pos->SetTopMargin( T/H );
  c05pos->SetBottomMargin( B/H );
  c05pos->SetTickx(0);
  c05pos->SetTicky(0);
 //end cms aesthetics
  
  c05pos->SetGridy();
  //c05pos->SetGridx(5);

  c05pos->cd();
TPaveText *t05pos=new TPaveText(0.145,0.88,0.37,0.94,"brNDC");
  t05pos->AddText("CMS Preliminary");
  t05pos->SetTextAlign(12);
  //Int_t trans_white = TColor::GetColorTransparent(0, 0);
  t05pos->SetShadowColor(trans_white);
  t05pos->SetFillColor(trans_white);
  t05pos->SetLineColor(trans_white);
  t05pos->SetLineColor(trans_white);
  //t05pos->AddText("more text");


  g_05GeVSegm_pos->GetYaxis()->SetRangeUser(0,.2);
  g_05GeVSegm_pos->GetXaxis()->SetRangeUser(-0.3,0.3);
  
  g_05GeVSegm_pos->GetXaxis()->SetTitleFont(42);
  g_05GeVSegm_pos->GetXaxis()->SetTitleSize(0.06);
  g_05GeVSegm_pos->GetXaxis()->SetTitleOffset(0.9);
  g_05GeVSegm_pos->GetXaxis()->SetLabelOffset(0.007);
  g_05GeVSegm_pos->GetXaxis()->SetLabelSize(0.05);
  g_05GeVSegm_pos->GetXaxis()->SetDecimals(kTRUE);
  g_05GeVSegm_pos->GetXaxis()->SetTickLength(0.03);
  //g_05GeVSegm_pos->GetYaxis()->SetTitleColor(kRed);
  g_05GeVSegm_pos->GetYaxis()->SetTitleFont(42);
  g_05GeVSegm_pos->GetYaxis()->SetTitleSize(0.06);
  g_05GeVSegm_pos->GetYaxis()->SetTitleOffset(1.2);
  g_05GeVSegm_pos->GetYaxis()->SetLabelOffset(0.007);
  g_05GeVSegm_pos->GetYaxis()->SetLabelSize(0.05);
  //g_05GeVSegm_pos->GetYaxis()->SetLabelColor(kRed);
  g_05GeVSegm_pos->GetYaxis()->SetDecimals(kTRUE);
  g_05GeVSegm_pos->GetYaxis()->SetTickLength(0.03);
  g_05GeVSegm_pos->GetXaxis()->SetTitle("#Delta #Phi");
  g_05GeVSegm_pos->GetYaxis()->SetTitle("Entries");
  //g_05GeVSegm_pos->GetYaxis()->SetRangeUser(0,1);
  //g_05GeVSegm_pos->GetYaxis()->SetLimits(0,1);
  //g_05GeVSegm_pos->GetXaxis()->SetLimits(1025/div,1600/div);
 // g_05GeVSegm_pos->Write(grTitle);


 g_05GeVSegm_pos->SetLineColor(kRed);     
 g_05GeVHit_pos->SetLineColor(kBlue);
 g_05GeVSegm_pos->Draw("HIST");     
 g_05GeVHit_pos->Draw("HISTSAMES");

 c05pos->Update(); //otherwise next commands don't work
 // default stats coordinates:
 // X1=.78  X2=.98
 // Y1=.775 Y2=0.935

 TPaveStats *st05pos = (TPaveStats*)g_05GeVSegm_pos->FindObject("stats");
 st05pos->SetY1NDC(.775-.070); //new x start position
 st05pos->SetY2NDC(.935-.070); //new x end position
 g_05GeVSegm_pos->SetName("from Segment #mu^{+} 5 GeV");
 st05pos = (TPaveStats*)g_05GeVHit_pos->FindObject("stats");
 st05pos->SetY1NDC(.755-.070); //new x start position
 st05pos->SetY2NDC(.595-.070); //new x end position
 g_05GeVHit_pos->SetName("from SimHit #mu^{+} 5 GeV");

 TLegend *leg05pos = new TLegend(0.18,0.765,0.6,0.865);
 leg05pos->SetTextSize(0.04);
 leg05pos->SetTextFont(42);
 leg05pos->AddEntry(g_05GeVSegm_pos, "from Segment #mu^{+} 5 GeV", "l");
 leg05pos->AddEntry(g_05GeVHit_pos, "from SimHit #mu^{+} 5 GeV", "l");
 leg05pos->Draw();

 t05pos->Draw("NB");
 c05pos->Update();


//================================ 10 GeV posMu  ==================================  
  
  
TCanvas *c10pos = new TCanvas("c10pos","posMu_10GeV", 50,50,W,H );    //canvas for 2 curves with tracking (one per gas)

 //cms aesthetics
  c10pos->SetFillColor(0);
  c10pos->SetBorderMode(0);
  c10pos->SetFrameFillStyle(0);
  c10pos->SetFrameBorderMode(0);
  c10pos->SetLeftMargin( L/W );
  c10pos->SetRightMargin( R/W );
  c10pos->SetTopMargin( T/H );
  c10pos->SetBottomMargin( B/H );
  c10pos->SetTickx(0);
  c10pos->SetTicky(0);
 //end cms aesthetics
  
  c10pos->SetGridy();
  //c10pos->SetGridx(5);

  c10pos->cd();
TPaveText *t10pos=new TPaveText(0.145,0.88,0.37,0.94,"brNDC");
  t10pos->AddText("CMS Preliminary");
  t10pos->SetTextAlign(12);
  //Int_t trans_white = TColor::GetColorTransparent(0, 0);
  t10pos->SetShadowColor(trans_white);
  t10pos->SetFillColor(trans_white);
  t10pos->SetLineColor(trans_white);
  t10pos->SetLineColor(trans_white);
  //t10pos->AddText("more text");


  g_10GeVSegm_pos->GetYaxis()->SetRangeUser(0,.2);
  g_10GeVSegm_pos->GetXaxis()->SetRangeUser(-0.3,0.3);
  
  g_10GeVSegm_pos->GetXaxis()->SetTitleFont(42);
  g_10GeVSegm_pos->GetXaxis()->SetTitleSize(0.06);
  g_10GeVSegm_pos->GetXaxis()->SetTitleOffset(0.9);
  g_10GeVSegm_pos->GetXaxis()->SetLabelOffset(0.007);
  g_10GeVSegm_pos->GetXaxis()->SetLabelSize(0.05);
  g_10GeVSegm_pos->GetXaxis()->SetDecimals(kTRUE);
  g_10GeVSegm_pos->GetXaxis()->SetTickLength(0.03);
  //g_10GeVSegm_pos->GetYaxis()->SetTitleColor(kRed);
  g_10GeVSegm_pos->GetYaxis()->SetTitleFont(42);
  g_10GeVSegm_pos->GetYaxis()->SetTitleSize(0.06);
  g_10GeVSegm_pos->GetYaxis()->SetTitleOffset(1.2);
  g_10GeVSegm_pos->GetYaxis()->SetLabelOffset(0.007);
  g_10GeVSegm_pos->GetYaxis()->SetLabelSize(0.05);
  //g_10GeVSegm_pos->GetYaxis()->SetLabelColor(kRed);
  g_10GeVSegm_pos->GetYaxis()->SetDecimals(kTRUE);
  g_10GeVSegm_pos->GetYaxis()->SetTickLength(0.03);
  g_10GeVSegm_pos->GetXaxis()->SetTitle("#Delta #Phi");
  g_10GeVSegm_pos->GetYaxis()->SetTitle("Entries");
  //g_10GeVSegm_pos->GetYaxis()->SetRangeUser(0,1);
  //g_10GeVSegm_pos->GetYaxis()->SetLimits(0,1);
  //g_10GeVSegm_pos->GetXaxis()->SetLimits(1025/div,1600/div);
 // g_10GeVSegm_pos->Write(grTitle);


 g_10GeVSegm_pos->SetLineColor(kRed);     
 g_10GeVHit_pos->SetLineColor(kBlue);
 g_10GeVSegm_pos->Draw("HIST");     
 g_10GeVHit_pos->Draw("HISTSAMES");

 c10pos->Update(); //otherwise next commands don't work
 // default stats coordinates:
 // X1=.78  X2=.98
 // Y1=.775 Y2=0.935

 TPaveStats *st10pos = (TPaveStats*)g_10GeVSegm_pos->FindObject("stats");
 st10pos->SetY1NDC(.775-.070); //new x start position
 st10pos->SetY2NDC(.935-.070); //new x end position
 g_10GeVSegm_pos->SetName("from Segment #mu^{+} 10 GeV");
 st10pos = (TPaveStats*)g_10GeVHit_pos->FindObject("stats");
 st10pos->SetY1NDC(.755-.070); //new x start position
 st10pos->SetY2NDC(.595-.070); //new x end position
 g_10GeVHit_pos->SetName("from SimHit #mu^{+} 10 GeV");

 TLegend *leg10pos = new TLegend(0.18,0.765,0.6,0.865);
 leg10pos->SetTextSize(0.04);
 leg10pos->SetTextFont(42);
 leg10pos->AddEntry(g_10GeVSegm_pos, "from Segment #mu^{+} 10 GeV", "l");
 leg10pos->AddEntry(g_10GeVHit_pos, "from SimHit #mu^{+} 10 GeV", "l");
 leg10pos->Draw();

 t10pos->Draw("NB");
 c10pos->Update();


//================================ 20 GeV posMu  ==================================  
  
  
TCanvas *c20pos = new TCanvas("c20pos","posMu_20GeV", 50,50,W,H );    //canvas for 2 curves with tracking (one per gas)

 //cms aesthetics
  c20pos->SetFillColor(0);
  c20pos->SetBorderMode(0);
  c20pos->SetFrameFillStyle(0);
  c20pos->SetFrameBorderMode(0);
  c20pos->SetLeftMargin( L/W );
  c20pos->SetRightMargin( R/W );
  c20pos->SetTopMargin( T/H );
  c20pos->SetBottomMargin( B/H );
  c20pos->SetTickx(0);
  c20pos->SetTicky(0);
 //end cms aesthetics
  
  c20pos->SetGridy();
  //c20pos->SetGridx(5);

  c20pos->cd();
TPaveText *t20pos=new TPaveText(0.145,0.88,0.37,0.94,"brNDC");
  t20pos->AddText("CMS Preliminary");
  t20pos->SetTextAlign(12);
  //Int_t trans_white = TColor::GetColorTransparent(0, 0);
  t20pos->SetShadowColor(trans_white);
  t20pos->SetFillColor(trans_white);
  t20pos->SetLineColor(trans_white);
  t20pos->SetLineColor(trans_white);
  //t20pos->AddText("more text");


  g_20GeVSegm_pos->GetYaxis()->SetRangeUser(0,.34);
  g_20GeVSegm_pos->GetXaxis()->SetRangeUser(-0.3,0.3);
  
  g_20GeVSegm_pos->GetXaxis()->SetTitleFont(42);
  g_20GeVSegm_pos->GetXaxis()->SetTitleSize(0.06);
  g_20GeVSegm_pos->GetXaxis()->SetTitleOffset(0.9);
  g_20GeVSegm_pos->GetXaxis()->SetLabelOffset(0.007);
  g_20GeVSegm_pos->GetXaxis()->SetLabelSize(0.05);
  g_20GeVSegm_pos->GetXaxis()->SetDecimals(kTRUE);
  g_20GeVSegm_pos->GetXaxis()->SetTickLength(0.03);
  //g_20GeVSegm_pos->GetYaxis()->SetTitleColor(kRed);
  g_20GeVSegm_pos->GetYaxis()->SetTitleFont(42);
  g_20GeVSegm_pos->GetYaxis()->SetTitleSize(0.06);
  g_20GeVSegm_pos->GetYaxis()->SetTitleOffset(1.2);
  g_20GeVSegm_pos->GetYaxis()->SetLabelOffset(0.007);
  g_20GeVSegm_pos->GetYaxis()->SetLabelSize(0.05);
  //g_20GeVSegm_pos->GetYaxis()->SetLabelColor(kRed);
  g_20GeVSegm_pos->GetYaxis()->SetDecimals(kTRUE);
  g_20GeVSegm_pos->GetYaxis()->SetTickLength(0.03);
  g_20GeVSegm_pos->GetXaxis()->SetTitle("#Delta #Phi");
  g_20GeVSegm_pos->GetYaxis()->SetTitle("Entries");
  //g_20GeVSegm_pos->GetYaxis()->SetRangeUser(0,1);
  //g_20GeVSegm_pos->GetYaxis()->SetLimits(0,1);
  //g_20GeVSegm_pos->GetXaxis()->SetLimits(1025/div,1600/div);
 // g_20GeVSegm_pos->Write(grTitle);


 g_20GeVSegm_pos->SetLineColor(kRed);     
 g_20GeVHit_pos->SetLineColor(kBlue);
 g_20GeVSegm_pos->Draw("HIST");     
 g_20GeVHit_pos->Draw("HISTSAMES");

 c20pos->Update(); //otherwise next commands don't work
 // default stats coordinates:
 // X1=.78  X2=.98
 // Y1=.775 Y2=0.935

 TPaveStats *st20pos = (TPaveStats*)g_20GeVSegm_pos->FindObject("stats");
 st20pos->SetY1NDC(.775-.070); //new x start position
 st20pos->SetY2NDC(.935-.070); //new x end position
 g_20GeVSegm_pos->SetName("from Segment #mu^{+} 20 GeV");
 st20pos = (TPaveStats*)g_20GeVHit_pos->FindObject("stats");
 st20pos->SetY1NDC(.755-.070); //new x start position
 st20pos->SetY2NDC(.595-.070); //new x end position
 g_20GeVHit_pos->SetName("from SimHit #mu^{+} 20 GeV");

 TLegend *leg20pos = new TLegend(0.18,0.765,0.6,0.865);
 leg20pos->SetTextSize(0.04);
 leg20pos->SetTextFont(42);
 leg20pos->AddEntry(g_20GeVSegm_pos, "from Segment #mu^{+} 20 GeV", "l");
 leg20pos->AddEntry(g_20GeVHit_pos, "from SimHit #mu^{+} 20 GeV", "l");
 leg20pos->Draw();

 t20pos->Draw("NB");
 c20pos->Update();


//================================ 50 GeV posMu  ==================================  
  
  
TCanvas *c50pos = new TCanvas("c50pos","posMu_50GeV", 50,50,W,H );    //canvas for 2 curves with tracking (one per gas)

 //cms aesthetics
  c50pos->SetFillColor(0);
  c50pos->SetBorderMode(0);
  c50pos->SetFrameFillStyle(0);
  c50pos->SetFrameBorderMode(0);
  c50pos->SetLeftMargin( L/W );
  c50pos->SetRightMargin( R/W );
  c50pos->SetTopMargin( T/H );
  c50pos->SetBottomMargin( B/H );
  c50pos->SetTickx(0);
  c50pos->SetTicky(0);
 //end cms aesthetics
  
  c50pos->SetGridy();
  //c50pos->SetGridx(5);

  c50pos->cd();
TPaveText *t50pos=new TPaveText(0.145,0.88,0.37,0.94,"brNDC");
  t50pos->AddText("CMS Preliminary");
  t50pos->SetTextAlign(12);
  //Int_t trans_white = TColor::GetColorTransparent(0, 0);
  t50pos->SetShadowColor(trans_white);
  t50pos->SetFillColor(trans_white);
  t50pos->SetLineColor(trans_white);
  t50pos->SetLineColor(trans_white);
  //t50pos->AddText("more text");


  g_50GeVSegm_pos->GetYaxis()->SetRangeUser(0,.4);
  g_50GeVSegm_pos->GetXaxis()->SetRangeUser(-0.3,0.3);
  
  g_50GeVSegm_pos->GetXaxis()->SetTitleFont(42);
  g_50GeVSegm_pos->GetXaxis()->SetTitleSize(0.06);
  g_50GeVSegm_pos->GetXaxis()->SetTitleOffset(0.9);
  g_50GeVSegm_pos->GetXaxis()->SetLabelOffset(0.007);
  g_50GeVSegm_pos->GetXaxis()->SetLabelSize(0.05);
  g_50GeVSegm_pos->GetXaxis()->SetDecimals(kTRUE);
  g_50GeVSegm_pos->GetXaxis()->SetTickLength(0.03);
  //g_50GeVSegm_pos->GetYaxis()->SetTitleColor(kRed);
  g_50GeVSegm_pos->GetYaxis()->SetTitleFont(42);
  g_50GeVSegm_pos->GetYaxis()->SetTitleSize(0.06);
  g_50GeVSegm_pos->GetYaxis()->SetTitleOffset(1.2);
  g_50GeVSegm_pos->GetYaxis()->SetLabelOffset(0.007);
  g_50GeVSegm_pos->GetYaxis()->SetLabelSize(0.05);
  //g_50GeVSegm_pos->GetYaxis()->SetLabelColor(kRed);
  g_50GeVSegm_pos->GetYaxis()->SetDecimals(kTRUE);
  g_50GeVSegm_pos->GetYaxis()->SetTickLength(0.03);
  g_50GeVSegm_pos->GetXaxis()->SetTitle("#Delta #Phi");
  g_50GeVSegm_pos->GetYaxis()->SetTitle("Entries");
  //g_50GeVSegm_pos->GetYaxis()->SetRangeUser(0,1);
  //g_50GeVSegm_pos->GetYaxis()->SetLimits(0,1);
  //g_50GeVSegm_pos->GetXaxis()->SetLimits(1025/div,1600/div);
 // g_50GeVSegm_pos->Write(grTitle);


 g_50GeVSegm_pos->SetLineColor(kRed);     
 g_50GeVHit_pos->SetLineColor(kBlue);
 g_50GeVSegm_pos->Draw("HIST");     
 g_50GeVHit_pos->Draw("HISTSAMES");

 c50pos->Update(); //otherwise next commands don't work
 // default stats coordinates:
 // X1=.78  X2=.98
 // Y1=.775 Y2=0.935

 TPaveStats *st50pos = (TPaveStats*)g_50GeVSegm_pos->FindObject("stats");
 st50pos->SetY1NDC(.775-.070); //new x start position
 st50pos->SetY2NDC(.935-.070); //new x end position
 g_50GeVSegm_pos->SetName("from Segment #mu^{+} 50 GeV");
 st50pos = (TPaveStats*)g_50GeVHit_pos->FindObject("stats");
 st50pos->SetY1NDC(.755-.070); //new x start position
 st50pos->SetY2NDC(.595-.070); //new x end position
 g_50GeVHit_pos->SetName("from SimHit #mu^{+} 50 GeV");

 TLegend *leg50pos = new TLegend(0.18,0.765,0.6,0.865);
 leg50pos->SetTextSize(0.04);
 leg50pos->SetTextFont(42);
 leg50pos->AddEntry(g_50GeVSegm_pos, "from Segment #mu^{+} 50 GeV", "l");
 leg50pos->AddEntry(g_50GeVHit_pos, "from SimHit #mu^{+} 50 GeV", "l");
 leg50pos->Draw();

 t50pos->Draw("NB");
 c50pos->Update();











// Double_t sdev_03_neg = g_03GeVSegm_neg->GetStdDev();
 Double_t sdev_05_neg = g_05GeVSegm_neg->GetStdDev();
 Double_t sdev_10_neg = g_10GeVSegm_neg->GetStdDev();
 Double_t sdev_20_neg = g_20GeVSegm_neg->GetStdDev();
 Double_t sdev_30_neg = g_50GeVSegm_neg->GetStdDev();

// Double_t sdev_03_pos = g_03GeVSegm_pos->GetStdDev();
 Double_t sdev_05_pos = g_05GeVSegm_pos->GetStdDev();
 Double_t sdev_10_pos = g_10GeVSegm_pos->GetStdDev();
 Double_t sdev_20_pos = g_20GeVSegm_pos->GetStdDev();
 Double_t sdev_30_pos = g_50GeVSegm_pos->GetStdDev();

// Double_t mean_03_neg = g_03GeVSegm_neg->GetMean();
 Double_t mean_05_neg = g_05GeVSegm_neg->GetMean();
 Double_t mean_10_neg = g_10GeVSegm_neg->GetMean();
 Double_t mean_20_neg = g_20GeVSegm_neg->GetMean();
 Double_t mean_30_neg = g_50GeVSegm_neg->GetMean();

// Double_t mean_03_pos = g_03GeVSegm_pos->GetMean();
 Double_t mean_05_pos = g_05GeVSegm_pos->GetMean();
 Double_t mean_10_pos = g_10GeVSegm_pos->GetMean();
 Double_t mean_20_pos = g_20GeVSegm_pos->GetMean();
 Double_t mean_30_pos = g_50GeVSegm_pos->GetMean();

// Double_t errmean_03_neg = g_03GeVSegm_neg->GetMeanError();
 Double_t errmean_05_neg = g_05GeVSegm_neg->GetMeanError();
 Double_t errmean_10_neg = g_10GeVSegm_neg->GetMeanError();
 Double_t errmean_20_neg = g_20GeVSegm_neg->GetMeanError();
 Double_t errmean_30_neg = g_50GeVSegm_neg->GetMeanError();

// Double_t errmean_03_pos = g_03GeVSegm_pos->GetMeanError();
 Double_t errmean_05_pos = g_05GeVSegm_pos->GetMeanError();
 Double_t errmean_10_pos = g_10GeVSegm_pos->GetMeanError();
 Double_t errmean_20_pos = g_20GeVSegm_pos->GetMeanError();
 Double_t errmean_30_pos = g_50GeVSegm_pos->GetMeanError();


// Double_t errsdev_03_neg = g_03GeVSegm_neg->GetStdDevError();
 Double_t errsdev_05_neg = g_05GeVSegm_neg->GetStdDevError();
 Double_t errsdev_10_neg = g_10GeVSegm_neg->GetStdDevError();
 Double_t errsdev_20_neg = g_20GeVSegm_neg->GetStdDevError();
 Double_t errsdev_30_neg = g_50GeVSegm_neg->GetStdDevError();

// Double_t errsdev_03_pos = g_03GeVSegm_pos->GetStdDevError();
 Double_t errsdev_05_pos = g_05GeVSegm_pos->GetStdDevError();
 Double_t errsdev_10_pos = g_10GeVSegm_pos->GetStdDevError();
 Double_t errsdev_20_pos = g_20GeVSegm_pos->GetStdDevError();
 Double_t errsdev_30_pos = g_50GeVSegm_pos->GetStdDevError();

cout << "MEAN: " << 	mean_05_pos << " " << mean_10_pos << " " << mean_20_pos << " " << mean_30_pos << endl;
cout << "STDEV: " << 	sdev_05_pos << " " << sdev_10_pos << " " << sdev_20_pos << " " << sdev_30_pos << endl;

Double_t energy[] = { 5, 10, 20, 50 };
Double_t errzero[] = { 0, 0, 0, 0 };
Double_t mean_pos[] = { mean_05_pos, mean_10_pos, mean_20_pos, mean_30_pos };
Double_t errmean_pos[] = { errmean_05_pos, errmean_10_pos, errmean_20_pos, errmean_30_pos };
Double_t sdev_pos[] = { sdev_05_pos, sdev_10_pos, sdev_20_pos, sdev_30_pos };
Double_t errsdev_pos[] = { errsdev_05_pos, errsdev_10_pos, errsdev_20_pos, errsdev_30_pos };

Double_t mean_neg[] = { mean_05_neg, mean_10_neg, mean_20_neg, mean_30_neg };
Double_t errmean_neg[] = { errmean_05_neg, errmean_10_neg, errmean_20_neg, errmean_30_neg };
Double_t sdev_neg[] = { sdev_05_neg, sdev_10_neg, sdev_20_neg, sdev_30_neg };
Double_t errsdev_neg[] = { errsdev_05_neg, errsdev_10_neg, errsdev_20_neg, errsdev_30_neg };




//========================================= mean ==================================================


TCanvas *c_m = new TCanvas("c_m","SegmentDeltaPhi_posMu", 50,50,W,H );    //canvas for 2 curves with tracking (one per gas)

 //cms aesthetics
  c_m->SetFillColor(0);
  c_m->SetBorderMode(0);
  c_m->SetFrameFillStyle(0);
  c_m->SetFrameBorderMode(0);
  c_m->SetLeftMargin( L/W );
  c_m->SetRightMargin( R/W );
  c_m->SetTopMargin( T/H );
  c_m->SetBottomMargin( B/H );
  c_m->SetTickx(0);
  c_m->SetTicky(0);
 //end cms aesthetics
  
  c_m->SetGridy();
  //c_m->SetGridx(5);
  //c_m->SetLogx();

TPaveText *t_m=new TPaveText(0.145,0.88,0.37,0.94,"brNDC");
  t_m->AddText("CMS Preliminary");
  t_m->SetTextAlign(12);
   //Int_t trans_white = TColor::GetColorTransparent(0, 0);
  t_m->SetShadowColor(trans_white);
  t_m->SetFillColor(trans_white);
  t_m->SetLineColor(trans_white);
  t_m->SetLineColor(trans_white);
  //t_m->AddText("more text");


TGraphErrors * g_mean_pos = new TGraphErrors(4, energy, mean_pos, errzero, errmean_pos);
TGraphErrors * g_mean_neg = new TGraphErrors(4, energy, mean_neg, errzero, errmean_neg);

  g_mean_pos->SetMarkerSize(.95);
  g_mean_neg->SetMarkerSize(.8);
  g_mean_pos->SetMarkerStyle(20);
  g_mean_neg->SetMarkerStyle(21);
  g_mean_pos->SetMarkerColor(kBlue);
  g_mean_neg->SetMarkerColor(kRed);

  g_mean_pos->GetYaxis()->SetRangeUser(-0.2,0.2);
//  g_mean_pos->GetXaxis()->SetRangeUser(-0.3,0.3);
  
  g_mean_pos->GetXaxis()->SetTitleFont(42);
  g_mean_pos->GetXaxis()->SetTitleSize(0.06);
  g_mean_pos->GetXaxis()->SetTitleOffset(0.9);
  g_mean_pos->GetXaxis()->SetLabelOffset(0.007);
  g_mean_pos->GetXaxis()->SetLabelSize(0.05);
  g_mean_pos->GetXaxis()->SetDecimals(kTRUE);
  g_mean_pos->GetXaxis()->SetTickLength(0.03);
  //g_mean_pos->GetYaxis()->SetTitleColor(kRed);
  g_mean_pos->GetYaxis()->SetTitleFont(42);
  g_mean_pos->GetYaxis()->SetTitleSize(0.06);
  g_mean_pos->GetYaxis()->SetTitleOffset(1.2);
  g_mean_pos->GetYaxis()->SetLabelOffset(0.007);
  g_mean_pos->GetYaxis()->SetLabelSize(0.05);
  //g_mean_pos->GetYaxis()->SetLabelColor(kRed);
  g_mean_pos->GetYaxis()->SetDecimals(kTRUE);
  g_mean_pos->GetYaxis()->SetTickLength(0.03);
  g_mean_pos->GetXaxis()->SetTitle("Energy (GeV)");
  g_mean_pos->GetYaxis()->SetTitle("#Delta #Phi Mean");
  //g_mean_pos->GetYaxis()->SetRangeUser(0,1);
  //g_mean_pos->GetYaxis()->SetLimits(0,1);
  //g_mean_pos->GetXaxis()->SetLimits(1025/div,1600/div);
 // g_mean_pos->Write(grTitle);


 g_mean_pos->Draw("AP");
 g_mean_neg->Draw("SAMEP");

 TLegend *leg_m = new TLegend(0.18,0.685,0.49,0.865);
 leg_m->SetTextSize(0.04);
 leg_m->SetTextFont(42);
 //leg_m->AddEntry(g_03GeVSegm_pos, "prompt #mu^{+} 3 GeV", "l");
 leg_m->AddEntry(g_mean_pos, "#Delta #Phi Mean #mu^{+}", "p");
 leg_m->AddEntry(g_mean_neg, "#Delta #Phi Mean #mu^{-}", "p");
 leg_m->Draw();

 t_m->Draw("NB");
 c_m->Update();



//========================================= sdev ==================================================


TCanvas *c_s = new TCanvas("c_s","SegmentDeltaPhi_posMu", 50,50,W,H );    //canvas for 2 curves with tracking (one per gas)

 //cms aesthetics
  c_s->SetFillColor(0);
  c_s->SetBorderMode(0);
  c_s->SetFrameFillStyle(0);
  c_s->SetFrameBorderMode(0);
  c_s->SetLeftMargin( L/W );
  c_s->SetRightMargin( R/W );
  c_s->SetTopMargin( T/H );
  c_s->SetBottomMargin( B/H );
  c_s->SetTickx(0);
  c_s->SetTicky(0);
 //end cms aesthetics
  
  c_s->SetGridy();
  //c_s->SetGridx(5);
  //c_s->SetLogx();

TPaveText *t_s=new TPaveText(0.145,0.88,0.37,0.94,"brNDC");
  t_s->AddText("CMS Preliminary");
  t_s->SetTextAlign(12);
   //Int_t trans_white = TColor::GetColorTransparent(0, 0);
  t_s->SetShadowColor(trans_white);
  t_s->SetFillColor(trans_white);
  t_s->SetLineColor(trans_white);
  t_s->SetLineColor(trans_white);
  //t_s->AddText("more text");


TGraphErrors * g_sdev_pos = new TGraphErrors(4, energy, sdev_pos, errzero, errsdev_pos);
TGraphErrors * g_sdev_neg = new TGraphErrors(4, energy, sdev_neg, errzero, errsdev_neg);

  g_sdev_pos->SetMarkerSize(.95);
  g_sdev_neg->SetMarkerSize(.8);
  g_sdev_pos->SetMarkerStyle(20);
  g_sdev_neg->SetMarkerStyle(21);
  g_sdev_pos->SetMarkerColor(kBlue);
  g_sdev_neg->SetMarkerColor(kRed);

  g_sdev_pos->GetYaxis()->SetRangeUser(0,0.1);
//  g_sdev_pos->GetXaxis()->SetRangeUser(-0.3,0.3);
  
  g_sdev_pos->GetXaxis()->SetTitleFont(42);
  g_sdev_pos->GetXaxis()->SetTitleSize(0.06);
  g_sdev_pos->GetXaxis()->SetTitleOffset(0.9);
  g_sdev_pos->GetXaxis()->SetLabelOffset(0.007);
  g_sdev_pos->GetXaxis()->SetLabelSize(0.05);
  g_sdev_pos->GetXaxis()->SetDecimals(kTRUE);
  g_sdev_pos->GetXaxis()->SetTickLength(0.03);
  //g_sdev_pos->GetYaxis()->SetTitleColor(kRed);
  g_sdev_pos->GetYaxis()->SetTitleFont(42);
  g_sdev_pos->GetYaxis()->SetTitleSize(0.06);
  g_sdev_pos->GetYaxis()->SetTitleOffset(1.2);
  g_sdev_pos->GetYaxis()->SetLabelOffset(0.007);
  g_sdev_pos->GetYaxis()->SetLabelSize(0.05);
  //g_sdev_pos->GetYaxis()->SetLabelColor(kRed);
  g_sdev_pos->GetYaxis()->SetDecimals(kTRUE);
  g_sdev_pos->GetYaxis()->SetTickLength(0.03);
  g_sdev_pos->GetXaxis()->SetTitle("Energy (GeV)");
  g_sdev_pos->GetYaxis()->SetTitle("#Delta #Phi StdDev");
  //g_sdev_pos->GetYaxis()->SetRangeUser(0,1);
  //g_sdev_pos->GetYaxis()->SetLimits(0,1);
  //g_sdev_pos->GetXaxis()->SetLimits(1025/div,1600/div);
 // g_sdev_pos->Write(grTitle);


 g_sdev_pos->Draw("AP");
 g_sdev_neg->Draw("SAMEP");

 TLegend *leg_s = new TLegend(0.18,0.685,0.49,0.865);
 leg_s->SetTextSize(0.04);
 leg_s->SetTextFont(42);
 //leg_s->AddEntry(g_03GeVSegm_pos, "prompt #mu^{+} 3 GeV", "l");
 leg_s->AddEntry(g_sdev_pos, "#Delta #Phi StdDev #mu^{+}", "p");
 leg_s->AddEntry(g_sdev_neg, "#Delta #Phi StdDev #mu^{-}", "p");
 leg_s->Draw();

 t_s->Draw("NB");
 c_s->Update();



//===================================== Save canvases ===============================================================

c1->SaveAs("allSegmentDeltaPhi_promptMu_pos.png");
c2->SaveAs("allSegmentDeltaPhi_promptMu_neg.png");
c03neg->SaveAs("promptMu_3GeV_comparison_neg.png");
c05neg->SaveAs("promptMu_5GeV_comparison_neg.png");
c10neg->SaveAs("promptMu_10GeV_comparison_neg.png");
c20neg->SaveAs("promptMu_20GeV_comparison_neg.png");
c50neg->SaveAs("promptMu_50GeV_comparison_neg.png");
c03pos->SaveAs("promptMu_3GeV_comparison_pos.png");
c05pos->SaveAs("promptMu_5GeV_comparison_pos.png");
c10pos->SaveAs("promptMu_10GeV_comparison_pos.png");
c20pos->SaveAs("promptMu_20GeV_comparison_pos.png");
c50pos->SaveAs("promptMu_50GeV_comparison_pos.png");
c_m->SaveAs("promptMu_meanDeltaPhi.png");
c_s->SaveAs("promptMu_stdDevDeltaPhi.png");
*/

//================================================= O L D ==================================================================
/* 
 mergeGraphs(gAND1,gAND2);
 mergeGraphs(gX1,gX2);
 mergeGraphs(gY1,gY2);
 
 //fits
 Int_t fitStyle=9;
 Int_t fitWidth=1.8;
Double_t minX, maxX, minY, maxY;
gAND1->ComputeRange(minX, minY, maxX, maxY);
TF1 *fitgAND1 = new TF1 ("fitgAND1", efficiency, minX, maxX, 3);
fitgAND1->SetLineColor(kRed);
fitgAND1->SetLineStyle(fitStyle);
fitgAND1->SetLineWidth(fitWidth);
fitgAND1->SetParameters(0.97, minX+(maxX - minX)/2, 10*div);
fitgAND1->SetParLimits(0,0,1);
//fitgAND1->FixParameter(0,0.97);
gAND1->Fit("fitgAND1", "0R");
*/
/*Double_t effAnd = fitgAND1->GetParameter(0); cout << effAnd << endl;
Double_t errAnd = fitgAND1->GetParError(0); cout << errAnd << endl;
TString effAndStr, errAndStr;
effAndStr.Form("%d", effAnd); cout << effAndStr << endl;
errAndStr.Form("%d", errAnd); cout << errAndStr << endl;*/

/*
gX1->ComputeRange(minX, minY, maxX, maxY);
TF1 *fitgX1 = new TF1 ("fitgX1", efficiency, minX, maxX, 3);
fitgX1->SetLineColor(kBlue);
fitgX1->SetLineStyle(fitStyle);
fitgX1->SetLineWidth(fitWidth);
fitgX1->SetParameters(0.97, minX+(maxX - minX)/2, 10*div);
fitgX1->SetParLimits(0,0,1);
//fitgX1->FixParameter(0,0.97);
gX1->Fit("fitgX1", "0R");

gY1->ComputeRange(minX, minY, maxX, maxY);
TF1 *fitgY1 = new TF1 ("fitgY1", efficiency, minX, maxX, 3);
fitgY1->SetLineColor(kGreen);
fitgY1->SetLineStyle(fitStyle);
fitgY1->SetLineWidth(fitWidth);
fitgY1->SetParameters(0.97, minX+(maxX - minX)/2, 10*div);
fitgY1->SetParLimits(0,0,1);
//fitgY1->FixParameter(0,0.97);
gY1->Fit("fitgY1", "0R");


  gStyle->SetOptTitle(0);

  Int_t MkSize=1.7;
  gX1->SetMarkerColor(kBlue);
  gX1->SetMarkerStyle(21);
  gX1->SetMarkerSize(MkSize);
  Int_t MkSize=1.7;
  gX1->SetMarkerColor(kBlue);
  gX1->SetMarkerStyle(21);
  gX1->SetMarkerSize(MkSize);
  //gX1->GetXaxis()->SetTitleColor(1);
  gX1->GetXaxis()->SetTitleFont(42);
  gX1->GetXaxis()->SetTitleSize(0.06);
  gX1->GetXaxis()->SetTitleOffset(0.9);
  gX1->GetXaxis()->SetLabelOffset(0.007);
  gX1->GetXaxis()->SetLabelSize(0.05);
  gX1->GetXaxis()->SetDecimals(kTRUE);
  gX1->GetXaxis()->SetTickLength(0.03);
  //gX1->GetYaxis()->SetTitleColor(kRed);
  gX1->GetYaxis()->SetTitleFont(42);
  gX1->GetYaxis()->SetTitleSize(0.06);
  gX1->GetYaxis()->SetTitleOffset(1.2);
  gX1->GetYaxis()->SetLabelOffset(0.007);
  gX1->GetYaxis()->SetLabelSize(0.05);
  //gX1->GetYaxis()->SetLabelColor(kRed);
  gX1->GetYaxis()->SetDecimals(kTRUE);
  gX1->GetYaxis()->SetTickLength(0.03);
  gX1->GetXaxis()->SetTitle("Divider Current (#muA)");
  gX1->GetYaxis()->SetTitle("Efficiency");
  gX1->GetYaxis()->SetRangeUser(0,1);
  gX1->GetYaxis()->SetLimits(0,1);
  gX1->GetXaxis()->SetLimits(1025/div,1600/div);
 // gX1->Write(grTitle);

  gX2->SetMarkerColor(kBlue);
  gX2->SetMarkerStyle(21);
  gX2->SetMarkerSize(MkSize);
  
  gY1->SetMarkerColor(kGreen);
  gY1->SetMarkerStyle(21);
  gY1->SetMarkerSize(MkSize);
  
  gY2->SetMarkerColor(kGreen);
  gY2->SetMarkerStyle(21);
  gY2->SetMarkerSize(MkSize);
  
  gAND1->SetMarkerColor(kRed);
  gAND1->SetMarkerStyle(22);
  gAND1->SetMarkerSize(MkSize+0.2);
  
  gAND2->SetMarkerColor(kRed);
  gAND2->SetMarkerStyle(22);
  gAND2->SetMarkerSize(MkSize+0.2);  

  gX1->RemovePoint(0);
  gY1->RemovePoint(0);
  gAND1->RemovePoint(0);
  
// g1->SetTitle("Efficiency - Back to back detector");
 c1->cd();
 gX1->Draw("AP");
 //gX2->Draw("SAMEP");
 gY1->Draw("SAMEP");
 //gY2->Draw("SAMEP");
 gAND1->Draw("SAMEP");
 //gAND2->Draw("SAMEP");
 fitgAND1->Draw("SAME");
 fitgX1->Draw("SAME");
 fitgY1->Draw("SAME");
 
 TLegend *leg = new TLegend(0.6,0.4,0.88,0.63);
 leg->SetTextSize(0.04);
 leg->SetTextFont(42);
 leg->AddEntry(gX1, "X Axis GEM", "p");
 leg->AddEntry(gY1, "Y Axis GEM", "p");
 leg->AddEntry(gAND1, "AND of GEMs", "p");
 //leg->AddEntry(g2, "Ar:CO_{2} 70:30", "p");
 leg->Draw();
 c1->Update();
 
 TLegend *legeff = new TLegend(0.5,0.17,0.88,0.4);
 legeff->SetTextSize(0.034);
 legeff->SetTextFont(42);
// 3 sigma tolerance 
 legeff->AddEntry(fitgX1, "Efficiency = (96.5 #pm 0.1)%", "l");
 legeff->AddEntry(fitgY1, "Efficiency = (96.6 #pm 0.1)%", "l");
 legeff->AddEntry(fitgAND1, "Efficiency = (93.8 #pm 0.1)%", "l");
 // 4 sigma tolerance
//  legeff->AddEntry(fitgX1, "Efficiency = (97.74 #pm 0.09)%", "l");
//  legeff->AddEntry(fitgY1, "Efficiency = (97.63 #pm 0.07)%", "l");
//  legeff->AddEntry(fitgAND1, "Efficiency = (96.0 #pm 0.1)%", "l");
 // 5 sigma tolerance
//  legeff->AddEntry(fitgX1, "Efficiency = (98.35 #pm 0.08)%", "l");
//  legeff->AddEntry(fitgY1, "Efficiency = (98.13 #pm 0.07)%", "l");
//  legeff->AddEntry(fitgAND1, "Efficiency = (96.99 #pm 0.09)%", "l");
 // 3 sigma tolerance - old alignment method
//  legeff->AddEntry(fitgX1, "Efficiency = (96.4 #pm 0.1)%", "l");
//  legeff->AddEntry(fitgY1, "Efficiency = (96.0 #pm 0.1)%", "l");
//  legeff->AddEntry(fitgAND1, "Efficiency = (93.4 #pm 0.1)%", "l");
 // Teruki 5 sigma tolerance
//  legeff->AddEntry(fitgX1, "Efficiency = (98.32 #pm 0.08)%", "l");
//  legeff->AddEntry(fitgY1, "Efficiency = (98.09 #pm 0.07)%", "l");
//  legeff->AddEntry(fitgAND1, "Efficiency = (96.93 #pm 0.09)%", "l");
 legeff->Draw();
 c1->Update();
 

//TString TextBox="CMS Preliminary";
TLatex *tex = new TLatex();
tex->SetTextSize(0.030);
tex->DrawLatex(1045/div-5, 0.23 , "Ar:CO_{2} 70:30");
tex->DrawLatex(1045/div-5, 0.18 , "Track reconstruction");//"Ar:CO_{2}:CF_{4} 45:15:40");
tex->DrawLatex(1045/div-5, 0.13 , "Muon Beam");
tex->DrawLatex(1045/div-5, 0.08 , "Thr = 15 A.U., Latency = 15 A.U.");
tex->DrawLatex(1045/div-5, 0.03 , "Ishaper = 120 A.U., Icomp = 75 A.U.");
//tex->DrawLatex(100, 270, "more text");
tex->Draw();

TPaveText *t1=new TPaveText(0.145,0.88,0.37,0.94,"brNDC");
  t1->AddText("CMS Preliminary");
  t1->SetTextAlign(12);
  //TColor *transp = gROOT->GetColor(0);
  //transp->SetAlpha(0);
  //Int_t ci = 1756;
  //TColor *color = new TColor(ci, 1, 1, 1, "transp", 0);
  Int_t trans_white = TColor::GetColorTransparent(0, 0);
  t1->SetShadowColor(trans_white);
  t1->SetFillColor(trans_white);
  t1->SetLineColor(trans_white);
  t1->SetLineColor(trans_white);
  //t1->AddText("more text");
  t1->Draw("NB");
 c1->Update();

TString title="PlotsForApproval/EfficiencyTracking-ArCO2";
TString titlepng=title+vers+".png";
TString titleroot=title+vers+".root";
c1->SaveAs(titlepng);
c1->SaveAs(titleroot);

//================================ Ar CO2 CF4 - tracking ==================================  
  

TCanvas *c2 = new TCanvas("c2","Ar CO2 CF4", 50,50,W,H );    //canvas for 2 curves with tracking (one per gas)

 //cms aesthetics
  c2->SetFillColor(0);
  c2->SetBorderMode(0);
  c2->SetFrameFillStyle(0);
  c2->SetFrameBorderMode(0);
  c2->SetLeftMargin( L/W );
  c2->SetRightMargin( R/W );
  c2->SetTopMargin( T/H );
  c2->SetBottomMargin( B/H );
  c2->SetTickx(0);
  c2->SetTicky(0);
 //end cms aesthetics
  
  c2->SetGridy();
  //c2->SetGridx(5);
  c2->cd();


 
 TFile *f4 = TFile::Open("EfficiencyPlots_cut_trk-Final3sigma/Efficiency_R595-R625_R633.root", "READ");    //AND
 TFile *f5 = TFile::Open("EfficiencyPlotsX_cut_trk-Final3sigma/Efficiency_R595-R625_R633.root", "READ");   //Asse x
 TFile *f6 = TFile::Open("EfficiencyPlotsY_cut_trk-Final3sigma/Efficiency_R595-R625_R633.root", "READ");   //Asse y
//f4->ls();  cout << "\n" << endl;
//f5->ls();  cout << "\n" << endl;
//f6->ls();  cout << "\n" << endl;

//=================================== to get TGraphErrors =======================================
// get the list of keys
   TList *list = (TList *)f4->GetListOfKeys();
   if (!list) {
      std::cout << "Cannot get the list of TKeys! Aborting..." << std::endl;
      return;
   }
   
   TObject *obj;
   TListIter *it = new TListIter(list,kIterForward);
   TString GrName;
   while ((obj = it->Next()))
     {
     TString title = obj->GetTitle();
     if (  title.BeginsWith("Ar:CO_{2}:CF_{4}") && title.EndsWith("Thr20-Lat15"))
     GrName = obj->GetTitle();
     }
   
  // cout << GrName << endl;

//================================== end =============================================================






 TGraphErrors * gAND = (TGraphErrors*)f4->Get(GrName);    //AND
 TGraphErrors * gX = (TGraphErrors*)f5->Get(GrName);    //X axis
 TGraphErrors * gY = (TGraphErrors*)f6->Get(GrName);    //Y axis
// TGraphErrors * gtdc1 = (TGraphErrors*)f1->Get("Efficiency Ar:CO_{2}:CF_{4} 45:15:40 - TDC");    //Ar CO2 CF4 with TDC
// TGraphErrors * gtdc2 = (TGraphErrors*)f2->Get("Efficiency Ar:CO_{2} 70:30 - TDC");    //Ar CO2 with TDC

gAND->ComputeRange(minX, minY, maxX, maxY);
TF1 *fitgAND = new TF1 ("fitgAND", efficiency, minX, maxX, 3);
fitgAND->SetLineColor(kRed);
fitgAND->SetLineStyle(fitStyle);
fitgAND->SetLineWidth(fitWidth);
fitgAND->SetParameters(0.94, minX+(maxX - minX)/2, 10*div);
fitgAND->SetParLimits(0,0,1);
//fitgAND->FixParameter(0,0.97);
gAND->Fit("fitgAND", "0R");

gX->ComputeRange(minX, minY, maxX, maxY);
TF1 *fitgX = new TF1 ("fitgX", efficiency, minX, maxX, 3);
fitgX->SetLineColor(kBlue);
fitgX->SetLineStyle(fitStyle);
fitgX->SetLineWidth(fitWidth);
fitgX->SetParameters(0.97, minX+(maxX - minX)/2, 10*div);
fitgX->SetParLimits(0,0,1);
//fitgX->FixParameter(0,0.97);
gX->Fit("fitgX", "0R");

gY->ComputeRange(minX, minY, maxX, maxY);
TF1 *fitgY = new TF1 ("fitgY", efficiency, minX, maxX, 3);
fitgY->SetLineColor(kGreen);
fitgY->SetLineStyle(fitStyle);
fitgY->SetLineWidth(fitWidth);
fitgY->SetParameters(0.97, minX+(maxX - minX)/2, 10*div);
fitgY->SetParLimits(0,0,1);
//fitgY->FixParameter(0,0.97);
gY->Fit("fitgY", "0R");


  gStyle->SetOptTitle(0);

  Int_t MkSize=1.7;
  gX->SetMarkerColor(kBlue);
  gX->SetMarkerStyle(21);
  gX->SetMarkerSize(MkSize);
  //gX->GetXaxis()->SetTitleColor(1);
  gX->GetXaxis()->SetTitleFont(42);
  gX->GetXaxis()->SetTitleSize(0.06);
  gX->GetXaxis()->SetTitleOffset(0.9);
  gX->GetXaxis()->SetLabelOffset(0.007);
  gX->GetXaxis()->SetLabelSize(0.05);
  gX->GetXaxis()->SetDecimals(kTRUE);
  gX->GetXaxis()->SetTickLength(0.03);
  //gX->GetYaxis()->SetTitleColor(kRed);
  gX->GetYaxis()->SetTitleFont(42);
  gX->GetYaxis()->SetTitleSize(0.06);
  gX->GetYaxis()->SetTitleOffset(1.2);
  gX->GetYaxis()->SetLabelOffset(0.007);
  gX->GetYaxis()->SetLabelSize(0.05);
  //gX->GetYaxis()->SetLabelColor(kRed);
  gX->GetYaxis()->SetDecimals(kTRUE);
  gX->GetYaxis()->SetTickLength(0.03);
  gX->GetXaxis()->SetTitle("Divider Current (#muA)");
  gX->GetYaxis()->SetTitle("Efficiency");
  gX->GetYaxis()->SetRangeUser(0,1);
  gX->GetYaxis()->SetLimits(0,1);
  gX->GetXaxis()->SetLimits(1250/div,1800/div); //1250, 1700
 // gX->Write(grTitle);

  
  gY->SetMarkerColor(kGreen);
  gY->SetMarkerStyle(21);
  gY->SetMarkerSize(MkSize);

  
  gAND->SetMarkerColor(kRed);
  gAND->SetMarkerStyle(22);
  gAND->SetMarkerSize(MkSize+0.2);

  if (GrName.EndsWith("-Lat14"))
    {
    gX->RemovePoint(0);
    gY->RemovePoint(0);
    gAND->RemovePoint(0);
    }
   if (GrName.EndsWith("-Lat15"))
    {
    gX->RemovePoint(2);
    gY->RemovePoint(2);
    gAND->RemovePoint(2);
    }
     
// g1->SetTitle("Efficiency - Back to back detector");
 c2->cd();
 gX->Draw("AP");
 gY->Draw("SAMEP");
 gAND->Draw("SAMEP");
 fitgAND->Draw("SAME");
 fitgX->Draw("SAME");
 fitgY->Draw("SAME");
 
 TLegend *leg2 = new TLegend(0.6,0.65,0.88,0.88);
 leg2->SetTextSize(0.04);
 leg2->SetTextFont(42);
 leg2->AddEntry(gX, "X Axis GEM", "p");
 leg2->AddEntry(gY, "Y Axis GEM", "p");
 leg2->AddEntry(gAND, "AND of GEMs", "p");
 //leg2->AddEntry(g2, "Ar:CO_{2} 70:30", "p");
 leg2->Draw();
 c2->Update();
 
  TLegend *legeff2 = new TLegend(0.52,0.17,0.88,0.4);
 legeff2->SetTextSize(0.034);
 legeff2->SetTextFont(42);
 // 3 sigma tolerance
 legeff2->AddEntry(fitgX, "Efficiency = (96.6 #pm 0.2)%", "l");
 legeff2->AddEntry(fitgY, "Efficiency = (98.1 #pm 0.2)%", "l");
 legeff2->AddEntry(fitgAND, "Efficiency = (94.1 #pm 0.2)%", "l");
 // 4 sigma tolerance
//  legeff2->AddEntry(fitgX, "Efficiency = (97.7 #pm 0.1)%", "l");
//  legeff2->AddEntry(fitgY, "Efficiency = (99.0 #pm 0.2)%", "l");
//  legeff2->AddEntry(fitgAND, "Efficiency = (96.1 #pm 0.2)%", "l");
 // 5 sigma tolerance
//  legeff2->AddEntry(fitgX, "Efficiency = (98.3 #pm 0.1)%", "l");
//  legeff2->AddEntry(fitgY, "Efficiency = (99.4 #pm 0.2)%", "l");
//  legeff2->AddEntry(fitgAND, "Efficiency = (97.0 #pm 0.2)%", "l");
 // 3 sigma tolerance - old alignment method
//  legeff2->AddEntry(fitgX, "Efficiency = (95.7 #pm 0.2)%", "l");
//  legeff2->AddEntry(fitgY, "Efficiency = (97.3 #pm 0.2)%", "l");
//  legeff2->AddEntry(fitgAND, "Efficiency = (93.0 #pm 0.2)%", "l");
 // Teruki 5 sigma tolerance
//  legeff2->AddEntry(fitgX, "Efficiency = (98.2 #pm 0.1)%", "l");
//  legeff2->AddEntry(fitgY, "Efficiency = (99.3 #pm 0.2)%", "l");
//  legeff2->AddEntry(fitgAND, "Efficiency = (96.9 #pm 0.2)%", "l");
 legeff2->Draw();
 c2->Update();

//TString TextBox="CMS Preliminary";
TLatex *tex2 = new TLatex();
tex2->SetTextSize(0.030);
tex2->DrawLatex(1630/div, 0.63 , "Ar:CO_{2}:CF_{4} 45:15:40"); //1550/div
tex2->DrawLatex(1630/div, 0.58 , "Track reconstruction");
tex2->DrawLatex(1630/div, 0.53 , "Muon Beam");
tex2->DrawLatex(1630/div, 0.48 , "Latency = 15 A.U.");
tex2->DrawLatex(1630/div, 0.43 , "Threshold = 20 A.U.");
tex2->DrawLatex(1630/div, 0.38 , "Ishaper = 120 A.U.");
tex2->DrawLatex(1630/div, 0.33 , "Icomp = 75 A.U.");
//tex->DrawLatex(100, 270, "more text");
tex2->Draw();
*/
/*
TPaveText *t1=new TPaveText(0.145,0.88,0.37,0.94,"brNDC");
  t1->AddText("CMS Preliminary");
  t1->SetTextAlign(12);
  //TColor *transp = gROOT->GetColor(0);
  //transp->SetAlpha(0);
  //Int_t ci = 1756;
  //TColor *color = new TColor(ci, 1, 1, 1, "transp", 0);
  Int_t trans_white = TColor::GetColorTransparent(0, 0);
  t1->SetShadowColor(trans_white);
  t1->SetFillColor(trans_white);
  t1->SetLineColor(trans_white);
  t1->SetLineColor(trans_white);
  //t1->AddText("more text");
*/

/*
 t1->Draw("NB");
 c2->Update();


TString title="PlotsForApproval/EfficiencyTracking-ArCO2CF4";
TString titlepng=title+vers+".png";
TString titleroot=title+vers+".root";
c2->SaveAs(titlepng);
c2->SaveAs(titleroot);

//======================== TDC ===============================================
 TGraphErrors * gtdc1 = (TGraphErrors*)f1->Get("Efficiency Ar:CO_{2}:CF_{4} 45:15:40 - TDC");	 //Ar CO2 CF4 with TDC
 TGraphErrors * gtdc2 = (TGraphErrors*)f4->Get("Efficiency Ar:CO_{2} 70:30 - TDC");    //Ar CO2 with TDC

gtdc1->SetLineColor(kRed);

TCanvas *c3 = new TCanvas("c3","tdc", 50,50,W,H );    //canvas for 2 curves with tracking (one per gas) + 2 curves with TDC (one per gas)

 //cms aesthetics
  c3->SetFillColor(0);
  c3->SetBorderMode(0);
  c3->SetFrameFillStyle(0);
  c3->SetFrameBorderMode(0);
  c3->SetLeftMargin( L/W );
  c3->SetRightMargin( R/W );
  c3->SetTopMargin( T/H );
  c3->SetBottomMargin( B/H );
  c3->SetTickx(0);
  c3->SetTicky(0);
 //end cms aesthetics
  
  c3->SetGridy();
  c3->cd();

gtdc1->ComputeRange(minX, minY, maxX, maxY);
TF1 *fitgtdc1 = new TF1 ("fitgtdc1", efficiency, minX, maxX, 3);
fitgtdc1->SetLineColor(kRed);
fitgtdc1->SetLineStyle(fitStyle);
fitgtdc1->SetLineWidth(fitWidth);
fitgtdc1->SetParameters(0.98, 1400/div, 20*div);
fitgtdc1->SetParLimits(0,0,1);
//fitgtdc1->FixParameter(0,0.97);
gtdc1->Fit("fitgtdc1", "0R");

gtdc2->ComputeRange(minX, minY, maxX, maxY);
TF1 *fitgtdc2 = new TF1 ("fitgtdc2", efficiency, minX, maxX, 3);
fitgtdc2->SetLineColor(kBlue);
fitgtdc2->SetLineStyle(fitStyle);
fitgtdc2->SetLineWidth(fitWidth);
fitgtdc2->SetParameters(0.98, 1204/div, 18.62*div);
fitgtdc2->SetParLimits(0,0.9,1);
//fitgtdc2->FixParameter(0,0.98);
fitgtdc2->FixParameter(1,1204/div);
fitgtdc2->FixParameter(2,18.62*div);
gtdc2->Fit("fitgtdc2", "0R");
//fitgtdc2->ReleaseParameter(0);
fitgtdc2->ReleaseParameter(1);
fitgtdc2->ReleaseParameter(2);
gtdc2->Fit("fitgtdc2", "0R");

  gStyle->SetOptTitle(0);

  Int_t MkSize=1.7;
  gtdc1->SetMarkerColor(kRed);
  gtdc1->SetMarkerStyle(8);
  gtdc1->SetMarkerSize(MkSize+0.2);
  //gtdc1->GetXaxis()->SetTitleColor(1);
  gtdc1->GetXaxis()->SetTitleFont(42);
  gtdc1->GetXaxis()->SetTitleSize(0.06);
  gtdc1->GetXaxis()->SetTitleOffset(0.9);
  gtdc1->GetXaxis()->SetLabelOffset(0.007);
  gtdc1->GetXaxis()->SetLabelSize(0.05);
  gtdc1->GetXaxis()->SetDecimals(kTRUE);
  gtdc1->GetXaxis()->SetTickLength(0.03);
  //gtdc1->GetYaxis()->SetTitleColor(kRed);
  gtdc1->GetYaxis()->SetTitleFont(42);
  gtdc1->GetYaxis()->SetTitleSize(0.06);
  gtdc1->GetYaxis()->SetTitleOffset(1.2);
  gtdc1->GetYaxis()->SetLabelOffset(0.007);
  gtdc1->GetYaxis()->SetLabelSize(0.05);
  //gtdc1->GetYaxis()->SetLabelColor(kRed);
  gtdc1->GetYaxis()->SetDecimals(kTRUE);
  gtdc1->GetYaxis()->SetTickLength(0.03);
  gtdc1->GetXaxis()->SetTitle("Divider Current (#muA)");
  gtdc1->GetYaxis()->SetTitle("Efficiency");
  gtdc1->GetYaxis()->SetRangeUser(0,1);
  gtdc1->GetYaxis()->SetLimits(0,1);
  gtdc1->GetXaxis()->SetLimits(1000/div, 2050/div); //100,1750
 // gtdc1->Write(grTitle);
  //gtdc1->SetLineColor(kBlack);

  
  gtdc2->SetMarkerColor(kBlue);
  gtdc2->SetMarkerStyle(8);
  gtdc2->SetMarkerSize(MkSize+0.2);
 // gtdc2->SetLineColor(kBlack);


  c3->cd();
  gtdc1->Draw("AP");
  gtdc2->Draw("SAMEP");
  fitgtdc1->Draw("SAME");
  fitgtdc2->Draw("SAME");
 

 TLegend *leg3 = new TLegend(0.57,0.65,0.88,0.88);
 leg3->SetTextSize(0.035);
 leg3->SetTextFont(42);
 leg3->AddEntry(gtdc1, "Ar:CO_{2}:CF_{4} 45:15:40", "p");
 leg3->AddEntry(gtdc2, "Ar:CO_{2} 70:30", "p");
 leg3->Draw();
 
 TLegend *legeff3 = new TLegend(0.5,0.17,0.88,0.4);
 legeff3->SetTextSize(0.034);
 legeff3->SetTextFont(42);
 legeff3->AddEntry(fitgtdc1, "Efficiency = (97.51 #pm 0.06)%", "l");
 legeff3->AddEntry(fitgtdc2, "Efficiency = (98.02 #pm 0.04)%", "l");
 //leg->AddEntry(g2, "Ar:CO_{2} 70:30", "p");
 legeff3->Draw();
 c3->Update();

  t1->Draw("NB");
  //TString TextBox="CMS Preliminary";
TLatex *tex3 = new TLatex();
tex3->SetTextSize(0.030);
tex3->DrawLatex(1660/div, 0.55 , "From TDC measurement"); //1500/div
tex3->DrawLatex(1660/div, 0.50 , "Muon Beam");
tex3->DrawLatex(1660/div, 0.45 , "Threshold = 20 A.U.");
tex3->DrawLatex(1660/div, 0.40 , "Ishaper = 120 A.U.");
tex3->DrawLatex(1660/div, 0.35 , "Icomp = 75 A.U.");
//tex->DrawLatex(100, 270, "more text");
tex3->Draw();

TString title="PlotsForApproval/EfficiencyTDC";
TString titlepng=title+vers+".png";
TString titleroot=title+vers+".root";
c3->SaveAs(titlepng);
c3->SaveAs(titleroot);

//=================================== TDC and tracking ====================================================
//======================== TDC ===============================================

TCanvas *c4 = new TCanvas("c4","tdc and tracking", 50,50,W,H );    //canvas for 2 curves with tracking (one per gas) + 2 curves with TDC (one per gas)

 //cms aesthetics
  c4->SetFillColor(0);
  c4->SetBorderMode(0);
  c4->SetFrameFillStyle(0);
  c4->SetFrameBorderMode(0);
  c4->SetLeftMargin( L/W );
  c4->SetRightMargin( R/W );
  c4->SetTopMargin( T/H );
  c4->SetBottomMargin( B/H );
  c4->SetTickx(0);
  c4->SetTicky(0);
 //end cms aesthetics
  
  c4->SetGridy();
  c4->cd();
  
  gtdc1->GetXaxis()->SetLimits(1000/div,1900/div);
  gtdc1->SetMarkerColor(kBlue);
  fitgtdc1->SetLineColor(kBlue);
  fitgtdc1->SetLineStyle(2);
  gtdc1->SetMarkerStyle(22);
  gtdc2->SetMarkerStyle(23);
  gAND->SetMarkerStyle(21);
  gAND1->SetMarkerStyle(8);
  fitgAND->SetLineStyle(2);
  gAND2->SetMarkerStyle(8);
  gAND->SetMarkerSize(MkSize);
 // gtdc1->SetLineWidth(4);
 // gAND->SetLineWidth(4);
 gtdc1->GetXaxis()->SetNdivisions(5);
 c4->Update();
  
  gtdc1->SetMarkerColor(kRed);
  gAND1->SetMarkerColor(kBlue);
  fitgtdc1->SetLineColor(kRed);
  fitgAND1->SetLineColor(kBlue);
  
  gtdc1->SetMarkerStyle(20);
  gAND2->SetMarkerStyle(21);
  gtdc2->SetMarkerStyle(33);
  gAND1->SetMarkerStyle(34);
  fitgAND1->SetLineStyle(2);
  fitgAND->SetLineStyle(2);
    
  c4->cd();
  gtdc1->Draw("AP");
  gtdc2->Draw("SAMEP");
  gAND->Draw("SAMEP");
  gAND1->Draw("SAMEP");
  //gAND2->Draw("SAMEP");
  fitgAND->Draw("SAME");
  fitgAND1->Draw("SAME");
  fitgtdc1->Draw("SAME");
  fitgtdc2->Draw("SAME");
  
 TLegend *leg4 = new TLegend(0.52,0.4,0.88,0.63);
 leg4->SetTextSize(0.035);
 leg4->SetTextFont(42);
 leg4->AddEntry(gAND, "Ar:CO_{2}:CF_{4} with tracking", "p");
 leg4->AddEntry(gtdc1, "Ar:CO_{2}:CF_{4} with TDC", "p");
 leg4->AddEntry(gAND1, "Ar:CO_{2}with tracking", "p");
 leg4->AddEntry(gtdc2, "Ar:CO_{2}with TDC", "p");
 leg4->Draw();
 
  TLegend *legeff4 = new TLegend(0.5,0.17,0.88,0.4);
 legeff4->SetTextSize(0.034);
 legeff4->SetTextFont(42);
 // 3 sigma tolerance
 legeff4->AddEntry(fitgAND, "Efficiency = (94.1 #pm 0.2)%", "l");
 legeff4->AddEntry(fitgtdc1, "Efficiency = (97.51 #pm 0.06)%", "l");
 legeff4->AddEntry(fitgAND1, "Efficiency = (93.8 #pm 0.1)%", "l");
 legeff4->AddEntry(fitgtdc2, "Efficiency = (98.02 #pm 0.04)%", "l");
 // 5 sigma tolerance
//  legeff4->AddEntry(fitgAND, "Efficiency = (97.0 #pm 0.2)%", "l");
//  legeff4->AddEntry(fitgtdc1, "Efficiency = (97.51 #pm 0.06)%", "l");
//  legeff4->AddEntry(fitgAND1, "Efficiency = (96.99 #pm 0.09)%", "l");
//  legeff4->AddEntry(fitgtdc2, "Efficiency = (98.02 #pm 0.04)%", "l");
  // 3 sigma tolerance - old alignment method
//  legeff4->AddEntry(fitgAND, "Efficiency = (96.1 #pm 0.2)%", "l");
//  legeff4->AddEntry(fitgtdc1, "Efficiency = (97.51 #pm 0.06)%", "l");
//  legeff4->AddEntry(fitgAND1, "Efficiency = (96.0 #pm 0.1)%", "l");
//  legeff4->AddEntry(fitgtdc2, "Efficiency = (98.02 #pm 0.04)%", "l");
   // Teruki 5 sigma tolerance
//  legeff4->AddEntry(fitgAND, "Efficiency = (96.9 #pm 0.2)%", "l");
//  legeff4->AddEntry(fitgtdc1, "Efficiency = (97.51 #pm 0.06)%", "l");
//  legeff4->AddEntry(fitgAND1, "Efficiency = (96.93 #pm 0.09)%", "l");
//  legeff4->AddEntry(fitgtdc2, "Efficiency = (98.02 #pm 0.04)%", "l");
 legeff4->Draw();
 c4->Update();

  t1->Draw("NB");
  //TString TextBox="CMS Preliminary";
TLatex *tex4 = new TLatex();
tex4->SetTextSize(0.030);
tex4->DrawLatex(1650/div, 0.80 , "Muon Beam");
tex4->DrawLatex(1650/div, 0.75 , "Ishaper = 120 A.U.");
tex4->DrawLatex(1650/div, 0.70 , "Icomp = 75 A.U.");
//tex->DrawLatex(100, 270, "more text");
tex4->Draw();

TString title="PlotsForApproval/EfficiencyTrackingAndTDC";
TString titlepng=title+vers+".png";
TString titleroot=title+vers+".root";
c4->SaveAs(titlepng);
c4->SaveAs(titleroot);
*/
return 0;
}
