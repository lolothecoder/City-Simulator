//Architecture Fig 11 b1
#include <iostream>
#include "graphic_gui.h"

using namespace std;

namespace{
	const Cairo::RefPtr<Cairo::Context>* ptcr(nullptr); 
	double width;
	double height;
	int convertX(double coord, double width);
	int convertY(double coord, double height);
	void convertRayon(double& rayon, double width);
	Color couleur(Black);
	double dim_max;
	double xMin;
	double xMax;
	double yMin;
	double yMax;
}



void graphic_set_context(const Cairo::RefPtr<Cairo::Context>& cr){
	
	static bool init(false);

	if(!init){
		ptcr = &cr;
		init = true;
	}
}



void graphic_draw_line(int xd, int yd, int xf, int yf){
	
	(*ptcr)->set_line_width(3);
	
	//Convertit les coordonnées
	xd = convertX(xd, width);
	yd = convertY(yd, height);
	xf = convertX(xf, width);
	yf = convertY(yf, height);
	
	//Changement de couleur
	switch(couleur){
		case Red:{
			(*ptcr)->set_source_rgb(0.8, 0.0, 0.0);
			break;	
		}
		case Green:{
			(*ptcr)->set_source_rgb(0.0, 128, 0.0);
			break;
		}
		default:{
			(*ptcr)->set_source_rgb(0.0, 0.0, 0.0);
		}
	}
	
	(*ptcr)->move_to(xd, yd);
	(*ptcr)->line_to(xf, yf);

	(*ptcr)->stroke();
}

void graphic_draw_circle(int xc, int yc, double rayon){
	(*ptcr)->set_line_width(3);
	
	//Convertit les coordonnées du centre et le rayon
	xc = convertX(xc, width);
	yc = convertY(yc, height);
	convertRayon(rayon, width);
	
	(*ptcr)->move_to(xc+rayon, yc);
	(*ptcr)->arc(xc,yc,rayon,0,M_PI*2);
	(*ptcr)->set_source_rgb(1.0, 1.0, 1.0);
	(*ptcr)->fill();
	
	//Changement de couleur
	switch(couleur){
		case Red:{
			(*ptcr)->set_source_rgb(0.8, 0.0, 0.0);
			break;	
		}
		case Green:{
			(*ptcr)->set_source_rgb(0.0, 128, 0.0);
			break;
		}
		default:{
			(*ptcr)->set_source_rgb(0.0, 0.0, 0.0);
		}
	}
	
	(*ptcr)->arc(xc,yc,rayon,0,M_PI*2);
	(*ptcr)->stroke();
}

//Méthode qui permet le dessin du rectangle dans Production
void graphic_draw_quadrilatere(int x1, int y1, int x2, int y2, int x3, int y3, int x4, 
							   int y4){
	(*ptcr)->set_line_width(3);
	
	x1 = convertX(x1, width);
	y1 = convertY(y1, height);
	x2 = convertX(x2, width);
	y2 = convertY(y2, height);
	x3 = convertX(x3, width);
	y3 = convertY(y3, height);
	x4 = convertX(x4, width);
	y4 = convertY(y4, height);
	
	switch(couleur){
		case Red:{
			(*ptcr)->set_source_rgb(0.8, 0.0, 0.0);
			break;	
		}
		case Green:{
			(*ptcr)->set_source_rgb(0.0, 128, 0.0);
			break;
		}
		default:{
			(*ptcr)->set_source_rgb(0.0, 0.0, 0.0);
		}
	}
	
	(*ptcr)->move_to(x1, y1);
	(*ptcr)->line_to(x2, y2);
	(*ptcr)->line_to(x3, y3);
	(*ptcr)->line_to(x4, y4);
	(*ptcr)->line_to(x1, y1);
	(*ptcr)->line_to(x2, y2);

	(*ptcr)->stroke();
}

//Setters
void set_width(double w){
	width = w;
}

void set_height(double h){
	height = h;
}

void set_color(Color c){
	couleur = c;
}

void set_dim_max(double dm){
	dim_max = dm;
}

void set_coordinates(double xmin, double xmax, double ymin, double ymax){
	xMin = xmin;
	xMax = xmax;
	yMin = ymin;
	yMax = ymax;
} 

int xModel(double coord, double width){
	return (coord/width)*(xMax-xMin) + xMin;
}

int yModel(double coord, double height){
	return yMax - (coord/height)*(yMax-yMin);
}

//Définition des méthodes appartenant à l'espace des noms non-nommés
namespace{
	int convertX(double coord, double width){
		double prefactor = width / (xMax - xMin); 
		return (coord - xMin) * prefactor;	
	}

	int convertY(double coord, double height){
		double prefactor = height / (yMax - yMin) ; 
		return (yMax - coord) * prefactor;	
	}

	void convertRayon(double& rayon, double width){
		rayon = width * rayon / (xMax - xMin);
	}
}

	
	
