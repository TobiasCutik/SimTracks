// Copyright (c) 2025 Tobias Cutik

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "SimTracks/EpidemicSimulation/Public/EpidemicSimState.h"
#include "SimTracks/EpidemicSimulation/Public/AgentState.h"
#include "SimAgent.generated.h"

UCLASS()
class SIMTRACKS_API ASimAgent : public AActor
{
	GENERATED_BODY()

public:
	ASimAgent();

	void InfectAgent();
	void UpdateMaterials();
	void Simulate(const std::shared_ptr<FEpidemicSimState>& SimState, float DeltaTime);

	FAgentState AgentState = FAgentState();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UPROPERTY(EditDefaultsOnly)
	UMaterialInterface* SusceptibleMaterial;

	UPROPERTY(EditDefaultsOnly)
	UMaterialInterface* InfectedMaterial;

	UPROPERTY(EditDefaultsOnly)
	UMaterialInterface* RecoveredMaterial;

	void SetMaterial(UMaterialInterface* Material);

	UFUNCTION()
	void OnCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void RecoverAgent();

	UPROPERTY()
	USphereComponent* SphereCollisionComponent = nullptr;
	UPROPERTY()
	UStaticMeshComponent* StaticMeshComponent = nullptr;
};