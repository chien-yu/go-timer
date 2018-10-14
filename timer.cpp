#include <iostream>
#include <thread>
#include <chrono>
#include <stdio.h>


#define MAINTIME 10
#define EXTRATIME 5
#define EXTRACOUNT 3
// #define FASTMODEEXTRATIME 6
// #define FASTMODEEXTRACOUNT 2
// #define FASTMODE

// Normal mode: define  MAINTIME
//                      EXTRATIME
//                      EXTRACOUNT
//
// Fast mode:   define  FASTMODEEXTRATIME
//                      FASTMODEEXTRACOUNT
//                      FASTMODE
//              set     MAINTIME            = 0
//                      EXTRACOUNT          = 1

struct PlayerStatus
{
    PlayerStatus(uint m, uint t1, uint c1,
        //TODO
        // uint t2, uint c2,
        std::string n):
        mainTime(m),
        extraTime(t1),
        extraRound(c1),
        //TODO
        // fastModeExtraTime(t2),
        // fastModeExtraCount(c2),
        name(n){};
    uint mainTime;
    uint extraTime;
    uint extraRound;
    //TODO
    // uint fastModeExtraTime;
    // uint fastModeExtraCount;
    std::string name;
};

enum class GameState
{
    PRE,
    BLACK,
    WHITE,
    PAUSE
};

// enum class TimeState
// {
//     MAIN,
//     BYO_YOMI
// };

// enum class GameMode
// {
//     NORMAL,
//     FAST,
// };

void print(std::string s)
{
    std::cout << s << std::endl;
}

GameState gameState = GameState::PRE;
// GameMode gameMode = GameMode::FAST;

void count()
{
    PlayerStatus pB(MAINTIME, EXTRATIME, EXTRACOUNT,
        // FASTMODEEXTRATIME, FASTMODEEXTRACOUNT,
        "BLACK");
    PlayerStatus pW(MAINTIME, EXTRATIME, EXTRACOUNT,
        // FASTMODEEXTRATIME, FASTMODEEXTRACOUNT,
        "WHITE");
    gameState = GameState::BLACK;
    // TimeState timeState = TimeState::MAIN;
    bool bIsPauseState = false;
    char tmp[100];
    while(1)
    {
        PlayerStatus* currentPlayerStatus;
        if (gameState == GameState::BLACK)
        {
            if (bIsPauseState)
            {
                system("say 繼續");
            }
            bIsPauseState = false;
            currentPlayerStatus = &pB;
        }
        else if (gameState == GameState::WHITE)
        {
            if (bIsPauseState)
            {
                system("say 繼續");
            }
            bIsPauseState = false;
            currentPlayerStatus = &pW;
        }
        else if (gameState == GameState::PAUSE)
        {
            if (!bIsPauseState)
            {
                system("say 暫停");
                bIsPauseState = true;
            }
            currentPlayerStatus = NULL;
        }

        if (currentPlayerStatus == NULL)
        {
            continue;
        }

        if(currentPlayerStatus->mainTime)
        {
            currentPlayerStatus->mainTime--;
            sprintf(tmp, "%s time left : %u...\n",
                currentPlayerStatus->name.c_str(),
                currentPlayerStatus->mainTime);
            print(tmp);
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        else
        {
            if(currentPlayerStatus->extraRound)
            {
                sprintf(tmp, "%s time left : %u. 還剩 %d 次讀秒...\n",
                    currentPlayerStatus->name.c_str(),
                    currentPlayerStatus->extraTime,
                    currentPlayerStatus->extraRound);
                print(tmp);

                if (currentPlayerStatus->extraTime <= 5)
                {
                    sprintf(tmp, "say -v Kyoko %d", currentPlayerStatus->extraTime);
                    system(tmp);
                }

                currentPlayerStatus->extraTime--;

                if(currentPlayerStatus->extraTime == 0)
                {
// TODO
// #ifndef FASTMODE
//                     if (currentPlayerStatus->fastModeExtraCount)
//                     {
//                         if (currentPlayerStatus->fastModeExtraTime)
//                         {
//                             currentPlayerStatus->fastModeExtraTime--;
//                             sprintf(tmp, "%s time left : %u. 還剩 %d 次讀秒...\n",
//                                 currentPlayerStatus->name.c_str(),
//                                 currentPlayerStatus->extraTime,
//                                 currentPlayerStatus->extraRound);
//                             print(tmp);
//                         }
//                         else
//                         {
//                             currentPlayerStatus->fastModeExtraCount--;
//                         }
//                     }
// #else
// #endif
                    currentPlayerStatus->extraRound--;
                    currentPlayerStatus->extraTime = EXTRATIME;
                    if(currentPlayerStatus->extraRound == 1)
                    {
                        system("say 最後一次讀秒");
                    }
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            }
            else
            {
                print(currentPlayerStatus->name + " lost, time-out");
                system("say 超時負");
                exit(0);
            }
        }
    }
}

void getKey()
{
    std::string str;
    while(1)
    {
        std::getline(std::cin, str);
        if (str != "pause")
        {
            gameState = gameState == GameState::BLACK ? GameState::WHITE : GameState::BLACK;
        }
        else
        {
            gameState = GameState::PAUSE;
        }
    }
}

int main()
{
    print("Press ENTER to start the game");
    system("say 按 enter 開始");

    std::string c; std::getline(std::cin, c);
    system("say 比賽開始");

    std::thread t1(count);
    std::thread t2(getKey);
    t1.join();
    t2.join();

    return 0;
}