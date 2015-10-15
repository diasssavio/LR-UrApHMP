/*
 * solver.cpp
 *
 *  Created on: Aug 21, 2015
 *      Author: savio
 */

#include "../include/solver.h"

solver::solver( model& _model ) : mod(_model), IloCplex(_model) { obj_value = 0.0; }

solver::~solver() { }

void solver::run(){
	int n = mod.instance.get_n();

	setParam(IloCplex::Threads, 1);
	setParam(IloCplex::Param::Preprocessing::Aggregator, 0);
	setParam(IloCplex::Param::Preprocessing::Presolve, 0);
	setParam(IloCplex::PreInd, IloFalse);
	solve();

	// Retrieving variables & obj function value from the relaxed problem
	for(IloInt i = 0; i < n; i++)
		for(IloInt j = 0; j < n; j++)
			z[i][j] = getValue(mod.z[i][j]);

	for(IloInt i = 0; i < n; i++)
		for(IloInt j = 0; j < n; j++)
			for(IloInt k = 0; k < n; k++)
				for(IloInt l = 0; l < n; l++)
					f[i][j][k][l] = getValue(mod.f[i][j][k][l]);

	obj_value = getObjValue();
}
