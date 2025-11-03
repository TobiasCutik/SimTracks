// Copyright (c) 2025 Tobias Cutik

#include "SimTracks/EpidemicSimulation/Public/EpidemicSimState.h"

FEpidemicSimState::FEpidemicSimState(const TArray<FSimulationParameter>& SimulationParams, const TArray<FEvaluationParameter>& EvaluationParams)
	: FSimState(SimulationParams, EvaluationParams)
{
}