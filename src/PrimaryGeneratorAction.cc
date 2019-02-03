#include "PrimaryGeneratorAction.hh"
#include "G4SystemOfUnits.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "Randomize.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction()
{
  G4int n_particle = 1;
  particleGun  = new G4ParticleGun(n_particle);
  
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  G4ParticleDefinition* particle
                    = particleTable->FindParticle(particleName="alpha");	// name of particle
  particleGun->SetParticleDefinition(particle);

}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete particleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  G4double px,py,pz;
  px=G4UniformRand()*2-1;//random -1 den 1
  py=G4UniformRand()*2-1;
  pz=G4UniformRand()*2-1;
  particleGun->SetParticleMomentumDirection(G4ThreeVector(px,py,pz));
// position  of the particle gun (source)
  particleGun->SetParticlePosition(G4ThreeVector(0.,0.,2*cm));	
// energy    of the particle gun (source)
  particleGun->SetParticleEnergy(6.*MeV);				

  particleGun->GeneratePrimaryVertex(anEvent);
}
