#ifndef mydatastore_h
#define mydatastore_h
#include <string>
#include <set>
#include <vector>
#include <map>
#include "product.h"
#include "user.h"
#include "datastore.h"
#include "util.h"
#include "book.h"
#include "clothing.h"
#include "movie.h"
using namespace std;

class MyDataStore : public DataStore {
public: 
  MyDataStore();
  ~MyDataStore();

  set<User*> userList;
  std::set<Product*> productList;
  std::map<std::string, std::set<Product*>> productMap;
  std::map<std::string, User*> userMap;
  std::map<std::string, std::vector<Product*>> cartMap;
  std::map<std::string, User*> findUser();
  
  void addProduct(Product* product);
  void addUser(User* user);
  std::vector<Product*> search(std::vector<std::string>& terms, int type);
  void dump(std::ostream& ofile);



};
#endif