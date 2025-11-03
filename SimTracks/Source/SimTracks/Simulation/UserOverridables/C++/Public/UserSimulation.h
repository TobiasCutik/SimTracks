// Copyright (c) 2025 Tobias Cutik

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SimTracks/Simulation/UserOverridables/C++/Public/SimState.h"
#include "UserSimulation.generated.h"

UCLASS(Abstract)
class SIMTRACKS_API UUserSimulation : public UObject
{
	GENERATED_BODY()
	
public:	
	UUserSimulation() = default;

	virtual void OnConstruction();
	virtual void Initialize();
	virtual void ApplySimState(const std::shared_ptr<FSimState>& SimState);
	virtual void StartSimulation(const std::shared_ptr<FSimState>& SimState);
	virtual void Simulate(const std::shared_ptr<FSimState>& SimState, float DeltaTime, float SimulationTime);

	TArray<FSimulationParameter> GetSimulationParams() const;
	TArray<FEvaluationParameter> GetEvaluationParams() const;
	float GetTimeStepSize();
	unsigned int GetCheckpointIntervalsInSteps();
	virtual std::shared_ptr<FSimState> GetInitSimState();
	virtual std::shared_ptr<FSimState> CopySimState(const std::shared_ptr<const FSimState>& SimState);
	
protected:
	TArray<FSimulationParameter> SimulationParams;
	TArray<FEvaluationParameter> EvaluationParams;

	UPROPERTY(EditDefaultsOnly)
	float TimeStepSize = 0.02f;

	UPROPERTY(EditDefaultsOnly)
	unsigned int CheckpointIntervalsInSteps = 5; 
};
