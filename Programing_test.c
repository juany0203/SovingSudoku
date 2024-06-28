#include <locale.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZE 16  // 16x16 크기의 스도쿠 퍼즐을 정의

int sudoku[SIZE][SIZE];  // 전역 변수로 스도쿠 퍼즐을 저장할 2차원 배열 선언

// 숫자 스도쿠 퍼즐을 알파벳 스도쿠 퍼즐로 변환하는 함수
void changeToAlphabet(char sudoku_alphabet[SIZE][SIZE], int sudoku[SIZE][SIZE]) {
    // 숫자와 알파벳 매핑 (0-9는 숫자, 10-15는 A-F, 16은 G로 표현)
    char kwangjae[17] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', 'G'};

    // 16x16 스도쿠 배열을 순회하며 숫자를 대응하는 알파벳으로 변환
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (sudoku[i][j] >= 0 && sudoku[i][j] < 17) {
                sudoku_alphabet[i][j] = kwangjae[sudoku[i][j]];  // 숫자를 알파벳으로 변환하여 저장
            }
        }
    }
}

// 숫자 스도쿠 퍼즐을 출력하는 함수
void print_sudoku(int sudoku[SIZE][SIZE]) {
    // 16x16 스도쿠 배열을 순회하며 각 숫자를 출력
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("%2d ", sudoku[i][j]);  // 각 숫자를 2칸의 공간으로 출력
        }
        printf("\n");  // 한 행이 끝나면 줄 바꿈
    }
}

// 알파벳 스도쿠 퍼즐을 출력하는 함수
void print_sudoku_alphabet(char sudoku_alphabet[SIZE][SIZE]) {
    // 16x16 스도쿠 배열을 순회하며 각 알파벳을 출력
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("%2c ", sudoku_alphabet[i][j]);  // 각 알파벳을 2칸의 공간으로 출력
        }
        printf("\n");  // 한 행이 끝나면 줄 바꿈
    }
}

// 현재 위치(row, col)에 num을 놓을 수 있는지 확인하는 함수
int is_valid(int row, int col, int num) {
    // 같은 행 또는 열에 num이 있는지 확인
    for (int x = 0; x < SIZE; x++) {
        if (sudoku[row][x] == num || sudoku[x][col] == num) {
            return 0;  // 같은 행 또는 열에 num이 있으면 0 반환 (유효하지 않음)
        }
    }

    // 현재 위치가 속한 4x4 박스의 시작점을 계산
    int startRow = row - row % 4, startCol = col - col % 4;
    // 4x4 박스 내에 num이 있는지 확인
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (sudoku[i + startRow][j + startCol] == num) {
                return 0;  // 같은 4x4 박스 내에 num이 있으면 0 반환 (유효하지 않음)
            }
        }
    }
    return 1;  // 현재 위치에 num을 놓아도 문제가 없으면 1 반환 (유효함)
}

// 스도쿠 퍼즐을 해결하는 함수
int solve_sudoku() {
    int row = -1, col = -1;  // 아직 빈 칸을 찾지 못한 상태를 나타내는 초기값
    int is_empty = 1;  // 빈 칸이 있는지 여부를 나타내는 플래그

    // 스도쿠 배열을 순회하여 빈 칸(값이 0인 칸)을 찾음
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (sudoku[i][j] == 0) {
                row = i;
                col = j;
                is_empty = 0;  // 빈 칸을 찾으면 플래그를 0으로 설정하고 루프 종료
                break;
            }
        }
        if (!is_empty) {
            break;
        }
    }

    // 빈 칸이 없으면 스도쿠가 해결된 것으로 간주하고 1 반환
    if (is_empty) {
        return 1;
    }

    // 1부터 16까지의 숫자를 순회하며 빈 칸에 놓을 수 있는지 확인
    for (int num = 1; num <= SIZE; num++) {
        if (is_valid(row, col, num)) {
            sudoku[row][col] = num;  // 유효한 숫자를 빈 칸에 놓음
            if (solve_sudoku()) {
                return 1;  // 재귀적으로 스도쿠를 해결
            }
            sudoku[row][col] = 0;  // 스도쿠를 해결하지 못하면 빈 칸으로 되돌림
        }
    }
    return 0;  // 스도쿠를 해결할 수 없으면 0 반환
}

int main() {
    setlocale(LC_ALL, "");  // 로케일 설정 (출력 형식을 위해 사용)

    // 스도쿠 퍼즐을 저장한 파일 열기
    FILE *file = fopen("sudoku.txt", "r");
    if (!file) {
        printf("파일을 열 수 없습니다.\n");
        fflush(stdout);  // 출력 버퍼를 비움
        return 1;  // 파일을 열 수 없으면 프로그램 종료
    }

    printf("파일을 성공적으로 열었습니다.\n");
    fflush(stdout);

    // 파일에서 16x16 스도쿠 배열을 읽어옴
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (fscanf(file, "%d", &sudoku[i][j]) != 1) {
                printf("파일 읽기 오류\n");
                fflush(stdout);
                fclose(file);  // 파일을 닫고
                return 1;  // 프로그램 종료
            }
        }
    }

    fclose(file);  // 파일 닫기

    printf("입력된 스도쿠 문제:\n");
    fflush(stdout);
    print_sudoku(sudoku);  // 입력된 스도쿠 퍼즐 출력
    printf("\n");

    char sudoku_alphabet[SIZE][SIZE];
    changeToAlphabet(sudoku_alphabet, sudoku);  // 숫자 스도쿠를 알파벳으로 변환
    printf("알파벳으로 변환된 입력된 스도쿠 문제:\n");
    fflush(stdout);
    print_sudoku_alphabet(sudoku_alphabet);  // 알파벳 스도쿠 퍼즐 출력
    printf("\n");

    // 스도쿠 퍼즐 해결 시도
    if (solve_sudoku()) {
        printf("해결된 스도쿠 문제:\n");
        fflush(stdout);
        print_sudoku(sudoku);  // 해결된 스도쿠 퍼즐 출력
        printf("\n");

        changeToAlphabet(sudoku_alphabet, sudoku);  // 해결된 스도쿠를 알파벳으로 변환
        printf("알파벳으로 변환된 해결된 스도쿠 문제:\n");
        fflush(stdout);
        print_sudoku_alphabet(sudoku_alphabet);  // 알파벳으로 변환된 해결된 스도쿠 퍼즐 출력
    } else {
        printf("풀 수 없는 스도쿠 문제입니다.\n");
        fflush(stdout);  // 스도쿠를 해결할 수 없을 때 메시지 출력
    }

    return 0;  // 프로그램 종료
}
