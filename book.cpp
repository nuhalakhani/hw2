#include "book.h"
#include "util.h"

using namespace std;

Book::Book(const std::string category, const std::string name, double price, int qty, std::string author, std::string ISBN):
  Product(category, name, price, qty) {
    nauthor = author;
    nISBN = ISBN;
  }

std::set<std::string> Book::keywords() const{
  std::set<std::string> key;
  std::set<std::string> nameSet = parseStringToWords(name_);
  std::set<std::string> authorSet = parseStringToWords(nauthor);

  for(std::set<std::string>::iterator it = nameSet.begin(); it != nameSet.end(); it++) {
    key.insert(convToLower(*it));
  }
  for(std::set<std::string>::iterator it = authorSet.begin(); it != authorSet.end(); it++) {
    key.insert(convToLower(*it));
  }
  key.insert(convToLower(nISBN));
  return key;
}

std::string Book::displayString() const {
  std::string result = name_ + "\n" + "Author: " + nauthor + " ISBN: " + nISBN + "\n" + std::to_string(price_) + " " + std::to_string(qty_) + " left." + "\n";
  return result;
}

void Book::dump(std::ostream& os) const {
  os << category_ << "\n" << name_ << "\n" << price_ << "\n" << qty_ << "\n" << nISBN << "\n" << nauthor << "\n";
}

Book::~Book() {
  
}
