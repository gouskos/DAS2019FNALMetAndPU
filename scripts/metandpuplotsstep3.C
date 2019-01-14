#include "setTDRStyle.h"
TH1F *create1DEmptyhistoVarBins(TString name,int bins,float *varbins,int color,int style);
TH1F *create1DEmptyhisto(TString name,int bins,float xmin, float xmax,int color,int style);
TH1F *create1Dhisto(TTree *tree,TString intLumi,TString cuts,TString branch,int bins,float xmin,float xmax,bool useLog,int color,
                    int style,TString name,bool norm,bool data);
TH1F *create1DhistoVarBins(TTree *tree,TString intLumi,TString cuts,TString branch,int bins,float *varbins,bool useLog,int color,
			   int style,TString name,bool norm,bool data);
TCanvas *createCanvas(TString cName, TLegend *leg, std::vector<TH1F*> hist,TString xname,TString yname,TString rname);

void metandpuplotsstep3(TString step) {

  gROOT->SetBatch(false);
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(1);
  gStyle->SetPalette(1);
  setTDRStyle();

  TFile *f_ = TFile::Open("/eos/uscms/store/user/cmsdas/2019/short_exercises/METAndPU/data/metandpu_step3.root","READONLY");
  // /eos/uscms/store/user/loukas/DASTEST/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/CRAB3_LGDAS2019_TRY/181129_234304/0000/metandpu_step3.root
  TTree *t_ = (TTree*)f_->Get("metandpuanalstep3/tree");


  std::vector<TString> cuts; cuts.clear();
  cuts.push_back("(0<=dimupt && dimupt<25)");
  cuts.push_back("(25<=dimupt && dimupt<50)");
  cuts.push_back("(50<=dimupt && dimupt<75)");
  cuts.push_back("(75<=dimupt && dimupt<100)");
  cuts.push_back("(100<=dimupt && dimupt<150)");
  cuts.push_back("(150<=dimupt && dimupt<200)");
  cuts.push_back("(200<=dimupt)");

  std::vector<TString> cuts_npv; cuts_npv.clear();
  cuts_npv.push_back("(0<=npv && npv<10)");
  cuts_npv.push_back("(10<=npv && npv<20)");
  cuts_npv.push_back("(20<=npv && npv<30)");
  cuts_npv.push_back("(30<=npv && npv<40)");
  cuts_npv.push_back("(40<=npv && npv<50)");
  cuts_npv.push_back("(50<=npv && npv<60)");
  cuts_npv.push_back("(60<=npv)");

  //  float varbins[] = {0.,10.,20.,30.,40.,50.,60.,70.,80.,90.,100.,120.,160.,200.,300.,500.};
  float varbins[] = {0.,25.,50.,75.,100.,150.,200.,300.};
  int bins = sizeof(varbins)/sizeof(Float_t) - 1;
  float varbins_npv[] = {0.,10.,20.,30.,40.,50.,60.,70.};
  int bins_npv = sizeof(varbins_npv)/sizeof(Float_t) - 1;

  TH1F *h_scale_raw   = create1DEmptyhistoVarBins("h_scale_raw",bins,varbins,1,1);
  TH1F *h_resperp_raw_vspt = create1DEmptyhistoVarBins("h_resperp_raw_vspt",bins,varbins,1,1);
  TH1F *h_respara_raw_vspt = create1DEmptyhistoVarBins("h_respara_raw_vspt",bins,varbins,1,1);
  TH1F *h_resperp_raw_vspu = create1DEmptyhistoVarBins("h_resperp_raw_vspu",bins_npv,varbins_npv,1,1);
  TH1F *h_respara_raw_vspu = create1DEmptyhistoVarBins("h_respara_raw_vspu",bins_npv,varbins_npv,1,1);

  TH1F *h_scale_t1   = create1DEmptyhistoVarBins("h_scale_t1",bins,varbins,2,1);
  TH1F *h_resperp_t1_vspt = create1DEmptyhistoVarBins("h_resperp_t1_vspt",bins,varbins,2,1);
  TH1F *h_respara_t1_vspt = create1DEmptyhistoVarBins("h_respara_t1_vspt",bins,varbins,2,1);
  TH1F *h_resperp_t1_vspu = create1DEmptyhistoVarBins("h_resperp_t1_vspu",bins_npv,varbins_npv,2,1);
  TH1F *h_respara_t1_vspu = create1DEmptyhistoVarBins("h_respara_t1_vspu",bins_npv,varbins_npv,2,1);

  std::vector<TH1F*> hist_scale;   hist_scale.clear();
  std::vector<TH1F*> hist_respara_vspt; hist_respara_vspt.clear();
  std::vector<TH1F*> hist_resperp_vspt; hist_resperp_vspt.clear();
  std::vector<TH1F*> hist_respara_vspu; hist_respara_vspu.clear();
  std::vector<TH1F*> hist_resperp_vspu; hist_resperp_vspu.clear();

  for (unsigned int i0=0; i0<cuts_npv.size(); ++i0) {

    TH1F *h_scale0_t1 = create1Dhisto(t_,"1.","ndimu>=2 && dimupt>=50. && "+cuts_npv[i0],"(uparat1)/dimupt",4000,-100.,100.,true,2,1,"h_scale0_t1",false,false);
    TH1F *h_upara_t1  = create1Dhisto(t_,"1.","ndimu>=2 && dimupt>=50. && "+cuts_npv[i0],"(uparat1-dimupt)",100,-500.,500.,true,2,1,"h_upara_t1",false,false);
    TH1F *h_uperp_t1  = create1Dhisto(t_,"1.","ndimu>=2 && dimupt>=50. && "+cuts_npv[i0],"uperpt1",20000,-1000.,1000.,true,2,1,"h_uperp_t1",false,false);

    float scalecor_t1 = 1.; if (h_scale0_t1->GetMean()!=0) { scalecor_t1 = 1./h_scale0_t1->GetMean(); }
    h_respara_t1_vspu->SetBinContent(i0+1,scalecor_t1*(h_upara_t1->GetRMS())); h_respara_t1_vspu->SetBinError(i0+1,scalecor_t1*(h_upara_t1->GetRMSError()));
    h_resperp_t1_vspu->SetBinContent(i0+1,scalecor_t1*(h_uperp_t1->GetRMS())); h_resperp_t1_vspu->SetBinError(i0+1,scalecor_t1*(h_uperp_t1->GetRMSError()));

    hist_respara_vspu.push_back(h_respara_raw_vspu);
    hist_resperp_vspu.push_back(h_resperp_raw_vspu);
    hist_respara_vspu.push_back(h_respara_t1_vspu);
    hist_resperp_vspu.push_back(h_resperp_t1_vspu);

    h_scale0_t1->Delete(); h_upara_t1->Delete(); h_uperp_t1->Delete();

  }

  for (unsigned int i0=0; i0<cuts.size(); ++i0) {
    
    TH1F *h_scale0_raw = create1Dhisto(t_,"1.","ndimu>=2 && "+cuts[i0],"(upararaw)/dimupt",4000,-100.,100.,true,2,1,"h_scale0_raw",false,false);
    TH1F *h_upara_raw  = create1Dhisto(t_,"1.","ndimu>=2 && "+cuts[i0],"(upararaw-dimupt)",100,-500.,500.,true,2,1,"h_upara_raw",false,false);
    TH1F *h_uperp_raw  = create1Dhisto(t_,"1.","ndimu>=2 && "+cuts[i0],"uperpraw",20000,-1000.,1000.,true,2,1,"h_uperp_raw",false,false);

    TH1F *h_scale0_t1 = create1Dhisto(t_,"1.","ndimu>=2 && "+cuts[i0],"(uparat1)/dimupt",4000,-100.,100.,true,2,1,"h_scale0_t1",false,false);
    TH1F *h_upara_t1  = create1Dhisto(t_,"1.","ndimu>=2 && "+cuts[i0],"(uparat1-dimupt)",100,-500.,500.,true,2,1,"h_upara_t1",false,false);
    TH1F *h_uperp_t1  = create1Dhisto(t_,"1.","ndimu>=2 && "+cuts[i0],"uperpt1",20000,-1000.,1000.,true,2,1,"h_uperp_t1",false,false);
    
    h_scale_raw->SetBinContent(i0+1,1.*(h_scale0_raw->GetMean())); h_scale_raw->SetBinError(i0+1,h_scale0_raw->GetMeanError());
    float scalecor_raw = 1.; if (h_scale0_raw->GetMean()!=0) { scalecor_raw = 1./h_scale0_raw->GetMean(); }
    if (step == "step3_resolution_pftype1") {
      //      h_respara_raw_vspt->SetBinContent(i0+1,scalecor_raw*(h_upara_raw->GetRMS())); h_respara_raw_vspt->SetBinError(i0+1,scalecor_raw*(h_upara_raw->GetRMSError()));
      //      h_resperp_raw_vspt->SetBinContent(i0+1,scalecor_raw*(h_uperp_raw->GetRMS())); h_resperp_raw_vspt->SetBinError(i0+1,scalecor_raw*(h_uperp_raw->GetRMSError()));
    }
    
    h_scale_t1->SetBinContent(i0+1,1.*(h_scale0_t1->GetMean())); h_scale_t1->SetBinError(i0+1,h_scale0_t1->GetMeanError());
    float scalecor_t1 = 1.; if (h_scale0_t1->GetMean()!=0) { scalecor_t1 = 1./h_scale0_t1->GetMean(); }
    if (step == "step3_resolution_pftype1") {
      h_respara_t1_vspt->SetBinContent(i0+1,scalecor_t1*(h_upara_t1->GetRMS())); h_respara_t1_vspt->SetBinError(i0+1,scalecor_t1*(h_upara_t1->GetRMSError()));
      h_resperp_t1_vspt->SetBinContent(i0+1,scalecor_t1*(h_uperp_t1->GetRMS())); h_resperp_t1_vspt->SetBinError(i0+1,scalecor_t1*(h_uperp_t1->GetRMSError()));
    }
    
    hist_scale.push_back(h_scale_raw); 
    if (step == "step3_resolution_pftype1") {
      hist_respara_vspt.push_back(h_respara_t1_vspt); 
      hist_resperp_vspt.push_back(h_resperp_t1_vspt); 
    }

    if (step == "step3_scale_pftype1") {
      hist_scale.push_back(h_scale_t1); 
    }

    h_scale0_raw->Delete(); h_upara_raw->Delete(); h_uperp_raw->Delete();
    h_scale0_t1->Delete();  h_upara_t1->Delete();  h_uperp_t1->Delete();

  }

  TLegend* leg = new TLegend(0.60,0.75,0.94,0.96);
  leg->SetFillStyle(0);
  leg->SetFillColor(0);
  leg->SetLineWidth(0);
  if (step == "step3_scale_pfraw")   { 
    leg->AddEntry(hist_scale[0],"PFMET Raw","L"); 
    TCanvas *c_scale = createCanvas("c_scale",leg,hist_scale,"q_{T} [GeV]","-u_{||}/q_{T}","_i / _0");
  }

  if (step == "step3_scale_pftype1") { 
    leg->AddEntry(hist_scale[0],"PFMET Raw","L");
    leg->AddEntry(hist_scale[1],"PFMET T1","L"); 
    TCanvas *c_scale = createCanvas("c_scale",leg,hist_scale,"q_{T} [GeV]","-u_{||}/q_{T}","_i / _0");
  }

  if (step == "step3_resolution_pftype1") {
    leg->AddEntry(hist_respara_vspt[0],"PFMET T1","L"); 
    TCanvas *c_respara_vspt = createCanvas("c_respara_vspt",leg,hist_respara_vspt,"q_{T} [GeV]","#sigma u_{||} [GeV]","_i / _0");
    TCanvas *c_resperp_vspt = createCanvas("c_resperp_vspt",leg,hist_resperp_vspt,"q_{T} [GeV]","#sigma u_{#perp} [GeV]","_i / _0");
    TCanvas *c_respara_vspu = createCanvas("c_respara_vspu",leg,hist_respara_vspu,"N_{vtx}","#sigma u_{||} [GeV]","_i / _0");
    TCanvas *c_resperp_vspu = createCanvas("c_resperp_vspu",leg,hist_resperp_vspu,"N_{vtx}","#sigma u_{#perp} [GeV]","_i / _0");
  }

}

TH1F *create1Dhisto(TTree *tree,TString intLumi,TString cuts,TString branch,int bins,float xmin,float xmax,bool useLog,int color,
                    int style,TString name,bool norm,bool data) {
  TH1::SetDefaultSumw2(kTRUE);


  TString cut ="("+cuts+")";

  TH1F *hTemp = new TH1F(name,name,bins,xmin,xmax); //hTemp->SetName(name);                                                                                   
  tree->Project(name,branch,cut);

  hTemp->SetLineWidth(3);
  hTemp->SetMarkerSize(0);
  hTemp->SetLineColor(color);
  hTemp->SetLineStyle(style);

  // ad overflow bin                                                                                                                                          
  double error =0.; double integral = hTemp->IntegralAndError(bins,bins+1,error);
  hTemp->SetBinContent(bins,integral);
  hTemp->SetBinError(bins,error);

  if (norm) { hTemp->Scale(1./(hTemp->Integral())); }

  return hTemp;
} //~ end of create1Dhisto


TH1F *create1DhistoVarBins(TTree *tree,TString intLumi,TString cuts,TString branch,int bins,float *varbins,bool useLog,int color,
			   int style,TString name,bool norm,bool data) {
  TH1::SetDefaultSumw2(kTRUE);


  TString cut ="("+cuts+")";

  TH1F *hTemp = new TH1F(name,name,bins,varbins); //hTemp->SetName(name);                                                                                   
  tree->Project(name,branch,cut);

  hTemp->SetLineWidth(3);
  hTemp->SetMarkerSize(0);
  hTemp->SetLineColor(color);
  hTemp->SetLineStyle(style);

  // ad overflow bin
  double error =0.; double integral = hTemp->IntegralAndError(bins,bins+1,error);
  hTemp->SetBinContent(bins,integral);
  hTemp->SetBinError(bins,error);

  if (norm) { hTemp->Scale(1./(hTemp->Integral())); }

  return hTemp;
} //~ end of create1Dhisto



TH1F *create1DEmptyhistoVarBins(TString name,int bins,float *varbins,int color,int style) {
  TH1::SetDefaultSumw2(kTRUE);

  TH1F *hTemp = new TH1F(name,name,bins,varbins);
  hTemp->SetLineWidth(3);
  hTemp->SetMarkerSize(0);
  hTemp->SetLineColor(color);
  hTemp->SetLineStyle(style);

  return hTemp;
} //~ end of create1Dhisto


TH1F *create1DEmptyhisto(TString name,int bins,float xmin, float xmax,int color,int style) {
  TH1::SetDefaultSumw2(kTRUE);

  TH1F *hTemp = new TH1F(name,name,bins,xmin,xmax);
  hTemp->SetLineWidth(3);
  hTemp->SetMarkerSize(0);
  hTemp->SetLineColor(color);
  hTemp->SetLineStyle(style);

  return hTemp;
} //~ end of create1Dhisto



TCanvas *createCanvas(TString cName, TLegend *leg, std::vector<TH1F*> hist,TString xname,TString yname,TString rname) {

  TCanvas *cTemp = new TCanvas("c_"+cName,"c_"+cName,500,500); cTemp->SetName("c_"+cName);
  for (unsigned int i0=0; i0<hist.size(); ++i0) {

    ostringstream tmpcount;
    tmpcount << i0;
    TString count = tmpcount.str();

    if (i0==0) {
      hist[i0]->GetYaxis()->SetTitleOffset(1.08);
      hist[i0]->GetYaxis()->SetTitle(yname);
      hist[i0]->GetXaxis()->SetTitle(xname);
      if (cName.Contains("scale")) { hist[i0]->GetYaxis()->SetRangeUser(0.5,1.2); }
      if (cName.Contains("para")) { hist[i0]->GetYaxis()->SetRangeUser(10.,60.); }
      if (cName.Contains("perp")) { hist[i0]->GetYaxis()->SetRangeUser(0.,30.); }
      if (cName.Contains("vspt")) { hist[i0]->GetXaxis()->SetRangeUser(50.,300.); }
      hist[i0]->GetXaxis()->SetNdivisions(109);
      hist[i0]->GetYaxis()->SetLabelSize(0.08);
      hist[i0]->GetYaxis()->SetTitleSize(0.08);
      hist[i0]->Draw("HIST E0");
    }
    else { 
      hist[i0]->Draw("HIST E0 sames");
    }

    leg->Draw("sames");
    cTemp->RedrawAxis();

  }

  return cTemp;
}
