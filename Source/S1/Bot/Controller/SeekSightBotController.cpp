
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
#include "Character/S1BotCharacter.h"
#include "Combat/S1CombatSystemComponent.h"
#include "Character/S1Character.h"
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

void ASeekSightBotController::SetStateAsAttacking(AActor* TargetActor, bool UseLastKnownAttackTarget)
{
    AActor* NewTargetEnemy = TargetActor;

    if (UseLastKnownAttackTarget && TargetEnemy)
    {
        NewTargetEnemy = TargetEnemy;
    }
    
    // @TODO HORK 나중에 인터페이스로 뺄 예정
    if (AS1BotCharacter* Bot = Cast<AS1BotCharacter>(NewTargetEnemy))
    {
        if (US1CombatSystemComponent* CombatSyatem = Bot->FindComponentByClass<US1CombatSystemComponent>())
        {
            if (CombatSyatem->IsDead())
            {
                SetStateAsPassive();
            }
            // NewTargetEnemy = TargetActor;
        }
    }

    // 타겟이 존재하지 않는다면 패시브 상태 전환
    if (!NewTargetEnemy)
    {
        SetStateAsPassive();
        return;
    }



    // ASC를 가져오고
    UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(NewTargetEnemy);
    if (ensure(TargetASC))
    {
        // ASC에서 HealthSet을 가져와 Hp를 가져온다
        const US1HealthSet* HealthSet = TargetASC->GetSet<US1HealthSet>();
        float CurHp = HealthSet->GetHealth();

        // @TODO HORK 테스트용으로 일단 HP 10을 넣어둠(나중에 수정해야함, 0으로)
        // 플레이어 죽었다면 Passive 상태로 전환해주자
        if (CurHp <= 0.f)
        {
            SetStateAsPassive();
            return;
        }
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
    if (AS1BotCharacter* Bot = Cast<AS1BotCharacter>(GetCharacter()))
    {
        if (AS1Character* Player = Cast<AS1Character>(TargetEnemy))
        {
            Player->ReturnAttackToken(Bot->GetCurrentAttackToken());
        }
    }
    
}

EAIState ASeekSightBotController::GetCurrentState() const
{
    return (EAIState)Blackboard->GetValueAsEnum(StateKeyName);
}

void ASeekSightBotController::HandleSensedSight(AActor* PerceptionActor)
{
    // @TODO HORK 나중에 TeamID 값으로 따라 갈 수 있도록 설정(아마.. 페로몬 폭탄 같은 거에서 쓸 수 있을 듯?) 
    if (GetWorld()->GetFirstPlayerController()->GetPawn() != PerceptionActor)
    {
        return;
    }
    
    if(UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(PerceptionActor))
    {
        KnownSeenActors.AddUnique(PerceptionActor);

        EAIState CurrentState = GetCurrentState();
        switch (CurrentState)
        {
        // 기존, 조사, 추적 상태에서 상대를 발견했다면, 공격 상태로 전환
        case EAIState::Passive:
        case EAIState::Investigating:
        case EAIState::Seeking:
            SetStateAsAttacking(PerceptionActor, true);
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
        SetStateAsAttacking(PerceptionActor, true);
        break;
    case EAIState::Attacking:
    case EAIState::Dead:
    default:
        break;
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
            //SetStateAsSkeeing(TargetEnemy->GetActorLocation());
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

void ASeekSightBotController::HandleForgotActor(AActor* TargetActor)
{
    // TargetEnemy가 지운 Actor라면? Passive 상태로 돌아간다
    if (TargetActor == TargetEnemy)
    {
        SetStateAsPassive();
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
    AIPerception->GetKnownPerceivedActors(UAISense_Sight::StaticClass(), PerceivedActors);

    if (PerceivedActors.Num() != KnownSeenActors.Num() && !KnownSeenActors.IsEmpty())
    {
        for (auto SeenIt = KnownSeenActors.CreateIterator(); SeenIt; ++SeenIt)
        {
            if (PerceivedActors.Find(*SeenIt) == INDEX_NONE)
            {
                //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("Forget Actor"));
                HandleForgotActor(*SeenIt);
                SeenIt.RemoveCurrent();
            }
        }

    }
}


