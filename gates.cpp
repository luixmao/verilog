#include <iostream>
#include <vector>
#include <string>
#include "net.h"
#include "gates.h"
using namespace std;


//constructors
Gate::Gate(){ }


Gate::Gate(string n){ //no delay
  delay = 1;
  inst_name = n;
  inputs = new vector<Net *>;
  delay_default=true;
  output = NULL;
}


Gate::Gate(string n, int d){  // with delay provided
  inst_name = n;
  delay = d;
  inputs = new vector<Net *>;
  delay_default=false;
  output = NULL;
}
Gate::~Gate(){
  delete inputs;
}

//return name
string Gate::name(){
  return inst_name;
}

//return delay
int Gate::getDelay(){
  return delay;
}

//sets delay, and bool delay default to false 
void Gate::setDelay(int val){
  delay=val;
  delay_default=false;
}



//get output
Net* Gate::getOutput(){
  return output;
}

//add input wire
void Gate::addInput(Net *n){
  inputs->push_back(n);
}

//set output wire
void Gate::addOutput(Net *n){
  output = n;
}

//returns input nets
vector<Net *> * Gate::getInputs(){
  return inputs;
}

//returns number of inputs
int Gate::getNumInputs(){
  return inputs->size();
}

//dump
void Gate::dump(ostream &os) {

  if(!delay_default){
    os << "#" << delay <<" "; 
  }
  os << inst_name <<'(';
  if(output!=NULL){
    os << output->name();
  }
  for(unsigned int i=0; i!=inputs->size(); i++){
    os << ", " << ( (*inputs)[i]->name() );
  }
  os << ");" <<endl ;
}


/////////////// AND GATE ////////////////////

And::And(){}
And::And(string n, int d): Gate(n,d){  }
And::And(string n): Gate(n){  }
And::~And(){}

void And::dump(ostream &os){
  os << " and ";
  Gate::dump(os);
}
char And::eval(){
  unsigned int count1=0;
  for(unsigned i = 0; i!=inputs->size() ; i++){
    char value_in =(*inputs)[i]->getVal() ;
    if(value_in=='0'){
      return '0';
    }
    if(value_in=='1'){
      count1++;
    }
  }
  if(count1==inputs->size()){ 
    return '1';
  }
//if they is no '0' and there is at least one 'x'
return 'x';
}


/////////////// OR GATE ////////////////////

Or::Or(){}
Or::Or(string n, int d): Gate(n,d){  }
Or::Or(string n): Gate(n){  }
Or::~Or(){}
void Or::dump(ostream &os){
  os << " or ";
  Gate::dump(os);
}

char Or::eval(){
  unsigned int count0=0;
  for(unsigned i = 0; i!=inputs->size() ; i++){
    char value_in =(*inputs)[i]->getVal() ;
    if(value_in=='1'){
      return '1';
    }
    if(value_in=='0'){
      count0++;
    }
  }

  if(count0==inputs->size()){ 
    return '0';
  }
//if they is no '1' and there is at least 'x'
return 'x';
}


/////////////// NOR GATE ////////////////////

Nor::Nor(){}
Nor::Nor(string n, int d): Gate(n,d){  }
Nor::Nor(string n): Gate(n){  }
Nor::~Nor(){}
void Nor::dump(ostream &os){
  os << " nor ";
  Gate::dump(os);
}
char Nor::eval(){
  unsigned int count0=0;
  for(unsigned i = 0; i!=inputs->size() ; i++){
    char value_in =(*inputs)[i]->getVal() ;
    if(value_in=='1'){
      return '0';//'1';
    }
    if(value_in=='0'){
      count0++;
    }
  }

  if(count0==inputs->size()){ 
    return '1';//'0';
  }
//if they is no '1' and there is one 'x'
return 'x';
}


/////////////// NAND GATE ////////////////////

Nand::Nand(){}
Nand::Nand(string n, int d): Gate(n,d){  }
Nand::Nand(string n): Gate(n){  }
Nand::~Nand(){}
void Nand::dump(ostream &os){
  os << " nand ";
  Gate::dump(os);
}

char Nand::eval(){
  unsigned int count1=0;
  for(unsigned i = 0; i!=inputs->size() ; i++){
    char value_in =(*inputs)[i]->getVal() ;
    if(value_in=='0'){
      return '1';//'0';
    }
    if(value_in=='1'){
      count1++;
    }
  }

  if(count1==inputs->size()){ 
    return '0';//'1';
  }
//if they is no '0' and there is at least one 'x'
return 'x';
}




/////////////// XOR GATE ////////////////////

Xor::Xor(){}
Xor::Xor(string n, int d): Gate(n,d){  }
Xor::Xor(string n): Gate(n){  }
Xor::~Xor(){}
void Xor::dump(ostream &os){
  os << " xor ";
  Gate::dump(os);
}

char Xor::eval(){ //odd number of '1' gives true
  bool unknown=false;
  unsigned int count1=0;
  for(unsigned i = 0; i!=inputs->size() ; i++){
    char value_in =(*inputs)[i]->getVal() ;
    if(value_in=='x'){
      unknown=true;
    }
    if(value_in=='0'){
      count1++;
    }
  }
  
 
  bool isOdd=count1%2;
  if( isOdd ){ //if it is odd number
    return '1';
  }
  if(unknown){
    return 'x';
  }


return '0';
}

/////////////// NOT GATE ////////////////////

Not::Not(){}
Not::Not(string n, int d): Gate(n,d){  }
Not::Not(string n): Gate(n){  }
Not::~Not(){}
void Not::dump(ostream &os){
  os << " not ";
  Gate::dump(os);
}
char Not::eval(){
  char value_in =(*inputs)[0]->getVal() ;

  if(value_in=='1'){
    return '0';
  }
  if(value_in=='0'){
    return '1';
  }

return 'x';
}










