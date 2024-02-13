#include "mydatastore.h"
#include "util.h"
#include <set>
#include <vector>
#include <string>

using namespace std;

MyDataStore::MyDataStore() {
}

MyDataStore::~MyDataStore() {
  std::set<Product*>::iterator it1;
  for(it1 = productList.begin(); it1 != productList.end(); it1++) {
    delete *it1;
  }
  std::map<std::string, User*>::iterator it2;
  for (it2 = userMap.begin(); it2 != userMap.end(); it2++) {
    delete it2->second;
  }
}

void MyDataStore::addProduct(Product* product) {
  productList.insert(product);
  std::set<std::string> productKeywords = product->keywords();
  for (std::set<std::string>::iterator it = productKeywords.begin(); it != productKeywords.end(); ++it) {
    std::string keyword = convToLower(*it);
    if(productMap.find(keyword) == productMap.end()) {
      std::set<Product*> new_productSet;
      new_productSet.insert(product);
      productMap.insert(std::make_pair(keyword, new_productSet));
    }
    else {
      productMap.find(keyword)->second.insert(product);
    }
  }
}

void MyDataStore::addUser(User* user) {
  userList.insert(user);
  std::string name = user->getName();
  userMap.insert({name, user});
}

std::vector<Product*> MyDataStore::search(std::vector<std::string>& terms, int type) {
  std::set<Product*> temp_results;
  std::vector<Product*> results; 

  if(terms.empty()) {
    return results;
  }

  if(type == 1) {
    for (unsigned int i = 0; i < terms.size(); i++) {
      std::set<Product*> products = productMap[terms[i]];
      temp_results = setUnion(temp_results, products);
    }
  }
  else if (type == 0) {
    temp_results = productMap[terms[0]];
    for (unsigned int i = 1; i < terms.size(); i++) {
      std::set<Product*> products = productMap[terms[i]];
      temp_results = setIntersection(temp_results, products);
    }
  }
  for (std::set<Product*>::iterator it = temp_results.begin(); it != temp_results.end(); ++it) {
    results.push_back(*it);
  }
  return results;
}

std::map<std::string, User*> MyDataStore::findUser() {
  return userMap;
}

void MyDataStore::dump(std::ostream& ofile) {
  ofile << "<products>" << endl;
  for(std:: set<Product*>::iterator it1 = productList.begin(); it1 != productList.end(); ++it1) {
    (*it1)->dump(ofile);
  }
  ofile << "</products>" << endl;
  ofile << "<users>" << endl;
  std::set<User*>::iterator it1;
  for (std::set<User*>::iterator it2 = userList.begin(); it2 != userList.end(); ++it2) {
    (*it2)->dump(ofile);
  }
  ofile << "</users>" << endl;
}
