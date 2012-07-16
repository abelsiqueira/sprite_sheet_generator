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
  if (argc < 2)
    return -1;
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
