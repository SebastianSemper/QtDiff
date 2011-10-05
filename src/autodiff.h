#ifndef AUTODIFF_H
#define AUTODIFF_H
/*########
autodiff.h
########*/

#include <iostream>
#include "math.h"
#include "string.h"

using namespace std;

class autodiff{
		friend autodiff pow(autodiff,autodiff);
		friend autodiff exp(autodiff);
		friend autodiff ln(autodiff);
		friend autodiff sin(autodiff);
		friend autodiff cos(autodiff);
		friend autodiff tan(autodiff);
		friend autodiff cot(autodiff);
		friend autodiff cosh(autodiff);
		friend autodiff sinh(autodiff);		
		friend autodiff tanh(autodiff);
		friend autodiff coth(autodiff);
		friend autodiff operator *(autodiff, autodiff);
		friend autodiff operator /(autodiff, autodiff);
		friend autodiff operator +(autodiff, autodiff);
		friend autodiff operator -(autodiff, autodiff);
		friend autodiff operator -(autodiff);
		friend autodiff operator *(double, autodiff);
	private:	
		//Das Zahlentripel für Funktionswert, Wert der 
		//Ableitung und Wert der zweiten Ableitung	
		double u;
		double v;
		double w;
	public:	
		//Schnittstellenfunktionen zur Eingabe
		void set_u(double);
		void set_v(double);
		void set_w(double);
		
		//Schnittstellenfunktionen zur Ausgabe
		double get_u();
		double get_v();
		double get_w();
		
		//Methode zur einfachen Ausgabe	
		string toStdString();
		
		//Zuweisungsoperator
		autodiff &operator =(const autodiff &s);
		
		//Konstruktor
		autodiff(double, double, double);
		
		//Kopierkonstruktor
		autodiff(const autodiff &a);
		
		//Destruktor
		~autodiff();				
};
#endif //AUTODIFF_H