//Architecture Fig 11 b1
#include <iostream>
#include <cairomm/context.h>

#include "graphic_gui.h"
#include "constantes.h"
#include "ville.h"
#include "gui.h"


using namespace std;

namespace{
	double xMin(-dim_max);
	double xMax(dim_max);
	double yMin(-dim_max);
	double yMax(dim_max);
	double zoom_factor(1.);
	double epsilon(0.01);
	bool housing_bouton_active(true);
	bool transport_bouton_active(false);
	bool production_bouton_active(false);
	bool can_add_circle(true);
	bool link_activated(false);
	S2d point_dep({0,0});
	int width;
	int height;
}
	

Interface::Interface():
	m_Box(Gtk::ORIENTATION_HORIZONTAL, 10),
	m_Box_Area(Gtk::ORIENTATION_VERTICAL, 10),
	m_Box_Top(Gtk::ORIENTATION_VERTICAL, 5),
	m_Box1(Gtk::ORIENTATION_VERTICAL, 10),
	m_Box2(Gtk::ORIENTATION_VERTICAL, 10),
	m_Box3(Gtk::ORIENTATION_VERTICAL, 10),
	m_Box4(Gtk::ORIENTATION_VERTICAL, 10),
	m_Box_sous1(Gtk::ORIENTATION_VERTICAL, 5),
	m_Box_sous2(Gtk::ORIENTATION_VERTICAL, 5),
	m_Box_sous3(Gtk::ORIENTATION_VERTICAL, 5),
	m_Box_sous4(Gtk::ORIENTATION_VERTICAL, 5),
	m_Label_ENJ(conv_to_string("ENJ : ", getENJ())),
	m_Label_CI(conv_to_string("CI : ", getCI())),
	m_Label_MTA(conv_to_string("MTA : ", getMTA())),
	m_Label_zoom_factor(conv_to_string("zoom: x",zoom_factor)),
	m_Button_exit("exit"),
	m_Button_new("new"),
	m_Button_open("open"),
	m_Button_save("save"),
	m_Btn_sp("shortest path"),
	m_Btn_zi("zoom in"),
	m_Btn_zo("zoom out"),
	m_Btn_zr("zoom reset"),
	m_Btn_el("edit link"),
	m_rb_h("housing"),
	m_rb_t("transport"),
	m_rb_p("production"),
	m_Frame1("General"),
	m_Frame2("Display"),
	m_Frame3("Editor"),
	m_Frame4("Informations")
{
	add(m_Box);
	m_Box.pack_start(m_Box_Top, Gtk::PACK_SHRINK);
	m_Box.pack_start(m_Box_Area);
	m_Area.set_size_request(default_drawing_size, default_drawing_size);
	m_Box_Area.pack_start(m_Area);
	
	ajouterGraphique(m_Box_Top, m_Box1, m_Box2, m_Box3, m_Box4);
	ajouterGraphique(m_Box1, m_Box2, m_Box3, m_Box4, m_Frame1, m_Frame2, m_Frame3,
					 m_Frame4);
	ajouterGraphique(m_Box_sous1, m_Button_exit, m_Button_new, m_Button_open, 
					 m_Button_save);
	ajouterGraphique(m_Box_sous2, m_Btn_sp, m_Btn_zi, m_Btn_zo, m_Btn_zr);
	m_Box_sous2.pack_start(m_Label_zoom_factor, Gtk::PACK_SHRINK);
	ajouterGraphique(m_Box_sous3, m_Btn_el, m_rb_h, m_rb_t, m_rb_p);
	ajouterGraphique(m_Box_sous4, m_Label_ENJ, m_Label_CI, m_Label_MTA);
	ajouterGraphique(m_Frame1, m_Frame2, m_Frame3, m_Frame4, m_Box_sous1, m_Box_sous2,
					 m_Box_sous3, m_Box_sous4);
	joinGroup(m_rb_h, m_rb_t, m_rb_p);
	setBorderWidth(m_Box1, m_Box2, m_Box3, m_Box4);
	//Signal handlers
	m_Button_exit.signal_clicked().connect(sigc::mem_fun(*this, &Interface::
														 on_button_clicked_exit));
    m_Button_new.signal_clicked().connect(sigc::mem_fun(*this, &Interface::
														on_button_clicked_new));
    m_Button_open.signal_clicked().connect(sigc::mem_fun(*this, &Interface::
														 on_button_clicked_open));
    m_Button_save.signal_clicked().connect(sigc::mem_fun(*this, &Interface::
														 on_button_clicked_save));
    m_Btn_sp.signal_clicked().connect(sigc::mem_fun(*this, &Interface::
													on_button_clicked_shortest_path));
    m_Btn_zi.signal_clicked().connect(sigc::mem_fun(*this, &Interface::
													on_button_clicked_zoom_in));
    m_Btn_zo.signal_clicked().connect(sigc::mem_fun(*this, &Interface::
													on_button_clicked_zoom_out));
    m_Btn_zr.signal_clicked().connect(sigc::mem_fun(*this, &Interface::
													on_button_clicked_zoom_reset));
    m_Btn_el.signal_clicked().connect(sigc::mem_fun(*this, &Interface::
													on_button_clicked_edit_link));
    m_rb_h.signal_clicked().connect(sigc::mem_fun(*this, &Interface::
												  on_button_clicked_housing));
    m_rb_t.signal_clicked().connect(sigc::mem_fun(*this, &Interface::
												  on_button_clicked_transport));
    m_rb_p.signal_clicked().connect(sigc::mem_fun(*this, &Interface::
												  on_button_clicked_production));    
    show_all_children();
}

//Destructeur
Interface::~Interface(){}

//Signal handlers
void Interface::on_button_clicked_exit(){
	exit(0);
}

void Interface::set_criterias(){
	m_Label_ENJ.set_text(conv_to_string("ENJ : ", getENJ()));
	m_Label_CI.set_text(conv_to_string("CI : ", getCI()));
	m_Label_MTA.set_text(conv_to_string("MTA : ", getMTA()));
}
void Interface::on_button_clicked_new(){
	reset_all();
	set_criterias();
	m_Area.clear();
}

void Interface::on_button_clicked_open(){
	Gtk::FileChooserDialog dialog("Please choose a file",
								  Gtk::FILE_CHOOSER_ACTION_OPEN);
	dialog.set_transient_for(*this);
	
	//Add response buttons the the dialog:
	dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);	
	dialog.add_button("_Open", Gtk::RESPONSE_OK);
	
	//Show the dialog and wait for a user response:
	int result = dialog.run();
	
	//Handle the response:
	switch(result){
		case(Gtk::RESPONSE_OK):{

			string stringfilename = dialog.get_filename();
			const char  *cstr = stringfilename.c_str();
			
			reset_all();
			lecture(cstr);
			set_criterias();
			reset_selected();
			m_Area.draw();
			break;
		}
		case(Gtk::RESPONSE_CANCEL):{
			break;
		}
		default:{
			break;
		}
	}
}

void Interface::on_button_clicked_save(){
	Gtk::FileChooserDialog dialog("Please choose a file",
								  Gtk::FILE_CHOOSER_ACTION_SAVE);
	dialog.set_transient_for(*this);
	
	//Add response buttons the the dialog:
	dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);	
	dialog.add_button("_Save", Gtk::RESPONSE_OK);
	
	//Show the dialog and wait for a user response:
	int result = dialog.run();
	
	//Handle the response:
	switch(result){
		case(Gtk::RESPONSE_OK):{
			//Notice that this is a std::string, not a Glib::ustring.
			string stringfilename = dialog.get_filename();
			const char *cstr = stringfilename.c_str();
			ecriture(cstr);
			break;
		}
		case(Gtk::RESPONSE_CANCEL):{
			break;
		}
		default:{
			break;
		}
	}
}

void Interface::on_button_clicked_shortest_path(){
	if(m_Btn_sp.get_active())
		m_Area.setShort_p(true);
	else 
		m_Area.setShort_p(false);
	
	m_Area.draw();
}

void Interface::on_button_clicked_zoom_in(){

	if((zoom_factor + delta_zoom) <= (max_zoom+epsilon)){
		zoom_factor += delta_zoom;

		m_Label_zoom_factor.set_text(conv_to_string("zoom: x",zoom_factor));
		width /= zoom_factor;
		height /= zoom_factor;
		set_width(width);
		set_height(height);
		
		xMin = -dim_max;
		xMax = dim_max;
		yMin = -dim_max;
		yMax = dim_max;
		
		xMin /= zoom_factor;
		xMax /= zoom_factor;
		yMin /= zoom_factor;
		yMax /= zoom_factor;

		set_coordinates(xMin, xMax, yMin, yMax);

		m_Area.draw();
	}
}

void Interface::on_button_clicked_zoom_out(){
	
	if((zoom_factor - delta_zoom) >= min_zoom){
		zoom_factor -= delta_zoom;
		m_Label_zoom_factor.set_text(conv_to_string("zoom: x",zoom_factor));
		width /= zoom_factor;
		height /= zoom_factor;
		set_width(width);
		set_height(height);
		
		xMin = -dim_max;
		xMax = dim_max;
		yMin = -dim_max;
		yMax = dim_max;
		
		xMin /= zoom_factor;
		xMax /= zoom_factor;
		yMin /= zoom_factor;
		yMax /= zoom_factor;
		
		set_coordinates(xMin, xMax, yMin, yMax);
		
		m_Area.draw();
	}
}

void Interface::on_button_clicked_zoom_reset(){
	zoom_factor = 1.;
	m_Label_zoom_factor.set_text(conv_to_string("zoom: x",zoom_factor));
	
	xMin = -dim_max;
	xMax = dim_max;
	yMin = -dim_max;
	yMax = dim_max;
	
	set_coordinates(xMin, xMax, yMin, yMax);
	
	m_Area.draw();
}

void Interface::on_button_clicked_edit_link(){
	if(m_Btn_el.get_active() and getSelectionMade()){
		setLinkMode(true);
		link_activated = true;
	}else{
		if(!getSelectionMade())
			cout << "Please select a node first" << endl;
		
		m_Btn_el.set_active(false); //Calls the on clicked again for some reason 
		setLinkMode(false);
		link_activated = false;
	}
	m_Area.draw();
}

void Interface::on_button_clicked_housing(){
	
	if(m_rb_h.get_active())
		housing_bouton_active = true;
	else
		housing_bouton_active = false;
}

void Interface::on_button_clicked_transport(){
	
	if(m_rb_t.get_active())
		transport_bouton_active = true;
	else
		transport_bouton_active = false;
}

void Interface::on_button_clicked_production(){
	
	if(m_rb_p.get_active())
		production_bouton_active = true;
	else
		production_bouton_active = false;
}

//Mouse event signal handlers:
bool Interface::on_button_press_event(GdkEventButton* event){
	if(event->type == GDK_BUTTON_PRESS){
		double clic_x = event->x ;
		double clic_y = event->y ;
		//origin of the drawing area
		double origin_x = m_Area.get_allocation().get_x();
		double origin_y = m_Area.get_allocation().get_y();
		//Coordinates in the drawing frame
		double xd(clic_x - origin_x);
		double yd(clic_y - origin_y);
		// get width and height of drawing area
		double width = m_Area.get_allocation().get_width();
		double height= m_Area.get_allocation().get_height();

		double xm(xModel(xd, width));
		double ym(yModel(yd, height));
		point_dep = {xm, ym};

		if(clic_x >= origin_x && clic_x <= origin_x + width &&
		   clic_y >= origin_y && clic_y <= origin_y + height){ 			
			if(event->button == 1){
				if(checkIfSelected(xm, ym, true));
				else{
					rayon_click_initial(can_add_circle, link_activated, xm, ym);
					if(can_add_circle){
						m_Area.addCercle({xm,ym});
						reset_selected();
					}
				}
				can_add_circle = true;
				set_criterias();
			}else if(event->button == 3){
				change_position(xm,ym);
				set_criterias();
			}
			m_Area.draw();
		}
	}
	return true;
}


bool Interface::on_button_release_event(GdkEventButton* event){
	
	if(event->type == GDK_BUTTON_RELEASE){
		// raw mouse coordinates in the window frame
		double clic_x = event->x ;
		double clic_y = event->y ;

		// origin of the drawing area
		double origin_x = m_Area.get_allocation().get_x();
		double origin_y = m_Area.get_allocation().get_y();
		double xd(clic_x - origin_x);
		double yd(clic_y - origin_y);
		
		// get width and height of drawing area
		double width = m_Area.get_allocation().get_width();
		double height= m_Area.get_allocation().get_height();
		
		double xm(xModel(xd, width));
		double ym(yModel(yd, height));
		
		if(clic_x >= origin_x && clic_x <= origin_x + width &&
		   clic_y >= origin_y && clic_y <= origin_y + height){ 
			if(event->button == 1){
				if(!link_activated and !mouse_click_on_node(point_dep))
					rayon_click_final(xm, ym);
				
				set_criterias();
				m_Area.draw();
			}
		}
	}
	return true;
}

bool Interface::on_key_press_event(GdkEventKey* key_event){
	if(key_event->type == GDK_KEY_PRESS){
		switch(gdk_keyval_to_unicode(key_event->keyval)){				
			case 'r':{
				zoom_factor = 1.;
				m_Label_zoom_factor.set_text(conv_to_string("zoom: x",zoom_factor));
				xMin = -dim_max;
				xMax = dim_max;
				yMin = -dim_max;
				yMax = dim_max;		
				set_coordinates(xMin, xMax, yMin, yMax);
				break;
			}
			case 'o':{
				if((zoom_factor - delta_zoom) >= min_zoom){
					zoom_factor -= delta_zoom;
					m_Label_zoom_factor.set_text(conv_to_string("zoom: x",zoom_factor));
					keyboard_zoom(zoom_factor);
				}
				break;
			}
			case 'i':{
				if((zoom_factor + delta_zoom) <= (max_zoom+epsilon)){
					zoom_factor += delta_zoom;
					m_Label_zoom_factor.set_text(conv_to_string("zoom: x",zoom_factor));
					keyboard_zoom(zoom_factor);
				}
				break;
			}
		}
		m_Area.draw();
	}
	return Gtk::Window::on_key_press_event(key_event);
}

//Permet d'ajouter des boîtes ou des boutons à une boîte supérieure
void Interface::ajouterGraphique(Gtk::Box& box1, Gtk::Label& obj2, 
								 Gtk::Label& obj3, Gtk::Label& obj4){
	box1.pack_start(obj2, Gtk::PACK_SHRINK);
	box1.pack_start(obj3, Gtk::PACK_SHRINK);
	box1.pack_start(obj4, Gtk::PACK_SHRINK);
}
	

void Interface::ajouterGraphique(Gtk::Container& obj1, Gtk::Container& obj2, 
								 Gtk::Container& obj3, Gtk::Container& obj4, 
								 Gtk::Container& obj5){
	obj1.add(obj2);
	obj1.add(obj3);
	obj1.add(obj4);
	obj1.add(obj5);
							
}

void Interface::ajouterGraphique(Gtk::Container& obj1, Gtk::Container& obj2, 
								 Gtk::Container& obj3, Gtk::Container& obj4, 
								 Gtk::Container& obj5, Gtk::Container& obj6,
								 Gtk::Container& obj7, Gtk::Container& obj8){
	obj1.add(obj5);
	obj2.add(obj6);
	obj3.add(obj7);
	obj4.add(obj8);
}

void Interface::setBorderWidth(Gtk::Box& box1, Gtk::Box& box2, 
							   Gtk::Box& box3, Gtk::Box& box4){
	box1.set_border_width(5);
	box2.set_border_width(5);
	box3.set_border_width(5);
	box4.set_border_width(5);
}

//Pour que les Radiobuttons ne soient pas indépendants
void Interface::joinGroup(Gtk::RadioButton& rb1, Gtk::RadioButton& rb2,
						  Gtk::RadioButton& rb3){
	rb2.join_group(rb1);
	rb3.join_group(rb1);
}

void Interface::keyboard_zoom(double zf){
	width /= zoom_factor;
	height /= zoom_factor;
	set_width(width);
	set_height(height);
					
	xMin = -dim_max;
	xMax = dim_max;
	yMin = -dim_max;
	yMax = dim_max;
					
	xMin /= zoom_factor;
	xMax /= zoom_factor;
	yMin /= zoom_factor;
	yMax /= zoom_factor;

	set_coordinates(xMin, xMax, yMin, yMax);
}

bool MyArea::getShort_path(){
	return short_path;
}
//Constructeur MyArea
MyArea::MyArea():empty(false), short_path(false) {}

//Destructeur MyArea
MyArea::~MyArea(){}

//Méthodes MyArea
void MyArea::draw(){
	empty = false;
	refresh();
}

void MyArea::clear(){
	empty = true;
	refresh();
}

void MyArea::refresh(){
	auto win = get_window();
	if(win){
		Gdk::Rectangle r(0,0, get_allocation().get_width(), 
						 get_allocation().get_height());
								
		win->invalidate_rect(r,false);
	}
}

void MyArea::setShort_p(bool state){
	if(not empty){
		this -> short_path = state;
		refresh();
	}
}

void MyArea::addCercle(S2d centre){

	unsigned int new_uid(assign_uid());
	
	if(housing_bouton_active)
		ajouterNoeudDessine(new Logement(new_uid, centre.x, centre.y, 
									     min_capacity));
	else if(transport_bouton_active)
		ajouterNoeudDessine(new Transport(new_uid, centre.x, centre.y, 
											   min_capacity));
	else
		ajouterNoeudDessine(new Production(new_uid, centre.x, centre.y, 
												min_capacity));
	draw();
}

bool MyArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr){
	Gtk::Allocation allocation = get_allocation();

	width = allocation.get_width();	
	height = allocation.get_height();
	
	cr->set_source_rgb(1,1,1);
	cr->paint();
	
	set_width(width);
	set_height(height);
	set_dim_max(dim_max);
	
	graphic_set_context(cr);
	
	double new_aspect_ratio((double)width/height);
	if(new_aspect_ratio > 1) 
		xMax = (double(width)/height)*(yMax - yMin) + xMin;		
	else if(new_aspect_ratio < 1)
		yMin = yMax - (double(height)/width)*(xMax - xMin);		  
	else{
		xMin = -dim_max/zoom_factor;
		xMax = dim_max/zoom_factor;
		yMin = -dim_max/zoom_factor;
		yMax = dim_max/zoom_factor;
	}
	
	set_coordinates(xMin, xMax, yMin, yMax);
	
	if(not empty and getNoeudsSize() != 0){
		if(short_path and getSelectionMade())
			draw_shortest_path();
		else{
			draw_links();
			draw_noeuds();
		}
	}
	graphic_set_context(cr);
	return true;
}
