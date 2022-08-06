#ifndef SOLVER_MODULE_H_
#define SOLVER_MODULE_H_

class Problem;

class SolverModule {
public:
	SolverModule(const Problem& problem) : ref_pb(problem) {};

protected:
    const Problem& ref_pb;
};

#endif // SOLVER_MODULE_H_
