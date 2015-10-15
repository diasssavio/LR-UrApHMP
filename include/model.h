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
	void add_remaining_const();
	void add_lagrangian_obj(IloNumArray&);

	// Variables
	// traffic proportion
	IloNumVarArray4 f;

	// Assigned Hubs
	IloNumVarArray2 z;
};

#endif /* SRC_MODEL_H_ */
