// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

/** foward declarations */
struct FDamageInfo;
enum class EHitResponse : uint8;


// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 전투와 관련된 인터페이스
 *	- Damge
 *	- Dead
 */
class S1_API ICombatInterface
{
	GENERATED_BODY()

public:
	/** 해당 함수를 통해 데미지를 가할 수 있습니다 */
	virtual void DamageOnEvent(AActor* DamageCursor, FDamageInfo Info) = 0;

	/** 죽음을 관장하는 함수 */
	virtual bool IsDead() = 0;
	virtual void SetDead(bool bDead) = 0;

	/*
	 * 각자 죽음에 대해 처리
	 *	- Boos는 시네마틱을 처리할 수도 있음
	 *  - 기본 몬스터는 바로 죽게 만들어줌
	 */
	virtual void Die() = 0;

	/*
	* 각자 공격을 Blueprint에서 함수를 여러 개 묶어서 사용할 수 있도록 설정
	*  - 각각의 보스 패턴들은 C++로 만들어 구현
	*/
	virtual void Attack(AActor* AttackTarget) = 0;


	/*
	 *	Hit 반응에 대해 처리
	 *  - 슈퍼아머를 지니고 있는 캐릭터는 스턴이나 넉백에 반응하지 않음
	 *	- 보스 캐릭터는 모두 상태에 대해 반응하지 않음
	 */
	virtual void HitReact(EHitResponse HitResponse, AActor* DamageCursor) = 0;
	
};
