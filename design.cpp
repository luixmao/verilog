#include <iostream>
#include <ostream>
#include <vector>
#include <map>
#include <string>
#include "gates.h"
#include "net.h"
#include "design.h"

using namespace std;

//constructor
Design::Design(){}

//constructor
Design::Design(string n){
  desname = n;
  PINets = new vector<Net *>(); // " () " used for initializing size to 0 
  PONets = new vector<Net *>();
  WireNets = new vector<Net*>();
  all_Nets = new vector<Net*>();
  all_Gates = new vector<Gate *>();
  //pis.clear();
  //pos.clear();


}

//destructor
Design::~Design(){
  //destructor for all nets
  map<string, Net *>::iterator it_net;
  for(it_net=designNets.begin() ; it_net!=designNets.end(); it_net++){
    //cout << (it_net->second)->name() <<endl;////////////////
    delete it_net->second;
  }

 // destructor for all gates
  map<string, Gate *>::iterator it_gate;
  for(it_gate=designGates.begin() ; it_gate!=designGates.end(); it_gate++){
    //cout << (it_gate->second)->name() <<endl;////////////////
    delete it_gate->second;
  }
  
  //delete vectors
  delete PINets;
  delete PONets;
  delete WireNets;
  delete all_Nets;
  delete all_Gates;
  
}

//returns name of design
string Design::name(){
  return desname;
}

//add input
void Design::addPI(string n){
  pis.push_back(n);
  Net* ptr_net = new Net(n);//make net
  PINets->push_back(ptr_net); 
  designNets[n]=ptr_net;
}

// add output
void Design::addPO(string n){ 
  pos.push_back(n);
  Net* ptr_net = new Net(n);//make net
  PONets->push_back(ptr_net); 
  designNets[n]=ptr_net; 
}

//find net or return NULL
Net* Design::findNet(string net_name){
  map<string, Net *>::iterator it;
  it=designNets.find(net_name);
  if(it!=designNets.end() ){
    return it->second;
  }
  else{
    return NULL;
  }
}

//find or create net
Net* Design::addFindNet(string n){
  map<string, Net *>::iterator it;
  it=designNets.find(n);
  if(it!=designNets.end() ){
    return it->second;
  }
  else{
    Net* net_ptr = new Net(n);
    designNets[n]= net_ptr;
    WireNets->push_back(net_ptr);

    return net_ptr;
  }
}

//finds gate or return NULL
Gate * Design::findGate(string inst_name){
  map<string, Gate*>::iterator it;
  it=designGates.find(inst_name);
  if(it!=designGates.end() ){
    return it->second;
  }
  else{
    return NULL;
  }
}

void Design::dump(ostream &os){
  //module
  os<< "module "<< desname <<"(";
  for(unsigned int i=0; i!=pis.size(); i++){
    os << pis[i] ; 
    if(i!=pis.size()-1 || pos.size()!=0 ){
      os<<",";
    }
  }
  for(unsigned int i=0; i!=pos.size(); i++){
    os << pos[i] ;   
    if(i!=pos.size()-1){
      os<<",";
    }
  }
  os <<"); "<<endl;


  // dump inputs
  for(unsigned int i=0; i!=pis.size(); i++){
    os<< " input " << pis[i] <<";" <<endl; 
  }

  //dump outputs
  for(unsigned int i=0; i!=pos.size(); i++){
    os<< " output " << pos[i] <<";" <<endl; 
  }


  //dump nets
  vector<Net *>* ptr_nets= getWireNets();  
  for(unsigned i =0 ; i!=ptr_nets->size(); i++){
    os << " wire " << ((*ptr_nets)[i])->name() << ';'<<endl;
  }

  //dump gates
  map<string, Gate *>::iterator it_gate;
  for(it_gate=designGates.begin() ; it_gate!=designGates.end(); it_gate++){
    (it_gate->second)->dump(os);
  }

  //dump endmodule
  os<< "endmodule"<<endl;
}


Gate* Design::addFindGate(int gtype, string n){
  Gate* ptr;
  map<string, Gate *>::iterator it_gate;
  it_gate=designGates.find(n);
  if(it_gate!= designGates.end() ){
    return it_gate->second;
  }
  else{
    if(gtype == AND){
      ptr = new And(n);
      designGates[n]=ptr;
    }
    else if(gtype == OR){
      ptr = new Or(n);
      designGates[n]=ptr;
    }
    else if(gtype == NAND){
      ptr = new Nand(n);
      designGates[n]=ptr;
    }
    else if(gtype == NOR){
      ptr = new Nor(n);
      designGates[n]=ptr;
    }
    else if(gtype == XOR){
      ptr = new Xor(n);
      designGates[n]=ptr;
    }
    else if(gtype == NOT){
      ptr = new Not(n);
      designGates[n]=ptr;
    }
    else{
      cout << " error in gate type " <<endl;
      return NULL;
    }
  }
return ptr; // return pointer to new gate
}
Gate* Design::addFindGate(int gtype, string n, int d){
  addFindGate(gtype, n)->setDelay(d);
  return addFindGate(gtype, n);
}


vector<Net *>* Design::getPINets(){
  return PINets;
} 

vector<Net *>* Design::getPONets(){
  return PONets;
} 

vector<Net *> *Design::getWireNets(){
  return WireNets;
}


vector<Net *> * Design::allNets(){
  map<string, Net *>::iterator it_net;
  for(it_net=designNets.begin() ; it_net!=designNets.end(); it_net++){
     all_Nets->push_back( it_net->second );
  }
  return all_Nets;
}


vector<Gate *> * Design::allGates(){
  map<string, Gate *>::iterator it_gate;
  for(it_gate=designGates.begin() ; it_gate!=designGates.end(); it_gate++){
     all_Gates->push_back( it_gate->second );
  }
  return all_Gates;
}


