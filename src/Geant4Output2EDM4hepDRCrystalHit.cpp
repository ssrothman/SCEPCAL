//===============================
// Author: Wonyong Chung
//         Princeton University
//===============================
#ifndef DD4HEP_DDG4_Geant4Output2EDM4hepDRCrystalHit_H
#define DD4HEP_DDG4_Geant4Output2EDM4hepDRCrystalHit_H
#include <DD4hep/Detector.h>
#include <DDG4/EventParameters.h>
#include <DDG4/Geant4OutputAction.h>
#include <DDG4/RunParameters.h>
#include <edm4hep/MCParticleCollection.h>
#include <edm4hep/SimTrackerHitCollection.h>
#include <edm4hep/CaloHitContributionCollection.h>
#include <edm4dr/SimDRCalorimeterHitCollection.h>
#include "DRCrystalHit.h"
#include <podio/Frame.h>
#include <podio/ROOTFrameWriter.h>
#include <podio/podioVersion.h>

namespace dd4hep {

  class ComponentCast;

  namespace sim {

    class Geant4ParticleMap;

    class Geant4Output2EDM4hepDRCrystalHit : public Geant4OutputAction  {
    protected:
      using writer_t = podio::ROOTFrameWriter;
      using stringmap_t = std::map< std::string, std::string >;
      using trackermap_t = std::map< std::string, edm4hep::SimTrackerHitCollection >;
      using calorimeterpair_t = std::pair< edm4dr::SimDRCalorimeterHitCollection, edm4hep::CaloHitContributionCollection >;
      using calorimetermap_t = std::map< std::string, calorimeterpair_t >;

      std::unique_ptr<writer_t>     m_file  { };
      podio::Frame                  m_frame { };
      edm4hep::MCParticleCollection m_particles { };
      trackermap_t                  m_trackerHits;
      calorimetermap_t              m_calorimeterHits;
      stringmap_t                   m_runHeader;
      stringmap_t                   m_eventParametersInt;
      stringmap_t                   m_eventParametersFloat;
      stringmap_t                   m_eventParametersString;
      stringmap_t                   m_cellIDEncodingStrings{};
      std::string                   m_section_name      { "events" };
      int                           m_runNo             { 0 };
      int                           m_runNumberOffset   { 0 };
      int                           m_eventNo           { 0 };
      int                           m_eventNumberOffset { 0 };
      bool                          m_filesByRun        { false };
      
      void saveParticles(Geant4ParticleMap* particles);
      void saveFileMetaData();

    public:
      Geant4Output2EDM4hepDRCrystalHit(Geant4Context* ctxt, const std::string& nam);
      virtual ~Geant4Output2EDM4hepDRCrystalHit();
      virtual void beginRun(const G4Run* run);
      virtual void endRun(const G4Run* run);
      virtual void saveRun(const G4Run* run);
      virtual void saveEvent( OutputContext<G4Event>& ctxt);
      virtual void saveCollection( OutputContext<G4Event>& ctxt, G4VHitsCollection* collection);
      virtual void commit( OutputContext<G4Event>& ctxt);
      virtual void begin(const G4Event* event);
    protected:
      template <typename T>
      void saveEventParameters(const std::map<std::string, std::string >& parameters)   {
        for(const auto& p : parameters)   {
          info("Saving event parameter: %-32s = %s", p.first.c_str(), p.second.c_str());
          m_frame.putParameter(p.first, p.second);
        }
      }
    };
    
    template <> void EventParameters::extractParameters(podio::Frame& frame)   {
      for(auto const& p: this->intParameters()) {
        printout(DEBUG, "Geant4OutputEDM4hep", "Saving event parameter: %s", p.first.c_str());
        frame.putParameter(p.first, p.second);
      }
      for(auto const& p: this->fltParameters()) {
        printout(DEBUG, "Geant4OutputEDM4hep", "Saving event parameter: %s", p.first.c_str());
        frame.putParameter(p.first, p.second);
      }
      for(auto const& p: this->strParameters()) {
        printout(DEBUG, "Geant4OutputEDM4hep", "Saving event parameter: %s", p.first.c_str());
        frame.putParameter(p.first, p.second);
      }
      #if podio_VERSION_MAJOR > 0 || podio_VERSION_MINOR > 16 || podio_VERSION_PATCH > 2
      for (auto const& p: this->dblParameters()) {
        printout(DEBUG, "Geant4OutputEDM4hep", "Saving event parameter: %s", p.first.c_str());
        frame.putParameter(p.first, p.second);
      }
      #endif
    }

    template <> void RunParameters::extractParameters(podio::Frame& frame)   {
      for(auto const& p: this->intParameters()) {
        printout(DEBUG, "Geant4OutputEDM4hep", "Saving run parameter: %s", p.first.c_str());
        frame.putParameter(p.first, p.second);
      }
      for(auto const& p: this->fltParameters()) {
        printout(DEBUG, "Geant4OutputEDM4hep", "Saving run parameter: %s", p.first.c_str());
        frame.putParameter(p.first, p.second);
      }
      for(auto const& p: this->strParameters()) {
        printout(DEBUG, "Geant4OutputEDM4hep", "Saving run parameter: %s", p.first.c_str());
        frame.putParameter(p.first, p.second);
      }
      #if podio_VERSION_MAJOR > 0 || podio_VERSION_MINOR > 16 || podio_VERSION_PATCH > 2
      for (auto const& p: this->dblParameters()) {
        printout(DEBUG, "Geant4OutputEDM4hep", "Saving run parameter: %s", p.first.c_str());
        frame.putParameter(p.first, p.second);
      }
      #endif
    }

  }   
}     
#endif

#include <DD4hep/InstanceCount.h>
#include <DD4hep/VolumeManager.h>

#include <DDG4/Geant4HitCollection.h>
#include <DDG4/Geant4DataConversion.h>
#include <DDG4/Geant4SensDetAction.h>
#include <DDG4/Geant4Context.h>
#include <DDG4/Geant4Particle.h>
#include <DDG4/Geant4Data.h>

#include <G4Threading.hh>
#include <G4AutoLock.hh>
#include <G4Version.hh>
#include <G4ParticleDefinition.hh>
#include <G4VProcess.hh>
#include <G4Event.hh>
#include <G4Run.hh>
#include <CLHEP/Units/SystemOfUnits.h>
#include <edm4hep/EventHeaderCollection.h>

using namespace dd4hep::sim;
using namespace dd4hep;
using namespace SCEPCal;

namespace {
  G4Mutex action_mutex = G4MUTEX_INITIALIZER;
}

#include <DDG4/Factories.h>
DECLARE_GEANT4ACTION(Geant4Output2EDM4hepDRCrystalHit)

Geant4Output2EDM4hepDRCrystalHit::Geant4Output2EDM4hepDRCrystalHit(Geant4Context* ctxt, const std::string& nam)
: Geant4OutputAction(ctxt,nam), m_runNo(0), m_runNumberOffset(0), m_eventNumberOffset(0)
{
  declareProperty("RunHeader",             m_runHeader);
  declareProperty("EventParametersInt",    m_eventParametersInt);
  declareProperty("EventParametersFloat",  m_eventParametersFloat);
  declareProperty("EventParametersString", m_eventParametersString);
  declareProperty("RunNumberOffset",       m_runNumberOffset);
  declareProperty("EventNumberOffset",     m_eventNumberOffset);
  declareProperty("SectionName",           m_section_name);
  declareProperty("FilesByRun",            m_filesByRun);
  info("Writer is now instantiated ..." );
  InstanceCount::increment(this);
}

Geant4Output2EDM4hepDRCrystalHit::~Geant4Output2EDM4hepDRCrystalHit()  {
  G4AutoLock protection_lock(&action_mutex);
  m_file.reset();
  InstanceCount::decrement(this);
}

void Geant4Output2EDM4hepDRCrystalHit::beginRun(const G4Run* run)  {
  G4AutoLock protection_lock(&action_mutex);
  std::string fname = m_output;
  m_runNo = run->GetRunID();
  if ( m_filesByRun )    {
    std::size_t idx = m_output.rfind(".");
    if ( idx != std::string::npos )   {
      fname = m_output.substr(0, idx) + _toString(m_runNo, ".run%08d") + m_output.substr(idx);
    }
  }
  if ( !fname.empty() )   {
    m_file = std::make_unique<podio::ROOTFrameWriter>(fname);
    if ( !m_file )   {
      fatal("+++ Failed to open output file: %s", fname.c_str());
    }
    printout( INFO, "Geant4Output2EDM4hepDRCrystalHit" ,"Opened %s for output", fname.c_str() ) ;
  }
}

void Geant4Output2EDM4hepDRCrystalHit::endRun(const G4Run* run)  {
  saveRun(run);
  saveFileMetaData();
  if ( m_file )   {
    m_file->finish();
    m_file.reset();
  }
}

void Geant4Output2EDM4hepDRCrystalHit::saveFileMetaData() {
  podio::Frame metaFrame{};
  for (const auto& [name, encodingStr] : m_cellIDEncodingStrings) {
    metaFrame.putParameter(name + "__CellIDEncoding", encodingStr);
  }

  m_file->writeFrame(metaFrame, "metadata");
}

void Geant4Output2EDM4hepDRCrystalHit::commit( OutputContext<G4Event>& /* ctxt */)   {
  if ( m_file )   {
    G4AutoLock protection_lock(&action_mutex);
    m_frame.put( std::move(m_particles), "MCParticles");
    for (auto it = m_trackerHits.begin(); it != m_trackerHits.end(); ++it)   {
      m_frame.put( std::move(it->second), it->first);
    }
    for (auto& [colName, calorimeterHits] : m_calorimeterHits) {
      m_frame.put( std::move(calorimeterHits.first), colName);
      m_frame.put( std::move(calorimeterHits.second), colName + "Contributions");
    }
    m_file->writeFrame(m_frame, m_section_name);
    m_particles.clear();
    m_trackerHits.clear();
    m_calorimeterHits.clear();
    m_frame = {};
    return;
  }
  except("+++ Failed to write output file. [Stream is not open]");
}

void Geant4Output2EDM4hepDRCrystalHit::saveRun(const G4Run* run)   {
  G4AutoLock protection_lock(&action_mutex);
  podio::Frame runHeader  {};
  for (const auto& [key, value] : m_runHeader)
    runHeader.putParameter(key, value);

  m_runNo = m_runNumberOffset > 0 ? m_runNumberOffset + run->GetRunID() : run->GetRunID();
  runHeader.putParameter("runNumber", m_runNo);
  runHeader.putParameter("GEANT4Version", G4Version);
  runHeader.putParameter("DD4hepVersion", versionString());
  runHeader.putParameter("detectorName", context()->detectorDescription().header().name());

  RunParameters* parameters = context()->run().extension<RunParameters>(false);
  if ( parameters ) {
    parameters->extractParameters(runHeader);
  }

  m_file->writeFrame(runHeader, "runs");
}

void Geant4Output2EDM4hepDRCrystalHit::begin(const G4Event* event)  {
  m_eventNo = event->GetEventID();
  m_frame = {};
  m_particles = {};
  m_trackerHits.clear();
  m_calorimeterHits.clear();
}

void Geant4Output2EDM4hepDRCrystalHit::saveParticles(Geant4ParticleMap* particles)    {
  typedef detail::ReferenceBitMask<const int> PropertyMask;
  typedef Geant4ParticleMap::ParticleMap ParticleMap;
  const ParticleMap& pm = particles->particleMap;

  m_particles.clear();
  if ( pm.size() > 0 )  {
    size_t cnt = 0;
    std::map<int,int> p_ids;
    std::vector<const Geant4Particle*> p_part;
    p_part.reserve(pm.size());
    for (const auto& iParticle : pm) {
      int id = iParticle.first;
      const Geant4ParticleHandle p = iParticle.second;
      PropertyMask mask(p->status);
      const G4ParticleDefinition* def = p.definition();
      auto mcp = m_particles.create();
      mcp.setPDG(p->pdgID);

      float ps_fa[3] = {float(p->psx/CLHEP::GeV),float(p->psy/CLHEP::GeV),float(p->psz/CLHEP::GeV)};
      mcp.setMomentum( ps_fa );

      float pe_fa[3] = {float(p->pex/CLHEP::GeV),float(p->pey/CLHEP::GeV),float(p->pez/CLHEP::GeV)};
      mcp.setMomentumAtEndpoint( pe_fa );

      double vs_fa[3] = { p->vsx/CLHEP::mm, p->vsy/CLHEP::mm, p->vsz/CLHEP::mm } ;
      mcp.setVertex( vs_fa );

      double ve_fa[3] = { p->vex/CLHEP::mm, p->vey/CLHEP::mm, p->vez/CLHEP::mm } ;
      mcp.setEndpoint( ve_fa );

      mcp.setTime(p->time/CLHEP::ns);
      mcp.setMass(p->mass/CLHEP::GeV);
      mcp.setCharge(def ? def->GetPDGCharge() : 0); 

      mcp.setGeneratorStatus(0);
      if( p->genStatus ) {
        mcp.setGeneratorStatus( p->genStatus ) ;
      } else {
        if ( mask.isSet(G4PARTICLE_GEN_STABLE) )             mcp.setGeneratorStatus(1);
        else if ( mask.isSet(G4PARTICLE_GEN_DECAYED) )       mcp.setGeneratorStatus(2);
        else if ( mask.isSet(G4PARTICLE_GEN_DOCUMENTATION) ) mcp.setGeneratorStatus(3);
        else if ( mask.isSet(G4PARTICLE_GEN_BEAM) )          mcp.setGeneratorStatus(4);
        else if ( mask.isSet(G4PARTICLE_GEN_OTHER) )         mcp.setGeneratorStatus(9);
      }

      mcp.setCreatedInSimulation(         mask.isSet(G4PARTICLE_SIM_CREATED) );
      mcp.setBackscatter(                 mask.isSet(G4PARTICLE_SIM_BACKSCATTER) );
      mcp.setVertexIsNotEndpointOfParent( mask.isSet(G4PARTICLE_SIM_PARENT_RADIATED) );
      mcp.setDecayedInTracker(            mask.isSet(G4PARTICLE_SIM_DECAY_TRACKER) );
      mcp.setDecayedInCalorimeter(        mask.isSet(G4PARTICLE_SIM_DECAY_CALO) );
      mcp.setHasLeftDetector(             mask.isSet(G4PARTICLE_SIM_LEFT_DETECTOR) );
      mcp.setStopped(                     mask.isSet(G4PARTICLE_SIM_STOPPED) );
      mcp.setOverlay(                     false );

      if( mcp.isCreatedInSimulation() )
        mcp.setGeneratorStatus( 0 )  ;

      mcp.setSpin(p->spin);
      mcp.setColorFlow(p->colorFlow);

      p_ids[id] = cnt++;
      p_part.push_back(p);
    }

    for(size_t i=0; i < p_ids.size(); ++i)   {
      const Geant4Particle* p = p_part[i];
      auto q = m_particles[i];

      for (const auto& idau : p->daughters) {
        const auto k = p_ids.find(idau);
        if (k == p_ids.end()) {
          fatal("+++ Particle %d: FAILED to find daughter with ID:%d",p->id,idau);
          continue;
        }
        int iqdau = (*k).second;
        auto qdau = m_particles[iqdau];
        q.addToDaughters(qdau);
      }

      for (const auto& ipar : p->parents) {
        if (ipar >= 0) { 
          const auto k = p_ids.find(ipar);
          if (k == p_ids.end()) {
            fatal("+++ Particle %d: FAILED to find parent with ID:%d",p->id,ipar);
            continue;
          }
          int iqpar = (*k).second;
          auto qpar = m_particles[iqpar];
          q.addToParents(qpar);
        }
      }
    }
  }
}

void Geant4Output2EDM4hepDRCrystalHit::saveEvent(OutputContext<G4Event>& ctxt)  {
  EventParameters* parameters = context()->event().extension<EventParameters>(false);
  int runNumber(0), eventNumber(0);
  const int eventNumberOffset(m_eventNumberOffset > 0 ? m_eventNumberOffset : 0);
  const int runNumberOffset(m_runNumberOffset > 0 ? m_runNumberOffset : 0);
  double eventWeight{0};
  if ( parameters ) {
    runNumber = parameters->runNumber() + runNumberOffset;
    eventNumber = parameters->eventNumber() + eventNumberOffset;
    parameters->extractParameters(m_frame);
    #if podio_VERSION_MAJOR > 0 || podio_VERSION_MINOR > 16 || podio_VERSION_PATCH > 2
    eventWeight = m_frame.getParameter<double>("EventWeights").value_or(0.0);
    #endif
  } else {
    runNumber = m_runNo + runNumberOffset;
    eventNumber = ctxt.context->GetEventID() + eventNumberOffset;
  }
  printout(INFO,"Geant4Output2EDM4hepDRCrystalHit","+++ Saving EDM4hep event %d run %d.", eventNumber, runNumber);

  edm4hep::EventHeaderCollection header_collection;
  auto header = header_collection.create();
  header.setRunNumber(runNumber);
  header.setEventNumber(eventNumber);
  header.setWeight(eventWeight);
  header.setTimeStamp( std::time(nullptr) ) ;
  m_frame.put( std::move(header_collection), "EventHeader");

  saveEventParameters<int>(m_eventParametersInt);
  saveEventParameters<float>(m_eventParametersFloat);
  saveEventParameters<std::string>(m_eventParametersString);

  Geant4ParticleMap* part_map = context()->event().extension<Geant4ParticleMap>(false);
  if ( part_map )   {
    print("+++ Saving %d EDM4hep particles....",int(part_map->particleMap.size()));
    if ( part_map->particleMap.size() > 0 )  {
      saveParticles(part_map);
    }
  }
}

struct LazyEncodingExtraction {
  LazyEncodingExtraction(Geant4HitCollection* coll) : m_coll(coll) {}
  operator std::string() const {
    const auto* sd = m_coll->sensitive();
    return dd4hep::sim::Geant4ConversionHelper::encoding(sd->sensitiveDetector());
  }
private:
  Geant4HitCollection* m_coll{nullptr};
};

void Geant4Output2EDM4hepDRCrystalHit::saveCollection(OutputContext<G4Event>& /*ctxt*/, G4VHitsCollection* collection)  {
  
  Geant4HitCollection* coll = dynamic_cast<Geant4HitCollection*>(collection);

  std::string colName = collection->GetName();
  if( coll == nullptr ){
    error(" no Geant4HitCollection:  %s ", colName.c_str());
    return ;
  }

  size_t nhits = collection->GetSize();

  Geant4ParticleMap* pm = context()->event().extension<Geant4ParticleMap>(false);
  debug("+++ Saving EDM4hep collection %s with %d entries.", colName.c_str(), int(nhits));

  m_cellIDEncodingStrings.try_emplace(colName, LazyEncodingExtraction{coll});

  if( typeid( Geant4Tracker::Hit ) == coll->type().type()  ){
    auto& hits = m_trackerHits[colName];
    for(unsigned i=0 ; i < nhits ; ++i){
      auto sth = hits->create();
      const Geant4Tracker::Hit* hit = coll->hit(i);
      const Geant4Tracker::Hit::Contribution& t = hit->truth;
      int   trackID   = pm->particleID(t.trackID);
      auto  mcp       = m_particles.at(trackID);
      const auto& mom = hit->momentum;
      const auto& pos = hit->position;
      edm4hep::Vector3f();
      sth.setCellID( hit->cellID ) ;
      sth.setEDep(hit->energyDeposit/CLHEP::GeV);
      sth.setPathLength(hit->length/CLHEP::mm);
      sth.setTime(hit->truth.time/CLHEP::ns);
      sth.setMCParticle(mcp);
      sth.setPosition( {pos.x()/CLHEP::mm, pos.y()/CLHEP::mm, pos.z()/CLHEP::mm} );
      sth.setMomentum( {float(mom.x()/CLHEP::GeV),float(mom.y()/CLHEP::GeV),float(mom.z()/CLHEP::GeV)} );
      auto particleIt = pm->particles().find(trackID);
      if( ( particleIt != pm->particles().end()) ){
        sth.setProducedBySecondary( (particleIt->second->originalG4ID != t.trackID) );
      }
    }
  }
  else if( typeid( DRCrystalHit ) == coll->type().type() ){
    
    Geant4Sensitive* sd = coll->sensitive();
    int hit_creation_mode = sd->hitCreationMode();

    auto& hits = m_calorimeterHits[colName];
    
    for(unsigned i=0 ; i < nhits ; ++i){
    
      auto sch = hits.first->create();
      const DRCrystalHit* hit = coll->hit(i);

      const auto& pos = hit->position;
      edm4hep::Vector3f hitpos( 10*pos.x(), 10*pos.y(), 10*pos.z() ); // returns in cm, convert to mm

      sch.setCellID( hit->cellID );
      sch.setPosition( hitpos );
      sch.setEnergy( hit->energyDeposit/CLHEP::GeV );

      sch.setNCerenkovProd(     hit->nCerenkovProd );
      sch.setNScintillationProd(hit->nScintillationProd );

      sch.setTAvgC( hit->tSumC / hit->nCerenkovProd );
      sch.setTAvgS( hit->tSumS / hit->nScintillationProd );

      for(auto ci=hit->truth.begin(); ci != hit->truth.end(); ++ci){

        auto sCaloHitCont = hits.second->create();
        sch.addToContributions( sCaloHitCont );

        const Geant4HitData::MonteCarloContrib& c = *ci;
    
        int trackID = pm->particleID(c.trackID);
        auto mcp = m_particles.at(trackID);
    
        sCaloHitCont.setEnergy( c.deposit/CLHEP::GeV );
        sCaloHitCont.setTime( c.time/CLHEP::ns );
        sCaloHitCont.setParticle( mcp );
        
        edm4hep::Vector3f p(c.x/CLHEP::mm, c.y/CLHEP::mm, c.z/CLHEP::mm);
    
        sCaloHitCont.setPDG( c.pdgID );
        sCaloHitCont.setStepPosition( p );
      }
    }
  } 
  else {
    error("+++ unknown type in Geant4HitCollection %s ", coll->type().type().name());
  }
}
