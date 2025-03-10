# Segmented Crystal Electromagnetic Precision Calorimeter (SCEPCal)

![SCEPCAL3d](https://github.com/wonyongc/SCEPCal/blob/main/example/SCEPCal_3d.png?raw=true)
![IDEAfull](https://github.com/wonyongc/SCEPCal/blob/main/example/IDEA_full_labeled.png?raw=true)

Repository for full simulation in dd4hep. Above: Dark/light blue: front/rear projective crystals. Green: timing layer crystals.

## Reference

W. Chung, Differentiable Full Detector Simulation of a Projective Dual-Readout Crystal Electromagnetic Calorimeter with Longitudinal Segmentation and Precision Timing (2024). [arXiv: 2408.11027](https://arxiv.org/abs/2408.11027)

### BiBTeX

```
@article{wchung_calor2024,
	author = {{Chung, Wonyong}},
	title = {Differentiable Full Detector Simulation of a Projective Dual-Readout Crystal Electromagnetic Calorimeter with Longitudinal Segmentation and Precision Timing},
	DOI= "10.1051/epjconf/202532000052",
	url= "https://doi.org/10.1051/epjconf/202532000052",
	journal = {EPJ Web Conf.},
	year = 2025,
	volume = 320,
	pages = "00052",
}
```
## Compile/Install (lxplus9)

### Use my install

You are free to use my install by doing the following:

```sh
source /cvmfs/sw.hsf.org/key4hep/setup.sh
source /eos/user/w/wochung/src/k4geo/install/bin/thisk4geo.sh
```
Otherwise, you are free to compile your own copy:

```sh
git clone git@github.com:wonyongc/k4geo.git
cd k4geo
git switch scepcal_with_readme # Make sure to checkout the scepcal branch
mkdir build install

source /cvmfs/sw.hsf.org/key4hep/setup.sh

cd build
cmake -DCMAKE_INSTALL_PREFIX=../install -DPython_EXECUTABLE=$(which python) ..
make install -j4

source ../install/bin/thisk4geo.sh
```
After compilation/installation, subsequent uses in new sessions need only:
```sh
source /cvmfs/sw.hsf.org/key4hep/setup.sh
source /your/install/bin/thisk4geo.sh
```


## To run

```sh
ddsim --steeringFile example/scepcal_steering.py
```
or, using command line options,
```sh
ddsim --steeringFile example/scepcal_steering.py -G --gun.direction "1 1 0" --gun.energy "1*GeV" --gun.particle="gamma" -O gamma_1GeV.root
```

### Crystal Geometry

Detector dimensions and options are defined in the `<define>` tag in `compact/SCEPCal.xml`. The interesting parameters are the crystal widths and front/rear tower divisions. The tower width (theta width) refers to the nominal square width of a single crystal tower, which will then contain the specified number of NxN crystals for the front and rear compartments. Care should be taken to make sure these numbers are consistent with what is intended. A tower width of 1cm with 1F/1R divisions will make the default 1:1 configuration of 1x1cm crystals for both front/rear. A tower width of 3cm with a 3F/2R division will make a 3x3 array of 1x1cm front crystals, and a 2x2 array of 1.5x1.5cm rear crystals, etc. (All dimensions nominal).

![SCEPCal_CrystalTowerDivisions](https://github.com/wonyongc/SCEPCal/blob/main/example/SCEPCal_CrystalTowerDivisions.png?raw=true)


```xml
  <define>
    <!-- Likely do not need to be changed -->
    <constant name="scepcal_barrel_half_z" value="2.45*m"/>
    <constant name="scepcal_barrel_inner_r" value="2.25*m"/>
    <constant name="scepcal_phi_segments" value="128"/>
    <constant name="scepcal_beampipe_opening" value="45*cm"/>
    <constant name="scepcal_mainlayer_reargap" value="1*cm"/>
    <constant name="scepcal_timinglayer_gap" value="1*cm"/>
    <constant name="scepcal_projective_offset_r" value="10*cm"/>
    <constant name="scepcal_projective_offset_x" value="10*cm"/>

    <!-- Nominal square width of a single crystal tower -->
    <constant name="scepcal_xtal_theta_width" value="1*cm"/>

    <!-- Square NxN divisions of front/rear crystals in a single tower -->
    <constant name="scepcal_xtal_divisions_f" value="1"/>
    <constant name="scepcal_xtal_divisions_r" value="1"/>

    <!-- Longitudinal length of front/rear crystals -->
    <constant name="scepcal_xtal_length_f" value="5*cm"/>
    <constant name="scepcal_xtal_length_r" value="15*cm"/>

    <!-- Timing crystals -->
    <constant name="scepcal_timing_xtal_depth" value="5*mm"/>
    <constant name="scepcal_timing_xtal_length" value="10*cm"/>
  </define>
```

### Steering File

Set all options in the steering file `example/scepcal_steering.py`. Relevant parameters are gathered at the top of the file:

```python
settings = {
     'opticalPhysics': setupCerenkovScint, # or setupCerenkovOnly
     'edep' : 1*keV,

     'N'        : 1,
     
     'MC'       : False,
     'MC_file'  : '/your/mc/file',

     'gun'      : True,
     'particle' : 'e-',
     'momentum' : 10*GeV,
     'plusminus': 0.05*10*GeV,
     'theta'    : [(10)*pi/180.0,(170)*pi/180.0],
     'phi'      : [(0)*pi/180.0, (360)*pi/180.0],

     'compactFile': '../compact/SCEPCal.xml'
}
```
Parameters can also be set via the command line at runtime. See `example/dd4hep_steering_template.py` for explanations of all options. Refer to dd4hep documentation for more details.

#### Event selection

Input MC files can be set in the steering file as above (see the template for a list of accepted formats), or the built-in ddsim particle gun can be used. If both are enabled, both will run. `wzp6_ee_ZZ_test_ecm240_1k.stdhep` with 1k events is provided as an example (lxplus only):

```python
SIM.inputFiles = ['example/wzp6_ee_ZZ_test_ecm240_1k.stdhep']
```

## Readout Collections

An example output file with the full IDEA detector can be found here:
```
/eos/user/w/wochung/wzp6_ee_ZZ_test_ecm240_1k_N10.root
```

There are three readout collections for each layer, Main and Timing:
```
SCEPCal_MainEdep
SCEPCal_MainScounts
SCEPCal_MainCcounts

SCEPCal_TimingEdep
SCEPCal_TimingScounts
SCEPCal_TimingCcounts
```
All collections are of type `edm4hep::SimCalorimeterHit`. The Edep collections save energy deposits for all particles, subject to the edep filter threshold. The S/C count collections save only the counts of generated S/C photons, which are counted at the first step and then killed. The counts are saved as the "energy" of the hit (a workaround requested to avoid having to introduce a custom readout class). MC Particles are saved but MC step constributions have been disabled to save disk space.

### edm4hep::SimCalorimeterHit

```yaml
edm4hep::SimCalorimeterHit:
  Description: "Simulated calorimeter hit"
  Author: "EDM4hep authors"
  Members:
    - uint64_t cellID                      // ID of the sensor that created this hit
    - float energy [GeV]                   // energy of the hit
    - edm4hep::Vector3f position [mm]      // position of the hit in world coordinates
  OneToManyRelations:
    - edm4hep::CaloHitContribution contributions  // Monte Carlo step contributions
```

```yaml
edm4hep::MCParticle:
  Description: "The Monte Carlo particle - based on the lcio::MCParticle."
  Author: "F.Gaede, DESY"
  Members:
    - int32_t PDG
    - int32_t generatorStatus
    - int32_t simulatorStatus
    - float charge
    - float time
    - double mass
    - edm4hep::Vector3d vertex
    - edm4hep::Vector3d endpoint
    - edm4hep::Vector3f momentum
    - edm4hep::Vector3f momentumAtEndpoint
    - edm4hep::Vector3f spin
    - edm4hep::Vector2i colorFlow
```

## Analysis

The same ROOT environment as used when running the simulation should be used to run any analysis on ROOT files, as ROOT needs to be aware of edm4hep in order to process the file. Utilities to read the ROOT collections into python objects are provided in `python/scepcal.py`.


### Example reading ROOT output and making a simple 3D plot

```python
import plotly
import plotly.graph_objs as go
plotly.offline.init_notebook_mode()
from scepcal.scepcal import *

# Define root collection (branch) names and hit types
# Use https://github.com/wonyongc/k4geo/tree/scepcal to simulate the full IDEA detector to get the other subdetector collections
collection_names_types = { 
    "MCParticles":            { 'type': 'MCParticle' },
    
    # "LumiCalCollection":      { 'type': 'SimCalorimeterHit' },
    
    # "VertexBarrelCollection": { 'type': 'SimTrackerHit' },
    # "VertexEndcapCollection": { 'type': 'SimTrackerHit' },
    
    # "DCHCollection":          { 'type': 'SimTrackerHit' },
    
    # "SiWrBCollection":        { 'type': 'SimTrackerHit' },
    # "SiWrDCollection":        { 'type': 'SimTrackerHit' },
    
    "SCEPCal_MainEdep":       { 'type': 'SimCalorimeterHit' },
    "SCEPCal_MainScounts":    { 'type': 'SimCalorimeterHit' },
    "SCEPCal_MainCcounts":    { 'type': 'SimCalorimeterHit' },

    "SCEPCal_TimingEdep":       { 'type': 'SimCalorimeterHit' },
    "SCEPCal_TimingScounts":    { 'type': 'SimCalorimeterHit' },
    "SCEPCal_TimingCcounts":    { 'type': 'SimCalorimeterHit' },

    # "DRBTScin":               { 'type': 'SimCalorimeterHit' },
    # "DRBTCher":               { 'type': 'SimCalorimeterHit' },
    
    # "DRETScinLeft":           { 'type': 'SimCalorimeterHit' },
    # "DRETScinRight":          { 'type': 'SimCalorimeterHit' },
    # "DRETCherLeft":           { 'type': 'SimCalorimeterHit' },
    # "DRETCherRight":          { 'type': 'SimCalorimeterHit' },
    
    # "MuonSystemCollection":   { 'type': 'SimTrackerHit' },
}

# Get all hits for all subdetectors, all events
file = 'output.root'
allHits = SubdetectorHitsForAllEvents(file, collection_names_types)

# Get subdetector hits for one event
event_num = 0
SCEPCal_hits = allHits.getHits(event_num, 'SCEPCal_MainEdep')

# Use python list comprehensions for easy filtering. System numbers defined in compact/SCEPCal.xml
barrelHits = SimCalorimeterHitCollection( [ h for h in SCEPCal_hits if h.system==4] )
endcapHits = SimCalorimeterHitCollection( [ h for h in SCEPCal_hits if h.system==5] )

# Simple 3D plot
layout = go.Layout(
    autosize=False,
    width=1000,
    height=1000,
    scene = dict(
                xaxis = dict(range=[-3000,3000],),
                yaxis = dict(range=[-3000,3000],),
                zaxis = dict(range=[-3000,3000],),
            )
    )

data = []
barrel_hitmarkers = go.Scatter3d(
        x=barrelHits[i].x,
        y=barrelHits[i].y,
        z=barrelHits[i].z,
        mode='markers',
        marker={'size': 1, 'color': 'blue'}
    )
endcap_hitmarkers = go.Scatter3d(
        x=barrelHits[i].x,
        y=barrelHits[i].y,
        z=barrelHits[i].z,
        mode='markers',
        marker={'size': 1, 'color': 'red'}
    )
data.extend(barrel_hitmarkers)
data.extend(endcap_hitmarkers)

fig = go.Figure(data=data, layout=layout)
plotly.offline.iplot(fig) 

```

### Running on Condor

Change the user directory paths in `batch/condor/SCEPCalsim.sh` and `batch/condor/SCEPCalsim.sub` to your own, and also change the path of the compact XML file in `example/scepcal_steering.py` to the absolute path rather than the relative path, e.g.:

```python
SIM.compactFile = ['/eos/user/w/wochung/src/SCEPCal/compact/SCEPCal.xml']
```

and then run

```sh
condor_submit scripts/SCEPCalsim.sub
```

on lxplus9.

### Contact

Feel free to get in touch for feature requests or collaboration:

wonyongc@princeton.edu