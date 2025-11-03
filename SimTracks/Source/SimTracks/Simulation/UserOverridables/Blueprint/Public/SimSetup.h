// Copyright (c) 2025 Tobias Cutik

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SimTracks/Simulation/UI/Public/SimTracksUI.h"
#include "SimTracks/Simulation/Core/Public/Simulator.h"
#include "SimTracks/Simulation/Core/Public/TrackManager.h"
#include "SimTracks/Simulation/UserOverridables/C++/Public/UserSimulation.h"
#include "SimTracks/Simulation/UserOverridables/Blueprint/Public/SimTracksUIWrapper.h"
#include "SimSetup.generated.h"

UCLASS()
class SIMTRACKS_API ASimSetup : public AActor
{
	GENERATED_BODY()
	
public:	
	ASimSetup();
	~ASimSetup() = default;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintImplementableEvent)
	void Initialize();

	UPROPERTY(BlueprintReadOnly)
	UUserSimulation* UserSimulation = nullptr;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	void ConstructSimTracksWidget(UTrackManager* TrackManager, UUserSimulation* Simulation);

	UPROPERTY(EditInstanceOnly)
	ASimulator* Simulator = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USimTracksUIWrapper> SimulationUiToConstruct;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserSimulation> UserSimulationToUse;

	UPROPERTY()
	USimTracksUIWrapper* SimTracksWrapperWidget = nullptr;
};