import matplotlib.pyplot as plt
from matplotlib.patches import Polygon

from cvxopt import solvers, matrix
import numpy as np

phi = np.array([2, 1]).reshape((2, 1))
y = np.array([1, 1]).reshape((2, 1))
gamma = 1

m, n = phi.shape

c = m*[1] + n*[0]

Im, In = np.eye(m), np.eye(n)
zero = np.zeros((n, m))
G1 = np.vstack((-Im, -Im, zero, zero))
G2 = np.vstack((phi, -phi, In, -In))
G = np.hstack((G1, G2))

g = gamma*np.ones((n, 1))
h = np.vstack((y, -y, g, g))

c = matrix(c, tc='d')
G = matrix(G, tc='d')
h = matrix(h, tc='d')

x = solvers.lp(c, G, h)['x'][-1]
print(x)  # 0.5

# plots

fig, ax = plt.subplots()
xs = list(range(5))
ys = [0.5*i for i in xs]
plt.plot(xs, ys, 'g')

plt.plot([2*x], [1*x], 'ro')
plt.plot([1], [1], 'bo')

ax.set_xlim(0, 4)
ax.set_ylim(0, 4)
plt.show()