// Copyright (c) 2025 Tobias Cutik

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SimTracks/Simulation/Core/Public/TrackManager.h"
#include "SimTracks/Simulation/Core/Public/Simulator.h"
#include "SimTracks/Simulation/UI/Public/SimTrackUI.h"
#include "SimTracks/Simulation/UI/Public/TrackArrowUI.h"
#include "SimTracks/Simulation/UI/Public/SimTrackElementManager.h"
#include "SimulationParamChangerUI.h"
#include "SimTracksUI.generated.h"

UCLASS(Abstract)
class SIMTRACKS_API USimTracksUI : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetTrackManager(UTrackManager* Manager);
	void SetSimulator(ASimulator* SimulatorPtr);
	void SetUserSimulation(UUserSimulation* Simulation);
	void InitializeUi();

	void UpdateCurrentSimTrackUi(float SimulationTime);
	void AddAnalysisPatchUi(float SimulationTime);
	UFUNCTION()
	void UpdateSimulationTimeDisplay(float SimulationTime);
	UFUNCTION()
	void UpdateCurrentEvaluationValueDisplays(float SimulationTime);

	UFUNCTION()
	void Play_PauseSimulation();
	UFUNCTION()
	void DeleteActiveTrack();

private:
	UFUNCTION()
	void SynchronizeCurrentCheckpointWithTrackSlider();
	UFUNCTION()
	void PreviewCurrentSimState(float SimulationTime) const;

	UFUNCTION()
	void StepCheckpointForward();
	UFUNCTION()
	void StepCheckpointBackward();

	UFUNCTION()
	void CreateNewTrack();
	void RebuildSimTracksBox();
	void RebuildHistoryBox() const;
	UFUNCTION()
	void UpdateValuesOfAnalysisPatches();
	void AddChildToSimTracksBox(UWidget* Child, float TopPadding, float LeftPadding);
	void AddEntryToHistoryBox(const USimTrackElement* SimTrackElement) const;

	UFUNCTION()
	void SwitchActiveTrack(unsigned int Id);

	static FString CreateTimeString(float Time);
	static FString CreateDigitPairString(int TimeValue);

	UPROPERTY(EditDefaultsOnly)
	unsigned int ScreenWidth = 1920;

	UPROPERTY(EditDefaultsOnly)
	float TimelineLengthInSeconds = 120.f;

	UPROPERTY(EditDefaultsOnly)
	float TrackVerticalSpacing = 5.f;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USimTrackUI> SimTrackUI;
	unsigned int NextTrackId = 1;
	unsigned int ActiveTrackId = 0;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UTrackArrowUI> TrackArrowUI;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UHistoryTrackUI> HistoryTrackUI;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UHistoryChangeUI> HistoryChangeUI;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UScrollBox* SimTracksBox;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UScrollBox* HistoryBox;

	UPROPERTY()
	UTrackManager* TrackManager = nullptr;
	UPROPERTY()
	ASimulator* Simulator = nullptr;
	UPROPERTY()
	UUserSimulation* UserSimulation = nullptr;
	UPROPERTY()
	USimTrackElementManager* SimTrackElementManager = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTrackSliderUI* TrackSlider;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class USimulationParamChangerUI* SimulationParamChanger;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UEvaluationParamSelectionUI* EvaluationParamSelection;

	// Buttons
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UIconButtonUI* Play_Pause;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UIconButtonUI* StepForwards;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UIconButtonUI* StepBackwards;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButtonUI* BranchOff;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* SimulationTimeText;
};