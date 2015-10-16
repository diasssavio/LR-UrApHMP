/*
 * model.h
 *
 *  Created on: Aug 21, 2015
 *      Author: savio
 */

#ifndef SRC_MODEL_H_
#define SRC_MODEL_H_

#include <sstream>
#include <ilcplex/ilocplex.h>

#include "../include/solution.h"
#include "../include/typedef.hpp"
#include "../include/UrApHMP.h"

class model: public IloModel {
private:
	void init(); // Initialize all variables for problem
	void add_const(); // Add constraints to model
	void add_obj(); // Add objective function to model

public:
	// Instance & Solution
	uraphmp& instance;
	solution& sol;

	model(IloEnv&, uraphmp&, solution&);
	virtual ~model();

	// Lagrangian Relaxation
	// TODO Make the formulation for constraints (6) & (7)
	void add_remaining_const(unsigned);
	void add_lagrangian_obj(IloNumArray&); // option = 1
	void add_lagrangian_obj(IloNumArray2&, unsigned); // option = 2, 4
	void add_lagrangian_obj(IloNum); // option = 3
	void add_lagrangian_obj(IloNumArray3&, IloNumArray3&); // option = 5

	// Variables
	// traffic proportion
	IloNumVarArray4 f;

	// Assigned Hubs
	IloNumVarArray2 z;
};

#endif /* SRC_MODEL_H_ */
