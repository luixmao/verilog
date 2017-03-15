#ifndef GATE_H
#define GATE_H

#include <vector>
#include <string>
#include "net.h"

using namespace std;

// forward declaration
class Net;

enum {AND, OR, NAND, NOR, XOR, NOT};

class Gate {
 public:
  Gate();
  Gate(string n); //constructor, no delay provided
  Gate(string n, int d);  // with delay provided
  virtual ~Gate();
  //virtual char eval() = 0;
  virtual void dump(ostream &os) ; // = 0;
  string name(); //return name
  void addInput(Net *n); //add input wire
  void addOutput(Net *n); //add output wire
  vector<Net *> *getInputs(); //returns input nets
  Net* getOutput(); //return output net pointer
  int getNumInputs(); //returns number of inputs
  int getDelay(); //return delay
  void setDelay(int val); //sets delay, and bool delay default to false 
 protected:
  vector<Net *> *inputs;
  Net *output;
  string inst_name;
  int delay;
  bool delay_default; //if used default delay
};

class And : public Gate {
 public:
  And();
  And(string n, int d);
  And(string n);
  ~And();
  char eval();
  void dump(ostream &os);
};

class Or : public Gate {
 public:
  Or();
  Or(string n, int d);
  Or(string n);
  ~Or();
  char eval();
  void dump(ostream &os);
};

class Nor : public Gate {
 public:
  Nor();
  Nor(string n, int d);
  Nor(string n);
  ~Nor();
  char eval();
  void dump(ostream &os);
};

class Nand : public Gate {
 public:
  Nand();
  Nand(string n, int d);
  Nand(string n);
  ~Nand();
  char eval();
  void dump(ostream &os);
};

class Xor : public Gate {
 public:
  Xor();
  Xor(string n, int d);
  Xor(string n);
  ~Xor();
  char eval();
  void dump(ostream &os);
};

class Not : public Gate {
 public:
  Not();
  Not(string n, int d);
  Not(string n);
  ~Not();
  char eval();
  void dump(ostream &os);
};

#endif
