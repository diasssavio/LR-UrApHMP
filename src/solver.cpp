/*
 * solver.cpp
 *
 *  Created on: Aug 21, 2015
 *      Author: savio
 */

#include "../include/solver.h"

solver::solver( model& _model ) : mod(_model), IloCplex(_model) {
	obj_value = 0.0;
	z = IloNumArray2(getEnv());
	f = IloNumArray4(getEnv());
}

solver::~solver() { }

void solver::run(){
	int n = mod.instance.get_n();

	setParam(IloCplex::Threads, 1);
	setParam(IloCplex::Param::Preprocessing::Aggregator, 0);
	setParam(IloCplex::Param::Preprocessing::Presolve, 0);
	setParam(IloCplex::PreInd, IloFalse);
//	std::cout << "before solving" << endl;
	solve();
//	std::cout << "after solving" << endl;

	// Retrieving variables & obj function value from the relaxed problem
	for(IloInt i = 0; i < n; i++){
		IloNumArray aux(getEnv());
		for(IloInt j = 0; j < n; j++)
			aux.add(getValue(mod.z[i][j]));
		z.add(aux);
	}

//	std::cout << "z got!" << endl;

	for(IloInt i = 0; i < n; i++){
		IloNumArray3 aux1(getEnv());
		for(IloInt j = 0; j < n; j++){
			IloNumArray2 aux2(getEnv());
			for(IloInt k = 0; k < n; k++){
				IloNumArray aux3(getEnv());
				for(IloInt l = 0; l < n; l++)
					aux3.add(getValue(mod.f[i][j][k][l]));
				aux2.add(aux3);
			}
			aux1.add(aux2);
		}
		f.add(aux1);
	}

//	std::cout << "f got!" << endl;

	obj_value = getObjValue();
//	std::cout << "obj got!" << endl;
}
