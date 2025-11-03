// Copyright (c) 2025 Tobias Cutik


#include "SimTracks/Simulation/Core/Public/Simulator.h"

ASimulator::ASimulator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

ASimulator::~ASimulator()
{
}

void ASimulator::BeginPlay()
{
	Super::BeginPlay();
}

void ASimulator::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ASimulator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bSimulating || !CurrentSimState || !CurrentSimState->bValid)
		return;

	const float TimeSimulationActive = GetWorld()->GetRealTimeSeconds() - SimulationPausedTimeTotal;

	while (TimeSimulationActive >= TimeStepSize * CurrentTimeStep)
	{
		Simulate();

		if (UserSimulation && TrackManager && CurrentTimeStep % CheckpointIntervalsInSteps == 0)
			SaveCheckpoint();

		CurrentTimeStep++;
	}
}

void ASimulator::Simulate()
{
	if (UserSimulation && CurrentSimState)
		UserSimulation->Simulate(CurrentSimState, TimeStepSize, TimeStepSize * CurrentTimeStep);
}

void ASimulator::SetUserSimulation(UUserSimulation* Simulation)
{
	if (!Simulation)
		return;

	this->UserSimulation = Simulation;
	this->UserSimulation->Initialize();

	TimeStepSize = this->UserSimulation->GetTimeStepSize();
	CheckpointIntervalsInSteps = this->UserSimulation->GetCheckpointIntervalsInSteps();
}

void ASimulator::StartSimulation()
{
	if (bSimulating || !UserSimulation)
		return;

	if (!CurrentSimState)
		PreviewCurrentSimState();

	bSimulating = true;

	UserSimulation->StartSimulation(CurrentSimState);
}

void ASimulator::PauseSimulation()
{
	if (!bSimulating || !CurrentTimeStep)
		return;

	bSimulating = false;

	// reset CurrentTimeStep to be able to deduce when the next checkpoint must be saved
	CurrentTimeStep = LastCheckpointTimeStep;

	// set CurrentSimState invalid as CurrentTimeStep was changed 
	CurrentSimState->bValid = false;
}

bool ASimulator::IsSimulating()
{
	return bSimulating;
}

void ASimulator::PreviewCurrentSimState()
{
	if (bSimulating || !TrackManager || !UserSimulation)
		return;

	if (const FCheckpoint* CurrentCheckpoint = TrackManager->GetCurrentCheckpoint())
	{
		CurrentSimState = UserSimulation->CopySimState(CurrentCheckpoint->SimState);
		CurrentTimeStep = CurrentCheckpoint->Time / TimeStepSize + 1;
	}
	else
	{
		CurrentSimState = UserSimulation->GetInitSimState();
		CurrentTimeStep = 1;
	}

	const float SimulatedTime = TimeStepSize * (CurrentTimeStep - 1);
	SimulationPausedTimeTotal = GetWorld()->GetRealTimeSeconds() - SimulatedTime;

	UserSimulation->ApplySimState(CurrentSimState);
}

void ASimulator::SetTrackManager(UTrackManager* Manager)
{
	TrackManager = Manager;
}

void ASimulator::SaveCheckpoint()
{
	const float SimulationTime = TimeStepSize * CurrentTimeStep;
	TrackManager->SaveCheckpoint(UserSimulation->CopySimState(CurrentSimState), SimulationTime);
	UpdateSimTracksUi.Broadcast(SimulationTime);
	LastCheckpointTimeStep = CurrentTimeStep;
}

void ASimulator::CreateNewTrack(const FString& ChangedSimParamName, float ChangedSimParamValue, unsigned int TrackId)
{
	if (!TrackManager)
		return;

	if (const FCheckpoint* CurrentCheckpoint = TrackManager->GetCurrentCheckpoint())
	{
		if (!UserSimulation)
			return;

		CurrentSimState = UserSimulation->CopySimState(CurrentCheckpoint->SimState);
		CurrentTimeStep = CurrentCheckpoint->Time / TimeStepSize;

		// apply ChangedSimParamValue to ChangedSimParam
		const auto Value = CurrentSimState->SimulationParams.Find(ChangedSimParamName);
		*Value = ChangedSimParamValue;

		TrackManager->CreateNewTrack(TrackId);
		SaveCheckpoint();
		UserSimulation->ApplySimState(CurrentSimState);
	}
	else
	{
		TrackManager->CreateNewTrack(TrackId);
	}
}