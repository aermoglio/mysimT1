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
/// \file DetectorConstruction.cc
/// \brief Implementation of the B1::DetectorConstruction class

#include "DetectorConstruction.hh"

#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4Material.hh"
#include "G4Trd.hh"

namespace B1
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();

  // Envelope parameters
  //
  G4double env_sizeXY = 50 * cm, env_sizeZ = 40 * cm;
    
    
  
// making the material (CsI)
  G4Element* Cs = nist->FindOrBuildElement("Cs");
  G4Element* I = nist->FindOrBuildElement("I");
  G4Element* Tl = nist->FindOrBuildElement("Tl");
    
  G4Material* CsI_dop= new G4Material("CsI",4.51 * g/cm3,3);
  CsI_dop-> AddElement(Cs,0.5101);
  CsI_dop-> AddElement(I,0.48815);
  CsI_dop-> AddElement(Tl,0.000173);
    
// making materials property table
  G4MaterialPropertiesTable*CsIMPT = new G4MaterialPropertiesTable();
    
  // adding properties to give scintillation
    
  const G4int nEntries =6;
  
    //defining energy dependent values
    
  G4double CsI_PhE[nEntries]={1.907*eV,1.99*eV,2.254*eV,2.431*eV,2.61*eV,3.1*eV};
    
  G4double Rind_CsI[nEntries]={1.79,1.79,1.79,1.79,1.79,1.79};
    
  G4double AbsL_CsI[nEntries]={40*cm,38.1*cm,34.8*cm,32.3*cm,30.4*cm,24.7*cm};
    
    //distribution of produced optical phonons (relative intensities)
  G4double CsI_SCINT[nEntries]={0.289,0.722,0.977,0.961,0.594,0.225};
    
    //adding values to component table
  CsIMPT->AddProperty("SCINTILLATIONCOMPONENT1", CsI_PhE,CsI_SCINT,nEntries);
  CsIMPT->AddProperty("SCINTILLATIONCOMPONENT2", CsI_PhE,CsI_SCINT,nEntries);
  CsIMPT->AddProperty("RINDEX",CsI_PhE,Rind_CsI,nEntries);
  CsIMPT->AddProperty("ABSLENGTH",CsI_PhE, AbsL_CsI,nEntries);
    
    
  CsIMPT->AddConstProperty("SCINTILLATIONTIMECONSTANT1",730.*ns);
  CsIMPT->AddConstProperty("SCINTILLATIONTIMECONSTANT2",3100.*ns);
  CsIMPT->AddConstProperty("SCINTILLATIONYIELD",54000./MeV);
  
    
  CsIMPT->AddConstProperty("SCINTILLATIONYIELD1",0.637);
  CsIMPT->AddConstProperty("SCINTILLATIONYIELD2",0.361);
  CsIMPT->AddConstProperty("RESOLUTIONSCALE",1.0);

  CsI_dop->SetMaterialPropertiesTable(CsIMPT);
    
    
  G4Material* env_mat = nist->FindOrBuildMaterial("G4_AIR");

  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  //
  // World
  //
  G4double world_sizeXY = 1.2 * env_sizeXY;
  G4double world_sizeZ = 1.2 * env_sizeZ;
    
    
    
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");

  auto solidWorld =
    new G4Box("World",  // its name
              0.5 * world_sizeXY, 0.5 * world_sizeXY, 0.5 * world_sizeZ);  // its size

  auto logicWorld = new G4LogicalVolume(solidWorld,  // its solid
                                        world_mat,  // its material
                                        "World");  // its name

  auto physWorld = new G4PVPlacement(nullptr,  // no rotation
                                     G4ThreeVector(),  // at (0,0,0)
                                     logicWorld,  // its logical volume
                                     "World",  // its name
                                     nullptr,  // its mother  volume
                                     false,  // no boolean operation
                                     0,  // copy number
                                     checkOverlaps);  // overlaps checking

  //
  // Envelope
  //
  auto solidEnv = new G4Box("Envelope",  // its name
                            0.5 * env_sizeXY, 0.5 * env_sizeXY, 0.5 * env_sizeZ);  // its size

  auto logicEnv = new G4LogicalVolume(solidEnv,  // its solid
                                      env_mat,  // its material
                                      "Envelope");  // its name

  new G4PVPlacement(nullptr,  // no rotation
                    G4ThreeVector(),  // at (0,0,0)
                    logicEnv,  // its logical volume
                    "Envelope",  // its name
                    logicWorld,  // its mother  volume
                    false,  // no boolean operation
                    0,  // copy number
                    checkOverlaps);  // overlaps checking

  //
  // Shape 1. Scintillator
  //
  G4Material* shape1_mat = CsI_dop;
  G4ThreeVector pos1 = G4ThreeVector(0, 2 * cm, -8 * cm);

// Box shape
  G4double shape1_dx = 5. * cm;
  G4double shape1_dy = 5. * cm;
  G4double shape1_dz = 0.5 * cm;
  
  auto solidShape1 = new G4Box("Shape1",shape1_dx,shape1_dy,shape1_dz);

  auto logicShape1 = new G4LogicalVolume(solidShape1,  // its solid
                                         shape1_mat,  // its material
                                         "Shape1");  // its name
    
  //fScoringVolume = logicShape1;

  new G4PVPlacement(nullptr,  // no rotation
                    pos1,  // at position
                    logicShape1,  // its logical volume
                    "Shape1",  // its name
                    logicEnv,  // its mother  volume
                    false,  // no boolean operation
                    0,  // copy number
                    checkOverlaps);  // overlaps checking

  //

  // Shape 2. PV
  //
  G4Material* shape2_mat = nist->FindOrBuildMaterial("G4_Si");
  G4ThreeVector pos2 = G4ThreeVector(0, 2 * cm, -7 * cm);

  // PV
  
    G4double shape2_dx=12. * cm;
    G4double shape2_dy=10. * cm;
    G4double shape2_dz=0.5 *cm;
    auto solidShape2 =new G4Box("Shape2",shape2_dx,shape2_dy,shape2_dz); // making the shape
  

  auto logicShape2 = new G4LogicalVolume(solidShape2,  // its solid
                                         shape2_mat,  // its material
                                         "Shape2");  // its name

  new G4PVPlacement(nullptr,  // no rotation
                    pos2,  // at position
                    logicShape2,  // its logical volume
                    "Shape2",  // its name
                    logicEnv,  // its mother  volume
                    false,  // no boolean operation
                    0,  // copy number
                    checkOverlaps);  // overlaps checking

  // Set Shape2 as scoring volume
  //
  fScoringVolume = logicShape2;

  //
  // always return the physical World
 
 
  //
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}  // namespace B1
