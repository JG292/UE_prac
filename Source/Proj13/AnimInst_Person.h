// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AnimInst_Person.generated.h"

/**
 * 
 */
UCLASS()
class PROJ13_API UAnimInst_Person : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly)
		class APerson *ThisPerson;

	UPROPERTY(BlueprintReadOnly)
		float Speed;

	UPROPERTY(BlueprintReadOnly)
		bool bInAir;

	UPROPERTY(BlueprintReadOnly)
		bool bAccel;

	UPROPERTY(BlueprintReadOnly)
		bool bEquipGun;

	UPROPERTY(BlueprintReadOnly)
		float VeloX;

	UPROPERTY(BlueprintReadOnly)
		float VeloY;
};
