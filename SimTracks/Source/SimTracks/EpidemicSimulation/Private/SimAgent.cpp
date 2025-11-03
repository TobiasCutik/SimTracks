// Copyright (c) 2025 Tobias Cutik


#include "SimTracks/EpidemicSimulation/Public/SimAgent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Math/RandomStream.h"

ASimAgent::ASimAgent()
{
	PrimaryActorTick.bCanEverTick = true;

	AgentState.InfectionRandomStream.Initialize(1);
}

void ASimAgent::InfectAgent()
{
	if (AgentState.bRecovered || AgentState.bInfected)
		return;

	AgentState.bInfected = true;
	SetMaterial(InfectedMaterial);

	AgentState.CurrentTimeUntilRecovered = AgentState.RecoveryTime;
}

void ASimAgent::UpdateMaterials()
{
	if (AgentState.bInfected)
		SetMaterial(InfectedMaterial);
	else if (AgentState.bRecovered)
		SetMaterial(RecoveredMaterial);
	else
		SetMaterial(SusceptibleMaterial);
}

void ASimAgent::Simulate(const std::shared_ptr<FEpidemicSimState>& SimState, float DeltaTime)
{
	if (!AgentState.bInfected)
		return;

	if (AgentState.CurrentTimeUntilRecovered > 0.f)
		AgentState.CurrentTimeUntilRecovered -= DeltaTime;
	else
		RecoverAgent();
}

void ASimAgent::RecoverAgent()
{
	AgentState.bRecovered = true;
	AgentState.bInfected = false;

	SetMaterial(RecoveredMaterial);
}

void ASimAgent::SetMaterial(UMaterialInterface* Material)
{
	if (StaticMeshComponent)
		StaticMeshComponent->SetMaterial(0, Material);
}

void ASimAgent::OnCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (const auto OtherSimAgent = Cast<ASimAgent>(OtherActor))
	{
		AgentState.ContactsAmount++;

		if (!OtherSimAgent->AgentState.bInfected)
			return;

		const float InfectionPossibility = AgentState.InfectionRandomStream.FRandRange(0, 1);

		if (InfectionPossibility <= AgentState.InfectionRate)
			InfectAgent();
	}
}

void ASimAgent::BeginPlay()
{
	Super::BeginPlay();

	if (const auto SphereComponent = FindComponentByClass<USphereComponent>())
		SphereCollisionComponent = SphereComponent;

	if (const auto MeshComponent = FindComponentByClass<UStaticMeshComponent>())
		StaticMeshComponent = MeshComponent;

	if (SphereCollisionComponent)
		SphereCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ASimAgent::OnCollision);

	SetMaterial(SusceptibleMaterial);
}

void ASimAgent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (SphereCollisionComponent)
		SphereCollisionComponent->OnComponentBeginOverlap.RemoveDynamic(this, &ASimAgent::OnCollision);
}