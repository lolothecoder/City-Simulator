//Architecture Fig 11 b1
#include <iomanip>
#include <fstream>
#include <sstream>
#include <typeinfo>
#include <stdio.h>
#include <cstring>
#include <cmath>

#include "constantes.h"
#include "ville.h"

using namespace std;

namespace{
	unsigned int current_index(no_link);
	bool selection_made(false); 
	bool link_mode(false);
	Cercle circle_test({0,0},0);
	double diff_rayon(0);
	S2d p1, p2, p_temp;
}

bool getSelectionMade(){return selection_made;}

//lecture d'un fichier
void lecture(const char* nom_fichier){     
	string line;     
	ifstream fichier(nom_fichier);      
	if(!fichier.fail()){ 
	// l’appel de getline filtre aussi les séparateurs         
		while(getline(fichier >> ws,line)){   
			// ligne de commentaire à ignorer, on passe à la suivante            
			if(line[0]=='#')  
				continue; 
			decodageLigne(line);         
		}       
	}    
	else exit(0);
}

//écrit un ficher pour le bouton save
void ecriture(const char* nom_ficher){
	ofstream fichier(nom_ficher);
	vector<Quartier*> noeuds(getNoeud());
	vector<Quartier*> noeuds_organise;
	vector<LinkInfo*> links(getLinks());
	int nbL(0), nbT(0), nbP(0);

	reorganisation_tableau(noeuds, noeuds_organise, nbL, nbT, nbP);
	
	//écrit les noeuds logements en premier
	fichier << nbL << "\t" << "# housing" << "\n";
	for(int i(0); i < nbL; i++) 
		write_node(fichier, noeuds_organise, i);
	fichier << "\n";
	
	//Ensuite les noeuds transports
	fichier << nbT << "\t" << "# transport" << "\n";
	for(int i(nbL); i < (nbL+nbT); i++) 
		write_node(fichier, noeuds_organise, i);	
	fichier << "\n";
	
	//Noeuds Productions
	fichier << nbP << "\t" << "# production" << "\n";
	for(int i(nbT+nbL); i < (nbL + nbT + nbP); i++) 
		write_node(fichier, noeuds_organise, i);
	fichier << "\n";
	
	//En dernier, les liens
	fichier << links.size() << "\t" << "# links" << "\n";
	for(size_t i(0); i < links.size(); i++)
		fichier << "\t" << links[i]->getUid1() << " " << links[i]->getUid2() << "\n";
}	

//Writes all the info of a node
void write_node(ofstream& fichier, vector<Quartier*>& noeuds_organise, unsigned int i){
	fichier << "\t" << noeuds_organise[i]->getUid() << " " 
		    << noeuds_organise[i]->getCentre().x << " " 
		    << noeuds_organise[i]->getCentre().y << " " 
		    << noeuds_organise[i]->getCapacite() << "\n";	
}

double getENJ(){
	double enj(0);
	int logementCap(getTypeCapacity(new Logement(0, 0, 0, 0, 0)));
	int transportCap(getTypeCapacity(new Transport(0, 0, 0, 0, 0)));
	int productionCap(getTypeCapacity(new Production(0, 0, 0, 0, 0)));
	
	if(getNoeudsSize() == 0) 
		enj = 0;
	else{
		double numerator(logementCap - transportCap - productionCap);
		double denominator(logementCap + transportCap + productionCap);
		enj = numerator/denominator;
	}

	return enj;
}

double getCI(){
	double ci(0);
	vector<LinkInfo*> links(getLinks());
	
	for(unsigned int i(0); i < links.size(); i++){
		if(links[i]->getFast()) 
			ci += links[i]->getDistance() * fast_speed * links[i]->getCapacity(); 
		else 
			ci += links[i]->getDistance() * default_speed * links[i]->getCapacity(); 
	}
	
	return ci;
}

double getMTA(){
	double mta(infinite_time + infinite_time);
	double logementCounter(0.);
	double temp_mta(0.);
	vector<Quartier*> path_nodes;
	
	if(getNoeudsSize() == 0){
		mta = 0;
		return mta;
	}
	for(unsigned int i(0); i < getNoeudsSize(); i++){
		if(checkHouse(i)){
			temp_mta += dijkstra(i, new Transport(0,0,0,0,0), path_nodes) +
						dijkstra(i, new Production(0,0,0,0,0), path_nodes);
			logementCounter = logementCounter + 1.;
		}
	}
	
	if(logementCounter == 0){
		mta = 0;
		return mta;
	}
	
	mta = temp_mta / logementCounter;
	return mta;
}

//Concatenate a string and a double as a string
string conv_to_string(string s, double x){
	ostringstream streamObj;

	streamObj << x;
	string strObj = streamObj.str();
	string s3 = s + strObj;
	return s3; 
}

void draw_links(const vector<Quartier*>& path_prod, 
				const vector<Quartier*>& path_transp, bool shortest){
	vector<Quartier*> noeuds (getNoeud());
	for(unsigned int i(0); i < noeuds.size(); i++){
		for(unsigned int j(0) ; j < noeuds.size(); j++){
			if(i == j) 
				continue;
			else if(noeuds[i]->isLinked(noeuds[j])){
				bool checkProd(false);
				bool checkTransp(false);
				bool inProd(checkIfInPath(path_prod, noeuds[i]) && 
							checkIfInPath(path_prod, noeuds[j]));
				bool inTransp(checkIfInPath(path_transp, noeuds[i]) && 
							  checkIfInPath(path_transp, noeuds[j]));
				
				if(inProd) 
					checkProd = checkSequence(path_prod, i, j);
				if(inTransp) 
					checkTransp = checkSequence(path_transp, i, j);
				
				if(shortest && (inProd or inTransp) && (checkProd or checkTransp)) 
					draw_link(noeuds[i], noeuds[j], Green);
				else 
					draw_link(noeuds[i], noeuds[j], Black);
			}
		}
	}
}

void draw_noeuds(const vector<Quartier*>& path_prod, 
				 const vector<Quartier*>& path_transp, bool shortest){
	vector<Quartier*> noeuds(getNoeud());
	
	for(unsigned int i(0); i < noeuds.size(); i++){
		if(typeid(*noeuds[i]) == typeid(*new Logement(0,0,0,0,0))){
			if(i == current_index)
				dessinerNoeudLogement(dynamic_cast<Logement*>(noeuds[i]), Red);
			else if(shortest && (checkIfInPath(path_prod, noeuds[i]) or 
					checkIfInPath(path_transp, noeuds[i])))
				dessinerNoeudLogement(dynamic_cast<Logement*>(noeuds[i]), Green);
			else 
				dessinerNoeudLogement(dynamic_cast<Logement*>(noeuds[i]));
		} 
		else if(typeid(*noeuds[i]) == typeid(*new Production(0,0,0,0,0))){
			if(i == current_index)
				dessinerNoeudProduction(dynamic_cast<Production*>(noeuds[i]), Red);
			else if(shortest && (checkIfInPath(path_prod, noeuds[i]) or 
					checkIfInPath(path_transp, noeuds[i]))) 
				dessinerNoeudProduction(dynamic_cast<Production*>(noeuds[i]), Green);
			else 
				dessinerNoeudProduction(dynamic_cast<Production*>(noeuds[i]));
		} 
		else if(typeid(*noeuds[i]) == typeid(*new Transport(0,0,0,0,0))){
			if(i == current_index)
				dessinerNoeudTransport(dynamic_cast<Transport*>(noeuds[i]), Red);
			else if(shortest && (checkIfInPath(path_prod, noeuds[i]) or 
					checkIfInPath(path_transp, noeuds[i])))
				dessinerNoeudTransport(dynamic_cast<Transport*>(noeuds[i]), Green);
			else 
				dessinerNoeudTransport(dynamic_cast<Transport*>(noeuds[i]));
		}
	}
}

//Permet d'avoir un tableau réorganisé avec les noeuds logements,
//suivis par transports, et ensuite production
void reorganisation_tableau(vector<Quartier*> noeuds, 
							vector<Quartier*>& noeuds_organise, 
							int& nbL, int& nbT, int& nbP){
	for(size_t i(0); i < noeuds.size(); i++){
		if(typeid(*noeuds[i]) == typeid(*new Logement(0,0,0,0,0))){
			noeuds_organise.push_back(noeuds[i]);
			nbL += 1;
		}
	}
	for(size_t i(0); i < noeuds.size(); i++){
		if(typeid(*noeuds[i]) == typeid(*new Transport(0,0,0,0,0))){
			noeuds_organise.push_back(noeuds[i]);
			nbT += 1;
		}
	}
	for(size_t i(0); i < noeuds.size(); i++){
		if(typeid(*noeuds[i]) == typeid(*new Production(0,0,0,0,0))){
			noeuds_organise.push_back(noeuds[i]);
			nbP += 1;
		}
	}
}

void draw_shortest_path(){
	vector<Quartier*> noeuds(getNoeud());
	if(selection_made && 
	   typeid(*noeuds[current_index]) == typeid(*new Logement(0,0,0,0,0))){
		vector<Quartier*> path_prod;
		vector<Quartier*> path_transp;
		dijkstra(current_index, new Production(0,0,0,0,0), path_prod, true);
		dijkstra(current_index, new Transport(0,0,0,0,0), path_transp, true);
		draw_links(path_prod, path_transp, true);
		draw_noeuds(path_prod, path_transp, true);
	}else{
		draw_links();
		draw_noeuds();
	}
}

//Verifies if a node is clicked or double clicked
bool checkIfSelected(double xm, double ym, bool delete_noeud){
	vector<Quartier*> noeuds(getNoeud());
	if(link_mode)
		setBlockSelection(true);
	else
		setBlockSelection(false);

	for(unsigned int i(0); i < noeuds.size(); i++){
		if(noeuds[i]->inNoeud(xm, ym)){
			if(link_mode){
				if(!checkOneLink(noeuds[current_index], noeuds[i])) 
					ajouterLien(noeuds[current_index]->getUid(), noeuds[i]->getUid());
				setDetected(false);
				return true;
			}else{
				if(i == current_index and selection_made and delete_noeud){
					delete_node(current_index);
					reset_selected();
				}else{
					current_index = i;
					selection_made = true;
				}
				return true;
			}
		}
	}
	if(link_mode) 
		return true;
	else 
		return false;
}

void reset_selected(){
	reset_selection();	
	selection_made = false;
	current_index = no_link;
}

void ajouterNoeudDessine(Quartier* noeud){
	vector<Quartier*> noeuds(getNoeud());

	if(!checkLinkNodeIntersection(noeud, noeuds, noeud->getCercle()) and
	   !checkIntersection(noeud, noeuds, noeud->getCercle()))
		ajouterNoeud(noeud);
}

void setLinkMode(bool state){
	link_mode = state;
	setEditLink(state);
}

void updateCercle(double x, double y, double rayon, Quartier* noeud){
	circle_test.setCentre({x,y});
	circle_test.setRayon(rayon);

	vector<Quartier*> noeuds(getNoeud());
	
	if(!checkIntersection(noeud, noeuds, circle_test) and 
	   !checkCapacity(noeud, circle_test) and 
	   !checkLinkNodeIntersection(noeud, noeuds, circle_test)){
		noeud->setX(x);
		noeud->setY(y);
	
		noeud->setCentre({x,y});
		noeud->setCapacite(pow(rayon,2));
		noeud->setCercle({noeud->getCentre(), sqrt(noeud->getCapacite())});
		updateLinks(noeud->getUid());
	}
}

unsigned int assign_uid(){
	
	vector<Quartier*> liste_noeuds(getNoeud());
	unsigned int max_uid;
	
	if(liste_noeuds.size() != 0)
		max_uid = liste_noeuds[0]->getUid();
	else
		max_uid = 0;

	for(size_t i(0); i < liste_noeuds.size(); ++i){
		if(liste_noeuds[i]->getUid() > max_uid and (max_uid+1) != no_link)
			max_uid = liste_noeuds[i]->getUid();
		else if((max_uid+1) >= no_link)
			break;
		
		if(i >= (liste_noeuds.size()-1))
			max_uid += 1;
		
	}
	if((max_uid+1) >= no_link){
		max_uid = 0;
		for(size_t i(0); i < liste_noeuds.size(); ++i){
			if(max_uid == liste_noeuds[i]->getUid()){
				max_uid += 1;
				i = -1;
			}
		}
	}
	return max_uid;
} 


void rayon_click_initial(bool& can_add_circle, bool link_activated, double xm, 
						 double ym){
	vector<Quartier*> liste_noeuds(getNoeud());
	for(size_t i(0); i < liste_noeuds.size(); i++){
		if(liste_noeuds[i]->getSelected()){
			can_add_circle = false;
			if(!link_activated)
				reset_rayon({xm, ym}, liste_noeuds[i]->getCentre());

			break;
		}
	}
}

void rayon_click_final(double xm, double ym){
	vector<Quartier*> liste_noeuds(getNoeud());
	for(size_t i(0); i < liste_noeuds.size(); i++){
		if(liste_noeuds[i]->getSelected()){
			finalise_rayon({xm, ym});
			updateCercle(liste_noeuds[i]->getCentre().x, 
						 liste_noeuds[i]->getCentre().y,
						 liste_noeuds[i]->getCercle().getRayon()+diff_rayon, 
						 liste_noeuds[i]);
				if(diff_rayon == 0)
					reset_selected();
				
				break;
		}
	}
}

void reset_rayon(S2d point, S2d centre){
	p1 = point;
	p2 = point;
	p_temp = centre;
	
	p1.x-=p_temp.x;
	p1.y-=p_temp.y;
	p2.x-=p_temp.x;
	p2.y-=p_temp.y;
}

void finalise_rayon(S2d point){
	p2 = point;
	
	p2.x-=p_temp.x;
	p2.y-=p_temp.y;
	
	if(distance({p1.x+p_temp.x, p1.y+p_temp.y},p_temp) > 
	   distance({p2.x+p_temp.x, p2.y+p_temp.y},p_temp))
		diff_rayon = -distance(p1,p2);
	else
		diff_rayon = distance(p1,p2);
}

void change_position(double xm, double ym){
	vector<Quartier*> liste_noeuds(getNoeud());
	for(size_t i(0); i < liste_noeuds.size(); i++){
		if(liste_noeuds[i]->getSelected()){
			updateCercle(xm, ym, liste_noeuds[i]->getCercle().getRayon(),
						 liste_noeuds[i]);
			break;
		}
	}
}			

bool mouse_click_on_node(S2d point_dep){
	vector<Quartier*> liste_noeuds(getNoeud());
	for(size_t i(0); i < liste_noeuds.size(); i++){
		if(liste_noeuds[i]->getSelected() and 
		   distance(point_dep, liste_noeuds[i]->getCercle().getCentre()) <
		   liste_noeuds[i]->getCercle().getRayon())
			return true;
	}
	return false;
}
