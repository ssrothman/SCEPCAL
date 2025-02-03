//===============================
// Author: Wonyong Chung
//         Princeton University
//===============================
#include <DD4hep/Printout.h>
#include <DD4hep/InstanceCount.h>
#include <DDG4/Geant4Data.h>
#include "DRCrystalHit.h"

SCEPCal::DRCrystalHit::DRCrystalHit()
: Geant4HitData(), position(), truth(), energyDeposit(0), nCerenkovProd(0), nScintillationProd(0), tSumC(0), tSumS(0) {

  dd4hep::InstanceCount::increment(this);

}

SCEPCal::DRCrystalHit::DRCrystalHit(const Position& pos)
: Geant4HitData(), position(pos), truth(), energyDeposit(0), nCerenkovProd(0), nScintillationProd(0), tSumC(0), tSumS(0) {

  dd4hep::InstanceCount::increment(this);

}

SCEPCal::DRCrystalHit::~DRCrystalHit() {
  dd4hep::InstanceCount::decrement(this);
}