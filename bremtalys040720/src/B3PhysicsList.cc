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
/// \file B3PhysicsList.cc
/// \brief Implementation of the B3PhysicsList class

#include "B3PhysicsList.hh"

// Physics lists
#include "G4EmStandardPhysics_option4.hh"
#include "G4EmStandardPhysics_option3.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4EmPenelopePhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4HadronElasticPhysics.hh"
#include "G4HadronInelasticQBBC.hh"
#include "G4IonBinaryCascadePhysics.hh"
#include "G4Decay.hh"
#include "G4RadioactiveDecayPhysics.hh"

#include "G4HadronPhysicsFTFP_BERT_HP.hh"
#include "G4HadronPhysicsQGSP_BIC_HP.hh"
#include "G4HadronPhysicsINCLXX.hh"
#include "G4IonElasticPhysics.hh"
#include "G4IonPhysicsXS.hh"
#include "G4IonINCLXXPhysics.hh"
#include "G4StoppingPhysics.hh"

#include "GammaNuclearPhysics.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B3PhysicsList::B3PhysicsList() 
: G4VModularPhysicsList(){
  SetVerboseLevel(1);

  // Decay physics
  RegisterPhysics(new G4DecayPhysics());

  // Radioactive decay
  RegisterPhysics(new G4RadioactiveDecayPhysics());

  // EM physics
  //RegisterPhysics(new G4EmStandardPhysics());
  RegisterPhysics(new G4EmLivermorePhysics());

  // Ion Elastic scattering
  RegisterPhysics( new G4IonElasticPhysics());

  // Ion Inelastic Physics
  RegisterPhysics( new G4IonPhysicsXS());
  ////RegisterPhysics( new G4IonINCLXXPhysics());

  // stopping Particles
  ////RegisterPhysics( new G4StoppingPhysics(verb));


  // Hadron models
  RegisterPhysics(new G4HadronElasticPhysics());

  //RegisterPhysics( new G4HadronPhysicsFTFP_BERT_HP());
  RegisterPhysics( new G4HadronPhysicsQGSP_BIC_HP());
  ////RegisterPhysics( new G4HadronInelasticQBBC());
  ////RegisterPhysics( new G4HadronPhysicsINCLXX());

  RegisterPhysics(new G4IonBinaryCascadePhysics());

  // Gamma-Nuclear Physics
  RegisterPhysics( new GammaNuclearPhysics("gamma"));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B3PhysicsList::~B3PhysicsList()
{ 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B3PhysicsList::SetCuts()
{
  G4VUserPhysicsList::SetCuts();
}
