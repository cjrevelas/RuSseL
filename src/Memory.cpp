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
void Memory::SetEos(const std::string &id, std::shared_ptr<class Eos> eos) {
  if (IsEos(id)) { ExitProgram("Memory::SetEos", "Eos with " + id + " exists."); }
  eosMap_.insert(std::pair<std::string, std::shared_ptr<class Eos>>(id, eos));
}

void Memory::SetVariable(const std::string &id, const std::string &type, const std::string &stringValue, std::string stringExpressionPrefix) {
  if (IsVariable(id)) {
    GetVariable(id)->Reset(type, stringValue, stringExpressionPrefix);
    PrintWarning("Memory::SetVariable", "Variable " + id + " has been replaced.");
  } else {
    variableMap_.insert(std::pair<std::string, std::shared_ptr<Variable>>(id, std::make_shared<Variable>(type, stringValue, stringExpressionPrefix)));
  }
}

void Memory::SetVariable(const std::string &id, std::shared_ptr<const double> pcdbl) { // CHECK: pointer to heap here ?
  if (IsVariable(id)) {
    ExitProgram("Memory::SetVariable", "Variable " + id + " already exists.");
  } else {
    variableMap_.insert(std::pair<std::string, std::shared_ptr<Variable>>(id, std::make_shared<Variable>(pcdbl)));
  }
}

void Memory::SetVariable(const std::string &id, std::shared_ptr<const int> pcint) { // CHECK: pointer to heap here ?
  if (IsVariable(id)) {
    ExitProgram("Memory::SetVariable", "Variable " + id + " already exists.");
  } else {
    variableMap_.insert(std::pair<std::string, std::shared_ptr<Variable>>(id, std::make_shared<Variable>(pcint)));
  }
}

void Memory::SetVariable(const std::string &id, std::shared_ptr<std::string> pcstr) { // CHECK: pointer to heap here ?
  bool exists = IsVariable(id);
  if (exists) {
    ExitProgram("Memory::SetVariable", "Variable " + id + " already exists.");
  } else {
    variableMap_.insert(std::pair<std::string, std::shared_ptr<Variable>>(id, std::make_shared<Variable>(pcstr)));
  }
}

// Getters
std::shared_ptr<class Eos> Memory::GetEos(const std::string &id) {
  auto it = eosMap_.find(id);
  if (it == eosMap_.end()) { ExitProgram("Memory::GetEos", "failed to retrieve eos with id: " + id); }
  return it->second;
}

std::shared_ptr<Variable> Memory::GetVariable(const std::string &id) {
  auto varIt = variableMap_.find(id);

  if (varIt == variableMap_.end()) {
    ExitProgram("Memory::GetVariable", "failed to retrieve scalar with id:" + id);
  }

  return varIt->second;
}

// Checkers
bool Memory::IsEos(const std::string &id) {
  auto it = eosMap_.find(id);
  return (it != eosMap_.end());
}

bool Memory::IsVariable(const std::string &id) {
  auto varIt = variableMap_.find(id);
  return (varIt != variableMap_.end());
}

// Deleters
void Memory::DeleteEos(const std::string &id) {
  auto it = eosMap_.find(id);
  if (it == eosMap_.end()) { ExitProgram("Memory::DeleteEos", "failed to find eos " + id + " to delete."); }
  (it->second).reset();
  eosMap_.erase(it);
}

void Memory::DeleteVariable(const std::string &id) {
  auto it = variableMap_.find(id);
  if (it == variableMap_.end()) { ExitProgram("Memory::DeteleVariable", "failed to find id " + id + " to delete."); }
  (it->second).reset();
  variableMap_.erase(it);
}

void Memory::DeleteVariableWithTag(const std::string &tag) {
  auto it = variableMap_.begin();
  while (it != variableMap_.end()) {
    if (tag == it->first.substr(0, tag.size())) {
      (it->second).reset();
      variableMap_.erase(it++);
    } else {
      it++;
    }
  }
}

} // RusselNS
