from cvxopt import solvers, matrix
import numpy as np

solvers.options['show_progress'] = False

capacities = [16, 13, 10, 4, 12, 14, 9, 7, 20, 4]

c = np.array([0, 0, 0, 0, 0, 0, 0, 0, -1, -1])

G = np.vstack( (-np.eye(10), np.eye(10)) )
h = 10*[0] + capacities

A = [[1, 0, -1, 1, -1, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 1, 0, -1, 1, -1, 0],
    [0, 1, 1, -1, 0, -1, 1, 0, 0, 0],
    [0, 0, 0, 0, 0, 1, 0, -1, 0, -1]]
b = [0, 0, 0, 0]

c = matrix(c, tc='d')
G = matrix(G, tc='d')
h = matrix(h, tc='d')
A = matrix(np.array(A), tc='d')
b = matrix(b, tc='d')

x = solvers.lp(c, G, h, A, b)['x']
print(x) 
# [12.968, 10.032, 3.854, 2.886, 12.0, 11.0, 0.0, 7.0, 19.0, 4.0]
print(x[-2] + x[-1])  # 22.9999