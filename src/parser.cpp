#include "parser.h"

using namespace std;

extern const autodiff X(1,1,0); 
extern const autodiff C(1,0,0);

parser::parser(string eing){
	if(valid(eing, 1)){
		input = cutspaces(eing);
		term.op = -1;
		term.text = input;
		teile(&term);
	}
	else{
		input = "non valid";
	}
}

Baum parser::get_baum(){
	return term;
}

void parser::set_input(string eingabe){
	input = eingabe;
}

string parser::get_input(){
	return input;
}

bool parser::zeichenval(string raw){
	int length = raw.length();
	bool val = 0;
	for(int i = 0; i < length; i++){
		val = 0;
		for(int j = 0; j < nz; j++){
			if(raw[i] == zeichen[j]){val = 1; break;}
		}
		if(!val){return 0;}
	}
	return val;
}

string parser::cutspaces(string raw){
	int length = raw.length();
	string clean = raw;
	for(int i = 0; i < length; i++){
		if(clean[i]==zeichen[8]){
			clean = clean.substr(0, i) + clean.substr(i + 1, length - i);
			length = clean.length();
			i = -1;
		}
	}
	return clean;
}

bool parser::klglkr(string raw){
	int length=raw.length();
	int kl = 0, kr = 0;
	for(int i = 0; i <= length; i++){
		if(raw[i]==zeichen[0]){kl++;}
		if(raw[i]==zeichen[1]){kr++;}
	}
	if(kl!=kr){return 0;}
	return 1;
}

int parser::klanz(string raw){
	int length=raw.length();
	int kl = 0;
	for(int i = 0; i <= length; i++){
		if(raw[i]==zeichen[0]){kl++;}
	}
	return kl;
}

int parser::opanz(string raw){
	int length=raw.length();
	int ops = 0;
	for(int i = 0; i < length; i++){
		for(int j = 2; j < 7; j++){
			if(raw[i]==zeichen[j]){
				ops++;
			}
		}
		for(int j = 20; j < 25; j++){
			if(raw[i]==zeichen[j]){
				ops++;
			}
		}
	}
	return ops;
}

int parser::opanzokl(string raw){
	int length=raw.length();
	int ops = 0, kl = 0, kr = 0;
	for(int i = 0; i < length; i++){
		if(raw[i] == zeichen[0]){kl++;}
		if(raw[i] == zeichen[1]){kr++;}
		if(kl == kr){
			for(int j = 2; j < 7; j++){
				if(raw[i]==zeichen[j]){
					ops++;
				}
			}
		}
	}
	return ops;
}

bool parser::firstz(string raw){
	for(int i = 1; i < 5; i++){
		if(raw[0] == zeichen[i]){return 0;}
	}
	if(raw[0] == zeichen[7]){return 0;}
	return 1;
}

bool parser::lastz(string raw){
	int length = raw.length();
	for(int i = 2; i < 8; i++){
		if(raw[length - 1] == zeichen[i]){return 0;}
	}
	if(raw[length - 1] == zeichen[0]){return 0;}
	return 1;
}

string parser::doublepm(string raw){
	int len;
	string clean = raw;
	if(raw[0] == zeichen[5]){
		len = clean.length();
		clean = clean.substr(1, len - 1);
	}
	if(raw[0] == zeichen[6]){
		clean = '0' + clean;
	}
	return clean;
}

bool parser::umkl(string raw){
	int length=raw.length();
	int kl = 0, kr = 0;
	if((raw[0]==zeichen[0])&&(raw[length-1]==zeichen[1])){
			kl = 0 ; kr = 0;
			for(int i = 0; i < length; i++){
				if(raw[i]==zeichen[0]){kl++;}
				if(raw[i]==zeichen[1]){kr++;}
				if((kr==kl)&&(i!=length-1)){
					return 0;
				}
				if((kr==kl)&&(i==length-1)){
					return 1;
				}
			}
	}
	return 0;
}

void parser::favop(string raw, int &op, int &pos){
	int length=raw.length();
	int kl = 0, kr = 0;
	op = 0;
	pos = 0;
	for(int k = length-1; k >= 0 ; k--){
		if(raw[k] == zeichen[1]){kl++;}
		if(raw[k] == zeichen[0]){kr++;}
		if(kl == kr){
			for(int j = 2; j < 7; j++){
				if((raw[k] == zeichen[j])&&(op < j)&&(raw[k-1]!='+')&&(raw[k-1]!='-')){
					op = j;
					pos = k;
				}
				if((raw[k] == '*')&&((raw[k+1] == '+')||(raw[k+1] == '-'))){
						op = 3;
						pos = k;
						k = -1;
						break;
				}
				else if((raw[k] == '/')&&((raw[k+1] == '+')||(raw[k+1] == '-'))){
					op = 4;
					pos = k;
					k = -1;
					break;
				}
				else if((raw[k] == '^')&&((raw[k+1] == '+')||(raw[k+1] == '-'))){
					op = 2;
					pos = k;
					k = -1;
					break;
				}
			}
		}
	}
}

string parser::cutumkl(string raw){
	int length = raw.length();
	string subraw = raw.substr(1, length - 2);
	return subraw;
}

bool parser::numvalid(string raw){
	int pcount = 0, len = raw.length();
	bool val;
	for(int i = 0; i < len; i++){
		val = 0;
		if(raw[i] == '.'){
			pcount++;
			val = 1;
		}
		else{
			for(int j = 9; j < 19; j++){
				if(raw[i] == zeichen[j]){val = 1;}
			}
		}
		if(pcount > 1){return 0;}
		if(val == 0){return 0;}
	}
	return val;
}

bool parser::valid(string raw, bool erster_drchlf){
	int length = raw.length();
	int ops = 0;
	
	cout << "valid called, raw:" << raw << endl;
	
	if(length == 0){return 0;}
	
	if(!firstz(raw)){return 0;}
	
	if(!lastz(raw)){return 0;}
	
	if(erster_drchlf){
		if(!zeichenval(raw)){return 0;}
		
		raw = cutspaces(raw);
		length = raw.length();		
	}
	
	if(!klglkr(raw)){return 0;}
	
	if(umkl(raw)){
		string subraw = raw, links ,rechts;
		subraw = cutumkl(subraw);
		
		cout << "cutumkl subraw: " << subraw << endl;
		
		if(valid(subraw, 0)){return 1;}
		return 0;
	}
	
	ops = opanzokl(raw);
	
	int opabs = 0;
	opabs = opanz(raw);
	if(opabs == 0){
		cout << "numvalid: " << numvalid(raw) << endl;
		if((length == 1)&&(raw == "x")){return 1;}
		else if(numvalid(raw)){return 1;}
		else return 0;
	}
	
	if(ops > 0){
		if((raw[0]==zeichen[5])||(raw[0]==zeichen[6])){
			string subraw = raw;
			
			subraw = doublepm(subraw);
			
			if(!valid(subraw, 0)){return 0;}
		}
		
		else{
			string subraw = raw, links ,rechts;
			
			int fav_op = 0, fav_pos = 0;
			favop(subraw, fav_op, fav_pos);
						
			if(fav_op == 0){return 0;}
			
			links = subraw.substr(0,fav_pos);
			rechts = subraw.substr(fav_pos+1,length-fav_pos);
			if(!valid(links, 0)){return 0;}
			if(!valid(rechts, 0)){return 0;}
		}
	}
	
	else if(ops == 0){
		
		if(length > 4){
			if(raw[0] == 'e'){
				if(raw[1]!='x'||raw[2]!='p'){return 0;}
				string subraw = raw.substr(4, length - 4 - 1);
				if(!valid(subraw, 0)){return 0;}
			}
			else if(raw[0] == 's'){
				if(raw[3]=='h'){
					if(raw[1]!='i'||raw[2]!='n'){return 0;}
					string subraw = raw.substr(5, length - 5 - 1);
					if(!valid(subraw, 0)){return 0;}
				}
				else{
					if(raw[1]!='i'||raw[2]!='n'){return 0;}
					string subraw = raw.substr(4, length - 4 - 1);
					if(!valid(subraw, 0)){return 0;}
				}
			}
			else if(raw[0] == 'c'){
				if(raw[3]=='h'){
					if(raw[2]=='t'){
						if(raw[1]!='o'){return 0;}
						string subraw = raw.substr(5, length - 5 - 1);
						if(!valid(subraw, 0)){return 0;}
					}
					else{
						if(raw[1]!='o'||raw[2]!='s'){return 0;}
						string subraw = raw.substr(5, length - 5 - 1);
						if(!valid(subraw, 0)){return 0;}
					}
				}
				else if(raw[2]=='t'){
					if(raw[1]!='o'){return 0;}
					string subraw = raw.substr(4, length - 4 - 1);
					if(!valid(subraw, 0)){return 0;}
				}
				else{
					if(raw[1]!='o'||raw[2]!='s'){return 0;}
					string subraw = raw.substr(4, length - 4 - 1);
					if(!valid(subraw, 0)){return 0;}
				}
			}
			else if(raw[0] == 't'){
				if(raw[3]=='h'){
					if(raw[1]!='a'||raw[2]!='n'){return 0;}
					string subraw = raw.substr(5, length - 5 - 1);
					if(!valid(subraw, 0)){return 0;}
				}
				else{
					if(raw[1]!='a'||raw[2]!='n'){return 0;}
					string subraw = raw.substr(4, length - 4 - 1);
					if(!valid(subraw, 0)){return 0;}
				}
			}
			else if(raw[0] == 'l'){
				if(raw[1]!='n'){return 0;}
				string subraw = raw.substr(3, length - 3 - 1);
				if(!valid(subraw, 0)){return 0;}
			}
		}
		else if((length == 1)&&(raw == "e")){return 1;}
		else if(!numvalid(raw)){return 0;}
		else if(klanz(raw) > 0){return 0;}
	}
	
	return 1;
}

string parser::entklammern(string raw){
	string clear = raw, backup = raw;
	int length = clear.length();
	if((clear[0] == zeichen[0])&&(clear[length-1] == zeichen[1])){
		while(valid(clear, 1)){
			backup = clear;
			clear = clear.substr(1, length - 2);
			length = clear.length();
		}
		return backup;
	}
	return clear;
}

void parser::teile(Baum *eingabe){
	
	string str = eingabe->text;
	
	cout << str << endl;
	
	str = entklammern(str);
	
	int len = str.length();
	
	int ops = 0;
	
	ops = opanzokl(str);
	
	if(ops == 0){
		
		if((str.substr(0,4) == "exp(")){
			eingabe->op = 20;
			string substr = str.substr(4, len - 4 - 1);
			eingabe->text = substr;
			
			eingabe->r = new Baum;
			eingabe->l = 0;
			
			eingabe->r->text = substr;
			teile(eingabe->r);
		}
		else if(str.substr(0,4) == "sin("){
			eingabe->op = 21;
			string substr = str.substr(4, len - 4 - 1);
			
			eingabe->r = new Baum;
			eingabe->l = 0;
			
			eingabe->r->text = substr;
			teile(eingabe->r);
		}
		else if(str.substr(0,5) == "sinh("){
			eingabe->op = 25;
			string substr = str.substr(5, len - 5 - 1);
			
			eingabe->r = new Baum;
			eingabe->l = 0;
			
			eingabe->r->text = substr;
			teile(eingabe->r);
		}
		else if(str.substr(0,4) == "cos("){
			eingabe->op = 22;
			string substr = str.substr(4, len - 4 - 1);
			eingabe->text = substr;
			
			eingabe->r = new Baum;
			eingabe->l = 0;
			
			eingabe->r->text = substr;
			teile(eingabe->r);
		}
		else if(str.substr(0,5) == "cosh("){
			eingabe->op = 26;
			string substr = str.substr(5, len - 5 - 1);
			eingabe->text = substr;
			
			eingabe->r = new Baum;
			eingabe->l = 0;
			
			eingabe->r->text = substr;
			teile(eingabe->r);
		}
		else if(str.substr(0,4) == "tan("){
			eingabe->op = 24;
			string substr = str.substr(4, len - 4 - 1);
			eingabe->text = substr;
			
			eingabe->r = new Baum;
			eingabe->l = 0;
			
			eingabe->r->text = substr;
			teile(eingabe->r);
		}
		else if(str.substr(0,5) == "tanh("){
			eingabe->op = 27;
			string substr = str.substr(5, len - 5 - 1);
			eingabe->text = substr;
			
			eingabe->r = new Baum;
			eingabe->l = 0;
			
			eingabe->r->text = substr;
			teile(eingabe->r);
		}
		else if(str.substr(0,4) == "cot("){
			eingabe->op = 28;
			string substr = str.substr(4, len - 4 - 1);
			eingabe->text = substr;
			
			eingabe->r = new Baum;
			eingabe->l = 0;
			
			eingabe->r->text = substr;
			teile(eingabe->r);
		}
		else if(str.substr(0,5) == "coth("){
			eingabe->op = 29;
			string substr = str.substr(5, len - 5 - 1);
			eingabe->text = substr;
			
			eingabe->r = new Baum;
			eingabe->l = 0;
			
			eingabe->r->text = substr;
			teile(eingabe->r);
		}
		else if(str.substr(0,3) == "ln("){
			eingabe->op = 23;
			string substr = str.substr(3, len - 3 - 1);
			eingabe->text = substr;
			
			eingabe->r = new Baum;
			eingabe->l = 0;
			
			eingabe->r->text = substr;
			teile(eingabe->r);
		}
		else{
			eingabe->r = 0;
			eingabe->l = 0;
			eingabe->text = str;
		}
		
	}
	else{
		int fav_pos = 0; int fav_art = 0;
		
		favop(str, fav_art, fav_pos);
		
		eingabe->op = fav_art;
		
		eingabe->r = new Baum;
		eingabe->l = new Baum;
		
		eingabe->l->text = str.substr(0, fav_pos);		
		eingabe->r->text = str.substr(fav_pos + 1, len - fav_pos);
		cout << eingabe->l->text << " / " << eingabe->r->text << endl;
		if(eingabe->l->text == ""){eingabe->l->text = "0";}
		
		teile(eingabe->r);
		teile(eingabe->l);	
	}
}

autodiff parser::derive(double x, Baum term){
	if ((term.l == 0)&&(term.r == 0)){	
		//Ableitung von der Variable x wird eingesetzt	
		if(term.text == "x"){
			autodiff res = X;
			res.set_u(x);
			return res;
		}
		//Konstante e wird korrekt erkannt
		if(term.text == "e"){
			return exp(1)*C;
		}
		//keiner der oberen Fälle ist eingetreten => Konstante multipliziert mit einem Vielfachen wird zurückgegeben
		return atof(term.text.c_str())*C;		
	}
	else{
		switch(term.op){
			//Je nach Operator werden die benötigten für autodiff überladenen Freundfunktionen angewendet.
			case 2:
				return pow(derive(x, *term.l),derive(x,*term.r));
			case 3:				
				return derive(x,*term.l)*derive(x,*term.r);
			case 4:				
				return derive(x,*term.l)/derive(x,*term.r);
			case 5:				
				return derive(x,*term.l)-derive(x,*term.r);
			case 6:				
				return derive(x,*term.l)+derive(x,*term.r);
			case 20:						
				return exp(derive(x,*term.r));
			case 21:						
				return sin(derive(x,*term.r));
			case 22:						
				return cos(derive(x,*term.r));
			case 23:						
				return ln(derive(x,*term.r));
			case 24:						
				return tan(derive(x,*term.r));
			case 25:						
				return sinh(derive(x,*term.r));
			case 26:						
				return cosh(derive(x,*term.r));
			case 27:						
				return tanh(derive(x,*term.r));	
			case 28:						
				return cot(derive(x,*term.r));
			case 29:						
				return coth(derive(x,*term.r));			
		}		
	}
}

taylor parser::tayl(int k,double x, Baum term){
	if ((term.l == 0)&&(term.r == 0)){
		
		// Instanz von taylor einer Variable mit Wert x wird erstellt		
		if(term.text == "x"){
			taylor tx('x',x,k);
			return tx;
		}
		
		// Konstante e wird korrekt erkannt
		if(term.text == "e"){
			taylor tc('c',exp(1),k);

			return tc;
		}
		
		// ansonsten wird Instanz einer Konstanten mit dem Wert in term.text (welcher mit atof(..) in double umgewandelt wurde) zurückgegeben
		taylor tc('c', atof(term.text.c_str()), k);
		return tc;		
	}
	else{
		cout << term.text << "/opart: " << zeichen[term.op] << endl;
		switch(term.op){
		// Je nach Operator werden die benötigten für taylor überladenen Freundfunktionen angewendet.
			case 2:
				return pow(tayl(k,x,*term.l), tayl(k,x,*term.r));
			case 3:				
				return tayl(k,x,*term.l)*tayl(k,x,*term.r);
			case 4:				
				return tayl(k,x,*term.l)/tayl(k,x,*term.r);
			case 5:				
				return tayl(k,x,*term.l)-tayl(k,x,*term.r);
			case 6:		
				return tayl(k,x,*term.l)+tayl(k,x,*term.r);
			case 20:						
				return exp(tayl(k,x,*term.r));
			case 21:						
				return sin(tayl(k,x,*term.r));
			case 22:						
				return cos(tayl(k,x,*term.r));
			case 23:						
				return ln(tayl(k,x,*term.r));
			case 24:						
				return tan(tayl(k,x,*term.r));
			case 25:						
				return sinh(tayl(k,x,*term.r));
			case 26:						
				return cosh(tayl(k,x,*term.r));
			case 27:						
				return tanh(tayl(k,x,*term.r));
			case 28:						
				return cot(tayl(k,x,*term.r));
			case 29:						
				return coth(tayl(k,x,*term.r));
			
		}
		
	}
}
