# Segmented Crystal Electromagnetic Precision Calorimeter (SCEPCal)

![SCEPCAL3d](https://github.com/SCEPCAL/SCEPCAL/blob/main/examples/scepcal3d.png?raw=true)

Repository for full simulation and analysis.

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

## Running the simulation

Set all options in the steering file `scripts/scepcal_steering.py` and/or set them via the command line at runtime. See `scripts/dd4hep_steering_template.py` for explanations of all options. Refer to dd4hep documentation for more details.

```sh
cd $TOP_DIR
ddsim scripts/scepcal_steering.py
```

### Simulation Options

#### Optical Physics

Disabled by default. Change in `scripts/scepcal_steering.py` to enable:

```python
opticalPhysics = True
```

#### Example particle gun input

```sh
ddsim --steeringFile scripts/scepcal_steering.py -G --gun.direction "1 1 0" --gun.energy "1*GeV" --gun.particle="gamma" -O gamma_1GeV.root
```

#### Changing the Geometry

Detector dimensions and options are defined in `compact/SCEPCal.xml`. See [arXiv: 2408.11027](https://arxiv.org/abs/2408.11027) for details.

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
      <endcap construct="true" phistart="0" phiend="128" thetastart="15"/>

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