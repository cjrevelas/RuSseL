#include <iomanip>

#include "Memory.hpp"

namespace RusselNS {

Memory::Memory() {
#ifdef EXPORT_MEMORY_STATUS
  PrintMessage("Create new Memory instance", 0);
#endif
  logArrays_.open("o.arrays", std::ios::out);
}

Memory::~Memory() {
#ifdef EXPORT_MEMORY_STATUS
  PrintMessage("Delete Memory instance", 0);
#endif
  logArrays_.close();
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
}

void Memory::ReportArrays() {
  if (!logArrays_) {
    std::cerr << "ERROR: o.arrays.txt file could not be opened for writing\n";
  } else {
    logArrays_ << "ww      ww_new      ww_mixed      phi_grafted      phi_matrix      phi_total\n";

    for (int ii=0; ii<mesh_->GetNumberOfNodes(); ++ii) {
      logArrays_ << std::setprecision(6) << std::scientific << wwField_[ii]      << "   ";
      logArrays_ << std::setprecision(6) << std::scientific << wwFieldNew_[ii]   << "   ";
      logArrays_ << std::setprecision(6) << std::scientific << wwFieldMixed_[ii] << "   ";
      logArrays_ << std::setprecision(6) << std::scientific << phiGrafted_[ii]   << "   ";
      logArrays_ << std::setprecision(6) << std::scientific << phiMatrix_[ii]    << "   ";
      logArrays_ << std::setprecision(6) << std::scientific << phiTotal_[ii]     << "   ";
      logArrays_ << '\n';
    }
  }
}

// Setters
void Memory::SetEos(const std::string &id, std::shared_ptr<class Eos> &eos) {
  if (IsEos(id)) { ExitProgram("Memory::SetEos", "Eos with " + id + " exists."); }
  eosMap_.insert(std::pair<std::string, std::shared_ptr<class Eos>>(id, eos));
}

void Memory::SetContour(const std::string &id, std::shared_ptr<class Contour> &contour) {
  if (IsContour(id)) { ExitProgram("Memory::SetContour", "Contour with " + id + " exists."); }
  contourMap_.insert(std::pair<std::string, std::shared_ptr<class Contour>>(id, contour));
}

void Memory::SetChain(const std::string &id, std::shared_ptr<class Chain> &chain) {
  if (IsChain(id)) { ExitProgram("Memory::SetChain", "Chain with " + id + " exists."); }
  chainMap_.insert(std::pair<std::string, std::shared_ptr<class Chain>>(id, chain));
}

void Memory::SetVariable(const std::string &id, const std::string &type, const std::string &stringValue, std::string stringExpressionPrefix) {
  if (IsVariable(id)) {
    GetVariable(id)->Reset(type, stringValue, stringExpressionPrefix);
    PrintWarning("Memory::SetVariable", "Variable " + id + " has been replaced.");
  } else {
    variableMap_.insert(std::pair<std::string, std::shared_ptr<Variable>>(id, std::make_shared<Variable>(type, stringValue, stringExpressionPrefix)));
  }
}

void Memory::SetVariable(const std::string &id, std::shared_ptr<const double> pcdbl) {
  if (IsVariable(id)) {
    ExitProgram("Memory::SetVariable", "Variable " + id + " already exists.");
  } else {
    variableMap_.insert(std::pair<std::string, std::shared_ptr<Variable>>(id, std::make_shared<Variable>(pcdbl)));
  }
}

void Memory::SetVariable(const std::string &id, std::shared_ptr<const int> pcint) {
  if (IsVariable(id)) {
    ExitProgram("Memory::SetVariable", "Variable " + id + " already exists.");
  } else {
    variableMap_.insert(std::pair<std::string, std::shared_ptr<Variable>>(id, std::make_shared<Variable>(pcint)));
  }
}

void Memory::SetVariable(const std::string &id, std::shared_ptr<std::string> pcstr) {
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

std::shared_ptr<class Contour> Memory::GetContour(const std::string &id) {
  auto it = contourMap_.find(id);
  if (it == contourMap_.end()) { ExitProgram("Memory::GetContour", "failed to retrieve contour with id: " + id); }
  return it->second;
}

std::shared_ptr<class Chain> Memory::GetChain(const std::string &id) {
  auto it = chainMap_.find(id);
  if (it == chainMap_.end()) { ExitProgram("Memory::GetChain", "failed to retrieve chain with id: " + id); }
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

bool Memory::IsContour(const std::string &id) {
  auto it = contourMap_.find(id);
  return (it != contourMap_.end());
}

bool Memory::IsChain(const std::string &id) {
  auto it = chainMap_.find(id);
  return (it != chainMap_.end());
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

void Memory::DeleteContour(const std::string &id) {
  auto it = contourMap_.find(id);
  if (it == contourMap_.end()) { ExitProgram("Memory::DeleteContour", "failed to find contour " + id + " to delete."); }
  (it->second).reset();
  contourMap_.erase(it);
}

void Memory::DeleteChain(const std::string &id) {
  auto it = chainMap_.find(id);
  if (it == chainMap_.end()) { ExitProgram("Memory::DeleteChain", "failed to find chain " + id + " to delete."); }
  (it->second).reset();
  chainMap_.erase(it);
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

void Memory::DeleteEosMap() {
  for (std::map<std::string, std::shared_ptr<class Eos>>::iterator it = eosMap_.begin(); it != eosMap_.end(); ++it) {
    (it->second).reset();
    eosMap_.erase(it);
  }
}

void Memory::DeleteContourMap() {
  for (std::map<std::string, std::shared_ptr<class Contour>>::iterator it = contourMap_.begin(); it != contourMap_.end(); ++it) {
    (it->second).reset();
    contourMap_.erase(it);
  }
}

void Memory::DeleteChainMap() {
  for (std::map<std::string, std::shared_ptr<class Chain>>::iterator it = chainMap_.begin(); it != chainMap_.end(); ++it) {
    (it->second).reset();
    chainMap_.erase(it);
  }
}

} // RusselNS
