//
// Created by Yusuf Pisan on 4/18/18.
//

#include "maze.h"
#include <algorithm>
#include <cassert>
#include <climits>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

// print maze object
ostream &operator<<(ostream &out, const Maze &maze) {
  for (int row = 0; row < maze.height; ++row) {
    for (int col = 0; col < maze.width; ++col) {
      out << maze.field[row][col];
    }
    out << endl;
  }
  out << endl;
  return out;
}

// default constructor
Maze::Maze() = default;

// Load maze file from current directory
bool Maze::load(const string &fileName) {
  ifstream inFile;
  inFile.open(fileName);
  if (!inFile) {
    cerr << "Unable to open file: " << fileName << endl;
    return false;
  }
  inFile >> width >> height;
  inFile >> exitRow >> exitColumn;
  inFile >> startRow >> startColumn;
  // ignore leftover charcaters up to newline
  inFile.ignore(INT_MAX, '\n');
  string line;
  for (int row = 0; row < height; ++row) {
    getline(inFile, line);
    field.push_back(line);
  }
  return true;
}

// true if maze can be solved
bool Maze::solve() {
  if (startRow == 0) {
    return pathFinder(2, startColumn, startRow);
  }
  return pathFinder(0, startColumn, startRow);
}

string Maze::pathString(int dir) {
  if (dir == 0) {
    return "N";
  }
  if (dir == 1) {
    return "E";
  }
  if (dir == 2) {
    return "S";
  }
  return "W";
}

bool Maze::pathFinder(int dir, int x, int y) {
  if (atExit(y, x)) {
    markAsPath(y, x);
    path.append(pathString(dir));
    return true;
  }
  if (isInside(y, x) && isClear(y, x)) {
    if (!atStart(y, x)) {
      path.append(pathString(dir));
    }
    markAsPath(y, x);
    for (int i = dir; i < dir + 4; i++) {
      int xChange = 0;
      int yChange = 0;
      if (i % 4 == 0) {
        yChange++;
      }
      if (i % 4 == 1) {
        xChange++;
      }
      if (i % 4 == 2) {
        yChange--;
      }
      if (i % 4 == 3) {
        xChange--;
      }
      if (pathFinder(i % 4, x + xChange, y - yChange)) {
        return true;
      }
      if (i % 4 == (dir + 1) % 4) {
        i++;
      }
    }
    markAsVisited(y, x);
    if (!atStart(y, x)) {
      path.erase(path.length() - 1, path.length());
    }
  }
  return false;
}

// path to exit
string Maze::getPath() const { return path; }

// true if row, column is the exit
bool Maze::atExit(int row, int column) const {
  return row == exitRow && column == exitColumn;
}

bool Maze::atStart(int row, int column) const {
  return row == startRow && column == startColumn;
}

// true if row, column is inside the maze
bool Maze::isInside(int row, int col) const {
  return row >= 0 && col >= 0 && row < field.size() && col < field[row].size();
}

// true if row, column is clear to move
bool Maze::isClear(int row, int col) const {
  assert(row >= 0 && col >= 0 && row < field.size() && col < field[row].size());
  return field[row][col] == CLEAR;
}

// mark location as part of the path to exit
void Maze::markAsPath(int row, int col) {
  assert(row >= 0 && col >= 0 && row < field.size() && col < field[row].size());
  field[row][col] = PATH;
}

// mark location as visited, not part of the path to exit
void Maze::markAsVisited(int row, int col) {
  assert(row >= 0 && col >= 0 && row < field.size() && col < field[row].size());
  field[row][col] = VISITED;
}
