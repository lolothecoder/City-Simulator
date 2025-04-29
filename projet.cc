//Architecture Fig 11 b1
#include <iostream>

#include "ville.h"
#include "gui.h"

using namespace std;

int main(int argc, char* argv[]){
	int fake_argc(1);
	if(argc == 2)
		lecture(argv[1]);
	
	auto app = Gtk::Application::create(fake_argc, argv, "org.gtkmm.example");
	Interface win;
	

	//Shows the window and returns when it is closed.
	return app->run(win);
	
}
