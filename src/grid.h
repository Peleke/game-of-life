#ifndef GOL_GRID_H_
#define GOL_GRID_H_

#include <iostream>
#include <random>

namespace game_of_life {


class Seeder {
  int lo;
  int hi;
public:
  Seeder() : lo{0}, hi{100} { }
  int seed() {
    std::random_device rd;
    std::mt19937 engine(rd());
    std::uniform_int_distribution<int> dist{lo, hi};
    return dist(engine);
  }
};

class Grid {
  unsigned long nrows;
  unsigned long ncolumns;
  Seeder s;
  std::vector<std::vector<unsigned long>> rows;
public:
  Grid() : nrows{10}, ncolumns{10}, rows{std::vector<std::vector<unsigned long>>(nrows, std::vector<unsigned long>(ncolumns, 0))} { }
  Grid(const unsigned long rows, const unsigned long columns) : nrows{rows}, ncolumns{columns}, rows{std::vector<std::vector<unsigned long>>(nrows, std::vector<unsigned long>(ncolumns, 0))} { }
  Grid(std::vector<std::vector<unsigned long>> rows) : nrows{rows.size()}, ncolumns{rows[0].size()}, rows{rows}  { }

  struct GridPoint {
    GridPoint(unsigned long x, unsigned long y) : x{x}, y{y} { }
    unsigned long x;
    unsigned long y;
    bool operator== (GridPoint const& other) const {
      if (x != other.x) {
        return false;
      }
      if (y != other.y) {
        return false;
      }
      return true;
    }
  };

  Grid& operator=(const Grid& g) {
    if (this == &g) {
      return *this;
    }
    rows = g.rows;
    nrows = g.nrows;
    ncolumns = g.ncolumns;
    s = g.s;
    return *this;
  }
  void Reset() {
    for (int i = 0; i < nrows; ++i) {
      for (int j = 0; j < ncolumns; ++j) {
        rows[i][j] = 0;
      }
    }
  }
  void Seed(int live_percentage = 10) {
    for (int i = 0; i < nrows; ++i) {
      for (int j = 0; j < ncolumns; ++j) {
        if (s.seed() < live_percentage) {
          SetAlive(i, j);
        }
      }
    }
  }
  inline int GetNumRows() const {
    return nrows;
  }
  inline int GetNumColumns() const {
    return ncolumns;
  }
  inline std::vector<std::vector<unsigned long>>& GetRows() {
    return rows;
  }
  void Render() const {
    for (const auto& row : rows) {
      for (const auto el : row) {
        std::cout << '[' << (el ? 'x' : ' ') << ']';
      }
      std::cout <<'\n';
    }
    std::cout <<'\n';
  }
  std::vector<GridPoint> Neighbors(unsigned long x, unsigned long y) const {
    std::vector<GridPoint> neighbors;

    if (x >= 1) {
      neighbors.emplace_back(GridPoint{x-1, y});
      if (y >= 1) {
        neighbors.emplace_back(GridPoint{x-1, y-1});
      }
    }
    if (x < nrows-1 && y < ncolumns-1) {
      neighbors.emplace_back(GridPoint{x+1, y+1});
    }
    if (y >= 1) {
      neighbors.emplace_back(GridPoint{x, y-1});
      if (x < nrows-1) {
        neighbors.emplace_back(GridPoint{x+1, y-1});
      }
    }
    if (x < nrows-1) {
      neighbors.emplace_back(GridPoint{x+1, y});
    }
    if (y < ncolumns-1) {
      neighbors.emplace_back(GridPoint{x, y+1});
      if (x >= 1) {
        neighbors.emplace_back(GridPoint{x-1, y+1});
      }
    }

    return neighbors;
  }
  bool Alive(unsigned long x, unsigned long y) const {
    return rows[x][y] == 1;
  }
  inline void SetAlive(unsigned long x, unsigned long y) {
    rows[x][y] =  1;
  }
  Grid Evolve() const {
    std::vector<std::vector<unsigned long>> new_rows(nrows, std::vector<unsigned long>(ncolumns, 0));
    for (int i = 0; i < nrows; ++i) {
      for (int j = 0; j < ncolumns; ++j) {
        if (i == nrows) {
          break;
        }
        new_rows[i][j] = Survives(i, j);
      }
    }
    return Grid{new_rows};
  }
  bool Survives(unsigned long i, unsigned long j) const {
    // compute next state
    unsigned long live_count = 0;
    for (GridPoint& point : this->Neighbors(i, j)) {
      if (this->Alive(point.x, point.y)) {
        ++live_count;
      }
    }
    bool is_alive = this->Alive(i, j);
    if ((is_alive && live_count == 2) || (is_alive && live_count == 3)) {
      is_alive = 1;
    } else if (!is_alive && live_count == 3) {
      is_alive = 1;
    } else {
      is_alive = 0;
    }
    return is_alive;
  }
};
} // namespace game_of_life
#endif // GOL_GRID_H_