#ifndef DESIGN_H
#define DESIGN_H
#include <ostream>
#include <vector>
#include <map>
#include <string>
#include "gates.h"
#include "net.h"

using namespace std;

class Design {
 public:
  Design();
  Design(string n); // constructor
  ~Design();
  string name(); // return name of design
  void addPI(string n); //add input
  void addPO(string n); // add output
  Net *findNet(string net_name); //returns pointer to a existing net, or NULL
  Gate *findGate(string inst_name); //returns pointer to a existing gate, or NULL
  Net* addFindNet(string n);  //return existing Net pointer or creates new net, and returns pointer to it
  Gate* addFindGate(int gtype, string n); // returns pointer to existing gate or newly created
  Gate* addFindGate(int gtype, string n, int d); //same as above but with set delay
  vector<Net *> *getPINets();  //returns pointer to vector with all pin nets
  vector<Net *> *getPONets();  //returns pointer to vector with all pout nets
  vector<Net *> *getWireNets(); //returns pointer to internal nets
  vector<Net *> *allNets();   // returns a new pointer to a vector of all nets
  vector<Gate *> *allGates(); //returns a new pointer to a vector of all gates
  vector<Net *> *toposort();
  void dump(ostream &os);
  bool isPI(Net *n);
  bool isPO(Net *n);
 private:
  void dfs_visit(Net *n, vector<Net *> *flist, map<string, int> &colormap);
  string desname;
  map<string, Net *> designNets;
  map<string, Gate *> designGates;
  vector<string> pis;
  vector<string> pos;
  vector<Net *>* PINets;   
  vector<Net *>* PONets;
  vector<Net *>* WireNets;
  vector<Net *>* all_Nets;
  vector<Gate *> *all_Gates;

};

#endif
