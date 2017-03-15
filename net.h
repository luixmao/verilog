#ifndef NET_H
#define NET_H
#include <vector>
#include <string>
using namespace std;

class Gate;

class Net {
 public:
  Net(); // constructor
  Net(string n); //constructor
  ~Net(); //destructor
  void addDriver(Gate *g); //adds driver
  void addLoad(Gate *g); //adds load
  vector<Gate *> *getLoads(); //returns pointers to vector of loads
  vector<Gate *> *getDrivers(); //returns pointers to vector of drivers
  string name(); // returns name
  void setVal(char v){ val = v; }; //sets val

  char getVal(){ return val; }; // returns val
  void printDriversLoads();  //print out drivers and loads
  void setDelay(int d) { delay = d; } //set delay
  int getDelay() { return delay; } //returns delay
  int computeDelay();
  char computeVal(); 

 private:
  vector<Gate *> *drivers;
  vector<Gate *> *loads;
  char val;
  string netname;
  int delay;
};
#endif
