#ifndef MIDDLE_H
#define MIDDLE_H

#include "TObject.h"

class middleparticle : public TObject {
  public:
     middleparticle();
     int pid;
     int motherid;
     int charge;
     int numberofdaughter;
     double pt;
     double eta;
     double phi;
     double mass;
     void clear () {pid = -999; motherid = -999; charge = -999; numberofdaughter = -999; pt = -999; eta = -999; phi = -999; mass = -999;};
  private:
     ClassDef(middleparticle, 1)
};

//  ClassImp(middleparticle)
#endif
