import matplotlib.pyplot as plt
import numpy as np
from PIL import Image
from pystruct.utils import make_grid_edges
from pystruct.inference import inference_dispatch
from utils import my_inference_dispatch
from skimage.color import rgb2yuv
import imageio
import visvis as vv

T = 1

W = 0.05

SIGMA = 18

K = 15


def load_image(filename: str) -> np.ndarray:
    img = imageio.imread(filename) 
    img = rgb2yuv(img)
    return img

def task3():
    n_states = K + 1
    image_left = load_image('./data/imL.png')   # (288, 384, 3)
    image_right = load_image('./data/imR.png')  # (288, 384, 3)
    assert image_left.shape == image_right.shape

    height, width, _ = image_left.shape  # (288, 384, 3)
    cutted_width = width - K

    unary_potentials = np.zeros( shape=(height, cutted_width, n_states))
    for x in range(cutted_width):
        for y in range(height):
            for state in range(n_states):
                diff = image_left[y, x + state] - image_right[y, x]
                unary_potentials[y, x, state] = min(np.linalg.norm(diff), SIGMA)
    
    edges = make_grid_edges(unary_potentials)  # (212462, 2)

    pairwise_potentials = np.zeros(shape=(n_states, n_states))
    for state_1 in range(n_states):
        for state_2 in range(n_states):
            pairwise_potentials[state_1, state_2] = W*min(abs(state_1 - state_2), T)
    
    result = inference_dispatch(-unary_potentials, -pairwise_potentials, edges, inference_method='qpbo')
    
    picture = result.reshape(height, cutted_width)
    plt.imshow(picture, vmin=0, vmax=n_states, cmap='gray')
    plt.show()


if __name__ == '__main__':
    task3()