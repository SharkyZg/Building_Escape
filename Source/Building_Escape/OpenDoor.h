// Marko Sarkanj 2020

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BUILDING_ESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UOpenDoor();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
	float TotalMassOfActors() const;
	void FindAudioComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	void LogRotationAndYaw();
	float InitialYaw;
	float CurrentYaw;
	bool OpenDoorSoundPlayed = false;
	bool CloseDoorSoundPlayed = false;

	UPROPERTY(EditAnywhere)
	float TargetYaw = 90.f;

	UPROPERTY(EditAnywhere)
	ATriggerVolume *PressurePlate = nullptr;

	UPROPERTY(EditAnywhere)
	float RequiredMassToOpenDoors = 50.f;

	UPROPERTY()
	UAudioComponent* AudioComponent = nullptr;
};
