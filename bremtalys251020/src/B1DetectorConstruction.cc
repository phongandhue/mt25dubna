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


  // Graphite Moderator

  G4double GM_sizeXY = 120.*cm;
  G4double GM_sizeZ  = 120.*cm;
  G4Material* GM_mat = nist->FindOrBuildMaterial("G4_GRAPHITE");

  G4Box* solidGM =
    new G4Box("GraphiteModerator",                       //its name
       0.5*GM_sizeXY, 0.5*GM_sizeXY, 0.5*GM_sizeZ);     //its size

  G4LogicalVolume* logicGM =
    new G4LogicalVolume(solidGM,          //its solid
                        GM_mat,           //its material
                        "GraphiteModerator");            //its name
  G4VPhysicalVolume* physGM=new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(0,0,0),         //at (0,0,0)
                    "GraphiteModeratorPV",              //its name
                    logicGM,                //its logical volume
                    physWorld,              //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking


  // Beam Pipe
  G4double BP_sizeR = 3.*cm;
  //G4double BP_sizeZ  = 50.*cm;
  G4double BP_sizeZ  = 60*cm-0.5*cm;
  G4Material* BP_mat = nist->FindOrBuildMaterial("G4_Galactic");

  G4Tubs* solidBP =
    new G4Tubs("BeamPipe",                    //its name
        0, BP_sizeR,BP_sizeZ/2,0,twopi); //its size

  G4LogicalVolume* logicBP =
          new G4LogicalVolume(solidBP,          //its solid
                              BP_mat,           //its material
                              "BeriliumChamber");            //its name

  G4double BP_posZ = GM_sizeZ/2 - BP_sizeZ/2;
  new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(0,0,BP_posZ),         //at (0,0,0)
                    "BeriliumChamberPV",              //its name
                    logicBP,                //its logical volume
                    physGM,              //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking

  // Berilium Chamber
  G4double BeC_sizeY = 20.*cm;
  G4double BeC_sizeXZ  = 50.*cm;
  G4Material* BeC_mat = nist->FindOrBuildMaterial("G4_Be");

  G4Box* solidBeC =
          new G4Box("BeriliumChamber",                       //its name
                    0.5*BeC_sizeXZ, 0.5*BeC_sizeY, 0.5*BeC_sizeXZ);     //its size
  G4Tubs* solidBP_4sub =
    new G4Tubs("BeamPipe_4sub",                    //its name
        0, BP_sizeR,BP_sizeZ/2,0,twopi); //its size
  G4VSolid* solidBeC_sub  = new G4SubtractionSolid("BeriliumChamber-BeamPipe", solidBeC, solidBP_4sub, 0, G4ThreeVector(0.,0.,BP_posZ));

  G4LogicalVolume* logicBeC =
          new G4LogicalVolume(solidBeC_sub,          //its solid
                              BeC_mat,           //its material
                              "BeriliumChamber");            //its name
  G4PVPlacement* physBeC = new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(0,0,0),         //at (0,0,0)
                    "BeriliumChamberPV",              //its name
                    logicBeC,                //its logical volume
                    physGM,              //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking

  // Uranium Target
  G4double UTar_sizeR = 1*cm;
  G4double UTar_sizeZ  = 1.*cm;
  G4Material* UTar_mat = nist->FindOrBuildMaterial("G4_U");

  G4Tubs* solidUTar =
    new G4Tubs("BeamPipe",                    //its name
        0, UTar_sizeR,UTar_sizeZ/2,0,twopi); //its size

  G4LogicalVolume* logicUTar =
          new G4LogicalVolume(solidUTar,          //its solid
                              UTar_mat,           //its material
                              "BeriliumChamber");            //its name

  //G4PVPlacement* physUTar =
  new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(0,0,0),         //at (0,0,0)
                    "BeriliumChamberPV",              //its name
                    logicUTar,                //its logical volume
                    physBeC,              //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking


  // Sample Chambers
  G4double SC_sizeY = 2.*cm;
  G4double SC_sizeXZ  = 50.*cm;
  G4Material* SC_mat = nist->FindOrBuildMaterial("G4_AIR");

  G4Box* solidSC[N_CHAMBER];
  G4LogicalVolume* logicSC[N_CHAMBER];
  G4VPhysicalVolume* physSC[N_CHAMBER];
  G4double SC_posY[]={-30*cm,-15*cm,15*cm,30*cm};

  G4double Sample_R_out = 25.*cm;
  G4double Sample_R_in = 24.*cm;
  G4double Sample_Z = 1.5*cm;
  G4Tubs* solidSample[N_CHAMBER];
  G4LogicalVolume* logicSample[N_CHAMBER];
  G4double Sample_posY[]={-30*cm,-15*cm,15*cm,30*cm};
  G4int Sample_placement[]={1,0,0,0};
  G4Material* Sample_mat = nist->FindOrBuildMaterial("G4_Cd");


  for (G4int i=0;i<N_CHAMBER;i++){
      char tmp[50];
      sprintf(tmp,"_%d",i);
      G4String tmpid=(G4String) tmp;


      solidSC[i] =
              new G4Box("SampleChamber"+tmpid,                       //its name
                        0.5*SC_sizeXZ, 0.5*SC_sizeY, 0.5*SC_sizeXZ);     //its size
      logicSC[i] =
              new G4LogicalVolume(solidSC[i],          //its solid
                                  SC_mat,           //its material
                                  "SampleChamber"+tmpid);            //its name
      physSC[i] = new G4PVPlacement(0,                       //no rotation
                        G4ThreeVector(0,SC_posY[i],0),         //at (0,0,0)
                        "SampleChamberPV"+tmpid,              //its name
                        logicSC[i],                //its logical volume
                        physGM,              //its mother  volume
                        false,                   //no boolean operation
                        0,                       //copy number
                        checkOverlaps);          //overlaps checking

      solidSample[i] =
              new G4Tubs("SampleSolid"+tmpid,                       //its name
                         Sample_R_in, Sample_R_out,Sample_Z/2,0,twopi);     //its size
      logicSample[i] =
              new G4LogicalVolume(solidSample[i],          //its solid
                                  Sample_mat,           //its material
                                  "Sample"+tmpid);            //its name
      G4RotationMatrix* rotM = new G4RotationMatrix;
      rotM->rotateX(90.*deg);
      if (Sample_placement[i]==1)
      new G4PVPlacement(rotM,                       //no rotation
                        G4ThreeVector(0,0,0),         //at (0,0,0)
                        "SamplePV"+tmpid,              //its name
                        logicSample[i],                //its logical volume
                        physSC[i],              //its mother  volume
                        false,                   //no boolean operation
                        0,                       //copy number
                        checkOverlaps);          //overlaps checking
  }

  //

  // Set Shape2 as scoring volume
  //
  fScoringVolume = logicSC[0];

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

    //logicWorld->SetVisAttributes(Grey);
    logicGM->SetVisAttributes(Grey);
    logicBeC->SetVisAttributes(Green);
    logicBP->SetVisAttributes(Turquoise);
    logicUTar->SetVisAttributes(Red);
    for (G4int i=0;i<N_CHAMBER;i++){
         logicSC[i]->SetVisAttributes(Blue);
         logicSample[i]->SetVisAttributes(Yellow);
    }

  //
  //always return the physical World
  //
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
