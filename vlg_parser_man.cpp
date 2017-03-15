#include <iostream>
#include <ostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cctype>
#include <map>
#include <string>
#include <cstdlib>
#include "gates.h"
#include "net.h"
#include "design.h"
#include "vlg_parser_auto.h"
#include "vlg_parser.h"


//prototypes
string remove_spaces(string s); //remove all spaces, no matter where
string errase_comments_and_spaces(string s); //erase comments and repeated spaces, leaving no contiguous spaces
bool has_proper_Semicolon(string s); //check if it has one semicolon, and that it is at the end
bool isID(string s); //check if it is a valid name
string remove_side_spaces(string s); //erase spaces that are at the begining or at the end
vector<string>* return_words(string line_of_text); //returns a pointer to a new vector containg all the words of a line of text
bool isDelay(string s); //check if it is a valid delat
bool isNum(string s); //check if it is a number
bool has_proper_parentesis(string s); //check if it has proper parentesis
int find_gate_type(string s); //return the value depending of the type of gate it is


int next_line_no;

Design* VlgParser::parse(char *filename) {


  //take text into ifile
  ifstream ifile(filename);

  //initializers
  Design* ptr_des1; //needs to be inialized later    // = new Design(designName);
  int next_line_no=1;//line number 
  bool hasModule=false; //whether it has module
  bool hasEndmodule=false; //whether it has endmodule

  //check that the first thing is module
  while ( !ifile.eof() ){
    string line_of_text;
    //get a line of text
    getline(ifile, line_of_text);
    //increase to the next line
    next_line_no++;
    //clean up, erase comments and repeated spaces
    line_of_text= errase_comments_and_spaces(line_of_text);

    stringstream ss(line_of_text);
    string token;
    ss >>token;
          //check that first thing is module
          if(token!="module" && token!=" "  && !token.empty()){
	      cout << " error: line " << next_line_no-1 << ": need module   " <<endl;
	      return NULL;
          } 
          //if module, set name
          else if (token=="module"){
            hasModule=true;
            string designName;
            getline(ss , designName, '(' );
            designName= remove_side_spaces( designName);//might contain side spaces, so remove them
            if(! (isID(designName)) ){
		 cout << " error: line " << next_line_no-1 << ": not a valid name   " <<endl; 
                 return NULL;
            }
            ptr_des1= new Design(designName);

            //check inside the parentesis
               //check proper close parenthesis ')'
               string pos_parentesis;
               getline(ss , pos_parentesis );
               if(!has_proper_parentesis(pos_parentesis) ){
	  	  cout << " error: line " << next_line_no-1 << ": not proper parenthesis   " <<endl; 
                  delete ptr_des1;
                  return NULL;
               }

               //check for validity inside parentesis
               stringstream temp(pos_parentesis);
               string inside_parentesis; 
               getline(temp , inside_parentesis ,')'); 
               inside_parentesis=remove_side_spaces(inside_parentesis);
 
               if (inside_parentesis[inside_parentesis.length()-1] == ','){//error if last item is ',' 
                  cout << " error: line " << next_line_no-1 << ":  additional ','  " <<endl; 
                  delete ptr_des1;
                  return NULL;
               }
               //check parentesis are right
               stringstream temp2(inside_parentesis);
               string name_id;
               int count_elements=0;
               while( temp2.good() ){ //while there are inputs
                 count_elements++;
                 getline(temp2,name_id, ',');
                 name_id=remove_side_spaces(name_id);
                 //check valid id
                 if( !isID(name_id)){
	  	   cout << " error: line " << next_line_no-1 << ": not valid ID, or issue with ','    " <<endl;
                   delete ptr_des1; 
                   return NULL;
                 }
               }
               //if not enough elements display error
 	       if(count_elements<2){
	  	   cout << " error: line " << next_line_no-1 << ": needs at least one input and one output ','   " <<endl; 
                   delete ptr_des1;
                   return NULL;
               }
            //end check inside parentesis
            break;
          }
  }
  //if didn't find module, error
  if(!hasModule){
     cout << " error: missing module declaration" <<endl;
     return NULL;
  }

     // cout << " error: line " << next_line_no-1 << ": not a valid name   " <<endl;


  //run while until there is no more text
  while ( !ifile.eof() ){
    string line_of_text;
    //get a line of text
    getline(ifile, line_of_text);
    //increase to the next line
    next_line_no++;
    //clean up, erase comments and repeated spaces
    line_of_text= errase_comments_and_spaces(line_of_text);

    stringstream ss(line_of_text);
    string token;
    ss >>token;
    if(ss.fail() || (token.length()==1 && token[0]==' ')){ //if no text in this line
      continue;
    }
    else{
       //if it has semicolon
       if(!has_proper_Semicolon(remove_spaces(line_of_text)) && token!="endmodule"){//has one ';' and it is last item 
          cout << " error: line " << next_line_no-1 << ":  improper ';'  " <<endl;
          delete ptr_des1;
          return NULL;
       }
       else{
          if( token=="endmodule" ){
             hasEndmodule=true;
             //check if there is anything after endmodule
             while ( !ifile.eof() ){
      	        getline(ifile, line_of_text);
  	        //increase to the next line
  	        next_line_no++;
  	        //clean up, erase comments and repeated spaces
  	        line_of_text= errase_comments_and_spaces(line_of_text);
   	        for(unsigned int i=0 ; i!=line_of_text.length();i++){
    	           if(line_of_text!=" "){
    	              cout << " error: line " << next_line_no-1 << ": nothing allowed after endmodule   " <<endl;
                      delete ptr_des1;
     	              return NULL;
     	           }
   	        }

             }
          }     
          //add input if it is possible ID
          else if( token=="input" ){
            string input_name;
 	    getline(ss , input_name, ';' );
            input_name= remove_side_spaces( input_name);//might contain spaces on sides, so remove them
            if(! (isID(input_name)) ){
		 cout << " error: line " << next_line_no-1 << ": not a valid name   " <<endl; 
                 delete ptr_des1;
                 return NULL;
            }
   	    ptr_des1->addPI(input_name); //add new input
          }     
          //add output if it is possible ID
          else if( token=="output" ){
            string output_name;
 	    getline(ss , output_name, ';' );
            output_name= remove_side_spaces( output_name);//might contain spaces on sides, so remove them
            if(! (isID(output_name)) ){
		 cout << " error: line " << next_line_no-1 << ": not a valid name   " <<endl; 
                 delete ptr_des1;
                 return NULL;
            }
   	    ptr_des1->addPO(output_name); //add new input
          }     

          //add wire if it is possible ID
          else if( token=="wire" ){
            string wire_name;
 	    getline(ss , wire_name, ';' );
            wire_name= remove_side_spaces( wire_name); //might contain spaces on sides, so remove them
            if(! (isID(wire_name)) ){
		 cout << " error: line " << next_line_no-1 << ": not a valid name   " <<endl; 
                 delete ptr_des1;
                 return NULL;
            }
   	    ptr_des1->addFindNet(wire_name); //add new input
          }     
          else if(token == "and" || token == "or" || token == "nand"|| token == "nor"|| token == "xor"|| token == "not"){
             int gate_type =  find_gate_type(token);
	     string pre_parentesis; //rest of the line after "and" declaration
             getline(ss , pre_parentesis, '(' );
             vector<string>* ptr_vec =return_words(pre_parentesis);
 	     int num_elements=ptr_vec->size();
             if(num_elements >2){
                 cout << " error: line " << next_line_no-1 << ": too many entries before '('   " <<endl; 
                 delete ptr_vec;
                 delete ptr_des1;
                 return NULL;
             }
             else if(num_elements ==1 || num_elements ==2){
               string gate_name;//gate name
               if(num_elements ==1 ){
                 gate_name =(*ptr_vec)[0];
                 if(! (isID(gate_name)) ){
  	  	    cout << " error: line " << next_line_no-1 << ": not a valid gate name   " <<endl; 
                    delete ptr_vec;
                    delete ptr_des1;
                    return NULL;
                 }
                 ptr_des1->addFindGate(gate_type, gate_name);
               }
               else{ //if(num_elements ==2)
                 //check proper name
                 gate_name =(*ptr_vec)[1];
                 if(! (isID(gate_name)) ){
	    	    cout << " error: line " << next_line_no-1 << ": not a valid gate name   " <<endl; 
                    delete ptr_vec;
                    delete ptr_des1;
                    return NULL;
                 }
                  
                 //check proper delay
                 string delay_str =(*ptr_vec)[0];// delay with
                 if(! (isDelay(delay_str)) ){
	  	    cout << " error: line " << next_line_no-1 << ": not a valid delay   " <<endl; 
                    delete ptr_vec;
                    delete ptr_des1;
                    return NULL;
                 }
                 delay_str.erase(delay_str.begin());
                 int delay = atoi( delay_str.c_str() );
                 ptr_des1->addFindGate(gate_type, gate_name)->setDelay(delay);
               }



               //check proper close parenthesis ')'

               string pos_parentesis;
               getline(ss , pos_parentesis );
               if(!has_proper_parentesis(pos_parentesis) ){
	  	  cout << " error: line " << next_line_no-1 << ": not proper parenthesis   " <<endl; 
                  delete ptr_vec;
                  delete ptr_des1;
                  return NULL;
               }

               //check for validity inside parentesis
               stringstream temp(pos_parentesis);
               string inside_parentesis; 
               getline(temp , inside_parentesis ,')'); 
               inside_parentesis=remove_side_spaces(inside_parentesis);
 
               if (inside_parentesis[inside_parentesis.length()-1] == ','){//error if last item is ',' 
                  cout << " error: line " << next_line_no-1 << ": not additional ','  " <<endl; 
                  delete ptr_vec;
                  delete ptr_des1;
                  return NULL;
               }
               
               stringstream temp2(inside_parentesis);
               string name_id;
               bool isOutput = true;
               int count_elements=0;
           
               while( temp2.good() ){ //while there are inputs, get the names and make new nets if valid name
                 count_elements++;
                 getline(temp2,name_id, ',');
                 name_id=remove_side_spaces(name_id);
                 //check valid id
                 if( !isID(name_id)){
	  	   cout << " error: line " << next_line_no-1 << ": not valid ID or missing ','   " <<endl;
                   delete ptr_vec;
                   delete ptr_des1; 
                   return NULL;
                 }
                 if(isOutput){ //first item is output
                   //add output
                   ptr_des1->addFindGate(gate_type, gate_name)->addOutput(ptr_des1->addFindNet(name_id));
		   isOutput=false;//to not enter again
                 }
                 else{
                   //add inputs
                   ptr_des1->addFindGate(gate_type, gate_name)->addInput(ptr_des1->addFindNet(name_id));
                 }
               }
               //error if elements is not enough
 	       if(count_elements<2){
	  	   cout << " error: line " << next_line_no-1 << ": needs at least one input and one output    " <<endl;
                   delete ptr_vec; 
                   delete ptr_des1;
                   return NULL;
               }

             }
             else{  
                  cout << " error: line " << next_line_no-1 << ": expected gate name   " <<endl; 
                  delete ptr_vec;
                  delete ptr_des1;
                  return NULL;
             }
           
             delete ptr_vec;
          }

          else{ //if it is not one of the expected declarations
    	     cout << " error: line " << next_line_no-1 << ": not a valid declaration   " <<endl;
             delete ptr_des1;
     	     return NULL;
          }
         
      }
    }

  }


  //if didn't find module, error
  if(!hasEndmodule){
     cout << " error: missing endmodule " <<endl;
     delete ptr_des1;
     return NULL;
  }


 
  ifile.close();
  return ptr_des1;
}


//////////////////////////////////////////

//takes input line of text, and returns several strings that were separated by spaces in the input string
vector<string>* return_words(string line_of_text){ //note that this will not have additional spaces given that line of text dont have contiguous spaces
  vector<string>* words=new vector<string>();
  stringstream ss (line_of_text);
  string word;
  while (ss >> word){
//cout<<word<<endl;
    words->push_back(word);
  }
  return words;
}

//erase comments and repeated spaces, leaving no contiguous spaces
string errase_comments_and_spaces(string s){
  bool exit = false;
  unsigned int i=0;
  while(i<s.length()){
    //delete everything after comments
    if(s[i]=='/'){
        if(s[i+1]=='/'){// if there is two adjacent "//"
          while (i!=s.length() ){ //delete all the values starting from the one located at i
            s.erase(s.begin()+i); 
            exit=true; //to exit loop
          }
          if(exit){
            break;
          }
        }
    }
    //delete repeated spaces
    if(s[i] == ' '){
       while(s[i+1] == ' ' && (i+1)<s.length() ){
         s.erase(s.begin()+i+1); 
       }
    }
    
    i++;
  }
  return s;
}


 //check if it has one semicolon, and that it is at the end
bool has_proper_Semicolon(string s){
    s=remove_side_spaces( s);
    string str2 (";");
    unsigned found = s.find(str2);
    if (found==(s.length()-1)){
      return true;
    }

return false;
}
//check if it is a valid name
bool isID(string s){
   if( !isalpha(s[0]) && s[0]!='_'){
      return false;
   }
   for( unsigned int i =1 ; i<s.length();i++){
     if( !isalnum(s[i]) && s[i]!='_' ){
       return false;
     }
   }
   
return true;
}


//remove all spaces, no matter where
string remove_spaces(string s){
  unsigned int i=0;
  while(i!=s.length()){
    if(s[i] == ' '){
       s.erase(s.begin()+i); 
    }
    else{
       i++;
    }
  }
  return s;
}

//erase spaces that are at the begining or at the end
string remove_side_spaces(string s){
  while(s[0] == ' '){
    s.erase(s.begin()); 
  }
  unsigned int i=s.length()-1;
  while(s[i] == ' '){
    s.erase(s.begin()+i); 
  }
  return s;
}

//check if it is a number
bool isNum(string s){
  remove_side_spaces(s);
  for( unsigned int i =0 ; i<s.length();i++){
     if(!isdigit(s[i])){
       return false;
     }
  }
  return true;
}

//check if it is a valid delay
bool isDelay(string s){
  remove_side_spaces(s);
  if(s[0]!='#'){
     return false;
  }
  else{
     s.erase(s.begin() );
     return isNum(s);
  }

}

//check if it has proper parentesis
bool has_proper_parentesis(string s){
    s=remove_spaces( s); //removes all spaces

    //check there is only one '(' and that it is in the second to last position
    string str2 (")");
    unsigned found = s.find(str2);
    if (found!=(s.length()-2)){
      cout<< " error: improper ')' "<<endl;
      return false;
    }
   
    //check there is not aditional ;
    string str3 ("(");
    unsigned found3 = s.find(str3);
    if (found3!=string::npos){
      cout<< " error: improper '(' "<<endl;
      return false;
    }

return true;
}
//return the value depending of the type of gate it is
int find_gate_type(string s){
  if (s=="and"){ return AND; }
  if (s=="or"){ return OR; }
  if (s=="nand"){ return NAND; }
  if (s=="nor"){ return NOR; }
  if (s=="xor"){ return XOR; }
  if (s=="not"){ return NOT; }
return 0;
}


