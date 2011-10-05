/*##########
autodiff.cpp
##########*/

#include "autodiff.h"
#include <sstream>
using namespace std;

/**#####################
Schnittstellenfunktionen 
#####################**/
double autodiff::get_u(){
	return u;
}
double autodiff::get_v(){
	return v;
}
double autodiff::get_w(){
	return w;
}
void autodiff::set_u(double x){
	u = x;
}
void autodiff::set_v(double x){
	v = x;
}
void autodiff::set_w(double x){
	w = x;
}

/**##############################################
Umwandlung des Tripels in einen sinnvollen String
##############################################**/
string autodiff::toStdString(){
	stringstream str;
	str << "(" << u << "/" << v << "/" << w << ")";
	return str.str();
}


/**########################################
(Kopier-)Kon(De)struktor und Zuweisungsoperator 
########################################**/
autodiff::autodiff(double x=0, double y=0, double z=0){
	u = x; 
	v = y; 
	w = z;
}
autodiff::autodiff(const autodiff &a){    
    u = a.u;
    v = a.v;  
    w = a.w; 
}
autodiff &autodiff::operator =(const autodiff &a){
	u = a.u;
	v = a.v;
	w = a.w;
	return *this;
}
autodiff::~autodiff(){

}

/**########################################################
Operator * wird mit einer skalaren Multiplikation überladen
########################################################**/
autodiff operator *(double k, autodiff a){
    	autodiff res(k*a.u, k*a.v, k*a.w);
    	return res;
}

/**#####################################
Die Potenzierungsfunktion wird überladen
#####################################**/
autodiff pow(autodiff f, autodiff g){	
	autodiff res(0,0,0);
	if (f.u == 0) {
		res.u = 0;
		res.v = (-pow(0.0000001,g.u+0.0000001))/(-0.0000001);
		res.w = 1;
	}
	else{
		res.u = pow(f.u,g.u);
		res.v = exp(g.u * log(fabs(f.u)))*(g.v * log(fabs(f.u)) + g.u*(f.v/f.u));
		res.w = exp(g.u * log(fabs(f.u))) * (pow((g.v * log(fabs(f.u)) + g.u*(f.v/f.u)),2) + g.w*log(fabs(f.u)) + 2*g.v*(f.v/f.u) + g.u*((f.w*f.u-f.v*f.v)/(f.u*f.u)));
	}	
	return res;
}

/**###########################################
Die transzendenten Funktionen werden überladen
###########################################**/
autodiff exp(autodiff x){ //Exponential-Funktion
	autodiff res(exp(x.u),  exp(x.u) * x.v,  exp(x.u)  * (x.v * x.v + x.w));
	return res;
}
autodiff sin(autodiff x){ //Sinus-Funktion
	autodiff res(sin(x.u),  cos(x.u) * x.v,  x.w * cos(x.u) - sin(x.u) * x.v * x.v);
	return res;	
}
autodiff cos(autodiff x){ //Cosinus-Funktion
	autodiff res(cos(x.u),  -sin(x.u)*x.v,  -(cos(x.u)*x.v*x.v + x.w*sin(x.u)));
	return res;
}
autodiff ln(autodiff x){ //Natürlicher Logarithmus
	autodiff res(log(x.u), x.v/x.u, (x.w*x.u - x.v*x.v)/(x.u*x.u));
	return res;
}
autodiff tan(autodiff x){ //Tangens definiert mit den bereits überladenen Funktionen Sinus und Cosinus
	autodiff res = sin(x)/cos(x);
	return res;
}
autodiff cot(autodiff x){ //Kotangens definiert mit den bereits überladenen Funktionen Sinus und Cosinus
	autodiff res = cos(x)/sin(x);
	return res;
}
/**###################################################################
Hyperbolische Winkelfunktionen als Komposition der Exponentialfunktion
###################################################################**/
autodiff cosh(autodiff x){
	autodiff res = 0.5*(exp(x) + exp(-x));
	return res;
}
autodiff sinh(autodiff x){
	autodiff res = 0.5*(exp(x) - exp(-x));
	return res;
}
autodiff tanh(autodiff x){
	autodiff res = sinh(x)/cosh(x);
	return res;
}
autodiff coth(autodiff x){
	autodiff res = cosh(x)/sinh(x);
	return res;
}

/**#################################
Standart-Operatoren werden überladen
#################################**/
autodiff operator + (autodiff a, autodiff b){
	//Bei Addition können die Werte einfach addiert werden.
	autodiff res(a.u + b.u,  a.v + b.v,  a.w + b.w);
	return res;
}
autodiff operator - (autodiff a, autodiff b){
	//Verwendung des bereits überladenen unären Operators (-)
	autodiff res = a + (-b);
	return res;
}
autodiff operator * (autodiff a, autodiff b){
	//Zweifache Anwendung der Produktregel.
	autodiff res(a.u * b.u,  a.u * b.v + a.v * b.u,  a.u * b.w + 2 * a.v * b.v + a.w*b.u);
	return res;
}
autodiff operator / (autodiff a, autodiff b){
	//Zweifache Anwendung der Quotientenregel mit Prüfung auf Nullstellen im Nenner.
	
	if (fabs(b.u) < 0.00001){
		autodiff res(0,0,0);
		return res;
	}
	else{	
		autodiff res(a.u / b.u,  (a.v*b.u-a.u*b.v)/(b.u*b.u), ((a.w*b.u-b.w*a.u)*b.u-((a.v*b.u)-(b.v*a.u))*2*b.v)/(b.u*b.u*b.u));
		return res;
	}
	
}

/**############
Unärer Operator
############**/
autodiff operator - (autodiff a){	
	autodiff res(-a.u, -a.v, -a.w);
	return res;	
}