#! /usr/bin/env python3 

# Example for how to read beacon data with python This mostly defines a
# beacon_data_reader class, but if you run it as a script it  will try to plot
# some waveforms from run 362. Normally, you would do something like
#    
#   import beacon_data_reader 
#   d = beacon_data_reader.Reader("/path/to/data",runno)  
#   d.setEntry(someInterestingEntry) 
#   d.wf(0) # get a numpy array of channel 0 



import ROOT
import numpy 
import sys
import os
import inspect
import cppyy

## You need to load the libbeaconroot library
## For the following line to work, the shared library must be in 
## your dynamic loader path (LD_LIBRARY_PATH or DYLD_LIBRARY_PATH, accordingly) 

ROOT.gSystem.Load("libbeaconroot.so"); 


## A class to read in data from a run
##  
##   e.g. d = beacon_data_reader("/path/to/runs",runno) 
##
##   To select the 100th entry in the run
##     d.setEntry(65079); 
##   Or, you can select by event number instead 
##     d.setEvent(360000065079); 
##  
##   You can then obtain the corresponding header, event or status objects
##     d.header()
##     d.event() 
##     d.status()  (this gets the status with the closest readout time to the event) 
##    
##   For now, you can look at the c++ doc to see what is in each
##  
##  You can also numpy arrays of the waveform values and time using 
## 
##  d.wf(channel)  
##   
##  For convenience, d.t() makes a time array with the right number of samples (the sample rate is always 0.5 GSPS) 

class Reader:
  '''
  This is the python reader wrapper that allows for pulling the event data for BEACON.
  
  Parameters
  ----------
  base_dir : str
      The directory in which you have BEACON data files stored.
  run : int
      The run number for which you want to access data.
  '''
  def __init__(self,base_dir, run):
    try:
      self.failed_setup = False
      self.run = run; 
      self.base_dir = base_dir

      self.event_file = ROOT.TFile.Open("%s/run%d/event.root" % (base_dir, run))
      self.event_tree = self.event_file.Get("event") 
  #    self.evt = ROOT.beacon.Event() 
      self.event_entry = -1; 
  #    self.event_tree.SetBranchAddress("event",ROOT.addressof(self.evt))

      self.head_file = ROOT.TFile.Open("%s/run%d/header.root" % (base_dir, run))
      self.head_tree = self.head_file.Get("header") 
  #    self.head = ROOT.beacon.Header(); 
      self.head_entry = -1
  #    self.head_tree.SetBranchAddress("header",ROOT.addressof(self.head))
      self.head_tree.BuildIndex("header.event_number") 

      self.status_file = ROOT.TFile.Open("%s/run%d/status.root" % (base_dir, run))
      self.status_tree = self.status_file.Get("status") 
  #    self.stat= ROOT.beacon.Status(); 
  #    self.status_tree.SetBranchAddress("status",self.stat) 
      self.status_tree.BuildIndex("status.readout_time","status.readout_time_ns"); 
      self.status_entry =-1; 

      self.current_entry = 0; 
      
      self.times_ns = numpy.linspace(0, (self.event().getBufferLength() - 1)*2, self.event().getBufferLength()) #Should have spacing of 2 ns
    except Exception as e:
      self.failed_setup = True
      print('\nError in %s'%inspect.stack()[0][3])
      print(e)
      exc_type, exc_obj, exc_tb = sys.exc_info()
      fname = os.path.split(exc_tb.tb_frame.f_code.co_filename)[1]
      print(exc_type, fname, exc_tb.tb_lineno)

  def setEntry(self,i): 
    '''
    Sets the current entry to i.  Other functions such as self.wf and self.status will then pull information for the set event.
    
    Parameters
    ----------
    i : int
        The enetry/eventid to set the state of the reader.
    '''
    try:
      if (i < 0 or i >= self.head_tree.GetEntries()):
        sys.stderr.write("Entry out of bounds!") 
      else: 
        self.current_entry = i; 
    except Exception as e:
      print('\nError in %s'%inspect.stack()[0][3])
      print(e)
      exc_type, exc_obj, exc_tb = sys.exc_info()
      fname = os.path.split(exc_tb.tb_frame.f_code.co_filename)[1]
      print(exc_type, fname, exc_tb.tb_lineno)

  def setEvent(self,i):
    '''
    Sets the current entry in the root tree to i.  Other functions such as self.wf and self.status will then pull information for the set event.
    
    Parameters
    ----------
    i : int
        The enetry/eventid to set the state of the reader.
    '''
    try:
      setEntry(self.head_tree.GetEntryNumberWithIndex(i)) 
    except Exception as e:
      print('\nError in %s'%inspect.stack()[0][3])
      print(e)
      exc_type, exc_obj, exc_tb = sys.exc_info()
      fname = os.path.split(exc_tb.tb_frame.f_code.co_filename)[1]
      print(exc_type, fname, exc_tb.tb_lineno)


  def event(self,force_reload = False):
    '''
    Does the required preparations for the event to be loaded.  By default this does nothing if the event is already properly set.
    
    Parameters
    ----------
    force_reload : bool
        Will force this to reset entry info.
    '''
    try:
      if (self.event_entry != self.current_entry or force_reload):
        self.event_tree.GetEntry(self.current_entry)
        self.event_entry = self.current_entry 
        self.evt = getattr(self.event_tree,"event")
      return self.evt 
    except Exception as e:
      print('\nError in %s'%inspect.stack()[0][3])
      print(e)
      exc_type, exc_obj, exc_tb = sys.exc_info()
      fname = os.path.split(exc_tb.tb_frame.f_code.co_filename)[1]
      print(exc_type, fname, exc_tb.tb_lineno)


  def wf(self,ch = 0):  
    '''
    This will pull the waveform data (returned in adu) for the requested channel.  The channel mapping for the
    2019 prototype is: 
    0: NE, Hpol
    1: NE, Vpol
    2: NW, Hpol
    3: NW, Vpol
    4: SE, Hpol
    5: SE, Vpol
    6: SW, Hpol
    7: SW, Vpol
    
    This is subject to change so always cross reference the run with with those in the know to be sure.
    
    Parameters
    ----------
    ch : int
      The channel you specifically want to read out a signal for.
    '''
    ## stupid hack because for some reason it doesn't always report the right buffer length 
    try:
      ev = self.event() 
      v = numpy.copy(numpy.frombuffer(ev.getData(ch), numpy.dtype('float64'), ev.getBufferLength()))
      return v
    except Exception as e:
      print('\nError in %s'%inspect.stack()[0][3])
      print(e)
      exc_type, exc_obj, exc_tb = sys.exc_info()
      fname = os.path.split(exc_tb.tb_frame.f_code.co_filename)[1]
      print(exc_type, fname, exc_tb.tb_lineno)

  def t(self):
    '''
    This will return the timing info for a typical run.  This is predefined to assuming 2ns timing between samples, and is
    calculated rather than measured. 
    '''
    try:
      return self.times_ns
    except Exception as e:
      print('\nError in %s'%inspect.stack()[0][3])
      print(e)
      exc_type, exc_obj, exc_tb = sys.exc_info()
      fname = os.path.split(exc_tb.tb_frame.f_code.co_filename)[1]
      print(exc_type, fname, exc_tb.tb_lineno)

  def header(self,force_reload = False): 
    '''
    This will print out the header info from the root tree.
    
    Parameters
    ----------
    force_reload : bool
      If True, this will ensure the event information is properly set before running the function.
    '''
    try:
      if (self.head_entry != self.current_entry or force_reload): 
        self.head_tree.GetEntry(self.current_entry); 
        self.head_entry = self.current_entry 
        self.head = getattr(self.head_tree,"header")
      return self.head 
    except Exception as e:
      print('\nError in %s'%inspect.stack()[0][3])
      print(e)
      exc_type, exc_obj, exc_tb = sys.exc_info()
      fname = os.path.split(exc_tb.tb_frame.f_code.co_filename)[1]
      print(exc_type, fname, exc_tb.tb_lineno)

  def status(self,force_reload = False): 
    '''
    This will print out the status info from the root tree.
    
    Parameters
    ----------
    force_reload : bool
      If True, this will ensure the event information is properly set before running the function.
    '''
    try:
      if (self.status_entry != self.current_entry or force_reload): 
        self.status_tree.GetEntry(self.status_tree.GetEntryNumberWithBestIndex(self.header().readout_time, self.header().readout_time_ns))
        self.status_entry = self.current_entry
        self.stat = getattr(self.status_tree,"status")

      return self.stat
    except Exception as e:
      print('\nError in %s'%inspect.stack()[0][3])
      print(e)
      exc_type, exc_obj, exc_tb = sys.exc_info()
      fname = os.path.split(exc_tb.tb_frame.f_code.co_filename)[1]
      print(exc_type, fname, exc_tb.tb_lineno)


  def N(self):
    '''
    This will return the number of entries (eventids) in a given run.
    '''
    try:
      return self.head_tree.GetEntries() 
    except Exception as e:
      print('\nError in %s'%inspect.stack()[0][3])
      print(e)
      exc_type, exc_obj, exc_tb = sys.exc_info()
      fname = os.path.split(exc_tb.tb_frame.f_code.co_filename)[1]
      print(exc_type, fname, exc_tb.tb_lineno)
  
  def global_scalers(self):
    ''' 
    Returns the global scalers for a given entry in the status tree
    '''
    # uses a workaround to access the array from cppyy.ll
    try:
      return numpy.frombuffer(cppyy.ll.cast['uint16_t*'](stat.global_scalers), dtype=numpy.uint16, count=3)
    except Exception as e:
      print('\nError in %s'%inspect.stack()[0][3])
      print(e)
      exc_type, exc_obj, exc_tb = sys.exc_info()
      fname = os.path.split(exc_tb.tb_frame.f_code.co_filename)[1]
      print(exc_type, fname, exc_tb.tb_lineno)

  def beam_scalers(self):
    ''' 
    Returns the beam scalers for a given entry in the status tree
    '''
    try:
      return numpy.frombuffer(cppyy.ll.cast['uint16_t*'](self.stat.beam_scalers), dtype=numpy.uint16, count=60).reshape(3,20)
    except Exception as e:
      print('\nError in %s'%inspect.stack()[0][3])
      print(e)
      exc_type, exc_obj, exc_tb = sys.exc_info()
      fname = os.path.split(exc_tb.tb_frame.f_code.co_filename)[1]
      print(exc_type, fname, exc_tb.tb_lineno)

# Plot some waveforms is run as a binary 
if __name__=="__main__": 

## pyplot for plotting
  import matplotlib.pyplot as plt

# If your data is elsewhere, pass it as an argument
  datapath = sys.argv[1] if len(sys.argv) > 1 else '/project2/avieregg/beacon/telem/root/'
  #datapath = sys.argv[1] if len(sys.argv) > 1 else "/data/beacon/root"

# look at run 362 by default, or else pass a run
  d = Reader(datapath,int(sys.argv[2]) if len(sys.argv) > 2 else 1507)#362) #1507 is pulsing run for the 2019-2020 prototype. 
# look at a random event by default, or else pass an event
  d.setEntry(int(sys.argv[3]) if len(sys.argv) > 3 else 18453)#53) #Event from pulsing 

## dump the headers and status, just to show they're there
  d.header().Dump(); 
  d.status().Dump(); 
  #print d.N() 

# plot all waveforms
  for i in range(8): 
    plt.subplot(2,4,i+1); 
    plt.plot(d.t(), d.wf(i))
  
  plt.show() 
