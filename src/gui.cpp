#include "gui.h"
#include <QtGui/QPainter>
#include "autodiff.h"
#include "taylor.cpp"

using namespace std;

//Boole-Variablen für den Status der übergeordneten Check-Boxen.
bool switch_taylor = true;
bool switch_plot = true;
bool switch_autodiff = true;

//Die Konstanten der Klasse Autodiff werden erstellt.
const autodiff X(1,1,0); 
const autodiff C(1,0,0);


gui::gui(QMainWindow *parent) : QMainWindow(parent){
        //Methode, die die erstellte GUI läd und erstellt.
        setupUi(this);
	
	//Von den Widgets in der GUI emittierte Signale werden mit Member-Funktionen der Klasse "GUI" verknüpft.
	connect(calc, SIGNAL(clicked()), this, SLOT(calculate()));
	connect(hlp, SIGNAL(clicked()), this, SLOT(help()));
       	connect(ad, SIGNAL(clicked()), this, SLOT(autodiff_toggled()));
       	connect(ta, SIGNAL(clicked()), this, SLOT(taylor_toggled()));
       	connect(plt, SIGNAL(clicked()), this, SLOT(plot_toggled()));
       	connect(eingabe, SIGNAL(returnPressed()), this, SLOT(calculate()));
       	connect(clear, SIGNAL(clicked()), this, SLOT(ausgabe_clear()));
}

//Destruktor der Klasse "GUI".
gui::~gui(){
}

//Ausgabefenster wird geleert. Und plots-Ordner geleert.
void gui::ausgabe_clear(){
    
	ausgabe -> clear();	
}

//Wechselt die Variablen für die globalen Checkboxen passend zu den Aktionen des Users.
void gui::autodiff_toggled(){
	switch_autodiff = 1 - switch_autodiff;
}
void gui::taylor_toggled(){
	switch_taylor = 1 - switch_taylor;
}
void gui::plot_toggled(){
	switch_plot = 1 - switch_plot;
}

//Hauptroutine, die ausgeführt wird, wenn "Calculate" gedrückt wird.
void gui::calculate(){
	
	//Überprüfung ob die Eingabe und die Funktionsauswahl bearbeitbar sind.
        if (eingabe -> text() == ""){
        	fail("Keine Eingabe vorhanden");
        	ausgabe->append(" ");
        	return;
        }
        if ((switch_autodiff == false)&&(switch_taylor == false)&&(switch_plot == false)){
		fail("Keine Funktion ausgewaehlt");
		ausgabe->append(" ");
		return;
	}
	if (((autodiff_1->checkState() != Qt::Checked)&&(autodiff_2->checkState() != Qt::Checked)&&(autodiff_3->checkState() != Qt::Checked)&&(switch_autodiff == true))||((taylor_1->checkState() != Qt::Checked)&&(taylor_2->checkState() != Qt::Checked)&&(switch_taylor == true))){
		fail("Keine Funktion ausgewaehlt");
		ausgabe->append(" ");
		return;
	}
	
	
	//Eine Instanz der Klasse parser wird erstellt und die Eingabe zur Verwertung an den Konstruktor übergeben.
	parser term((eingabe -> text()).toStdString());	
	
	//Falls der Parser den eingegebenen Term nicht als valide erkannt hat, wird ein Fehler ausgegeben.
	if (term.get_input() == "non valid"){
		fail("Syntaxerror");
	}
	else{
		
		//Wenn Eingabe sinnvoll, dann Eingabe ausgeben.
		QString input = "> Input interpretation: f(x) = " + input.fromStdString(term.get_input());		
		ausgabe->setTextColor(QColor(0,0,255,255));
		ausgabe->append(" ");
		ausgabe->append(" ");
		ausgabe->append(input);
		ausgabe->append(" ");
		ausgabe->setTextColor(QColor(0,0,0,255));
		
		
		if (switch_autodiff==true){
			if (autodiff_1->checkState() == Qt::Checked){
				/**##############
				f(x),f'(x),f''(x)				
				##############**/
				//Stelle, an der die Ableitungen ausgerechnet werden sollen wird aus der GUI eingelesen.
				double x = autodiff_1_1->value();			
					
				//Es wird der eingegebene Term an der Stelle x ausgewertet.
				autodiff abl = term.derive(x,term.get_baum());
			
				//Die Ausgabe erfolgt, indem die berechneten Werte mit den Schnittstellenfunktionen ausgelesen werden.
				ausgabe->setTextColor(QColor(0,0,0,150));
				ausgabe->append("> Funktionswerte und Ableitungen:");
				ausgabe->append(" ");
				ausgabe->setTextColor(QColor(0,0,0,255));
				ausgabe->append(QString("     f(%1) = %2").arg(x).arg(abl.get_u()));
				ausgabe->append(QString("    f'(%1) = %2").arg(x).arg(abl.get_v()));
				ausgabe->append(QString("   f''(%1) = %2").arg(x).arg(abl.get_w()));
				ausgabe->append(" ");
			}
			if (autodiff_2->checkState() == Qt::Checked){
				/**#############
				NEWTON-ITERATION
				#############**/
			
				//Zahl der Iterationsschritte
				int newton_i = 0;
				//Boole ob Newtonverfahren erfolgreich war
				bool success = true;
				//Variable für die Nullstelle
				autodiff ns(0,0,0);
				//Abweichung zwischen den Iterationsschritten, Nullstelle aus letztem Schritt, Nullstelle aus aktuellem Schritt
				double e,x_0,x_0_neu;
				//Startwert wird aus der GUI eingelesen
				x_0 = autodiff_2_1->value();
				//Abweichung als Abbruchsbedingung wird eingelesen
				e = autodiff_2_2->value();
				//Aktuelle Variable groß genug setzen, dass zumindest die Iteration begonnen wird
				x_0_neu = x_0 + 2*e;
					
			
				while (fabs(x_0-x_0_neu) > e){
					x_0 = x_0_neu;
					//Anzahl der Iterationen wird um 1 erhöht.			
					newton_i++;
				
					//Ableitung bei x_0 wird bestimmt.
					ns = term.derive(x_0,term.get_baum());
				
					//Prüfung ob eine waagerechte Tangente erwischt wurde. Wenn ja Abbruch und eine Fehlermeldung wird ausgegeben.
					if (ns.get_v() == 0){
						//Fehlermeldung
						fail(QString("Newton: Waagerechte Tangente in Iterationsschritt %1 erwischt. Abbruch.").arg(newton_i));
						success = false;
						//Abbruch der Schleife
						break;
					}
					
					//Nach zu vielen Iterationen ohne Erfolg wird die Iteration abgebrochen.
					if (newton_i > 9000){
					    //Fehlermeldung
						fail(QString("Newton: Nach mehr als 9000 Iterationen keine Nullstelle gefunden. Abbruch."));
						success = false;
						//Abbruch der Schleife
						break;
					}
					else
					{					
						//Ansonsten wird die neue Nullstelle berechnet.
						x_0_neu = x_0 - (ns.get_u()/ns.get_v());				
					}	
				}
			
				//Wenn Newton erfolgreich war, dann werden die Ergebnisse ausgegeben
				if (success == true){
					ausgabe->setTextColor(QColor(0,0,0,150));
					ausgabe->append(QString("> Newton mit s=%1 und Toleranz d=%2 nach %3 Iterationen:").arg(autodiff_2_1->value()).arg(e).arg(newton_i));
					ausgabe->append(" ");
					ausgabe->setTextColor(QColor(0,0,0,255));
					ausgabe->append(QString("     x_0 = %1").arg(x_0));
					ausgabe->append(" ");
				}
			}
			if (autodiff_3->checkState() == Qt::Checked){
				/**#########
				WERTETABELLE
				#########**/
			
				int i = 0;
				double a,s,b;
				QString z1,z2,z3,z4;
			
				//Auslesen der Felder in der GUI für Startwert, Schrittweite und Obergrenze.
				a = autodiff_3_1->value();
				s = autodiff_3_2->value();
				b = autodiff_3_3->value();
			
				//Zeilen, die die errechneten Werte in eine Tabelle eingebettet enthalten.
				z1 = "<table cellpadding=5><tr><th align=right>x = </th>";
				z2 = "<tr><td align=right>f(x) = </td>";
				z3 = "<tr><td align=right>f'(x) = </td>";
				z4 = "<tr><td align=right>f''(x) = </td>";
			
				//Schleife inkrementiert die Stelle x immer um den vorgegebenen Wert bis die Obergrenze erreicht oder überschritten ist.
				while((a+i*s) <= b){
					//Auswertung der Eingabe an der aktuellen Position.
					autodiff abl = term.derive(a+i*s,term.get_baum());
					z1 += QString("<th align=right>%1</th>").arg(a+i*s);
					z2 += QString("<td align=right>%1</td>").arg(abl.get_u());
					z3 += QString("<td align=right>%1</td>").arg(abl.get_v());
					z4 += QString("<td align=right>%1</td>").arg(abl.get_w());
					i += 1;
				}
				z1 += "</tr>";
				z2 += "</tr>";
				z3 += "</tr>";
				z4 += "</tr></table>";
				ausgabe->setTextColor(QColor(0,0,0,150));
				ausgabe->append(QString("> Wertetabelle fuer x = %1..%2(%3):").arg(a).arg(b).arg(s));
				ausgabe->append(" ");
				ausgabe->setTextColor(QColor(0,0,0,255));
				
				//Ausgabe der Tabelle mit HTML formatiert.
				ausgabe->insertHtml(z1+z2+z3+z4);			
				ausgabe->append(" ");
			}
		}
		if (switch_taylor == true){
			
			// Auslesen der Felder in der GUI für Koeffizientenarrylaenge und Entwicklungspunkt.
			double t0 = taylor_1_2->value();
			int k = taylor_1_1->value();
			
			taylor tpol = term.tayl(k,t0,term.get_baum());
			
			if (taylor_1->checkState() == Qt::Checked){
				
				// Ausgabeprozedur fuer den Term des Taylorpolynoms.
				ausgabe->setTextColor(QColor(0,0,0,150));
				ausgabe->append("> Taylorpolynom: ");
				ausgabe->append("");
			
				QString tpoly = "", tmp = "";
				
				tpoly = tpoly.setNum(tpol.akout()[0]);
				
				if(k > 1){
					
					if(t0 == 0){
						if(tmp.setNum(tpol.akout()[1]) != "0"){
							tpoly += " + " + tmp.setNum(tpol.akout()[1]) + " * x";
						}
						tmp = "";
						for(int i = 2; i < k; i++){
							
				
							if(tmp.setNum(tpol.akout()[i]) == "0"){continue;}
				
							tpoly += " + " + tmp.setNum(tpol.akout()[i]) + " * x^";
							tmp = "";
							tpoly += tmp.setNum(i);
						}
					}
					else{
						if(tmp.setNum(tpol.akout()[1]) != "0"){
							tpoly += " + " + tmp.setNum(tpol.akout()[1]) + " * (x";
							tmp = "";
							if(t0 > 0){
								tpoly += tmp.setNum(-t0) + ")";
							}
							else{
								tpoly += "+" + tmp.setNum(-t0) + ")";
							}
						}
						tmp = "";
						for(int i = 2; i < k; i++){
											
							if(tmp.setNum(tpol.akout()[i]) == "0"){continue;}
				
							tpoly += " + " + tmp.setNum(tpol.akout()[i]) + " * (x";
							tmp = "";
							if(t0 > 0){
								tpoly += tmp.setNum(-t0) + ")^";
							}
							else{
								tpoly += "+" + tmp.setNum(-t0) + ")^";
							}
							tmp = "";
							tpoly += tmp.setNum(i);
						}
					}
				}
			
				// QString des Taylorpolynoms wird im Ausgabebereich angehaengt.
				ausgabe->setTextColor(QColor(0,0,0,255));
				ausgabe->append("     T(x) = " + tpoly + " + R(xi)");
				ausgabe->append("");
			}
			if (taylor_2->checkState() == Qt::Checked){
				
				// Auslesen des Feldes in der GUI für den x-Wert.
				double x = taylor_2_1->value();
				
				// Wert des Taylorpolynoms an der Stelle x wird berechnet.
				double tval = tpol.xval(x, t0, k);
				
				// Ergebnisse werden im Ausgabebereich angehaengt.
				QString tp = "", tmp = "";
				tp.setNum(tval);
				tmp.setNum(x);
				ausgabe->setTextColor(QColor(0,0,0,150));
				ausgabe->append("> Wert des Taylerpolynoms bei x=" + tmp);
				ausgabe->append("");
				ausgabe->setTextColor(QColor(0,0,0,255));
				ausgabe->append("     T(" + tmp + ") = " + tp);
				ausgabe->append("");
			}
		}
		
		//Falls Plot ausgewählt ist, wird geplottet
		if (switch_plot == true){
			plot(plt_1_1->value(),plt_1_2->value(),plt_2_1->value(),plt_2_2->value(),plt_gen->value(),term,switch_autodiff,switch_taylor);
		}
		ausgabe->append(" ");		
	}	
}
 
void gui::fail(QString text){
	ausgabe->setTextColor(QColor(255,0,0,255));
	ausgabe->append("> " + text );
	ausgabe->append(" ");
	ausgabe->setTextColor(QColor(0,0,0,255));			
} 

void gui::help(){
	/**##########
	####Hilfe####
	##########**/		
		
		
	ausgabe->insertHtml("<h2>QtDiff - Hilfe</h2><br>");
	help_body("QtDiff ist ein Programm zur automatischen Differentiation. <br> Generell kann man mit Checkboxen zwischen den Klassen Autodiff und Taylor und der Funktion auswaehlen, dabei sollte mindestens eine Checkbox ausgewaehlt sein. <br> Mit dem Calculate-Button wird die Eingabe mit den ausgewaehlten Checkboxen berechnet und im Ausgabefenster ausgegeben. <br> Mit dem Clear-Button laesst sich das Ausgabefeld leeren.");		
        
       	if (eingabe -> text() == ""){
		QString text = "";		
		text += (" <table cellpadding=5><tr><th align=left>Befehl</th><th align=left>Bedeutung</th></tr>");
		text += (" <tr><td>exp(..)</td><td>Die Exponentialfunktion</td></tr>");
		text += (" <tr><td>ln(..)</td><td>Der natuerliche Logarithmus</td></tr>");
		text += (" <tr><td>sin(..)</td><td>Die trigonometrische Sinusfunktion</td></tr>");
		text += (" <tr><td>cos(..)</td><td>Die trigonometrische Kosinusfunktion</td></tr>");
		text += (" <tr><td>tan(..)</td><td>Die trigonometrische Tangensfunktion</td></tr>");
		text += (" <tr><td>cot(..)</td><td>Die trigonometrische Kotangensfunktion</td></tr>");
		text += (" <tr><td>sinh(..)</td><td>Die Hyperbelfunktion Sinus Hyperbolicus</td></tr>");
		text += (" <tr><td>cosh(..)</td><td>Die Hyperbelfunktion Kosinus Hyperbolicus</td></tr>");
		text += (" <tr><td>tanh(..)</td><td>Die Hyperbelfunktion Tangens Hyperbolicus</td></tr>");
		text += (" <tr><td>coth(..)</td><td>Die Hyperbelfunktion Kotangens Hyperbolicus</td></tr></table>");
		
        	help_headline("Funktionsweise");
       	        help_body("In der Eingabezeile kann man eine Funktion eintippen. Dazu kann man folgende Operationen und transzendenten Funktionen verwenden: <br> Gueltige Operatoren : . , + , - , * , / , ^ , ) , ( " + text);
        }
        
        
        if ((switch_autodiff == true)){
                help_headline("Autodiff");
		help_body("Bei der Funktion f(x),f'(x),f''(x) gibt man x an und fuer dieses wird dann der Funktionswert der eingegebenen Funktion , der 1.ten und der 2.ten Ableitung berechnet. <br> Bei der Funktion <i>'Newton'</i> gibt man einen Startwert s und die Genauigkeit d an um mit dem Newtonsches-Iterationsverfahren die Nullstelle anzunaehern <br> Bei der Funktion Wertetabelle gibt man ein Intervall [a,b] an in der die Funktionswerte der Funktion und der beiden Ableitungen in gewissen Schritten berechnet wird (mit s erfolgt diese Angabe) und in einer Wertetabelle ausgegeben");		
	}
	
		
	
	if ((switch_taylor == true)){
		help_headline("Taylor");
		help_body("Bei der Funktion Entwicklungspunkt gibt man k und den Entwicklungspunkt t_0 an, mit denen man die ersten k Taylorkoeffizienten berechnet. <br> Bei der Funktion x-Wert gibt man den x-Wert an, an der die Taylorfunktion berechnet werden soll");		
	}
	
	
       	if ((switch_plot == true)){
       		help_headline("Plot");       		
		help_body("Mit der extra Funktion Plot, die unabhaengig von den Klassen ausgewaehlt werden kann, kann man sich die Funktionen unter Angabe des Definitionsbereiches [x_1,x_2], des Wertebereiches [y_1,y_2] und des Genauigkeitswertes plotten lassen");		
	} 	
       	
       	if ((switch_plot == false)||(switch_taylor == false)||(switch_autodiff == false)){       	
       		help_body("Fuer weitere Hilfe die jeweilige Checkbox aktivieren und Help-Button erneut betaetigen");
       	}
       	
       	if (eingabe -> text() != ""){ 
       		help_body("Fuer weitere Hilfe zur Eingabe zulaessiger Funktionen, das Eingabefeld leeren und Help-Button erneut betaetigen");
       	}
       	
}

void gui::help_headline(QString text){
	ausgabe->insertHtml("<h3 color=#FF0000>" + text + "</h3><br>");

}
void gui::help_body(QString text){
	ausgabe->insertHtml("<p color=blue>" + text + "</p><br>");

}

void gui::plot(double x1, double x2, double y1, double y2, double gen, parser term, bool diff, bool tay){	
	//Optimale Größe des angezeigten Bildes wird berechnet.
	double hg = ausgabe->height()-40;
	double y_width = (fabs(y2-y1));	
	double x_width = (fabs(x2-x1));
	double wd = hg*(x_width/y_width);
	cout << wd << endl;
	if (wd > ausgabe->width()-40){
		double faktor = (ausgabe->width()-20)/wd;
		wd *= faktor;
		hg *= faktor;
	}	
	hg = int(hg);
	wd = int(wd);
	//Sklarierung der Koordinatenachsen.
	double x_scale = wd/x_width;
	double y_scale = hg/y_width;	
	
	//Stifte für die verschiedenen Zeichenstile festlegen.
	QPen koord(Qt::black, 1);
	QPen f(Qt::black, 2);
	QPen f_(QColor(150,0,0), 1,Qt::DashLine);
	QPen f__(QColor(0,0,150), 1,Qt::DashLine);
	QPen tayl_pen(QColor(180,180,180), 1);
	
	//Bild zum Reinzeichnen wird angelegt
	QImage img(wd,hg, QImage::Format_ARGB32);
	QPainter painter(&img);	
	painter.begin(&img);
	painter.fillRect(0,0,wd,hg,Qt::white);
	painter.setRenderHint(QPainter::Antialiasing, true);	
	
	//Legende für die Funktion
	painter.drawText(40,10,"f(x)");
	painter.setPen(f);
	painter.drawLine(10,5,30,5);
	
	//Translation des Koordinatensystemns, dass der Ursprung richtig zu den Intervallen passt.
	painter.translate(-x1*x_scale,y2*y_scale);
	
	//Laufvariable in der Schleife	
	int i = 0;
	
	//x-Koordinate an der geplottet wird, Funktionswert und Ableitungen.
	double x,u,v,w;
	autodiff abl(0,0,0);
	
	//x-Koordinate an der geplottet wird, Funktionswert und Ableitungen aus letztem Plottschritt.
	double x_alt,u_alt,v_alt,w_alt;
	
	//Je nach Angaben in der GUI wird eine Instanz von Taylor erstellt.
	double t0 = taylor_1_2->value();
	int k = taylor_1_1->value();
	taylor tpol = term.tayl(k,t0,term.get_baum());
	
	//Array für die Funktionswerte des Taylorpolynoms für ein bestimmtes k.
	double *tayl_y, *tayl_y_alt;
	tayl_y = new double[k+1];
	tayl_y_alt = new double[k+1];	
	
	//Schleife durchläuft alle x-Werte in Abhängigkeit von [x1,x2] und der Genauigkeit.
	while (x1 + i*gen <= x2){		
		//x-Wert
		x = x1 + i*gen;
		
		//Funktionswert usw bei x
		abl = term.derive(x,term.get_baum());
		u = abl.get_u();
		
		//Ableitungen in Variablen schreiben, falls benötigt
		if (diff == true){
			v = abl.get_v();
			w = abl.get_w();
		}
		
		//Taylor-Polynome zeichnen
		if (tay == true){
			for (int i = 0;i <= k;i++){
				tayl_y[i] = tpol.xval(x, t0, i);		
			}
		}
		
		//Nach einem Durchlauf werden die ersten Linien gezeichnet
		if (i>0){			
			if (diff == true){
				painter.setPen(f_);
				painter.drawLine(x_alt*x_scale,-v_alt*y_scale,x*x_scale,-v*y_scale);
				painter.setPen(f__);
				painter.drawLine(x_alt*x_scale,-w_alt*y_scale,x*x_scale,-w*y_scale);
			}
			painter.setPen(tayl_pen);
			if (tay == true){
				for (int i = 0;i < k;i++){
					painter.drawLine(x_alt*x_scale,-tayl_y_alt[i]*y_scale,x*x_scale,-tayl_y[i]*y_scale);							
				}				
			}
			
			painter.setPen(f);
			painter.drawLine(x_alt*x_scale,-u_alt*y_scale,x*x_scale,-u*y_scale);
		}
		
		i++;
		
		//Merken der Werte für nächsten Durchlauf
		if (tay == true){
			for (int i = 0;i < k;i++){
				tayl_y_alt[i] = tayl_y[i];		
			}
		}
		x_alt = x;
		u_alt = u;
		v_alt = v;
		w_alt = w;
	}
	
	//Rücktransformation des Koordinatensystems
	painter.setPen(koord);	
	
	//Koordinatensystem wird über die Graphen gezeichnet, um sichtbar zu bleiben.
	painter.drawLine(x1*x_scale,0,x2*x_scale,0);
	painter.drawLine(0,-y1*y_scale,0,-y2*y_scale);
	painter.drawLine(x2*x_scale-8,5,x2*x_scale,0);
	painter.drawLine(x2*x_scale-8,-5,x2*x_scale,0);
	painter.drawText(x2*x_scale-8,-10,"x");	
	painter.drawLine(-5,-y2*y_scale+8,0,-y2*y_scale);	
	painter.drawLine(5,-y2*y_scale+8,0,-y2*y_scale);
	painter.drawText(10,-y2*y_scale+8,"y");
	
	//Einheiten auf den Achsen
	for(int i = x1+1; i <= x2-1; i++){
		if (x_scale > 20){		
			if (i != 0){
				painter.drawLine(x_scale*i,-5,x_scale*i,5);
				painter.drawText(x_scale*i-7,+17,QString("%1").arg(i));
			}
		}
		else{
			if ((i % int(x_scale/5))==0){
				painter.drawLine(x_scale*i,-5,x_scale*i,5);
				painter.drawText(x_scale*i-7,+17,QString("%1").arg(i));
			}
		
		}
	}
	for(int i = y1+1; i <= y2-1; i++){
		if (y_scale > 20){		
			if (i != 0){
				painter.drawLine(-5,-i*y_scale,+5,-i*y_scale);
				painter.drawText(-17,-y_scale*i+5,QString("%1").arg(i));
			}
		}
		else{
			if ((i % int(y_scale/5))==0){
				painter.drawLine(-5,-i*y_scale,+5,-i*y_scale);
				painter.drawText(-17,-y_scale*i+5,QString("%1").arg(i));
			}
		
		}		
	}
	
	//Legende für die Ableitungen wird zuletzt gezeichnet, um sichtbar zu bleiben.	
	if (diff==true){
		painter.translate(x1*x_scale,-y2*y_scale);	
		painter.drawText(40,25,"f'(x)");
		painter.drawText(40,40,"f''(x)");	
		painter.setPen(f_);
		painter.drawLine(10,20,30,20);
		painter.setPen(f__);
		painter.drawLine(10,35,30,35);
	}
	
	painter.end();
	ausgabe->setTextColor(QColor(0,0,0,150));
	ausgabe->append("> Plot: ");
	ausgabe->append(" ");
	ausgabe->append(" ");
	int suffix = rand();
	img.save(QString("plots/plot_%1.png").arg(suffix),"PNG");
	ausgabe->insertHtml(QString("<img src=plots/plot_%1.png>").arg(suffix));
}

