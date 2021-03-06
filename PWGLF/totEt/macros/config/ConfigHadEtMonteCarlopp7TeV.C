
AliAnalysisHadEtMonteCarlo * ConfigHadEtMonteCarlo(){
  //cout<<"Hello I am configuring you"<<endl;
  cout<<"You are analyzing 7 TeV p+p simulations"<<endl;
  AliAnalysisHadEtMonteCarlo *hadEtMC = new AliAnalysisHadEtMonteCarlo();
  //Whether or not to investigate the effects of efficiency, momentum resolution, PID, etc.
  hadEtMC->InvestigateSmearing(kTRUE);

  //Turns off O(100) histograms that we do not normally use
  hadEtMC->RunLightweight(kFALSE);

  //Whether or not to look at Et(sim)-Et(reco) for full acceptance
  hadEtMC->InvestigateFull(kTRUE);

  //Whether or not to look at Et(sim)-Et(reco) for EMCAL acceptance
  hadEtMC->InvestigateEMCAL(kFALSE);

  //Whether or not to look at Et(sim)-Et(reco) for PHOS acceptance
  hadEtMC->InvestigatePHOS(kFALSE);

  //Whether or not to look at Et(sim)-Et(reco) for Pi/K/p in full acceptance (full acceptance must be turned on)
  hadEtMC->InvestigatePiKP(kTRUE);

  //Look at ITS+TPC tracks
  hadEtMC->RequireITSHits(kTRUE);

  //Look at the 2010 p+p data...
  hadEtMC->SetDataSet(2010);
  hadEtMC->SetV0ScaleDataSet(2010);

  //Turn baryon enhancement on and off
  hadEtMC->EnhanceBaryons(kTRUE);

  //Do trigger checks
  hadEtMC->DoTriggerChecks();

  hadEtMC->Init();
  return hadEtMC;
}
