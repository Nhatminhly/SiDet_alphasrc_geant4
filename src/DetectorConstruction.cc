#include "DetectorConstruction.hh"
#include "G4SystemOfUnits.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

DetectorConstruction::DetectorConstruction()
:constructed(false)
{;}

DetectorConstruction::~DetectorConstruction()
{;}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  if(!constructed)
  { 
    constructed = true;
    DefineMaterials();
    SetupGeometry();
  }
  return worldPhys;
}

void DetectorConstruction::DefineMaterials()
{ 
  G4String name, symbol;             //a=mass of a mole;
  G4double a, z, density;            //z=mean number of protons;  

  G4int ncomponents, natoms;
  G4double fractionmass;

//~~~~~~~~~~~~ Elements Definition -Dinh nghia nguyen to ~~~~~~//
  // N: Nitrogen
  a = 14.01*g/mole;
  G4Element* N  = new G4Element(name="Nitrogen",symbol="N" , z= 7., a);
  // O: Oxygen
  a = 16.00*g/mole;
  G4Element* O  = new G4Element(name="Oxygen"  ,symbol="O" , z= 8., a);
  // Na: Sodium
  //a =  22.99*g/mole;
  //G4Element* Na = new G4Element(name="Sodium", symbol="Na", z=11., a);
  // I: Iodine
  //a = 126.9*g/mole;
  //G4Element* I = new G4Element(name="Iodine", symbol="I", z=53., a); 
  // Tl: Thallium
  //a = 204.383*g/mole;
  //G4Element* Tl = new G4Element(name="Thallium", symbol="Tl", z=81., a);
  // Si: Silicon
  //a = 28.085*g/mole;
  //G4Element* Si = new G4Elenment(name="Silicon", symbol="Si", z=14., a);
  
//~~~~~~~~~~ Materials Definition - Dinh nghia vat lieu ~~~~~//

  // Air
  density = 1.290*mg/cm3;
  air = new G4Material(name="Air"  , density, ncomponents=2);
  air->AddElement(N, fractionmass=0.7);
  air->AddElement(O, fractionmass=0.3);
  //Vacuum
  //a = 1.;
  //density = 1.e-25*g/cm3;
  //Vacuum = new G4Material("Vacuum", z=1.,a,density);
               
  //NaI
  //density = 3.67*g/cm3;
  //NaI = new G4Material("NaI", density, ncomponents=2);
  //NaI->AddElement(Na, natoms=1);
  //NaI->AddElement(I, natoms=1);
  //NaI_Tl  (Tl 0.4%)
  //density=3.67*g/cm3;
  //NaITl = new G4Material("NaI(Tl)", density, ncomponents=2);
  //NaITl->AddMaterial(NaI, fractionmass=0.996);
  //NaITl->AddElement(Tl, fractionmass=0.004);
  //Pb
  a=207*g/mole; density=8.9*g/cm3;
  Pb = new G4Material("Lead", z=82.,a, density);
  //Si
  a = 28.085*g/mole; density=2.3290*g/cm3;
  Si = new G4Material("Si", z=14.,a, density);
}
void DetectorConstruction::SetupGeometry()
{

//~~~~~~~~~~~~~~~~~~~~~~~~ World Definition ~~~~~~~~~~~~~~~~~~~~~~~~//
  G4VSolid* worldSolid = new G4Box("World",40.*cm,40.*cm,40.*cm);

  G4LogicalVolume* worldLogical = new G4LogicalVolume(worldSolid,air,"World");

  worldPhys = new G4PVPlacement(0,G4ThreeVector(),worldLogical,"World",
                        0,false,0);              



//~~~~~~~~~~~ Si Definition -Hinh tru 2cm*0.1cm~~~~~~~~~//

  G4double inradius_Si_det = 0.*cm; G4double outradius_Si_det = 2*cm;
  G4double hight_Si_det = 0.5*cm;

  G4Tubs* Sitube 
    = new G4Tubs("Si detector",inradius_Si_det,
                 outradius_Si_det,hight_Si_det,0,CLHEP::twopi);
  G4LogicalVolume* Silog 
    = new G4LogicalVolume(Sitube,Si,"Si Logical");
  //
  Siphys = new G4PVPlacement(0,
             G4ThreeVector(0,0,-0.25),
             Silog,"Si Physical",worldLogical,false,0);
  //~~~~~~~~~~~~~~~~~~~xay dung khoichi~~~~~~~~~~~~~~~~~~~//
  G4double inradius_lead_shield = 0.*cm; G4double outradius_lead_shield = 10*cm;
  G4double hight_lead_shield = 1*cm;

  G4Tubs* leadtubs=new G4Tubs("Lead",inradius_lead_shield,
                 outradius_lead_shield,hight_lead_shield,0,CLHEP::twopi);
  G4LogicalVolume* leadlog
	= new G4LogicalVolume(leadtubs,Pb,"leadshield Logical");
  //
  leadphys = new G4PVPlacement(0, G4ThreeVector(0,0,7.5*cm),
		leadlog,"leadshied Physical", worldLogical,false,0);                                        
  // Visualization attributes - Dinh nghia mau (red,green,blue)
  //
  G4Colour red (1.0,0.0,0.0); // red
  G4Colour green (0.0,1.0,0.0); // green
  G4Colour yellow (1.0,1.0,0.0); // yellow
  G4Colour blue (0.0,0.0,1.0); // blue
  G4Colour magenta (1.0,0.0,1.0) ;  // magenta 
  G4Colour cyan (0.0,1.0,1.0) ;  // cyan

  // Make the World volume visible or invisible
  //worldLogical->SetVisAttributes(G4VisAttributes::Invisible);

  // Set colour for Si detector:
  Silog->SetVisAttributes(G4VisAttributes(G4Colour(yellow)));
}
