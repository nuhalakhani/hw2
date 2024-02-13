#ifndef book_h
#define book_h
#include "product.h"
#include <string>
#include <iostream>
using namespace std;

class Book : public Product {
public:

  string nauthor;
  string nISBN;

Book(const std::string category, const std::string name, double price, int qty, std::string author, std::string ISBN);
~Book();

  std::set<std::string> keywords() const;
  std::string displayString() const;
  void dump(std::ostream& os) const;
};

#endif