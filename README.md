ROOT wrappers for beacon data classes 
---

Requires ROOT (only tested with ROOT 6). If you want to read in raw data, that hasn't already been rootified, you also need libbeacon (see below). 

In most cases, you just need to: 

 - set BEACON_INSTALL_DIR to something sensible  (e.g., in bash,  export BEACON_INSTALL_DIR=~/beacon) 
 - make
 - make install  

 This will install the beaconroot libraries into that directory. It is then
 probably a good idea to add $beacon_INSTALL_DIR/lib to your LD_LIBRARY_PATH
 (or DYLD_LIBRARY_PATH on a mac, although never tested on a mac). 


There is an example of how to use python to read in rootified data under examples. 


---

If you need to convert raw data, you should configure (make configure or
ccmake) and enable LIBBEACON_SUPPORT. If libbeacon is in BEACON_INSTALL_DIR
you can leave LIBBEACON_PATH empty, otherwise you should set it properly. 








