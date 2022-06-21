#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <iostream>
#include <iomanip>
#include <fstream>

#include "MeshSim.h"
#include "MeshSimAdapt.h"
#include "SimMeshTools.h"
#include "SimParasolidKrnl.h"
#include "SimAdvMeshing.h"
#include "SimMeshMove.h"
#include "SimModel.h"
#include "SimUtil.h"
#include "SimDiscrete.h"
#include "SimDiscreteErrorCodes.h"

#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <queue>

using std::cout;
using std::endl;
using std::cerr;
int main(int argc, char *argv[]){

  Sim_logOn("simLog_reparametrizeForDiscrete.dat");	
  pNativeModel nmodel = 0;
  pGModel model = 0;
  pMesh mesh;     // declare the original mesh object


  char sim_lic_file[256];
  char *sim_lic_file_env = 0;
  sim_lic_file_env = getenv("SIM_LICENSE_FILE");
  if(sim_lic_file_env) {
    cout << "SIM_LICENSE_FILE is set : " << sim_lic_file_env << endl;
    sprintf(sim_lic_file,sim_lic_file_env);
    Sim_readLicenseFile(sim_lic_file);
  }
  else {
    Sim_readLicenseFile("/net/common/meshSim/license/license.txt");
  }
  MS_init();      // initialize the MeshSim
  SimAdvMeshing_start();
  char geom_file[1024];
  strcpy(geom_file,argv[1]);
  char model_file[1024];
  strcpy(model_file,argv[2]);
  char mesh_file[1024];
  strcpy(mesh_file,argv[3]);

  cout<<endl;
  cout<<" Reading ... "<<endl;
  cout<<"  Geometry from file: "<<geom_file<<endl;
  cout<<"  Model from file : "<<model_file<<endl;
  cout<<"  Mesh from file  : "<<mesh_file<<endl;
  cout<<endl;

  int modeler = -1;
  char ext[8];
  strcpy(ext,geom_file+(strlen(model_file)-4));

  SimDiscrete_start(0);
  SimParasolid_start(1);
  pProgress prog;
  prog = Progress_new();

  model=GM_load(model_file,nmodel,prog);

  if(!model) {
    cerr << " ERROR : Didn't load a model, check that code was compiled with the correct MODELER specified and that the model file has an extension such as .xmt_txt, or .XMT_TXT, or .x_t" << endl << endl;
    return 1;
  }
  mesh = M_load(mesh_file, model, prog);
  NM_release(nmodel);

  cout<<endl;
  cout<<" Reading model and mesh done ..."<<endl;
  cout<<endl;

  void  MS_reparameterizeForDiscrete(pMesh);
  MS_reparameterizeForDiscrete(mesh);

  char fname[80];
  GM_write(M_model(mesh), "model_new.smd", 0, prog);
  cout<< "Model written" << endl;
  M_write(mesh, "mesh_reparameterized.sms", 0, prog);
  cout<< "Done writing"<<endl;

  GM_release(model);
  M_release(mesh);	
  Progress_delete(prog);
  Sim_unregisterAllKeys();
  SimParasolid_stop(1);
  SimAdvMeshing_stop();
  SimDiscrete_stop(0);
  Sim_logOff();
  return 0;





}
