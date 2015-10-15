/*
 * solution.h
 *
 *  Created on: Apr 20, 2015
 *      Author: Sávio S. Dias
 */

#ifndef SOLUTION_H_
#define SOLUTION_H_

#include <vector>
#include <algorithm>
#include <utility>
#include <limits>

#include "../include/UrApHMP.h"

using namespace std;

class solution{
private:
	// Instance
	uraphmp instance;

	// Primary parameters
	int p; // Number of hubs to be allocated
	int r; // Number of hubs to be assigned to each node

	// Solution itself
	vector< int > alloc_hubs; // Allocated hubs. Size: p. H
	vector< vector< int > > assigned_hubs; // Assigned hubs to each node. Size: nxr. A
	vector< vector< int > > f_chosen; // First chosen hub to route between i and j. Size: nxn. H(1)
	vector< vector< int > > s_chosen; // Second chosen hub to route between i and j. Size: nxn. H(2)

	// Solution cost
	vector< vector< double > > cost; // Cost matrix to go from i to j using H(1)ij and H(2)ij. Size: nxn.
	vector< double > hubs_cost; // Hubs cost in the objective function. Size: p.
	double _cost;

public:
	// Constructors & Destructors
	solution();
	solution( uraphmp&, int, int );
//	solution( solution );
	virtual ~solution();

	// Setters
	void set_instance( uraphmp& );

	void set_p( int );
	void set_r( int );

	void set_alloc_hubs( vector< int >& );
	void set_assigned_hubs( vector< vector< int > >& );
	void set_assigned_hub( int, int, int );
	void set_f_chosen( vector< vector< int > >& );
	void set_s_chosen( vector< vector< int > >& );

	void set_cost( vector< vector< double > >& );
	void set_hubs_cost( vector< double >& );

	// Getters
	uraphmp& get_instance();

	int get_p();
	int get_r();

	vector< int >& get_alloc_hubs();
	vector< vector< int > >& get_assigned_hubs();
	vector< int >& get_assigned_hubs( int );
	vector< vector< int > >& get_f_chosen();
	vector< vector< int > >& get_s_chosen();

	vector< vector< double > >& get_cost();
	vector< double >& get_hubs_cost();
	double get_total_cost(); // Objective function value
	double get_total_hubs_cost();

	// Useful Methods
	static bool my_comparison( pair< double, int >, pair< double, int > );
	static bool my_sol_comparison( solution, solution );
	void show_data();
	bool is_hub( int );

	void assign_hubs();
	void assign_partial_hubs( int, int, int );
	void route_traffics();
	void route_partial_traffics( int );
	void generate_hubs_cost(); // Generate the hubs costs in the solution
};

#endif /* SOLUTION_H_ */
