#ifndef TYPEDEF_HPP_
#define TYPEDEF_HPP_

// SYSTEM LIBRARIES
#include <ilcplex/ilocplex.h>
#include <vector>

///// CONCERT
typedef IloArray<IloNumVarArray> IloNumVarArray2;
typedef IloArray<IloNumVarArray2> IloNumVarArray3;
typedef IloArray<IloNumVarArray3> IloNumVarArray4;

typedef IloArray<IloNumArray2> IloNumArray3;
typedef IloArray<IloNumArray3> IloNumArray4;

typedef IloArray<IloFloatVarArray> IloFloatVarArray2;
typedef IloArray<IloFloatVarArray2> IloFloatVarArray3;
typedef IloArray<IloFloatVarArray3> IloFloatVarArray4;

typedef IloArray<IloBoolVarArray> IloBoolVarArray2;
typedef IloArray<IloBoolVarArray2> IloBoolVarArray3;
typedef IloArray<IloBoolVarArray3> IloBoolVarArray4;

typedef std::vector<int>::iterator VecIntIT;
typedef std::vector<int>::const_iterator VecIntCIT;
typedef std::vector<int> VecInt;

#endif /* TYPEDEF_HPP_ */
