#ifndef VLGPARSERAUTO_H
#define VLGPARSERAUTO_H

#include "vlg_parser.h"
#include "design.h"

// prototype for the FLEX/BISON parser
int read_verilog(const char *fn);

class VlgParserAuto : public VlgParser {
 public:
  VlgParserAuto();
  Design *parse(char *filename);

};

#endif
