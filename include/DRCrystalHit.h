//===============================
// Author: Wonyong Chung
//         Princeton University
//===============================
#ifndef DRCrystalHit_h
#define DRCrystalHit_h 1
#include "DDG4/Geant4Data.h"

namespace SCEPCal {

  typedef ROOT::Math::XYZVector Position;

    class DRCrystalHit : public dd4hep::sim::Geant4HitData {

      public:
        typedef dd4hep::sim::Geant4HitData base_t;

        Position      position;
        Contributions truth;
        double        energyDeposit;
        
        int           nCerenkovProd;
        int           nScintillationProd;
        double        tSumC;
        double        tSumS;
        
      public:
        DRCrystalHit();
        DRCrystalHit(DRCrystalHit&& c) = delete;
        DRCrystalHit(const DRCrystalHit& c) = delete;
        DRCrystalHit(const Position& cell_pos);
        virtual ~DRCrystalHit();
        DRCrystalHit& operator=(DRCrystalHit&& c) = delete;
        DRCrystalHit& operator=(const DRCrystalHit& c) = delete;
    };

};

#endif
