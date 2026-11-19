//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
/// \file SteppingAction.cc
/// \brief Implementation of the B1::SteppingAction class

#include "SteppingAction.hh"

#include "DetectorConstruction.hh"
#include "EventAction.hh"

#include "G4Event.hh"
#include "G4LogicalVolume.hh"
#include "G4AnalysisManager.hh"
#include "G4OpticalPhoton.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "G4Step.hh"

namespace CsI_Tl
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(EventAction* eventAction) : fEventAction(eventAction) {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* step)
{

  const auto secondaries = step->GetSecondaryInCurrentStep();
  
  for (const auto sec : *secondaries){
    if (sec->GetDefinition() ->GetParticleName() == "opticalphoton"){
      auto creatorProcess=sec->GetCreatorProcess()->GetProcessName();
      if (creatorProcess == "Scintillation"){
        fEventAction->AddScintillationPhoton();
    }
  }
}

  if (!scintScoringVolume) {
    const auto detConstruction = static_cast<const DetectorConstruction*>(
      G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    scintScoringVolume = detConstruction->GetScintScoringVolume();
    SiScoringVolume = detConstruction->GetSiScoringVolume();
  }

  // get volume of the current step
  G4LogicalVolume* volume =
    step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();

  // check if we are in any scoring volume
  if (volume != scintScoringVolume && volume != SiScoringVolume) {return;}

  if (volume == scintScoringVolume){
  G4double edepStep = step->GetTotalEnergyDeposit();
  fEventAction->AddEdep(edepStep);
}

if (volume == SiScoringVolume){
  G4Track* track= step->GetTrack();
  const G4ParticleDefinition* particle = track->GetDefinition();

  //checking for interactions with ONLY optical photons within the Si
  if (particle->GetParticleName() == "opticalphoton") {
    G4String processName = step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
    //checking for absorbed optical photons
    if (processName == "OpAbsorption") {
		//getting their KE
      G4double photonEnergy=track->GetKineticEnergy();

      G4int eventID = G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID();
		//filling in the second csv with every photon and their respective energy
		//this allows us to look at a histogram of the energy emission spectra
      auto analysisManager = G4AnalysisManager::Instance();
      analysisManager->FillNtupleIColumn(1,0, eventID);
      analysisManager->FillNtupleIColumn(1,1, track->GetTrackID());
      analysisManager->FillNtupleDColumn(1,2, photonEnergy);
      analysisManager->AddNtupleRow(1);
      //incrementing the absorbed photon counter
      fEventAction->AddAbsorbedPhoton();
      //adding the photon energy to the total photon energy deposition
      fEventAction->AddSiPhotonEdep(photonEnergy);
    }


    }
  }

}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}  // namespace B1