#include "grid.h"
#include <chrono>
#include <iostream>
#include <stdlib.h>
#include <thread>
using namespace std::chrono_literals;

void animate(std::vector<game_of_life::Grid>& grids) {
  std::system("clear");
  grids[0].Render();

  for (int i = 1; i < grids.size(); ++i) {
    grids[i] = grids[i-1].Evolve();

    std::this_thread::sleep_for(0.5s);
    std::system("clear");

    grids[i].Render();
  }
}

int main() {
  std::vector<game_of_life::Grid> grids(100);
  
  game_of_life::Grid grid{78, 78};
  grid.Seed();
  grids[0] = grid;

  animate(grids);
}