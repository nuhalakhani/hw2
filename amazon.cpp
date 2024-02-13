#include <iostream>
#include <fstream>
#include <set>
#include <sstream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include "product.h"
#include "db_parser.h"
#include "product_parser.h"
#include "util.h"
#include <map>
#include "mydatastore.h"
#include "book.h"
#include "movie.h"
#include "clothing.h"

using namespace std;
struct ProdNameSorter {
    bool operator()(Product* p1, Product* p2) {
        return (p1->getName() < p2->getName());
    }
};
void displayProducts(vector<Product*>& hits);

int main(int argc, char* argv[])
{
    if(argc < 2) {
        cerr << "Please specify a database file" << endl;
        return 1;
    }

    /****************
     * Declare your derived DataStore object here replacing
     *  DataStore type to your derived type
     ****************/
    MyDataStore ds;



    // Instantiate the individual section and product parsers we want
    ProductSectionParser* productSectionParser = new ProductSectionParser;
    productSectionParser->addProductParser(new ProductBookParser);
    productSectionParser->addProductParser(new ProductClothingParser);
    productSectionParser->addProductParser(new ProductMovieParser);
    UserSectionParser* userSectionParser = new UserSectionParser;

    // Instantiate the parser
    DBParser parser;
    parser.addSectionParser("products", productSectionParser);
    parser.addSectionParser("users", userSectionParser);

    // Now parse the database to populate the DataStore
    if( parser.parse(argv[1], ds) ) {
        cerr << "Error parsing!" << endl;
        return 1;
    }

    cout << "=====================================" << endl;
    cout << "Menu: " << endl;
    cout << "  AND term term ...                  " << endl;
    cout << "  OR term term ...                   " << endl;
    cout << "  ADD username search_hit_number     " << endl;
    cout << "  VIEWCART username                  " << endl;
    cout << "  BUYCART username                   " << endl;
    cout << "  QUIT new_db_filename               " << endl;
    cout << "====================================" << endl;

    vector<Product*> hits;
    std::map<string, vector<Product*>> cartMap;
    bool done = false;
    while(!done) {
        cout << "\nEnter command: " << endl;
        string line;
        getline(cin,line);
        stringstream ss(line);
        string cmd;
        if((ss >> cmd)) {
            if( cmd == "AND") {
                string term;
                vector<string> terms;
                while(ss >> term) {
                    term = convToLower(term);
                    terms.push_back(term);
                }
                hits = ds.search(terms, 0);
                displayProducts(hits);
            }
            else if ( cmd == "OR" ) {
                string term;
                vector<string> terms;
                while(ss >> term) {
                    term = convToLower(term);
                    terms.push_back(term);
                }
                hits = ds.search(terms, 1);
                displayProducts(hits);
            }
            else if ( cmd == "QUIT") {
                string filename;
                if(ss >> filename) {
                    ofstream ofile(filename.c_str());
                    ds.dump(ofile);
                    ofile.close();
                }
                done = true;
            }
	    /* Add support for other commands here */
            else if (cmd == "ADD") {
              std::map<string, User*> userMap = ds.findUser();
              std::string user;
              unsigned int index;
              ss >> user >> index;
              index--;

              if(index < 0 || index >= hits.size() || userMap.find(user) == userMap.end()) {
                cout << "Invalid request" << endl;
              }
              else {
                Product* hitProduct = hits.at(index);
                if (cartMap.find(user) == cartMap.end()) {
                  std::vector<Product*> new_cart_items = {hitProduct};
                  cartMap.insert({user, new_cart_items});
                }
                else {
                  cartMap[user].push_back(hitProduct);
                }
              }
            }
            else if (cmd == "VIEWCART") {
              std::map<string, User*> userMap = ds.findUser();
              string user;
              ss >> user;
              if(cartMap.find(user) == cartMap.end()) {
                cout << "Invalid username" << endl;
              }
              else {
                std::vector<Product*> cart;
                if(cartMap.find(user) != cartMap.end()) {
                  cart = cartMap[user];
                }

                int i = 1;
                for (Product* currCart : cart) {
                  cout << "Item " << i << endl;
                  cout << currCart->displayString() << endl << endl;
                  i++;
                }
              }
            }
            else if (cmd == "BUYCART") {
              std::map<string, User*> userMap = ds.findUser();
              std::string user;
              ss >> user;
              if (cartMap.find(user) == cartMap.end()) {
                cout << "Invalid username" << endl;
              }
              else {
                std::vector<Product*>currCart = cartMap[user];
                User* currUser = userMap[user];
                std::vector<Product*> ncart;
                for (std::vector<Product*>::iterator it = currCart.begin(); it != currCart.end();) {
                  Product* product = *it;
                  if(product->getQty() > 0 && product->getPrice() <= currUser->getBalance()) {
                    it = currCart.erase(it);
                    product->subtractQty(1);
                    currUser->deductAmount(product->getPrice());
                  }
                  else if (currUser->getBalance() < product->getPrice()) {
                    std::cout << "Insufficient balance to purchase " << product->getName() << std::endl;
                    ncart.push_back(product);
                    it = currCart.erase(it);
                  }
                  else {
                    it += 1;
                  }
                }
                cartMap[user] = currCart;
                if(!ncart.empty()) {
                  cartMap[user].insert(cartMap[user].end(), ncart.begin(), ncart.end());
                }
              }
            }
            else {
                cout << "Unknown command" << endl;
            }
        }

    }
    return 0;
}

void displayProducts(vector<Product*>& hits)
{
    int resultNo = 1;
    if (hits.begin() == hits.end()) {
    	cout << "No results found!" << endl;
    	return;
    }
    std::sort(hits.begin(), hits.end(), ProdNameSorter());
    for(vector<Product*>::iterator it = hits.begin(); it != hits.end(); ++it) {
        cout << "Hit " << setw(3) << resultNo << endl;
        cout << (*it)->displayString() << endl;
        cout << endl;
        resultNo++;
    }
}