import matplotlib.pyplot as plt
from matplotlib.patches import Polygon

from cvxopt import solvers, matrix
import numpy as np

solvers.options['show_progress'] = False
solvers.options['feastol'] = 10**(-10)

def distance(t):
    G = np.array(
        [[1, 1, 0, 0],
        [-1, -1, 0, 0],
        [1, 0, 0, 0],
        [-1, 0, 0, 0],
        [0, 1, 0, 0],
        [0, -1, 0, 0]])

    h = 6*[t]

    A = [0, 0, 1, 2]
    b = [10]

    P = 2*np.kron([[1, -1], [-1, 1]], np.eye(2))
    q = 4*[0]

    P = matrix(P, tc='d')
    q = matrix(q, tc='d')
    G = matrix(G, tc='d')
    h = matrix(h, tc='d')
    A = matrix(A, (1, 4), tc='d')
    b = matrix(b, tc='d')

    sol = solvers.qp(P, q, G, h, A, b)

    x = np.array(sol['x']).reshape((4, 1))
    return (0.5 * x.T @ P @ x)[0][0]

def is_almost_zero(x):
    return abs(x) < 10**(-9)

# binary search

left = 0
right = 100
eps = 10**(-5)


while  right - left >= eps:
    mid = (right + left) / 2
    if is_almost_zero( distance(mid) ):
        right = mid
    else:
        left = mid
t = mid

print(t)  # 5.0001

fig, ax = plt.subplots()
ax.add_patch(
    Polygon([[-t, t], [0, t], [t, 0], [t, -t], [0, -t], [-t, 0]], True)
)
xs = list(range(-11, 11))
ys = [-0.5*i + 5 for i in xs]
plt.plot(xs, ys, 'g')
plt.plot([0], [5], 'ro')

ax.set_xlim(-10, 10)
ax.set_ylim(-10, 10)
plt.show()
