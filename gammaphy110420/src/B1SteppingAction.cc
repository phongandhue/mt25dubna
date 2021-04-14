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
/// \file B1SteppingAction.cc
/// \brief Implementation of the B1SteppingAction class

#include "B1SteppingAction.hh"
#include "B1EventAction.hh"
#include "B1DetectorConstruction.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include "G4SystemOfUnits.hh"

#include "G4HadronicProcess.hh"
#include "g4root.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1SteppingAction::B1SteppingAction(B1EventAction* eventAction)
: G4UserSteppingAction(),
  fEventAction(eventAction),
  fScoringVolume(0)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1SteppingAction::~B1SteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1SteppingAction::UserSteppingAction(const G4Step* step)
{
  if (!fScoringVolume) { 
    const B1DetectorConstruction* detectorConstruction
      = static_cast<const B1DetectorConstruction*>
        (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    fScoringVolume = detectorConstruction->GetScoringVolume();   
  }

  // get volume of the current step
  G4LogicalVolume* volume 
    = step->GetPreStepPoint()->GetTouchableHandle()
      ->GetVolume()->GetLogicalVolume();//prestep on Primary target

  G4String volumeName= volume->GetName();

  // get reactions channels
  const G4StepPoint* endPoint = step->GetPostStepPoint();
  G4VProcess* process   =
                   const_cast<G4VProcess*>(endPoint->GetProcessDefinedStep());

  // check that an real interaction occured (eg. not a transportation)
  G4StepStatus stepStatus = endPoint->GetStepStatus();
  G4bool transmit = (stepStatus==fGeomBoundary || stepStatus==fWorldBoundary);
  if (transmit) return;

  //initialisation of the nuclear channel identification
  //
  G4ParticleDefinition* particle = step->GetTrack()->GetDefinition();
  G4String partName = particle->GetParticleName();


  G4String nuclearChannel = process->GetProcessName()+" - ";
  nuclearChannel += partName;

  if (partName!="gamma")
      return;

  G4HadronicProcess* hproc = dynamic_cast<G4HadronicProcess*>(process);

  const G4Isotope* target = NULL;
  if (hproc) {
      target = hproc->GetTargetIsotope();
  }else{
      return;
  }

  G4String targetName = "XXXX";
  if (target) targetName = target->GetName();
  nuclearChannel += " + " + targetName + " --> ";

  //scattered primary particle (if any)
  //
  G4int ih = 1;
  if (step->GetTrack()->GetTrackStatus() == fAlive) {
    nuclearChannel += partName + " + ";
  }

  //secondaries
  //
  const std::vector<const G4Track*>* secondary
                                    = step->GetSecondaryInCurrentStep();

  G4cout<<"nuclear channel = "<<nuclearChannel;
  for (size_t lp=0; lp<(*secondary).size(); lp++) {
    particle = (*secondary)[lp]->GetDefinition();
    G4String name   = particle->GetParticleName();
    G4String type   = particle->GetParticleType();
    G4double energy = (*secondary)[lp]->GetKineticEnergy();
    //energy spectrum
    ih = 0;
         if (particle == G4Gamma::Gamma())       ih = 2;
    else if (particle == G4Neutron::Neutron())   ih = 3;
    else if (particle == G4Proton::Proton())     ih = 4;
    else if (particle == G4Deuteron::Deuteron()) ih = 5;
    else if (particle == G4Alpha::Alpha())       ih = 6;
    else if (type == "nucleus")                  ih = 7;
    else if (type == "meson")                    ih = 8;
    else if (type == "baryon")                   ih = 9;
    //atomic mass
    if (type == "nucleus") {
      G4int A = particle->GetAtomicMass();
    }
    if (particle == G4Electron::Electron()) particle = G4Gamma::Gamma();
    //particle flag
    fParticleFlag[particle]++;
    G4cout<<name<<" + ";
  }
  G4cout<<G4endl;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

