//Architecture Fig 11 b1

#include "graphic.h"
#include "tools.h"


using namespace std;

Segment::Segment(S2d origine , S2d vecteur){
	this->origine = origine;
	this->vecteur = vecteur;
}
// Affichage externe
ostream& operator<<(ostream& out, S2d s){
	return cout << "(" << s.x << "," << s.y << ")";
}

ostream& operator<<(ostream& out, Cercle c){
	return c.affiche(out);
}

ostream& operator<<(ostream& out, Segment s){
	return s.affiche(out);
}

//Affichage interne
ostream& Cercle::affiche(ostream& out){
	return cout << "Cercle de centre " << centre << " de rayon = " << rayon;
}

ostream& Segment::affiche(ostream& out){
	return cout << "Segment d'origne " << origine << " et de vecteur " << vecteur;
}

//Distances
double distance(S2d s1, S2d s2){
	return sqrt(pow(s1.x - s2.x,2) + pow(s1.y - s2.y,2));
}

double distance(Cercle c1, Cercle c2){
	return distance(c1.getCentre(), c2.getCentre());
}
	
double distance(Cercle c1, S2d s1){
	return distance(c1.getCentre(), s1);
}
	
double distance(S2d s1, Cercle c1){
	return distance(c1, s1);
}

double distance(Segment s1, Cercle c1){
	double v1(s1.getVecteur().x); 
	double v2(s1.getVecteur().y);
	double xCercle(c1.getCentre().x); 
	double yCercle(c1.getCentre().y);
	double xOrigineSeg(s1.getOrigine().x); 
	double yOrigineSeg(s1.getOrigine().y);
	double xFinSeg(xOrigineSeg + v1); 
	double yFinSeg(yOrigineSeg + v2);
	
	S2d FinSeg{xFinSeg, yFinSeg};
	S2d collision;
	
	collision = determine_collision(v1, v2, xCercle, yCercle, xOrigineSeg, 
								    yOrigineSeg);
	
	//Options de reponses
	double option1 (distance (c1, collision));
	double option2 (distance (c1, FinSeg));
	double option3 (distance (c1, s1.getOrigine()));
	
	//Longueur des elements a tester
	double longueurSeg(distance (s1.getOrigine(), FinSeg));
	double longueurFinCol(distance (FinSeg, collision));
	double longueurDebCol(distance (s1.getOrigine(), collision));
	
	//Selection de la bonne reponse
	if(longueurFinCol > longueurSeg or longueurDebCol > longueurSeg){
		if(longueurFinCol > longueurDebCol) 
			return option3;
		else 
			return option2;
	}
	else 
		return option1;
}

double distance(Cercle c1, Segment s1){
	return distance(s1, c1);
}

double distance(Segment s1, S2d p1){
	Cercle temp(p1);
	return distance(s1, temp);
}
	
double distance(S2d p1, Segment s1){
	return distance(s1, p1);
}

//Retourne le point de collision
S2d determine_collision(double v1, double v2, double xc, double yc, double x0, 
						double y0){
	if(v2 == 0)
		return {xc, y0};
	else if(v1 == 0)
		return {x0, yc};
	else{
		//Pentes des equations
		double m(v2/v1); double mPerp(-v1/v2);
		
		//Ordonne a l'origine des equations
		double h(y0 - x0 * m);
		double hPerp(yc - xc * mPerp);
		
		//Point d'intersection des droites
		double xInt((hPerp - h)/(m - mPerp));
		double yInt(m*xInt + h);
		return {xInt, yInt};
	}
}

//Intersection
bool intersection(Cercle c1, Cercle c2){
	if(distance(c1,c2) <= (c1.getRayon() + c2.getRayon())) 
		return true;
	return false;
}

bool intersection(Segment s1, Cercle c1){
	if(distance(s1, c1) <= c1.getRayon()) 
		return true;
	return false;
}

bool intersection(Cercle c1, Segment s1){
	return intersection(s1,c1);
}

//Getters
S2d Segment::getOrigine(){return origine;}
S2d Segment::getVecteur(){return vecteur;}
S2d Cercle::getCentre(){return centre;}
double Cercle::getRayon(){return rayon;}

//Setters
void Cercle::setCentre(S2d centre){this->centre = centre;}
void Cercle::setRayon(double rayon){this->rayon = rayon;}

//Graphic
void draw_line(Segment line, Color c){
	double xd(line.getOrigine().x);
	double xf(line.getOrigine().x + line.getVecteur().x);
	double yd(line.getOrigine().y);
	double yf(line.getOrigine().y + line.getVecteur().y);
	set_color(c);
	
	graphic_draw_line(xd, yd, xf, yf);
}

void draw_circle(Cercle circle, Color c){
	double xc(circle.getCentre().x);
	double yc(circle.getCentre().y);
	double rayon(circle.getRayon());
	set_color(c);
	
	graphic_draw_circle(xc, yc, rayon);
}

void draw_quadrilatere(S2d p1, S2d p2, S2d p3, S2d p4, Color c){
	set_color(c);
	graphic_draw_quadrilatere(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y);
}
	
//Permet de changer la couleur du dessin
void change_couleur(Color couleur){
	set_color(couleur);
}
