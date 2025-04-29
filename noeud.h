//Architecture Fig 11 b1
#ifndef NOEUD_HEADER_H
#define NOEUD_HEADER_H

#include <iostream>
#include <vector>

#include "tools.h"

class Quartier;
class LinkInfo;
class Logement;
class Transport;
class Production;

//Rajoute noeud au vect apres les verifications
void ajouterNoeud(Quartier* noeud);

//Verifications sur les noeuds
void identical_uid(Quartier* noeud); 
void too_much_capacity(Quartier* noeud); 
void too_little_capacity(Quartier* noeud); 
void reserved_uid(Quartier* noeud); 
void node_node_superposition(Quartier* noeud); 

//Rajoute lien au vecteur du noeud apres les verifications
void ajouterLien(unsigned int uid_d, unsigned int uid_f); 

//Verifications sur les liens
void self_link_node(unsigned int uid_d, unsigned int uid_f); 
void link_vacuum(unsigned int uid_d, unsigned int uid_f); 
void multiple_same_link(unsigned int uid_d, unsigned int uid_f); 
void max_links(unsigned int uid_d, unsigned int uid_f); 
void node_link_superposition(unsigned int uid_d, unsigned int uid_f); 

//Fonctions d'affichages
void afficherLiens(Quartier* noeud);
void afficherQuartier();
void afficherLiens();

//Verifie si un quartier est bien dans le vecteur noeuds
bool isInQuartiers(unsigned int uid);

//Accede a un quartier dans le vecteur noeud
Quartier* accessQuartier(unsigned int uid);

//decodeur normale					 
void decodageLigne(string line);

//Gets the total capcity of a type of node
unsigned int getTypeCapacity(Quartier* type);

//Gets the total amount of links in a city
unsigned int getNumbLinks();

// Getters of data inside of the unamed namespace
vector<Quartier*> getNoeud();
vector<LinkInfo*> getLinks();
unsigned int getNoeudsSize();

//Return the fastest access time to a production or transport node
double dijkstra(unsigned int d, Quartier* type, vector<Quartier*>& path_nodes, 
				bool activate_short = false);

//Initializes the values for dijkstra
void initDijkstra(unsigned int d, vector<unsigned int>& indices);

//Displays a vector of unsigned int
void show_vect(vector<unsigned int>& path);

//Sorts with respect to access times
void sorting(vector<unsigned int>& indices);

//Finds the indice with the fastest access time
unsigned int find_min_access(vector<unsigned int>& ta);

//Finds the indice using the uid
unsigned int find_indice(unsigned int uid);

//Computes the access time
double cmpt(unsigned int lv, unsigned int n);

//Shows the status of the algorithm
void show_dijk_status();

//Creates the shortest path and stores it to a vector called path
void create_path(unsigned int n, vector<unsigned int>& path);

//Verifies if they are nodes left to check for the dijktra algorithm
bool nodes_to_observe();

double best_time(double answer, unsigned int d, vector<unsigned int>& path, 
				 vector<Quartier*>& path_nodes);

//Verifies that the quartier at that index is a logement node
bool checkHouse(unsigned int index);

//Resets all data
void reset_all();

//setter and getter for the detected variable
bool getDetected();
void setDetected(bool new_detect);

//Blocks selection when in edit link mode
void setBlockSelection(bool state);

void setEditLink(bool state);

unsigned int find_index(unsigned int uid);

//Dessin de noeuds
void dessinerNoeudLogement(Logement* l, Color c = Black);
void dessinerNoeudTransport(Transport* t, Color c = Black);
void dessinerNoeudProduction(Production* p, Color c = Black);

//Draws the links of a node
void draw_links_noeud(Quartier* quartier);
void draw_link(Quartier* q1, Quartier* q2, Color c = Black);

void delete_node(unsigned int index);
void delete_global_link(unsigned int uid1, unsigned int uid2);

//Checks is node is in the shortest path
bool checkIfInPath(const vector<Quartier*>& path, Quartier* q);

//Makes sure there is a valid shortest path
bool checkPathValidity(const vector<unsigned int>& path, unsigned int starter);
bool checkSequence(const vector<Quartier*>& path, unsigned int i, unsigned int j);

void reset_selection();

//For the verifications with dist_min
bool intersection_dist_min(Cercle c1, Cercle c2);
bool intersection_dist_min(Segment s1, Cercle c1);
bool intersection_dist_min(Cercle c1, Segment s1);

//Checks if two nodes are have a minimum distance of dist_min between them 
bool checkIntersection(Quartier* noeud, vector<Quartier*> noeuds, Cercle circle);

//Makes sure the new radius respects the min_capacity and max_capacity
bool checkCapacity(Quartier* noeud, Cercle circle);

//Checks if the moved node has a minimum, of dist_min with the links
bool checkLinkNodeIntersection(Quartier* noeud, vector<Quartier*> noeuds, 
							   Cercle circle);
							   
//Checks if the new link is at least dist_min away of all nodes
bool checkOneLink(Quartier* noeud_dep, Quartier* noeud_fin);

//Updates link data when a node is changed
void updateLinks(unsigned int uid);

//General node class
class Quartier{
protected:
	unsigned int uid;
	S2d centre;
	unsigned int capacite;
	unsigned int nbliens;
	Cercle cercle;
	vector<Quartier*> link;
	double x;
	double y;
	bool isHouse;
	bool selected;
	Color c;
	bool in;
	double access;
	unsigned int parent;
		
public:	
	//Constructeur Quartiers
	Quartier(unsigned int uid, double x, double y, unsigned int capacite, 
			 bool isHouse = false, bool selected = false);
	
	//Destructeur
	virtual ~Quartier();
	
	//Getters	 
	unsigned int getUid();
	S2d getCentre();
	unsigned int getCapacite();
	unsigned int getNbLiens();
	Cercle getCercle();
	bool getIsHouse();
	vector<Quartier*> getLink();
	bool getSelected();
	bool getIn();
	unsigned int getParent();
	double getAccess();
	
	void setSelected(bool state);
	void setX(double x);
	void setY(double y);
	void setCentre(S2d centre);
	void setCercle(Cercle cercle);
	void setCapacite(double capacite);
	void setIn(bool in);
	void setParent(unsigned int parent);
	void setAccess(double access);
		
	//Fonction d'affichages
	virtual void afficher();
	void afficherLiens();
	
	void ajouterLien(Quartier* noeud);
	void delete_link(unsigned int uid);
	
	//Checks if the node is linked to the node q
	bool isLinked(Quartier* q);
	
	//Checks if the user clicked in the node
	bool inNoeud(double xm, double ym);
	
	//Clears links of a node
	void reset_links();
};

class Logement : public Quartier{
public:
	Logement(unsigned int uid, double x, double y,
			 unsigned int capacite, bool isHouse = true):
			 Quartier(uid, x, y, capacite, isHouse){};
};

class Production : public Quartier{
public:
	Production(unsigned int uid, double x, double y,
			   unsigned int capacite, bool isHouse = false):
			   Quartier(uid, x, y, capacite, isHouse){};
};

class Transport : public Quartier{
public:
	Transport(unsigned int uid, double x, double y,
			  unsigned int capacite, bool isHouse = false):
			  Quartier(uid, x, y, capacite, isHouse){};	
};

//Used to store distance, speed and min capacity of links
class LinkInfo{
private:
	unsigned int uid1;
	unsigned int uid2;
	double dist;
	Quartier* dep;
	Quartier* fin;
	bool fast;
	unsigned int capacity;

public:
	//Constructeur
	LinkInfo(Quartier*& dep, Quartier*& fin);
	
	//Getters
	double getDistance();
	bool getFast();
	unsigned int getCapacity();
	unsigned int getUid1();
	unsigned int getUid2();
	Quartier* getDep();
	Quartier* getFin();
	
	//Setters
	void setUid1(unsigned int uid1);
	void setUid2(unsigned int uid2);
	void setDist(double dist);
	void setDep(Quartier* dep);
	void setFin(Quartier* fin);
	void setFast(bool fast);
	void setCapacity(unsigned int capacity);
	
};

#endif
