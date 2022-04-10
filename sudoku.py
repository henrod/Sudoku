class Sudoku:
    def __init__(self, matrix: list[list[int]]) -> None:
        self._size = 9
        self._matrix = matrix

    def __str__(self) -> str:
        cell_size = self._size // 3
        r: list[str] = ['-'] * 97 + ['\n']

        for i, row in enumerate(self._matrix):
            r.append('|')
            for j, number in enumerate(row):
                r.append(f'\t{number}')
                if (j + 1) % cell_size == 0:
                    r.append('\t|')
            r.append('\n')

            if (i + 1) % cell_size == 0:
                r.append('-' * 97)

            r.append('\n')

        return ''.join(r)

    def _row_numbers(self, row: int) -> set[int]:
        return set(self._matrix[row])

    def _col_numbers(self, col: int) -> set[int]:
        return set(row[col] for row in self._matrix)

    def _cell_numbers(self, row: int, col: int) -> set[int]:
        cell_size = self._size // 3
        cell_row = row // cell_size * cell_size
        cell_col = col // cell_size * cell_size
        return set(
            self._matrix[cell_row + i][cell_col + j]
            for i in range(cell_size)
            for j in range(cell_size)
        )

    def _possible_numbers(self, row: int, col: int) -> set[int]:
        all_numbers = set(i for i in range(1, self._size + 1))

        possible_row_numbers = all_numbers - self._row_numbers(row)
        possible_col_numbers = all_numbers - self._col_numbers(col)
        possible_cell_numbers = all_numbers - self._cell_numbers(row, col)

        return possible_row_numbers & possible_col_numbers & possible_cell_numbers

    def _empty_spots(self) -> list[tuple[int, int]]:
        return [
            (row_index, col_index)
            for row_index, row in enumerate(self._matrix)
            for col_index, number in enumerate(row)
            if number == 0
        ]

    def _solve(self, spots: list[tuple[int, int]], spot_index: int) -> bool:
        if spot_index >= len(spots):
            return True

        (row, col) = spots[spot_index]
        possible_numbers = self._possible_numbers(row, col)

        for possible_number in possible_numbers:
            self._matrix[row][col] = possible_number
            solved = self._solve(spots, spot_index + 1)
            if solved:
                return True

        self._matrix[row][col] = 0
        return False

    def solve(self) -> list[list[int]]:
        spots = self._empty_spots()
        self._solve(spots, 0)
        return self._matrix


if __name__ == '__main__':
    n_cases = int(input())
    for case in range(1, n_cases + 1):
        _ = input()
        print(f"Case #{case}:")
        matrix: list[list[int]] = [
            [int(number_str) for number_str in input()]
            for _ in range(9)
        ]

        sudoku = Sudoku(matrix)

        print(sudoku)
        sudoku.solve()
        print(sudoku)
