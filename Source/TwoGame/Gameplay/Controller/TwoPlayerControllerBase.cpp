// Copyright (c) 2025 鸿源z. All rights reserved.


#include "TwoPlayerControllerBase.h"

#include "EnhancedInputSubsystems.h"
#include "TwoGame/AbilitySystem/TwoAbilitySystemBlueprintLibrary.h"
#include "TwoGame/Character/Player/TwoCheatManager.h"
#include "TwoGame/Input/TwoEnhancedInputComponent.h"

ATwoPlayerControllerBase::ATwoPlayerControllerBase()
{
	bReplicates = true; //启动复制

	CheatClass = UTwoCheatManager::StaticClass();

	bAutoManageActiveCameraTarget = false;
}

void ATwoPlayerControllerBase::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController()) //在本地控制器中添加Mapping
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultContext, 10);
			//TODO 后期独立为一个控制
			SetInputMode(FInputModeGameOnly());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("AGenshinPlayerController::BeginPlay() UEnhancedInputLocalPlayerSubsystem is nullptr"));
		}
	}
}

void ATwoPlayerControllerBase::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (IsLocalController()) //在本地控制器中添加Bind
	{
		if (UTwoEnhancedInputComponent* GenshinEnhancedInput = CastChecked<UTwoEnhancedInputComponent>(InputComponent))
		{
			GenshinEnhancedInput->BindAction(
				MoveAction,
				ETriggerEvent::Triggered,
				this,
				&ThisClass::HandleMove
			);

			GenshinEnhancedInput->BindAction(
				LookAction,
				ETriggerEvent::Triggered,
				this,
				&ThisClass::HandleLook
			);

			GenshinEnhancedInput->BindAbilityAction(
				InputDataAsset,
				this,
				&ThisClass::AbilityInputTagPressed,
				&ThisClass::AbilityInputTagReleased,
				&ThisClass::AbilityInputTagHeld);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("UGenshinPlayerController::SetupInputComponent() UGenshinEnhancedInputComponent is nullptr"));
		}
	}
}

void ATwoPlayerControllerBase::HandleMove(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();
	if (APawn* ControlledPawn = GetPawn())
	{
		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// 从控制旋转中提取出Forward和Right方向
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		ControlledPawn->AddMovementInput(ForwardDirection, MovementVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ATwoPlayerControllerBase::HandleLook(const FInputActionValue& Value)
{
	const FVector2D LookVector = Value.Get<FVector2D>();
	AddPitchInput(LookVector.Y * UpDownRate);
	AddYawInput(LookVector.X * LeftRightRate);
}

void ATwoPlayerControllerBase::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (GetASC() == nullptr) return;
	GetASC()->AbilityInputTagPressed(InputTag);
}

void ATwoPlayerControllerBase::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (GetASC() == nullptr) return;
	GetASC()->AbilityInputTagReleased(InputTag);
}

void ATwoPlayerControllerBase::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (GetASC() == nullptr) return;
	GetASC()->AbilityInputTagHeld(InputTag);
}

UTwoAbilitySystemComponentBase* ATwoPlayerControllerBase::GetASC()
{
	if (GenshinAbilitySystemComponent == nullptr or !GenshinAbilitySystemComponent.IsValid())
	{
		GenshinAbilitySystemComponent = Cast<UTwoAbilitySystemComponentBase>(UTwoAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn()));
	}
	return GenshinAbilitySystemComponent.Get();
}
