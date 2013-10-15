#include<iostream>
#include<sstream>
#include<string>
#include<vector>
using namespace std;
int main(){
  std::string line;
  std::vector<std::string> words;

  while ( std::getline( std::cin, line ) ) {
    std::istringstream s( line );
    std::string word;
    while ( s >> word ) {
      words.push_back( word );
    }
  }
    for (std::vector<string>::iterator it = words.begin() ; it != words.end(); ++it)
          std::cout << ' ' << *it<<endl;
  return 0;
}
