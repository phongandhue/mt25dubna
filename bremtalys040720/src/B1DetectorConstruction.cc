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
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Tubs.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4VisAttributes.hh"
#include "G4RotationMatrix.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4IntersectionSolid.hh"


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
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();
    
  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  //     
  // World
  //
  G4double world_sizeXY = 40.*cm;
  G4double world_sizeZ  = 80.*cm;
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
                     
  //     
  // Bia so cap
  //  
  G4double primTar_sizeR = 20.*mm;
  G4double primTar_sizeZ  = 25.*mm;
  G4Tubs* primTar =
    new G4Tubs("PrimaryTarget",                    //its name
        0, primTar_sizeR,primTar_sizeZ/2,0,twopi); //its size
//  G4Box* primTar =
//    new G4Box("PrimaryTarget",                    //its name
//        0.5*primTar_sizeXY, 0.5*primTar_sizeXY, 0.5*primTar_sizeZ); //its size
  G4Material* primTar_mat = nist->FindOrBuildMaterial("G4_W");

  G4LogicalVolume* logicPrimTar =
    new G4LogicalVolume(primTar,            //its solid
                        primTar_mat,             //its material
                        "PrimaryTarget");         //its name

  G4double primTarZpos=primTar_sizeZ/2*mm;
  new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(0,0,primTarZpos),         //at (0,0,0)
                    logicPrimTar,                //its logical volume
                    "PrimaryTarget",              //its name
                    logicWorld,              //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking

 // Nuoc
  //
  G4double nuoc_sizeR = 120.*mm;
  G4double nuoc_sizeZ  = 280.*mm;
//  G4Box* nuoc =
//    new G4Box("Nuoc",                    //its name
//        0.5*nuoc_sizeXY, 0.5*nuoc_sizeXY, 0.5*nuoc_sizeZ); //its size

  G4Tubs* nuoc =
    new G4Tubs("Nuoc",                    //its name
        0, nuoc_sizeR,nuoc_sizeZ/2,0,twopi); //its size

  G4Material* nuoc_mat = nist->FindOrBuildMaterial("G4_WATER");

  G4double beampipe_sizeR = 98./2.*mm; G4double beampipe_sizeZ = nuoc_sizeR;
  G4Tubs* beampipe =
    new G4Tubs("beampipe",                    //its name
        0, beampipe_sizeR,beampipe_sizeZ/2,0,twopi); //its size

  G4RotationMatrix* rotmat_beampipe=new G4RotationMatrix;
  rotmat_beampipe->set(0,0,0);
  rotmat_beampipe->rotateX(90.*deg);
  G4SubtractionSolid* nuoc_sub= new G4SubtractionSolid("nuoc-beampipe",nuoc,beampipe,rotmat_beampipe,G4ThreeVector(0,-nuoc_sizeR/2,0));

  G4LogicalVolume* logicNuoc =
    new G4LogicalVolume(nuoc_sub,            //its solid
                        nuoc_mat,             //its material
                        "Nuoc");         //its name


  G4RotationMatrix* rotmat_nuoc=new G4RotationMatrix;
  rotmat_nuoc->set(0,0,0);
  rotmat_nuoc->rotateX(90.*deg);
  new G4PVPlacement(rotmat_nuoc,                       //no rotation
                    G4ThreeVector(0,0,0),         //at (0,0,0)
                    "NuocPHY",              //its name
                    logicNuoc,                //its logical volume
                    physWorld,              //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking

  //
  // bia thu cap
  //
  // Eu2O3
  G4Isotope *IO=new G4Isotope("Oxygen",8,16,16.00*g/mole);
  G4Element *elO=new G4Element("Oxygen","O",1);
  elO->AddIsotope(IO,100.*perCent);

  G4Isotope *IEu151=new G4Isotope("Eu151",63,151,151.00*g/mole);
  G4Isotope *IEu153=new G4Isotope("Eu153",63,153,153.00*g/mole);
  G4Element *elEu=new G4Element("Eu","Eu",1);
  //elEu->AddIsotope(IEu151,47.8*perCent);
  //elEu->AddIsotope(IEu153,52.2*perCent);
  elEu->AddIsotope(IEu153,100*perCent);
  G4double EuDen=7.40*g/cm3;
  //G4Material *Eumat=new G4Material("Eu",EuDen,2);
  //Eumat->AddElement(elEu,2);
  //Eumat->AddElement(elO,3);
  G4Material *Eu153=new G4Material("Eu",EuDen,1);
  Eu153->AddElement(elEu,1);
  //G4Material* tar_mat=Eumat;

  //! Ag
  G4Material *AuNist = nist->FindOrBuildMaterial("G4_Au");
  G4double secTar_sizeR = 10.*mm;
  G4double secTar_sizeZ  = 2*mm;
  //G4double distanceSecTar=3.*cm;
  G4double secTarZpos=15.*mm;

  G4double secTar_posX[]={26.14*mm, 49.59*mm, 16.04*mm, -24.64*mm, -48.99*mm, -34.45*mm, 0.2875*mm, 50.78*mm};
  G4double secTar_posZ[]={44.4*mm, 15.61*mm, 12.25*mm, 45.94*mm, 17.16*mm, -39.2*mm, -51.45*mm, -13.79*mm};

  G4int N_SecTar=8;
  G4Tubs* secTar[N_SecTar];
  G4LogicalVolume* logicSecTar[N_SecTar];
  for (G4int i=0;i<N_SecTar;i++){
      char tmp[50];
      sprintf(tmp,"_%d",i);
      G4String tmpid=(G4String) tmp;

      secTar[i] = new G4Tubs("SecondaryTargetSolid"+tmpid,                    //its name
                 0, secTar_sizeR,secTar_sizeZ/2,0,twopi); //its size
      logicSecTar[i] =
        new G4LogicalVolume(secTar[i],            //its solid
                            AuNist,             //its material
                            "SecondaryTarget"+tmpid);        //its name
      G4RotationMatrix* rotmat_SecTar=new G4RotationMatrix;
      rotmat_SecTar->set(0,0,0);
      rotmat_SecTar->rotateX(90.*deg);

      new G4PVPlacement(rotmat_SecTar,                       //no rotation
                        G4ThreeVector(secTar_posX[i],secTarZpos,secTar_posZ[i]),         //at (0,0,0)
                        logicSecTar[i],                //its logical volume
                        "SecondaryTargetP"+tmpid,              //its name
                        logicNuoc,              //its mother  volume
                        false,                   //no boolean operation
                        0,                       //copy number
                        checkOverlaps);          //overlaps checking
  }




  // Set Shape2 as scoring volume
  //
  fScoringVolume = logicSecTar[0];

  // Visualization attributes
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
    //Blue->SetForceWireframe(true);

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

  logicPrimTar->SetVisAttributes (Magenta);
  //G4VisAttributes* vislogicNhom=new G4VisAttributes(1,G4Color(1,0,1,0.6));
  //vislogicNhom->SetForceSolid();
  //logicNhom->SetVisAttributes (vislogicNhom);

  logicNuoc->SetVisAttributes (Blue);

  for (G4int i=0;i<N_SecTar;i++) logicSecTar[i]->SetVisAttributes (Yellow);

  //
  //always return the physical World
  //
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
