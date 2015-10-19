/*
 * subgradient.h
 *
 *  Created on: Oct 15, 2015
 *      Author: Sávio S. Dias
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
	solution& sol; // Solution for instance info's
	double primal_bound; // Feasible solution for comparison
	unsigned it_count; // Maximum iteration without improvement
	unsigned option; // Represent the consts which will be dualized

	// Method data
	IloNumArray multipliers; // Lagrangian multipliers for consts (2) -- option = 1
	IloNumArray2 multipliers_2D; // Lagrangian multipliers 2D (matrix) for consts (3) & (5) -- option = 2, 4
	IloNum multiplier; // Lagrangian multiplier for const (4) -- option = 3
	IloNumArray3 u_multipliers_3D; // 1st Lagrangian multipliers 3D for consts (6) -- option = 5
	IloNumArray3 w_multipliers_3D; // 2nd Lagrangian multipliers 3D for consts (7) -- option = 5

	double sub_step; // Subgradient step (agility)
	double step_limit; // Subgradient step limit
	double best_dual; // Best dual bound (lagrangian)
	IloNumArray2 z_dual; // Current best z_ik dual solution (lagrangian)
	IloNumArray4 f_dual; // Current best f_ijkl dual solution (lagrangian)

	// Method calculations
	IloNumArray subgradients_1D(const IloNumArray2&); // Compute subgradients vector for consts (2)
	IloNumArray2 subgradients_2D(const IloNumArray2&); // Compute subgradients 2D matrix for consts (3)
	IloNum _subgradient(const IloNumArray2&); // Compute subgradient for const (4)
	IloNumArray2 subgradients_2D(const IloNumArray4&); // Compute subgradients 2D matrix for consts (5)
	IloNumArray3 subgradients_3D(const IloNumArray2&, const IloNumArray4&); // Compute subgradients 3D matrix for consts (6)
	IloNumArray3 _subgradients_3D(const IloNumArray2&, const IloNumArray4&); // Compute subgradients 3D matrix for consts (7)

	IloNum step_size(double, const IloNumArray&); // Compute step size for consts (2)
	IloNum step_size(double, const IloNumArray2&); // Compute step size for consts (3) e (5)
	IloNum step_size(double, IloNum); // Compute step size for const (4)
	IloNum step_size(double, const IloNumArray3&, const IloNumArray3&); // Compute step size for consts (6) e (7)

	void print_sub(IloNum);
	void print_sub(const IloNumArray&);
	void print_sub(const IloNumArray2&);
	void print_sub(const IloNumArray3&);
	void print_mult();

public:
	subgradient(IloEnv&, solution&, double, unsigned, unsigned);
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
