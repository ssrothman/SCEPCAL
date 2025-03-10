#include "DD4hep/Factories.h"
#include "DD4hep/detail/SegmentationsInterna.h"

namespace {
template <typename T>
dd4hep::SegmentationObject* create_segmentation(const dd4hep::BitFieldCoder* decoder) {
  return new dd4hep::SegmentationWrapper<T>(decoder);
}
}

#include "SCEPCal_MainSegmentation.h"
DECLARE_SEGMENTATION(SCEPCal_MainSegmentation, create_segmentation<dd4hep::DDSegmentation::SCEPCal_MainSegmentation>)

#include "SCEPCal_TimingSegmentation.h"
DECLARE_SEGMENTATION(SCEPCal_TimingSegmentation, create_segmentation<dd4hep::DDSegmentation::SCEPCal_TimingSegmentation>)