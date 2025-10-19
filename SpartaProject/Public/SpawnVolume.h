#pragma once

#include "CoreMinimal.h"
#include "ItemSpawnRow.h"
#include "GameFramework/Actor.h"
#include "SpawnVolume.generated.h"

class UBoxComponent;
class UDataTable;

UCLASS()
class SPARTAPROJECT_API ASpawnVolume : public AActor
{
    GENERATED_BODY()

public:
    ASpawnVolume();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
    USceneComponent* Scene;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
    UBoxComponent* SpawningBox;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
    UDataTable* ItemDataTable;

public: // UFUNCTION을 외부에서 호출할 수 있도록 public으로 변경
    // 데이터 테이블을 기반으로 랜덤 아이템을 스폰하는 함수
    UFUNCTION(BlueprintCallable, Category = "Spawning")
    AActor* SpawnRandomItem(); // 리턴 형식을 AActor* 로 변경

protected:
    // 데이터 테이블에서 확률에 따라 아이템 Row를 선택하는 함수
    FItemSpawnRow* GetRandomItem() const;

    // 특정 아이템 클래스를 스폰하는 함수 (내부 호출용)
    AActor* SpawnItem(TSubclassOf<AActor> ItemClass); // 리턴 형식을 AActor* 로 변경

    // 스폰 볼륨 내부에서 무작위 좌표를 얻어오는 함수
    FVector GetRandomPointInVolume() const;
};