/* Copyright 2012 - Abel Soares Siqueira
 * 
 * This file is part of Sprite Sheet Generator.
 * 
 * Sprite Sheet Generator is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Sprite Sheet Generator is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Sprite Sheet Generator.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <string>
#include <sstream>

using namespace std;
  
int main(int argc, char **argv) {
	int imageWidth = 0;
	int imageHeight = 0;

	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_BITMAP *outImage;
	
  // Argument reading
  if (argc < 2) {
    cout << "Usage: sprite_sheet_generator <sprite_name> [options]" << endl
         << "Run\nsprite_sheet_generator --help\nto show options" << endl;
    return -1;
  }
  string fileExtension("bmp");
  int numberOfFrames = 8;
  string outStr;
  stringstream outFile;

  string path;

  for (int i = 1; i < argc; i++) {
    string aux(argv[i]);
    if (aux[0] == '-') { // An option
      size_t eq = aux.find_first_of('=');
      string option(aux, 1, eq - 1);
      string value(aux, eq + 1, aux.size() - eq);
      if (option.compare("type") == 0) {
        fileExtension.assign(value);
      } else if (option.compare("frames") == 0) {
        stringstream a;
        a << value;
        a >> numberOfFrames;
      } else if (option.compare("output") == 0) {
        outStr.assign(value);
        outStr += ".";
      } else if ( (option.compare("-help") == 0) ||
                  (option.compare("h") == 0) ) {
        cout << "---------------------------------------" << endl
             << "Sprite Sheet Generator - Copyright Abel Soares Siqueira" << endl
             << "LICENSE GPL v3.0" << endl
             << endl
             << "Use this software to create a tile sheet from Reiner's Tile Set" << endl
             << "images (http://www.reinerstilesets.de)" << endl
             << "You must have all 8 directions of the sprite (n, ne, e, se, s, sw, w, nw)" << endl
             << endl
             << "Usage: sprite_sheet_generator <sprite_name> [options]" << endl
             << "Options: " << endl
             << "  -frames=N       Set number of frames to N (default: 8)" << endl
             << "  -type=ext       Set file extension to ext (default: bmp)" << endl
             << "  -output=name    Set output file to name.<type>" << endl
             << "  -h,--help       Show this help" << endl;   
        return 0;
      } else {
        cout << "option '" << option << "' unrecognized" << endl;
      }
    } else {
      path.assign(aux);
    }
  }

  if (outStr.empty()) {
    outStr = path;
    outStr += "_sheet.";
  }
  outStr += fileExtension;
  outFile << outStr;
  
	const int arraySize = numberOfFrames;
	ALLEGRO_BITMAP *image[arraySize][8]; //8 directions

	if(!al_init())										//initialize Allegro
		return -1;

	display = al_create_display(100, 100);			//create our display object

	if(!display)										//test display object
		return -1;

	//addon init
	al_init_image_addon();

  string directions[8] = {"n","ne","e","se","s","sw","w","nw"};

  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < arraySize; j++) {
      stringstream aux;
      aux << path << ' ' << directions[i];
      aux.width(4);
      aux.fill('0');
      aux << j << ".bmp";
      image[j][i] = al_load_bitmap(aux.str().c_str());
      if (!image[j][i]) {
        al_destroy_display(display);
        cout << "Could not open image '" << aux.str() << "'" <<  endl;
        return -2;
      }
    }
  }

	imageWidth = al_get_bitmap_width(image[0][0]);
	imageHeight = al_get_bitmap_height(image[0][0]);

	outImage = al_create_bitmap(imageWidth * arraySize, imageHeight * 8);

	al_set_target_bitmap(outImage);
	al_clear_to_color(al_map_rgb(0, 0, 0));

  for (int j = 0; j < 8; j++) {
    for(int i = 0; i < arraySize; i++)
    {
      al_draw_bitmap(image[i][j], i * imageWidth, j * imageHeight, 0);
    }
  }

  cout << "outFile = " << outFile.str() << endl;
	al_save_bitmap(outFile.str().c_str(), outImage);

	al_set_target_bitmap(al_get_backbuffer(display));

	for(int i = 0; i < arraySize; i++)
    for (int j = 0; j < 8; j++)
      al_destroy_bitmap(image[i][j]);

	al_destroy_bitmap(outImage);
	al_destroy_display(display);						//destroy our display object

	return 0;
}
