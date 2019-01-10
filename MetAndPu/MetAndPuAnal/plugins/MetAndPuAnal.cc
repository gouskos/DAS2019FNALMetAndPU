// -*- C++ -*-
//
// Package:    MetAndPu/MetAndPuAnal
// Class:      MetAndPuAnal
// 
/**\class MetAndPuAnal MetAndPuAnal.cc MetAndPu/MetAndPuAnal/plugins/MetAndPuAnal.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Loukas Gouskos
//         Created:  Tue, 20 Nov 2018 23:02:05 GMT
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/Math/interface/LorentzVector.h"

//ROOT includes                                                                                                                                               
#include "TTree.h"
#include <TFile.h>
#include <TROOT.h>
#include "TBranch.h"
#include <string>
#include <vector>
#include "TSystem.h"
#include "TVector3.h"
#include "TLorentzVector.h"

//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<> and also remove the line from
// constructor "usesResource("TFileService");"
// This will improve performance in multithreaded jobs.

class MetAndPuAnal : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit MetAndPuAnal(const edm::ParameterSet&);
      ~MetAndPuAnal();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      // ----------member data ---------------------------
  edm::EDGetTokenT<reco::VertexCollection> vtxToken_;
  edm::EDGetTokenT<pat::MuonCollection> muonToken_;
  edm::EDGetTokenT<pat::METCollection> metToken_;

  TFile *file = new TFile("step3_metperformance.root","recreate");
  TTree *tree = new TTree("tree","tree");

  edm::RunNumber_t irun;
  edm::EventNumber_t ievent;
  edm::LuminosityBlockNumber_t ilumiblock;
  edm::Timestamp itime;

  size_t run, event, lumi, time;

  unsigned int npv;

  Float_t metpt;
  Float_t metphi;

  Float_t upara;
  Float_t uperp;
  Float_t upara_illia;
  Float_t uperp_illia;

  Int_t ndimu;
  Float_t dimupt;
  Float_t dimueta;
  Float_t dimuphi;
  Float_t dimuq;
  Float_t dimumass;

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
MetAndPuAnal::MetAndPuAnal(const edm::ParameterSet& iConfig):
  vtxToken_(consumes<reco::VertexCollection>(iConfig.getParameter<edm::InputTag>("vertices"))),
  muonToken_(consumes<pat::MuonCollection>(iConfig.getParameter<edm::InputTag>("muons"))),
  metToken_(consumes<pat::METCollection>(iConfig.getParameter<edm::InputTag>("mets")))
{
   //now do what ever initialization is needed
   usesResource("TFileService");

   tree->Branch("npv",&npv,"npv/i");

   tree->Branch("metpt"  , &metpt  , "metpt/F"  );
   tree->Branch("metphi" , &metphi , "metphi/F" );

   tree->Branch("upara"       , &upara       , "upara/F" );
   tree->Branch("uperp"       , &uperp       , "uperp/F" );
   tree->Branch("upara_illia" , &upara_illia , "upara_illia/F" );
   tree->Branch("uperp_illia" , &uperp_illia , "uperp_illia/F" );

   tree->Branch("ndimu"    , &ndimu    , "ndimu/I"    );
   tree->Branch("dimupt"   , &dimupt   , "dimupt/F"   );
   tree->Branch("dimueta"  , &dimueta  , "dimueta/F"  );
   tree->Branch("dimuphi"  , &dimuphi  , "dimuphi/F"  );
   tree->Branch("dimuq"    , &dimuq    , "dimuq/F"    );
   tree->Branch("dimumass" , &dimumass , "dimumass/F" );

}


MetAndPuAnal::~MetAndPuAnal()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
  file->Close();
}


//
// member functions
//

// ------------ method called for each event  ------------
void
MetAndPuAnal::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   
   using namespace edm;
   typedef math::XYZTLorentzVector LorentzVector;

   edm::Handle<reco::VertexCollection> vertices;
   iEvent.getByToken(vtxToken_, vertices);
   if (vertices->empty()) return; // skip the event if no PV found
   const reco::Vertex &PV = vertices->front();

   edm::Handle<pat::MuonCollection> muons;
   iEvent.getByToken(muonToken_, muons);

   edm::Handle<pat::METCollection> mets;
   iEvent.getByToken(metToken_, mets);
   const pat::MET &met = mets->front();


   // event variables
   irun       = iEvent.id().run();
   ievent     = iEvent.id().event();
   ilumiblock = iEvent.id().luminosityBlock();
   itime      = iEvent.time();

   run   = (size_t)irun;
   event = (size_t)ievent;
   lumi  = (size_t)ilumiblock;
   time  = (size_t)((iEvent.time().value())>>32);


   // muon collection
   std::vector<pat::Muon> dimu;
   for (const pat::Muon &mu : *muons) {
     if (mu.pt() < 10 || fabs(mu.eta())>2.4) { continue; }
     if (!mu.isLooseMuon())                  { continue; }

     float isodbeta = (mu.pfIsolationR04().sumChargedHadronPt + std::max(0., mu.pfIsolationR04().sumNeutralHadronEt + mu.pfIsolationR04().sumPhotonEt - 0.5*mu.pfIsolationR04().sumPUPt))/mu.pt();

     if (isodbeta>0.2)  { continue; }

     float d0 = mu.innerTrack().isNonnull() ? -1.*mu.innerTrack()->dxy(PV.position()) : 0 ;
     float dz = mu.innerTrack().isNonnull() ? mu.innerTrack()->dz(PV.position()) : 0 ;

     if (std::fabs(d0)>0.05) { continue; }
     if (std::fabs(dz)>0.10) { continue; }

     //di-mu events
     if (mu.pt()>=10) { dimu.push_back(mu); }

   }


   // calculate boson recoil
   TLorentzVector metT1p4;      metT1p4.SetPtEtaPhiM(met.pt(),met.eta(),met.phi(),met.mass());
   TLorentzVector bosonp4;      bosonp4.SetPtEtaPhiM(0.,0.,0.,0.);
   TLorentzVector recmetT1p4;   recmetT1p4.SetPtEtaPhiM(0.,0.,0.,0.);
   float          recmetT1_par_ = -9.;
   float          recmetT1_per_ = -9.;
   float          upara_illia_  = -9.;
   float          uperp_illia_  = -9.;


   //di-mu events
   float dimupt_   = -9.;
   float dimueta_  = -9.;
   float dimuphi_  = -9.;
   float dimumass_ = -9.;
   float dimuq_    = 0.;
   if (dimu.size()>=2) {

     TLorentzVector mu1p4;      mu1p4.SetPtEtaPhiM(dimu[0].pt(),dimu[0].eta(),dimu[0].phi(),dimu[0].mass());
     TLorentzVector mu2p4;      mu2p4.SetPtEtaPhiM(dimu[1].pt(),dimu[1].eta(),dimu[1].phi(),dimu[1].mass());

     if (dimu[0].charge()*dimu[1].charge()<0.) {

       bosonp4       = mu1p4 + mu2p4;
       recmetT1p4    = metT1p4 + mu1p4 + mu2p4;
       float cosPhi  = (bosonp4.Px()*recmetT1p4.Px()+bosonp4.Py()*recmetT1p4.Py())/(bosonp4.Pt()*recmetT1p4.Pt());
       float sinPhi  = sqrt(1.-cosPhi*cosPhi);
       recmetT1_par_ = cosPhi*recmetT1p4.Pt();
       recmetT1_per_ = sinPhi*recmetT1p4.Pt();


       dimupt_   = bosonp4.Pt();
       dimueta_  = bosonp4.Eta();
       dimuphi_  = bosonp4.Phi();
       dimumass_ = (mu1p4 + mu2p4).M();
       dimuq_    = dimu[0].charge()*dimu[1].charge();

       upara_illia_ = ((( -1.*metT1p4.Pt()*TMath::Cos(metT1p4.Phi()) - dimupt_ * TMath::Cos(dimuphi_))* dimupt_*TMath::Cos( dimuphi_ )+(-1.*metT1p4.Pt()* TMath::Sin(metT1p4.Phi() )- dimupt_*TMath::Sin(dimuphi_))* dimupt_*TMath::Sin(dimuphi_ ))/dimupt_ + dimupt_);
       uperp_illia_ = ((( -1.*metT1p4.Pt()*TMath::Cos(metT1p4.Phi() ) - dimupt_ * TMath::Cos( dimuphi_))* dimupt_*TMath::Sin( dimuphi_ )-(-1.*metT1p4.Pt()* TMath::Sin(metT1p4.Phi() )- dimupt_*TMath::Sin(dimuphi_))* dimupt_*TMath::Cos( dimuphi_ ))/dimupt_);

     }
   } // end of di-mu case


   // store the variables
   npv = vertices->size();

   metpt       = met.pt();
   metphi      = met.phi();

   upara       = recmetT1_par_;
   uperp       = recmetT1_per_;
   upara_illia = upara_illia_;
   uperp_illia = uperp_illia_;


   ndimu    = dimu.size();
   dimupt   = dimupt_;
   dimueta  = dimueta_;
   dimuphi  = dimuphi_;
   dimumass = dimumass_;
   dimuq    = dimuq_;


   if (dimu.size()>1) { tree->Fill(); }


#ifdef THIS_IS_AN_EVENT_EXAMPLE
   Handle<ExampleData> pIn;
   iEvent.getByLabel("example",pIn);
#endif
   
#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
   ESHandle<SetupData> pSetup;
   iSetup.get<SetupRecord>().get(pSetup);
#endif
}


// ------------ method called once each job just before starting event loop  ------------
void 
MetAndPuAnal::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
MetAndPuAnal::endJob() 
{
  file->cd();
  tree->Write();
  file->Write();
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
MetAndPuAnal::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(MetAndPuAnal);
