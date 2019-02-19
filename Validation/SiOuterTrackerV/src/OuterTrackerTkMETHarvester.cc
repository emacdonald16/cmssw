#include "Validation/SiOuterTrackerV/interface/OuterTrackerTkMETHarvester.h"

OuterTrackerTkMETHarvester::OuterTrackerTkMETHarvester(const edm::ParameterSet& iConfig)
{
}

OuterTrackerTkMETHarvester::~OuterTrackerTkMETHarvester()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void OuterTrackerTkMETHarvester::dqmEndJob(DQMStore::IBooker & ibooker, DQMStore::IGetter & igetter) {
  using namespace edm;

  // Global variables
  TF1* fit = new TF1("fit", "gaus", -100.0,100.0);


  std::vector<double> sigma1;
  std::vector<double> error1;


  //float bins[]={0,20,30,40,55,75,120,150,250};
  //int binnum = 8;

  float bins[]={0,20,30,40,55,75,120};
  int binnum = 6;

  dbe = nullptr;
  dbe = edm::Service<DQMStore>().operator->();

  if (dbe) {
    // Find all monitor elements for histograms
    MonitorElement *tkMET_bin1 = dbe->get("SiOuterTrackerV/tkMET/tkMETResolution/tkMETDiff_bin1");
    MonitorElement *tkMET_bin2 = dbe->get("SiOuterTrackerV/tkMET/tkMETResolution/tkMETDiff_bin2");
    MonitorElement *tkMET_bin3 = dbe->get("SiOuterTrackerV/tkMET/tkMETResolution/tkMETDiff_bin3");
    MonitorElement *tkMET_bin4 = dbe->get("SiOuterTrackerV/tkMET/tkMETResolution/tkMETDiff_bin4");
    MonitorElement *tkMET_bin5 = dbe->get("SiOuterTrackerV/tkMET/tkMETResolution/tkMETDiff_bin5");
    MonitorElement *tkMET_bin6 = dbe->get("SiOuterTrackerV/tkMET/tkMETResolution/tkMETDiff_bin6");
    MonitorElement *tkMET_bin7 = dbe->get("SiOuterTrackerV/tkMET/tkMETResolution/tkMETDiff_bin7");
    MonitorElement *tkMET_bin8 = dbe->get("SiOuterTrackerV/tkMET/tkMETResolution/tkMETDiff_bin8");

    if (tkMET_bin1 && tkMET_bin2 && tkMET_bin3 && tkMET_bin4 && tkMET_bin5 && tkMET_bin6 && tkMET_bin7 && tkMET_bin8 ) {
      // Set the current directoy
      dbe->setCurrentFolder("SiOuterTrackerV/tkMET/FinalResolution");

      // Grab the histograms
      TH1F *res_trkMET1a = tkMET_bin1->getTH1F();
      TH1F *res_trkMET1b = tkMET_bin2->getTH1F();
      TH1F *res_trkMET1c = tkMET_bin3->getTH1F();
      TH1F *res_trkMET1d = tkMET_bin4->getTH1F();
      TH1F *res_trkMET1e = tkMET_bin5->getTH1F();
      TH1F *res_trkMET1f = tkMET_bin6->getTH1F();
      TH1F *res_trkMET1g = tkMET_bin7->getTH1F();
      TH1F *res_trkMET1h = tkMET_bin8->getTH1F();

      // Book the new histogram to contain the results
      MonitorElement *me_res_trkMET1 = ibooker.book1D("tkMETRes","tkMET Resolution",binnum,bins);
      TH1F *resTkMET1 = me_res_trkMET1->getTH1F();
      resTkMET1->GetXaxis()->SetTitle("trueTkMET [GeV]");
      resTkMET1->GetYaxis()->SetTitle("#sigma(#DeltaTkMET)/trueTkMET");
      resTkMET1->SetMinimum(0.0);
      resTkMET1->SetStats(false);

      int testNumEntries1 = res_trkMET1a->GetEntries();
      if (testNumEntries1>0) {
        // Fit the histograms with a gaussian curve - take sigma and the error from the fit
        res_trkMET1a->Fit(fit,"R"); res_trkMET1b->Fit(fit,"R"); res_trkMET1c->Fit(fit,"R"); res_trkMET1d->Fit(fit,"R");
        res_trkMET1e->Fit(fit,"R"); res_trkMET1f->Fit(fit,"R");
        //res_trkMET1g->Fit(fit,"R"); res_trkMET1h->Fit(fit,"R");
        sigma1.push_back(res_trkMET1a->GetFunction("fit")->GetParameter(2));
        sigma1.push_back(res_trkMET1b->GetFunction("fit")->GetParameter(2));
        sigma1.push_back(res_trkMET1c->GetFunction("fit")->GetParameter(2));
        sigma1.push_back(res_trkMET1d->GetFunction("fit")->GetParameter(2));
        sigma1.push_back(res_trkMET1e->GetFunction("fit")->GetParameter(2));
        sigma1.push_back(res_trkMET1f->GetFunction("fit")->GetParameter(2));
        //sigma1.push_back(res_trkMET1g->GetFunction("fit")->GetParameter(2));
        //sigma1.push_back(res_trkMET1h->GetFunction("fit")->GetParameter(2));

        error1.push_back(res_trkMET1a->GetFunction("fit")->GetParError(2));
        error1.push_back(res_trkMET1b->GetFunction("fit")->GetParError(2));
        error1.push_back(res_trkMET1c->GetFunction("fit")->GetParError(2));
        error1.push_back(res_trkMET1d->GetFunction("fit")->GetParError(2));
        error1.push_back(res_trkMET1e->GetFunction("fit")->GetParError(2));
        error1.push_back(res_trkMET1f->GetFunction("fit")->GetParError(2));
        //error1.push_back(res_trkMET1g->GetFunction("fit")->GetParError(2));
        //error1.push_back(res_trkMET1h->GetFunction("fit")->GetParError(2));

        // Fill the new histogram to create resolution plot
        for(int i=0;i<6;i++) {
          //for(int i=0;i<=binnum;i++) {
          float avgMET = (bins[i]+bins[i+1])/2;
          resTkMET1->SetBinContent(i+1,sigma1[i]/avgMET);
          resTkMET1->SetBinError(i+1,error1[i]/avgMET);
        }
      }
      else {
        edm::LogWarning("DataNotFound")<<"Entries not found for tkMET resolution!\n";
        for(int i=0;i<=binnum;i++) {
          resTkMET1->SetBinContent(i+1,-1);
          resTkMET1->SetBinError(i+1,-1);
        }
      }
    } //if ME found
    else {edm::LogWarning("DataNotFound")<<"Monitor elements for tkMET resolution cannot be found!\n";}



  } //if dbe found
  else {edm::LogWarning("DataNotFound")<<"Cannot find valid DQM back end \n";}
  delete fit;
} //end dqmEndJob

DEFINE_FWK_MODULE(OuterTrackerTkMETHarvester);
