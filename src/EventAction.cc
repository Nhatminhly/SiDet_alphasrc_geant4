#include "EventAction.hh"

#include "G4Event.hh"
#include "G4SystemOfUnits.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction()
{
 printModulo = 1000000; }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event* evt)
{  

  G4int evtNb = evt->GetEventID();
  if (evtNb%printModulo == 0) 
    G4cout << "\n---> Begin of event: " << evtNb << G4endl;
  edep = 0.; 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "fstream"
using namespace std;

void EventAction::EndOfEventAction(const G4Event*)
{ 
  ofstream luufile;
  // Luu file ket qua
  luufile.open ("/home/Documents/Si_alpha/run001_6MV.dat",ios::app);//<path>: duong dan den folder chua file ket qua 'filename'
  //luufile.open ("........./runxxx_xM.txt",ios::app);
  //luufile=fopen("sum_time.txt", "w");
  if (edep>0) luufile << edep <<"\n";

  luufile.close();

  // Xuat ket qua ra man hinh
  G4cout <<"\t" << edep <<"	MeV"<<"\n";
}  
