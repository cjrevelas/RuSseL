#ifndef DEFAULTS_HPP
#define DEFAULTS_HPP

namespace RusselNS {

  double dflt_adsorptionDistance    = 1.2e1;
  double dflt_numGraftedChainsTol   = 5.0e-3;
  double dflt_graftPointDistance    = 4.0e-1;
  double dflt_binThickness          = 5.0e-1;
  double dflt_fraction              = 1.0e0;
  double dflt_fieldTol              = 5.0e-1;
  double dflt_freeEnergyTol         = 1.0e-6;
  double dflt_freeEnergyTolForDelta = 1.0e-3; 
  // double polymerHamakerAlpha        = 0.0e0;
  double dflt_wallDistance          = 5.0e0;
  double dflt_pressure              = 0.0e0;

  int dflt_iterations          = 1;
  int dlft_initialIterationId  = 0;
  int dflt_matrixExist         = 0;
  int dflt_graftedExist        = 1;
  int dflt_profileDimensions   = 3;
  int dlft_mumpsMatrixType     = 1; // TODO: change this to mumps_asymm enum value
  int dflt_exportPhiNodal      = 1;
  int dflt_exportPhiSmeared    = 0;
  int dflt_exportPhiIndividual = 0;
  int dflt_exportPhiEndMiddle  = 0;
  int dflt_exportField         = 1;
  int dflt_exportFieldBin      = 1;
  int dflt_exportPropagators   = 0;
  int dflt_exportBrushThickess = 0;
  int dflt_exportChainsPerArea = 0;
  int dflt_exportAdsorbedFree  = 0;

  bool dflt_domainIsPeriodic = false;
  bool dflt_squareGradient   = false;

} // RusselN

#endif
