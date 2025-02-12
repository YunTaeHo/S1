// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/Interface.h"
#include "BlockInterface.generated.h"

enum class EBlockingState : uint8;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UBlockInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *  해당 인터페이스는 Character가 Block을 수행할 시 필요한 클래스입니다
 */
class S1_API IBlockInterface
{
	GENERATED_BODY()

public:
	/** 방어 할 수 있는 지 확인하는 함수 */
	virtual void TryToBlock() = 0;

	/*
	 * 공격을 방어하는 수단의 시작
	 *	- 방어 자세를 취하면서 Block한다
	 *	- HitReact에서 Block이 성공하면 방어 애니메이션을 취해주거나 반격한다
	 *	- Blcok이 실패한다면, 넉백이나 기절, 정지 등을 표현해주자
	 */
	virtual void StartBlcok(EBlockingState BlockingState) = 0;

	/*
	 * 공격 방어를 마무리하는 단계
	 *	- 애니메이션을 가드에서 Idle 단계로 넘어갈 수 있도록 처리해준다
	 *  - 만약 필요하다면 가드가 끝난 직후 바로 스킬을 사용할 수 있도록 표현한다
	 */
	virtual void EndBlcok() = 0;

	/*
	 * 공격 방어
	 *	- BlockingState에 따라 변화시켜주면 된다
	 *	- None이 들어왔을 때 Blcoking이 된다면 Block Successfully
	 *  - BlockKnockback이 들어왔을 때 저항이 없다면 넉백
	 */
	virtual void Blocking(EBlockingState BlockingState) = 0;

};
