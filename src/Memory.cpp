#include "Memory.hpp"
#include "Mesh.hpp"

namespace RusselNS {

Memory::Memory() {
#ifdef DEV_REPORT_HEAP
  PrintMessage("Add Memory", 1);
#endif
}

Memory::~Memory() {
#ifdef DEV_REPORT_HEAP
  PrintMessage("Delete Memory", 1);
#endif
}

void Memory::InitializeArrays() {
  wwField_     = std::shared_ptr<double []>(new double[mesh_->GetNumberOfNodes()]);
  wwFieldNew_  = std::shared_ptr<double []>(new double[mesh_->GetNumberOfNodes()]);
  wwFieldMixed_= std::shared_ptr<double []>(new double[mesh_->GetNumberOfNodes()]);
  phiGrafted_  = std::shared_ptr<double []>(new double[mesh_->GetNumberOfNodes()]);
  phiMatrix_   = std::shared_ptr<double []>(new double[mesh_->GetNumberOfNodes()]);
  phiTotal_    = std::shared_ptr<double []>(new double[mesh_->GetNumberOfNodes()]);

  for (int ii=0; ii<mesh_->GetNumberOfNodes(); ++ii) {
    wwField_[ii]      = 0.0;
    wwFieldNew_[ii]   = 0.0;
    wwFieldMixed_[ii] = 0.0;
    phiGrafted_[ii]   = 0.0;
    phiMatrix_[ii]    = 0.0;
    phiTotal_[ii]     = 0.0;
  }

  // TODO: add 2D propagator arrays using matrix.cpp with smart pointers
}

// Setters
void Memory::SetVariable(const std::string &id, const std::string &type, const std::string &stringValue, std::string stringExpressionPrefix) {
  bool exists = IsVariable(id);
  if (exists) {
    GetVariable(id)->Reset(type, stringValue, stringExpressionPrefix);
    PrintWarning("Memory::SetVariable", "Variable " + id + " has been replaced.");
  } else {
    // CJR: shared pointer for heap allocated memory here
    variableMap_.insert(std::pair<std::string, std::shared_ptr<Variable>>(id, std::make_shared<Variable>(type, stringValue, stringExpressionPrefix)));
  }
}

void Memory::SetVariable(const std::string &id, std::shared_ptr<const double> pcdbl) { // CHECK: pointer to heap here ?
  bool exists = IsVariable(id);
  if (exists) {
    ExitProgram("Memory::SetVariable", "Variable " + id + " already exists.");
  } else {
    // CJR: shared pointer for heap allocated memory here
    variableMap_.insert(std::pair<std::string, std::shared_ptr<Variable>>(id, std::make_shared<Variable>(pcdbl)));
  }
}

void Memory::SetVariable(const std::string &id, std::shared_ptr<const int> pcint) { // CHECK: pointer to heap here ?
  bool exists = IsVariable(id);
  if (exists) {
    ExitProgram("Memory::SetVariable", "Variable " + id + " already exists.");
  } else {
    // CJR: shared pointer for heap allocated memory here
    variableMap_.insert(std::pair<std::string, std::shared_ptr<Variable>>(id, std::make_shared<Variable>(pcint)));
  }
}

void Memory::SetVariable(const std::string &id, std::shared_ptr<std::string> pcstr) { // CHECK: pointer to heap here ?
  bool exists = IsVariable(id);
  if (exists) {
    ExitProgram("Memory::SetVariable", "Variable " + id + " already exists.");
  } else {
    // CJR: shared pointer for heap allocated memory here
    variableMap_.insert(std::pair<std::string, std::shared_ptr<Variable>>(id, std::make_shared<Variable>(pcstr)));
  }
}

// Getters
std::shared_ptr<Variable> Memory::GetVariable(const std::string &id) {
  std::unordered_map<std::string, std::shared_ptr<Variable>>::iterator varIt = variableMap_.find(id);

  if (varIt == variableMap_.end()) {
    ExitProgram("Memory::GetVariable", "failed to retrieve scalar with id:" + id);
  }

  return varIt->second;
}

// Checkers
bool Memory::IsVariable(const std::string &id) {
  std::unordered_map<std::string, std::shared_ptr<Variable>>::iterator varIt = variableMap_.find(id);
  return (varIt != variableMap_.end());
}


} // RusselNS
