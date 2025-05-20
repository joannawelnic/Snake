#include "allmodels.h"
#include <iostream>

Models::ObjModel ModelResources::apple;

//Models::ObjModel ModelResources::apple("C:/Users/Tomasz/Downloads/g3d_st_04_win/g3d_st_04_win/g3d_st_04_win/x64/Debug/apple.obj");


//TexturedModel ModelResources::tile; // Dodajemy obiekt


void loadModels() {
    // Tu mo¿esz dodaæ inne modele jeœli chcesz...
    //ModelResources::tile.readFromOBJ("C:/Users/Tomasz/Downloads/g3d_st_04_win/g3d_st_04_win/g3d_st_04_win/x64/Debug/models/tile.obj");         // Prosty model kafelka
    //ModelResources::tile.loadTexture("C:/Users/Tomasz/Downloads/g3d_st_04_win/g3d_st_04_win/g3d_st_04_win/x64/Debug/textures/tile_texture.jpg"); // Tekstura pod³ogi
    ModelResources::apple.load("apple.obj");
    std::cout << "APPLE vertexCount = " << ModelResources::apple.vertexCount << std::endl;

}

void cleanupModels() {
    //ModelResources::tile.cleanup();
}

