/*
 * model.cpp
 *
 *  Created on: Aug 21, 2015
 *      Author: savio
 */

#include "../include/model.h"

model::model(IloEnv& _env, uraphmp& _instance, solution& _sol) : IloModel(_env), instance(_instance), sol(_sol) {
	init();
}

model::~model() { }

void model::init(){
	int n = instance.get_n();
	f = IloNumVarArray4(getEnv(), n);
	z = IloNumVarArray2(getEnv(), n);

	for(IloInt i = 0; i < n; i++){
		f[i] = IloNumVarArray3(getEnv(), n);
		for(IloInt j = 0; j < n; j++){
			f[i][j] = IloNumVarArray2(getEnv(), n);
			for(IloInt k = 0; k < n; k++){
				f[i][j][k] = IloNumVarArray(getEnv(), n);
				for(IloInt l = 0; l < n; l++){
					f[i][j][k][l] = IloNumVar(getEnv(), 0, +IloInfinity, ILOFLOAT);
					stringstream f_name;
					f_name << "f[" << i << "][" << j << "][" << k << "][" << l << "]";
					f[i][j][k][l].setName(f_name.str().c_str());
					add(f[i][j][k][l]);
				}
			}
		}

		z[i] = IloNumVarArray(getEnv(), n);
		for(IloInt k = 0; k < n; k++){
			z[i][k] = IloNumVar(getEnv(), 0, 1, ILOINT);
			stringstream z_name;
			z_name << "z[" << i << "][" << k << "]";
			z[i][k].setName(z_name.str().c_str());
			add(z[i][k]);
		}
	}
}

void model::add_const(){
	int n = instance.get_n();
	int p = sol.get_p();
	int r = sol.get_r();

	// Constraints (2): attribution to at most r hubs
	for(IloInt i = 0; i < n; i++){
		IloExpr expr(getEnv());
		for(IloInt k = 0; k < n; k++)
			expr += z[i][k];
		IloConstraint c2 = (expr <= r);
		stringstream c2_name;
		c2_name << "Cons(2)[" << i << "]";
		c2.setName(c2_name.str().c_str());
		add(c2);
	}

	// Constraints (3): attribution only to hubs
	for(IloInt i = 0; i < n; i++){
		for(IloInt k = 0; k < n; k++){
			IloConstraint c3 = (z[i][k] <= z[k][k]);
			stringstream c3_name;
			c3_name << "Cons(3)[" << i << "]" << "[" << k << "]";
			c3.setName(c3_name.str().c_str());
			add(c3);
		}
	}

	// Constraint (4): allocation of p hubs
	IloExpr expr(getEnv());
	for(IloInt k = 0; k < n; k++)
		expr += z[k][k];
	IloConstraint c4 = (expr == p);
	stringstream c4_name;
	c4_name << "Cons(4)";
	c4.setName(c4_name.str().c_str());
	add(c4);

	// Constraints (5): traffic needs to be total routed
	for(IloInt i = 0; i < n; i++)
		for(IloInt j = 0; j < n; j++){
			IloExpr expr(getEnv());
			for(IloInt k = 0; k < n; k++)
				for(IloInt l = 0; l < n; l++)
					expr += f[i][j][k][l];
			IloConstraint c5 = (expr == 1);
			stringstream c5_name;
			c5_name << "Cons(5)[" << i << "]" << "[" << j << "]";
			c5.setName(c5_name.str().c_str());
			add(c5);
		}

	// Constraints (6) & (7): Allocation of clients to hubs due to its path
	for(IloInt i = 0; i < n; i++)
		for(IloInt j = 0; j < n; j++)
			for(IloInt k = 0; k < n; k++){
				IloExpr expr(getEnv());
				for(IloInt l = 0; l < n; l++)
					expr += f[i][j][k][l];
				IloConstraint c6 = (expr <= z[i][k]);
				stringstream c6_name;
				c6_name << "Cons(6)[" << i << "]" << "[" << j << "]" << "[" << k << "]";
				c6.setName(c6_name.str().c_str());
				add(c6);
			}
	for(IloInt i = 0; i < n; i++)
		for(IloInt j = 0; j < n; j++)
			for(IloInt l = 0; l < n; l++){
				IloExpr expr(getEnv());
				for(IloInt k = 0; k < n; k++)
					expr += f[i][j][k][l];
				IloConstraint c7 = (expr <= z[j][l]);
				stringstream c7_name;
				c7_name << "Cons(7)[" << i << "]" << "[" << j << "]" << "[" << l << "]";
				c7.setName(c7_name.str().c_str());
				add(c7);
			}

	// Defining values of z[k][k]
	/*for(IloInt k = 0; k < n; k++){
		IloConstraint c8;
		if(k == 3 || k == 6 || k == 11 || k == 13 || k == 16)
			c8 = (z[k][k] == 1.0);
		else
			c8 = (z[k][k] == 0.0);
		stringstream c8_name;
		c8_name << "Cons(8)[" << k << "]";
		c8.setName(c8_name.str().c_str());
		add(c8);
	}*/
}

void model::add_obj(){
	int n = instance.get_n();
	vector< vector< double > > traffics = instance.get_traffics();
	vector< vector< double > > distances = instance.get_distances();
	double collection_rate = instance.get_collection_rate();
	double transfer_rate = instance.get_transfer_rate();
	double distribution_rate = instance.get_distribution_rate();

	// Creating OF expression
	IloExpr expr(getEnv());
	for(IloInt i = 0; i < n; i++)
		for(IloInt j = 0; j < n; j++)
			for(IloInt k = 0; k < n; k++)
				for(IloInt l = 0; l < n; l++)
					expr += traffics[i][j] * ((collection_rate * distances[i][k]) + (transfer_rate * distances[k][l]) + (distribution_rate * distances[l][j])) * f[i][j][k][l];

	// Adding objective function
	add(IloMinimize(getEnv(), expr));
}

void model::add_remaining_const(){
	int n = instance.get_n();
	int p = sol.get_p();

	// Constraints (3): attribution only to hubs
	for(IloInt i = 0; i < n; i++){
		for(IloInt k = 0; k < n; k++){
			IloConstraint c3 = (z[i][k] <= z[k][k]);
			stringstream c3_name;
			c3_name << "Cons(3)[" << i << "]" << "[" << k << "]";
			c3.setName(c3_name.str().c_str());
			add(c3);
		}
	}

	// Constraint (4): allocation of p hubs
	IloExpr expr(getEnv());
	for(IloInt k = 0; k < n; k++)
		expr += z[k][k];
	IloConstraint c4 = (expr == p);
	stringstream c4_name;
	c4_name << "Cons(4)";
	c4.setName(c4_name.str().c_str());
	add(c4);

	// Constraints (5): traffic needs to be total routed
	for(IloInt i = 0; i < n; i++)
		for(IloInt j = 0; j < n; j++){
			IloExpr expr(getEnv());
			for(IloInt k = 0; k < n; k++)
				for(IloInt l = 0; l < n; l++)
					expr += f[i][j][k][l];
			IloConstraint c5 = (expr == 1);
			stringstream c5_name;
			c5_name << "Cons(5)[" << i << "]" << "[" << j << "]";
			c5.setName(c5_name.str().c_str());
			add(c5);
		}

	// Constraints (6) & (7): Allocation of clients to hubs due to its path
	for(IloInt i = 0; i < n; i++)
		for(IloInt j = 0; j < n; j++)
			for(IloInt k = 0; k < n; k++){
				IloExpr expr(getEnv());
				for(IloInt l = 0; l < n; l++)
					expr += f[i][j][k][l];
				IloConstraint c6 = (expr <= z[i][k]);
				stringstream c6_name;
				c6_name << "Cons(6)[" << i << "]" << "[" << j << "]" << "[" << k << "]";
				c6.setName(c6_name.str().c_str());
				add(c6);
			}
	for(IloInt i = 0; i < n; i++)
		for(IloInt j = 0; j < n; j++)
			for(IloInt l = 0; l < n; l++){
				IloExpr expr(getEnv());
				for(IloInt k = 0; k < n; k++)
					expr += f[i][j][k][l];
				IloConstraint c7 = (expr <= z[j][l]);
				stringstream c7_name;
				c7_name << "Cons(7)[" << i << "]" << "[" << j << "]" << "[" << l << "]";
				c7.setName(c7_name.str().c_str());
				add(c7);
			}
}

void model::add_lagrangian_obj(IloNumArray& l_multipliers){
	int n = instance.get_n();
	int r = sol.get_r();
	vector< vector< double > > traffics = instance.get_traffics();
	vector< vector< double > > distances = instance.get_distances();
	double collection_rate = instance.get_collection_rate();
	double transfer_rate = instance.get_transfer_rate();
	double distribution_rate = instance.get_distribution_rate();

	// Creating OF expression
	IloExpr expr(getEnv());
	for(IloInt i = 0; i < n; i++)
		for(IloInt j = 0; j < n; j++)
			for(IloInt k = 0; k < n; k++)
				for(IloInt l = 0; l < n; l++)
					expr += traffics[i][j] * ((collection_rate * distances[i][k]) + (transfer_rate * distances[k][l]) + (distribution_rate * distances[l][j])) * f[i][j][k][l];

	// Adding dualized constraints
	IloExpr dual(getEnv());
	for(IloInt i = 0; i < n; i++){
		IloExpr aux(getEnv());
		aux += r;
		for(IloInt k = 0; k < n; k++)
			aux -= z[i][k];
		dual += l_multipliers[i] * aux;
	}

	// Adding objective function
	add(IloMinimize(getEnv(), expr - dual));
}

