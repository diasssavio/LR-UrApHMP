/*
 * solution.cpp
 *
 *  Created on: Apr 20, 2015
 *      Author: savio
 */

#include "../include/solution.h"

bool solution::my_sol_comparison( solution s1, solution s2 ){
	return (s1.get_total_cost() < s2.get_total_cost());
}

solution::solution(){

}

/*solution::solution( solution sol ){
	this->set_instance(sol.get_instance());
	this->set_p(sol.get_p());
	this->set_r(sol.get_r());
	this->set_alloc_hubs(sol.get_alloc_hubs());
	this->set_assigned_hubs(sol.get_assigned_hubs());
	this->set_f_chosen(sol.get_f_chosen());
	this->set_s_chosen(sol.get_s_chosen());
	this->set_cost(sol.get_cost());
	this->set_hubs_cost(sol.get_hubs_cost());
}*/

solution::solution( uraphmp& instance, int p_cons, int r_cons ) : p(p_cons), r(r_cons) {
	this->set_instance(instance);
}

solution::~solution() {

}

void solution::set_instance( uraphmp& instance ){
	this->instance = instance;
}

void solution::set_p( int p ){
	this->p = p;
}

void solution::set_r( int r ){
	this->r = r;
}

void solution::set_alloc_hubs( vector< int >& alloc_hubs ){
	this->alloc_hubs = alloc_hubs;
//	sort(this->alloc_hubs.begin(), this->alloc_hubs.end());
}

void solution::set_assigned_hubs( vector< vector< int > >& assigned_hubs ){
	this->assigned_hubs = assigned_hubs;
}

void solution::set_assigned_hub( int i, int j, int assigned_hub ){
	this->assigned_hubs[i][j] = assigned_hub;
}

void solution::set_f_chosen( vector< vector< int > >& f_chosen ){
	this->f_chosen = f_chosen;
}

void solution::set_s_chosen( vector< vector< int > >& s_chosen ){
	this->s_chosen = s_chosen;
}

void solution::set_cost( vector< vector< double > >& cost ){
	this->cost = cost;
	double _cost = 0.0;
	for(int i = 0; i < instance.get_n(); i++){
		//if(is_hub(i)) continue;
		for(int j = 0; j < instance.get_n(); j++){
			//if(is_hub(j)) continue;
			_cost += this->cost[i][j];
		}
	}
	this->_cost = _cost;
}

void solution::set_hubs_cost( vector< double >& hubs ){
	this->hubs_cost = hubs;
}

uraphmp& solution::get_instance(){
	return this->instance;
}

int solution::get_p(){
	return this->p;
}

int solution::get_r(){
	return this->r;
}

vector< int >& solution::get_alloc_hubs(){
	return this->alloc_hubs;
}

vector< vector<int > >& solution::get_assigned_hubs(){
	return this->assigned_hubs;
}
vector< int >& solution::get_assigned_hubs( int i ){
	return this->assigned_hubs[i];
}
vector< vector<int > >& solution::get_f_chosen(){
	return this->f_chosen;
}
vector< vector<int > >& solution::get_s_chosen(){
	return this->s_chosen;
}

vector< vector< double > >& solution::get_cost(){
	return this->cost;
}

vector< double >& solution::get_hubs_cost(){
	return this->hubs_cost;
}

double solution::get_total_hubs_cost(){
	double cost = 0.0;
	for(int i = 0; i < p; i++)
		cost += this->hubs_cost[i];

	return cost;
}

double solution::get_total_cost(){
	return _cost;
}

void solution::generate_hubs_cost(){
	vector< vector< double > > traffics = instance.get_traffics();
	vector< vector< double > > distances = instance.get_distances();
	double X = instance.get_collection_rate();
	double alpha = instance.get_transfer_rate();
	double delta = instance.get_distribution_rate();

	vector< double > costs;
	for(int h = 0; h < p; h++){
		double value1 = 0.0, value2 = 0.0;
		for(int i = 0; i < instance.get_n(); i++){
			if(is_hub(i)) continue;
			for(int j = 0; j < instance.get_n(); j++){
				if(is_hub(j)) continue;
				if(f_chosen[i][j] == alloc_hubs[h]){
					value1 += traffics[i][j] * ((X * distances[i][f_chosen[i][j]]) +
						((alpha/2) * distances[f_chosen[i][j]][s_chosen[i][j]]));
				}
				if(s_chosen[i][j] == alloc_hubs[h]){
					value2 += traffics[i][j]* (((alpha/2) * distances[s_chosen[i][j]][f_chosen[i][j]]) +
							delta * distances[s_chosen[i][j]][j]);
				}
			}
		}
		costs.push_back(value1 + value2);
	}

	this->hubs_cost = costs;
}

void solution::show_data(){
	printf("\np: %d\tr: %d\n", this->p, this->r);
	printf("Allocated Hubs: ");
	for(int i = 0; i < p; i++)
		printf("%d\t", alloc_hubs[i]);

	/*printf("\n\nAssigned Hubs:\n");
	for(int i = 0; i < instance.get_n(); i++){
		if(is_hub(i)) continue;
		printf("H[%d]: ", i);
		for(int j = 0; j < r; j++)
			printf("%d\t", assigned_hubs[i][j]);
		printf("\n");
	}

	printf("\n\nTraffics Routes:\n");
	for(int i = 0; i < instance.get_n(); i++){
		if(is_hub(i)) continue;
		printf("i[%d]: ", i);
		for(int j = 0; j < instance.get_n(); j++){
			if(is_hub(j)) continue;
			printf("%.2lf\t", cost[i][j]);
		}
		printf("\n");
	}

	printf("\n\nH1:\n");
	for(int i = 0; i < instance.get_n(); i++){
		if(is_hub(i)) continue;
		printf("i[%d]: ", i);
		for(int j = 0; j < instance.get_n(); j++){
			if(is_hub(j)) continue;
			printf("%d\t", f_chosen[i][j]);
		}
		printf("\n");
	}
	printf("\n\nH2:\n");
	for(int i = 0; i < instance.get_n(); i++){
		if(is_hub(i)) continue;
		printf("i[%d]: ", i);
		for(int j = 0; j < instance.get_n(); j++){
			if(is_hub(j)) continue;
			printf("%d\t", s_chosen[i][j]);
		}
		printf("\n");
	}*/

	printf("\n\nHUBS COST:\t");
	for(int i = 0; i < p; i++)
		printf("%.2lf\t", hubs_cost[i]);

	printf("\n\nTOTAL COST: %.2lf\n\n", get_total_cost());
}

bool solution::is_hub( int index ){
	return (find(alloc_hubs.begin(), alloc_hubs.end(), index) != alloc_hubs.end());
}

bool solution::my_comparison( pair< double, int > p1, pair< double, int > p2 ){
	return (p1.first < p2.first);
}

void solution::assign_hubs(){
	vector< vector< int > > assigned;

	vector< vector< double > > traffics = instance.get_traffics();
	vector< vector< double > > distances = instance.get_distances();

	for(int i = 0; i < instance.get_n(); i++){
		// Calculating alloc(i,h), i.e., the assignment cost of i to every hub h
		vector< pair< double, int> > alloc_value;
		for(int h = 0; h < p; h++){
			double value = 0.0;

			for(int j = 0; j < instance.get_n(); j++)
				value += traffics[i][j];
			value *= distances[i][alloc_hubs[h]];

			for(int j = 0; j < instance.get_n(); j++)
				value += traffics[i][j] * distances[alloc_hubs[h]][j];
			alloc_value.push_back(make_pair(value, alloc_hubs[h]));
		}

		// Sorting the elements of alloc_value
		sort(alloc_value.begin(), alloc_value.end(), my_comparison);

		// Selecting the r hubs to be assigned to i
		vector< int > i_assigned;
		for(int j = 0; j < r; j++)
			i_assigned.push_back(alloc_value[j].second);

		// Adding the hubs assigned to i (Hi)
		assigned.push_back(i_assigned);
	}

	set_assigned_hubs(assigned);
}

void solution::route_traffics(){
	vector< vector< double > > traffics = instance.get_traffics();
	vector< vector< double > > distances = instance.get_distances();

	vector< vector< double > > cost;
	vector< vector< int > > H1, H2;
	for(int i = 0; i < instance.get_n(); i++){
		vector< double > temp_cost;
		vector< int > temp_H1, temp_H2;
		for(int j = 0; j < instance.get_n(); j++){
			// Calculating all costs using all hubs
			vector< vector< double > > c_ij;
			for(int hi = 0; hi < r; hi++){
				vector< double > aux;
				for(int hj = 0; hj < r; hj++){
					double temp = traffics[i][j] * ((instance.get_collection_rate() * distances[i][assigned_hubs[i][hi]]) +
							(instance.get_transfer_rate() * distances[assigned_hubs[i][hi]][assigned_hubs[j][hj]]) +
							(instance.get_distribution_rate() * distances[assigned_hubs[j][hj]][j]));
					aux.push_back(temp);
				}
				c_ij.push_back(aux);
			}

			// Finding the minimum distance and hubs
			vector< double >::iterator it = min_element(c_ij[0].begin(), c_ij[0].end());
			int h1 = 0;
			int h2 = it - c_ij[0].begin();
			for(int hi = 1; hi < r; hi++){
				vector< double >::iterator it2 = min_element(c_ij[hi].begin(), c_ij[hi].end());
				if(*it2 < *it){
					it = it2;
					h1 = hi;
					h2 = it - c_ij[hi].begin();
				}
			}

			temp_cost.push_back(*it);
			temp_H1.push_back(assigned_hubs[i][h1]);
			temp_H2.push_back(assigned_hubs[j][h2]);
		}

		cost.push_back(temp_cost);
		H1.push_back(temp_H1);
		H2.push_back(temp_H2);
	}

	set_cost(cost);
	set_f_chosen(H1);
	set_s_chosen(H2);
	generate_hubs_cost();
}

void solution::assign_partial_hubs( int i, int j, int new_hub ){
	this->assigned_hubs[i][j] = new_hub;
}

void solution::route_partial_traffics( int new_hub ){

}
