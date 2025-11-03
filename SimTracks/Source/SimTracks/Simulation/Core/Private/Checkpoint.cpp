// Copyright (c) 2025 Tobias Cutik

#include "SimTracks/Simulation/Core/Public/Checkpoint.h"

FCheckpoint::FCheckpoint(const std::shared_ptr<const FSimState>& SimState, float SimulationTime)
{
	this->SimState = SimState;
    this->Time = SimulationTime;
}

FCheckpoint::~FCheckpoint()
{
}