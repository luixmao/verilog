#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <algorithm>

#include "vlg_parser_auto.h"
#include "design.h"
#include "net.h"
#include "gates.h"

using namespace std;

extern Design* current_design;

VlgParserAuto::VlgParserAuto()
{

}

Design *VlgParserAuto::parse(char *filename)
{
  if(read_verilog(filename)){
    Design *d = current_design;
    //d->dump(cout);
    return d;
  }
  return NULL;
}
