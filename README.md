# StopWatch_Watch_Project

## 요구사항

### 1. 모드 전환 (시계 ↔ StopWatch)

- `btn_mode`(PC0)를 누를 때마다 시계 ↔ StopWatch 전환
- 시작 모드: StopWatch
- 구현: `fnd_change_mode()`의 상태 머신 (WATCH / STOPWATCH)

### 2. 버튼 (3개)

| 버튼 | 핀 | StopWatch 모드 | 시계 모드 |
|---|---|---|---|
| `btn_mode` | PC0 | 시계로 전환 | StopWatch로 전환 |
| `btn_run_stop` | PC1 | 시작 / 정지 | (추가 기능) 시:분 ↔ 분:초 |
| `btn_clear` | PC3 | 카운터 초기화 (STOP 상태에서만) | 사용 안 함 |

### 3. LED (4개)

| LED | 핀 | 의미 |
|---|---|---|
| 시계 | PB15 | 시계 모드일 때 ON |
| StopWatch | PB14 | StopWatch 모드일 때 ON |
| stop | PB13 | StopWatch가 STOP일 때 ON |
| run | PC4 | StopWatch가 RUN일 때 ON |

### 4. StopWatch

**상태도**
- 시작 → STOP
- STOP → RUN: `btn_run_stop`
- RUN → STOP: `btn_run_stop`
- STOP → CLEAR: `btn_clear`
- CLEAR → STOP: 카운터 0으로 만든 뒤 자동 복귀

**동작**
- RUN 상태에서 0.1초마다 카운터 1 증가 (0000 ~ 9999, 넘으면 0으로)
- STOP 상태에서는 현재 값 유지

### 5. 시계 기능

**FND 표시**
- 4자리 중 왼쪽 2자리 = **분**, 오른쪽 2자리 = **초**
- 2번째 자리의 점(DP)을 **0.5초 ON / 0.5초 OFF**로 깜빡임
- 초가 바뀌는 순간 점이 켜지도록 맞춤

**시계용 타이머 (TIM2)**
- 클럭 100MHz, PSC `10000-1`, ARR `5000-1` → **0.5초마다 인터럽트**
- NVIC에서 TIM2 global interrupt 활성화
- `HAL_TIM_Base_Start_IT(&htim2)`로 시작

**인터럽트로 시간 증가**
- 콜백에서 `watch_interrupt()` 호출
- 0.5초마다 `half` 토글 → 점 ON/OFF
- `half`가 0으로 돌아올 때(1초) 초 증가
- 60초 → 분 증가, 60분 → 시 증가, 24시 → 0시

**FND 드라이버 수정**
- `fnd_set_dp()` 추가
- `fnd_disp_number()`에 DP 인자 추가, 2번째 자리에만 적용

### 6. 타이머 역할 분담

| 타이머 | 주기 | 역할 |
|---|---|---|
| TIM3 | 1ms | FND 다이나믹 구동 (자리 전환) |
| TIM2 | 0.5초 | 시계 시간 증가, 점 토글 |
| SysTick | 1ms | StopWatch 0.1초 측정 (`HAL_GetTick`) |

### 7. 제출

- 동작 영상 업로드
- 영상에 보여줄 것: 모드 전환, StopWatch의 run/stop/clear, 시계의 분·초 증가와 점 깜빡임, 각 상태의 LED

### 추가 기능 (요구사항 외)

- 시계 모드에서 `btn_run_stop`(PC1)으로 시:분 ↔ 분:초 표시 전환
- 표시 모드를 PB13, PC4 LED로 표시


## System Architecture
<img width="827" height="767" alt="image" src="https://github.com/user-attachments/assets/6cd5713a-25d7-403e-982e-fb29d94c5604" />



