import matplotlib.pyplot as plt
from matplotlib.patches import Polygon

from cvxopt import solvers, matrix
import numpy as np

phi = np.array([2, 1]).reshape((2, 1))
y = np.array([1, 1]).reshape((2, 1))
epsilon = 1

m, n = phi.shape

c = n*[1] + n*[0]

I = np.eye(n)
zero = np.zeros((m, n))
G1 = np.vstack((-I, -I, zero, zero))
G2 = np.vstack((I, -I, phi, -phi))
G = np.hstack((G1, G2))

e = epsilon*np.ones((m, 1))
zero = np.zeros((1, n))
h = np.vstack((zero, zero, e + y, e - y))

c = matrix(c, tc='d')
G = matrix(G, tc='d')
h = matrix(h, tc='d')

x = solvers.lp(c, G, h)['x'][-1]
print(x)  # 4.637e-09

# plots

fig, ax = plt.subplots()
xs = list(range(-1, 5))
ys = [0.5*i for i in xs]
plt.plot(xs, ys, 'g')

plt.plot([2*x], [1*x], 'ro')
plt.plot([1], [1], 'bo')

ax.set_xlim(-1, 4)
ax.set_ylim(-1, 4)
plt.show()