#include "SCEPCalSegmentation.h"
#include "CLHEP/Vector/ThreeVector.h"
#include <DDG4/Geant4SensDetAction.inl>
#include <DDG4/Geant4ParticleInformation.h>
#include <DDG4/Factories.h>

namespace det {
  class SegmentedCrystalCalorimeterSD_G4Hit {
    public:
  };
}

namespace dd4hep {
  namespace sim   {
    using namespace det;
    template <> void Geant4SensitiveAction<SegmentedCrystalCalorimeterSD_G4Hit>::defineCollections()    {
      m_collectionID = declareReadoutFilteredCollection<Geant4Calorimeter::Hit>();
    }
    template <> bool 
    Geant4SensitiveAction<SegmentedCrystalCalorimeterSD_G4Hit>::process(const G4Step* step,G4TouchableHistory* /*hist*/ ) {
        G4double edep = step->GetTotalEnergyDeposit();
        
        if (edep == 0.) return false;
        typedef Geant4Calorimeter::Hit Hit;
        Geant4StepHandler    h(step);
        HitContribution      contrib = Hit::extractContribution(step);
        Geant4HitCollection* coll    = collection(m_collectionID);

        dd4hep::Segmentation* _geoSeg = &m_segmentation;
        auto segmentation=dynamic_cast<dd4hep::DDSegmentation::SCEPCalSegmentation *>(_geoSeg->segmentation());

        G4StepPoint*       aPreStepPoint     = step->GetPreStepPoint();
        G4TouchableHandle  aPreStepTouchable = aPreStepPoint->GetTouchableHandle();
        auto copyNum64 = segmentation->convertFirst32to64(aPreStepTouchable->GetCopyNumber(0));
        int cellID = (int)copyNum64;

        Hit* hit = coll->findByKey<Hit>(cellID);
        if ( !hit ) {
          DDSegmentation::Vector3D pos = segmentation->myPosition(copyNum64);
          Position global(pos.x(),pos.y(),pos.z());
          hit = new Hit(global);
          hit->cellID = cellID;
          coll->add(cellID, hit);
        }
        hit->truth.emplace_back(contrib);
        hit->energyDeposit += edep;

        mark(h.track);
        return true;
    }
  }
}

namespace dd4hep { namespace sim {
    typedef Geant4SensitiveAction<SegmentedCrystalCalorimeterSD_G4Hit> SCEPCalSDAction_G4Hit;
  }}
DECLARE_GEANT4SENSITIVE(SCEPCalSDAction_G4Hit)
