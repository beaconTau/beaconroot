
import beacon_data_reader
import sys




run = int(sys.argv[1]) if len(sys.argv) > 1 else 188
datapath = sys.argv[2] if len(sys.argv) > 2 else "/data/beacon/root"


d = beacon_data_reader.Reader(datapath,run) 

last_wf = [-1]; 

for i in range(d.N()):
  d.setEntry(i) 
  wf = d.wf(0)
  print i, last_wf[0], wf[0]
  last_wf = wf; 
