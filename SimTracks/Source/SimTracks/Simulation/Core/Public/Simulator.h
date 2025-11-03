// Copyright (c) 2025 Tobias Cutik

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SimTracks/Simulation/UserOverridables/C++/Public/UserSimulation.h"
#include "SimTracks/Simulation/Core/Public/TrackManager.h"
#include "Simulator.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnUpdateSimTracksUi, float);

UCLASS()
class SIMTRACKS_API ASimulator : public AActor
{
	GENERATED_BODY()
	
public:	
	ASimulator();
	~ASimulator();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	virtual void Tick(float DeltaTime) override;

	void SetTrackManager(UTrackManager* Manager);
	void SetUserSimulation(UUserSimulation* Simulation);

	void StartSimulation();
	void PauseSimulation();
	bool IsSimulating();
	void PreviewCurrentSimState();

	void CreateNewTrack(const FString& ChangedSimParamName, float ChangedSimParamValue, unsigned int TrackId);

	FOnUpdateSimTracksUi UpdateSimTracksUi;

private:
	UPROPERTY()
	UTrackManager* TrackManager = nullptr;

	float SimulationPausedTimeTotal = 0.f;

	unsigned int CurrentTimeStep = 0;
	unsigned int LastCheckpointTimeStep = 0;

	float TimeStepSize = 0.f;
	unsigned int CheckpointIntervalsInSteps = 0;

	UUserSimulation* UserSimulation = nullptr;
	std::shared_ptr<FSimState> CurrentSimState = nullptr;
	bool bSimulating = false;

	void Simulate();
	void SaveCheckpoint();
};