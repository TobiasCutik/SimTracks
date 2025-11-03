// Copyright (c) 2025 Tobias Cutik

#pragma once

#include "CoreMinimal.h"
#include "ButtonUI.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"
#include "Math/Color.h"
#include "SimTracks/Simulation/UI/Public/AnalysisPatchUI.h"
#include "Components/HorizontalBoxSlot.h"
#include "SimTrackUI.generated.h"

UCLASS(Abstract)
class SIMTRACKS_API USimTrackUI : public UButtonUI
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;

	void SetWidth(float Width);
	float GetWidth() const;
	void SetName(FText Name);
	float GetHeight() const;
	void SetActiveVisually(bool bActive);
	void AddAnalysisPatch(float SimulationTime, float PerformanceValue, float ColorValue, float WidthInPixel);
	void UpdateAnalysisPatchSizes(float WidthInPixel);
	void UpdateAnalysisPatchValues(float SimulationTime, float PerformanceValue, float ColorValue);
	void ShowEvaluationValueAt(float SimulationTime, float TimelineLengthInSeconds, unsigned int ScreenWidth);

	float LeftPadding = 0.f;

private:
	bool bActiveVisually = false;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class USizeBox* SizeBox;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* TrackName;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UHorizontalBox* AnalysisBox;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* EvaluationValue;
	UPROPERTY()
	UHorizontalBoxSlot* EvaluationValueSlot = nullptr;

	UPROPERTY(EditDefaultsOnly)
	FLinearColor DefaultBackgroundColor;
	UPROPERTY(EditDefaultsOnly)
	FLinearColor ActiveBackgroundColor;

	UPROPERTY(EditDefaultsOnly)
	FLinearColor BestPerformanceColor;
	UPROPERTY(EditDefaultsOnly)
	FLinearColor WorstPerformanceColor;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAnalysisPatchUI> AnalysisPatchUI;

	UPROPERTY()
	TArray<UAnalysisPatchUI*> AnalysisPatches;
};