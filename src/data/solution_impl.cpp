#include "solution_impl.h"


StructOfVec::StructOfVec(const Problem& problem) :
    startDate(problem.size), endDate(problem.size),
    macParent(problem.size), macChild(problem.size),
    isCritMachine(problem.size)
{
}

StructOfVec::StructOfVec(const StructOfVec& other) :
    startDate(other.startDate), endDate(other.endDate),
    macParent(other.macParent), macChild(other.macChild),
    isCritMachine(other.isCritMachine)
{
}

StructOfVec& StructOfVec::operator=(const StructOfVec& other) {
    if (this != &other) {
        startDate = other.startDate;
        endDate = other.endDate;
        macParent = other.macParent;
        macChild = other.macChild;
        isCritMachine = other.isCritMachine;
    }
    return *this;
}

StructOfVec& StructOfVec::operator=(StructOfVec&& other) {
    if (this != &other) {
        startDate = std::move(other.startDate);
        endDate = std::move(other.endDate);
        macParent = std::move(other.macParent);
        macChild = std::move(other.macChild);
        isCritMachine = std::move(other.isCritMachine);
    }
    return *this;
}

unsigned& StructOfVec::getStartDate(const unsigned oid) {
    return startDate[oid];
}
unsigned& StructOfVec::getEndDate(const unsigned oid) {
    return endDate[oid];
}
int& StructOfVec::getMacParent(const unsigned oid) {
    return macParent[oid];
}
bool& StructOfVec::getIsCritMachine(const unsigned oid) {
    return isCritMachine[oid];
}
int& StructOfVec::getMacChild(const unsigned oid) {
    return macChild[oid];
}

// --------------------------- //

VecOfStruct::VecOfStruct(const Problem& problem) :
    dataVec(problem.size)
{
}

VecOfStruct::VecOfStruct(const VecOfStruct& other) :
    dataVec(other.dataVec)
{
}

VecOfStruct& VecOfStruct::operator=(const VecOfStruct& other) {
    if (this != &other) {
        dataVec = other.dataVec;
    }
    return *this;
}

VecOfStruct& VecOfStruct::operator=(VecOfStruct&& other) {
    if (this != &other) {
        dataVec = std::move(other.dataVec);
    }
    return *this;
}

unsigned& VecOfStruct::getStartDate(unsigned oid) {
    return dataVec[oid].startDate;
}
unsigned& VecOfStruct::getEndDate(unsigned oid) {
    return dataVec[oid].endDate;
}
int& VecOfStruct::getMacParent(unsigned oid) {
    return dataVec[oid].macParent;
}
bool& VecOfStruct::getIsCritMachine(unsigned oid) {
    return dataVec[oid].isCritMachine;
}
int& VecOfStruct::getMacChild(unsigned oid) {
    return dataVec[oid].macChild;
}