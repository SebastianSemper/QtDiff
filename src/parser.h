#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <math.h>
#include <string>
#include <stdlib.h>
#include "autodiff.h"
#include "taylor.h"
#include <sstream>

using namespace std;

const int nz=31;
const char zeichen[nz]={'(',')','^','*','/','-','+','.',' ','0','1','2','3','4','5','6','7','8','9','x','e','s','c','l','t','o','i','n','p','a','h'};
	// anz:		 1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19  20  21  22  23  24  25  26  27  28  29  30  31
	// array:	 0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19  20  21  22  23  24  25  26  27  28  29  30
	
struct Baum
{
	int op;
	string text;
	Baum *l;
	Baum *r;
};

class parser{
	private:
		string input;
		Baum term;
		bool zeichenval(string);
		string cutspaces(string);
		bool klglkr(string);
		int klanz(string);
		int opanz(string);
		int opanzokl(string);
		bool firstz(string);
		bool lastz(string);
		string doublepm(string);
		bool umkl(string);	
		void favop(string, int&, int&);
		string cutumkl(string);
		bool numvalid(string);
		
	public:
		//Schnittstellenfunktionen
		Baum get_baum();
		void set_input(string);
		string get_input();
		
		bool valid(string, bool);
		bool validalt(string, bool);
		string entklammern(string);
		
		void teile(Baum *);
		double calc(double, Baum);
		autodiff derive(double, Baum);
		taylor tayl(int, double, Baum);
		
		
		parser(){}
		parser(string);
		
};

#endif //PARSER_H
