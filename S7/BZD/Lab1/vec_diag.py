import numpy as np
import matplotlib.pyplot as plt
from itertools import combinations
from math import sin, cos, pi, inf
import csv


def compute_circle_intersection(c1, c2, r1, r2):
    x1, y1 = c1
    x2, y2 = c2
    R2 = (x2 - x1)**2 + (y2 - y1)**2
    res = 0.5*np.array([x1 + x2, y1 + y2])
    res += (r1**2 - r2**2)/(2*R2)*np.array([x2 - x1, y2 - y1])
    k = max(0.5*( 2*(r1**2 + r2**2)/R2 - (r1**2 - r2**2)**2/R2**2 - 1 ), 0)
    print(k)
    a = k**0.5*np.array([y2 - y1, x1 - x2])

    return (res + a, res - a)

def find_common_point(v1, v2):
    min_distance = inf
    res = None
    for x in v1:
        for y in v2:
            d = np.linalg.norm(x - y)
            if d < min_distance:
                min_distance = d
                res = (x, y)
    return res

def compute_neutral(ca, cb, cc, ua01, ub01, uc01):
    

    intersection1 = compute_circle_intersection(ca, cb, ua01, ub01)


    intersections = []
    for x, y in combinations(zip([ca, cb, cc], [ua01, ub01, uc01]), 2):
        cx, ux01 = x
        cy, uy01 = y
        intersections.append(compute_circle_intersection(cx, cy, ux01, uy01)) 
    
    common_points = []
    for i1, i2 in combinations(intersections, 2):
        common_points.extend(find_common_point(i1, i2))
    return np.mean(common_points, axis=0)
    
def draw_vector_diagramm(ua, ub, uc, ua01, ub01, uc01):
    plt.clf()

    ca, cb, cc = (ux*np.array([cos(pi/2 - 2*i*pi/3), sin(pi/2 - 2*i*pi/3)]) for i, ux in enumerate([ua, ub, uc]))
    x_n, y_n = compute_neutral(ca, cb, cc, ua01, ub01, uc01)

    X = [0, 0, 0, 0, x_n, x_n, x_n]
    Y = [0, 0, 0, 0, y_n, y_n, y_n]
    U = [ca[0], cb[0], cc[0], x_n, ca[0] - x_n, cb[0] - x_n, cc[0] - x_n]
    V = [ca[1], cb[1], cc[1], y_n, ca[1] - y_n, cb[1] - y_n, cc[1] - y_n]

    C = ['k', 'k', 'k', 'r', 'b', 'b', 'b']

    plt.quiver(X, Y, U, V, color=C, angles='xy', scale_units='xy', scale=1)
    plt.xlim(-40, 40)
    plt.ylim(-40, 40)
    plt.grid(linestyle='-', linewidth=1)
    # plt.show()
    # plt.savefig('test.png')
    return plt

    print(x_n, y_n)

if __name__ == '__main__': 
    ua, ub, uc = 30, 28.4, 27.2
    with open('diag_data.csv', newline='') as csvfile:
        reader = csv.reader(csvfile, delimiter=',')
        for i, row in enumerate(reader):
            draw_vector_diagramm(ua, ub, uc, *map(lambda x: float(x), row)).savefig(f'diag_{i}.png')
        
