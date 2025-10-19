#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "SpartaGameState.generated.h"

UCLASS()
class SPARTAPROJECT_API ASpartaGameState : public AGameState
{
    GENERATED_BODY()

public:
    ASpartaGameState();

protected:
    virtual void BeginPlay() override;

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Score")
    int32 Score;

    // 현재 레벨에서 스폰된 코인 개수
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coin")
    int32 SpawnedCoinCount;

    // 플레이어가 수집한 코인 개수
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coin")
    int32 CollectedCoinCount;

    // 각 레벨이 유지되는 시간 (초 단위)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
    float LevelDuration;

    // 현재 진행 중인 레벨 인덱스
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
    int32 CurrentLevelIndex;

    // 전체 레벨의 개수
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
    int32 MaxLevels;

    // 실제 레벨 맵 이름 배열. 여기 있는 인덱스를 차례대로 연동
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
    TArray<FName> LevelMapNames;

    // 매 레벨이 끝나기 전까지 시간이 흐르도록 관리하는 타이머
    FTimerHandle LevelTimerHandle;

    // HUD 업데이트를 위한 타이머 핸들
    FTimerHandle HUDUpdateTimerHandle;

    // --- 함수 선언 ---
    UFUNCTION(BlueprintPure, Category = "Score")
    int32 GetScore() const;

    UFUNCTION(BlueprintCallable, Category = "Score")
    void AddScore(int32 Amount);

    // 게임이 완전히 끝났을 때 (모든 레벨 종료) 실행되는 함수
    UFUNCTION(BlueprintCallable, Category = "Level")
    void OnGameOver();

    // 레벨을 시작할 때, 아이템 스폰 및 타이머 설정
    void StartLevel();

    // 레벨 제한 시간이 만료되었을 때 호출
    void OnLevelTimeUp();

    // 코인을 주웠을 때 호출
    void OnCoinCollected();

    // 레벨을 강제 종료하고 다음 레벨로 이동
    void EndLevel();

    // HUD를 갱신하는 함수
    void UpdateHUD();

    // 현재 웨이브 번호
    int32 CurrentWave;

    // 최대 웨이브 수
    int32 MaxWave;

    // 각 웨이브의 제한 시간
    float WaveTimeLimit;

    // 웨이브별로 스폰할 아이템 개수를 담을 배열
    TArray<int32> ItemsToSpawnPerWave;

    // 웨이브 시작/종료를 위한 함수 선언
    void StartWave();
    void EndWave();
    void OnWaveTimeUp(); // 타이머가 끝나면 호출될 함수

private:
    // 웨이브 시간 측정을 위한 타이머 핸들
    FTimerHandle WaveTimerHandle;
};