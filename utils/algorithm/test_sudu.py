# -*- coding: utf-8 -*-

# 精确覆盖 给定全集 X 和 X 的子集的集合 Y ，存在一个 Y 的子集 Y*，使得 Y* 构成 X 的一种分割。


def solve(X, Y, solution=[]):
    if not X:
        yield list(solution)
    else:
        c = min(X, key=lambda c: len(X[c]))
        for r in list(X[c]):
            solution.append(r)
            cols = select(X, Y, r)
            for s in solve(X, Y, solution):
                yield s
            deselect(X, Y, r, cols)
            solution.pop()


def select(X, Y, r):
    cols = []
    for j in Y[r]:
        for i in X[j]:
            for k in Y[i]:
                if k != j:
                    X[k].remove(i)
        cols.append(X.pop(j))
    return cols


def deselect(X, Y, r, cols):
    for j in reversed(Y[r]):
        X[j] = cols.pop()
        for i in X[j]:
            for k in Y[i]:
                if k != j:
                    X[k].add(i)


if __name__ == '__main__':
    X = {1, 2, 3, 4, 5, 6, 7}
    Y = {
        'A': [1, 4, 7],
        'B': [1, 4],
        'C': [4, 5, 7],
        'D': [3, 5, 6],
        'E': [2, 3, 6, 7],
        'F': [2, 7]}

    # 字典来代替双向链表来表示矩阵。我们已经有了 Y。从它那我们能快速的访问每行的列元素。
    # 现在我们还需要生成行的反向表，换句话说就是能从列中快速访问行元素。为实现这个目的，我们把X转换为字典。
    # 在上述的例子中，它应该写为
    # X = {
    #     1: {'A', 'B'},
    #     2: {'E', 'F'},
    #     3: {'D', 'E'},
    #     4: {'A', 'B', 'C'},
    #     5: {'C', 'D'},
    #     6: {'D', 'E'},
    #     7: {'A', 'C', 'E', 'F'}}

    X = {j: set() for j in X}
    for i in Y:
        for j in Y[i]:
            X[j].add(i)
    print X
    solution = []
    solve(X, Y, solution=[])
    print solution

