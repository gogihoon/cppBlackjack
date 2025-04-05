## 1. **주요 클래스 및 함수**

### 1.1 **`Card` 클래스**
- **역할**: 카드의 문양과 숫자를 나타냅니다.
- **주요 메서드**:
  - `getSuit()`: 카드의 문양(♠, ♥, ♦, ♣) 반환.
  - `getRank()`: 카드의 숫자(A, 2~10, J, Q, K) 반환.
  - `toString()`: 문양과 숫자를 결합하여 문자열로 반환.

---

### 1.2 **`Deck` 클래스**
- **역할**: 카드 덱을 관리합니다.
- **주요 메서드**:
  - `drawCard(int owner)`: 랜덤으로 카드를 뽑아 소유자(플레이어/딜러)에게 할당.
  - `showDeck(int owner)`: 특정 소유자의 카드 목록 출력.

---

### 1.3 **`Player` 클래스 (템플릿)**
- **역할**: 플레이어와 딜러의 공통 기능을 정의합니다.
- **주요 속성**:
  - `hand`: 플레이어가 가진 카드 목록.
  - `points`: 현재 점수.
  - `money`: 플레이어의 자산.
- **주요 메서드**:
  - `initialize()`: 카드와 점수를 초기화.
  - `betting()`: 베팅 금액 입력.
  - `calculatePoints()`: 카드 점수를 계산.
  - `wantsToHit()`: 카드를 더 뽑을지 결정 (순수 가상 함수).

---

### 1.4 **`HumanPlayer` 클래스**
- **역할**: 사람 플레이어의 행동을 정의합니다.
- **주요 메서드**:
  - `draw(Deck& deck)`: 덱에서 카드를 뽑아 손에 추가.
  - `wantsToHit()`: 사용자 입력을 통해 카드를 더 뽑을지 결정.

---

### 1.5 **`DealerPlayer` 클래스**
- **역할**: 딜러의 행동을 정의합니다.
- **주요 메서드**:
  - `draw(Deck& deck)`: 덱에서 카드를 뽑아 손에 추가.
  - `wantsToHit()`: 점수가 17 미만일 경우 카드를 더 뽑음.

---

### 1.6 **`displayBothCards()` 함수**
- **역할**: 플레이어와 딜러의 카드를 출력합니다.

---

## 2. **`main()` 함수**
- **역할**: 게임의 전체 흐름을 제어합니다.
- **주요 흐름**:
  1. **초기화**:
     - 랜덤 시드 설정.
     - `Deck`, `HumanPlayer`, `DealerPlayer` 객체 생성.
  2. **게임 루프**:
     - 플레이어와 딜러의 자산 출력.
     - 게임 지속 여부 입력 (`y/n`).
     - 베팅 금액 입력.
     - 초기 카드 2장씩 뽑기.
     - 블랙잭 여부 확인.
  3. **카드 뽑기**:
     - 플레이어와 딜러가 번갈아 가며 카드를 뽑음.
     - 플레이어는 입력(`y/n`)으로 결정, 딜러는 점수에 따라 자동 결정.
  4. **결과 계산**:
     - 점수 비교 후 승패 결정.
     - 베팅 금액에 따라 자산 업데이트.
  5. **초기화**:
     - 덱과 플레이어 상태 초기화 후 다음 게임으로 진행.

---

## 3. **게임 규칙 구현**
- **점수 계산**:
  - 숫자 카드: 해당 숫자.
  - J, Q, K: 10점.
  - A: 플레이어는 1 또는 11 선택, 딜러는 자동 계산.
- **승패 조건**:
  - 21 초과 시 버스트.
  - 점수 비교로 승자 결정.
  - 블랙잭(첫턴 21점) 시 1.5배 보너스 지급.

---
