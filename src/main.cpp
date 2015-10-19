#include <cstdio>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <ilcplex/ilocplex.h>

#include "../include/model.h"
#include "../include/subgradient.h"
#include "../include/solution.h"
#include "../include/UrApHMP.h"

using namespace std;

template<typename T>
T string_to(const string& s){
	istringstream i(s);
	T x;
	if (!(i >> x)) return 0;
	return x;
}

ILOSTLBEGIN
int main(int argc, char* args[]){
	// Reading input file
	int n;

	scanf("%d", &n);
	double X = 1.0, alpha_1 = 0.2, delta = 1.0;
	uraphmp instance(n, X, alpha_1, delta);

	vector< vector< double> > aux;
	for(int i = 0; i < n; i++){
		vector< double > aux2;
		for(int j = 0; j < n; j++){
			double temp;
			scanf("%lf", &temp);
			aux2.push_back(temp);
		}
		aux.push_back(aux2);
	}
	instance.set_traffics(aux);

	aux.clear();
	for(int i = 0; i < n; i++){
		vector< double > aux2;
		for(int j = 0; j < n; j++){
			double temp;
			scanf("%lf", &temp);
			aux2.push_back(temp);
		}
		aux.push_back(aux2);
	}
	instance.set_distances(aux);
	aux.clear();

	// Creating a solution object
	int p = string_to<int>(args[1]);
	int r = string_to<int>(args[2]);
	solution sol(instance, p, r);

	// Initializing cplex environment
	IloEnv env;

	try{
		subgradient method(env, sol, 534.0, 2, 2);
		method.run();
	}catch(IloException& e){
		cerr << "Concert Exception: " << e << endl;
	}
	// Closing the environment
	env.end();

	return 0;
}
