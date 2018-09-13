import numpy as np
from pystruct.inference import inference_dispatch
from pystruct.utils import make_grid_edges
from scipy.io import loadmat
import matplotlib.pyplot as plt


def task1():
    data = loadmat('data/mrf_potentials_segmentation.mat')
    unary_potentials = data.get('unary_potentials')  # type: np.ndarray (338, 430, 3)
    pairwise_potentials_vert = data.get('pairwise_potentials_vert')  # type: np.ndarray (3, 3)
    pairwise_potentials_horz = data.get('pairwise_potentials_horz')  # type: np.ndarray (3, 3)
    
    edges = make_grid_edges(unary_potentials)  # (289912, 2)

    height, width, n_states = unary_potentials.shape  
    n_edges = edges.shape[0]

    pairwise_potentials = np.zeros(shape=(n_edges, n_states, n_states))
    for i, edge in enumerate(edges):
        vert_1, vert_2 = edge
        y1, x1 = divmod(vert_1, width)
        y2, x2 = divmod(vert_2, width)
        
        if abs(y1 - y2) == 1: 
            # vertical edge
            pairwise_potentials[i, :, :] = pairwise_potentials_vert
        elif abs(x1 - x2) == 1:
            # horisontal edge
            pairwise_potentials[i, :, :] = pairwise_potentials_horz 
    
    # QPBO because max-product works slow. 
    result = inference_dispatch(-1.0*unary_potentials, -1.0*pairwise_potentials, edges, inference_method='qpbo')
    
    picture = result.reshape(height, width)
    plt.imshow(picture, vmin=0, vmax=n_states, cmap='gray')
    plt.show()

if __name__ == '__main__':
    task1()