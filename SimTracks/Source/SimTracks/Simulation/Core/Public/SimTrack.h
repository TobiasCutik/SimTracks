// Copyright (c) 2025 Tobias Cutik

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SimTracks/Simulation/Core/Public/Checkpoint.h"
#include "SimTracks/Simulation/UI/Public/SimTrackElementInfo.h"
#include "SimTracks/Simulation/UI/Public/SimTrackEvaluationParams.h"
#include "SimTrack.generated.h"

UCLASS()
class SIMTRACKS_API USimTrack : public UObject
{
	GENERATED_BODY()
	friend class USimTrackGraph;
	
public:	
	USimTrack() = default;
	void AddCheckpoint(const std::shared_ptr<const FSimState>& SimState, float SimulationTime);
	const FCheckpoint* GetCheckpointAt(float SimulationTime);
	const FCheckpoint* GetFirstCheckpoint();
	const FCheckpoint* GetLastCheckpoint();

	USimTrack* GetParent();
	USimTrack* CreateChild(unsigned int Number, float SimulationTime);

	USimTrack* FindTrackInChildren(unsigned int Number) const;
	void AddTrackInfoOfChildren(TArray<FSimTrackElementInfo>& InfoList);
	void AddAllEvaluationParametersOfChildren(TArray<FSimTrackEvaluationParams>& AllTracksEvaluationParams);

private:
	UPROPERTY()
	USimTrack* Parent = nullptr;
	UPROPERTY()
	TArray<FCheckpoint> Checkpoints;
	UPROPERTY()
	TArray<USimTrack*> Children;

	unsigned int Id = 0;
	float StartTime = 0.f;
};