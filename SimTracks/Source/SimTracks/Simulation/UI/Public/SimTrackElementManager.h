// Copyright (c) 2025 Tobias Cutik

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SimTracks/Simulation/UI/Public/SimTrackElement.h"
#include "SimTrackElementManager.generated.h"

UCLASS()
class SIMTRACKS_API USimTrackElementManager : public UObject
{
	GENERATED_BODY()

public:
    USimTrackElementManager() = default;

	USimTrackElement* AddNewElement(UWidget* Owner, TSubclassOf<USimTrackUI> SimTrackUIClass, TSubclassOf<UTrackArrowUI> TrackArrowUIClass, 
		TSubclassOf<UHistoryTrackUI> HistoryTrackUIClass, unsigned int Id, const FString& Name);
	USimTrackElement* GetElement(unsigned int Id) const;
	void AddAnalysisPatchToActiveTrack(float SimulationTime, float PerformanceValue, float ColorValue, float CheckpointStepSize, float TimelineLengthInSeconds, unsigned int ScreenWidth) const;

	void UpdateActiveSimTrackWidgetWidth(float SimulationTime, float TimelineLengthInSeconds, unsigned int ScreenWidth) const;
	void SetAllSimTracksVisuallyInactive();
	void SetActiveSimTrackElement(unsigned int Id);
	void SetSimTracksVisuallyActive(TArray<unsigned int> Ids);
	void UpdateAnalysisPatchSizes(float CheckpointStepSize, float TimelineLengthInSeconds, unsigned int ScreenWidth);
	void ShowEvaluationValuesAt(float SimulationTime, float TimelineLengthInSeconds, unsigned int ScreenWidth);

	void DeleteActiveSimTrackElement();

private:
	UPROPERTY()
	TArray<USimTrackElement*> SimTrackElements;

	UPROPERTY()
	USimTrackElement* ActiveSimTrackElement = nullptr;
};