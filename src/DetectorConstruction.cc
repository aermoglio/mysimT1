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
    
    
  
// MAKING MATERIALS
    
  const G4int nEntries =6;

//CSI(TL)
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

// ......................................

  //MAKING A-SILICON
    
  G4Material* aSi = nist->FindOrBuildMaterial("G4_Si");

  //adding optical properties to AMORPHOUS silicon
  G4MaterialPropertiesTable* aSiMPT= new G4MaterialPropertiesTable();
  
  //will use same wavelengths as those emitted by CsI, but looking up absorption lengths for each.
  G4double aSi_PhE[nEntries]={1.907*eV,1.99*eV,2.254*eV,2.431*eV,2.61*eV,3.1*eV};
  
  G4double Rind_aSi[nEntries]={4.523,4.6171,4.8521,4.9897,5.1028,5.1852};
  G4double AbsL_aSi[nEntries]={0.276*um,0.19*um,0.0783*um,0.0502*um,0.0344*um,0.0150*um};

  aSiMPT->AddProperty("RINDEX",aSi_PhE,Rind_aSi,nEntries);
  aSiMPT->AddProperty("ABSLENGTH",aSi_PhE, AbsL_aSi,nEntries);  
  aSi->SetMaterialPropertiesTable(aSiMPT);

// ......................................

  //MAKING GLASS
  G4Element* Si = nist->FindOrBuildElement("G4_Si");
  G4Element* O = nist->FindOrBuildElement("G4_O");

  G4Material* glass = new G4Material("Glass", 2.2 * g/cm3,2)

  glass-> AddElement(Si,natoms=1);
  glass-> AddElement(O,natoms=2);

  G4MaterialPropertiesTable*glassMPT = new G4MaterialPropertiesTable();
  
  G4double glass_PhE[nEntries]={1.907*eV,1.99*eV,2.254*eV,2.431*eV,2.61*eV,3.1*eV};
  G4double Rind_glass[nEntries]={1.456,1.4574,1.4599,1.4618,1.4638,1.4701};
  G4double AbsL_glass[nEntries]={10000000000.*cm,10000000000.*cm,10000000000.*cm,10000000000.*cm,10000000000.*cm,10000000000.*cm};

  glassMPT->AddProperty("RINDEX",glass_PhE,Rind_glass,nEntries);
  glassMPT->AddProperty("ABSLENGTH",glass_PhE, AbsL_glass,nEntries);  

  glass->SetMaterialPropertiesTable(glassMPT);

  //................................

  //MAKING IPO
  G4Element* In = nist->FindOrBuildElement("G4_In");
  G4Element* Sn = nist->FindOrBuildElement("G4_Sn");

  G4Material* In2O3 = new G4Material("In2O3", 7.18 * g/cm3,2)
  In203-> AddElement(In,natoms=2);
  In203-> AddElement(O,natoms=3);

  G4Material* SnO2 = new G4Material("SnO2", 6.95 * g/cm3,2)
  SnO2-> AddElement(Sn,natoms=1);
  SnO2-> AddElement(O,natoms=2);

  G4Material* ITO = new G4Material("ITO", 7.14 * g/cm3,3)
  ITO-> AddMaterial(In2O3,0.9);
  ITO-> AddMaterial(SnO2,0.1);

  G4MaterialPropertiesTable*ITO_MPT = new G4MaterialPropertiesTable();
  G4double ITO_PhE[nEntries]={1.907*eV,1.99*eV,2.254*eV,2.431*eV,2.61*eV,3.1*eV};
  G4double Rind_ITO[nEntries]={1.7653,1.7952,1.8636,1.9034,1.9401,2.0355};
  G4double AbsL_ITO[nEntries]={15.34*um, 15.51*um, 13.56*um,10.82*um,7.97*um, 2.93*um};
  
  ITO_MPT->AddProperty("RINDEX",ITO_PhE,Rind_ITO,nEntries);
  ITO_MPT->AddProperty("ABSLENGTH",ITO_PhE, AbsL_ITO,nEntries);  

  ITO->SetMaterialPropertiesTable(ITO_MPT);

  // .........................................................
  // ALUMINIUM
  G4Material* Al = nist->FindOrBuildMaterial("G4_Al"); 

  // .........................................................

  // WORLD

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
  // CSI SCINTILLATOR
  //
  G4Material* Scint_mat = CsI_dop;
  G4ThreeVector Scint_pos = G4ThreeVector(0, 2 * cm, -8 * cm);

// Box shape
  G4double Scint_dx = 5. * cm;
  G4double Scint_dy = 5. * cm;
  G4double Scint_dz = 0.5 * cm;
  
  auto Scint = new G4Box("CsI(Tl)",Scint_dx,Scint_dy,Scint_dz);

  auto logic_Scint = new G4LogicalVolume(Scint,  // its solid
                                         Scint_mat,  // its material
                                         "CsI(Tl)");  // its name
    
  //fScoringVolume = logicShape1;

  new G4PVPlacement(nullptr,  // no rotation
                    Scint_pos,  // at position
                    logic_Scint,  // its logical volume
                    "CsI(Tl)",  // its name
                    logicEnv,  // its mother  volume
                    false,  // no boolean operation
                    0,  // copy number
                    checkOverlaps);  // overlaps checking

  //  //
 
// SETTING UP SILICON BLOCK
  G4ThreeVector Si_pos = G4ThreeVector(0, 2 * cm, -7 * cm);
  
  G4double aSi_dx=12. * cm;
  G4double aSi_dy=10. * cm;
  G4double aSi_dz=0.5 *cm;
  auto aSilicon =new G4Box("Silicon",aSi_dx,aSi_dy,aSi_dz); // making the shape
  

  auto logic_aSi = new G4LogicalVolume(aSilicon,  // its solid
                                         aSi,  // its material
                                         "Silicon");  // its name

  new G4PVPlacement(nullptr,  // no rotation
                    Si_pos,  // at position
                    logic_aSi,  // its logical volume
                    "Silicon",  // its name
                    logicEnv,  // its mother  volume
                    false,  // no boolean operation
                    0,  // copy number
                    checkOverlaps);  // overlaps checking

  // Set SILICON as scoring volume
  //
  fScoringVolume = logic_aSi;

  //
  // always return the physical World
 
 
  //
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}  // namespace B1
