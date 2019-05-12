// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "projectlevelCharacter.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpadateInventoryDelegate, const TArray<class APickableItem*>&, UI_itemslot);

UENUM(BlueprintType)
enum class ESKillTypeEnum : uint8
{
	TYPE_SKILL1 		UMETA(DisplayName = "SKILL1"),
	TYPE_SKILL2		UMETA(DisplayName = "SKILL2"),
	TYPE_SKILL3		UMETA(DisplayName = "SKILL3"),
	TYPE_SKILL4		UMETA(DisplayName = "SKILL4")
};

UCLASS(config=Game)
class AprojectlevelCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class ASkillEffect> SkillEffect;

public:
	AprojectlevelCharacter();
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Skill)
		ESKillTypeEnum SkillType;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class AMainWeapon> Spawn;

	UPROPERTY(EditAnywhere)
		TArray<class APickableItem*> Inventory;
	void AddToInventory(class APickableItem* item);

protected:


	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);
	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	void BeginPlay();

	void ToggleSkill();

	void Attack();

	void Skill();

	void Dash();

	void Walk();

	
	//Open Inventory
	UFUNCTION(BlueprintCallable)
	void OpenInventory();
	//Get 


	//Send Item infomation to UI_itemslot
	UPROPERTY(BlueprintAssignable, Category = Pickup)
		FUpadateInventoryDelegate OnUpdateInventory;
	//Character's weapon actor
	class AMainWeapon* MainWeapon;

	//set dash/walk mode 
	bool onDash = false;

	void Tick(float DeltaTime);

	//Animation 
	class UAnimSequence* SkillAnim;
	class UAnimSequence* IdleAnim;

	
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector SkillOffset;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }	

	
private:
	UPROPERTY()
	class AUIPlayerController* UIPlayerController;

	//server
public:
	int SessionId;
	// 세션아이디에 매칭되는 액터 반환
	AActor* FindActorBySessionId(TArray<AActor*> ActorArray, const int& SessionId);

	// 스폰시킬 다른 캐릭터
	UPROPERTY(EditAnywhere, Category = "Spawning")
		TSubclassOf<ACharacter> WhoToSpawn;
	

};


