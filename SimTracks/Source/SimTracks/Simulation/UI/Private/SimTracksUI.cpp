// Copyright (c) 2025 Tobias Cutik


#include "SimTracks/Simulation/UI/Public/SimTracksUI.h"
#include "SimTracks/Simulation/UI/Public/TrackSliderUI.h"
#include "Components/ScrollBox.h"
#include "Components/ScrollBoxSlot.h"
#include "SimTracks/Simulation/UI/Public/EvaluationParamSelectionUI.h"
#include "SimTracks/Simulation/UI/Public/SimTrackElementManager.h"
#include "SimTracks/Simulation/UI/Public/IconButtonUI.h"
#include "GenericPlatform/GenericPlatformMath.h"

void USimTracksUI::SetTrackManager(UTrackManager* Manager)
{
	TrackManager = Manager;
}

void USimTracksUI::SetSimulator(ASimulator* SimulatorPtr)
{
	Simulator = SimulatorPtr;
}

void USimTracksUI::SetUserSimulation(UUserSimulation* Simulation)
{
	UserSimulation = Simulation;
}

void USimTracksUI::InitializeUi()
{
	if (!UserSimulation)
		return;

	// setup event bindings
	Play_Pause->GetOnClickedEvent().AddDynamic(this, &USimTracksUI::Play_PauseSimulation);
	StepForwards->GetOnClickedEvent().AddDynamic(this, &USimTracksUI::StepCheckpointForward);
	StepBackwards->GetOnClickedEvent().AddDynamic(this, &USimTracksUI::StepCheckpointBackward);
	BranchOff->GetOnClickedEvent().AddDynamic(this, &USimTracksUI::CreateNewTrack);

	SimTrackElementManager = NewObject<USimTrackElementManager>(this, USimTrackElementManager::StaticClass());
	UpdateSimulationTimeDisplay(0.f);
	TrackSlider->SetMaxValue(TimelineLengthInSeconds);
	TrackSlider->SetupMouseDraggingSteps(UserSimulation->GetTimeStepSize() * UserSimulation->GetCheckpointIntervalsInSteps());
	TrackSlider->SetScreenWidth(ScreenWidth);
	TrackSlider->GetOnMouseCaptureEnd().AddDynamic(this, &USimTracksUI::SynchronizeCurrentCheckpointWithTrackSlider);
	TrackSlider->ValueChanged.AddUObject(this, &USimTracksUI::UpdateSimulationTimeDisplay);
	TrackSlider->ValueChanged.AddUObject(this, &USimTracksUI::UpdateCurrentEvaluationValueDisplays);
	TrackSlider->ValueChanged.AddUObject(this, &USimTracksUI::PreviewCurrentSimState);

	auto SimulationParams = UserSimulation->GetSimulationParams();
	SimulationParamChanger->Init(SimulationParams);

	auto EvaluationParams = UserSimulation->GetEvaluationParams();
	EvaluationParamSelection->Init(EvaluationParams);
	EvaluationParamSelection->NewEvaluationParamSelected.AddUObject(this, &USimTracksUI::UpdateValuesOfAnalysisPatches);
}

void USimTracksUI::UpdateCurrentSimTrackUi(float SimulationTime)
{
	TrackSlider->SetValue(SimulationTime);
	SimTrackElementManager->UpdateActiveSimTrackWidgetWidth(SimulationTime, TimelineLengthInSeconds, ScreenWidth);
}

void USimTracksUI::AddAnalysisPatchUi(float SimulationTime)
{
	if (const auto CurrentCheckpoint = TrackManager->GetCurrentCheckpoint())
	{
		const float PerformanceValue = EvaluationParamSelection->GetSelectedEvaluationParamPerformance(CurrentCheckpoint->SimState->EvaluationParams);
		const float ColorValue = EvaluationParamSelection->GetSelectedEvaluationParamColorValue(CurrentCheckpoint->SimState->EvaluationParams);
		const float CheckpointStepSize = UserSimulation->GetTimeStepSize() * UserSimulation->GetCheckpointIntervalsInSteps();
		SimTrackElementManager->AddAnalysisPatchToActiveTrack(SimulationTime, PerformanceValue, ColorValue, CheckpointStepSize, TimelineLengthInSeconds, ScreenWidth);
	}
}

void USimTracksUI::UpdateSimulationTimeDisplay(float SimulationTime)
{
	const auto TimeString = CreateTimeString(SimulationTime);
	SimulationTimeText->SetText(FText::FromString(TimeString));
}

void USimTracksUI::UpdateCurrentEvaluationValueDisplays(float SimulationTime)
{
	SimTrackElementManager->ShowEvaluationValuesAt(SimulationTime, TimelineLengthInSeconds, ScreenWidth);
}

void USimTracksUI::CreateNewTrack()
{
	if (Simulator->IsSimulating())
		return;

	if (!Simulator || !SimTrackElementManager)
		return;

	SynchronizeCurrentCheckpointWithTrackSlider();

	// start first track at cursor position and every child track thereafter one checkpoint step behind as each child track starts with the last checkpoint of its parent
	float TrackStartTime = TrackSlider->GetValue();
	if (NextTrackId > 1) 
		TrackStartTime -= UserSimulation->GetTimeStepSize() * UserSimulation->GetCheckpointIntervalsInSteps();

	FString TrackName = "T";
	TrackName.AppendInt(NextTrackId);
	const auto SimTrackElement = SimTrackElementManager->AddNewElement(this, SimTrackUI, TrackArrowUI, HistoryTrackUI, NextTrackId, TrackName);
	SimTrackElement->CalculatePadding(TrackStartTime, TimelineLengthInSeconds, ScreenWidth);
	SimTrackElement->TrackClicked.AddUObject(this, &USimTracksUI::SwitchActiveTrack);

	// create no HistoryChangeUI for first track as it has no changes
	if (NextTrackId > 1)
		SimTrackElement->CreateHistoryChangeUI(this, HistoryChangeUI, SimulationParamChanger->GetCurrentSimParamName(), SimulationParamChanger->GetCurrentValueInput(), CreateTimeString(TrackStartTime));

	Simulator->CreateNewTrack(SimulationParamChanger->GetCurrentSimParamName(), SimulationParamChanger->GetCurrentValueInput(), NextTrackId);
	AddEntryToHistoryBox(SimTrackElement);

	RebuildSimTracksBox();

	ActiveTrackId = NextTrackId;
	NextTrackId++;
}

void USimTracksUI::RebuildSimTracksBox()
{
	if (!TrackManager || !SimTrackElementManager)
		return;

	SimTracksBox->ClearChildren();

	auto SimTrackInfoList = TrackManager->GetSimTrackElementInfoList();
	for (const auto SimTrackInfo : SimTrackInfoList)
	{
		const auto SimTrackElement = SimTrackElementManager->GetElement(SimTrackInfo.TrackId);
		if (!SimTrackElement)
			continue;

		const auto SimTrackWidget = SimTrackElement->GetSimTrackWidget();
		AddChildToSimTracksBox(SimTrackWidget, TrackVerticalSpacing, SimTrackWidget->LeftPadding);

		const auto TrackArrowWidget = SimTrackElement->GetTrackArrowWidget();
		const float NewArrowLineHeight = SimTrackInfo.ChildDepth * (SimTrackWidget->GetHeight() + TrackVerticalSpacing);
		const float TrackArrowTopPadding = -TrackArrowWidget->GetArrowHeadImageSize().Y - NewArrowLineHeight;
		AddChildToSimTracksBox(TrackArrowWidget, TrackArrowTopPadding, TrackArrowWidget->LeftPadding);
		TrackArrowWidget->SetArrowLineHeight(NewArrowLineHeight);
	}

	SimTrackElementManager->UpdateAnalysisPatchSizes(UserSimulation->GetTimeStepSize() * UserSimulation->GetCheckpointIntervalsInSteps(), TimelineLengthInSeconds, ScreenWidth);
}

void USimTracksUI::RebuildHistoryBox() const
{
	HistoryBox->ClearChildren();

	auto ParentIds = TrackManager->GetParentIdsOfTrack(ActiveTrackId);

	for (const auto ParentId : ParentIds)
		AddEntryToHistoryBox(SimTrackElementManager->GetElement(ParentId));

	AddEntryToHistoryBox(SimTrackElementManager->GetElement(ActiveTrackId));
}

void USimTracksUI::UpdateValuesOfAnalysisPatches()
{
	auto EvaluationParamsOfAllTracks = TrackManager->GetAllEvaluationParameters();

	for (const auto& EvaluationParamsOfTrack : EvaluationParamsOfAllTracks)
	{
		const auto SimTrackElement = SimTrackElementManager->GetElement(EvaluationParamsOfTrack.TrackId);
		if (!SimTrackElement)
			continue;

		for (const auto& EvaluationParamsOfCheckpoint : EvaluationParamsOfTrack.EvaluationParamsOfCheckpoints)
		{
			const float PerformanceValue = EvaluationParamSelection->GetSelectedEvaluationParamPerformance(EvaluationParamsOfCheckpoint.Value);
			const float ColorValue = EvaluationParamSelection->GetSelectedEvaluationParamColorValue(EvaluationParamsOfCheckpoint.Value);
			SimTrackElement->GetSimTrackWidget()->UpdateAnalysisPatchValues(EvaluationParamsOfCheckpoint.Key, PerformanceValue, ColorValue);
		}
	}
}

void USimTracksUI::SwitchActiveTrack(unsigned int Id)
{
	if (Simulator->IsSimulating())
		return;

	SimTrackElementManager->SetAllSimTracksVisuallyInactive();
	SimTrackElementManager->SetActiveSimTrackElement(Id);
	SimTrackElementManager->SetSimTracksVisuallyActive(TrackManager->GetParentIdsOfTrack(Id));

	TrackManager->SetCurrentTrack(Id);
	SynchronizeCurrentCheckpointWithTrackSlider();

	ActiveTrackId = Id;
	RebuildHistoryBox();
}

void USimTracksUI::Play_PauseSimulation()
{
	if (!Simulator || !TrackManager)
		return;

	if (SimTracksBox->GetAllChildren().IsEmpty())
		CreateNewTrack();

	Play_Pause->SwitchActiveTexture();

	SynchronizeCurrentCheckpointWithTrackSlider();

	if (Simulator->IsSimulating())
		Simulator->PauseSimulation();
	else
		Simulator->StartSimulation();
}

void USimTracksUI::DeleteActiveTrack()
{
	if (Simulator->IsSimulating())
		return;

	// do not delete the root SimTrack
	if (ActiveTrackId == 1)
		return;

	unsigned int ParentId = 1;
	const auto ParentIds = TrackManager->GetParentIdsOfTrack(ActiveTrackId);
	if (!ParentIds.IsEmpty())
		ParentId = ParentIds.Last();

	TrackManager->DeleteCurrentTrack();
	SimTrackElementManager->DeleteActiveSimTrackElement();
	RebuildSimTracksBox();

	SwitchActiveTrack(ParentId);
}

void USimTracksUI::SynchronizeCurrentCheckpointWithTrackSlider()
{
	if (!Simulator || !TrackManager)
		return;

	float Time = 0.f;

	const auto FirstCheckpoint = TrackManager->GetFirstCheckpoint();
	const auto LastCheckpoint = TrackManager->GetLastCheckpoint();
	
	if (!FirstCheckpoint || !LastCheckpoint)
		return;

	if (FirstCheckpoint->Time > TrackSlider->GetValue())
		Time = FirstCheckpoint->Time;
	else if (LastCheckpoint->Time < TrackSlider->GetValue())
		Time = LastCheckpoint->Time;
	else
		Time = TrackSlider->GetValue();

	UpdateCurrentSimTrackUi(Time);
	UpdateSimulationTimeDisplay(Time);
	PreviewCurrentSimState(Time);
}

void USimTracksUI::PreviewCurrentSimState(float SimulationTime) const
{
	TrackManager->SetCurrentCheckpointTime(SimulationTime);
	Simulator->PreviewCurrentSimState();
}

void USimTracksUI::StepCheckpointForward()
{
	TrackSlider->StepCheckpointForward();
}

void USimTracksUI::StepCheckpointBackward()
{
	TrackSlider->StepCheckpointBackward();
}

void USimTracksUI::AddChildToSimTracksBox(UWidget* Child, float TopPadding, float LeftPadding)
{
	const auto PanelSlot = SimTracksBox->AddChild(Child);
	if (const auto ScrollBoxSlot = Cast<UScrollBoxSlot>(PanelSlot))
	{
		ScrollBoxSlot->SetHorizontalAlignment(HAlign_Left);

		FMargin Margin = FMargin();
		Margin.Top = TopPadding;
		Margin.Left = LeftPadding;
		ScrollBoxSlot->SetPadding(Margin);
	}
}

void USimTracksUI::AddEntryToHistoryBox(const USimTrackElement* SimTrackElement) const
{
	if (!SimTrackElement)
		return;

	HistoryBox->AddChild(SimTrackElement->GetHistoryChangeWidget());
	HistoryBox->AddChild(SimTrackElement->GetHistoryTrackWidget());
}

FString USimTracksUI::CreateTimeString(float Time)
{
	const int Minutes = Time / 60.f;
	const int Seconds = FGenericPlatformMath::Fmod(Time, 60.f);
	const int Centiseconds = FGenericPlatformMath::Fmod(Time, 1.f) * 100.f;

	return CreateDigitPairString(Minutes) + ":" + CreateDigitPairString(Seconds) + "." + CreateDigitPairString(Centiseconds);
}

FString USimTracksUI::CreateDigitPairString(int TimeValue)
{
	FString TimeString = FString();

	if (TimeValue < 10)
		TimeString += "0";

	TimeString.AppendInt(TimeValue);

	return TimeString;
}