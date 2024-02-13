#include "clothing.h"
#include "util.h"

Clothing::Clothing(const string category, const string name, double price, int qty, string size, string brand):
  Product(category, name, price, qty) {
    nsize = size;
    nbrand = brand;
  }

std::set<std::string> Clothing::keywords() const{
  std::set<std::string> key;
  std::set<std::string> nameSet = parseStringToWords(name_);
  std::set<std::string> brandSet = parseStringToWords(nbrand);

  for(std::set<std::string>::iterator it = nameSet.begin(); it != nameSet.end(); it++) {
    key.insert(convToLower(*it));
  }
  for(std::set<std::string>::iterator it = brandSet.begin(); it != brandSet.end(); it++) {
    key.insert(convToLower(*it));
  }
  key.insert(convToLower(nsize));
  return key;
}

std::string Clothing::displayString() const {
  std::string concatprice = std::to_string(price_);
  concatprice = concatprice.substr(0, concatprice.find(".")+3);
  return name_ + "\n" + "Size: " + nsize + " Brand: " + nbrand + "\n" + concatprice + " " + std::to_string(qty_) + " left." + "\n";
}

void Clothing::dump(std::ostream& os) const {
  os << category_ << "\n" << name_ << "\n" << price_ << "\n" << qty_ << "\n" << nsize << "\n" << nbrand << "\n";
}

Clothing::~Clothing() {
  
}