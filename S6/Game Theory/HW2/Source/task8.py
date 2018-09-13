from cvxopt import solvers, matrix
import numpy as np
from itertools import combinations

solvers.options['show_progress'] = False

phi = np.array([[1, 1, 0, 0], 
                [0, 0, 1, 1],
                [1, 0, 1, 0]])

psi = np.array([[1, 3, 0, 5], 
                [2, 0, 2, 1],
                [1, 1, 4, 0],
                [0, 2, 3, 2]])

y = np.array([5, 8, 6])

A = phi @ psi
m = np.linalg.matrix_rank(phi)
n = 4

print(" 1 ------------------------------")

xs = []
is_solution_found = False
for k in range(1, m + 1):
    for c in combinations(range(m), k):
        a = A[:, c]
        solution, residuals, *_ = np.linalg.lstsq(a, y)
        
        if residuals[0] < 10**(-10):
            x = n*[0]
            for counter, index in enumerate(c):
                x[index] = solution[counter]
            xs.append(x)

            is_solution_found = True
    
    if is_solution_found:
        break

for i, x in enumerate(xs):
    print("x{} = {}".format(i, x)) 
    # x0 = [1.0, 0, 1.0000000000000002, 0]
    print("z{} = {}".format(i, psi @ x)) 
    # z0 = [ 1.  4.  5.  3.]

print(" 2 ------------------------------")
 
c = n*[1] + n*[0]

G = np.kron([[-1, 1], [-1, -1]], np.eye(n))
h = 2*n*[0]

A_ = np.hstack((np.zeros((n - 1, n)), A))
b = y

c = matrix(c, tc='d')
G = matrix(G, tc='d')
h = matrix(h, tc='d')
A_ = matrix(A_, tc='d')
b = matrix(b, tc='d')
sol = np.array(solvers.lp(c, G, h, A_, b)['x'])

x = sol[n:]
print("x = {}".format(x)) 
# x = [ 9.999e-01, -1.016e-09, 1.000e+00, 1.1773e-09]
print("z = {}".format(psi @ x)) 
# z = [1., 4., 5., 3.]

print(" 3 ------------------------------")

u = np.array([29, 19, -6, -22]).reshape((n, 1)) / 29
x0 = np.array([0, -19, 35, 22]).reshape((n, 1)) / 29

x = (np.eye(n)  - u @ u.T / (u.T @ u)) @ x0

print("x = {}".format(x)) 
# x = [ 0.612, -0.253, 1.0801, 0.293]
print("z = {}".format(psi @ x)) 
# z = [1.321, 3.679, 4.679, 3.321]