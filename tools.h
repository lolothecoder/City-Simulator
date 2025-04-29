//Architecture Fig 11 b1
#ifndef TOOLS_HEADER_H
#define TOOLS_HEADER_H

#include <iostream>
#include <cmath>

#include "graphic_color.h"

using namespace std;

struct S2d{
	double x;
	double y;
};

class Segment;
class Cercle;

//Distances
double distance(S2d s1, S2d s2);
double distance(Cercle c1, Cercle c2);
double distance(Cercle c1, S2d s1);
double distance(S2d s1, Cercle c1);
double distance(Segment s1, Cercle c1);
double distance(Cercle c1, Segment s1);
double distance(Segment s1, S2d p1);
double distance(S2d p1, Segment s1);

//Determine le point de collision pour la distance entre un cercle et un segment
S2d determine_collision(double v1, double v2, double xc, double yc, double x0, 
						double y0);

//Intersections
bool intersection(Cercle c1, Cercle c2);
bool intersection(Segment s1, Cercle c1);
bool intersection(Cercle c1, Segment s1);

//Affichages
ostream& operator<<(ostream& out, S2d s); //S2d
ostream& operator<<(ostream& out, Cercle c); //Cercle
ostream& operator<<(ostream& out, Segment s); //Segment

//Lien de dessin entre le modèle et Graphic
void draw_line(Segment line, Color c = Black);
void draw_circle(Cercle circle, Color c = Black);
void draw_quadrilatere(S2d p1, S2d p2, S2d p3, S2d p4, Color c = Black);

//Change la couleur avec l'accès à Enum 
void change_couleur(Color couleur);

class Segment{
private:
	S2d origine;
	S2d vecteur;
public:
	//Constructeur
	Segment(S2d origine = {0,0}, S2d vecteur = {1.0,1.0});
	//Useful
	ostream& affiche(ostream& out);
	S2d getOrigine();
	S2d getVecteur();
};

class Cercle{
private:
	S2d centre;
	double rayon;
public :
	//Constructeur
	Cercle(S2d centre = {0,0}, double rayon = 1.0) : centre(centre), rayon(rayon){};
		
	//Useful
	ostream& affiche(ostream& out);
	S2d getCentre();
	double getRayon();
	void setCentre(S2d centre);
	void setRayon(double rayon);
};


#endif
