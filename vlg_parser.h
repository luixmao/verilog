#ifndef VLGPARSER_H
#define VLGPARSER_H

// forward declaration
class Design;

class VlgParser {
 public:
  virtual Design *parse(char *filename);   // =0;

};

#endif
