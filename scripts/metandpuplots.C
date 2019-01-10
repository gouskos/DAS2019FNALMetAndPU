#include "setTDRStyle.h"
#include <sstream>
#include <fstream>
#include "TTree.h"
#include "TFile.h"
#include "TH1F.h"
#include <vector>
#include "Rtypes.h"
#include "TColor.h"
#include "TVectorF.h"
#include <cstdlib>


TH1F *create1Dhisto(TTree *tree,TString intLumi,TString cuts,TString branch,int bins,float xmin,float xmax,
		    bool useLog,int color, int style,TString name,bool norm,bool data);
//TCanvas *makeCanvas(TString name, TTree* tree, std::vector<TString> branches,std::vector<TString> legends, std::vector<int> colors,
//		    int xbins, float xmin, float xmax, TString xname, TString yname, bool uselog);
TCanvas *makeCanvas(TString name, TFile* file, TString dir, std::vector<TString> branches, std::vector<TString> legends, std::vector<int> colors,
		    int rebin, TString xname, TString yname, bool uselog);
void setTDRStyle();

void metandpuplots(TString part) {

  setTDRStyle();
  gROOT->SetBatch(false);
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(1);
  gStyle->SetPalette(1);

  TFile *file;

  if (part=="step2a") {
    file = TFile::Open("../outputs/metandpu_step2.root","READONLY" );
    TString dir = "metandpuanalstep2a";
    std::vector<TString> br;
    br.push_back("h_metrawpt"); br.push_back("h_metpt"); br.push_back("h_metsmearpt");
    std::vector<TString> leg;
    leg.push_back("PFMET Raw"); leg.push_back("PFMET T1"); leg.push_back("PFMET T1+smear");
    std::vector<int> col;
    col.push_back(1); col.push_back(2); col.push_back(4);
    TCanvas *c_step2_a = makeCanvas(part+"_metcor",file,dir,br,leg,col,1,"p_{T}^{miss} [GeV]","Events / bin",true);
  }

  if (part=="step2b") {
    file = TFile::Open("../outputs/metandpu_step2.root","READONLY" );
    TString dir = "metandpuanalstep2a";
    std::vector<TString> br;
    br.push_back("h_metpt");
    br.push_back("h_metuncuppt"); br.push_back("h_metuncdnpt");
    br.push_back("h_metjesuppt"); br.push_back("h_metjesdnpt");
    br.push_back("h_metjeruppt"); br.push_back("h_metjerdnpt");
    std::vector<TString> leg;
    leg.push_back("PFMET T1");
    leg.push_back("Unc-Up"); leg.push_back("Unc-Down");
    leg.push_back("JES-Up"); leg.push_back("JES-Down");
    leg.push_back("JER-Up"); leg.push_back("JER-Down");
    std::vector<int> col;
    col.push_back(1);
    col.push_back(4); col.push_back(4);
    col.push_back(2); col.push_back(2);
    col.push_back(8); col.push_back(8);
    TCanvas *c_step2_a = makeCanvas(part+"_metunc",file,dir,br,leg,col,1,"p_{T}^{miss} [GeV]","Events / bin",true);
  }

}

TCanvas *makeCanvas(TString name, TFile* file, TString dir, std::vector<TString> branches, std::vector<TString> legends, std::vector<int> colors,
		    int rebin, TString xname, TString yname, bool uselog) {

  TCanvas *c = new TCanvas("c_"+name,"c_"+name,600,700);
  TPad *pMain  = new TPad("pMain_"+name,"pMain_"+name,0.0,0.35,1.0,1.0);
  pMain->SetRightMargin(0.05);
  pMain->SetLeftMargin(0.17);
  pMain->SetBottomMargin(0.);
  pMain->SetTopMargin(0.1);
  
  TPad *pRatio = new TPad("pRatio_"+name,"pRatio_"+name,0.0,0.0,1.0,0.35);
  pRatio->SetRightMargin(0.05);
  pRatio->SetLeftMargin(0.17);
  pRatio->SetTopMargin(0.);
  pRatio->SetBottomMargin(0.37);
  pMain->Draw();
  pRatio->Draw();

  TLegend* leg = new TLegend(0.60,0.55,0.94,0.86);
  leg->SetFillStyle(0);
  leg->SetFillColor(0);
  leg->SetLineWidth(0);

  std::vector<TH1F*> histos; histos.clear();
  std::vector<TH1F*> ratios; ratios.clear();
  for (unsigned int i0=0; i0<branches.size(); ++i0) {
    TH1F *h_temp = (TH1F*)file->Get(dir+"/"+branches[i0]); h_temp->SetName("h_"+name+"_"+branches[i0]);
    h_temp->SetLineWidth(3); h_temp->SetLineColor(colors[i0]);
    h_temp->Rebin(rebin); 
    histos.push_back(h_temp);
    
    TH1F *h_ratio = (TH1F*)histos[i0]->Clone("h_"+name+"_"+branches[i0]+"_ratio");
    h_ratio->Divide(histos[i0],histos[0]);
    ratios.push_back(h_ratio);
  }

  for (unsigned int i0=0; i0<histos.size(); ++i0) {
    pMain->cd();
    if (i0==0) {
      histos[i0]->GetYaxis()->SetTitle(yname);
      if (uselog) { gPad->SetLogy(); histos[i0]->GetYaxis()->SetRangeUser(0.1,3.*(histos[i0]->GetMaximum())); }
	else { histos[i0]->GetYaxis()->SetRangeUser(0.,1.5*(histos[i0]->GetMaximum())); }
      histos[i0]->GetXaxis()->SetTitle(xname);
      if (name.Contains("2b")) { 
	histos[i0]->GetXaxis()->SetRangeUser(0.,100.); 
	if (uselog) { gPad->SetLogy(); histos[i0]->GetYaxis()->SetRangeUser(100,3.*(histos[i0]->GetMaximum())); }
      }
      histos[i0]->Draw("HIST E0");
    }
    else { histos[i0]->Draw("HIST E0 sames"); }
    leg->AddEntry(histos[i0],legends[i0],"L");
    leg->Draw("sames");
 
    pRatio->cd();
    if (i0==0) { 
      ratios[i0]->GetYaxis()->SetTitle("Ratio");
      ratios[i0]->GetYaxis()->SetRangeUser(0.,2.);
      ratios[i0]->GetXaxis()->SetTitle(xname);
      if (name.Contains("2b")) { 
	ratios[i0]->GetYaxis()->SetRangeUser(0.8,1.2);
	ratios[i0]->GetXaxis()->SetRangeUser(0.,100.);
      }
      ratios[i0]->Draw("HIST"); 
    } else { ratios[i0]->Draw("HIST sames"); }

  }
  
  return c;
}



/*
TCanvas *makeCanvas(TString name, TTree* tree, std::vector<TString> branches, std::vector<TString> legends, std::vector<int> colors,
		    int xbins, float xmin, float xmax,TString xname, TString yname, bool uselog) {

  TCanvas *c = new TCanvas("c_"+name,"c_"+name,600,700);
  TPad *pMain  = new TPad("pMain_"+name,"pMain_"+name,0.0,0.35,1.0,1.0);
  pMain->SetRightMargin(0.05);
  pMain->SetLeftMargin(0.17);
  pMain->SetBottomMargin(0.);
  pMain->SetTopMargin(0.1);
  
  TPad *pRatio = new TPad("pRatio_"+name,"pRatio_"+name,0.0,0.0,1.0,0.35);
  pRatio->SetRightMargin(0.05);
  pRatio->SetLeftMargin(0.17);
  pRatio->SetTopMargin(0.);
  pRatio->SetBottomMargin(0.37);

  TLegend* leg = new TLegend(0.60,0.55,0.94,0.86);
  leg->SetFillStyle(0);
  leg->SetFillColor(0);
  leg->SetLineWidth(0);


  std::vector<TH1F*> histos; histos.clear();
  std::vector<TH1F*> ratios; ratios.clear();
  for (unsigned int i0=0; i0<branches.size(); ++i0) {
    TH1F *h_temp = create1Dhisto(tree,"1.","1==1",branches[i0],xbins,xmin,xmax,uselog,colors[i0],1,branches[i0],false,false); h_temp->SetName(name+"_"+branches[i0]);
    histos.push_back(h_temp);
    
    TH1F *h_ratio = (TH1F*)h_temp->Clone(name+"_"+branches[i0]+"_ratio");
    h_ratio->Divide(h_temp,histos[0]);
    ratios.push_back(h_ratio);
  }

  for (unsigned int i0=0; i0<histos.size(); ++i0) {
    std::cout << i0 <<  "\n";
    pMain->cd();
    if (i0==0) {
      histos[i0]->GetYaxis()->SetTitle(yname);
      histos[i0]->GetYaxis()->SetRangeUser(0.,1.5*(histos[i0]->GetMaximum()));
      histos[i0]->GetXaxis()->SetTitle(xname);
      histos[i0]->Draw("HIST E0");
    }
    else { histos[i0]->Draw("HIST E0 sames"); }
    leg->AddEntry(legends[i0],"L");
 
    pRatio->cd();
    if (i0==0) { ratios[i0]->Draw("L"); } else { ratios[i0]->Draw("L sames"); }

  }

}
*/


TH1F *create1Dhisto(TTree *tree,TString intLumi,TString cuts,TString branch,int bins,float xmin,float xmax,
                    bool useLog,int color, int style,TString name,bool norm,bool data) {
  TH1::SetDefaultSumw2(kTRUE);

  TString cut ="("+intLumi+")*("+cuts+")";

  TH1F *hTemp = new TH1F(name,name,bins,xmin,xmax); //hTemp->SetName(name);                                                                                                                                        
  tree->Project(name,branch,cut);

  hTemp->SetLineWidth(3);
  hTemp->SetMarkerSize(0);
  hTemp->SetLineColor(color);
  hTemp->SetFillColor(color);
  hTemp->SetLineStyle(style);

  // ad overflow bin                                                                                                                                                                                               
  double error =0.; double integral = hTemp->IntegralAndError(bins,bins+1,error);
  hTemp->SetBinContent(bins,integral);
  hTemp->SetBinError(bins,error);

  if (norm) { hTemp->Scale(1./(hTemp->Integral())); }

  return hTemp;
} //~ end of create1Dhisto





