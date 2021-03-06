 /**************************************************************************
 * Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 *                                                                        *
 * Author: The ALICE Off-line Project.                                    *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

// --- ROOT system ---
#include <TLorentzVector.h>
#include <TH2.h>
#include <TList.h>

//---- AliRoot system ----
#include "AliNeutralMesonSelection.h" 
#include "AliLog.h"

/// \cond CLASSIMP
ClassImp(AliNeutralMesonSelection) ;
/// \endcond
  
//____________________________________________________
/// Default constructor. Initialize parameters
//____________________________________________________
AliNeutralMesonSelection::AliNeutralMesonSelection() : 
TObject(),             fAsymmetryCut(1),                
fUseAsymmetryCut(0),   fM(0),                 
fInvMassMaxCut(0.),    fInvMassMinCut(0.),   
fLeftBandMinCut(0.),   fLeftBandMaxCut(0.),            
fRightBandMinCut(0.),  fRightBandMaxCut(0.),              
fAngleMaxParam(),      fUseAngleCut(0),       
fKeepNeutralMesonHistos(0),
fParticle(""),         fDecayBit(0),
fDebug(0),
// histograms
fhAnglePairNoCut(0),          fhAnglePairOpeningAngleCut(0),   
fhAnglePairAsymmetryCut(0),   fhAnglePairAllCut(0), 
fhInvMassPairNoCut(0),        fhInvMassPairOpeningAngleCut(0), 
fhInvMassPairAsymmetryCut(0), fhInvMassPairAllCut(0),
fhAsymmetryNoCut(0),          fhAsymmetryOpeningAngleCut(0),   
fhAsymmetryAllCut(0),
// histogram ranges and bins
fHistoNEBins(0),       fHistoEMax(0.),                  fHistoEMin(0.),
fHistoNAngleBins(0),   fHistoAngleMax(0.),              fHistoAngleMin(0.),
fHistoNIMBins(0),      fHistoIMMax(0.),                 fHistoIMMin(0.)
{
  InitParameters();
}

//_________________________________________________________
/// Create histograms to be saved in output file and 
/// store them in outputContainer of the analysis class that calls this class.
//_________________________________________________________
TList *  AliNeutralMesonSelection::GetCreateOutputObjects()
{    
  TList * outputContainer = new TList() ; 
  outputContainer->SetName("MesonDecayHistos") ; 
  
  if(fKeepNeutralMesonHistos){
	  
	  outputContainer->SetOwner(kFALSE);
	  
	  fhAnglePairNoCut  = new TH2F
	  ("AnglePairNoCut",
	   "Angle between all #gamma pair vs E_{#pi^{0}}",fHistoNEBins,fHistoEMin,fHistoEMax,fHistoNAngleBins,fHistoAngleMin,fHistoAngleMax); 
	  fhAnglePairNoCut->SetYTitle("Angle (rad)");
	  fhAnglePairNoCut->SetXTitle("E_{ #pi^{0}} (GeV)");
	      
    fhAsymmetryNoCut  = new TH2F
	  ("AsymmetryNoCut","Asymmetry of all #gamma pair vs E_{#pi^{0}}",
	   fHistoNEBins,fHistoEMin,fHistoEMax,100,0,1); 
	  fhAsymmetryNoCut->SetYTitle("Asymmetry");
	  fhAsymmetryNoCut->SetXTitle("E_{ #pi^{0}} (GeV)");    
    
    fhInvMassPairNoCut  = new TH2F
	  ("InvMassPairNoCut","Invariant Mass of all #gamma pair vs E_{#pi^{0}}",
	   fHistoNEBins,fHistoEMin,fHistoEMax,fHistoNIMBins,fHistoIMMin,fHistoIMMax); 
	  fhInvMassPairNoCut->SetYTitle("Invariant Mass (GeV/c^{2})");
	  fhInvMassPairNoCut->SetXTitle("E_{ #pi^{0}} (GeV)");    
    
    outputContainer->Add(fhAnglePairNoCut) ; 
	  outputContainer->Add(fhAsymmetryNoCut) ; 
    outputContainer->Add(fhInvMassPairNoCut) ; 

    if(fUseAngleCut) {
      fhAnglePairOpeningAngleCut  = new TH2F
      ("AnglePairOpeningAngleCut",
       "Angle between all #gamma pair (opening angle) vs E_{#pi^{0}}"
       ,fHistoNEBins,fHistoEMin,fHistoEMax,fHistoNAngleBins,fHistoAngleMin,fHistoAngleMax); 
      fhAnglePairOpeningAngleCut->SetYTitle("Angle (rad)");
      fhAnglePairOpeningAngleCut->SetXTitle("E_{ #pi^{0}} (GeV)");
      
      fhAsymmetryOpeningAngleCut  = new TH2F
      ("AsymmetryOpeningAngleCut",
       "Asymmetry of #gamma pair (angle cut) vs E_{#pi^{0}}",
       fHistoNEBins,fHistoEMin,fHistoEMax,100,0,1); 
      fhAsymmetryOpeningAngleCut->SetYTitle("Asymmetry");
      fhAsymmetryOpeningAngleCut->SetXTitle(" E_{#pi^{0}}(GeV)");   
      
      fhInvMassPairOpeningAngleCut  = new TH2F
      ("InvMassPairOpeningAngleCut",
       "Invariant Mass of #gamma pair (angle cut) vs E_{#pi^{0}}",
       fHistoNEBins,fHistoEMin,fHistoEMax,fHistoNIMBins,fHistoIMMin,fHistoIMMax); 
      fhInvMassPairOpeningAngleCut->SetYTitle("Invariant Mass (GeV/c^{2})");
      fhInvMassPairOpeningAngleCut->SetXTitle(" E_{#pi^{0}}(GeV)");
      
      outputContainer->Add(fhAnglePairOpeningAngleCut) ;
      outputContainer->Add(fhAsymmetryOpeningAngleCut) ;
      outputContainer->Add(fhInvMassPairOpeningAngleCut) ;
    }
    
	  if(fUseAsymmetryCut) {
      fhAnglePairAsymmetryCut  = new TH2F
      ("AnglePairAsymmetryCut",
       "Angle between all #gamma pair (opening angle + asymetry cut) vs E_{#pi^{0}}"
       ,fHistoNEBins,fHistoEMin,fHistoEMax,fHistoNAngleBins,fHistoAngleMin,fHistoAngleMax); 
      fhAnglePairAsymmetryCut->SetYTitle("Angle (rad)");
      fhAnglePairAsymmetryCut->SetXTitle("E_{ #pi^{0}} (GeV)");
      
      fhInvMassPairAsymmetryCut  = new TH2F
      ("InvMassPairAsymmetryCut",
       "Invariant Mass of #gamma pair (opening angle + asymmetry) vs E_{#pi^{0}}",
       fHistoNEBins,fHistoEMin,fHistoEMax,fHistoNIMBins,fHistoIMMin,fHistoIMMax); 
      fhInvMassPairAsymmetryCut->SetYTitle("Invariant Mass (GeV/c^{2})");
      fhInvMassPairAsymmetryCut->SetXTitle("E_{#pi^{0}}(GeV)");      
      
      outputContainer->Add(fhAnglePairAsymmetryCut) ;
      outputContainer->Add(fhInvMassPairAsymmetryCut) ;
      
    }
    
	  fhAnglePairAllCut  = new TH2F
	  ("AnglePairAllCut",
	   "Angle between all #gamma pair (opening angle + asymmetry + inv mass cut) vs E_{#pi^{0}}"
	   ,fHistoNEBins,fHistoEMin,fHistoEMax,fHistoNAngleBins,fHistoAngleMin,fHistoAngleMax); 
	  fhAnglePairAllCut->SetYTitle("Angle (rad)");
	  fhAnglePairAllCut->SetXTitle("E_{ #pi^{0}} (GeV)");    
        
	  fhInvMassPairAllCut  = new TH2F
	  ("InvMassPairAllCut",
	   "Invariant Mass of #gamma pair (opening angle + asymmetry + invmass cut) vs E_{#pi^{0}}",
	   fHistoNEBins,fHistoEMin,fHistoEMax,fHistoNIMBins,fHistoIMMin,fHistoIMMax); 
	  fhInvMassPairAllCut->SetYTitle("Invariant Mass (GeV/c^{2})");
	  fhInvMassPairAllCut->SetXTitle("E_{#pi^{0}}(GeV)");
	  
	  fhAsymmetryAllCut  = new TH2F
	  ("AsymmetryAllCut",
	   "Asymmetry of #gamma pair (opening angle+invmass cut) vs E_{#pi^{0}}",
	   fHistoNEBins,fHistoEMin,fHistoEMax,100,0,1); 
	  fhAsymmetryAllCut->SetYTitle("Asymmetry");
	  fhAsymmetryAllCut->SetXTitle("E_{#pi^{0}}(GeV)");
    
    outputContainer->Add(fhAnglePairAllCut) ; 
	  outputContainer->Add(fhAsymmetryAllCut) ; 
    outputContainer->Add(fhInvMassPairAllCut) ;     

  }
  
  return outputContainer;
}

//_____________________________________________
/// Initialize the parameters of the analysis.
//_____________________________________________
void AliNeutralMesonSelection::InitParameters()
{  
  fAngleMaxParam.Set(4) ;
  fAngleMaxParam.Reset(0.);

  SetParticle("Pi0");
  
  //Histogrammes settings
  fHistoNEBins     = 200 ;
  fHistoEMax       = 50  ;
  fHistoEMin       = 0.  ;  

  fHistoNAngleBins = 200 ;
  fHistoAngleMax   = 0.5 ;
  fHistoAngleMin   = 0.  ;
}

//______________________________________________________________________________
/// Check if the opening angle of the candidate pairs is inside 
/// our selection window.
/// Attention, only valid for Pi0, if needed for Eta need to revise max angle function or change parameters
//______________________________________________________________________________
Bool_t AliNeutralMesonSelection::IsAngleInWindow(Float_t angle, Float_t e) const
{
  Double_t max =  fAngleMaxParam.At(0)*TMath::Exp(fAngleMaxParam.At(1)*e)
    +fAngleMaxParam.At(2)+fAngleMaxParam.At(3)*e;
  Double_t arg = (e*e-2*fM*fM)/(e*e);
  Double_t min = 100. ;
  if(arg>0.)
    min = TMath::ACos(arg)+fShiftMinAngle[0]+fShiftMinAngle[1]*e;
  
  if((angle<max)&&(angle>=min)) return kTRUE  ;
  else                          return kFALSE ;

}

//_________________________________________________________________
/// Search for the neutral pion within selection cuts.
/// \param gammai: kinematics of first photon cluster.
/// \param gammaj: kinematics of second photon cluster.
/// \param calo: calorimeter ID.
/// \return pair is pi0/eta whithing selection range.
//_________________________________________________________________
Bool_t  AliNeutralMesonSelection::SelectPair(TLorentzVector gammai, 
                                             TLorentzVector gammaj, 
                                             Int_t calo)
{  
  fDecayBit = 0;
  
  //  Double_t pt  = (gammai+gammaj).Pt();
  Double_t phi = (gammai+gammaj).Phi();
  if(phi < 0)
    phi+=TMath::TwoPi();
  
  Double_t invmass = (gammai+gammaj).M();
  Double_t angle   = gammaj.Angle(gammai.Vect());
  Double_t e       = (gammai+gammaj).E();
  Double_t asy     = TMath::Abs((gammai-gammaj).E())/(gammai+gammaj).E();

  //Fill histograms with no cuts applied.
  if(fKeepNeutralMesonHistos)
  {
	  fhAnglePairNoCut  ->Fill(e,angle);
	  fhInvMassPairNoCut->Fill(e,invmass);
	  fhAsymmetryNoCut  ->Fill(e,asy);
  }
  
  //Cut on the aperture of the pair
  if(fUseAngleCut)
  {
    if(IsAngleInWindow(angle,e))
    {
      if(fKeepNeutralMesonHistos )
      {
        fhAnglePairOpeningAngleCut  ->Fill(e,angle);
        fhInvMassPairOpeningAngleCut->Fill(e,invmass);
        fhAsymmetryOpeningAngleCut  ->Fill(e,asy);
      }
      
      AliDebug(2,Form("Angle cut: energy %f, phi %f",e,phi));
      
    }
    else return kFALSE;
  }
  
  // Asymmetry cut
  if(fUseAsymmetryCut)
  {
    if(fAsymmetryCut > asy)
    {
      if(fKeepNeutralMesonHistos)
      {
        fhInvMassPairAsymmetryCut->Fill(e,invmass);
        fhAnglePairAsymmetryCut  ->Fill(e,angle);
      }
    }
    else
      return kFALSE;
  }
  
  //Cut on the invariant mass of the pair
  
  Float_t invmassmaxcut          = fInvMassMaxCut;
  Float_t invmassRightBandMinCut = fRightBandMinCut;
  Float_t invmassRightBandMaxCut = fRightBandMaxCut;

  // kEMCAL=0, kPHOS=1
  if(calo==0 && e > 6.)
  {
    // for EMCAL, pi0s, mass depends strongly with energy for e > 6, loose max cut
  
    invmassmaxcut          = (fInvMassMaxCutParam[0]+fInvMassMaxCut  )+fInvMassMaxCutParam[1]*e+fInvMassMaxCutParam[2]*e*e;
    invmassRightBandMinCut = (fInvMassMaxCutParam[0]+fRightBandMinCut)+fInvMassMaxCutParam[1]*e+fInvMassMaxCutParam[2]*e*e;
    invmassRightBandMaxCut = (fInvMassMaxCutParam[0]+fRightBandMaxCut)+fInvMassMaxCutParam[1]*e+fInvMassMaxCutParam[2]*e*e;

    //printf("e %f, max cut %f, p00 %f,p0 %f,p1 %f,p2 %f\n",
    //       e,invmassmaxcut,fInvMassMaxCut,fInvMassMaxCutParam[0],fInvMassMaxCutParam[1],fInvMassMaxCutParam[2]);
  }
  
  // normal case, invariant mass selection around pi0/eta peak
  if( !fParticle.Contains("SideBand") )
  {
    if( invmass > fInvMassMinCut && invmass < invmassmaxcut )
    {
      if     (fParticle=="Pi0") fDecayBit = kPi0;
      else if(fParticle=="Eta") fDecayBit = kEta;
      else AliWarning("Unexpected particle peak name!");
      
      if(fKeepNeutralMesonHistos)
      {
        fhInvMassPairAllCut->Fill(e,invmass);
        fhAnglePairAllCut  ->Fill(e,angle);
        fhAsymmetryAllCut  ->Fill(e,asy);
      }
      
      AliDebug(2,Form("IM in peak cut: energy %f, im %f, bit map %d",e,invmass,fDecayBit));
      
      return kTRUE;
      
    }//(invmass>0.125) && (invmass<0.145)
    else 
    {
      return kFALSE;
    }
  }// normal selection
 
  else // select a band around pi0/eta
  {
    Bool_t ok = kFALSE;
    
    // Left side?
    if( invmass > fLeftBandMinCut  && invmass < fLeftBandMaxCut )
    {
      if     (fParticle.Contains("Pi0")) fDecayBit = kPi0LeftSide;
      else if(fParticle.Contains("Eta")) fDecayBit = kEtaLeftSide;
      else AliWarning("Unexpected particle side_band name!");
      
      ok = kTRUE;
    }
    
    //Right side?
    if( invmass > invmassRightBandMinCut && invmass < invmassRightBandMaxCut )
    {
      if     (fParticle.Contains("Pi0")) { fDecayBit = kPi0RightSide; if(ok) fDecayBit = kPi0BothSides; }
      else if(fParticle.Contains("Eta")) { fDecayBit = kEtaRightSide; if(ok) fDecayBit = kEtaBothSides; }
      else AliWarning("Unexpected particle side_band name!");
      
      ok = kTRUE;
    }
    
    // Any of the sides?
    if(ok)
    {
      if(fKeepNeutralMesonHistos)
      {
        fhInvMassPairAllCut->Fill(e,invmass);
        fhAnglePairAllCut  ->Fill(e,angle);
        fhAsymmetryAllCut  ->Fill(e,asy);
      }      
      
      AliDebug(2,Form("IM side band cut: energy %f, im %f, bit map %d",e,invmass,fDecayBit));
      
      return kTRUE;
      
    }//(invmass>0.125) && (invmass<0.145)
    else 
    {
      return kFALSE;
    }
  }
}

//_______________________________________________________________
/// Set some default parameters for selection of pi0 or eta
//_______________________________________________________________
void  AliNeutralMesonSelection::SetParticle(TString particleName)
{
  fParticle = particleName ;

  if(particleName.Contains("Pi0"))
  {
    fHistoNIMBins          = 150 ;
    fHistoIMMax            = 0.3 ;
    fHistoIMMin            = 0.  ;  
    
    fM                     = 0.135 ; // GeV
    fInvMassMaxCut         = 0.16  ; // GeV
    fInvMassMinCut         = 0.11  ; // GeV

    fLeftBandMinCut        = 0.05  ; // GeV
    fLeftBandMaxCut        = 0.09  ; // GeV
    fRightBandMinCut       = 0.160 ; // GeV
    fRightBandMaxCut       = 0.165 ; // GeV

    fInvMassMaxCutParam[0] = 0.0   ;
    fInvMassMaxCutParam[1] =-7.e-5 ;
    fInvMassMaxCutParam[2] = 8.e-5 ;    
         
    fShiftMinAngle[0]      =-0.03  ;
    fShiftMinAngle[1]      = 0.0025;
    
    fAngleMaxParam.AddAt( 0.8,  0) ;
    fAngleMaxParam.AddAt(-1,    1) ;
    fAngleMaxParam.AddAt( 0.09, 2) ; //for pi0 shift, for eta maybe 0.09 
    fAngleMaxParam.AddAt(-2.e-3,3) ;
  }  
  else if(particleName.Contains("Eta"))
  {
    fHistoNIMBins          = 200  ; // GeV
    fHistoIMMax            = 0.75 ; // GeV
    fHistoIMMin            = 0.35 ; // GeV 
  
    fM                     = 0.547 ; // GeV
    fInvMassMaxCut         = 0.590 ; // GeV
    fInvMassMinCut         = 0.510 ; // GeV
    
    fLeftBandMinCut        = 0.450 ; // GeV
    fLeftBandMaxCut        = 0.500 ; // GeV
    fRightBandMinCut       = 0.600 ; // GeV
    fRightBandMaxCut       = 0.650 ; // GeV
    
    fInvMassMaxCutParam[0] = 0.00 ;
    fInvMassMaxCutParam[1] = 0.00 ;
    fInvMassMaxCutParam[2] = 0.00 ;
    
    fShiftMinAngle[0]      =-0.03 ;
    fShiftMinAngle[0]      = 0.00 ;
    
    fAngleMaxParam.AddAt( 0.80,  0) ; // Same as pi0
    fAngleMaxParam.AddAt(-0.25,  1) ; // Same as pi0
    fAngleMaxParam.AddAt( 0.12,  2) ; // Shifted with respect to pi0
    fAngleMaxParam.AddAt(-5.e-4, 3) ; // Same as pi0
  }
  else 
    printf("AliAnaNeutralMesonSelection::SetParticle(%s) *** Particle NOT defined (Pi0 or Eta), Pi0 settings by default *** \n",particleName.Data());
}

//______________________________________________________________
/// Print some relevant parameters set for the analysis.
//______________________________________________________________
void AliNeutralMesonSelection::Print(const Option_t * opt) const
{
  if(! opt)
    return;
  
  printf("**** Print %s %s ****\n", GetName(), GetTitle() ) ;

  printf("Particle %s, bit %d, mass : %f  \n", fParticle.Data(), fDecayBit, fM );
  printf("Invariant mass limits : %f < m < %f \n", fInvMassMinCut , fInvMassMinCut );
  
  printf("Use asymmetry cut? : %d ; A < %f \n", fUseAngleCut, fAsymmetryCut );
  
  printf("Use angle cut? : %d  \n", fUseAngleCut );
  if(fUseAngleCut)
  {
    printf("Angle selection param: \n");
    printf("p0 :     %f\n", fAngleMaxParam.At(0));
    printf("p1 :     %f\n", fAngleMaxParam.At(1));
    printf("p2 :     %f\n", fAngleMaxParam.At(2));
    printf("p3 :     %f\n", fAngleMaxParam.At(3));
    printf("Min angle shift : p0 = %1.3f, p1 = %1.3f,\n", fShiftMinAngle[0],fShiftMinAngle[1]);
  }
  
  printf("Keep Neutral Meson Histos = %d\n",fKeepNeutralMesonHistos);
  
  if(fKeepNeutralMesonHistos)
  {
    printf("Histograms: %3.1f < E  < %3.1f,  Nbin = %d\n",   fHistoEMin,     fHistoEMax,     fHistoNEBins);
    printf("Histograms: %3.1f < angle < %3.1f, Nbin = %d\n", fHistoAngleMin, fHistoAngleMax, fHistoNAngleBins);
    printf("Histograms: %3.1f < IM < %3.1f, Nbin = %d\n",    fHistoIMMin,    fHistoIMMax,    fHistoNIMBins);    
  }
} 
