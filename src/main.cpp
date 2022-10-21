#include <iostream>
#include "dmumps_c.h"
#include "mesh.hpp"

int main(int argc, char *argv[]) {
  int gid = 4852;

  Mesh *mesh = new Mesh("in.mesh");

  mesh->elementsContainingNode(gid);
  std::cout << mesh->computeMeshVolume() << '\n';

  delete mesh;
  mesh = NULL;

  return 0;
}
