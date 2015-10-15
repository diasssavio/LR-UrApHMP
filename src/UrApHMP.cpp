/*
 * uraphmp.cpp
 *
 *  Created on: Apr 20, 2015
 *      Author: savio
 */

#include "../include/UrApHMP.h"

uraphmp::uraphmp(){
}

uraphmp::uraphmp( int n ) : n(n) {
}

uraphmp::uraphmp( int n, double X, double a, double del ) : n(n), collection_rate(X), transfer_rate(a), distribution_rate(del) {
}

uraphmp::~uraphmp() {

}

void uraphmp::set_n( int n ){
	this->n = n;
}

void uraphmp::set_traffics( vector< vector< double> >& traffics){
	this->traffics = traffics;
}

void uraphmp::set_distances( vector< vector< double> >& distances){
	this->distances = distances;
}

int uraphmp::get_n(){
	return this->n;
}

vector< vector< double > >& uraphmp::get_traffics(){
	return this->traffics;
}

vector< vector< double > >& uraphmp::get_distances(){
	return this->distances;
}

double uraphmp::get_collection_rate(){
	return this->collection_rate;
}

double uraphmp::get_transfer_rate(){
	return this->transfer_rate;
}

double uraphmp::get_distribution_rate(){
	return this->distribution_rate;
}

void uraphmp::show_data(){
	printf("number of points: %d\n\n", n);
	printf("trafics matrix:\n");
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++)
			printf("%.2lf\t", traffics[i][j]);
		printf("\n");
	}
	printf("\n\ndistances matrix:\n");
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++)
			printf("%.2lf\t", distances[i][j]);
		printf("\n");
	}
}
