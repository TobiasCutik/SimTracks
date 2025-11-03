// Copyright (c) 2025 Tobias Cutik

#include "SimTracks/Simulation/UserOverridables/C++/Public/SimState.h"

FSimState::FSimState(TArray<FSimulationParameter> SimulationParams, TArray<FEvaluationParameter> EvaluationParams)
{
	for (FSimulationParameter& SimulationParam : SimulationParams)
		this->SimulationParams.Add(SimulationParam.GetName(), SimulationParam.GetStartingValue());
		
	for (FEvaluationParameter& EvaluationParam : EvaluationParams)
		this->EvaluationParams.Add(EvaluationParam.GetName(), EvaluationParam.GetStartingValue());
}

FSimState::~FSimState()
{
}