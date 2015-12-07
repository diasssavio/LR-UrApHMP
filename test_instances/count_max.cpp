#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

using namespace std;

template<typename T>
T string_to(const string& s){
	istringstream i(s);
	T x;
	if (!(i >> x)) return 0;
	return x;
}

int main(int argc, char** args){
  ifstream file;
  file.open(args[1]);
  if(!file.is_open()) return 0;
  string line;
  float max_time = 0.0;
  while(getline(file, line)){
    if(!line.substr(0, 25).compare("Total (root+branch&cut) =")){
      string aux = line.substr(25, line.size() - 25);
      unsigned i = -1, j;
      for(unsigned it = 0; it < aux.size(); ++it)
        if((aux[it] != ' ') && (i == -1))
          i = it;
        else if((aux[it] == ' ') && (i != -1))
          j = it;
      float number = string_to<float>(aux.substr(i, j+1));
      if(number > max_time)
        max_time = number;
    }
  }

  cout << max_time << endl;

  return 0;
}
