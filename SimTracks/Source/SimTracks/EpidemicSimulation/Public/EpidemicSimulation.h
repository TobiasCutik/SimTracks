// Copyright (c) 2025 Tobias Cutik

#pragma once

#include "CoreMinimal.h"
#include "SimTracks/EpidemicSimulation/Public/EpidemicSimState.h"
#include "SimAgent.h"
#include "SimTracks/Simulation/UserOverridables/C++/Public/UserSimulation.h"
#include "EpidemicSimulation.generated.h"

UCLASS(Blueprintable)
class SIMTRACKS_API UEpidemicSimulation : public UUserSimulation
{
	GENERATED_BODY()
	
public:	
	UEpidemicSimulation();
	~UEpidemicSimulation();

	virtual void OnConstruction() override;
	virtual void Initialize() override;
	virtual void ApplySimState(const std::shared_ptr<FSimState>& SimState) override;
	virtual void StartSimulation(const std::shared_ptr<FSimState>& SimState) override;
	virtual void Simulate(const std::shared_ptr<FSimState>& SimState, float DeltaTime, float SimulationTime) override;
	virtual std::shared_ptr<FSimState> GetInitSimState() override;
	virtual std::shared_ptr<FSimState> CopySimState(const std::shared_ptr<const FSimState>& SimState) override;

	UFUNCTION(BlueprintCallable)
	void SetSpawnArea(UStaticMeshComponent* Area);

private:
	void SetupSimulationParams();
	void SetupEvaluationParams();
	void SpawnAgents();
	void InitializeAgentStates();
	void InitializeAgentStartLocations();
	void InitializeAgentInfectionParameters();
	void InfectFirstAgent();
	void SimulateAgents(const std::shared_ptr<FEpidemicSimState>& SimState, float DeltaTime);
	void UpdateRandomMovement(const FRandomStream& RandomStream);
	void UpdateSimulationParameters(const std::shared_ptr<FEpidemicSimState>& SimState);
	void CalculateEvaluationParameters(const std::shared_ptr<FEpidemicSimState>& SimState, float SimulationTime);

	TArray<ASimAgent*> Agents;
	TArray<FAgentState> AgentStates;
	UStaticMeshComponent* SpawnArea;
	FRandomStream AgentSpawnRandomStream;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASimAgent> SimAgentToSpawn;

	UPROPERTY(EditDefaultsOnly)
	float AgentSpawnSafetyMargin = 20.f;

	UPROPERTY(EditDefaultsOnly)
	int AgentsAmount = 300;

	UPROPERTY(EditDefaultsOnly)
	float AgentMovementChangeInterval = 1.f;

	// Simulation Parameters
	const FString AgentSpeedString = "AgentSpeed";
	const FString InfectionRateString = "InfectionRate";
	const FString RecoveryTimeString = "RecoveryTime";
	UPROPERTY(EditDefaultsOnly)
	float AgentSpeed = 84.f;
	UPROPERTY(EditDefaultsOnly)
	float InfectionRate = 0.5f;
	UPROPERTY(EditDefaultsOnly)
	float RecoveryTime = 7.f;

	// Evaluation Parameters
	const FString BasicRValueString = "BasicRValue";
	const FString BasicRValueStringAlt = "BasicRValueAlt";
	const FString EffectiveRValueString = "EffectiveRValue";
	const FString InfectedPercentageString = "InfectedPercentage";
};