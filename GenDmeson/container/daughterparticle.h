#ifndef DAUGHTER_H
#define DAUGHTER_H

#include "TObject.h"

class daughterparticle : public TObject {
  public:
     daughterparticle();
     int pid;
     int motherid;
     int charge;
     double pt;
     double eta;
     double phi;
     double mass;
     void clear () {pid = -999; motherid = -999; charge = -999; pt = -999; eta = -999; phi = -999; mass = -999;};
  private:
     ClassDef(daughterparticle, 1)
};

//  ClassImp(daughterparticle)
#endif
