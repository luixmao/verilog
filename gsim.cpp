#include <iostream>
#include <ostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>
#include "gates.h"
#include "net.h"
#include "design.h"
#include "vlg_parser_auto.h"

using namespace std;

//prototypes
//string remove_spaces(string s);
//string errase_repeated_spaces(string s);



int main(int argc, char* argv[]){

  if(argc < 2){ //use "argc < 3" for pa6
    cerr << "Usage: ./gatesim verilog_file input_test_file" << endl;
    return 1;
  }

/*************** autoparsing **************
//note: to use the autoparsing, need to change "parse" to a pure virtual fuction, in vlg_parser.h, namely:
//  virtual Design *parse(char *filename) =0;

  VlgParserAuto auto_;
  Design* design2 = auto_.parse(argv[1]);
  ofstream output_stream("autoparsing_result.v");

  design2->dump(output_stream);
  output_stream.close();
*******************************************/

  //self parsing
  VlgParser pars;
  Design* design1= pars.parse(argv[1]);
  ofstream ofile("output.v") ; 
  
  //print results if no errors
  if(design1!=NULL){
    cout<< " parser correct " <<endl;
    design1->dump(ofile);
    delete design1;
  }
  else{ //if error, report
    cout<< " Parser reports error " <<endl;
  }


  ofile.close();

  return 0;
}


