# Segmented Crystal Electromagnetic Precision Calorimeter (SCEPCal)

![SCEPCAL3d](https://github.com/wonyongc/SCEPCal/blob/main/examples/scepcal3d.png?raw=true)

Repository for full simulation and analysis. Above: Dark/light blue: front/rear projective crystals. Red/green: timing layer crystals. Purple/yellow: front/rear non-projective crystals to mitigate projective gaps.

## Citations

W. Chung, Differentiable Full Detector Simulation of a Projective Dual-Readout Crystal Electromagnetic Calorimeter with Longitudinal Segmentation and Precision Timing (2024). [arXiv: 2408.11027](https://arxiv.org/abs/2408.11027)

M. T. Lucchini, W. Chung, S. C. Eno, Y. Lai, L. Lucchini, M.-T. Nguyen, C. G. Tully, New Perspectives on Segmented Crystal Calorimeters for Future Colliders, JINST 15 (11) (2020) P11005. [arXiv:2008.00338](https://arxiv.org/abs/2008.00338), [doi:10.1088/1748-0221/15/11/P11005](https://doi.org/10.1088/1748-0221/15/11/P11005)

### BiBTeX

```
@misc{chung2024_differentiable-full-sim,
      title={Differentiable Full Detector Simulation of a Projective Dual-Readout Crystal Electromagnetic Calorimeter with Longitudinal Segmentation and Precision Timing}, 
      author={Wonyong Chung},
      year={2024},
      eprint={2408.11027},
      archivePrefix={arXiv},
      primaryClass={physics.ins-det},
      url={https://arxiv.org/abs/2408.11027}, 
}

@article{Lucchini_2020,
   title={New perspectives on segmented crystal calorimeters for future colliders},
   volume={15},
   ISSN={1748-0221},
   url={http://dx.doi.org/10.1088/1748-0221/15/11/P11005},
   DOI={10.1088/1748-0221/15/11/p11005},
   number={11},
   journal={Journal of Instrumentation},
   publisher={IOP Publishing},
   author={M.T. Lucchini, W. Chung, S.C. Eno, Y. Lai, L. Lucchini, M. Nguyen, and C.G. Tully},
   year={2020},
   month=nov, pages={P11005–P11005}
}
```

## Compile/Install on lxplus9

```sh
git clone git@github.com:wonyongc/SCEPCal.git
cd SCEPCal
mkdir build install

export TOP_DIR=$PWD
export MY_INSTALL_DIR=$PWD/install

source /cvmfs/sw.hsf.org/key4hep/setup.sh

cd build; cmake -DCMAKE_INSTALL_PREFIX=$MY_INSTALL_DIR -DPython_EXECUTABLE=$(which python) ..
make install -j4

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MY_INSTALL_DIR/lib64
export PYTHONPATH=$PYTHONPATH:$MY_INSTALL_DIR/python
```

After compilation/installation, subsequent uses in new sessions need only:

```sh
source /cvmfs/sw.hsf.org/key4hep/setup.sh
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MY_INSTALL_DIR/lib64
export PYTHONPATH=$PYTHONPATH:$MY_INSTALL_DIR/python
```

## Running the simulation


### Steering file
Set all options in the steering file `scripts/scepcal_steering.py` and/or set them via the command line at runtime. See `scripts/dd4hep_steering_template.py` for explanations of all options. Refer to dd4hep documentation for more details.

### Simulation Options

#### Optical Physics

Disabled by default. Change in `scripts/scepcal_steering.py` to enable:

```python
opticalPhysics = True
```

With optical physics enabled, primary S/C generated photons are counted and then immediately killed, i.e. no secondaries are counted or propagated.

#### Event selection

Input MC files can be set in the steering file (see the template for a list of accepted formats), or the built-in ddsim particle gun can be used. If both are enabled, both will run. `wzp6_ee_ZZ_test_ecm240_1k.stdhep` with 1k events is provided as an example:

```python
SIM.inputFiles = ['examples/wzp6_ee_ZZ_test_ecm240_1k.stdhep']
```

To run,

```sh
cd $TOP_DIR
ddsim scripts/scepcal_steering.py
```

or, using command line options,

```sh
ddsim --steeringFile scripts/scepcal_steering.py -G --gun.direction "1 1 0" --gun.energy "1*GeV" --gun.particle="gamma" -O gamma_1GeV.root
```

#### edm4hep output classes

Each event in `gamma_1GeV.root` contains the trees `SCEPCal_readout` and `MCParticles`.

Hits in `SCEPCal_readout` have the following schema as defined in `edm4dr.yaml`. Currently only the number of S/C photons produced and their average arrival times are recorded. Scale factors and poisson smearing can be applied offline.

```yaml
edm4dr::SimDRCalorimeterHit:
  Description: "Simulated dual-readout calorimeter hit"
  Author: "Wonyong Chung"
  Members:
    - uint64_t cellID                           // detector cellID
    - float energy [GeV]                        // energy of the hit
    - edm4hep::Vector3f position [mm]           // position of the calorimeter cell in world coords
    - int32_t nCerenkovProd                     // number of cerenkov photons produced
    - int32_t nScintillationProd                // number of scint photons produced
    - float tAvgC [ns]                          // avg arrival time for cerenkov photons
    - float tAvgS [ns]                          // avg arrival time for scint photons
```
Hits in `MCParticles` are default edm4hep classes:

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

#### Analysis

The same ROOT environment as used when running the simulation should be used to run any analysis on ROOT files, as ROOT needs to be aware of the edm4hep/edm4dr dictionary in order to process the file.

#### Convert to hdf5

Alternatively, a script to convert the ROOT file to an hdf5 file is provided for offline/notebook analysis.

```sh
python scripts/convertROOT2HDF5.py gamma_1GeV.root
```

This will produce the file `gamma_1GeV.hdf5` with the following file structure:

```
/
├── Events
│   ├── Event_0001
│   │   ├── HitCollection
│   │   │   ├── cellID              (uint64)
│   │   │   ├── E                   (float32)
│   │   │   ├── x                   (float32)
│   │   │   ├── y                   (float32)
│   │   │   ├── z                   (float32)
│   │   │   ├── system              (int32)
│   │   │   ├── eta                 (int32)
│   │   │   ├── phi                 (int32)
│   │   │   ├── depth               (int32)
│   │   │   ├── ncerenkovprod       (int32)
│   │   │   ├── nscintillationprod  (int32)
│   │   │   ├── tavgc               (float32)
│   │   │   ├── tavgs               (float32)
│   │   │   ├── r                   (float32)
│   │   │   ├── theta               (float32)
│   │   │   └── phi                 (float32)
│   │   ├── MCCollection
│   │       ├── PDG                 (int32)
│   │       ├── generatorStatus     (int32)
│   │       ├── simulatorStatus     (int32)
│   │       ├── charge              (float32)
│   │       ├── time                (float32)
│   │       ├── mass                (float64)
│   │       ├── vx                  (float64)
│   │       ├── vy                  (float64)
│   │       ├── vz                  (float64)
│   │       ├── endx                (float64)
│   │       ├── endy                (float64)
│   │       ├── endz                (float64)
│   │       ├── px                  (float32)
│   │       ├── py                  (float32)
│   │       ├── pz                  (float32)
│   │       ├── endpx               (float32)
│   │       ├── endpy               (float32)
│   │       ├── endpz               (float32)
│   │       ├── spin_x              (float32)
│   │       ├── spin_y              (float32)
│   │       ├── spin_z              (float32)
│   │       ├── colorFlow_x         (int32)
│   │       └── colorFlow_y         (int32)
│   ├── Event_0002
│   │   ├── HitCollection
│   │   │   └── ...
│   │   ├── MCCollection
│   │       └── ...
│   └── ...
```

Python classes and functions to unpack and use the hdf5 file are provided in `scripts/scepcal_utils.py`.

#### Example python usage

```python
from scepcal import *

hdf5file = 'gamma_10GeV_n10_isotrop.hdf5'
SDhits_allevents, MCP_allevents = load_allevents_from_hdf5(hdf5file)
SDhits = SDhits_allevents[0] #event number 0
MCcoll = MCP_allevents[0]

barrelHits = HitCollection( [ h for h in SDhits if h.system==1] )
endcapHits = HitCollection( [ h for h in SDhits if h.system==2] )
timingHits = HitCollection( [ h for h in SDhits if h.system==3] )

layout = go.Layout(
    autosize=False,
    width=1000,
    height=1000,
    scene = dict(
                xaxis = dict(range=[-250,250],),
                yaxis = dict(range=[-250,250],),
                zaxis = dict(range=[-250,250],),
            )
    )

data = []
for i in range(10):
    hitmarkers = go.Scatter3d(
            x=SDhits_allevents[i].x,
            y=SDhits_allevents[i].y,
            z=SDhits_allevents[i].z,
            mode='markers',
            marker={'size': 1}
        )
    data.append(hitmarkers)

fig = go.Figure(data=data, layout=layout)
plotly.offline.iplot(fig) 

```
![gamma_10GeV_n10_isotrop](https://github.com/wonyongc/SCEPCal/blob/main/examples/gamma_10GeV_n10_isotrop.png?raw=true)

See `scepcal_utils.py` for the hits and HitCollection definitions.


#### Geometry Details / Changing the Geometry

Detector dimensions and options are defined in `compact/SCEPCal.xml`. See [arXiv: 2408.11027](https://arxiv.org/abs/2408.11027) for details.

The overall detector dimensions and crystal dimensions can be changed in the `<dim>` tag. The geometry construction is fully parameterized and will auto-generate the detector for the given inputs. 

`phiSegments` determines the number of phi segmentations in the geometry, but only `phistart` to `phiend` will actually be constructed (for visualization purposes). Make sure `phiend` is set equal to `phiSegments` (and `phistart` is 0) for a full geometry.  `thetastart` in the endcap refers to how much of a gap in theta slices to leave for the beampipe. 5 is generally fine.

A 10x scaled up version of the geometry (`compact/SCEPCal_10x.xml`) is provided for visualization purposes.

PbWO and LYSO are included in the material definitions in the compact XML file.

`projectiveFill` refers to the number of non-projective theta slices added, centered at z=0, used to offset the detector to mitigate projective gaps. These are colored in purple/yellow in the top image.

```xml
  <detectors>
 
    <detector id="1"
              name="SCEPCal"
              type="SegmentedCrystalECAL" 
              readout="SCEPCal_readout"
              vis="scepcalAssemblyGlobalVis"
              sensitive="true">
      <sensitive type="SegmentedCrystalCalorimeter"/>

      <timing construct="true" phistart="0" phiend="128"/>
      <barrel construct="true" phistart="0" phiend="128"/>
      <endcap construct="true" phistart="0" phiend="128" thetastart="5"/>

      <dim    barrelHalfZ="2.25*m"
              barrelInnerR="2*m" 
              crystalFaceWidthNominal="10*mm"
              crystalFlength="50*mm"
              crystalRlength="150*mm"
              crystalTimingThicknessNominal="3*mm"
              sipmThickness="0.5*mm"
              phiSegments="128"
              projectiveFill="3"
      />

      <projF                                    vis="projectiveFillFVis"/>
      <projR                                    vis="projectiveFillRVis"/>
      <crystalF        material="PbWO"          vis="crystalFVis"/>
      <crystalR        material="PbWO"          vis="crystalRVis"/>
      <timingLayerLg   material="LYSO"          vis="timingVisLg"/>
      <timingLayerTr   material="LYSO"          vis="timingVisTr"/>
      <inst            material="AluminumOxide" vis="instVis"/>
      <sipmLg          material="Silicon"       vis="sipmVisLg"/>
      <sipmTr          material="Silicon"       vis="sipmVisTr"/>

      <scepcalAssembly vis="scepcalAssemblyVis"/>
      
      <timingAssemblyGlobalVis  vis="timingAssemblyGlobalVis"/>
      <barrelAssemblyGlobalVis  vis="barrelAssemblyGlobalVis"/>
      <endcapAssemblyGlobalVis  vis="endcapAssemblyGlobalVis"/>

      <scepcalAssemblyGlobalVis vis="scepcalAssemblyGlobalVis"/>

    </detector>

  </detectors>
```

### Running on Condor

Change the user directory paths in `scripts/SCEPCalsim.sh` and `scripts/SCEPCalsim.sub` to your own, and also change the path of the compact XML file in `scripts/scepcal_steering.py` to the absolute path rather than the relative path, e.g.:

```python
# SIM.compactFile = ['install/share/compact/SCEPCal.xml']
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