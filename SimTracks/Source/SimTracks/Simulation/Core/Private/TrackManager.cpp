// Copyright (c) 2025 Tobias Cutik


#include "SimTracks/Simulation/Core/Public/TrackManager.h"

void UTrackManager::Initialize()
{
	if (!SimTrackGraph)
		SimTrackGraph = NewObject<USimTrackGraph>(this, USimTrackGraph::StaticClass());
}

void UTrackManager::SaveCheckpoint(const std::shared_ptr<const FSimState>& SimState, float SimulationTime)
{
	if (!CurrentTrack)
		return;

	CurrentTrack->AddCheckpoint(SimState, SimulationTime);
	SetCurrentCheckpointTime(SimulationTime);
}

const FCheckpoint* UTrackManager::GetFirstCheckpoint() const
{
	if (!CurrentTrack)
		return nullptr;

	return CurrentTrack->GetFirstCheckpoint();
}

const FCheckpoint* UTrackManager::GetLastCheckpoint() const
{
	if (!CurrentTrack)
		return nullptr;

	return CurrentTrack->GetLastCheckpoint();
}

const FCheckpoint* UTrackManager::GetCheckpointAt(float SimulationTime) const
{
	if (!CurrentTrack)
		return nullptr;

	return CurrentTrack->GetCheckpointAt(SimulationTime);
}

const FCheckpoint* UTrackManager::GetCurrentCheckpoint() const
{
	if (!CurrentTrack)
		return nullptr;

	return CurrentTrack->GetCheckpointAt(CurrentCheckpointTime);
}

void UTrackManager::SetCurrentCheckpointTime(float SimulationTime)
{
	if (!CurrentTrack)
		return;

	if (const auto Checkpoint = CurrentTrack->GetCheckpointAt(SimulationTime))
		CurrentCheckpointTime = Checkpoint->Time;
}

void UTrackManager::SetCurrentTrack(unsigned int Id)
{
	if (!SimTrackGraph)
		return;

	if (const auto Track = SimTrackGraph->GetTrack(Id))
		CurrentTrack = Track;
}

USimTrack* UTrackManager::GetCurrentTrack() const
{
	return CurrentTrack;
}

void UTrackManager::CreateNewTrack(unsigned int Id)
{
	if (!CurrentTrack)
	{
		CurrentTrack = SimTrackGraph->CreateRoot(Id);
		return;
	}

	CurrentTrack = CurrentTrack->CreateChild(Id, CurrentCheckpointTime);
}

TArray<unsigned int> UTrackManager::GetParentIdsOfTrack(unsigned int Id) const
{
	if (!SimTrackGraph)
		return TArray<unsigned int>();

	return SimTrackGraph->GetParentIdsOfTrack(Id);
}

TArray<FSimTrackElementInfo> UTrackManager::GetSimTrackElementInfoList() const
{
	if (!SimTrackGraph)
		return TArray<FSimTrackElementInfo>();

	return SimTrackGraph->GetSimTrackElementInfoList();
}

TArray<FSimTrackEvaluationParams> UTrackManager::GetAllEvaluationParameters() const
{
	if (!SimTrackGraph)
		return TArray<FSimTrackEvaluationParams>();

	return SimTrackGraph->GetAllEvaluationParameters();
}

void UTrackManager::DeleteCurrentTrack()
{
	if (!SimTrackGraph)
		return;

	SimTrackGraph->DeleteTrack(CurrentTrack);
}