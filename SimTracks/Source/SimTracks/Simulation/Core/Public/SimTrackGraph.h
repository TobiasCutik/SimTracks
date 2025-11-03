// Copyright (c) 2025 Tobias Cutik

#pragma once

#include "CoreMinimal.h"
#include "SimTracks/Simulation/Core/Public/SimTrack.h"
#include "SimTrackGraph.generated.h"

UCLASS()
class SIMTRACKS_API USimTrackGraph : public UObject
{
	GENERATED_BODY()
	
public:	
	USimTrackGraph() = default;
	USimTrack* CreateRoot(unsigned int Id);
	USimTrack* GetTrack(unsigned int Id) const;
	TArray<unsigned int> GetParentIdsOfTrack(unsigned int Id) const;
	TArray<FSimTrackElementInfo> GetSimTrackElementInfoList() const;
	TArray<FSimTrackEvaluationParams> GetAllEvaluationParameters() const;

	void DeleteTrack(USimTrack* Track);

private:
	UPROPERTY()
	USimTrack* Root = nullptr;
};