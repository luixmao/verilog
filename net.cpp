#include <iostream>
#include <vector>
#include <string>
#include "net.h"
using namespace std;

Net::Net(){}
Net::Net(string n){
  netname = n;
  val ='x';
  drivers = new vector<Gate *>;
  loads = new vector<Gate *>;
}

Net::~Net(){
  delete drivers;
  delete loads;
}

string Net::name(){
  return netname;
}

//adds driver
void Net::addDriver(Gate *g){
  drivers->push_back(g);
}

//adds load
void Net::addLoad(Gate *g){
  loads->push_back(g);
}


vector<Gate *> * Net::getLoads(){
  return loads;
}

vector<Gate *> * Net::getDrivers(){
  return drivers;
}


//print out drivers and loads
void Net::printDriversLoads(){
  cout << " net " << name();

  cout << ", drivers: ";
  for(unsigned int i=0; i!=drivers->size();i++){
    cout << (*drivers)[i];
  }

  cout << "  loads: ";
  for(unsigned int i=0; i!=loads->size();i++){
    cout << (*loads)[i];
  }
  cout<< endl;
}







