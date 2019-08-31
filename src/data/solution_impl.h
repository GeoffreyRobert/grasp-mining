#ifndef SOLUTION_IMPL_
#define SOLUTION_IMPL_

#include <vector>

#include "problem.h"

using std::vector;

template<class T>
class VecAccess {
public:
    T& operator[](const unsigned oid) {
        return accessFn(implSol, oid);
    };
private:
    const std::function<T(const AccessImpl&, const unsigned)> accessFn;
};

class StructOfVec {
public:
    StructOfVec(const Problem&);

    StructOfVec(const StructOfVec&);

    StructOfVec& operator=(const StructOfVec&);

    StructOfVec& operator=(StructOfVec&&);

    unsigned& getStartDate(unsigned);
    unsigned& getEndDate(unsigned);
    int& getMacParent(unsigned);
    bool& getIsCritMachine(unsigned);
    int& getMacChild(unsigned);

private:
    // structure de vecteurs
    vector<unsigned int> startDate;				// date de début de chaque opération
    vector<unsigned int> endDate;				// date de fin de chaque operation
    vector<int> macParent;						// parent sur la machine
    vector<bool> isCritMachine;					// parent critique sur la machine
    vector<int> macChild;						// successeur(s) sur la machine
};


class VecOfStruct {
public:
    VecOfStruct(const Problem&);

    VecOfStruct(const VecOfStruct&);

    VecOfStruct& operator=(const VecOfStruct&);

    VecOfStruct& operator=(VecOfStruct&&);

    unsigned& getStartDate(unsigned);
    unsigned& getEndDate(unsigned);
    int& getMacParent(unsigned);
    bool& getIsCritMachine(unsigned);
    int& getMacChild(unsigned);

private:
    // vecteur de structures
    struct OperationData {
        unsigned int startDate;				// date de début de chaque opération
        unsigned int endDate;				// date de fin de chaque operation
        int macParent;						// parent sur la machine
        bool isCritMachine;					// parent critique sur la machine
        int macChild;						// successeur(s) sur la machine
    };
    vector<OperationData> dataVec;
};
#endif
