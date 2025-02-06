
// Copyright (c) S1

#include "SeekSightBotController.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/Attributes/S1HealthSet.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Damage.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AIPerceptionTypes.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(SeekSightBotController)

ASeekSightBotController::ASeekSightBotController(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{

}

void ASeekSightBotController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    GetWorld()->GetTimerManager().SetTimer(CheckForgottenActosTimer, this, &ThisClass::CheckForgettenSeenActor, 0.5f, true);
}

void ASeekSightBotController::OnUnPossess()
{
    Super::OnUnPossess();
    GetWorld()->GetTimerManager().ClearTimer(CheckForgottenActosTimer);
}

void ASeekSightBotController::ActorsPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
    for (AActor* PerceptionActor : UpdatedActors)
    {
        FAIStimulus AIStimulus;

        // 해당 시야에 Actor가 걸렸다면?
        if (CanSenseActor(PerceptionActor, EAISense::Sight, AIStimulus))
        {
            // Sight에 관한 데이터를 핸들링한다
            HandleSensedSight(PerceptionActor);
        }
        else
        {
            // 해당 액터를 Sight하지 못했기 때문에 배열에 존재한다면 제거해준다
            HandleLostSight(PerceptionActor);
        }

        // 해당 Actor가 공격당했다면?
        if (CanSenseActor(PerceptionActor, EAISense::Damage, AIStimulus))
        {
            // Damage에 관한 데이터를 핸들링한다
            HandleSensedDamage(PerceptionActor);

        }
        
        // 해당 소리가 Actor에 걸렸다면?
        if (CanSenseActor(PerceptionActor, EAISense::Hearing, AIStimulus))
        {
            // Sound에 관한 데이터를 핸들링한다
            HandleSensedSound(AIStimulus.StimulusLocation);
        }
    }
}

void ASeekSightBotController::SetStateAsPassive()
{
    Blackboard->SetValueAsEnum(StateKeyName, (uint8)EAIState::Passive);
}

void ASeekSightBotController::SetStateAsAttacking(AActor* TargetActor)
{
    AActor* NewTargetEnemy = nullptr;

    // 타겟이 기존에 있었다면 유지
    if (TargetEnemy)
    {
        NewTargetEnemy = TargetEnemy;
    }
    // 기존 타겟이 없다면 현재 들어온 Actor로 타겟을 변경
    else
    {
        NewTargetEnemy = TargetActor;
    }


    // 타겟이 존재하지 않는다면 패시브 상태 전환
    if (!NewTargetEnemy)
    {
        SetStateAsPassive();
        return;
    }

    // ASC를 가져오고
    UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(NewTargetEnemy);
    check(TargetASC);

    // ASC에서 HealthSet을 가져와 Hp를 가져온다
    const US1HealthSet* HealthSet = TargetASC->GetSet<US1HealthSet>();
    float CurHp = HealthSet->GetHealth();

    // @TODO HORK 테스트용으로 일단 HP 10을 넣어둠(나중에 수정해야함, 0으로)
    // 플레이어 죽었다면 Passive 상태로 전환해주자
    if (CurHp <= 10)
    {
        SetStateAsPassive();
        return;
    }


    // 타겟을 정하고, 공격 상태로 전이
    TargetEnemy = NewTargetEnemy;

    Blackboard->SetValueAsObject(TargetEnemyKeyName, TargetEnemy);
    Blackboard->SetValueAsEnum(StateKeyName, (uint8)EAIState::Attacking);
}

void ASeekSightBotController::SetStateAsInvestigating(FVector Location)
{
    Blackboard->SetValueAsVector(PointOfInterestKeyName, Location);
    Blackboard->SetValueAsEnum(StateKeyName, (uint8)EAIState::Investigating);
}

void ASeekSightBotController::SetStateAsFrozen()
{
    Blackboard->SetValueAsEnum(StateKeyName, (uint8)EAIState::Frozen);
}

void ASeekSightBotController::SetStateAsSkeeing(FVector Location)
{
    Blackboard->SetValueAsVector(PointOfInterestKeyName, Location);
    Blackboard->SetValueAsEnum(StateKeyName, (uint8)EAIState::Seeking);
}

void ASeekSightBotController::SetStateAsDead()
{
    Blackboard->SetValueAsEnum(StateKeyName, (uint8)EAIState::Dead);
}

EAIState ASeekSightBotController::GetCurrentState() const
{
    return (EAIState)Blackboard->GetValueAsEnum(StateKeyName);
}

bool ASeekSightBotController::CanSenseActor(AActor* PerceptionActor, EAISense Sense, FAIStimulus& Stimulus)
{
    FActorPerceptionBlueprintInfo PerceptionInfo;

    // 시물레이션 정보를 가져온다
    AIPerception->GetActorsPerception(PerceptionActor, PerceptionInfo);

    
    TSubclassOf<UAISense> CurrentAISense;

    // 들어온 Enum의 기준으로 CurrentAISense를 활성화한다.
    switch (Sense)
    {
    case EAISense::Sight:
        CurrentAISense = UAISense_Sight::StaticClass();
        break;
    case EAISense::Hearing:
        CurrentAISense = UAISense_Hearing::StaticClass();
        break;
    case EAISense::Damage:
        CurrentAISense = UAISense_Damage::StaticClass();
        break;
    case EAISense::None:
    default:
        break;
    }


    // 가져온 시뮬레이션 정보들을 순회하며
    for (const FAIStimulus& AIStimulus : PerceptionInfo.LastSensedStimuli)
    {
        // 해당 시뮬레이션의 Sense 메타 데이터를 가져오고
        TSubclassOf<UAISense> StimulusAISense = UAIPerceptionSystem::GetSenseClassForStimulus(this, AIStimulus);

        // AISense가 해당 Sense라면?
        if (CurrentAISense == StimulusAISense)
        {
            // 시뮬레이션에 대한 정보를 전달하고, true를 반환한다
            Stimulus = AIStimulus;
            return true;
        }
    }

    // 아무 Sense도 발생하지 않았기 때문에 감지를 못하도록 막아주자
    return false;
}

void ASeekSightBotController::HandleSensedSight(AActor* PerceptionActor)
{
    // @TODO HORK 나중에 TeamID 값으로 따라 갈 수 있도록 설정(아마.. 페로몬 폭탄 같은 거에서 쓸 수 있을 듯?) 
    if (GetWorld()->GetFirstPlayerController()->GetPawn() == PerceptionActor)
    {
        KnownSeenActors.AddUnique(PerceptionActor);

        EAIState CurrentState = GetCurrentState();
        switch (CurrentState)
        {
        // 기존, 조사, 추적 상태에서 상대를 발견했다면, 공격 상태로 전환
        case EAIState::Passive:
        case EAIState::Investigating:
        case EAIState::Seeking:
            SetStateAsAttacking(PerceptionActor);
            break;
        // 공격 상태라면 Seek상태에 대한 타이머를 초기화 시켜준다
        case EAIState::Attacking:
            if (PerceptionActor == TargetEnemy)
            {
                GetWorld()->GetTimerManager().ClearTimer(SeekAttackTargetTimer);
            }
            break;
        case EAIState::Frozen:
        case EAIState::Dead:
        default:
            break;
        }
    
    }


    
}

void ASeekSightBotController::HandleSensedSound(FVector LocationAtSound)
{
    EAIState CurrentState = GetCurrentState();

    switch (CurrentState)
    {
    case EAIState::Passive:
    case EAIState::Investigating:
    case EAIState::Seeking:
        SetStateAsInvestigating(LocationAtSound);
        break;
    case EAIState::Attacking:
    case EAIState::Frozen:
    case EAIState::Dead:
    default:
        break;
    }
}

void ASeekSightBotController::HandleSensedDamage(AActor* PerceptionActor)
{
    EAIState CurrentState = GetCurrentState();

    switch (CurrentState)
    {
    case EAIState::Passive:
    case EAIState::Investigating:
    case EAIState::Seeking:
    case EAIState::Frozen:
        SetStateAsAttacking(PerceptionActor);
        break;
    case EAIState::Attacking:
    case EAIState::Dead:
    default:
        break;
    }
}

void ASeekSightBotController::HandleForgotActor(AActor* TargetActor)
{
    // Shight로 못찾은 Actor를 지워준다
    KnownSeenActors.Remove(TargetActor);

    // TargetEnemy가 지운 Actor라면? Passive 상태로 돌아간다
    if (TargetActor == TargetEnemy)
    {
        SetStateAsPassive();
    }
}

void ASeekSightBotController::HandleLostSight(AActor* TargetActor)
{
    if (TargetActor == TargetEnemy)
    {
        EAIState CurrentState = GetCurrentState();

        switch (CurrentState)
        {
        case EAIState::Attacking:
        case EAIState::Frozen:
        case EAIState::Investigating:
            GetWorld()->GetTimerManager().ClearTimer(SeekAttackTargetTimer);
            GetWorld()->GetTimerManager().SetTimer(SeekAttackTargetTimer, this, &ThisClass::SeekAttackTarget, TimeToSeekAfterLosingShight);
            break;
        case EAIState::Passive:
        case EAIState::Seeking:
        case EAIState::Dead:
        default:
            break;
        }
    }
}

void ASeekSightBotController::SeekAttackTarget()
{
    check(TargetEnemy);

    SetStateAsSkeeing(TargetEnemy->GetActorLocation());
    GetWorld()->GetTimerManager().ClearTimer(SeekAttackTargetTimer);
}

void ASeekSightBotController::CheckForgettenSeenActor()
{
    TArray<AActor*> PerceivedActors;
    AIPerception->GetKnownPerceivedActors(UAISense::StaticClass(), PerceivedActors);

    if (PerceivedActors.Num() != KnownSeenActors.Num())
    {
        for (AActor* SeenActor : KnownSeenActors)
        {
            if (PerceivedActors.Find(SeenActor) == INDEX_NONE)
            {
                HandleForgotActor(SeenActor);
            }
        }
    }
}


