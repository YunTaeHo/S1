
// Copyright (c) S1

#include "S1GameplayAbility_RangedWeapon.h"
#include "S1RangedWeaponInstance.h"
#include "Physics/S1CollisionChannels.h"
#include "AbilitySystem/S1AbilitySystemComponent.h"
#include "AbilitySystem/S1GameplayAbilityTargetData_SingleTargetHit.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(S1GameplayAbility_RangedWeapon)

US1GameplayAbility_RangedWeapon::US1GameplayAbility_RangedWeapon(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{

}

void US1GameplayAbility_RangedWeapon::StartRangedWeaponTargeting()
{
    // ActorInfo는 AbilitySet에서 GiveAbility() 호출로 설정된다
    // 1. S1AbilitySet::GiveToAbilitySystem()에서 GiveAbility()를 호출한다
    // 2. AbilitySystemComponent::GiveAbility()에서 OnGiveAbility()를 호출한다
    // 3. UGameplayAbility::OnGiveAbility()에서 SetCurrentActorInfo()가 설정된다
    check(CurrentActorInfo);

    AActor* AvatarActor = CurrentActorInfo->AvatarActor.Get();
    check(AvatarActor);

    UAbilitySystemComponent* MyASC = CurrentActorInfo->AbilitySystemComponent.Get();
    check(MyASC);

    // @TODO HORK 샷건 처리와 같은 탄착 처리 필요

    // 총알의 궤적의 Hit 정보를 계산
    TArray<FHitResult> FoundHits;
    PerformLocalTargeting(FoundHits);

    // GameplayAbilityTargetData는 싱글게임 기반이므로 큰 의미는 없다
    FGameplayAbilityTargetDataHandle TargetData;
    TargetData.UniqueId = 0;

    if (FoundHits.Num() > 0)
    {
        // Cartridge란, 일반 권총의 경우, 탄약에 하나의 총알이 들어있지만,
        // 샷건의 경우, 탄약에 여러개의 총알이 있다(탄약 == 카트리지)
        const int32 CartridgeID = FMath::Rand();
        for (const FHitResult& FoundHit : FoundHits)
        {
            // AbilityTargetData에 SingleTargetHit 정보를 담는다
            // TargetData.Add()의 경우, SharedPtr에 넣기 때문에 new를 써도 누수가 발생하지 않는다
            FS1GameplayAbilityTargetData_SingleTargetHit* NewTargetData = new FS1GameplayAbilityTargetData_SingleTargetHit();
            NewTargetData->HitResult = FoundHit;
            NewTargetData->CartridgeID = CartridgeID;
            TargetData.Add(NewTargetData);
        }
    }

    // 가공된 AbilityTargetData가 준비되었으므로, OnTargetDataReadyCallback을 호출한다
    OnTargetDataReadyCallback(TargetData, FGameplayTag());
}

US1RangedWeaponInstance* US1GameplayAbility_RangedWeapon::GetWeaponInstance()
{
    return Cast< US1RangedWeaponInstance>(GetAssociatedEquipment());
}

FVector US1GameplayAbility_RangedWeapon::GetWeaponTargetingSourceLocation() const
{
    // 현재 이 함수는 미구현 함수 @TODO HORK
    APawn* const AvatarPawn = Cast<APawn>(GetAvatarActorFromActorInfo());
    check(AvatarPawn);

    const FVector SourceLoc = AvatarPawn->GetActorLocation();
    return SourceLoc;
}

int32 FindFirstPawnHitResult(const TArray<FHitResult>& HitResults)
{
    for (int32 Idx = 0; Idx < HitResults.Num(); ++Idx)
    {
        const FHitResult& CurHitResult = HitResults[Idx];
        if (CurHitResult.HitObjectHandle.DoesRepresentClass(APawn::StaticClass()))
        {
            return Idx;
        }
        else
        {
            AActor* HitActor = CurHitResult.HitObjectHandle.FetchActor();

            // 한단계 AttachParent에 Actor가 Pawn이라면?
            // - 보통 복수개 단계로 AttachParent를 하지 않으므로, AttachParent 대상이 APawn일 확률이 높다 
            // 두 단계, 세 단계 올라간다면 어떻게? @TODO HORK LAST 이건 나중에 하자
            if ((HitActor != nullptr) && (HitActor->GetAttachParentActor() != nullptr) && (Cast<APawn>(HitActor->GetAttachParentActor()) != nullptr))
            {
                return Idx;
            }
        }
    }

    return INDEX_NONE;
}

FHitResult US1GameplayAbility_RangedWeapon::DoSingleBulletTrace(const FVector& StartTrace, const FVector& EndTrace, float SweepRadius, bool bIsSimulated, TArray<FHitResult>& OutHits)
{
    FHitResult Impact;
    
    // 우선 SweepRadius 없이 한번 Trace 진행한다 (SweepTrace는 무겁기 때문)
    // - FindFirstPawnHitResult()를 여러번 Trace 진행을 막기 위해 
    //  OutHits를 확인해서 APawn 충돌이 존재한다면 더이상 Trace 하지 않는다
    if (FindFirstPawnHitResult(OutHits) == INDEX_NONE)
    {
        Impact = WeaponTrace(StartTrace, EndTrace, /*SweepRadius=*/0.f, bIsSimulated, /*out*/OutHits);
    }

    if (FindFirstPawnHitResult(OutHits) == INDEX_NONE)
    {
        // 만약 SweepRadius가 0보다 크면, 0.0일때 대비 충돌 가능성이 커지므로 한번 더 Trace 진행
        if (SweepRadius > 0.f)
        {
            // SweppHits에서 Trace의 OutHits 정보를 저장
            TArray<FHitResult> SweepHits;
            Impact = WeaponTrace(StartTrace, EndTrace, SweepRadius, bIsSimulated, SweepHits);

            // Sphere Trace로 진행한 결과인 SweepHits를 검색하여, Pawn이 있는가 검색
            const int32 FirstPawnIdx = FindFirstPawnHitResult(SweepHits);
            if (SweepHits.IsValidIndex(FirstPawnIdx))
            {
                // 만약 있다면, SweepHits를 FirstPawnIdx까지 순회하며, bBlockingHit와 기존 OutHits에 없을 경우 체크한다
                bool bUseSweepHits = true;
                for (int32 Idx = 0; Idx < FirstPawnIdx; Idx)
                {
                    const FHitResult& CurHitResult = SweepHits[Idx];

                    auto Pred = [&CurHitResult](const FHitResult& Other)
                        {
                            return Other.HitObjectHandle == CurHitResult.HitObjectHandle;
                        };

                    // OutHits에 있다면 SweepHits를 OutHits로 업데이트 하지 않는다(이미 충돌했던 정보가 존재함)
                    if (CurHitResult.bBlockingHit && OutHits.ContainsByPredicate(Pred))
                    {
                        bUseSweepHits = false;
                        break;
                    }
                }

                // SweepHits
                if (bUseSweepHits)
                {
                    OutHits = SweepHits;
                }
            }
        }
    }

    return Impact;
}

void US1GameplayAbility_RangedWeapon::TraceBulletsInCartridge(const FRangedWeaponFiringInput& InputData, TArray<FHitResult>& OutHits)
{
    US1RangedWeaponInstance* WeaponData = InputData.WeaponData;
    check(WeaponData);

    // MaxDamageRange를 고려하여, EndTrace를 정의하자
    const FVector BulletDir = InputData.AimDir;
    const FVector EndTrace = InputData.StartTrace + (BulletDir * WeaponData->MaxDamageRange);

    // HitLocation의 초기화 값으로 EndTrace로 설정
    FVector HitLocation = EndTrace;

    // 총알을 Trace 하나로 진행한다
    // @TODO HORK 샷건과 같은 경우 Cartridge에 여러개의 총알이 있을 경우를 처리하기 위해,
    // for-loop를 활용하여, 복수개 Bullet를 Trace한다
    TArray<FHitResult> AllImpacts;
    FHitResult Impact = DoSingleBulletTrace(InputData.StartTrace, EndTrace,
        WeaponData->BulletTraceWeaponRadius, /**bIsSimulated=*/false, /**out*/AllImpacts);

    const AActor* HitActor = Impact.GetActor();
    if (HitActor)
    {
        if (AllImpacts.Num() > 0)
        {
            OutHits.Append(AllImpacts);
        }

        HitLocation = Impact.ImpactPoint;
    }

    // OutHits가 적어도 하나가 존재하도록, EndTrace를 활용하여, OutHits에 추가해준다
    if (OutHits.Num() == 0)
    {
        if (!Impact.bBlockingHit)
        {
            Impact.Location = EndTrace;
            Impact.ImpactPoint = EndTrace;
        }

        OutHits.Add(Impact);
    }
}

void US1GameplayAbility_RangedWeapon::AddAdditionalTraceIgnoreActors(FCollisionQueryParams& TraceParams) const
{
    if (AActor* Avatar = GetAvatarActorFromActorInfo())
    {
        TArray<AActor*> AttachedActors;

        // GetAttachedActors는 재귀적으로 모든 Actors를 추출한다?
        // FindFirstPawnHitResult는 그럼 왜 한 단계만 진행? 
        Avatar->GetAttachedActors(AttachedActors);

        TraceParams.AddIgnoredActors(AttachedActors);
    }
}

ECollisionChannel US1GameplayAbility_RangedWeapon::DetermineTraceChannel(FCollisionQueryParams& TraceParams, bool bIsSimulated) const
{
    return S1_TraceChannel_Weapon;
}

FHitResult US1GameplayAbility_RangedWeapon::WeaponTrace(const FVector& StartTrace, const FVector& EndTrace, float SweepRadius, bool bIsSimulated, TArray<FHitResult>& OutHitResults) const
{
    TArray<FHitResult> HitResults;

    // Complex Geometry로 Trace를 진행하며, AvatarActor를 AttachParanet를 가지는 오브젝트와의 충돌은 무시한다
    FCollisionQueryParams TraceParams(SCENE_QUERY_STAT(WeaponTrace), /*bTraceComplex*/true, /*IgnoreActor=*/GetAvatarActorFromActorInfo());
    TraceParams.bReturnPhysicalMaterial = true;

    // AvatarActor에 부착된 Actors를 찾아 IgnoredActors에 추가한다
    AddAdditionalTraceIgnoreActors(TraceParams);

    // Weapon 관련 Collision Channel로 Trace 진행
    const ECollisionChannel TraceChannel = DetermineTraceChannel(TraceParams, bIsSimulated);
    if (SweepRadius > 0.f)
    {
        GetWorld()->SweepMultiByChannel(HitResults, StartTrace, EndTrace, FQuat::Identity, TraceChannel, FCollisionShape::MakeSphere(SweepRadius), TraceParams);
    }
    else
    {
        GetWorld()->LineTraceMultiByChannel(HitResults, StartTrace, EndTrace, TraceChannel, TraceParams);
    }

    FHitResult Hit(ForceInit);
    if (HitResults.Num() > 0)
    {
        // HitResults 중에 중복(같은) Object의 HitResult 정보를 제거
        for (FHitResult& CurHitResult : HitResults)
        {
            auto Pred = [&CurHitResult](const FHitResult& Other)
                {
                    return Other.HitObjectHandle == CurHitResult.HitObjectHandle;
                };

            if (!OutHitResults.ContainsByPredicate(Pred) && TEXT("CollisionCylinder") != CurHitResult.Component.Get()->GetName())
            {
                OutHitResults.Add(CurHitResult);
            }
        }

        if (OutHitResults.IsEmpty())
        {
            Hit.TraceStart = StartTrace;
            Hit.TraceEnd = EndTrace;
        }
        else
        {
            // Hit의 가장 마지막 값을 Impact로 저장 
            Hit = OutHitResults.Last();
        }
    }
    else
    {
        // Hit 결과 값을 기본 값으로 캐싱
        Hit.TraceStart = StartTrace;
        Hit.TraceEnd = EndTrace;
    }

    return Hit;
}

FTransform US1GameplayAbility_RangedWeapon::GetTargetingTransform(APawn* SourcePawn, ES1AbilityTargetingSource Source)
{
    check(SourcePawn);
    check(Source == ES1AbilityTargetingSource::CameraTowardsFocus);

    // @ TODO HORK 완전한 로직 작성(Lyra)

    AController* Controller = SourcePawn->Controller;
    if (Controller == nullptr)
    {
        return FTransform();
    }

    // 매직넘버
    double FocalDistance = 1024.0;
    FVector FocalLoc;
    FVector CamLoc;
    FRotator CamRot;

    // PlayerController로부터, Location, Rotation 정보를 가져옴
    APlayerController* PC = Cast<APlayerController>(Controller);
    check(PC);
    PC->GetPlayerViewPoint(CamLoc, CamRot);

    FVector AimDir = CamRot.Vector().GetSafeNormal();
    FocalLoc = CamLoc + (AimDir * FocalDistance);

    // WeaponLoc이 아닌 Pawn의 Loc이다(@TODO Hork WeaponLoc으로 바꿔야함)
    const FVector WeaponLoc = GetWeaponTargetingSourceLocation();
    FVector FinalCamLoc = FocalLoc + (((WeaponLoc - FocalLoc) | AimDir) * AimDir);

#if 0
    
    // WeaponLoc (사실상 ActorLoc)
    DrawDebugPoint(GetWorld(), WeaponLoc, 10.f, FColor::Red, false, 60.f);
    // CamLoc
    DrawDebugPoint(GetWorld(), CamLoc, 10.f, FColor::Yellow, false, 60.f);
    // FinalCamLoc
    DrawDebugPoint(GetWorld(), FinalCamLoc, 10.f, FColor::Magenta, false, 60.f);

    // (WeaponLoc - FocalLoc)
    DrawDebugLine(GetWorld(), FocalLoc, WeaponLoc, FColor::Yellow, false, 60.f, 0, 2.f);

    // (AimDir)
    DrawDebugLine(GetWorld(), CamLoc, FocalLoc, FColor::Blue, false, 60.f, 0, 2.f);

    // Project Direction Line
    DrawDebugLine(GetWorld(), WeaponLoc, FinalCamLoc, FColor::Red, false, 60.f, 0, 2.f);

#endif

    // Camera -> Focus 계산 완료,
    return FTransform(CamRot, FinalCamLoc);
}



void US1GameplayAbility_RangedWeapon::PerformLocalTargeting(TArray<FHitResult>& OutHits)
{
    APawn* const AvatarPawn = Cast<APawn>(GetAvatarActorFromActorInfo());

    US1RangedWeaponInstance* WeaponData = GetWeaponInstance();
    if (AvatarPawn && AvatarPawn->IsLocallyControlled() && WeaponData)
    {
        FRangedWeaponFiringInput InputData;
        InputData.WeaponData = WeaponData;
        InputData.bCanPlayBulletFX = true;

        const FTransform TargetTransfrom = GetTargetingTransform(AvatarPawn, ES1AbilityTargetingSource::CameraTowardsFocus);
        // Axis::X (앞 방향)기준으로 Ray 발사
        InputData.AimDir = TargetTransfrom.GetUnitAxis(EAxis::X);
        InputData.StartTrace = TargetTransfrom.GetTranslation();
        InputData.EndAim = InputData.StartTrace + InputData.AimDir * WeaponData->MaxDamageRange;

#if 0
        {
            static float DebugThickness = 2.f;
            DrawDebugLine(GetWorld(), InputData.StartTrace, InputData.StartTrace + (InputData.AimDir * 100.f), 
                FColor::Yellow, false, 10.f, 0, DebugThickness);
        }
#endif

        TraceBulletsInCartridge(InputData, OutHits);
    }
}


void US1GameplayAbility_RangedWeapon::OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& InData, FGameplayTag ApplicationTag)
{
    UAbilitySystemComponent* MyASC = CurrentActorInfo->AbilitySystemComponent.Get();
    check(MyASC);

    if (const FGameplayAbilitySpec* AbilitySpec = MyASC->FindAbilitySpecFromHandle(CurrentSpecHandle))
    {
        // 현재 Stack에 InData에서 Local로 Ownership을 가져온디
        FGameplayAbilityTargetDataHandle LocalTargetDataHandle(MoveTemp(const_cast<FGameplayAbilityTargetDataHandle&>(InData)));
    
        // CommitAbility 호출로 GE(GameplayEffect)를 처리한다
        if (CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo))
        {
            // OnRangeWeaponTargetDataReady BP 노드 호출한다
            // GCN(GameplayCueNotify)도 마찬가지로 처리한다
            OnRangeWeaponTargetDataReady(LocalTargetDataHandle);
        }
        else
        {
            // CommitAbility가 실패했다면, EndAbility BP 노드를 호출한다
            K2_EndAbility();
        }
    }
}