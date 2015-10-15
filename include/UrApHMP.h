/*
 * uraphmp.h
 *
 *  Created on: Apr 20, 2015
 *      Author: Sávio S. Dias
 */

#ifndef URAPHMP_H_
#define URAPHMP_H_

#include <vector>
#include <cstdio>

using namespace std;

class uraphmp {
private:
	// Primary parameter
	int n; // number of nodes on the instance
	double collection_rate; // X
	double transfer_rate; // alpha
	double distribution_rate; // delta

	// General data
	vector< vector< double > > traffics; // traffics matrix from node i to j
	vector< vector< double > > distances; // distances matrix from node i to j

public:
	// Constructors & Destructors
	uraphmp();
	uraphmp( int );
	uraphmp( int, double, double, double );
	virtual ~uraphmp();

	// Setters
	void set_n( int );
	void set_traffics( vector< vector< double > >& );
	void set_distances( vector< vector< double > >& );

	// Getters
	int get_n();
	vector< vector< double > >& get_traffics();
	vector< vector< double > >& get_distances();

	double get_collection_rate();
	double get_transfer_rate();
	double get_distribution_rate();

	// Useful Methods
	void show_data();
};

#endif /* URAPHMP_H_ */
