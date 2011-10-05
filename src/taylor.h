#ifndef TAYLOR_H
#define TAYLOR_H

#include <math.h>

using namespace std;

class taylor{
	private:
		/**
		 * Anzahl der Taylorkoeefizienten
		 */
		int k;
		
		/**
		 * Zeiger auf das Array der Taylorkoeefizienten
		 */
		double *ak;
		
	public:
		/**
		 * Konstruktoren
		 */
		taylor();
		taylor(int);
		taylor(int, double *);
		/**
		 * Konstruktor der eine Taylor-Instanz für eine Konstante oder eine Variable erstellt
		 */
		taylor(char, double, int);
		
		/**
		 * Kopier-Konstruktor, Zuweisungsoperatoren und Destruktor
		 */
		~taylor();
		taylor &operator =(const taylor &s);
		taylor &operator =(double);
		taylor(const taylor &a);
		
		/**
		 * Methoden zur ausgabe des Koeffizientenarrays, zum setzen der Taylorkoeffizienten für Konstanten und Variablen
		 * und Berechnung des Wertes des Taylorpolynoms fuer ein festes x
		 */
		double* akout();
		void set_xkoef(double, int);
		void set_ckoef(double, int);
		double xval(double, double, int);
		
		/**
		 * Durch Freundfunktionen ueberladene Operatoren und mathematische Funktionen
		 */
		friend taylor operator +(taylor, taylor);
		friend taylor operator -(taylor, taylor);
		friend taylor operator *(taylor, taylor);
		friend taylor operator /(taylor, taylor);
		friend taylor operator -(taylor);
		friend taylor operator *(double, taylor);
		friend taylor sin(taylor);
		friend taylor cos(taylor);
		friend taylor exp(taylor);
		friend taylor ln(taylor);
		friend taylor tan(taylor);
		friend taylor tanh(taylor);
		friend taylor cosh(taylor);
		friend taylor sinh(taylor);
		friend taylor coth(taylor);
		friend taylor cot(taylor);
		friend taylor pow(taylor, taylor);
};

#endif // TAYLOR_H
