/*
 * solver.h
 *
 *  Created on: Aug 21, 2015
 *      Author: savio
 */

#ifndef SRC_SOLVER_H_
#define SRC_SOLVER_H_

#include <ilcplex/ilocplex.h>

#include "../include/solution.h"
#include "../include/model.h"
#include "../include/typedef.hpp"
#include "../include/UrApHMP.h"

class solver : public IloCplex {
private:
	IloNumArray2 z;
	IloNumArray4 f;

	model& mod;

	double obj_value;

public:
	solver(model&);
	virtual ~solver();

	void run();

	const IloNumArray4& get_f() const { return f; }
	double get_obj_value() const { return obj_value; }
	const IloNumArray2& get_z() const { return z; }
};

#endif /* SRC_SOLVER_H_ */
