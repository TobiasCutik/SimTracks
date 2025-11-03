// Copyright (c) 2025 Tobias Cutik

#include "SimTracks/Simulation/Core/Public/EvaluationParameter.h"

FEvaluationParameter::FEvaluationParameter(FString Name, float WorstValue, float BestValue, float StartingValue)
{
	this->Name = Name;
	this->WorstValue = WorstValue;
	this->BestValue = BestValue;
	this->StartingValue = StartingValue;
}

FString& FEvaluationParameter::GetName()
{
	return Name;
}

float FEvaluationParameter::GetWorstValue()
{
	return WorstValue;
}

float FEvaluationParameter::GetBestValue()
{
	return BestValue;
}

float FEvaluationParameter::GetStartingValue()
{
	return StartingValue;
}

FEvaluationParameter::~FEvaluationParameter()
{
}