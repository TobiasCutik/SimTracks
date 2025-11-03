// Copyright (c) 2025 Tobias Cutik


#include "SimTracks/Simulation/UserOverridables/C++/Public/UserSimulation.h"

void UUserSimulation::OnConstruction()
{
}

void UUserSimulation::Initialize()
{
}

void UUserSimulation::ApplySimState(const std::shared_ptr<FSimState>& SimState)
{
}

void UUserSimulation::StartSimulation(const std::shared_ptr<FSimState>& SimState)
{
}

void UUserSimulation::Simulate(const std::shared_ptr<FSimState>& SimState, float DeltaTime, float SimulationTime)
{
}

float UUserSimulation::GetTimeStepSize()
{
	return TimeStepSize;
}

unsigned int UUserSimulation::GetCheckpointIntervalsInSteps()
{
	return CheckpointIntervalsInSteps;
}

std::shared_ptr<FSimState> UUserSimulation::GetInitSimState()
{
	return std::make_shared<FSimState>(SimulationParams, EvaluationParams);
}

std::shared_ptr<FSimState> UUserSimulation::CopySimState(const std::shared_ptr<const FSimState>& SimState)
{
	return std::make_shared<FSimState>(*SimState);
}

TArray<FSimulationParameter> UUserSimulation::GetSimulationParams() const
{
	return SimulationParams;
}

TArray<FEvaluationParameter> UUserSimulation::GetEvaluationParams() const
{
	return EvaluationParams;
}