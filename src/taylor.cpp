#include "taylor.h"

using namespace std;

/**
 * Default-konstruktor
 */
taylor::taylor(){
	k = 0;
	ak = 0;
}

/**
 * Konstruktor erstellt Instanz mit j Taylorkoeffizienten
 */
taylor::taylor(int j){
	k = j;
	ak = new double[k];
}

/**
 * Konstruktor erstellt Instanz mit j Taylorkoeffizienten und gegebenem Koeffizientenarray
 */
taylor::taylor(int j, double *koeff){
	k = j;
	ak = new double[j];
	ak = koeff;
}

/**
 * Konstruktor erstellt für m gleich 'c' ein Koeffizientenarray der Länge j für eine Konstante mit Wert w
 * oder für m gleich 'x' ein Koeffizientenarray der Länge j für eine Variable mit Wert w
 */
taylor::taylor(char m, double w, int j){
	if(m=='c'){
		k = j;
		ak = new double[k];
		ak[0] = w;
		if(k > 0){			
			for(int i = 1; i < k; i++){
				ak[i] = 0;
			}
		}
	}
	else if(m=='x'){
		k = j;
		ak = new double[k];
		ak[0] = w;
		if(k > 0){
			ak[1] = 1.0;
			if(k > 1){
				for(int i = 2; i < k; i++){
					ak[i] = 0;
				}
			}
		}
	}
	else{
	k = 0;
	ak = 0;
	}
}

/**
 * Schnittstellenfunktionen
 * 
 * set_xkoef() udn set_ckoef() setzten aehnlich wie obiger Konstruktor das Koeffizientenarray auf das einer Konstanten bzw. Variablen
 */

void taylor::set_xkoef(double w, int j){
	k = j;
	ak = new double[k];

	ak[0] = w;
	if(k > 0){
		ak[1] = 1.0;
		if(k > 1){
			for(int i = 2; i < k; i++){
				ak[i] = 0;
			}
		}
	}
}

void taylor::set_ckoef(double w, int j){
	k = j;
	ak = new double[k];
	ak[0] = w;
	if(k > 0){			
		for(int i = 1; i < k; i++){
			ak[i] = 0;
		}
	}
}

/**
 * akout() gibt einen Zeiger auf das Koeffizientenarray zurueck
 */
double* taylor::akout(){
	return ak;
}

double taylor::xval(double x, double t0, int k){
	double x0 = x - t0, tval = ak[k];
	for(int i = k-1; i >= 0; i--){
		tval *= x0;
		tval += ak[i];
	}
	return tval;
}

/**
 * Kopier-Konstruktor, Zuweisungsoperatoren und Destruktor
 */

taylor::taylor(const taylor &a){    
	k = a.k;
	ak = new double[k];
	if( k > 0){
		for(int i = 0; i < k; i++){
			ak[i] = a.ak[i];
		}
	}
}

taylor &taylor::operator =(const taylor &a){
	k = a.k;
	ak = new double[k];
	if( k > 0){
		for(int i = 0; i < k; i++){
			ak[i] = a.ak[i];
		}
	}
	else{
		ak = 0;
	}
	return *this;
}

// = operator kann wie obiger Konstruktor bzw. Methoden das Koeffizientenarray auf das einer Konstanten setzen
taylor &taylor::operator =(double w){
        if(k > 0){
                ak[0] = w;
                if(k > 0){
                        for(int i = 1; i < k; i++){
                                ak[i] = 0;
                        }
                }
        return *this;
        }
}

taylor::~taylor(){
	if(ak != 0) delete[] ak;
}

/**
 * Freundfunktionen fuer ueberladene Operatoren und mathematische Funktionen
 */

taylor operator + (taylor a, taylor b){
	taylor res;
	res.k = a.k;
	res.ak = new double[res.k];
	for(int i = 0; i < res.k; i++){
		res.ak[i] = a.ak[i] + b.ak[i];
	}
	return res;
}

/**
 * unaeres Minus
 */
taylor operator -(taylor a){
	taylor res;
	res.k = a.k;
	res.ak = new double[res.k];
	for(int i = 0; i < res.k; i++){
		res.ak[i] = -a.ak[i];
	}
	return res;
}

taylor operator - (taylor a, taylor b){
	taylor res;
	res.k = a.k;
	res.ak = new double[res.k];
	for(int i = 0; i < res.k; i++){
		res.ak[i] = a.ak[i] - b.ak[i];
	}
	return res;
}

taylor operator * (taylor a, taylor b){
	taylor res;
	res.k = a.k;
	res.ak = new double[res.k];
	for(int i = 0; i < res.k; i++){
		res.ak[i]=0;
		for(int j = 0; j <= i; j++){
			res.ak[i] += (a.ak[i-j] * b.ak[j]);
		}
	}
	return res;
}

taylor operator / (taylor a, taylor b){
	taylor res;
	res.k = a.k;
	res.ak = new double[res.k];
	for(int i = 0; i < res.k; i++){
		res.ak[i] = a.ak[i];
		for(int j = 1; j <= i; j++){
			res.ak[i] -= (b.ak[j] * res.ak[i-j]);
		}
		res.ak[i] /= b.ak[0];
	}
	return res;
}

/**
 * Sinus und Cosinus werden jeweils parallel berechnet
 */
taylor sin(taylor a){
	taylor sico[2];
	sico[0].k=a.k;
	sico[1].k=a.k;
	sico[0].ak = new double[sico[0].k];
	sico[1].ak = new double[sico[1].k];
	
	sico[0].ak[0] = sin(a.ak[0]);
	sico[1].ak[0] = cos(a.ak[0]);
	
	for(int k = 1; k < a.k; k++){
		
		sico[0].ak[k] = 0;
		sico[1].ak[k] = 0;
		
		for(int j = 0; j < k; j++){
			sico[0].ak[k] += (j+1) * sico[1].ak[k-j-1] * a.ak[j+1];
			sico[1].ak[k] += (j+1) * sico[0].ak[k-j-1] * a.ak[j+1];
		}
		
		sico[0].ak[k] /= k;
		cout << "bam: " << sico[0].ak[k] << endl;
		sico[1].ak[k] /= -k;
		cout << "bam2: " << sico[1].ak[k] << endl;
	}
	
	return sico[0];
}

taylor cos(taylor a){
	taylor sico[2];
	sico[0].k=a.k;
	sico[1].k=a.k;
	sico[0].ak = new double[sico[0].k];
	sico[1].ak = new double[sico[1].k];
	
	sico[0].ak[0] = sin(a.ak[0]);
	sico[1].ak[0] = cos(a.ak[0]);
	
	for(int k = 1; k < a.k; k++){
		
		sico[0].ak[k] = 0;
		sico[1].ak[k] = 0;
		
		for(int j = 0; j < k; j++){
			sico[0].ak[k] += (j+1) * sico[1].ak[k-j-1] * a.ak[j+1];
			sico[1].ak[k] += (j+1) * sico[0].ak[k-j-1] * a.ak[j+1];
		}
		
		sico[0].ak[k] /= k;
		cout << "bam: " << sico[0].ak[k] << endl;
		sico[1].ak[k] /= -k;
		cout << "bam2: " << sico[1].ak[k] << endl;
	}
	
	return sico[1];
}

taylor exp(taylor a){
	taylor res;
	res.k = a.k;
	res.ak = new double[res.k];
	res.ak[0] = exp(a.ak[0]);
	for(int k = 1; k < res.k; k++){
		res.ak[k] = 0;
		for(int j = 0; j < k; j++){
			res.ak[k] += (k-j) * res.ak[j] * a.ak[k-j];
		}
		res.ak[k] /= k;
	}
	return res;
}

taylor ln(taylor a){
	taylor res;
	res.k = a.k;
	res.ak = new double[res.k];
	res.ak[0] = log(a.ak[0]);
	for(int k = 1; k < res.k; k++){
		res.ak[k] = a.ak[k];
		for(int j = 1; j < k; j++){
			res.ak[k] -= (1-double(j)/double(k)) * a.ak[j] * res.ak[k-j];
		}
		res.ak[k] /= a.ak[0];
	} 
	return res;
}

/**
 * tan(), cosh(), sinh(), tanh(), cot() und coth() koennen nun mithilfe obiger Funktionen definiert werden
 */
taylor tan(taylor a){
	return sin(a)/cos(a);
}

taylor cosh(taylor a){
	taylor c('c',0.5,a.k);
	return c*(exp(a)+exp(-a));
}

taylor sinh(taylor a){
	taylor c('c',0.5,a.k);
	return c*(exp(a)-exp(-a));
}

taylor tanh(taylor a){
	return sinh(a)/cosh(a);
}

taylor cot(taylor a){
	return cos(a)/sin(a);
}

taylor coth(taylor a){
	return cosh(a)/sinh(a);
}

/**
 * pow() fungiert als fast funktionierede Potenzfunktion
 */
taylor pow(taylor u, taylor v){	
	taylor res;
	res.k = u.k;
	res.ak = new double[res.k];
	if (u.ak[0] == 0){
		res.set_ckoef(0, res.k);
		return res;
	}
	else if (v.ak[0] == 0){
		res.set_ckoef(1, res.k);
		return res;
	}
	else if (u.ak[0] == exp(1)){
		return exp(v);
	}
	else if(int(v.ak[0]*10.0) == int(v.ak[0])*10 && v.ak[1] == 0)
	{	
		res = u;
		
		for(int i = 0; i < int(v.ak[0])-1; i++){
			res = res * u;
		}
		return res;
	}
	else
	{
		if(u.ak[0] < 0){
			return exp(v * ln(-u));
		}
		else{
			return exp(v * ln(u));
		}
	}
}
