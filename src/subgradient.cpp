/*
 * subgradient.cpp
 *
 *  Created on: Oct 15, 2015
 *      Author: savio
 */

#include "../include/subgradient.h"
#include <cfloat>

subgradient::subgradient(IloEnv& _env, solution& _sol, double PB, unsigned _it) : env(_env), primal_bound(PB), it_count(_it) {
	this->sol = _sol;

	// Initializing Subgradient Method parameters
	sub_step = 2.0;
	step_limit = 0.005;
	for(IloInt i = 0; i < _sol.get_instance().get_n(); i++)
		multipliers.add(0);
	best_dual = DBL_MIN;
}

subgradient::~subgradient() { }

void subgradient::run(){
	int n = sol.get_instance().get_n();

	unsigned count = 1;
	double current_dual;
	IloNumArray2 current_z;
	IloNumArray4 current_f;
	do{
		// Solving the lagrangian relaxation based on the current multipliers
		model relaxed(env, sol.get_instance(), sol);
		relaxed.add_remaining_const();
		relaxed.add_lagrangian_obj(multipliers);
		solver cplex(relaxed);
		cplex.run();

		// Retrieving variables & obj function value from the relaxed problem
		current_z = cplex.get_z();
		current_f = cplex.get_f();
		current_dual = cplex.get_obj_value();

		std::cout << "Subgradient agility: " << sub_step << std::endl;
		std::cout << "Current dual value: " << current_dual << std::endl;
		std::cout << "Best dual value: " << best_dual << std::endl;

		// Calculating subgradients & step size
		IloNumArray _subgradients = subgradients(current_z);
		IloNum _step_size = step_size(current_dual, _subgradients);

		std::cout << "Subgradients: ";
		for(IloInt i = 0; i < n; i++)
			std::cout << _subgradients[i] << " ";
		std::cout << endl;

		std::cout << "Multipliers: ";
		for(IloInt i = 0; i < n; i++)
			std::cout << multipliers[i] << " ";
		std::cout << endl;

		// Updating Lagrangian multipliers
		for(IloInt i = 0; i < n; i++){
			IloNum aux = multipliers[i] + _step_size * _subgradients[i];
			multipliers[i] = (aux > 0) ? aux : 0;
		}

		// Saving the best bound
		if(current_dual > best_dual){
			best_dual = current_dual;
			count = 1;
		} else count++;

		// Checking the improvement
		if(count == it_count)
			sub_step /= 2.0;
	}while( (current_dual != primal_bound) || (sub_step > step_limit) );
}

IloNumArray subgradient::subgradients(IloNumArray2& _z){
	int n = sol.get_instance().get_n();
	int r = sol.get_r();

	IloNumArray sub;
	for(IloInt i = 0; i < n; i++){
		IloNum count = r;
		for(IloInt k = 0; k < n; k++)
			count -= _z[i][k];
		sub.add(count);
	}

	return sub;
}

IloNum subgradient::step_size(double dual, const IloNumArray& subgradients){
	int n = sol.get_instance().get_n();

	IloNum sub_sum, aux;
	for(IloInt i = 0; i < n; i++)
		aux += subgradients[i] * subgradients[i];

	sub_sum = ( sub_step * (primal_bound - dual) ) / aux;

	return sub_sum;
}
