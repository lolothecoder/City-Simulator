OUT = projet
CXX = g++
CXXFLAGS = -Wall -std=c++11 
CXXFILES = projet.cc ville.cc noeud.cc tools.cc error.cc gui.cc graphic.cc
LINKING = `pkg-config --cflags gtkmm-3.0`
LDLIBS = `pkg-config --libs gtkmm-3.0`
OFILES = projet.o ville.o noeud.o tools.o error.o gui.o graphic.o

all: $(OUT)

projet.o: projet.cc ville.h gui.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)

ville.o: ville.cc ville.h error.h noeud.h tools.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)

noeud.o: noeud.cc noeud.h tools.h error.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)
	
tools.o: tools.cc tools.h graphic.h graphic_color.h 
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)

error.o: error.cc error.h 
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)
	
gui.o: gui.cc ville.h tools.h graphic_gui.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)

graphic.o: graphic.cc graphic.h graphic_gui.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)


$(OUT): $(OFILES)
	$(CXX) $(CXXFLAGS) $(LINKING) $(OFILES) -o $@ $(LDLIBS)

clean:
	@echo "Cleaning compilation files"
	@rm *.o $(OUT) *.cc~ *.h~
