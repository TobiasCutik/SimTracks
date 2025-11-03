// Copyright (c) 2025 Tobias Cutik

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SimTracks/Simulation/Core/Public/SimTrack.h"
#include "SimTracks/Simulation/Core/Public/SimTrackGraph.h"
#include "TrackManager.generated.h"

UCLASS()
class SIMTRACKS_API UTrackManager : public UObject
{
	GENERATED_BODY()
	
public:	
	UTrackManager() = default;
	void Initialize();

	void SaveCheckpoint(const std::shared_ptr<const FSimState>& SimState, float SimulationTime);
	const FCheckpoint* GetFirstCheckpoint() const;
	const FCheckpoint* GetLastCheckpoint() const;
	const FCheckpoint* GetCheckpointAt(float SimulationTime) const;
	const FCheckpoint* GetCurrentCheckpoint() const;

	void SetCurrentCheckpointTime(float SimulationTime);

	void SetCurrentTrack(unsigned int Id);
	USimTrack* GetCurrentTrack() const;
	void CreateNewTrack(unsigned int Id);

	TArray<unsigned int> GetParentIdsOfTrack(unsigned int Id) const;
	TArray<FSimTrackElementInfo> GetSimTrackElementInfoList() const;
	// return evaluation parameters for each checkpoint of each track
	TArray<FSimTrackEvaluationParams> GetAllEvaluationParameters() const;

	void DeleteCurrentTrack();

private:
	float CurrentCheckpointTime = 0.f;

	UPROPERTY()
	USimTrack* CurrentTrack = nullptr;
	UPROPERTY()
	USimTrackGraph* SimTrackGraph = nullptr;
};