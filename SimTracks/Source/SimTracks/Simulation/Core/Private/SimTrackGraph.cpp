// Copyright (c) 2025 Tobias Cutik


#include "SimTracks/Simulation/Core/Public/SimTrackGraph.h"

USimTrack* USimTrackGraph::CreateRoot(unsigned int Id)
{
	if (!Root)
	{
		Root = NewObject<USimTrack>(this, USimTrack::StaticClass());
		Root->Id = Id;
	}

	return Root;
}

USimTrack* USimTrackGraph::GetTrack(unsigned int Id) const
{
	if (!Root)
		return nullptr;

	if (Root->Id == Id)
		return Root;

	return Root->FindTrackInChildren(Id);
}

TArray<unsigned int> USimTrackGraph::GetParentIdsOfTrack(unsigned int Id) const
{
	TArray<unsigned int> ParentIds;

	auto Track = GetTrack(Id);
	if (!Track)
		return ParentIds;

	while (const auto Parent = Track->GetParent())
	{
		ParentIds.Insert(Parent->Id, 0);
		Track = Parent;
	}

	return ParentIds;
}

TArray<FSimTrackElementInfo> USimTrackGraph::GetSimTrackElementInfoList() const
{
	TArray<FSimTrackElementInfo> InfoList;
	FSimTrackElementInfo SimTrackInfo;

	if (!Root)
		return InfoList;

	if (!Root->Checkpoints.IsEmpty())
		SimTrackInfo.EndTime = Root->Checkpoints.Last().Time;

	SimTrackInfo.TrackId = Root->Id;
	SimTrackInfo.StartTime = Root->StartTime;
	SimTrackInfo.ChildDepth = 0;

	InfoList.Add(SimTrackInfo);

	Root->AddTrackInfoOfChildren(InfoList);

	return InfoList;
}

TArray<FSimTrackEvaluationParams> USimTrackGraph::GetAllEvaluationParameters() const
{
	TArray<FSimTrackEvaluationParams> AllTracksEvaluationParams;
	FSimTrackEvaluationParams ThisTrackEvaluationParams;

	if (!Root)
		return AllTracksEvaluationParams;

	ThisTrackEvaluationParams.TrackId = Root->Id;
	for (const auto& Checkpoint : Root->Checkpoints)
		ThisTrackEvaluationParams.EvaluationParamsOfCheckpoints.Add(Checkpoint.Time, Checkpoint.SimState->EvaluationParams);

	AllTracksEvaluationParams.Add(ThisTrackEvaluationParams);

	Root->AddAllEvaluationParametersOfChildren(AllTracksEvaluationParams);

	return AllTracksEvaluationParams;
}

void USimTrackGraph::DeleteTrack(USimTrack* Track)
{
	if (!Track)
		return;

	const auto ParentTrack = Track->GetParent();
	if (!ParentTrack)
		return;

	USimTrack* ChildToDelete = nullptr;

	for (const auto Child : ParentTrack->Children)
	{
		if (Child->Id == Track->Id)
		{
			ChildToDelete = Child;
			break;
		}
	}

	ParentTrack->Children.Remove(ChildToDelete);
}