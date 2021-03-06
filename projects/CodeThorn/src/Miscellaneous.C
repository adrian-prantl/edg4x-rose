/*************************************************************
 * Copyright: (C) 2012 by Markus Schordan                    *
 * Author   : Markus Schordan                                *
 * License  : see file LICENSE in the CodeThorn distribution *
 *************************************************************/

#include "Miscellaneous.h"
#include "CommandLineOptions.h"
#include <cctype>

void CodeThorn::write_file(std::string filename, std::string data) {
  std::ofstream myfile;
  myfile.open(filename.c_str(),std::ios::out);
  myfile << data;
  myfile.close();
}

string CodeThorn::int_to_string(int x) {
  stringstream ss;
  ss << x;
  return ss.str();
}

string CodeThorn::color(string name) {
  if(!boolOptions["colors"]) 
	return "";
  string c="\33[";
  if(name=="normal") return c+"0m";
  if(name=="bold") return c+"1m";
  if(name=="bold-off") return c+"22m";
  if(name=="blink") return c+"5m";
  if(name=="blink-off") return c+"25m";
  if(name=="underline") return c+"4m";
  if(name=="default-text-color") return c+"39m";
  if(name=="default-bg-color") return c+"49m";
  bool bgcolor=false;
  string prefix="bg-";
  size_t pos=name.find(prefix);
  if(pos==0) {
	bgcolor=true;
	name=name.substr(prefix.size(),name.size()-prefix.size());
  }
  string colors[]={"black","red","green","yellow","blue","magenta","cyan","white"};
  int i;
  for(i=0;i<8;i++) {
	if(name==colors[i]) {
	  break;
	}
  }
  if(i<8) {
	if(bgcolor)
	  return c+"4"+int_to_string(i)+"m";
	else
	  return c+"3"+int_to_string(i)+"m";
  }
  else
	throw "Error: unknown color code.";
}


/* returns true if the string w can be parsed on stream is.
   otherwise false.
   returns true for an empty string w.
   istream remains unmodified if string s cannot be parsed.
 */
bool
CodeThorn::Parse::checkWord(string w,istream& is) {
  size_t i;
  for(i=0;i<w.size();i++) {
	if(is.peek()==w[i]) {
	  is.get();
	} else {
	  break;
	}
  }
  // ensure that the word is followed either by anychar or some char not in [a-zA-Z]
  if(i==w.size() && !std::isalpha(is.peek())) {
	return true;
  }
  if(i==0) return false;
  --i; // was peeked
  // putback all chars that were read
  while(i>0) {
	is.putback(w[i--]);
  }
  is.putback(w[0]); // note: i is unsigned
  return false;
}

/* Consumes input from istream if the string w can be parsed
   otherwise generates an error message and throws exception.
   Returns without performing any action for an empty string w.
*/
void
CodeThorn::Parse::parseString(string w,istream& is) {
  size_t i;
  char c;
  for(i=0;i<w.size();i++) {
	if(is.peek()==w[i]) {
	  is >> c;
	} else {
	  break;
	}
  }
  // check that string was sucessfully parsed
  if(i==w.size()) {
	return;
  } else {
	cerr<< "Error: parsing of \""<<w<<"\" failed."<<endl;
	string s;
	is>>s;
	cerr<< "Parsed "<<i<<"characters. Remaining input: "<<s<<"..."<<endl;
	throw "Parser Error.";
  }
}

bool
CodeThorn::Parse::integer(istream& is, int& num) {
  if(std::isdigit(is.peek())) {
	is>>num;
	return true;
  } else {
	return false;
  }
}

int
CodeThorn::Parse::spaces(istream& is) {
  int num=0;
  while(is.peek()==' ') {
	is.get();
	num++;
  }
  return num;
}

int
CodeThorn::Parse::whitespaces(istream& is) {
  int num=0;
  while(std::isspace(is.peek())) {
	is.get();
	num++;
  }
  return num;
}
