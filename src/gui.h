#ifndef WR_H
#define WR_H

#include <string>
#include "ui_gui.h"
#include "parser.h"
#include "autodiff.h"
#include "taylor.h"

using namespace std; 
 
class gui : public QMainWindow, public Ui::MainWindow { 
	Q_OBJECT
	 
	public slots:
		
		
	public:
	    	
	    	void input(string);
	    	
	    	gui(QMainWindow *parent = 0);
	    	~gui();
	private slots:
		void ausgabe_clear();
	    	void calculate();
	    	void plot_toggled();
	    	void autodiff_toggled();
	    	void taylor_toggled();
	    	void fail(QString);
	    	void help();
	    	void help_headline(QString);
	    	void help_body(QString);
	    	void plot(double, double, double, double, double,parser,bool,bool);
	private :
		
		
};

#endif //WR_H
