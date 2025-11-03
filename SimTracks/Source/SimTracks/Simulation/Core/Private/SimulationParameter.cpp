// Copyright (c) 2025 Tobias Cutik

#include "SimTracks/Simulation/Core/Public/SimulationParameter.h"

FSimulationParameter::FSimulationParameter(FString Name, float MinValue, float MaxValue, float StartingValue)
{
	this->Name = Name;
	this->MinValue = MinValue;
	this->MaxValue = MaxValue;
	this->StartingValue = StartingValue;
}

FString& FSimulationParameter::GetName()
{
	return Name;
}

float FSimulationParameter::GetMinValue()
{
	return MinValue;
}

float FSimulationParameter::GetMaxValue()
{
	return MaxValue;
}

float FSimulationParameter::GetStartingValue()
{
	return StartingValue;
}

FSimulationParameter::~FSimulationParameter()
{
}