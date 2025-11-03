// Copyright (c) 2025 Tobias Cutik


#include "SimTracks/EpidemicSimulation/Public/EpidemicSimulation.h"
#include "Kismet/KismetSystemLibrary.h"

UEpidemicSimulation::UEpidemicSimulation()
{
	AgentSpawnRandomStream.Initialize(1);
}

UEpidemicSimulation::~UEpidemicSimulation()
{
}

void UEpidemicSimulation::OnConstruction()
{
	SetupSimulationParams();
	SetupEvaluationParams();
}

void UEpidemicSimulation::SetupSimulationParams()
{
	SimulationParams.Add(FSimulationParameter(InfectionRateString, 0.f, 1.f, InfectionRate));
	SimulationParams.Add(FSimulationParameter(RecoveryTimeString, 0.f, 120.f, RecoveryTime));
	SimulationParams.Add(FSimulationParameter(AgentSpeedString, 0.f, 999.f, AgentSpeed));
}

void UEpidemicSimulation::SetupEvaluationParams()
{
	EvaluationParams.Add(FEvaluationParameter(BasicRValueString, 10.f, 0.f, 0.f));
	EvaluationParams.Add(FEvaluationParameter(BasicRValueStringAlt, 10.f, 0.f, 0.f));
	EvaluationParams.Add(FEvaluationParameter(EffectiveRValueString, 10.f, 0.f, 0.f));
	EvaluationParams.Add(FEvaluationParameter(InfectedPercentageString, 1.f, 0.f, 0.f));
}

void UEpidemicSimulation::SetSpawnArea(UStaticMeshComponent* Area)
{
	if (Area)
		SpawnArea = Area;
}

void UEpidemicSimulation::SpawnAgents()
{
	const FVector Location = FVector();
	const FRotator Rotation = FRotator();

    for (int i = 0; i < AgentsAmount; ++i)
		Agents.Add(GetWorld()->SpawnActor<ASimAgent>(SimAgentToSpawn, Location, Rotation));
}

void UEpidemicSimulation::InitializeAgentStates()
{
	AgentStates.Init(FAgentState(), Agents.Num());

	InitializeAgentStartLocations();
	InitializeAgentInfectionParameters();
	InfectFirstAgent();
}

void UEpidemicSimulation::InitializeAgentStartLocations()
{
	FVector Origin = FVector();
	FVector BoxExtent = FVector();
	float SphereRadius = 0.f;
	UKismetSystemLibrary::GetComponentBounds(SpawnArea, Origin, BoxExtent, SphereRadius);

	const FVector PossibleSpawnArea = FVector(BoxExtent.X - AgentSpawnSafetyMargin, BoxExtent.Y - AgentSpawnSafetyMargin, 0.f);

	for (int i = 0; i < Agents.Num(); ++i)
	{
		const float LocationX = AgentSpawnRandomStream.FRandRange(-PossibleSpawnArea.X, PossibleSpawnArea.X);
		const float LocationY = AgentSpawnRandomStream.FRandRange(-PossibleSpawnArea.Y, PossibleSpawnArea.Y);

		const FVector SpawnLocation = FVector(LocationX, LocationY, Origin.Z);

		AgentStates[i].Location = SpawnLocation;
	}
}

void UEpidemicSimulation::InitializeAgentInfectionParameters()
{
	for (auto& AgentState : AgentStates)
	{
		AgentState.InfectionRate = InfectionRate;
		AgentState.RecoveryTime = RecoveryTime;
	}
}

void UEpidemicSimulation::InfectFirstAgent()
{
	AgentStates[0].bInfected = true;
	AgentStates[0].CurrentTimeUntilRecovered = AgentStates[0].RecoveryTime;
}

void UEpidemicSimulation::SimulateAgents(const std::shared_ptr<FEpidemicSimState>& SimState, float DeltaTime)
{
	for (const auto Agent : Agents)
	{
		Agent->Simulate(SimState, DeltaTime);

		FVector Movement = Agent->AgentState.Movement;
		Movement *= DeltaTime;

		auto OldLocation = Agent->AgentState.Location;
		Agent->SetActorLocation(OldLocation + Movement, true);

		Agent->AgentState.Location = Agent->GetActorLocation();
	}

	SimState->TimeUntilAgentMovementChange -= DeltaTime;

	if (SimState->TimeUntilAgentMovementChange <= 0.f)
	{
		UpdateRandomMovement(SimState->AgentMovementRandomStream);
		SimState->TimeUntilAgentMovementChange = AgentMovementChangeInterval;
	}
}

void UEpidemicSimulation::Initialize()
{
	SpawnAgents();
	InitializeAgentStates();
}

void UEpidemicSimulation::ApplySimState(const std::shared_ptr<FSimState>& SimState)
{
	const auto EpidemicSimState = std::static_pointer_cast<FEpidemicSimState>(SimState);
	if (!EpidemicSimState)
		return;

	UpdateSimulationParameters(EpidemicSimState);

	for (int i = 0; i < Agents.Num(); ++i)
		Agents[i]->SetActorLocation(EpidemicSimState->AgentStates[i].Location, false);

	for (int i = 0; i < Agents.Num(); ++i)
	{
		Agents[i]->AgentState = EpidemicSimState->AgentStates[i];
		Agents[i]->UpdateMaterials();
	}
}

void UEpidemicSimulation::StartSimulation(const std::shared_ptr<FSimState>& SimState)
{
}

void UEpidemicSimulation::Simulate(const std::shared_ptr<FSimState>& SimState, float DeltaTime, float SimulationTime)
{
	const auto EpidemicSimState = std::static_pointer_cast<FEpidemicSimState>(SimState);
	if (!EpidemicSimState)
		return;

	if (EpidemicSimState->AgentStates.Num() != Agents.Num())
	{
		ensureMsgf(false, TEXT("EpidemicSimulation: EpidemicSimState->AgentStates.Num() != Agents.Num()"));
		return;
	}
		
	SimulateAgents(EpidemicSimState, DeltaTime);

	for (int i = 0; i < Agents.Num(); ++i)
		EpidemicSimState->AgentStates[i] = Agents[i]->AgentState;

	CalculateEvaluationParameters(EpidemicSimState, SimulationTime);
}

void UEpidemicSimulation::UpdateRandomMovement(const FRandomStream& RandomStream)
{
	for (auto Agent : Agents)
	{
		float XValue = RandomStream.FRandRange(-1, 1);
		float YValue = RandomStream.FRandRange(-1, 1);

		FVector NewMovement = FVector(XValue, YValue, 0.f);
		NewMovement.Normalize();

		NewMovement.X *= AgentSpeed;
		NewMovement.Y *= AgentSpeed;

		Agent->AgentState.Movement = NewMovement;
	}
}

std::shared_ptr<FSimState> UEpidemicSimulation::GetInitSimState()
{
	const auto EpidemicSimState = std::make_shared<FEpidemicSimState>(SimulationParams, EvaluationParams);
	EpidemicSimState->AgentStates = this->AgentStates;

	return EpidemicSimState;
}

std::shared_ptr<FSimState> UEpidemicSimulation::CopySimState(const std::shared_ptr<const FSimState>& SimState)
{
	const auto EpidemicSimState = std::static_pointer_cast<const FEpidemicSimState>(SimState);
	if (!EpidemicSimState)
		return nullptr;

	return std::make_shared<FEpidemicSimState>(*EpidemicSimState);
}

void UEpidemicSimulation::UpdateSimulationParameters(const std::shared_ptr<FEpidemicSimState>& SimState)
{
	const auto InfectionRateValue = SimState->SimulationParams.Find(InfectionRateString);
	const auto RecoveryTimeValue = SimState->SimulationParams.Find(RecoveryTimeString);

	for (auto& AgentState : SimState->AgentStates)
	{
		AgentState.InfectionRate = *InfectionRateValue;
		AgentState.RecoveryTime = *RecoveryTimeValue;
	}

	const auto AgentSpeedValue = SimState->SimulationParams.Find(AgentSpeedString);
	AgentSpeed = *AgentSpeedValue;
}

void UEpidemicSimulation::CalculateEvaluationParameters(const std::shared_ptr<FEpidemicSimState>& SimState, float SimulationTime)
{
	const float CurrentInfectionRate = *SimState->SimulationParams.Find(InfectionRateString);
	const float CurrentRecoveryTime = *SimState->SimulationParams.Find(RecoveryTimeString);

	float ContactsAmount = 0.f;
	float InfectedAmount = 0.f;
	float SusceptibleAmount = 0.f;

	for (const auto& AgentState : SimState->AgentStates)
	{
		ContactsAmount += AgentState.ContactsAmount;

		if (AgentState.bInfected)
			InfectedAmount++;
		else if (!AgentState.bInfected && !AgentState.bRecovered)
			SusceptibleAmount++;
	}

	const float AverageContactsAmount = ContactsAmount / AgentsAmount;
	const float InfectedPercentage = InfectedAmount / AgentsAmount;
	const float SusceptiblePercentage = SusceptibleAmount /  AgentsAmount;

	const auto BasicRValue = SimState->EvaluationParams.Find(BasicRValueString);
	const float NewBasicRValue = CurrentInfectionRate * (AverageContactsAmount / SimulationTime) * CurrentRecoveryTime;
	*BasicRValue = NewBasicRValue;

	const auto BasicRValueAlt = SimState->EvaluationParams.Find(BasicRValueStringAlt);
	const float NewBasicRValueAlt = ((AverageContactsAmount / SimulationTime) * CurrentInfectionRate) /*cB*/ / (1 / CurrentRecoveryTime) /* delta*/ * ((float)AgentsAmount / ((float)AgentsAmount + 1.f));
	*BasicRValueAlt = NewBasicRValueAlt;

	const auto EffectiveRValue = SimState->EvaluationParams.Find(EffectiveRValueString);
	const float NewEffectiveRValue = NewBasicRValue * SusceptiblePercentage;
	*EffectiveRValue = NewEffectiveRValue;

	*SimState->EvaluationParams.Find(InfectedPercentageString) = InfectedPercentage;
}