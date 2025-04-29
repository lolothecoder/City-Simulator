//Architecture Fig 11 b1
#ifndef GUI_HEADER_H
#define GUI_HEADER_H

#include <gtkmm.h>

class MyArea:public Gtk::DrawingArea{
public:
	MyArea();
	virtual ~MyArea();
	void refresh();
	void draw();
	void clear();
	void setShort_p(bool state);
	bool getShort_path();
	void addCercle(S2d centre);
	void reset_rayon(S2d point, S2d centre);
	void finalise_rayon(S2d Point);

protected:
	//Override default signal handler:
	bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
private:
	bool empty;
	bool short_path;
	vector<Quartier*> noeuds_dessine;
};

class Interface:public Gtk::Window{
public:
	Interface();
	virtual ~Interface();
	//méthodes surchargées
	void ajouterGraphique(Gtk::Box& obj1, Gtk::Label& obj2, Gtk::Label& obj3, 
						  Gtk::Label& obj4);
	void ajouterGraphique(Gtk::Container& obj1, Gtk::Container& obj2, 
						  Gtk::Container& obj3, Gtk::Container& obj4, 
						  Gtk::Container& obj5);
	void ajouterGraphique(Gtk::Container& obj1, Gtk::Container& obj2, 
						  Gtk::Container& obj3, Gtk::Container& obj4, 
						  Gtk::Container& obj5, Gtk::Container& obj6,
						  Gtk::Container& obj7, Gtk::Container& obj8);
	void setBorderWidth(Gtk::Box& box1, Gtk::Box& box2, Gtk::Box& box3, 
						Gtk::Box& box4);
	void joinGroup(Gtk::RadioButton& rb1, Gtk::RadioButton& rb2,
				   Gtk::RadioButton& rb3);
	void keyboard_zoom(double zf);
	void set_criterias();
		
protected:
	//Signal handlers
	void on_button_clicked_exit();
	void on_button_clicked_new();
	void on_button_clicked_open();
	void on_button_clicked_save();
	void on_button_clicked_shortest_path();
	void on_button_clicked_zoom_in();
	void on_button_clicked_zoom_out();
	void on_button_clicked_zoom_reset();
	void on_button_clicked_edit_link();
	void on_button_clicked_housing();
	void on_button_clicked_transport();
	void on_button_clicked_production();
	
	// Mouse event signal handlers:
	bool on_button_press_event(GdkEventButton* event);
	bool on_button_release_event(GdkEventButton* event);
	
	//Keyboard event signal handlers
	bool on_key_press_event(GdkEventKey* key_event);
	
	//Child widgets
	Gtk::Box m_Box, m_Box_Area, m_Box_Top, m_Box1, m_Box2, m_Box3, m_Box4;
	Gtk::Box m_Box_sous1, m_Box_sous2, m_Box_sous3, m_Box_sous4;
	Gtk::Label  m_Label_ENJ;
	Gtk::Label  m_Label_CI;
	Gtk::Label  m_Label_MTA;
	Gtk::Label  m_Label_Informations;
	Gtk::Label  m_Label_zoom_factor;
	Gtk::Button m_Button_exit;
	Gtk::Button m_Button_new;
	Gtk::Button m_Button_open;
	Gtk::Button m_Button_save;
	Gtk::ToggleButton m_Btn_sp;
	Gtk::Button m_Btn_zi;
	Gtk::Button m_Btn_zo;
	Gtk::Button m_Btn_zr;
	Gtk::ToggleButton m_Btn_el;
	Gtk::RadioButton m_rb_h;
	Gtk::RadioButton m_rb_t;
	Gtk::RadioButton m_rb_p;
	Gtk::Frame m_Frame1;
	Gtk::Frame m_Frame2;
	Gtk::Frame m_Frame3;
	Gtk::Frame m_Frame4;
	MyArea m_Area;
};

#endif
