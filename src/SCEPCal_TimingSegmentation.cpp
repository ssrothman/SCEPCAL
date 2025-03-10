//===============================
// Author: Wonyong Chung
//         Princeton University
//===============================
#include "SCEPCal_TimingSegmentation.h"
#include <climits>
#include <cmath>
#include <stdexcept>

namespace dd4hep {
namespace DDSegmentation {

SCEPCal_TimingSegmentation::SCEPCal_TimingSegmentation(const std::string& cellEncoding) : Segmentation(cellEncoding) {
    _type = "SCEPCal_TimingSegmentation";
    _description = "SCEPCal timing layer segmentation";
    registerIdentifier("identifier_system", "Cell ID identifier for System", fSystemId, "system");
    registerIdentifier("identifier_phi",    "Cell ID identifier for Phi",    fPhiId,    "phi");
    registerIdentifier("identifier_theta",  "Cell ID identifier for Theta",  fThetaId,  "theta");
    registerIdentifier("identifier_gamma",  "Cell ID identifier for Gamma",  fGammaId,  "gamma");
}

SCEPCal_TimingSegmentation::SCEPCal_TimingSegmentation(const BitFieldCoder* decoder) : Segmentation(decoder) {
    _type = "SCEPCal_TimingSegmentation";
    _description = "SCEPCal timing layer segmentation";
    registerIdentifier("identifier_system", "Cell ID identifier for System", fSystemId, "system");
    registerIdentifier("identifier_phi",    "Cell ID identifier for Phi",    fPhiId,    "phi");
    registerIdentifier("identifier_theta",  "Cell ID identifier for Theta",  fThetaId,  "theta");
    registerIdentifier("identifier_gamma",  "Cell ID identifier for Gamma",  fGammaId,  "gamma");
}

SCEPCal_TimingSegmentation::~SCEPCal_TimingSegmentation() {}

Vector3D SCEPCal_TimingSegmentation::position(const CellID& cellId) const {
    if (fPositionOf.find(cellId) != fPositionOf.end()) {
        return fPositionOf.find(cellId)->second;
    }

    return Vector3D(0,0,0);
}
}
}
