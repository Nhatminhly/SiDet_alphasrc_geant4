#include "G4RunManager.hh"
#include "G4UImanager.hh"

#include "DetectorConstruction.hh"
#include "EventAction.hh"
#include "PhysicsList.hh"
#include "PrimaryGeneratorAction.hh"
#include "SteppingAction.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv)
{     
  // Construct the default run manager
  //
  G4RunManager * runManager = new G4RunManager;

  // Set mandatory initialization classes
  //
  DetectorConstruction* det = new DetectorConstruction;
  runManager->SetUserInitialization(det);

  //
  PhysicsList* physics = new PhysicsList;
  runManager->SetUserInitialization(physics);
      
  // Set user action classes
  //
  PrimaryGeneratorAction* gen_action = 
                          new PrimaryGeneratorAction();
  runManager->SetUserAction(gen_action);
  //
  EventAction* evt = new EventAction();  
  runManager->SetUserAction(evt);
  //
  SteppingAction* step = new SteppingAction(det,evt);
  runManager->SetUserAction(step);
  //
  // Initialize G4 kernel
  //
  runManager->Initialize();
  
  // Get the pointer to the User Interface manager
  //
  G4UImanager* UImanager = G4UImanager::GetUIpointer();
  
  if (argc!=1)   // batch mode
    {
      G4String command = "/control/execute ";
      G4String fileName = argv[1];
      UImanager->ApplyCommand(command+fileName);    
    }
  else
    {  // interactive mode : define visualization and UI terminal
#ifdef G4VIS_USE
      G4VisManager* visManager = new G4VisExecutive;
      visManager->Initialize();
      UImanager->ApplyCommand("/control/execute vis.mac");     
#endif

#ifdef G4UI_USE
      G4UIExecutive* ui = new G4UIExecutive(argc, argv);
      ui->SessionStart();
      delete ui;
#endif

#ifdef G4VIS_USE
      delete visManager;
#endif
  }
  // job terminate        
  delete runManager;

  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......