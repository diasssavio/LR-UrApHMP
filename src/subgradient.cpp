/*
 * subgradient.cpp
 *
 *  Created on: Oct 15, 2015
 *      Author: savio
 */

#include "../include/subgradient.h"
#include <cfloat>

subgradient::subgradient(IloEnv& _env, solution& _sol, double PB, unsigned _it, unsigned _op) : env(_env), sol(_sol), primal_bound(PB), it_count(_it), option(_op) {
	int n = _sol.get_instance().get_n();

	// Initializing Subgradient Method parameters
	sub_step = 2.0;
	step_limit = 0.005;
	best_dual = DBL_MIN;

	// Initializing lagrangian multipliers
	if(option == 1){
		multipliers = IloNumArray(env);
		for(IloInt i = 0; i < n; i++)
			multipliers.add(1);
	}

	if((option == 2) || (option == 4)){
		multipliers_2D = IloNumArray2(env);
		for(IloInt i = 0; i < n; i++){
			IloNumArray aux(env);
			for(IloInt j = 0; j < n; j++)
				aux.add(0);
			multipliers_2D.add(aux);
		}
	}

	if(option == 3)
		multiplier = 0;

	if(option == 5){
		u_multipliers_3D = IloNumArray3(env);
		w_multipliers_3D = IloNumArray3(env);
		for(IloInt i = 0; i < n; i++){
			IloNumArray2 aux2(env);
			IloNumArray2 _aux2(env);
			for(IloInt j = 0; j < n; j++){
				IloNumArray aux(env);
				IloNumArray _aux(env);
				for(IloInt k = 0; k < n; k++){
					aux.add(0);
					_aux.add(0);
				}
				aux2.add(aux);
				_aux2.add(aux2);
			}
			u_multipliers_3D.add(aux2);
			w_multipliers_3D.add(aux2);
		}
	}
}

subgradient::~subgradient() { }

void subgradient::run(){
	int n = sol.get_instance().get_n();

	unsigned count = 0;
	double current_dual;
	IloNumArray2 current_z;
	IloNumArray4 current_f;
	do{
		// Solving the lagrangian relaxation based on the current multipliers
		model relaxed(env, sol.get_instance(), sol);
		relaxed.add_remaining_const(option);
		if(option == 1)
			relaxed.add_lagrangian_obj(multipliers);
		if((option == 2) || (option == 4))
			relaxed.add_lagrangian_obj(multipliers_2D, option);
		if(option == 3)
			relaxed.add_lagrangian_obj(multiplier);
		if(option == 5)
			relaxed.add_lagrangian_obj(u_multipliers_3D, w_multipliers_3D);
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
		print_mult();
		if(option == 1){
			IloNumArray _subgradients = subgradients_1D(current_z);
			IloNum _step_size = step_size(current_dual, _subgradients);

			print_sub(_subgradients);
			std::cout << "Subgradient step size: " << _step_size << std::endl;

			// Updating Lagrangian multipliers
			for(IloInt i = 0; i < n; i++){
				IloNum aux = multipliers[i] - ( _step_size * _subgradients[i] );
				multipliers[i] = (aux > 0) ? aux : 0;
			}
		}
		if((option == 2) || (option == 4)){
			IloNumArray2 _subgradients;
			if(option == 2)
				_subgradients = subgradients_2D(current_z);
			else
				_subgradients = subgradients_2D(current_f);

			IloNum _step_size = step_size(current_dual, _subgradients);

			print_sub(_subgradients);
			std::cout << "Subgradient step size: " << _step_size << std::endl;

			// Updating Lagrangian multiplier
			if(option == 2)
				for(IloInt i = 0; i < n; i++)
					for(IloInt k = 0; k < n; k++){
						IloNum aux = multipliers_2D[i][k] - (_step_size * _subgradients[i][k]);
						multipliers_2D[i][k] = (aux > 0) ? aux : 0;
					}
			if(option == 4)
				for(IloInt i = 0; i < n; i++)
					for(IloInt j = 0; j < n; j++)
						multipliers_2D[i][j] = multipliers_2D[i][j] - (_step_size * _subgradients[i][j]);
		}
		if(option == 3){
			IloNum subgradient = _subgradient(current_z);
			IloNum _step_size = step_size(current_dual, subgradient);

			print_sub(subgradient);

			// Updating Lagrangian multiplier
			multiplier = multiplier - (_step_size * subgradient);
		}
		if(option == 5){
			// TODO Implement the Subgradient method
		}

		// Saving the best bound
		if(current_dual > best_dual){
			best_dual = current_dual;
			z_dual = current_z;
			f_dual = current_f;
			count = 0;
		} else count++;

		// Checking the improvement
		if(count == it_count){
			sub_step /= 2.0;
			count = 0;
		}
	}while( (current_dual < primal_bound) || (sub_step > step_limit) );
}

IloNumArray subgradient::subgradients_1D(const IloNumArray2& _z){
	int n = sol.get_instance().get_n();
	int r = sol.get_r();

	IloNumArray sub(env);
	for(IloInt i = 0; i < n; i++){
		IloNum count = r;
		for(IloInt k = 0; k < n; k++)
			count -= _z[i][k];
		sub.add(count);
	}

	return sub;
}

IloNumArray2 subgradient::subgradients_2D(const IloNumArray2& _z){
	int n = sol.get_instance().get_n();

	IloNumArray2 sub(env);
	for(IloInt i = 0; i < n; i++){
		IloNumArray aux(env);
		for(IloInt k = 0; k < n; k++)
			aux.add(_z[k][k] - _z[i][k]);
		sub.add(aux);
	}

	return sub;
}

IloNum subgradient::_subgradient(const IloNumArray2& _z){
	int n = sol.get_instance().get_n();
	int p = sol.get_p();

	IloNum sub = 0.0;
	for(IloInt k = 0; k < n; k++)
		sub += _z[k][k];
	sub -= p;

	return sub;
}

IloNumArray2 subgradient::subgradients_2D(const IloNumArray4& _f){
	int n = sol.get_instance().get_n();

	IloNumArray2 sub(env);
	for(IloInt i = 0; i < n; i++){
		IloNumArray aux(env);
		for(IloInt j = 0; j < n; j++){
			IloNum aux2 = 0.0;
			for(IloInt k = 0; k < n; k++)
				for(IloInt l = 0; l < n; l++)
					aux2 += _f[i][j][k][l];
			aux.add(aux2 - 1.0);
		}
		sub.add(aux);
	}

	return sub;
}

IloNumArray3 subgradient::subgradients_3D(const IloNumArray2& _z, const IloNumArray4& _f){
	// TODO Calculate subgradients for u in Consts (6)
}

IloNumArray3 subgradient::_subgradients_3D(const IloNumArray2& _z, const IloNumArray4& _f){
	// TODO Calculate subgradients for u in Consts (7)
}

IloNum subgradient::step_size(double dual, const IloNumArray& subgradients){
	int n = sol.get_instance().get_n();

	IloNum aux = 0.0;
	for(IloInt i = 0; i < n; i++)
		aux += subgradients[i] * subgradients[i];

	IloNum sub_sum = ( sub_step * (primal_bound - dual) ) / aux;

	return sub_sum;
}

IloNum subgradient::step_size(double dual, const IloNumArray2& subgradients){
	int n = sol.get_instance().get_n();

	IloNum aux = 0.0;
	for(IloInt i = 0; i < n; i++)
		for(IloInt j = 0; j < n; j++)
			aux += subgradients[i][j] * subgradients[i][j];

	IloNum sub_sum = ( sub_step * (primal_bound - dual) ) / aux;

	return sub_sum;
}

IloNum subgradient::step_size(double dual, IloNum subgradients){
	return ( sub_step * (primal_bound - dual) ) / (subgradients * subgradients);
}

void subgradient::print_sub(IloNum _sub){
	std::cout << "Subgradient: " << _sub << endl;
}

void subgradient::print_sub(const IloNumArray& _subgradients){
	int n = sol.get_instance().get_n();
	std::cout << "Subgradients: ";
	for(IloInt i = 0; i < n; i++)
		std::cout << _subgradients[i] << " ";
	std::cout << endl;
}

void subgradient::print_sub(const IloNumArray2& _subgradients){
	int n = sol.get_instance().get_n();
	std::cout << "Subgradients: ";
	for(IloInt i = 0; i < n; i++){
		for(IloInt j = 0; j < n; j++)
			std::cout << _subgradients[i][j] << " ";
		std::cout << endl;
	}
	std::cout << endl;
}

void subgradient::print_mult(){
	int n = sol.get_instance().get_n();
	if(option == 1){
		std::cout << "Multipliers: ";
		for(IloInt i = 0; i < n; i++)
			std::cout << multipliers[i] << " ";
		std::cout << endl;
	}
	if(option == 2){
		std::cout << "Multipliers: ";
		for(IloInt i = 0; i < n; i++){
			for(IloInt j = 0; j < n; j++)
				std::cout << multipliers_2D[i][j] << " ";
			std::cout << endl;
		}
		std::cout << endl;
	}
}
