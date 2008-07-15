#ifndef DQMOFFLINE_TRIGGER_EGAMMAHLTEFFSRC
#define DQMOFFLINE_TRIGGER_EGAMMAHLTEFFSRC


//class: EgammaHLTEffSrc
//
//author: Sam Harper (June 2008)
//
//WARNING: interface is NOT final, please dont use this class for now without clearing it with me
//         as I will change it and possibly break all your code
//
//aim: to create MonitorElements (ie histograms) which can be used to measure trigger efficiencies
//
//
//implimentation:

#include "DQMOffline/Trigger/interface/MonElemManager.h"
#include "DQMOffline/Trigger/interface/EgHLTDQMCut.h"
#include "DQMOffline/Trigger/interface/EgHLTOffData.h"

template<class T> class EgammaHLTEffSrcBase {
 public:
  EgammaHLTEffSrcBase(){}
  virtual ~EgammaHLTEffSrcBase(){}

  virtual void fill(const T& obj,const EgHLTOffData& evtData,float weight)=0;

};

template<class T,typename varType> class EgammaHLTEffSrc : public EgammaHLTEffSrcBase<T> {

 private:
  MonElemManager<T,varType> *numMonElem_; //numerator hist, we own this
  MonElemManager<T,varType> *denMonElem_; //denomenator hist we own this

  EgHLTDQMCut<T> *cut_; //we own this
  
 private:
  //no copying allowed
  EgammaHLTEffSrc(const EgammaHLTEffSrc& rhs){}
  EgammaHLTEffSrc& operator=(const EgammaHLTEffSrc& rhs){return *this;}

 public:
  EgammaHLTEffSrc(const std::string& name,const std::string& title,int nrBins,double xMin,double xMax,
		  varType (T::*varFunc)()const,
		  EgHLTDQMCut<T>* cut);
  ~EgammaHLTEffSrc();

  void fill(const T& obj,const EgHLTOffData& evtData,float weight);

};


template<class T,typename varType> EgammaHLTEffSrc<T,varType>::EgammaHLTEffSrc(const std::string& name,const std::string &title,
									       int nrBins,double xMin,double xMax,
									       varType (T::*varFunc)()const,
									       EgHLTDQMCut<T>* cut)
{
  numMonElem_ = new MonElemManager<T,varType>(name+"_pass",title+" Pass",nrBins,xMin,xMax,varFunc);
  denMonElem_ = new MonElemManager<T,varType>(name+"_all",title+" All",nrBins,xMin,xMax,varFunc); 
  cut_=cut;

}

template<class T,typename varType> EgammaHLTEffSrc<T,varType>::~EgammaHLTEffSrc()
{
  delete cut_;
  delete numMonElem_;
  delete denMonElem_;
}


template<class T,typename varType> void EgammaHLTEffSrc<T,varType>::fill(const T& obj,const EgHLTOffData& evtData,float weight)
{
  denMonElem_->fill(obj,weight);
  if(cut_->pass(obj,evtData)) numMonElem_->fill(obj,weight);
}



#endif
