#include "SpartaGameState.h"
#include "SpartaGameInstance.h"
#include "SpartaPlayerController.h"
#include "SpawnVolume.h"
#include "CoinItem.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"

ASpartaGameState::ASpartaGameState()
{
    Score = 0;
    LevelDuration = 30.0f;
    CurrentLevelIndex = 0;
    MaxLevels = 3;
    SpawnedCoinCount = 0;
    CollectedCoinCount = 0;

    CurrentWave = 0; // 아직 시작 안 했으므로 0
    MaxWave = 3;     // 총 3번의 웨이브
    WaveTimeLimit = 15.0f; // 각 웨이브는 15초

    // 웨이브별 아이템 스폰 개수 설정
    ItemsToSpawnPerWave.Add(10); // Wave 1: 10개
    ItemsToSpawnPerWave.Add(20); // Wave 2: 20개
    ItemsToSpawnPerWave.Add(30); // Wave 3: 30개
}

void ASpartaGameState::BeginPlay()
{
    Super::BeginPlay();

    // [추가] 게임 시작 시 HUD를 먼저 띄워줍니다.
    if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
    {
        if (ASpartaPlayerController* SpartaPlayerController = Cast<ASpartaPlayerController>(PlayerController))
        {
            SpartaPlayerController->ShowGameHUD();
        }
    }

    StartWave(); // 레벨 시작 시 바로 첫 웨이브 시작

    GetWorldTimerManager().SetTimer(
        HUDUpdateTimerHandle,
        this,
        &ASpartaGameState::UpdateHUD,
        0.1f,
        true
    );
}

int32 ASpartaGameState::GetScore() const
{
    return Score;
}

void ASpartaGameState::AddScore(int32 Amount)
{
    if (UGameInstance* GameInstance = GetGameInstance())
    {
        if (USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance))
        {
            if (SpartaGameInstance)
            {
                SpartaGameInstance->AddToScore(Amount);
            }
        }
    }
}

//void ASpartaGameState::StartLevel()
//{
//    if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
//    {
//        if (ASpartaPlayerController* SpartaPlayerController = Cast<ASpartaPlayerController>(PlayerController))
//        {
//            SpartaPlayerController->ShowGameHUD();
//        }
//    }
//
//    if (UGameInstance* GameInstance = GetGameInstance())
//    {
//        if (USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance))
//        {
//            if (SpartaGameInstance)
//            {
//                CurrentLevelIndex = SpartaGameInstance->CurrentLevelIndex;
//            }
//        }
//    }
//
//    SpawnedCoinCount = 0;
//    CollectedCoinCount = 0;
//
//    TArray<AActor*> FoundVolumes;
//    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);
//
//    const int32 ItemToSpawn = 40;
//
//    if (FoundVolumes.Num() > 0)
//    {
//        if (ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[0]))
//        {
//            for (int32 i = 0; i < ItemToSpawn; i++)
//            {
//                AActor* SpawnedActor = SpawnVolume->SpawnRandomItem();
//                if (SpawnedActor && SpawnedActor->IsA(ACoinItem::StaticClass()))
//                {
//                    SpawnedCoinCount++;
//                }
//            }
//        }
//    }
//
//    GetWorldTimerManager().SetTimer(
//        LevelTimerHandle,
//        this,
//        &ASpartaGameState::OnLevelTimeUp,
//        LevelDuration,
//        false
//    );
//}

//void ASpartaGameState::OnLevelTimeUp()
//{
//    EndLevel();
//}

void ASpartaGameState::OnCoinCollected()
{
    CollectedCoinCount++;

    // [수정] 모든 코인을 모으면 즉시 웨이브 종료
    if (CollectedCoinCount >= SpawnedCoinCount)
    {
        EndWave();
    }
}

//void ASpartaGameState::EndLevel()
//{
//    GetWorldTimerManager().ClearTimer(LevelTimerHandle);
//
//    if (UGameInstance* GameInstance = GetGameInstance())
//    {
//        if (USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance))
//        {
//            if (SpartaGameInstance)
//            {
//                AddScore(Score);
//                CurrentLevelIndex++;
//                SpartaGameInstance->CurrentLevelIndex = CurrentLevelIndex;
//
//                if (CurrentLevelIndex >= MaxLevels)
//                {
//                    OnGameOver();
//                    return;
//                }
//
//                if (LevelMapNames.IsValidIndex(CurrentLevelIndex))
//                {
//                    UGameplayStatics::OpenLevel(GetWorld(), LevelMapNames[CurrentLevelIndex]);
//                }
//                else
//                {
//                    OnGameOver();
//                }
//            }
//        }
//    }
//}

void ASpartaGameState::OnGameOver()
{
    GetWorldTimerManager().ClearTimer(WaveTimerHandle); // 게임 오버 시 타이머 정지
    if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
    {
        if (ASpartaPlayerController* SpartaPlayerController = Cast<ASpartaPlayerController>(PlayerController))
        {
            SpartaPlayerController->SetPause(true);
            SpartaPlayerController->ShowMainMenu(true); // true는 게임 오버 메뉴를 의미
        }
    }
}


void ASpartaGameState::UpdateHUD()
{
    if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
    {
        if (ASpartaPlayerController* SpartaPlayerController = Cast<ASpartaPlayerController>(PlayerController))
        {
            if (UUserWidget* HUDWidget = SpartaPlayerController->GetHUDWidget())
            {
                // [수정] LevelTimerHandle -> WaveTimerHandle 로 변경
                if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Time"))))
                {
                    float RemainingTime = GetWorldTimerManager().GetTimerRemaining(WaveTimerHandle);
                    TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time: %.1f"), RemainingTime)));
                }

                if (UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Score"))))
                {
                    if (UGameInstance* GameInstance = GetGameInstance())
                    {
                        if (USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance))
                        {
                            // [선택] 현재 웨이브에서 먹은 코인까지 합산해서 보여주려면 아래처럼 수정 가능
                            // int32 CurrentTotalScore = SpartaGameInstance->TotalScore + CollectedCoinCount;
                            // ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), CurrentTotalScore)));

                            ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), SpartaGameInstance->TotalScore)));
                        }
                    }
                }

                // [수정] CurrentLevelIndex -> CurrentWave 로 변경
                // 위젯 이름을 "Level" 그대로 두었다면 텍스트 내용만 "Wave"로 바꿔서 보여줄 수도 있습니다.
                if (UTextBlock* LevelIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Level"))))
                {
                    LevelIndexText->SetText(FText::FromString(FString::Printf(TEXT("Wave: %d"), CurrentWave)));
                }
            }
        }
    }
}

void ASpartaGameState::StartWave()
{
    CurrentWave++;
    CollectedCoinCount = 0; // 웨이브 시작 시 수집한 코인 수 초기화

    if (CurrentWave <= MaxWave)
    {
        // 화면과 로그에 현재 웨이브 시작 알림 출력
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Wave %d Start!"), CurrentWave));
        UE_LOG(LogTemp, Warning, TEXT("Wave %d Start!"), CurrentWave);

        // 현재 웨이브에 맞는 개수만큼 코인 스폰
        SpawnedCoinCount = ItemsToSpawnPerWave[CurrentWave - 1]; // 배열 인덱스는 0부터 시작

        TArray<AActor*> FoundVolumes;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);
        if (FoundVolumes.Num() > 0)
        {
            ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[0]);
            if (SpawnVolume)
            {
                for (int32 i = 0; i < SpawnedCoinCount; i++)
                {
                    SpawnVolume->SpawnRandomItem();
                }
            }
        }

        // 제한 시간 후에 OnWaveTimeUp 함수를 호출하도록 타이머 설정
        GetWorldTimerManager().SetTimer(WaveTimerHandle, this, &ASpartaGameState::OnWaveTimeUp, WaveTimeLimit, false);
    }
    else
    {
        // 모든 웨이브가 끝났다면 게임 오버 처리
        OnGameOver();
    }
}

void ASpartaGameState::EndWave()
{
    GetWorldTimerManager().ClearTimer(WaveTimerHandle);
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Wave %d End!"), CurrentWave));

    // 점수 정산 (GameInstance에 현재까지 모은 코인 수를 더함)
    if (USpartaGameInstance* GameInstance = Cast<USpartaGameInstance>(GetGameInstance()))
    {
        GameInstance->TotalScore += CollectedCoinCount;
    }

    // 다음 웨이브 시작
    StartWave();
}

void ASpartaGameState::OnWaveTimeUp()
{
    EndWave();
}