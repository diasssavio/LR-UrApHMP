/*
 * subgradient.h
 *
 *  Created on: Oct 15, 2015
 *      Author: savio
 */

#ifndef SUBGRADIENT_H_
#define SUBGRADIENT_H_

#include <ilcplex/ilocplex.h>

#include "../include/solution.h"
#include "../include/model.h"
#include "../include/solver.h"
#include "../include/typedef.hpp"
#include "../include/UrApHMP.h"

class subgradient {
private:
	// Input
	IloEnv& env; // CPLEX environment
	solution sol; // Solution for instance info's
	double primal_bound; // Feasible solution for comparison
	unsigned it_count; // Maximum iteration without improvement

	// Method data
	IloNumArray multipliers; // Lagrangian multipliers
	double sub_step; // Subgradient step (agility)
	double step_limit; // Subgradient step limit
	double best_dual; // Best dual bound (lagrangian)
	IloNumArray2 z_dual; // Current best z_ik dual solution (lagrangian)
	IloNumArray4 f_dual; // Current best f_ijkl dual solution (lagrangian)

	IloNumArray subgradients(IloNumArray2&); // Compute subgradients vector
	IloNum step_size(double, const IloNumArray&); // Compute step size

public:
	subgradient(IloEnv&, solution&, double, unsigned);
	virtual ~subgradient();

	// Subgradient method
	void run();

	// Getters
	const IloNumArray& get_multipliers() const { return multipliers; }
	double get_best_dual() const { return best_dual; }
	const IloNumArray2& get_z_dual() const { return z_dual; }
	const IloNumArray4& get_f_dual() const { return f_dual; }
};

#endif /* SUBGRADIENT_H_ */
