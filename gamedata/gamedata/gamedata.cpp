#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <Windows.h>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <cstdlib>

#define COLOR_GREEN 10
#define COLOR_RED 12
#define COLOR_GREY 7

// 0411 UV 추가 날짜 정확하게 나오기  RU 신규 유저수 MUV MTS
// 유저관련 지표 https://m.post.naver.com/viewer/postView.naver?volumeNo=28067435&memberNo=2647347

#define MAX_ENHANCEMENT_LEVEL 10

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void showMainMenu() {
    std::cout << "------------\n";
    std::cout << "메인 화면\n\n";
    std::cout << "1. 게임 시작\n";
    std::cout << "2. 게임 설명\n";
    std::cout << "3. 게임 종료\n";
    std::cout << "------------\n";
}

void showGameDescription() {
    std::cout << "-------------------------------------------------------------------------------\n";
    std::cout << "                                게임 설명\n\n";
    std::cout << "                엔터키를 누르면 강화가 되는 게임입니다.                    \n";
    std::cout << "        강화에 성공할수록 성공확률이 줄어들고 실패확률, 파괴확률이 올라갑니다.\n";
    std::cout << "    강화 레벨이 " << MAX_ENHANCEMENT_LEVEL << "에 도달하면 최대 강화 레벨에 도달했다는 메시지가 출력됩니다.\n";
    std::cout << "                장비가 파괴되면 게임이 종료됩니다.\n";
    std::cout << "-------------------------------------------------------------------------------\n";
}

void saveGameData(int enhancements, int successes, int failures, int csvEnhancements) {
    std::ofstream file("DATA/game_data.csv", std::ios::out | std::ios::app);
    if (file.is_open()) {
        // 파일이 비어있는 경우에만 헤더를 추가
        if (file.tellp() == 0) {
            file << "강화 횟수,성공 횟수,실패 횟수,강화 레벨\n";
        }
        // 각 데이터를 쉼표로 구분하여 한 행에 저장
        file << enhancements << "," << successes << "," << failures <<","<< csvEnhancements << "\n";
        file.close();
    }
    else {
        std::cerr << "Unable to open file for saving game data.\n";
    }
}

int main() {
    std::srand(std::time(nullptr));

    int enhancementLevel = 0; // 현재 강화 레벨 변수
    int successes = 0;
    int failures = 0;
    int upgradetry = 1; // 구글 시트에 강화 횟수 더해주는 변수
    int maxEnhancements = 0;
    int csvEnhancements = 0;
    int activeNum = 1; // 구글 시트 Apps Scripts 함수 제어 변수
    bool gameon = true; // while 문 통제 변수
    std::string username;
    // 현재시간 저장하는 변수
    auto now = std::chrono::system_clock::now();
    // 시간 포인터를 시간 구조체로 변환
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    struct std::tm* ptm = std::localtime(&now_time);
    // 날짜와 시간을 문자열로 변환하여 변수에 저장
    std::ostringstream oss;
    oss << std::put_time(ptm, "%Y.%m.%d %H시%M분%S초");
    std::string starttime = oss.str();  // 시작 시간 저장 변수
    // 구글 시트 Apps Scripts url
    std::string url = "https://script.google.com/macros/s/AKfycbwcZtJomdqP7A4iDwyOpD4PuQVS5oKzF2mHw5n7jbwbJGKFHG1u95QbVR-EpkWR7oXR/exec";
    // 커맨드로 보낼 데이터 변수
    std::string data;
    std::string command;
    int successChance[] = { 95, 85, 75, 60, 50, 35, 20, 10, 5, 2 };
    int destructionChance[] = { 0, 0, 0, 0, 1, 5, 10, 15, 20, 30 };

    int choice;
    bool once; // 버퍼를 첫 1회 및 장비 파괴 후 다시 시작할때만 지우기 위해 만든 변수

    std::cout << "유저명을 입력해주세요 : ";
    std::cin >> username;
    std::cout << "환영합니다. " << username << "님";
    Sleep(1000);
    system("cls");

    while (gameon) {
        showMainMenu();
        once = true;
        std::cout << "선택하세요: ";
        std::cin >> choice;

        switch (choice) {
        case 1:
            while (true) {
                successes = 0;
                failures = 0;

                std::cout << "-----------------------------------------\n";
                std::cout << "현재 강화 레벨: " << enhancementLevel << "\n";
                std::cout << "성공 확률 : " << successChance[enhancementLevel] << " % \n";
                std::cout << "실패 확률: " << 100 - destructionChance[enhancementLevel] - successChance[enhancementLevel] << "%\n";
                std::cout << "파괴 확률: " << destructionChance[enhancementLevel] << "%\n";
                std::cout << "강화하시겠습니까? (Enter 키로 강화) \n";
                std::cout << "-----------------------------------------\n";

                if (once)
                {
                    std::cin.ignore(); // 버퍼 비우기
                    once = false;
                }
                std::cin.get();    // 엔터키 입력 대기
                std::cout << "강화 중입니다...\n";
                now = std::chrono::system_clock::now(); // 시간 갱신
                now_time = std::chrono::system_clock::to_time_t(now);
                ptm = std::localtime(&now_time);
                std::ostringstream oss2;
                oss2 << std::put_time(ptm, "%Y.%m.%d %H시%M분%S초");
                std::string trytime = oss2.str();
                Sleep(1000);

                int result = std::rand() % 100;

                if (enhancementLevel < 5) {
                    if (result < successChance[enhancementLevel]) {
                        enhancementLevel++;
                        csvEnhancements++;
                        ++successes;
                        setColor(COLOR_GREEN);
                        std::cout << "\n강화 성공!\n";
                        setColor(COLOR_GREY);
                    }
                    else if (result >= successChance[enhancementLevel] && result < successChance[enhancementLevel] + destructionChance[enhancementLevel]) {
                        enhancementLevel = 0;
                        ++failures;
                        std::cout << "\n장비가 파괴되었습니다!\n";
                        break;
                    }
                    else {
                        ++failures;
                        setColor(COLOR_RED);
                        std::cout << "\n강화 실패!\n";
                        setColor(COLOR_GREY);
                    }
                }
                else {
                    if (result < successChance[enhancementLevel]) {
                        enhancementLevel++;
                        csvEnhancements++;
                        ++successes;
                        setColor(COLOR_GREEN);
                        std::cout << "\n강화 성공!\n";
                        setColor(COLOR_GREY);
                    }
                    else if (result >= successChance[enhancementLevel] && result < successChance[enhancementLevel] + destructionChance[enhancementLevel]) {
                        enhancementLevel = 0;
                        ++failures;
                        std::cout << "\n장비가 파괴되었습니다!\n";
                        break;
                    }
                    else {
                        enhancementLevel--;
                        csvEnhancements--;
                        ++failures;
                        setColor(COLOR_RED);
                        std::cout << "\n강화 실패!\n";
                        setColor(COLOR_GREY);

                        if (enhancementLevel < 0) {
                            std::cout << "\n장비가 파괴되었습니다!\n";
                            break;
                        }
                    }
                }

                // 커맨드로 보낼 데이터 입력
                data = "upgradetry=" + std::to_string(upgradetry) + "&username="+ username + "&starttime="+ starttime + "&trytime=" 
                    + trytime + "&enhancementLevel=" + std::to_string(enhancementLevel);
                command = "curl -X POST -d \"" + data + "\" \"" + url + "\"";
                // 보내고 이전내용 화면에서 지우기
                system(command.c_str());
                system("cls");

                if (enhancementLevel >= MAX_ENHANCEMENT_LEVEL) {
                    maxEnhancements++;
                    std::cout << "\n최대 강화 레벨에 도달했습니다!\n";
                    break;
                }
            }

            // Save game data to CSV file
            saveGameData(successes + failures, successes, failures, csvEnhancements);

            break;
        case 2:
            showGameDescription();
            std::cin.ignore(); // 버퍼 비우기
            std::cin.get();    // 엔터키 입력 대기
            break;
        case 3:
            std::cout << "게임을 종료합니다.\n";
            gameon = false;
            break;
        default:
            std::cout << "잘못된 선택입니다. 다시 선택하세요.\n";
            break;
        }
    }

    now = std::chrono::system_clock::now();
    now_time = std::chrono::system_clock::to_time_t(now);
    ptm = std::localtime(&now_time);

    std::ostringstream oss3;
    oss3 << std::put_time(ptm, "%Y.%m.%d %H시%M분%S초");
    std::string endtime = oss3.str();  // 시작 시간 저장 변수
    data = "upgradetry=" + std::to_string(upgradetry) + "&username=" + username + "&starttime=" + starttime + "&endtime=" + endtime + "&activeNum=" + std::to_string(activeNum);
    command = "curl -X POST -d \"" + data + "\" \"" + url + "\"";
    system(command.c_str());

    return 0;
}
