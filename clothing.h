#ifndef clothing_h
#define clothing_h
#include "product.h"
#include "util.h"
#include <string>
#include <iostream>
using namespace std;

class Clothing : public Product {
public:

  string nsize;
  string nbrand;

  Clothing(const string category, const string name, double price, int qty, string size, string brand);
  ~Clothing();

  set<string> keywords() const;
  string displayString() const;

  void dump(ostream& os) const;
};
#endif