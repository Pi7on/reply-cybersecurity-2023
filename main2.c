#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 5

typedef struct {
    int val;
    char cell_region;
} Cell;

typedef struct {
    char region;
    int result;
    char operation;
} Operation;

bool isdigit(char c) {
    return c >= '0' && c <= '9';
}

int parse_operations(const char operations[], Operation ops[]) {
    int index = 0;
    int ops_count = 0;

    while (operations[index] != '\0') {
        ops[ops_count].region = operations[index++];
        while (operations[index] == ' ') {
            index++;
        }  // skip spaces

        // Parse the integer result
        int num = 0;
        while (isdigit(operations[index])) {
            num = num * 10 + (operations[index++] - '0');
        }
        ops[ops_count].result = num;

        while (operations[index] == ' ') {
            index++;
        }  // skip spaces

        ops[ops_count].operation = operations[index++];

        while (operations[index] == ' ' || operations[index] == '\n') {
            index++;
        }

        ops_count++;
    }

    return ops_count;  // Return the number of parsed operations
}

bool region_is_complete(Cell grid[N][N], char region) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (grid[i][j].cell_region == region && grid[i][j].val == 0) {
                return false;  // Found an unfilled cell in the region
            }
        }
    }
    return true;
}

bool region_is_valid(Cell grid[N][N], char region, int result, char operation) {
    int sum = 0, product = 1, maxVal = -1, minVal = N + 1;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (grid[i][j].cell_region == region) {
                if (grid[i][j].val == 0) {
                    switch (operation) {
                        case '+':
                        case '*':
                            return false;
                        default:
                            break;
                    }
                }

                int value = grid[i][j].val;
                sum += value;
                product *= value;

                if (value > maxVal) maxVal = value;
                if (value < minVal) minVal = value;
            }
        }
    }

    int computedValue;
    switch (operation) {
        case '+':
            computedValue = sum;
            break;
        case '*':
            computedValue = product;
            break;
        case '-':
            computedValue = maxVal - minVal;
            break;
        case '/':
            computedValue = maxVal / minVal;
            break;
        default:
            return false;
    }

    // Debug print
    // printf("Checking region %c with operation %c, computed value = %d, expected result = %d\n", region, operation, computedValue, result);

    return computedValue == result;
}

void grid_print(Cell grid[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            // printf("%d(%c) ", grid[i][j].val, grid[i][j].cell_region);
        }
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d", grid[i][j].val);
        }
    }
}

bool can_place_num(Cell grid[N][N], int row, int col, int num) {
    for (int x = 0; x < N; x++) {
        if (grid[row][x].val == num || grid[x][col].val == num) {
            return false;
        }
    }
    return true;
}

bool grid_is_valid(Cell grid[N][N], Operation ops[], int opsCount) {
    for (int i = 0; i < opsCount; i++) {
        if (!region_is_valid(grid, ops[i].region, ops[i].result, ops[i].operation)) {
            return false;
        }
    }
    return true;
}

bool grid_solve(Cell grid[N][N], Operation ops[], int opsCount) {
    int row = -1, col = -1;
    bool isEmpty = true;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (grid[i][j].val == 0) {
                row = i;
                col = j;
                isEmpty = false;
                break;
            }
        }
        if (!isEmpty) {
            break;
        }
    }

    if (isEmpty) {
        return grid_is_valid(grid, ops, opsCount);
    }

    for (int num = 1; num <= N; num++) {
        if (can_place_num(grid, row, col, num)) {
            grid[row][col].val = num;
            if (grid_solve(grid, ops, opsCount)) {
                return true;
            }
            grid[row][col].val = 0;
        }
    }
    return false;
}
void grid_fill(Cell grid[N][N], Operation ops[], int opsCount) {
    if (!grid_solve(grid, ops, opsCount)) {
        printf("No solution exists.\n");
    }
}

void grid_assign_regions(Cell grid[N][N], const char regions[]) {
    int index = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            while (regions[index] == ' ' || regions[index] == '\n') {
                index++;
            }
            grid[i][j].cell_region = regions[index];
            index++;
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <filename.txt>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        printf("Failed to open file");
        return 1;
    }

    char line[300];  // Assuming no line is longer than 100 characters

    // Read regions
    char regions[300] = {0};  // Including spaces and newline characters
    for (int i = 0; i < N; i++) {
        fgets(line, sizeof(line), file);
        strcat(regions, line);  // Append the line to regions
    }
    fgets(line, sizeof(line), file);  // Consume the empty line

    // Read operations
    char operations[2000];  // Assuming operations input is no longer than 200 characters
    char *op_ptr = operations;

    while (fgets(line, sizeof(line), file) != NULL) {
        strcpy(op_ptr, line);
        op_ptr += strlen(line);
    }

    fclose(file);

    Cell c;
    c.cell_region = 'z';
    c.val = 0;
    Cell grid[N][N] = {{c}};

    Operation ops[1000];
    int ops_count = parse_operations(operations, ops);

    grid_assign_regions(grid, regions);
    grid_fill(grid, ops, ops_count);
    grid_print(grid);

    return 0;
}