//Architecture Fig 11 b1
#include <map>
#include <algorithm>
#include <typeinfo>
#include <cstdlib>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>

#include "error.h"
#include "constantes.h"
#include "noeud.h"

using namespace std;

namespace{
	vector<Quartier*> noeuds;
	vector<LinkInfo*> links;
	
	bool errorNoeud(false);
	bool errorLien(false);
	bool detected(false); //Checks if there's at least one error 
	bool block_selection(false);
	bool edit_link(false);
	
	unsigned int numberOfLinks(0);
}

//Constructeur Quartier
Quartier::Quartier(unsigned int uid, double x, double y, unsigned int capacite,
				   bool isHouse, bool selected){
	this->uid = uid;
	this->capacite = capacite;
	centre = {x,y};
	nbliens = 0;
	cercle = {centre, sqrt(capacite)};
	this->isHouse = isHouse;
	this->selected = selected;
}

//Destructeur
Quartier::~Quartier(){}

//Constructeur LinkInfo
LinkInfo::LinkInfo(Quartier*& dep, Quartier*& fin){
	this->dist = distance(dep->getCercle(), fin->getCercle());
	
	Quartier* transp = new Transport(0,0,0,0,0);
	if(typeid(*dep) == typeid(*transp) && typeid(*fin) == typeid(*transp))
		this->fast = true; 
	else
		this->fast = false;
	
	this->capacity = min(dep->getCapacite(), fin->getCapacite()); 
	this->uid1 = dep->getUid();
	this->uid2 = fin->getUid();
}

//Getters
unsigned int Quartier::getUid(){return uid;}
S2d Quartier::getCentre(){return centre;}
unsigned int Quartier::getCapacite(){return capacite;}
unsigned int Quartier::getNbLiens(){return nbliens;}
Cercle Quartier::getCercle(){return cercle;}
vector <Quartier*> Quartier::getLink() {return link;}
bool Quartier::getIsHouse(){return isHouse;}
bool Quartier::getSelected(){return selected;}
double LinkInfo::getDistance(){return dist;}
bool LinkInfo::getFast(){return fast;}
unsigned int LinkInfo::getCapacity(){return capacity;}
Quartier* LinkInfo::getDep(){return dep;}
Quartier* LinkInfo::getFin(){return fin;}
unsigned int LinkInfo::getUid1(){return uid1;}
unsigned int LinkInfo::getUid2(){return uid2;}
unsigned int getNoeudsSize(){return noeuds.size();}
bool getDetected(){return detected;}
unsigned int getNumbLinks(){return numberOfLinks;}
vector<Quartier*> getNoeud(){return noeuds;}
vector<LinkInfo*> getLinks(){return links;}
bool Quartier::getIn(){return in;}
unsigned int Quartier::getParent(){return parent;}
double Quartier::getAccess(){return access;}

//Setters
void setDetected(bool new_detect){detected = new_detect;}
void setEditLink(bool state) {edit_link = state;}
void Quartier::setSelected(bool state){this->selected = state;}
void Quartier::setX(double x){this->x = x;}
void Quartier::setY(double y){this->y = y;}
void Quartier::setCentre(S2d centre){this->centre = centre;}
void Quartier::setCercle(Cercle cercle){this->cercle = cercle;}
void Quartier::setCapacite(double capacite){this->capacite = capacite;}
void Quartier::setIn(bool in){this->in = in;}
void Quartier::setParent(unsigned int parent){this->parent = parent;}
void Quartier::setAccess(double access){this->access = access;}
void LinkInfo::setUid1(unsigned int uid1){this->uid1 = uid1;}
void LinkInfo::setUid2(unsigned int uid2){this->uid2 = uid2;}
void LinkInfo::setDist(double dist){this->dist = dist;}
void LinkInfo::setDep(Quartier* dep){this->dep = dep;}
void LinkInfo::setFin(Quartier* fin){this->fin = fin;}
void LinkInfo::setFast(bool fast){this->fast = fast;}
void LinkInfo::setCapacity(unsigned int capacity){this->capacity = capacity;}

bool Quartier::inNoeud(double xm, double ym){
	S2d p({xm, ym});

	if(distance(p, cercle) <= cercle.getRayon()){
		if(!block_selection){
			reset_selection();
			setSelected(true);
		}
		return true;
	}
	else 
		return false;
}

void reset_selection(){
	for(unsigned int i(0); i < noeuds.size(); i++){
		if(noeuds[i]->getSelected())
			noeuds[i]->setSelected(false);
	}
}
//Fonctions d'affichages
void Quartier::afficher(){
	cout << "Noeud avec uid: " << getUid() << " et capacité "  << getCapacite() 
		 << " et centre: " << getCentre() << " et nombre de liens: " << getNbLiens() 
		 << "." << endl;
}

void afficherQuartier(){
	for(size_t i(0); i < noeuds.size(); ++i){
		noeuds[i]->afficher();
	}
}

void Quartier::afficherLiens(){
	if(link.size() == 0)
		cout << "No links for this noeud" << endl;
	else{
		for(unsigned int i(0); i < link.size(); i++){ 
			cout << "Lien de " << uid << " a " << link[i]->getUid() << endl;
		}
	}
} 

void Quartier::delete_link(unsigned int uid){
	for(unsigned int i(0); i < link.size(); i++){
		if(link[i]->getUid() == uid){
			swap(link[i], link[link.size()-1]);
			link.pop_back();
			nbliens--;
		}
	}
}

void afficherLiens(){
	for(unsigned int i(0); i < noeuds.size(); i++){
		noeuds[i]->afficherLiens();
	}
}

//Rajoute noeud au vect apres les verifications
void ajouterNoeud(Quartier* noeud){
	errorNoeud = false;
	identical_uid(noeud);
	too_much_capacity(noeud);
	too_little_capacity(noeud);
	reserved_uid(noeud);
	node_node_superposition(noeud);
	if(errorNoeud==false)
		noeuds.push_back(noeud);
	else
		detected = true;
}


//Verifications sur noeuds
void identical_uid(Quartier* noeud){
	unsigned int uid(noeud->getUid());
	for(size_t i(0); i < noeuds.size(); ++i){
		if(uid == noeuds[i]->getUid()){
			cout << error::identical_uid(uid);
			errorNoeud = true;
		}
	}
}

void too_much_capacity(Quartier* noeud){
	unsigned int capacite(noeud->getCapacite());
	if(capacite > max_capacity){
		cout << error::too_much_capacity(capacite);
		errorNoeud = true;
	}
}
	
void too_little_capacity(Quartier* noeud){
	unsigned int capacite(noeud->getCapacite());
	if(capacite < min_capacity){
		cout << error::too_little_capacity(capacite);
		errorNoeud = true;
	}
}

void node_node_superposition(Quartier* noeud){
	unsigned int uid(noeud->getUid());
	Cercle c1(noeud->getCercle());
	for(size_t i(0); i < noeuds.size(); ++i){
		if(intersection(c1, noeuds[i]->getCercle())){
			cout << error::node_node_superposition(uid,noeuds[i]->getUid());
			errorNoeud = true;
		}
	}
}
		
void reserved_uid(Quartier* noeud){
	unsigned int uid(noeud->getUid());
    if(uid >= no_link){
        cout << error::reserved_uid();
        errorNoeud = true;
    }
}

//Rajoute lien au vecteur du noeud apres les verifications
void ajouterLien(unsigned int uid_d, unsigned int uid_f){
	if(!detected){
		errorLien = false;
		link_vacuum(uid_d, uid_f);
		if(!errorLien){
			multiple_same_link(uid_d, uid_f);
			if(detected) 
				return;
			self_link_node(uid_d, uid_f);
			if(detected) 
				return;
			node_link_superposition(uid_d, uid_f);
			if(detected) 
				return;

			Quartier* noeud_d(accessQuartier(uid_d));
			Quartier* noeud_f(accessQuartier(uid_f));

			if(noeud_d->getIsHouse()){
				max_links(uid_d, uid_f);
				if(detected) 
					return;
			}
			if(noeud_f->getIsHouse()){
				max_links(uid_f, uid_d);
				if(detected) 
					return;
			}
			if(!errorLien){
				noeud_d->ajouterLien(noeud_f);
				noeud_f->ajouterLien(noeud_d);
				numberOfLinks++;
				LinkInfo* current = new LinkInfo(noeud_d, noeud_f);
				links.push_back(current);
			}
		}
	}
}


//Verifications sur les liens
void link_vacuum(unsigned int uid_d, unsigned int uid_f){
	if(!(isInQuartiers(uid_d))){
		cout << error::link_vacuum(uid_d);
		errorLien = true;
		detected = true;
	}else if(!(isInQuartiers(uid_f))){
		cout << error::link_vacuum(uid_f);
		errorLien = true;
		detected = true;
	}
}

void multiple_same_link(unsigned int uid_d, unsigned int uid_f){
	Quartier* noeud1(accessQuartier(uid_d)); 
	
	for(unsigned int i(0); i < noeud1->getLink().size(); i++){
		unsigned int uid1 = noeud1->getLink()[i]->getUid();
		if(uid1== uid_f){
			if(edit_link){
				noeuds[find_index(uid_d)]->delete_link(uid_f);
				noeuds[find_index(uid_f)]->delete_link(uid_d);
				delete_global_link(uid_d, uid_f);
			}else{
				cout << error::multiple_same_link(uid_d, uid_f);
				errorLien = true;
			}
			detected = true;
		}
	}
}

unsigned int find_index(unsigned int uid){
	for(size_t i(0); i < noeuds.size(); ++i){
		if(uid == noeuds[i]->getUid())
			return i;
	}
	return no_link;
}

void self_link_node(unsigned int uid_d, unsigned int uid_f){
	if(uid_d == uid_f){
		cout << error::self_link_node(uid_d);
		errorLien = true;
		detected = true;
	}
}
	
void max_links(unsigned int uid_d, unsigned int uid_f){
	Quartier* noeud1(accessQuartier(uid_d));
	
	if(noeud1->getNbLiens() >= max_link){
		cout << error::max_link(uid_d);
		errorLien = true;
		detected = true;
	} 
}

void node_link_superposition(unsigned int uid_d, unsigned uid_f){
	Quartier* noeud1(accessQuartier(uid_d));
	Quartier* noeud2(accessQuartier(uid_f));
	S2d vecteur{noeud2->getCentre().x - noeud1->getCentre().x, 
				noeud2->getCentre().y - noeud1->getCentre().y};
	Segment link(noeud1->getCentre(), vecteur);
	
	for(unsigned int i(0); i < noeuds.size(); i++){
		if(noeuds[i]->getUid() == uid_d or noeuds[i]->getUid() == uid_f)
			 continue;
		else if(intersection(link, noeuds[i]->getCercle())){
			cout << error::node_link_superposition(noeuds[i]->getUid());
			errorLien = true;
			detected = true;
		}
	}
}
//Ajouter un lien
void Quartier::ajouterLien(Quartier* noeud){
	link.push_back(noeud);
	nbliens++;
}

//Verifie si un quartier est bien dans le vecteur noeuds
bool isInQuartiers(unsigned int uid){
	for(size_t i(0); i < noeuds.size(); ++i){
		if(uid == noeuds[i]->getUid())
			return true;
	}
	return false;
}

//Accede a un quartier dans le vecteur noeud
Quartier* accessQuartier(unsigned int uid){
	for(size_t i(0); i < noeuds.size(); ++i){
		if(uid == noeuds[i]->getUid())
			return noeuds[i];
	}
	return new Quartier(0, 0, 0, 0);
}

//Retourne la capacite d'un type de noeud
unsigned int getTypeCapacity(Quartier* type){
	unsigned int sum(0);
	
	for(unsigned int i(0); i < noeuds.size(); i++){
		if(typeid(*noeuds[i]) == typeid(*type))
			sum += noeuds[i]->getCapacite();
	}
	return sum;
}

//Tri par insertion en fonction d'access time
void sorting(vector<unsigned int>& indices){
	for(unsigned int i(1); i < noeuds.size(); i++){
		double tmp_access(noeuds[indices[i]]->getAccess());
		unsigned int tmp_pos(indices[i]);
		unsigned int j(i);
		
		while(j >= 1 && tmp_access < noeuds[indices[j-1]]->getAccess()){
			indices[j] = indices[j-1];
			j = j-1;
		}
		indices[j] = tmp_pos;
	}
}

//Finds the indice with the fastest access time
unsigned int find_min_access(vector<unsigned int>& ta){
	for(unsigned int i(0); i < noeuds.size(); i++){
		if(noeuds[ta[i]]->getIn())
			return ta[i];
	}
	return no_link;
}

//Displays a vector of unsigned int
void show_vect(vector<unsigned int>& path){
	for(unsigned int i(0); i < path.size(); i++){
		cout << path[i] << " "; 
	}
	cout << endl;
}

//Finds the indice using the uid
unsigned int find_indice(unsigned int uid){
	for(unsigned int i(0); i < noeuds.size(); i++){
		if(noeuds[i]->getUid() == uid)
			return i;
	}
	return no_link;
}

//Computes the access time
double cmpt(unsigned int lv, unsigned int n){
	
	for(unsigned int i(0); i < links.size(); i++){
		if((noeuds[n]->getUid() == links[i]->getUid1() && 
		    noeuds[lv]->getUid() == links[i]->getUid2()) or 
		   (noeuds[n]->getUid() == links[i]->getUid2() && 
			noeuds[lv]->getUid() == links[i]->getUid1())){
			
			double distance(links[i]->getDistance());
			
			double vitesse(0.);
			if(links[i]->getFast()) 
				vitesse = fast_speed;
			else 
				vitesse = default_speed;
			
			double accessTime(distance/vitesse);

			return accessTime;
		}
	}
	return 0;
}

//Return the fastest access time to a production or transport node
double dijkstra(unsigned int d, Quartier* type, vector<Quartier*>& path_nodes, 
				bool activate_short){ 
	vector<unsigned int> ta;
	vector<unsigned int> path;
	
	initDijkstra(d, ta);
	sorting(ta);
	double answer;
	while(noeuds.size() > 0 && nodes_to_observe()){
		unsigned int n(find_min_access(ta));
		
		if(typeid(*noeuds[n]) == typeid(*type)){
			path.push_back(n);
			create_path(n, path);
			answer = noeuds[n]->getAccess();
			break;
		}
		noeuds[n]->setIn(false);
		if(typeid(*noeuds[n]) == typeid(*new Production(0,0,0,0,0))) 
			continue;
		for(unsigned int i(0); i < noeuds[n]->getLink().size(); i++){
			unsigned int lv(find_indice(noeuds[n]->getLink()[i]->getUid()));
			if(noeuds[lv]->getIn()){
				double alt(noeuds[n]->getAccess() +  cmpt(lv, n));
				if(noeuds[lv]->getAccess() > alt){
					noeuds[lv]->setAccess(alt);
					noeuds[lv]->setParent(n);
					sorting(ta);
				}
			}
		}
	}
	return best_time(answer, d, path, path_nodes);
}

void initDijkstra(unsigned int d, vector<unsigned int>& indices){
	for(unsigned int i(0); i < noeuds.size(); i++){
		noeuds[i]->setIn(true);
		noeuds[i]->setAccess(infinite_time);
		noeuds[i]->setParent(no_link);
		indices.push_back(i);
	}
	noeuds[d]->setAccess(0);
}

double best_time(double answer, unsigned int d, vector<unsigned int>& path, 
				 vector<Quartier*>& path_nodes){
	if(path.size() > 0 and checkPathValidity(path, d)){
		for(unsigned int i(0); i < path.size(); i++){
			if(!checkIfInPath(path_nodes, noeuds[path[i]]))
				path_nodes.push_back(noeuds[path[i]]);
		}
		return answer;
	}
	else 
		return infinite_time;
}
//Shows the status of the algorithm
void show_dijk_status(){
	cout << "********* STATUS REPORT ****************" << endl;
	for(unsigned int i(0); i < noeuds.size(); i++){
		cout << i << " has " << noeuds[i]->getParent() 
			 << " as parent, an access time of " << noeuds[i]->getAccess() 
			 << " and a in value of " << noeuds[i]->getIn() << endl;
	}
	cout << "****************************************" << endl;
}

bool checkPathValidity(const vector<unsigned int>& path, unsigned int starter){
	for(unsigned int i(0); i < path.size(); i++){
		if(path[i] == starter) 
			return true;
	}
	return false;
}
//Creates the shortest path and stores it to a vector called path
void create_path(unsigned int n, vector<unsigned int>& path){
	unsigned int parent(noeuds[n]->getParent());
	while(parent != no_link){
		path.insert(path.begin(), parent);
		unsigned int new_n(parent);
		parent = noeuds[new_n]->getParent();
	}
}

//Verifies if they are nodes left to check for the dijktra algorithm
bool nodes_to_observe(){
	for(unsigned int i(0); i < noeuds.size(); i++){
		if(noeuds[i]->getIn()) 
			return true;
	}
	return false;
}

//Verifies that the quartier is at that indes is a logement node
bool checkHouse(unsigned int index){
	if(typeid(*noeuds[index]) == typeid(*new Logement(0,0,0,0,0)))
		return true;
	return false;
}

//Resets all data
void reset_all(){
	for(unsigned int i(0); i < noeuds.size(); i++){
		noeuds[i]->reset_links();
		delete noeuds[i];
		noeuds[i] = NULL;
	}
	noeuds.clear();
	
	for(unsigned int i(0); i < links.size(); i++){
		delete links[i];
		links[i] = NULL;
	}
	links.clear();
}

//Clears links of a node
void Quartier::reset_links(){
	link.clear();
}

//Draws the links of a node
void draw_links_noeud(Quartier* quartier){
	S2d start(quartier->getCentre());
	for(unsigned int i(0); i < quartier->getLink().size(); i++){
		S2d end(quartier->getLink()[i]->getCentre());
		S2d vecteur({end.x - start.x, end.y - start.y});
		Segment current(start, vecteur);
		draw_line(current);
	}
}

void draw_link(Quartier* q1, Quartier* q2, Color c){
	S2d start(q1->getCentre());
	S2d end(q2->getCentre());
	S2d vecteur({end.x - start.x, end.y - start.y});
	Segment current(start, vecteur);
	draw_line(current, c);
}

//Decodeur		
void decodageLigne(string line){
	istringstream data(line);
  
	// états de l'automate de lecture					 
	enum Etat_lecture{NB0,HOUSING,NB1,TRANSPORT,NB2,PRODUCTION,NB3,LINKS,FIN};
  
	static int etat(NB0); // état initial
	static int total(0), i(0);
	unsigned int uid(0), uid2(0);
	double x(0), y(0);
	unsigned int capacite(0);
	
	if(etat == NB0){ 
		data >> total; 
		if(total==0) 
			etat=NB1; 
		else 
			etat=HOUSING;  
	}else if(etat == HOUSING){ 
		data >> uid >> x >> y >> capacite;
		ajouterNoeud(new Logement(uid , x, y, capacite));
		if(i == total-1) 
			etat=NB1;
		i++;                  
	}else if(etat == NB1){
		data >> total; 
		if(total==0) 
			etat=NB2; 
		else 
			etat=TRANSPORT; 
		i = 0;
	}else if(etat == TRANSPORT){ 
		data >> uid >> x >> y >> capacite;
		ajouterNoeud(new Transport(uid , x, y, capacite));
		if(i == total-1) 
			etat=NB2;
		i++;                    
	}else if(etat == NB2){
		data >> total; 
		if(total==0) 
			etat=NB3; 
		else 
			etat=PRODUCTION; 
		i = 0; 
	}else if(etat == PRODUCTION){ 
		data >> uid >> x >> y >> capacite;
		ajouterNoeud(new Production(uid , x, y, capacite));
		if(i == total-1) 
			etat=NB3;
		i++;                   
	}else if(etat == NB3){ 
		data >> total; 
		if(total==0) 
			etat=FIN;
		else 
			etat=LINKS; 
		i = 0; 
	}else if(etat == LINKS){ 
		data >> uid >> uid2;
		ajouterLien(uid, uid2); 
		if(i == total-1) 
			etat = FIN;
		else 
			i++;                    
	}if(etat == FIN){
		if(!detected)
			cout << error::success(); 
		else{
			detected = false;
			reset_all();
		}
		etat = NB0;
		i = 0;
		total = 0; 
	}
}

//Dessiner noeuds
void dessinerNoeudLogement(Logement* l, Color c){
	draw_circle(l->getCercle(), c);
}

void dessinerNoeudTransport(Transport* t, Color c){
	draw_circle(t->getCercle(), c);
	
	Segment verticale({t->getCentre().x, t->getCentre().y - t->getCercle().getRayon()},
					  {0, 2*t->getCercle().getRayon()});
	draw_line(verticale, c);
	
	Segment horizontale({t->getCentre().x-t->getCercle().getRayon(), t->getCentre().y},
						{2*t->getCercle().getRayon(),0});
	draw_line(horizontale, c);
	
	Segment diagonale1({t->getCentre().x - (t->getCercle().getRayon() * cos(M_PI/4)),
						t->getCentre().y - (t->getCercle().getRayon() * sin(M_PI/4))},
					   {2*t->getCercle().getRayon()*cos(M_PI/4),
						2*t->getCercle().getRayon()*sin(M_PI/4)});
	draw_line(diagonale1, c);
	
	Segment diagonale2({t->getCentre().x - (t->getCercle().getRayon() * cos(M_PI/4)),
						t->getCentre().y + (t->getCercle().getRayon() * sin(M_PI/4))},
					   {2*t->getCercle().getRayon()*cos(M_PI/4),
						-2*t->getCercle().getRayon()*sin(M_PI/4)});
	draw_line(diagonale2, c);
}

void dessinerNoeudProduction(Production* p, Color c){
	draw_circle(p->getCercle(), c);
	
	S2d point1({p->getCentre().x - (p->getCercle().getRayon() * 3/4),
				p->getCentre().y + (p->getCercle().getRayon()/8)});
	
	S2d point2({p->getCentre().x + (p->getCercle().getRayon() * 3/4),
				p->getCentre().y + (p->getCercle().getRayon()/8)});
				
	S2d point3({p->getCentre().x + (p->getCercle().getRayon() * 3/4),
				p->getCentre().y - (p->getCercle().getRayon()/8)});
	
	S2d point4({p->getCentre().x - (p->getCercle().getRayon() * 3/4),
				p->getCentre().y - (p->getCercle().getRayon()/8)});
				
	draw_quadrilatere(point1, point2, point3, point4, c);
}

void delete_node(unsigned int index){
	unsigned int uid(noeuds[index]->getUid());
	for(unsigned int j(0); j < noeuds[index]->getLink().size(); j++){ 
		for(unsigned int k(0); k < noeuds[index]->getLink()[j]->getLink().size(); k++){
			if(noeuds[index]->getLink()[j]->getLink()[k]->getUid() == uid){
				noeuds[index]->getLink()[j]->
				delete_link(noeuds[index]->getLink()[j]->getLink()[k]->getUid());
				delete_global_link(uid, noeuds[index]->getLink()[j]->getUid());
			}
		}	
	}
	if(noeuds.size() > 1){
		swap(noeuds[index], noeuds[noeuds.size() - 1]);
		noeuds[noeuds.size() - 1]->reset_links();
		delete noeuds[noeuds.size() - 1];
		noeuds[noeuds.size() - 1] = NULL;
		noeuds.pop_back();
	}else{
		delete noeuds[0];
		noeuds[0] = NULL;
		noeuds.pop_back();
	} 
}

void delete_global_link(unsigned int uid1, unsigned int uid2){
	for(unsigned int i(0); i < links.size(); i++){
		if((links[i]->getUid1() == uid1 and links[i]->getUid2() == uid2) or
		   (links[i]->getUid2() == uid1 and links[i]->getUid1() == uid2)){
			if(links.size() > 1){
				swap(links[i], links[links.size()-1]);
				delete links[links.size()-1];
				links[links.size()-1] = NULL;
				links.pop_back();
			}else{
				delete links[0];
				links[0] = NULL;
				links.pop_back();
			}
		}
	}
}

bool checkIfInPath(const vector<Quartier*>& path, Quartier* q){
	for(unsigned int i(0); i < path.size(); i++){
		if(path[i]->getUid() == q->getUid()) 
			return true;
	} 
	return false;
}

bool checkSequence(const vector<Quartier*>& path, unsigned int i, unsigned int j){
	for(unsigned int k(0); k < path.size(); k++){
		if(path[k]->getUid() == noeuds[i]->getUid()){
			if(k+1 < path.size() && path[k+1]->getUid() == noeuds[j]->getUid())
				return true;
			if(k-1 != no_link && path[k-1]->getUid() == noeuds[j]->getUid())
				return true;
		}
	}
	return false;
}

bool Quartier::isLinked(Quartier* q){
	for(unsigned int i(0); i < link.size(); i++){
		if(link[i]->getUid() == q->getUid())
			return true;
	}
	return false;
}

void setBlockSelection(bool state){
	block_selection = state;
}

bool intersection_dist_min(Cercle c1, Cercle c2){
	if(distance(c1,c2) <= (c1.getRayon() + c2.getRayon() + dist_min)) 
		return true;
	return false;
}

bool intersection_dist_min(Segment s1, Cercle c1){
	if(distance(s1, c1) <= (c1.getRayon() + dist_min)) 
		return true;
	return false;
}

bool intersection_dist_min(Cercle c1, Segment s1){
	return intersection_dist_min(s1,c1);
}

bool checkIntersection(Quartier* noeud, vector<Quartier*> noeuds, Cercle circle){
	for(size_t i(0); i < noeuds.size(); i++){
		if(noeud->getUid() != noeuds[i]->getUid()){
			if(intersection_dist_min(circle,noeuds[i]->getCercle())){
				cout << error::node_node_superposition(noeud->getUid(), 
													   noeuds[i]->getUid());
				return true;
			}
		}
	}
	return false;
}

bool checkCapacity(Quartier* noeud, Cercle circle){
	if(circle.getRayon() < 0){
		cout << error::too_little_capacity(0);
		return true;
	}
	
	unsigned int capacity(pow(circle.getRayon(),2));
	
	if(capacity < min_capacity){
		cout << error::too_little_capacity(capacity);
		return true;
	}else if(capacity > max_capacity){
		cout << error::too_much_capacity(capacity);
		return true;
	}
	return false;
}

bool checkLinkNodeIntersection(Quartier* noeud, vector<Quartier*> noeuds, 
							   Cercle circle){
	for(size_t i(0); i < noeuds.size(); i++){
		vector<Quartier*> liens(noeuds[i]->getLink());
			for(size_t j(0); j < liens.size(); j++){
				double xf(liens[j]->getCentre().x);
				double yf(liens[j]->getCentre().y);
				Segment lien({noeuds[i]->getCentre().x, noeuds[i]->getCentre().y},
						 {xf-noeuds[i]->getCentre().x, yf-noeuds[i]->getCentre().y});
				if(noeud->getUid() != liens[j]->getUid() and 
				   noeud->getUid() != noeuds[i]->getUid() and 
				   intersection_dist_min(lien,circle)){
						cout << error::node_link_superposition(noeud->getUid());
						return true;
				}
			}
		vector<Quartier*> noeud_links(noeud->getLink());
		for(size_t k(0); k < noeud_links.size(); k++){
			double xc(circle.getCentre().x);
			double yc(circle.getCentre().y);
			S2d centre(noeud_links[k]->getCentre());
			Segment link({centre.x, centre.y}, {xc-centre.x, yc-centre.y});
			if(intersection_dist_min(link, noeuds[i]->getCercle()) and 
			   noeud->getUid() != noeuds[i]->getUid() and 
			   noeud_links[k]->getUid() != noeuds[i]->getUid()){
				cout << error::node_link_superposition(noeuds[i]->getUid());
				return true;
			}
		}
	}
	return false;
}

bool checkOneLink(Quartier* noeud_dep, Quartier* noeud_fin){
	S2d centre_dep(noeud_dep->getCentre());
	S2d centre_fin(noeud_fin->getCentre());
	Segment lien({centre_dep.x, centre_dep.y}, 
				 {centre_fin.x - centre_dep.x, centre_fin.y - centre_dep.y});
	
	for(unsigned int i(0); i < noeuds.size(); i++){
		if(noeuds[i]->getUid() == noeud_dep->getUid() or 
		   noeuds[i]->getUid() == noeud_fin->getUid()) 
			continue;
		if(intersection_dist_min(lien, noeuds[i]->getCercle())){
			cout << error::node_link_superposition(noeuds[i]->getUid());
			return true;
		}
	}
	return false;
}

void updateLinks(unsigned int uid){
	for(unsigned int i(0); i < links.size(); i++){
		if(links[i]->getUid1() == uid or links[i]->getUid2() == uid){
			
			unsigned int current1, current2;
			for(unsigned int k(0); k < noeuds.size(); k++){
				if(noeuds[k]->getUid() == links[i]->getUid1()){
					current1 = k;
				} else if (noeuds[k]->getUid() == links[i]->getUid2()){
					current2 = k;
				}
			}
			
			links[i]->setDist(distance(noeuds[current1]->getCercle(), 
									   noeuds[current2]->getCercle()));
			links[i]->setCapacity(min(noeuds[current1]->getCapacite(), 
									  noeuds[current2]->getCapacite()));
		}
	}
}
