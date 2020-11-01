// Marko Sarkanj 2020

#include "OpenDoor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Components/PrimitiveComponent.h"
#include "Components/AudioComponent.h"

#include "GameFramework/Actor.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	TargetYaw += InitialYaw;

	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("Pressure plate not found on object %s containing OpenDoor component."), *GetOwner()->GetName());
	}

	FindAudioComponent();
	OpenDoorSoundPlayed = false;
	CloseDoorSoundPlayed = true;
}

void UOpenDoor::FindAudioComponent()
{
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();

	if (!AudioComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing audio component"), *GetOwner()->GetName());
	}
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (TotalMassOfActors() > RequiredMassToOpenDoors)
	{
		OpenDoor(DeltaTime);
	}
	else
	{
		CloseDoor(DeltaTime);
	}
}

void UOpenDoor::OpenDoor(float DeltaTime)
{

	// LogRotationAndYaw();

	CurrentYaw = FMath::FInterpTo(CurrentYaw, TargetYaw, DeltaTime, 1);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);

	if (!OpenDoorSoundPlayed && AudioComponent)
	{
		AudioComponent->Play();
		OpenDoorSoundPlayed = true;
		CloseDoorSoundPlayed = false;
	}
}

void UOpenDoor::CloseDoor(float DeltaTime)
{

	// LogRotationAndYaw();

	CurrentYaw = FMath::FInterpTo(CurrentYaw, InitialYaw, DeltaTime, 1);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);

	if (!CloseDoorSoundPlayed && AudioComponent)
	{
		AudioComponent->Play();
		CloseDoorSoundPlayed = true;
		OpenDoorSoundPlayed = false;
	}
}

void UOpenDoor::LogRotationAndYaw()
{
	UE_LOG(LogTemp, Warning, TEXT(" Actor rotation is %s"), *GetOwner()->GetActorRotation().ToString());
	UE_LOG(LogTemp, Warning, TEXT(" Yaw is %f"), GetOwner()->GetActorRotation().Yaw);
}

float UOpenDoor::TotalMassOfActors() const
{
	float TotalMass = 0.f;

	TArray<AActor *> OverlappingActors;

	if (!PressurePlate)
	{
		return TotalMass;
	}
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	for (const AActor *Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("%s is on the pressureplate"), *Actor->GetName());
		UE_LOG(LogTemp, Warning, TEXT("%s mass is %f"), *Actor->GetName(), Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass());
	}

	return TotalMass;
}