#ifndef SOLVER_MODULE_H_
#define SOLVER_MODULE_H_

class SolverModule {
public:
	virtual void ResourcesAlloc(const Problem&) = 0;
};

#endif // SOLVER_MODULE_H_

