from ROOT import TFile
from math import atan2, atan, acos, asin, sqrt, sin, cos, tan, floor, ceil
import numpy as np
import h5py
import sys

if len(sys.argv) > 1:
    arg1 = sys.argv[1]
    print(f"Input ROOT file: {arg1}")
else:
    sys.exit("No input ROOT file specified.")

def getsystem(cellID):
    return cellID & 0b1111   # system last 4 bits

def geteta(cellID):
    return (cellID>>4) & 0b11111111111    # eta preceding 11 bits

def getphi(cellID):
    return (cellID>>15) & 0b11111111111     # phi preceding 11 bits

def getdepth(cellID):
    return (cellID>>26) & 0b1111            # depth preceding 4 bits


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
    # Takes edm4hep SimCalorimeterDRHit
    def __init__(self, hit):
        self.cellID           = hit.cellID
        self.E                = hit.energy
        self.x                = hit.position.x
        self.y                = hit.position.y
        self.z                = hit.position.z

        # convert from readout bits
        self.system           = getsystem(self.cellID)
        self.neta             = geteta(self.cellID)
        self.nphi             = getphi(self.cellID)
        self.ndepth           = getdepth(self.cellID)

        self.ncerenkovprod      = hit.nCerenkovProd
        self.nscintillationprod = hit.nScintillationProd
        self.tavgc              = hit.tAvgC
        self.tavgs              = hit.tAvgS

        self.r                = sqrt(self.x * self.x + self.y * self.y + self.z * self.z)
        self.theta            = acos(self.z / self.r) if self.r != 0 else 0
        self.phi              = atan2(self.y, self.x)
        # self.contribs = hit.contributions  # one-to-many relations not implemented in python classes

class HitCollection():
    # Takes python array of RawHit
    def __init__(self, rawhits):
        self.hits                = np.array(rawhits)
        self.N                   = len(rawhits)
        self.cellID              = np.array([hit.cellID              for hit in self.hits])
        self.E                   = np.array([hit.E                   for hit in self.hits])
        self.x                   = np.array([hit.x                   for hit in self.hits])
        self.y                   = np.array([hit.y                   for hit in self.hits])
        self.z                   = np.array([hit.z                   for hit in self.hits])
        self.system              = np.array([hit.system              for hit in self.hits])
        self.neta                = np.array([hit.neta                for hit in self.hits])
        self.nphi                = np.array([hit.nphi                for hit in self.hits])
        self.ndepth              = np.array([hit.ndepth              for hit in self.hits])
        self.ncerenkovprod       = np.array([hit.ncerenkovprod       for hit in self.hits])
        self.nscintillationprod  = np.array([hit.nscintillationprod  for hit in self.hits])
        self.tavgc               = np.array([hit.tavgc               for hit in self.hits])
        self.tavgs               = np.array([hit.tavgs               for hit in self.hits])
        self.r                = np.array([hit.r                      for hit in self.hits])
        self.theta            = np.array([hit.theta                  for hit in self.hits])
        self.phi              = np.array([hit.phi                    for hit in self.hits])
    def __iter__(self):
        for hit in self.hits:
            yield hit


def getHitsForAllEvents(fname):
    f = TFile.Open(fname)

    SDhitsForEvent = {}
    MCParticlesForEvent = {}

    for i, event in enumerate(f.events):
        SDhitlayer  = event.SCEPCal_readout
        MClayer     = event.MCParticles

        SDhitsForEvent[i]      = HitCollection([RawHit(hit) for hit in SDhitlayer]) if SDhitlayer else None
        MCParticlesForEvent[i] = MCCollection([MCParticle(mcp) for mcp in MClayer])

    return SDhitsForEvent, MCParticlesForEvent

def save_allevents_to_hdf5(SDhits_allevents, MCP_allevents, filename):
    with h5py.File(filename, 'w') as f:
        events_grp = f.create_group('Events')
        event_numbers = sorted(SDhits_allevents.keys())
        print(f'Events: {event_numbers}')
        for event_num in event_numbers:
            event_grp = events_grp.create_group(f'Event_{event_num}')
            hc = SDhits_allevents[event_num]

            print(f'HitCollection.N: {hc.N}')
            mccoll = MCP_allevents.get(event_num, None)
            if mccoll is None:
                print(f"Warning: No MCCollection found for event {event_num}")
                continue
            
            hits_grp = event_grp.create_group('HitCollection')
            
            hit_attrs = {
                'cellID': 'uint64',
                'E': 'float32',
                'x': 'float32',
                'y': 'float32',
                'z': 'float32',
                'system': 'int32',
                'neta': 'int32',
                'nphi': 'int32',
                'ndepth': 'int32',
                'ncerenkovprod': 'int32',
                'nscintillationprod': 'int32',
                'tavgc': 'float32',
                'tavgs': 'float32',
                'r': 'float32',
                'theta': 'float32',
                'phi': 'float32'
            }
                    
            for attr, dtype in hit_attrs.items():
                data = getattr(hc, attr)
                hits_grp.create_dataset(attr, data=data, dtype=dtype, compression="gzip", compression_opts=4)
            
            mc_grp = event_grp.create_group('MCCollection')
            
            mc_attrs = {
                'PDG': 'int32',
                'generatorStatus': 'int32',
                'simulatorStatus': 'int32',
                'charge': 'float32',
                'time': 'float32',
                'mass': 'float64',
                'vx': 'float64',
                'vy': 'float64',
                'vz': 'float64',
                'endx': 'float64',
                'endy': 'float64',
                'endz': 'float64',
                'px': 'float32',
                'py': 'float32',
                'pz': 'float32',
                'endpx': 'float32',
                'endpy': 'float32',
                'endpz': 'float32',
                'spinx': 'float32',
                'spiny': 'float32',
                'spinz': 'float32',
                'colorFlowa': 'int32',
                'colorFlowb': 'int32'
            }
            
            spin_array = np.array([ [mcp.spinx, mcp.spiny, mcp.spinz] for mcp in mccoll.particles ], dtype='float32')
            colorFlow_array = np.array([ [mcp.colorFlowa, mcp.colorFlowb] for mcp in mccoll.particles ], dtype='int32')
            
            for attr, dtype in mc_attrs.items():
                if attr.startswith('spin'):
                    component = attr[4]  # 'x', 'y', or 'z'
                    index = {'x': 0, 'y': 1, 'z': 2}[component]
                    data = spin_array[:, index]
                    mc_grp.create_dataset(attr, data=data, dtype=dtype)
                elif attr.startswith('colorFlow'):
                    component = attr[9]  # 'a' or 'b'
                    index = {'a': 0, 'b': 1}[component]
                    data = colorFlow_array[:, index]
                    mc_grp.create_dataset(attr, data=data, dtype=dtype)
                else:
                    data = getattr(mccoll, attr)
                    mc_grp.create_dataset(attr, data=data, dtype=dtype)
            
        f.attrs['N_Events'] = len(event_numbers)
    
    print(f"All events successfully saved to {filename}")

inputROOT = sys.argv[1]

SDhits_allevents, MCP_allevents = getHitsForAllEvents(inputROOT)

save_allevents_to_hdf5(SDhits_allevents, MCP_allevents, f'{inputROOT[:-5]}.hdf5')