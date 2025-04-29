//Architecture Fig 11 b1
#ifndef GRAPHIC_HEADER_H
#define GRAPHIC_HEADER_H

#include "graphic_color.h"
//Dessins
void graphic_draw_line(int xd, int yd, int xf, int yf);

void graphic_draw_circle(int xc, int yc, double rayon);

void graphic_draw_quadrilatere(int x1, int y1, int x2, int y2, int x3, int y3, int x4, 
							   int y4);
//Setter
void set_color(Color c);
void set_width(double w);
void set_height(double h);
void set_dim_max(double dm);
void set_coordinates(double xmin, double xmax, double ymin, double ymax);


int xModel(double coord, double width);
int yModel(double coord, double height);

#endif //GRAPHIC_HEADER_H
