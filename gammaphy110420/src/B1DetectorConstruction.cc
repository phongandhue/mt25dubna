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
//
/// \file B1DetectorConstruction.cc
/// \brief Implementation of the B1DetectorConstruction class

#include "B1DetectorConstruction.hh"

#include "G4RunManager.hh"

#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh"
#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4PSEnergyDeposit.hh"
#include "G4PSDoseDeposit.hh"
#include "G4VisAttributes.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4SubtractionSolid.hh"
#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4UnionSolid.hh"


#define N_CHAMBER 4
#define N_SAMPLE 121*3
#define N_SAMPLE_ONE 121

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::B1DetectorConstruction()
: G4VUserDetectorConstruction(),
  fScoringVolume(0)
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::~B1DetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* B1DetectorConstruction::Construct()
{  

    // Cleanup old geometry
    G4GeometryManager::GetInstance()->OpenGeometry();
    G4PhysicalVolumeStore::GetInstance()->Clean();
    G4LogicalVolumeStore::GetInstance()->Clean();
    G4SolidStore::GetInstance()->Clean();
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();
    
  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  //     
  // World
  //
  G4double world_sizeXY = 200.*cm;
  G4double world_sizeZ  = 200.*cm;
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_Galactic");
  
  G4Box* solidWorld =    
    new G4Box("World",                       //its name
       0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);     //its size
      
  G4LogicalVolume* logicWorld =                         
    new G4LogicalVolume(solidWorld,          //its solid
                        world_mat,           //its material
                        "World");            //its name
                                   
  G4VPhysicalVolume* physWorld = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking


  // Uranium Target
  G4double UTar_sizeR = 1*cm;
  G4double UTar_sizeZ  = 1.*cm;
  G4Material* UTar_mat = nist->FindOrBuildMaterial("G4_Ce");

/*
  G4Isotope* U5 = new G4Isotope("U235", 92, 235, 235.01*g/mole);
  G4Isotope* U8 = new G4Isotope("U238", 92, 238, 238.03*g/mole);
  G4Element* U  = new G4Element("natural Uranium", "U", 2);
  U->AddIsotope(U5,  0.7255 *perCent);
  U->AddIsotope(U8,  99.2745*perCent);
  G4Material* U_noU234 = new G4Material("Uranium", 18.95*g/cm3, 1);
  U_noU234->AddElement(U, 1);
*/

  G4Tubs* solidUTar =
    new G4Tubs("UraniumTarget",                    //its name
        0, UTar_sizeR,UTar_sizeZ/2,0,twopi); //its size

  G4LogicalVolume* logicUTar =
          new G4LogicalVolume(solidUTar,          //its solid
                              UTar_mat,           //its material
                              "UraniumTarget");            //its name

  //G4PVPlacement* physUTar =
  new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(0,0,0),         //at (0,0,0)
                    "UraniumTargetPV",              //its name
                    logicUTar,                //its logical volume
                    physWorld,              //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking


  fScoringVolume = logicUTar;

  //
  // visualization attributes
  //

  G4VisAttributes* Red=
    new G4VisAttributes(G4Colour(1.0,0.0,0.0,0.3));  // Red
    Red->SetVisibility(true);
    Red->SetForceSolid(true);



  G4VisAttributes* Green=
    new G4VisAttributes(G4Colour(0.0,1.0,0.0,0.2));  // Green
    Green->SetVisibility(true);
    Green->SetForceSolid(true);

  G4VisAttributes* Blue=
    new G4VisAttributes(G4Colour(0.0,0.0,1.0,0.1));  // Blue
    Blue->SetVisibility(true);
    Blue->SetForceSolid(true);

  G4VisAttributes* Yellow=
    new G4VisAttributes(G4Colour(1.0,1.0,0.0,0.3));  // Yellow
    Yellow->SetVisibility(true);
    Yellow->SetForceSolid(true);

  G4VisAttributes* Turquoise=
    new G4VisAttributes(G4Colour(0.0,0.8,0.8,0.1));  // Turquoise
    Turquoise->SetVisibility(true);
    Turquoise->SetForceSolid(true);

  G4VisAttributes* Magenta=
    new G4VisAttributes(G4Colour(1.0,0.0,1.0,0.3));  // Magenta
    Magenta->SetVisibility(true);
    Magenta->SetForceSolid(true);

  G4VisAttributes* Grey=
    new G4VisAttributes(G4Colour(0.5,0.5,0.5,0.08));  // Gray
    Grey->SetVisibility(true);
    Grey->SetForceSolid(true);
\
    logicUTar->SetVisAttributes(Red);

  //
  //always return the physical World
  //
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
