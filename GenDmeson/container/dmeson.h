#ifndef Dmeson_H
#define Dmeson_H

#include <vector>
#include "daughterparticle.h"
#include "motherquark.h"
#include "middleparticle.h"


//using std::vector;
using namespace std;


class dmeson : public TObject {
  public:
     dmeson();
     int pid;
     int motherid;
     int charge;
     int numberofdaughter;
     double pt;
     double eta;
     double phi;
     double mass;
     vector<daughterparticle> daughters;
     vector<motherquark>  momquark;
     vector<middleparticle> middle;
     void clear (){pid = -999; motherid = -999; charge = -999; numberofdaughter = -999; pt = -999; eta = -999; phi = -999; mass = -999; daughters.clear();
                 momquark.clear();middle.clear();};
     
  private:
     ClassDef(dmeson, 1)
};

  //ClassImp(dmeson)
#endif
