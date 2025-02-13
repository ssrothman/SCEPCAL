//===============================
// Author: Wonyong Chung
//         Princeton University
//===============================
#include "DRCrystalHit.h"
#include "SCEPCalSegmentation.h"
#include "DDG4/Geant4SensDetAction.inl"
#include "DDG4/Factories.h"
#include "G4OpticalPhoton.hh"
#include "G4VProcess.hh"

namespace SCEPCal {

  class SegmentedCrystalCalorimeterSD_DRHit {
    public:
      typedef DRCrystalHit Hit;
  };

}

namespace dd4hep {
  namespace sim {
    using namespace SCEPCal;
    
    template <> void Geant4SensitiveAction<SegmentedCrystalCalorimeterSD_DRHit>::defineCollections()    {
      m_collectionID = declareReadoutFilteredCollection<SegmentedCrystalCalorimeterSD_DRHit::Hit>();
    }

    template <> bool 
    Geant4SensitiveAction<SegmentedCrystalCalorimeterSD_DRHit>::process(const G4Step* step,G4TouchableHistory* /*hist*/ ) {
      G4double edep = step->GetTotalEnergyDeposit();
      G4StepPoint *thePrePoint = step->GetPreStepPoint();
      G4StepPoint *thePostPoint = step->GetPostStepPoint();
      G4TouchableHandle  thePreStepTouchable = thePrePoint->GetTouchableHandle();
      G4VPhysicalVolume *thePrePV = thePrePoint->GetPhysicalVolume();
      G4double pretime = thePrePoint->GetGlobalTime();
      G4VPhysicalVolume *thePostPV = thePostPoint->GetPhysicalVolume();
      G4double posttime = thePostPoint->GetGlobalTime();
      G4String thePrePVName = "";
      if (thePrePV) thePrePVName = thePrePV->GetName();
      G4String thePostPVName = "";
      if (thePostPV) thePostPVName = thePostPV->GetName();

      Geant4StepHandler    h(step);
      Geant4HitData::MonteCarloContrib contrib = Geant4HitData::extractContribution(step);
      Geant4HitCollection* coll    = collection(m_collectionID);

      dd4hep::Segmentation* _geoSeg = &m_segmentation;
      auto segmentation=dynamic_cast<dd4hep::DDSegmentation::SCEPCalSegmentation *>(_geoSeg->segmentation());
      auto copyNum64 = segmentation->convertFirst32to64(thePreStepTouchable->GetCopyNumber(0));
      int cellID = (int)copyNum64;

      SegmentedCrystalCalorimeterSD_DRHit::Hit* hit = coll->findByKey<SegmentedCrystalCalorimeterSD_DRHit::Hit>(cellID);
      if(!hit) {    
        DDSegmentation::Vector3D pos = segmentation->myPosition(copyNum64);    
        
        Position global(pos.x(),pos.y(),pos.z());
    
        hit = new SegmentedCrystalCalorimeterSD_DRHit::Hit(global);
        hit->cellID = cellID;
        coll->add(cellID, hit);

      }
      
      G4Track * track =  step->GetTrack();

      if(track->GetDefinition()==G4OpticalPhoton::OpticalPhotonDefinition()) {

        float avgarrival=(pretime+posttime)/2.;

        // count 1st and kill
        // apply scale factor and poisson smearing offline

        int phstep = track->GetCurrentStepNumber();

        if (track->GetCreatorProcess()->G4VProcess::GetProcessName()=="CerenkovPhys") {
          if(phstep==1) {
            // sum all times here, then divide by count when writing to output
            hit->nCerenkovProd+=1;
            hit->tSumC+= avgarrival;
          }
          // track->SetTrackStatus(fStopAndKill);
        } 

        else if (track->GetCreatorProcess()->G4VProcess::GetProcessName()=="ScintillationPhys") {
          if(phstep==1) {
            // sum all times here, then divide by count when writing to output
            hit->nScintillationProd+=1;
            hit->tSumS+= avgarrival;
          }
          // track->SetTrackStatus(fStopAndKill);
        }

      }

      hit->truth.emplace_back(contrib);
      hit->energyDeposit+=edep;

      mark(h.track);

      return true;
    }

  }
}


namespace dd4hep { namespace sim {
    typedef Geant4SensitiveAction<SegmentedCrystalCalorimeterSD_DRHit> SCEPCalSDAction_DRHit;
  }}
DECLARE_GEANT4SENSITIVE(SCEPCalSDAction_DRHit)