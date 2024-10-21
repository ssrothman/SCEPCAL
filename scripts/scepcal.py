import h5py
import numpy as np
from math import atan2, atan, acos, asin, sqrt, sin, cos, tan, floor, ceil
from collections import defaultdict

class MCParticle():
    def __init__(self, mcp):
        self.PDG                = mcp.PDG
        self.generatorStatus    = mcp.generatorStatus
        self.simulatorStatus    = mcp.simulatorStatus
        self.charge             = mcp.charge
        self.time               = mcp.time
        self.mass               = mcp.mass
        self.vx                 = mcp.vertex.x
        self.vy                 = mcp.vertex.y
        self.vz                 = mcp.vertex.z
        self.endx               = mcp.endpoint.x
        self.endy               = mcp.endpoint.y
        self.endz               = mcp.endpoint.z
        self.px                 = mcp.momentum.x
        self.py                 = mcp.momentum.y
        self.pz                 = mcp.momentum.z
        self.endpx              = mcp.momentumAtEndpoint.x
        self.endpy              = mcp.momentumAtEndpoint.y
        self.endpz              = mcp.momentumAtEndpoint.z
        self.spinx              = mcp.spin.x
        self.spiny              = mcp.spin.y
        self.spinz              = mcp.spin.z
        self.colorFlowa         = mcp.colorFlow.a
        self.colorFlowb         = mcp.colorFlow.b
        self.energy             = sqrt(self.px*self.px +self.py*self.py +self.pz*self.pz +self.mass*self.mass)

class MCCollection():
    def __init__(self, mcplist):
        self.particles           = np.array(mcplist)
        self.N                   = len(mcplist)
        self.PDG                 = np.array([mcp.PDG                 for mcp in self.particles])
        self.generatorStatus     = np.array([mcp.generatorStatus     for mcp in self.particles])
        self.simulatorStatus     = np.array([mcp.simulatorStatus     for mcp in self.particles])
        self.charge              = np.array([mcp.charge              for mcp in self.particles])
        self.time                = np.array([mcp.time                for mcp in self.particles])
        self.mass                = np.array([mcp.mass                for mcp in self.particles])
        self.vx                  = np.array([mcp.vx                  for mcp in self.particles])
        self.vy                  = np.array([mcp.vy                  for mcp in self.particles])
        self.vz                  = np.array([mcp.vz                  for mcp in self.particles])
        self.endx                = np.array([mcp.endx                for mcp in self.particles])
        self.endy                = np.array([mcp.endy                for mcp in self.particles])
        self.endz                = np.array([mcp.endz                for mcp in self.particles])
        self.px                  = np.array([mcp.px                  for mcp in self.particles])
        self.py                  = np.array([mcp.py                  for mcp in self.particles])
        self.pz                  = np.array([mcp.pz                  for mcp in self.particles])
        self.endpx               = np.array([mcp.endpx               for mcp in self.particles])
        self.endpy               = np.array([mcp.endpy               for mcp in self.particles])
        self.endpz               = np.array([mcp.endpz               for mcp in self.particles])
        self.spinx               = np.array([mcp.spinx               for mcp in self.particles])
        self.spiny               = np.array([mcp.spiny               for mcp in self.particles])
        self.spinz               = np.array([mcp.spinz               for mcp in self.particles])
        self.colorFlowa          = np.array([mcp.colorFlowa          for mcp in self.particles])
        self.colorFlowb          = np.array([mcp.colorFlowb          for mcp in self.particles])
        self.energy              = np.array([mcp.energy              for mcp in self.particles])
    def __iter__(self):
        for mcp in self.particles:
            yield mcp

class RawHit():
    def __init__(self, hit):
        self.cellID           = hit.cellID
        self.E                = hit.energy
        self.x                = hit.position.x
        self.y                = hit.position.y
        self.z                = hit.position.z
        self.system           = hit.system
        self.neta             = hit.eta
        self.nphi             = hit.phi
        self.ndepth           = hit.depth
        self.ncerenkov        = hit.ncerenkov
        self.nscintillator    = hit.nscintillator
        self.nwavelen_cer     = np.array(hit.nwavelen_cer)
        self.nwavelen_scint   = np.array(hit.nwavelen_scint)
        self.ntime_cer        = np.array(hit.ntime_cer)
        self.ntime_scint      = np.array(hit.ntime_scint)
        self.r                = sqrt(self.x * self.x + self.y * self.y + self.z * self.z)
        self.theta            = acos(self.z / self.r) if self.r != 0 else 0
        self.phi              = atan2(self.y, self.x)

class HitCollection():
    def __init__(self, rawhits):
        self.hits             = np.array(rawhits)
        self.N                = len(rawhits)
        self.cellID           = np.array([hit.cellID                   for hit in self.hits])
        self.E                = np.array([hit.E                        for hit in self.hits])
        self.x                = np.array([hit.x                        for hit in self.hits])
        self.y                = np.array([hit.y                        for hit in self.hits])
        self.z                = np.array([hit.z                        for hit in self.hits])
        self.system           = np.array([hit.system                   for hit in self.hits])
        self.neta             = np.array([hit.neta                     for hit in self.hits])
        self.nphi             = np.array([hit.nphi                     for hit in self.hits])
        self.ndepth           = np.array([hit.ndepth                   for hit in self.hits])
        self.ncerenkov        = np.array([np.array(hit.ncerenkov)      for hit in self.hits])
        self.nscintillator    = np.array([np.array(hit.nscintillator)  for hit in self.hits])
        self.nwavelen_cer     = np.array([np.array(hit.nwavelen_cer)   for hit in self.hits])
        self.nwavelen_scint   = np.array([np.array(hit.nwavelen_scint) for hit in self.hits])
        self.ntime_cer        = np.array([np.array(hit.ntime_cer)      for hit in self.hits])
        self.ntime_scint      = np.array([np.array(hit.ntime_scint)    for hit in self.hits])
        self.r                = np.array([hit.r                        for hit in self.hits])
        self.theta            = np.array([hit.theta                    for hit in self.hits])
        self.phi              = np.array([hit.phi                      for hit in self.hits])
    def __iter__(self):
        for hit in self.hits:
            yield hit

class RawHit_h5:
    def __init__(self, hit):
        self.cellID           = hit['cellID']
        self.E                = hit['E']
        self.x                = hit['x']
        self.y                = hit['y']
        self.z                = hit['z']
        self.system           = hit['system']
        self.neta             = hit['neta']
        self.nphi             = hit['nphi']
        self.ndepth           = hit['ndepth']
        self.ncerenkov        = hit['ncerenkov']
        self.nscintillator    = hit['nscintillator']
        self.nwavelen_cer     = hit['nwavelen_cer']
        self.nwavelen_scint   = hit['nwavelen_scint']
        self.ntime_cer        = hit['ntime_cer']
        self.ntime_scint      = hit['ntime_scint']
        self.r                = hit['r']
        self.theta            = hit['theta']
        self.phi              = hit['phi']

class MCParticle_h5:
    def __init__(self, mcp):
        self.PDG                = mcp['PDG']
        self.generatorStatus    = mcp['generatorStatus']
        self.simulatorStatus    = mcp['simulatorStatus']
        self.charge             = mcp['charge']
        self.time               = mcp['time']
        self.mass               = mcp['mass']
        self.vx                 = mcp['vx']
        self.vy                 = mcp['vy']
        self.vz                 = mcp['vz']
        self.endx               = mcp['endx']
        self.endy               = mcp['endy']
        self.endz               = mcp['endz']
        self.px                 = mcp['px']
        self.py                 = mcp['py']
        self.pz                 = mcp['pz']
        self.endpx              = mcp['endpx']
        self.endpy              = mcp['endpy']
        self.endpz              = mcp['endpz']
        self.spinx              = mcp['spinx']
        self.spiny              = mcp['spiny']
        self.spinz              = mcp['spinz']
        self.colorFlowa         = mcp['colorFlowa']
        self.colorFlowb         = mcp['colorFlowb']
        self.energy             = sqrt(self.px*self.px +self.py*self.py +self.pz*self.pz +self.mass*self.mass)

def load_allevents_from_hdf5(filename):
    SDhits_allevents = {}
    MCP_allevents = {}

    with h5py.File(filename, 'r') as f:
        events_grp = f['Events']
        for event_name in events_grp:
            event_grp = events_grp[event_name]
            try:
                event_num = int(event_name.split('_')[1])
            except (IndexError, ValueError):
                print(f"Warning: Invalid event name format '{event_name}'. Skipping.")
                continue

            hits_grp = event_grp['HitCollection']
            try:
                cellID = hits_grp['cellID'][:]
                E      = hits_grp['E'][:]
                x      = hits_grp['x'][:]
                y      = hits_grp['y'][:]
                z      = hits_grp['z'][:]
                system = hits_grp['system'][:]
                neta    = hits_grp['neta'][:]
                nphi    = hits_grp['nphi'][:]
                ndepth  = hits_grp['ndepth'][:]
                ncerenkov = hits_grp['ncerenkov'][:]
                nscintillator = hits_grp['nscintillator'][:]
                nwavelen_cer = hits_grp['nwavelen_cer'][:]
                nwavelen_scint = hits_grp['nwavelen_scint'][:]
                ntime_cer = hits_grp['ntime_cer'][:]
                ntime_scint = hits_grp['ntime_scint'][:]
                r      = hits_grp['r'][:]
                theta  = hits_grp['theta'][:]
                phi    = hits_grp['phi'][:]
            except KeyError as e:
                print(f"Error: Missing dataset {e} in HitCollection of event {event_num}. Skipping.")
                continue

            rawhits = []
            N_hits = cellID.shape[0]
            for i in range(N_hits):
                hit = {
                    'cellID': cellID[i],
                    'E': E[i],
                    'x': x[i],
                    'y': y[i],
                    'z': z[i],
                    'system': system[i],
                    'neta': neta[i],
                    'nphi': nphi[i],
                    'ndepth': ndepth[i],
                    'ncerenkov': ncerenkov[i],
                    'nscintillator': nscintillator[i],
                    'nwavelen_cer': nwavelen_cer[i],
                    'nwavelen_scint': nwavelen_scint[i],
                    'ntime_cer': ntime_cer[i],
                    'ntime_scint': ntime_scint[i],
                    'r': r[i],
                    'theta': theta[i],
                    'phi': phi[i]
                }
                rawhit = RawHit_h5(hit)
                rawhits.append(rawhit)

            hc = HitCollection(rawhits)

            mc_grp = event_grp['MCCollection']
            try:
                PDG = mc_grp['PDG'][:]
                generatorStatus = mc_grp['generatorStatus'][:]
                simulatorStatus = mc_grp['simulatorStatus'][:]
                charge = mc_grp['charge'][:]
                time = mc_grp['time'][:]
                mass = mc_grp['mass'][:]
                vx = mc_grp['vx'][:]
                vy = mc_grp['vy'][:]
                vz = mc_grp['vz'][:]
                endx = mc_grp['endx'][:]
                endy = mc_grp['endy'][:]
                endz = mc_grp['endz'][:]
                px = mc_grp['px'][:]
                py = mc_grp['py'][:]
                pz = mc_grp['pz'][:]
                endpx = mc_grp['endpx'][:]
                endpy = mc_grp['endpy'][:]
                endpz = mc_grp['endpz'][:]
                spinx = mc_grp['spinx'][:]
                spiny = mc_grp['spiny'][:]
                spinz = mc_grp['spinz'][:]
                colorFlowa = mc_grp['colorFlowa'][:]
                colorFlowb = mc_grp['colorFlowb'][:]
            except KeyError as e:
                print(f"Error: Missing dataset {e} in MCCollection of event {event_num}. Skipping.")
                continue

            mcp_list = []
            N_mcp = PDG.shape[0]
            for i in range(N_mcp):
                mcp = {
                    'PDG': PDG[i],
                    'generatorStatus': generatorStatus[i],
                    'simulatorStatus': simulatorStatus[i],
                    'charge': charge[i],
                    'time': time[i],
                    'mass': mass[i],
                    'vx': vx[i],
                    'vy': vy[i],
                    'vz': vz[i],
                    'endx': endx[i],
                    'endy': endy[i],
                    'endz': endz[i],
                    'px': px[i],
                    'py': py[i],
                    'pz': pz[i],
                    'endpx': endpx[i],
                    'endpy': endpy[i],
                    'endpz': endpz[i],
                    'spinx': spinx[i],
                    'spiny': spiny[i],
                    'spinz': spinz[i],
                    'colorFlowa': colorFlowa[i],
                    'colorFlowb': colorFlowb[i]
                }
                mcp_obj = MCParticle_h5(mcp)
                mcp_list.append(mcp_obj)

            mccoll = MCCollection(mcp_list)

            SDhits_allevents[event_num] = hc
            MCP_allevents[event_num] = mccoll

    print(f"Successfully loaded {len(SDhits_allevents)} events from '{filename}'.")
    return SDhits_allevents, MCP_allevents
