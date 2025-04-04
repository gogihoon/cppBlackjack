#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
using namespace std;

// 카드의 문양과 숫자를 나타내는 클래스
class Card {
    int suit, rank; // suit: 문양, rank: 숫자
public:
    Card(int s, int r) : suit(s), rank(r) {}

    string getSuit() const {
        switch (suit) {
            case 0: return "♠"; // 스페이드
            case 1: return "♥"; // 하트
            case 2: return "♦"; // 다이아몬드
            case 3: return "♣"; // 클럽
            default: return "?"; // 잘못된 값
        }
    }

    string getRank() const {
        if (rank == 0) return "A"; // Ace
        if (rank >= 1 && rank <= 9) return to_string(rank + 1); // 숫자 카드
        if (rank == 10) return "J"; // Jack
        if (rank == 11) return "Q"; // Queen
        if (rank == 12) return "K"; // King
        return "?"; // 잘못된 값
    }

    string toString() const {
        return getSuit() + getRank(); // 문양 + 숫자
    }
};

// 덱 클래스
class Deck {
    int  cards[4][13] = {0}; // 총 52장
public:
    // 덱을 초기화하는 생성자
    Deck() {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 13; j++) {
                cards[i][j] = 0;
            }
        }
    }
    // 카드 뽑기 함수
    int drawCard(int owner) {
        int randomNumber;
        do {
            randomNumber = rand() % 52;
        } while (cards[randomNumber / 13][randomNumber % 13] > 0); // 이미 뽑힌 카드인지 확인
        cards[randomNumber / 13][randomNumber % 13] = owner;
        
        return randomNumber % 13;
    }
    // 카드 덱을 출력하는 함수
    void showDeck(int owner) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 13; j++) {
                if(cards[i][j] == owner) {
                    Card c(i, j);
                    cout << c.toString() << " ";
                }
            }
        }
    }
};

// 플레이어 클래스
template <typename T>
class Player {
protected:
    vector<int> hand;
    int points;
    bool isHuman;
    T money;
public:
    // isHuman: true면 플레이어, false면 딜러
    Player(bool isHuman, T initailMoney=0):isHuman(isHuman),money(initailMoney){}
    // 플레이어의 카드와 점수를 초기화하는 함수
    void initialize() {
        hand={};
        points = 0;
    }
    // 베팅 금액을 입력받는 함수
    T bet;
    void betting() {
        bet=0;
        while (1)
        {
            cout << "베팅 금액을 입력하세요: ";
            cin >> bet;
            if (bet > money) {
                cout << "베팅 금액이 부족합니다." << endl;
            }else
                break;
        }
    }
    // 플레이어의 돈을 업데이트하는 함수
    T operator+(T amount) {
        money += amount;
        return money;
    }
    // 카드 뽑기 함수는 자식 클래스에서 구현
    virtual void draw(Deck& deck) {}
    // 점수를 계산하는 함수
    virtual void calculatePoints() {
        points = 0;
        for (int i : hand) {
            if(isHuman) { // 플레이어일때
                if (i >= 10) {
                    points += 10;
                } else if (i == 0) {
                    while (1) {
                        cout << "A가 나왔습니다.11로 계산할까요?(y/n)" << endl; // A일때 1과 11 중 선택
                        char input;
                        cin >> input;
                        if (input == 'y') {
                            points += 11;
                            break;
                        } else if (input == 'n') {
                            points += 1;
                            break;
                        } else {
                            cout << "잘못된 입력입니다." << endl;
                            continue; // 잘못된 입력 시 다시 선택
                        }
                    }
                } else {
                    points += (i + 1);
                }
            } else { // 딜러일때
                if (i >= 10) {
                    points += 10;
                } else if (i == 0) {
                    if (points + 11 > 21) {
                        points += 1;
                    } else {
                        points += 11;
                    }
                } else {
                    points += (i + 1);
                }
            }
        }
    }
    // 플레이어의 돈을을 반환하는 함수
    T getMoney() const { return money; }
    // 플레이어의 점수를 반환하는 함수
    int getPoints() const { return points; }

    virtual bool wantsToHit() const = 0; // 플레이어가 카드를 더 뽑을지 결정하는 함수
};
// 플레이어 클래스의 자식 클래스 중 사람람
template <typename T>
class HumanPlayer : public Player<T> {
public:
    // 생성자에서 돈을 초기화
    HumanPlayer(T initialMoney): Player<T>(true, initialMoney) {}
    // 카드를 뽑는 함수
    void draw(Deck& deck) override {
        int card = deck.drawCard(2);
        this->hand.push_back(card);
    }
    // 카드를 더 뽑을지 결정하는 함수
    // y를 입력하면 true, n을 입력하면 false를 반환
    bool wantsToHit() const override {
        char input;
        cout << "더할꺼? (y/n) : ";
        cin >> input;
        return (input == 'y');
    }
};
// 플레이어 클래스의 자식 클래스 중 딜러
template <typename T>
class DealerPlayer : public Player<T> {
public:
    // 생성자에서 돈을 초기화
    DealerPlayer(T initalMoney): Player<T>(false,initalMoney){}
    void draw(Deck& deck) override {
        int card = deck.drawCard(1);
        this->hand.push_back(card);
    }
    // 딜러는 17 미만일 때 카드를 뽑음
    bool wantsToHit() const override {
        return (this->getPoints() < 17);
    }
};
// 카드와 점수를 출력하는 함수
void displayBothCards(Deck& deck) {
    cout << "플레이어의 카드: ";
    deck.showDeck(2);
    cout << endl;
    cout << "딜러의 카드: ";
    deck.showDeck(1);
    cout << endl;
}

int main() {
    srand(static_cast<unsigned int>(time(0))); // 랜덤 시드 초기화
    Deck deck;
    HumanPlayer<double> player(100); // 플레이어 초기화
    DealerPlayer<double> dealer(1000); // 딜러 초기화
    // 게임 시작
    while(1) {
        cout << "플레이어의 자산: " << player.getMoney() << endl;
        cout << "딜러의 자산: " << dealer.getMoney() << endl;
        cout << "게임을 계속 하시겠습니까? (y/n): ";
        char input;
        cin >> input;
        if (input == 'n') {
            break; // 게임 종료
        } else if (input != 'y') {
            cout << "잘못된 입력입니다." << endl;
            continue; // 잘못된 입력 시 다시 시작
        }
        player.betting(); // 베팅 금액 입력
        
        
        // 초기 카드 뽑기 / 블랙잭은 첫턴에 2장씩 뽑음
        for (int i = 0; i < 2; ++i) {
            player.draw(deck);
            dealer.draw(deck);
        }
        displayBothCards(deck);
        player.calculatePoints();
        dealer.calculatePoints();
        if(player.getPoints() == 21) {
            cout << "플레이어 블랙잭, 플레이어가 이겼습니다." << endl;
            player + player.bet * 1.5; // 블랙잭 보너스
            dealer + (-player.bet * 1.5);
            deck = Deck(); // 덱 초기화
            player.initialize(); // 플레이어 초기화
            dealer.initialize(); // 딜러 초기화
            deck=Deck(); // 덱 초기화
            continue; // 다음 게임으로 넘어감
        }
        
        while (1) {
            system("cls"); // 화면 지우기
            
            cout << "플레이어의 점수: " << player.getPoints() << endl;
            cout << "딜러의 점수: " << dealer.getPoints() << endl;
            
            if(dealer.wantsToHit()) {
                dealer.draw(deck);
                dealer.calculatePoints();
            }
            if (player.getPoints() < 21) {
                if(player.wantsToHit()){
                    player.draw(deck);
                    player.calculatePoints();
                }
                else
                    break;
            } else
                break;
            displayBothCards(deck);
        }
        
        // 결과 출력
        displayBothCards(deck);
        cout << "최종 플레이어 점수: " << player.getPoints() << endl;
        cout << "최종 딜러 점수: " << dealer.getPoints() << endl;
        
        if (player.getPoints() > 21) {
            cout << "플레이어 버스트, 딜러가 이겼습니다." << endl;
            player + (-player.bet);
            dealer + player.bet;
        } else if (dealer.getPoints() > 21) {
            cout << "딜러 버스트, 플레이어가 이겼습니다." << endl;
            player + player.bet;
            dealer + (-player.bet);
        } else if (player.getPoints() > dealer.getPoints()) {
            cout << "플레이어가 이겼습니다." << endl;
            player + player.bet;
            dealer + (-player.bet);
        } else if (player.getPoints() < dealer.getPoints()) {
            cout << "딜러가 이겼습니다." << endl;
            player + (-player.bet);
            dealer + player.bet;
        } else {
            cout << "무승부입니다." << endl;
        }
        deck = Deck(); // 덱 초기화
        player.initialize(); // 플레이어 초기화
        dealer.initialize(); // 딜러 초기화
    }
    return 0;
}
