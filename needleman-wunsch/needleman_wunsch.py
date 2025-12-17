import numpy as np


class GlobalAlignment:
    def __init__(self, match_score: int, mismatch_score: int, gap_score: int) -> None:
        self._match_score = match_score
        self._mismatch_score = mismatch_score
        self._gap_score = gap_score

    def _score(self, a, b):
        if (a == "-" and b != "-") or (b == "-" and a != "-"):
            return self._gap_score  # gap
        if a == b:
            return self._match_score  # match
        if a != b:
            return self._mismatch_score  # mismatch

    def needleman_wunsch(self, a: str, b: str):
        ag = "-" + a
        bg = "-" + b
        score_matrix = np.zeros((len(ag), len(bg)), dtype=int)
        # score_matrix[0, 0] = 0 # just for completeness
        dir_table = {}
        # init of first row and first column
        for i in range(1, score_matrix.shape[0]):
            score_matrix[i, 0] = score_matrix[i - 1, 0] + self._score(ag[i], bg[0])
            dir_table[i, 0] = [1]
        for j in range(1, score_matrix.shape[1]):
            score_matrix[0, j] = score_matrix[0, j - 1] + self._score(ag[0], bg[j])
            dir_table[0, j] = [2]
        # fill score matrix
        for i in range(1, score_matrix.shape[0]):
            for j in range(1, score_matrix.shape[1]):
                possible_dirs = {
                    0: score_matrix[i - 1, j - 1] + self._score(ag[i], bg[j]),
                    1: score_matrix[i - 1, j] + self._gap_score,
                    2: score_matrix[i, j - 1] + self._gap_score,
                }
                score_matrix[i, j] = max(possible_dirs.values())
                dir_table[(i, j)] = [
                    dir
                    for dir, value in possible_dirs.items()
                    if value == score_matrix[i, j]
                ]

        alignments = self._traceback(ag, bg, dir_table, len(a), len(b))
        return score_matrix, alignments, score_matrix[-1, -1]

    def _traceback(self, a, b, dir_table, i, j):
        # Base case: start of matrix
        if i == 0 and j == 0:
            return [("", "")]

        alignments = []

        for direction in dir_table[(i, j)]:
            if direction == 0:  # diagonal
                prev = self._traceback(a, b, dir_table, i - 1, j - 1)
                for x, y in prev:
                    alignments.append((x + a[i], y + b[j]))

            elif direction == 1:  # up
                prev = self._traceback(a, b, dir_table, i - 1, j)
                for x, y in prev:
                    alignments.append((x + "-", y + b[j]))

            elif direction == 2:  # left
                prev = self._traceback(a, b, dir_table, i, j - 1)
                for x, y in prev:
                    alignments.append((x + a[i], y + "-"))

        return alignments


if __name__ == "__main__":
    # needleman_wunsch("GATTACA", "GCATGCU")
    examples = [("GATTACA", "GCATGCU"), ("ACGT", "ACCT"), ("ATGCT", "AGCT")]
    for a, b in examples:
        ga = GlobalAlignment(1, -1, -2)
        score_matrix, alignements, final_score = ga.needleman_wunsch(a, b)
        print(f"Alignment for {a} and {b}:")
        print("Score Matrix:\r\n", score_matrix)
        print("Optimal global alignments:")
        for i, alignment in enumerate(alignements):
            print(f"#{i}:", alignment)
        print("Final score:", final_score)
