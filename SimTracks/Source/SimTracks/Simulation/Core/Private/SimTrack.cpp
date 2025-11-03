// Copyright (c) 2025 Tobias Cutik


#include "SimTracks/Simulation/Core/Public/SimTrack.h"

void USimTrack::AddCheckpoint(const std::shared_ptr<const FSimState>& SimState, float SimulationTime)
{
	if (!Checkpoints.IsEmpty() && SimulationTime <= Checkpoints.Last().Time)
		return;
	
	Checkpoints.Emplace(SimState, SimulationTime);
}

const FCheckpoint* USimTrack::GetCheckpointAt(float SimulationTime)
{
	if (Checkpoints.IsEmpty())
		return nullptr;

	for (const auto& Checkpoint : Checkpoints)
	{
		if (FMath::IsNearlyEqual(Checkpoint.Time, SimulationTime, KINDA_SMALL_NUMBER))
			return &Checkpoint; 
	}

	return nullptr;
}

const FCheckpoint* USimTrack::GetFirstCheckpoint()
{
	if (Checkpoints.IsEmpty())
		return nullptr;

	return &Checkpoints[0];
}

const FCheckpoint* USimTrack::GetLastCheckpoint()
{
	if (Checkpoints.IsEmpty())
		return nullptr;

	return &Checkpoints.Last();
}

USimTrack* USimTrack::GetParent()
{
	return Parent;
}

USimTrack* USimTrack::CreateChild(unsigned int Number, float SimulationTime)
{
	const int Index = Children.Add(NewObject<USimTrack>(this, USimTrack::StaticClass()));
	const auto Child = Children[Index];

	Child->Parent = this;
	Child->Id = Number;
	Child->StartTime = SimulationTime;

	Algo::Sort(Children, [](const USimTrack* A, const USimTrack* B)
	{
		return A->StartTime > B->StartTime;
	});

	return Child;
}

USimTrack* USimTrack::FindTrackInChildren(unsigned int Number) const
{
	for (const auto ChildTrack : Children)
	{
		if (ChildTrack->Id == Number)
			return ChildTrack;

		if (const auto Track = ChildTrack->FindTrackInChildren(Number))
			return Track;
	}

	return nullptr;
}

void USimTrack::AddTrackInfoOfChildren(TArray<FSimTrackElementInfo>& InfoList)
{
	// child tracks are always sorted after descending start times (enforced in USimTrack::CreateChild)
	// the child track with the highest starting time gets drawn as the first child track of this SimTrack in the UI
	// so add starting times in descending order

	const int ThisSimTrackIndex = InfoList.Num() - 1;

	for (const auto Child : Children)
	{
		FSimTrackElementInfo TrackParams;

		if (!Child->Checkpoints.IsEmpty())
			TrackParams.EndTime = Child->Checkpoints.Last().Time;

		TrackParams.TrackId = Child->Id;
		TrackParams.StartTime = Child->StartTime;
		TrackParams.ChildDepth = InfoList.Num() - 1 - ThisSimTrackIndex;

		InfoList.Add(TrackParams);

		Child->AddTrackInfoOfChildren(InfoList);
	}
}

void USimTrack::AddAllEvaluationParametersOfChildren(TArray<FSimTrackEvaluationParams>& AllTracksEvaluationParams)
{
	// use same child track order as in USimTrack::AddTrackInfoOfChildren

	for (const auto Child : Children)
	{
		FSimTrackEvaluationParams ThisTrackEvaluationParams;

		ThisTrackEvaluationParams.TrackId = Child->Id;
		for (const auto& Checkpoint : Child->Checkpoints)
			ThisTrackEvaluationParams.EvaluationParamsOfCheckpoints.Add(Checkpoint.Time, Checkpoint.SimState->EvaluationParams);

		AllTracksEvaluationParams.Add(ThisTrackEvaluationParams);

		Child->AddAllEvaluationParametersOfChildren(AllTracksEvaluationParams);
	}
}