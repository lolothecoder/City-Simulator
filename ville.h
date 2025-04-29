//Architecture Fig 11 b1
#ifndef VILLE_HEADER_H
#define VILLE_HEADER_H

#include <iostream>

#include "noeud.h"

//lecture d'un fichier
void lecture(const char* nom_fichier);

//écrit un ficher pour le bouton save
void ecriture(const char* nom_ficher); 

//Calculs des criteres
double getENJ();
double getCI();
double getMTA();

//Concatenate a string and a double as a string
string conv_to_string(string s, double x);

void draw_links(const vector<Quartier*>& path_prod = vector<Quartier*>(), 
				const vector<Quartier*>& path_transp = vector<Quartier*>(),  
				bool shortest = false);
void draw_noeuds(const vector<Quartier*>& path_prod = vector<Quartier*>(), 
				 const vector<Quartier*>& path_transp = vector<Quartier*>(), 
				 bool shortest = false);

//Organizes nodes in the order : logmements, transports, productions
void reorganisation_tableau(vector<Quartier*> noeuds, 
							vector<Quartier*>& noeuds_organise, 
							int& nbL, int& nbT, int& nbP);

//Writes all the info of a node
void write_node(ofstream& fichier, vector<Quartier*>& noeuds_organise, unsigned int i);


void draw_shortest_path();

//Checks if a node was clicked and selects it
bool checkIfSelected(double xm, double ym, bool delete_noeud);
//Unselects everything
void reset_selected();
//Verifies if a node is selected
bool getSelectionMade();



//Permet d'ajouter un noeud dessiné dans le module gui dans la liste des noeuds
void ajouterNoeudDessine(Quartier* noeud);

void setLinkMode(bool state);

//Checks for potential errors when changing position or size of node
void updateCercle(double x, double y, double rayon, Quartier* noeud);

//Determines the uid to give created node
unsigned int assign_uid();

//Functions related to change in size of radius
void rayon_click_initial(bool& can_add_circle, bool link_activated, double xm, 
						 double ym);
						 
void rayon_click_final(double xm, double ym);

void reset_rayon(S2d point, S2d centre);

void finalise_rayon(S2d point);

//Chnges position of centre of node
void change_position(double xm, double ym);

//Checks if mouse was clicked inside a node that became a selected node, avoids 
//being able to change radius size when selecting node
bool mouse_click_on_node(S2d point_dep);

class Ville{
public:
	Ville(){};
};

#endif
