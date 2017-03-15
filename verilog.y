%{
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include "gates.h"
#include "design.h"
#include "net.h"
using namespace std;

 extern int yyerror(const char *s);
 extern int yylex(void);
%}

%union {
  int ival;
  char *sval;
  Gate *gval;
  Design *dval;
};
%token NUMBER ID BINARY MODULE ENDMODULE INPUT OUTPUT WIRE GAND GOR GNAND GNOR GXOR GNOT
%type <ival> NUMBER gate_type GAND GOR GNAND GNOR GXOR GNOT
%type <sval> ID
%type <dval> design design_decl
%type <gval> gate_inst 


%{
  extern int yylineno;
  extern char *yytext;
  Design *current_design = NULL;
  Gate *current_gate = NULL;
  Net *current_net = NULL;
%}

%%
design : design_decl  port_decl ';' body_list ENDMODULE { $$ = $1; }
;

design_decl : MODULE ID { $$ = new Design($2);
         current_design = $$; //cout << "Module " << $2 << endl; 
       }
;

port_decl : 
     '(' port_list ')'
;
port_list :  ID { /*out << "port_list single ID " << $1 << endl;*/ }
 | port_list ',' ID { /*cout << "port_list, ID " << $3 << endl; */}
;

body_list : body_item ';'
    | body_list body_item ';'
;
body_item : 
      INPUT ID  { current_design->addPI($2); 
	current_design->addFindNet($2); }
    | OUTPUT ID  { current_design->addPO($2);
        current_design->addFindNet($2); }
                         
    | WIRE ID { current_design->addFindNet($2); 
                         }
;
    | gate_inst net_decl  {

                         }
net_decl : '(' net_list ')' 
   ;
net_list :  ID { current_net = current_design->addFindNet($1);  
	       // cout << "** adding first net with name " << $1 << endl;
                          current_gate->addOutput(current_net);
			  current_net->addDriver(current_gate);
                        }
| net_list ',' ID { current_net = current_design->addFindNet($3); 
	           //cout << "** adding other net with name " << $3 << endl;
                          current_gate->addInput(current_net);
			  current_net->addLoad(current_gate);
                        }
   ;

gate_inst:  gate_type '#' NUMBER ID  { 
     current_gate = current_design->addFindGate($1-GAND,$4,$3);             
     $$ = current_gate;
   }
  | gate_type ID  { current_gate = current_design->addFindGate($1-GAND,$2);
     $$ = current_gate;
   }
;

gate_type : 
            GAND { $$ = GAND; } 
          | GOR  { $$ = GOR; } 
          | GNAND { $$ = GNAND; } 
          | GNOR { $$ = GNOR; } 
          | GXOR { $$ = GXOR; } 
          | GNOT { $$ = GNOT; } 
   ;

%%


int yyerror(const char *s)
{
  
  cerr << "ERROR: " << s << " at symbol \"" << yytext;
  cerr << "\" on line " << yylineno << endl;
  return 1;
}

int 
read_verilog(const char *fn)
{
  extern FILE *yyin;
  int ret;
  extern int yylineno;
  yylineno = 1;

  yyin=fopen(fn,"r");
  if(yyin==NULL){
     cerr << "Cannot open " << fn << endl;
     return 0;
  }
  yylineno=1;  
  if(yyparse()==0){
     
     cerr << "\nParsing OK!\n";
     ret=1;
  }else{
     cerr << "\nSyntax Problem!\n";
     ret=0;
  }
  fclose(yyin);
  return ret;
}

#ifdef PARSE_ONLY
int
main(int argc,char **argv)
{
  //  int i;
  //extern FILE *vlog_in;
  //yydebug = 1;
  if(read_verilog(argv[1])){
    Design *d = current_design;
    d->dump(cout);
    return 0;
  }
  return 1;
}

#endif
