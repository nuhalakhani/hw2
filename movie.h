#ifndef movie_h
#define movie_h
#include "product.h"
#include "util.h"
#include <string>
#include <iostream>
using namespace std;

class Movie : public Product {
public:

  string ngenre;
  string nrating;

  Movie(const string category, const string name, double price, int qty, string genre, string rating);
  ~Movie();

  set<string> keywords() const;
  string displayString() const;

  void dump(ostream& os) const;
};
#endif