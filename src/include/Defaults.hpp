#ifndef DEFAULTS_HPP
#define DEFAULTS_HPP

#include "MumpsFlags.hpp"

namespace RusselNS {
  constexpr double dflt_adsorptionDistance    = 1.2e1;
  constexpr double dflt_numGraftedChainsTol   = 5.0e-3;
  constexpr double dflt_graftPointDistance    = 4.0e-1;
  constexpr double dflt_binThickness          = 5.0e-1;
  constexpr double dflt_fraction              = 1.0e0;
  constexpr double dflt_fieldTol              = 5.0e-1;
  constexpr double dflt_freeEnergyTol         = 1.0e-6;
  constexpr double dflt_freeEnergyTolForDelta = 1.0e-3; 
  constexpr double dflt_wallDistance          = 5.0e0;
  constexpr double dflt_pressure              = 0.0e0;

  constexpr int dflt_iterations          = 1;
  constexpr int dlft_initialIterationId  = 0;
  constexpr int dflt_matrixExist         = 0;
  constexpr int dflt_graftedExist        = 1;
  constexpr int dflt_profileDimensions   = 3;
  constexpr int dlft_mumpsMatrixType     = static_cast<int>(Mumps::asymm);
  constexpr int dflt_exportPhiNodal      = 1;
  constexpr int dflt_exportPhiSmeared    = 0;
  constexpr int dflt_exportPhiIndividual = 0;
  constexpr int dflt_exportPhiEndMiddle  = 0;
  constexpr int dflt_exportField         = 1;
  constexpr int dflt_exportFieldBin      = 1;
  constexpr int dflt_exportPropagators   = 0;
  constexpr int dflt_exportBrushThickess = 0;
  constexpr int dflt_exportChainsPerArea = 0;
  constexpr int dflt_exportAdsorbedFree  = 0;

  constexpr bool dflt_domainIsPeriodic = false;
  constexpr bool dflt_squareGradient   = false;
} // RusselN

#endif // DEFAULTS_HPP
