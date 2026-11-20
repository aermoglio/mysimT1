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
/// \file RunAction.hh
/// \brief Definition of the B1::RunAction class

#ifndef B1RunAction_h
#define B1RunAction_h 1

#include "G4UserRunAction.hh"

#include "G4Accumulable.hh"
#include "globals.hh"

class G4Run;

namespace CsI_Tl
{

/// Run action class
///
/// In EndOfRunAction(), it calculates the dose in the selected volume
/// from the energy deposit accumulated via stepping and event actions.
/// The computed dose is then printed on the screen.

class RunAction : public G4UserRunAction
{
  public:
    RunAction();
    ~RunAction() override = default;

    void BeginOfRunAction(const G4Run*) override;
    void EndOfRunAction(const G4Run*) override;

    void AddEdep(G4double edep);

    void AddEdepSiliconTotal(G4double edepSiTot);
    void AddEdepGlassTotal(G4double edepGlassTot);
    void AddEdepITOTotal(G4double edepITOTot);

    void AddScintillationPhoton(G4double photons);
    void AddSiPhotonEdep(G4double SiPhotonEdep);
    void AddAbsorbedPhoton(G4double abphotons);

  private:
    G4Accumulable<G4double> fEdep = 0.;
    G4Accumulable<G4double> fEdep2 = 0.;

    G4Accumulable<G4double> fEdepSiTot = 0.;
    G4Accumulable<G4double> fEdepSiTot2 = 0.;

    G4Accumulable<G4double> fEdepGlassTot = 0.;
    G4Accumulable<G4double> fEdepGlassTot2 = 0.;

    G4Accumulable<G4double> fEdepITOTot = 0.;
    G4Accumulable<G4double> fEdepITOTot2 = 0.;

    G4Accumulable<G4int> fScintillationPhotons = 0.;

    G4Accumulable<G4double> fSiPhotonEdep = 0.;
    G4Accumulable<G4double> fSiPhotonEdep2 = 0.;

    G4Accumulable<G4int> fAbsorbedPhotons = 0.;
};

}  // namespace B1

#endif
