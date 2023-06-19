#include <gtest/gtest.h>
#include <cstdio>
#include <iostream>

#include "grid.h"

// How to get equality?...
TEST(GridPointConstructorTest, BasicAssertions) {
  // TEST argumentless
  game_of_life::Grid grid;
  EXPECT_EQ(grid.GetNumRows(), 10);
  EXPECT_EQ(grid.GetNumColumns(), 10);

  // TEST from rows
  unsigned long length = 50;
  std::vector<std::vector<unsigned long>> rows(length, std::vector<unsigned long>(length, 0));
  for (int i = 0; i < length; ++i) {
    for (int j = 0; j < length; ++j) {
      rows[i][j] = length;
    }
  }
  game_of_life::Grid grid2{rows};
  for (auto row : grid2.GetRows()) {
    for (auto el : row) {
      EXPECT_EQ(el, length);
    }
  }
  EXPECT_EQ(grid2.GetNumRows(), length);
  EXPECT_EQ(grid2.GetNumColumns(), length);

  // TEST  neighbors
  unsigned long x = 5;
  unsigned long y = 5;
  std::vector<game_of_life::Grid::GridPoint> expected1{
    game_of_life::Grid::GridPoint{x-1, y},
    game_of_life::Grid::GridPoint{x-1, y-1},
    game_of_life::Grid::GridPoint{x+1, y+1},
    game_of_life::Grid::GridPoint{x, y-1},
    game_of_life::Grid::GridPoint{x+1, y-1},
    game_of_life::Grid::GridPoint{x+1, y},
    game_of_life::Grid::GridPoint{x, y+1},
    game_of_life::Grid::GridPoint{x-1, y+1},
  };
  EXPECT_EQ(grid2.Neighbors(x, y), expected1);

  x = 0;
  y = length-1;
  std::vector<game_of_life::Grid::GridPoint> expected2{
    game_of_life::Grid::GridPoint{x, y-1},
    game_of_life::Grid::GridPoint{x+1, y-1},
    game_of_life::Grid::GridPoint{x+1, y},
  };
  EXPECT_EQ(grid2.Neighbors(x, y), expected2);

  // TEST alive
  x = 5;
  y = 5;
  // set alive
  for (game_of_life::Grid::GridPoint point : grid2.Neighbors(x, y)) {
    grid2.SetAlive(point.x, point.y);
  }
  // assert alive
  for (game_of_life::Grid::GridPoint point : grid2.Neighbors(x, y)) {
    EXPECT_TRUE(grid2.Alive(point.x, point.y));
  }

  // TEST next -- 4 way (box)
  // Original grid is 10 x 10 with zeros -- let's set 5, 5 alive and program it to "die", 
  // then the survive/resurrect cases
  // death test - one live neighbor
  grid.Reset();
  grid.SetAlive(5, 5);
  grid.SetAlive(5, 6);
  grid.Render();
  EXPECT_FALSE(grid.Survives(5, 5));

  // survival test 1 - alive and 2 live neighbors
  grid.Reset();
  grid.SetAlive(5, 5);
  grid.SetAlive(5, 6);
  grid.SetAlive(6, 5);
  grid.Render();
  EXPECT_TRUE(grid.Survives(5, 5));

  // survival test 1 - alive and 3 live neighbors
  grid.Reset();
  grid.SetAlive(5, 5);
  grid.SetAlive(5, 4);
  grid.SetAlive(5, 6);
  grid.SetAlive(6, 5);
  grid.Render();
  EXPECT_TRUE(grid.Survives(5, 5));

  // death test - 4 live neighbors
  grid.Reset();
  grid.SetAlive(5, 5);
  grid.SetAlive(5, 4);
  grid.SetAlive(5, 6);
  grid.SetAlive(4, 5);
  grid.SetAlive(6, 5);
  grid.Render();
  EXPECT_FALSE(grid.Survives(5, 5));

  // TEST next -- 4 way (diagonal)
  // Original grid is 10 x 10 with zeros -- let's set 5, 5 alive and program it to "die", 
  // then the survive/resurrect cases
  // death test - one live neighbor
  grid.Reset();
  grid.SetAlive(5, 5);
  grid.SetAlive(4, 4);
  grid.Render();
  EXPECT_FALSE(grid.Survives(5, 5));

  // survival test 1 - alive and 2 live neighbors
  grid.Reset();
  grid.SetAlive(5, 5);
  grid.SetAlive(4, 4);
  grid.SetAlive(6, 6);
  grid.Render();
  EXPECT_TRUE(grid.Survives(5, 5));

  // survival test 1 - alive and 3 live neighbors
  grid.Reset();
  grid.SetAlive(5, 5);
  grid.SetAlive(4, 4);
  grid.SetAlive(4, 6);
  grid.SetAlive(6, 6);
  grid.Render();
  EXPECT_TRUE(grid.Survives(5, 5));

  // death test - 4 live neighbors
  grid.Reset();
  grid.SetAlive(5, 5);
  grid.SetAlive(4, 4);
  grid.SetAlive(4, 6);
  grid.SetAlive(6, 6);
  grid.SetAlive(6, 4);
  grid.Render();
  EXPECT_FALSE(grid.Survives(5, 5));
}