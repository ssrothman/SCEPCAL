//===============================
// Author: Wonyong Chung
//         Princeton University
//===============================
#include "SCEPCal_MainSegmentation.h"
#include <climits>
#include <cmath>
#include <stdexcept>

namespace dd4hep {
namespace DDSegmentation {

SCEPCal_MainSegmentation::SCEPCal_MainSegmentation(const std::string& cellEncoding) : Segmentation(cellEncoding) {
    _type = "SCEPCal_MainSegmentation";
    _description = "SCEPCal main layer segmentation";
    registerIdentifier("identifier_system",  "Cell ID identifier for System",  fSystemId,  "system");
    registerIdentifier("identifier_phi",     "Cell ID identifier for Phi",     fPhiId,     "phi");
    registerIdentifier("identifier_theta",   "Cell ID identifier for Theta",   fThetaId,   "theta");
    registerIdentifier("identifier_gamma",   "Cell ID identifier for Gamma",   fGammaId,   "gamma");
    registerIdentifier("identifier_epsilon", "Cell ID identifier for Epsilon", fEpsilonId, "epsilon");
    registerIdentifier("identifier_depth",   "Cell ID identifier for Depth",   fDepthId,   "depth");
}

SCEPCal_MainSegmentation::SCEPCal_MainSegmentation(const BitFieldCoder* decoder) : Segmentation(decoder) {
    _type = "SCEPCal_MainSegmentation";
    _description = "SCEPCal main layer segmentation";
    registerIdentifier("identifier_system",  "Cell ID identifier for System",  fSystemId,  "system");
    registerIdentifier("identifier_phi",     "Cell ID identifier for Phi",     fPhiId,     "phi");
    registerIdentifier("identifier_theta",   "Cell ID identifier for Theta",   fThetaId,   "theta");
    registerIdentifier("identifier_gamma",   "Cell ID identifier for Gamma",   fGammaId,   "gamma");
    registerIdentifier("identifier_epsilon", "Cell ID identifier for Epsilon", fEpsilonId, "epsilon");
    registerIdentifier("identifier_depth",   "Cell ID identifier for Depth",   fDepthId,   "depth");
}

SCEPCal_MainSegmentation::~SCEPCal_MainSegmentation() {}

Vector3D SCEPCal_MainSegmentation::position(const CellID& cellId) const {
    if (fPositionOf.find(cellId) != fPositionOf.end()) {
        return fPositionOf.find(cellId)->second;
    }

    return Vector3D(0,0,0);
}
}
}
