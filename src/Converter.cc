#include <stdio.h> 
#include "beaconConverter.h" 
#include "beaconEvent.h" 
#include "beaconHeader.h" 
#include "beaconStatus.h" 
#include "beaconHk.h" 
#include "TTree.h" 
#include "TFile.h" 

static bool allow_overwrite = false; 
void beacon::convert::setAllowOverwrite(bool allow) { allow_overwrite = allow; } 


#ifdef HAVE_LIBBEACON
#include "beacon.h" 

#include <vector> 
#include <zlib.h>
#include <dirent.h> 
#include <sys/types.h> 
#include <sys/stat.h> 
#include <unistd.h> 
#include <algorithm> 

template <class RootType, class RawType, int(*RawReadFunction)(gzFile,RawType*)>
static int convert_impl(int nfiles, const char ** infiles, const char * outfile, const char * treename) 
{
  int nprocessed = 0; 

  TFile of(outfile, allow_overwrite ? "RECREATE" : "CREATE"); 
  of.SetCompressionSettings(105); 
  TTree * tree = new TTree(treename,treename); 
  tree->SetAutoSave(0); 
  RootType * ptr = new RootType; 
  RawType raw; 
  tree->Branch(treename,&ptr); 
  for (int i = 0; i < nfiles; i++) 
  {
    gzFile f = gzopen(infiles[i],"r"); 
    while(!RawReadFunction(f,&raw))
    {
      nprocessed++; 
      new (ptr) RootType(&raw); 
      tree->Fill(); 
    }
    gzclose(f); 
  }
  of.Write(); 

  return nprocessed;
}

template <class RootType, class RawType, int(*RawReadFunction)(gzFile,RawType*)>
static int convertDir(const char * dir, const char * outfile, const char * treename)
{
  std::vector<std::string>  files; 
  std::vector<const char * > file_ptrs; 


  DIR * dirp = opendir(dir); 
   
  if (!dirp) 
  {
    fprintf(stderr,"Could not open dir %s\n", dir); 
    return 0; 
  }
  struct dirent * dent; 
  
  while ((dent = readdir(dirp)))
  {
    TString str; 
    if (dent->d_name[0] == '.') continue; 
    str.Form("%s/%s",dir,dent->d_name);
    files.push_back(std::string(str.Data())); //since I know std::sort works for strings 
  }

  closedir(dirp); 

  std::sort(files.begin(), files.end()); 

  file_ptrs.reserve(files.size()); 

  for (unsigned i = 0; i < files.size(); i++) 
  {
    file_ptrs.push_back(files[i].c_str()); 
  }

  int nprocessed = convert_impl<RootType,RawType,RawReadFunction>(file_ptrs.size(),&file_ptrs[0], outfile,treename); 

  return nprocessed; 
}


int beacon::convert::convertHkFiles(int nfiles, const char ** infiles, const char * outfile) 
{
  return convert_impl<Hk, beacon_hk, beacon_hk_gzread>(nfiles, infiles, outfile,"hk"); 
}

int beacon::convert::convertHeaderFiles(int nfiles, const char ** infiles, const char * outfile) 
{
  return convert_impl<Header, beacon_header,beacon_header_gzread> (nfiles, infiles, outfile,"header"); 
}

int beacon::convert::convertEventFiles(int nfiles, const char ** infiles, const char * outfile) 
{
  return convert_impl<Event, beacon_event,beacon_event_gzread> (nfiles, infiles, outfile,"event"); 
}

int beacon::convert::convertStatusFiles(int nfiles, const char ** infiles, const char * outfile) 
{
  return convert_impl<Status, beacon_status,beacon_status_gzread> (nfiles, infiles, outfile,"status"); 
}


int beacon::convert::convertHkDir(const char * dir, const char * outfile) 
{
  return convertDir<Hk, beacon_hk,beacon_hk_gzread> (dir, outfile,"hk"); 
}

int beacon::convert::convertHeaderDir(const char * dir, const char * outfile) 
{
  return convertDir<Header, beacon_header,beacon_header_gzread> (dir, outfile,"header"); 
}

int beacon::convert::convertEventDir(const char * dir, const char * outfile) 
{
  return convertDir<Event, beacon_event,beacon_event_gzread> (dir, outfile,"event"); 
}

int beacon::convert::convertStatusDir(const char * dir, const char * outfile) 
{
  return convertDir<Status, beacon_status,beacon_status_gzread> (dir, outfile,"status"); 
}


int beacon::convert::convertRun(const char * base_raw, const char * base_root, int run) 
{

  TString outf; 
  TString inf;

  // make output directory (may already exist) 
  outf.Form("%s/run%d", base_root, run); 
  if (access(outf.Data(), W_OK))
  {
    if (!mkdir(outf.Data(),0755))
    {
      fprintf(stderr,"Problem creating %s\n",outf.Data()); 
      return 1; 
    }
  }

  outf.Form("%s/run%d/status%d.root", base_root, run, run); 
  inf.Form("%s/run%d/status/", base_raw, run); 
  convertStatusDir(inf.Data(), outf.Data()); 

  outf.Form("%s/run%d/event%d.root", base_root, run, run); 
  inf.Form("%s/run%d/event/", base_raw, run); 
  convertEventDir(inf.Data(), outf.Data()); 

  outf.Form("%s/run%d/header%d.root", base_root, run, run); 
  inf.Form("%s/run%d/header/", base_raw, run); 
  convertHeaderDir(inf.Data(), outf.Data()); 
  return 0; 

} 


#else

int beacon::convert::convertHkFiles(int nfiles, const char ** infiles, const char * outfile) 
{
  (void) nfiles; 
  (void) infiles; 
  (void) outfile; 
  fprintf(stderr,"%s requires HAVE_LIBBEACON\n",__func__); 
  return -1; 
}

int beacon::convert::convertHeaderFiles(int nfiles, const char ** infiles, const char * outfile) 
{

  (void) nfiles; 
  (void) infiles; 
  (void) outfile; 
  fprintf(stderr,"%s requires HAVE_LIBBEACON\n",__func__); 
  return -1; 
}

int beacon::convert::convertEventFiles(int nfiles, const char ** infiles, const char * outfile) 
{
  (void) nfiles; 
  (void) infiles; 
  (void) outfile; 
  fprintf(stderr,"%s requires HAVE_LIBBEACON\n",__func__); 
  return -1; 
}

int beacon::convert::convertStatusFiles(int nfiles, const char ** infiles, const char * outfile) 
{
  (void) nfiles; 
  (void) infiles; 
  (void) outfile; 
  fprintf(stderr,"%s requires HAVE_LIBBEACON\n",__func__); 
  return -1; 
}


int beacon::convert::convertHkDir(const char * dir, const char * outfile) 
{
  (void) dir; 
  (void) outfile; 
  fprintf(stderr,"%s requires HAVE_LIBBEACON\n",__func__); 
  return -1; 
}

int beacon::convert::convertHeaderDir(const char * dir, const char * outfile) 
{

  (void) dir; 
  (void) outfile; 
  fprintf(stderr,"%s requires HAVE_LIBBEACON\n",__func__); 
  return -1; 
}

int beacon::convert::convertEventDir(const char * dir, const char * outfile) 
{
  (void) dir; 
  (void) outfile; 
  fprintf(stderr,"%s requires HAVE_LIBBEACON\n",__func__); 
  return -1; 
}

int beacon::convert::convertStatusDir(const char * dir, const char * outfile) 
{
  (void) dir; 
  (void) outfile; 
  fprintf(stderr,"%s requires HAVE_LIBBEACON\n",__func__); 
  return -1; 
}


int beacon::convert::convertRun(const char * base_raw, const char * base_root, int run) 
{
  (void) base_raw; 
  (void) base_root; 
  (void) run; 
  fprintf(stderr,"%s requires HAVE_LIBBEACON\n",__func__); 
  return -1; 

} 


#endif

int beacon::convert::makeFilteredHeadTree(const char * filtered_event_file, const char * full_head_file, const char * filtered_head_file) 
{

  TFile fef(filtered_event_file); 
  TTree * eventTree  = (TTree*) fef.Get("event") ; 
  TFile fhf(full_head_file); 
  TTree * headTree = (TTree*) fhf.Get("header"); 

  if (!eventTree || !headTree) return 1; 
  headTree->BuildIndex("event_number"); 



  TFile of(filtered_head_file, allow_overwrite ? "RECREATE":"CREATE"); 
  TTree * filteredTree = new TTree("header","filtered header files"); 

  Header * hd = 0; 
  Event * ev = 0; 

  headTree->SetBranchAddress("header",&hd); 
  eventTree->SetBranchAddress("event",&ev); 
  filteredTree->Branch("header",&hd); 

  for (int i = 0; i < eventTree->GetEntries(); i++) 
  {
    eventTree->GetEntry(i); 
    headTree->GetEntry(ev->getEventNumber() % 1000000000-1); 
//    printf("%lld %lld\n", ev->getEventNumber(), hd->event_number); 
    of.cd(); 
    filteredTree->Fill(); 
  }


  of.Write(); 
  return 0; 
}

//
