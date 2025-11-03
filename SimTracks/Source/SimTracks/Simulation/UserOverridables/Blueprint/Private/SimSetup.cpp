// Copyright (c) 2025 Tobias Cutik


#include "SimTracks/Simulation/UserOverridables/Blueprint/Public/SimSetup.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "SimTracks/Simulation/Input/Public/SimTracksController.h"

ASimSetup::ASimSetup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ASimSetup::BeginPlay()
{
	Super::BeginPlay();

	UserSimulation = NewObject<UUserSimulation>(this, UserSimulationToUse);
	UserSimulation->OnConstruction();
	const auto TrackManager = NewObject<UTrackManager>(this, UTrackManager::StaticClass());
	TrackManager->Initialize();
	Simulator->SetTrackManager(TrackManager);
	ConstructSimTracksWidget(TrackManager, UserSimulation);

	if (const auto Controller = Cast<ASimTracksController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
		Controller->SetupInput(SimTracksWrapperWidget->GetSimTracksUI());

	Simulator->UpdateSimTracksUi.AddUObject(SimTracksWrapperWidget->GetSimTracksUI(), &USimTracksUI::UpdateCurrentSimTrackUi);
	Simulator->UpdateSimTracksUi.AddUObject(SimTracksWrapperWidget->GetSimTracksUI(), &USimTracksUI::AddAnalysisPatchUi);

	Initialize();
	Simulator->SetUserSimulation(UserSimulation);
}

void ASimSetup::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ASimSetup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASimSetup::ConstructSimTracksWidget(UTrackManager* TrackManager, UUserSimulation* Simulation)
{
	const auto GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
	SimTracksWrapperWidget = CreateWidget<USimTracksUIWrapper>(GameInstance, SimulationUiToConstruct);
	SimTracksWrapperWidget->AddToPlayerScreen();
	SimTracksWrapperWidget->GetSimTracksUI()->SetTrackManager(TrackManager);
	SimTracksWrapperWidget->GetSimTracksUI()->SetSimulator(Simulator);
	SimTracksWrapperWidget->GetSimTracksUI()->SetUserSimulation(UserSimulation);
	SimTracksWrapperWidget->GetSimTracksUI()->InitializeUi();
}