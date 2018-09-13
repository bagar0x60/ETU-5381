from cvxopt import solvers, matrix
import numpy as np

solvers.options['show_progress'] = False
solvers.options['feastol'] = 10**(-10)

E = np.array(
    [[1, 0, 0, 0, 0, 0],
    [-1, 1, 1, 1, 0, 0],
    [0, -1, 0, 0, 1, 1]]
)
e = np.array([1, 0, 0]).reshape((3, 1))

F = np.array(
    [[1, 0, 0, 0, 0, 0],
    [-1, 1, 1, 0, 0, 0],
    [-1, 0, 0, 1, 1, 1]]
)
f = np.array([1, 0, 0]).reshape((3, 1))

A_ = np.array(
    [[0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0],
    [0, 0, 0, 1, 2, 3],
    [0, 0, 0, 0, 0, 0],
    [0, 10, -8, 0, 0, 0],
    [0, -11, 14, 0, 0, 0]]
)

# first player

c = np.vstack( (np.zeros((6, 1)), f) )

G1 = np.hstack( (-A_.T, -F.T) )
G2 = np.hstack( (-np.eye(6), np.zeros( (6, 3) )) )
G = np.vstack( (G1, G2) )
h = np.zeros( (12, 1) )

A = np.hstack( (E, np.zeros( (3, 3) )) )
b = e

c = matrix(c, tc='d')
G = matrix(G, tc='d')
h = matrix(h, tc='d')
A = matrix(A, tc='d')
b = matrix(b, tc='d')

x = solvers.lp(c, G, h, A, b)['x'][:6]
print(x)  # [1.000, 0.999, 2.294e-07, -8.409-09, 0.581, 0.418]

# second player

c = np.vstack( (np.zeros((6, 1)), e) )

G1 = np.hstack( (A_, -E.T) )
G2 = np.hstack( (-np.eye(6), np.zeros( (6, 3) )) )
G = np.vstack( (G1, G2) )
h = np.zeros( (12, 1) )

A = np.hstack( (F, np.zeros( (3, 3) )) )
b = f

c = matrix(c, tc='d')
G = matrix(G, tc='d')
h = matrix(h, tc='d')
A = matrix(A, tc='d')
b = matrix(b, tc='d')

y = solvers.lp(c, G, h, A, b)['x'][:6]
print(y)  # [0.999, 0.511, 0.488, 0.878, 0.081, 0.0393]

print(x.T @ A_ @ y)  # 1.20930232