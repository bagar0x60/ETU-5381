import numpy as np
from pystruct.inference import inference_dispatch
from pystruct.utils import make_grid_edges
from scipy.io import loadmat
import matplotlib.pyplot as plt
import imageio
import visvis as vv
from numpy.linalg import norm



class ImagesData:
    def __init__(self, images_names, markup_name=None):
        self.shape = None
        self.images = []
        self.markup = None
        self.__load_images(images_names)
        if markup_name is not None:
            self.__load_markup(markup_name)

    def __check_shape(self, image):
        if self.shape is None:
            self.shape = image.shape
        else:
            if self.shape != image.shape:
                print('images shapes are not equile')
                print(self.shape, image.shape)
                exit(1)

    def __load_images(self, images_names):
        self.images = []
        for image_name in images_names:
            self.images.append( imageio.imread(image_name) )            
            self.__check_shape(self.images[-1])

    def __load_markup(self, markup_name):
        self.markup = imageio.imread(markup_name) // 20
        self.__check_shape(self.markup)                
    
    def __scale_image(self, image, scale):
        height, width, colors = self.shape
        new_height, new_width = height//scale, width//scale
        new_image = np.zeros( shape=(new_height, new_width, colors) )
        for x in range(new_width):
            for y in range(new_height):
                square = image[ scale*y:scale*(y + 1), scale*x:scale*(x + 1) ]
                new_image[y, x] = np.rint( np.mean(square, axis=(0, 1)) )
        return new_image

    def __scale_markup(self, markup, scale):
        height, width, colors = self.shape
        new_height, new_width = height//scale, width//scale
        new_markup = np.zeros( shape=(new_height, new_width, colors) )
        n_states = len(self.images)
        for x in range(new_width):
            for y in range(new_height):
                value = 0
                count = 0
                for i in range(scale*x, scale*(x + 1)):
                    for j in range(scale*y, scale*(y + 1)):
                        if markup[j, i, 0] < n_states:
                           value = markup[j, i, 0]
                           count += 1
                if count >= scale**2 / 2:
                    new_markup[y, x] = np.array([value, value, value])
                else:
                    new_markup[y, x] = np.array([255 // 20, 255 // 20, 255 // 20])
        return new_markup
            
    def scale(self, scale: int):
        # scale images and markup to 1/scale times
        # each new pixel is mean of scale x scale square in origin image
        
        self.images = [self.__scale_image(image, scale) for image in self.images]
        if self.markup is not None:
            self.markup = self.__scale_markup(self.markup, scale)
        
        height, width, colors = self.shape
        self.shape = (height//scale, width//scale, colors)


class Task2:
    def __init__(self, images_data: ImagesData, unary_potentials):
        self.imgs = images_data
        self.unary_potentials = unary_potentials
        self.__result = None
        self.path_to_dump_file = None
        self.diff_function = diff_standart

    def __compute_pairwise_potentials(self, edges):
        n_states = len(self.imgs.images)
        n_edges = edges.shape[0]  #  849946
        height, width, _ = self.imgs.shape
        pairwise_potentials = np.zeros(shape=(n_edges, n_states, n_states)) 

        
        """
 ________________________________
< wow. such slow loop down there! >
 --------------------------------
        \   ^__^
         \  (oo)\_______
            (__)\       )\/\
                ||----w |
                ||     ||
        """

        for i, edge in enumerate(edges):
            if i % 1000 == 0:
                print('{} / {}'.format(i // 1000, len(edges) // 1000))

            vert_1, vert_2 = edge
            y1, x1 = divmod(vert_1, width)
            y2, x2 = divmod(vert_2, width)
            
            for state_1 in range(n_states):
                for state_2 in range(n_states):
                    if state_1 != state_2:
                        pairwise_potentials[i, state_1, state_2] = self.diff_function(self.imgs.images, state_1, state_2, x1, y1, x2, y2)
       
        return pairwise_potentials

    def __load_pairwise_potentials(self, path):
        n_states = len(self.imgs.images)
        res = np.fromfile(path)
        elements_count = res.shape[0]
        return res.reshape( (elements_count // n_states**2, n_states, n_states) )

    def __get_regions(self):
        if self.__result is not None:
            height, width, _ = self.imgs.shape
            return self.__result.reshape(height, width)

    def set_dump_file(self, path_to_dump_file: str):
        self.path_to_dump_file = path_to_dump_file

    def set_diff_function(self, diff_function):
        self.diff_function = diff_function

    def draw_regions(self):
        if self.__result is not None:
            regions = self.__get_regions()
            n_states = len(self.imgs.images)
            plt.imshow(regions, vmin=0, vmax=n_states)
            plt.show()
    
    def draw_picture(self):
        if self.__result is not None:
            regions = self.__get_regions()
            height, width = regions.shape
            picture = np.zeros(shape=(height, width, 3))
            
            for x in range(width):
                for y in range(height):
                    region = regions[y, x]
                    picture[y, x] = self.imgs.images[region][y, x]

            vv.imshow(picture)

    def get_pairwise_potentials(self, edges, from_file, save_to_file):
        if (from_file or save_to_file) and self.path_to_dump_file is None:
            raise Exception('Path to dump file is not set')

        if from_file:
            pairwise_potentials = self.__load_pairwise_potentials(self.path_to_dump_file)
        else:
            pairwise_potentials = self.__compute_pairwise_potentials(edges)   

        if save_to_file:
            pairwise_potentials.tofile(self.path_to_dump_file)

        return pairwise_potentials


    def compute(self, eta, from_file=True, save_to_file=False):
        unary_potentials = self.unary_potentials 
        edges = make_grid_edges(unary_potentials)
        pairwise_potentials = self.get_pairwise_potentials(edges, from_file, save_to_file)        

        print('start computing')
        self.__result = inference_dispatch(-unary_potentials, -eta*pairwise_potentials, edges, inference_method='qpbo')
        print('stop comuting')
        """
        ‘qpbo’ for QPBO alpha-expansion (fast but approximate).
        ‘lp’ for build-in lp relaxation via cvxopt (slow).
        ‘ad3’ for AD^3 subgradient based dual solution of LP.
        ‘ogm’ for OpenGM wrappers.
        ‘max-product’ for max-product message passing.
        ‘unary’ for using unary potentials only.
        """

def diff_standart(images, state_1, state_2, x1, y1, x2, y2):
    d1 = images[state_1][y1, x1] - images[state_2][y1, x1]
    d2 = images[state_1][y2, x2] - images[state_2][y2, x2]
    return norm(d1) + norm(d2)

def diff_alternative(images, state_1, state_2, x1, y1, x2, y2):
    d1 = images[state_1][y1, x1] - images[state_2][y1, x1]
    d2 = images[state_1][y2, x2] - images[state_2][y2, x2]

    d3 = images[state_1][y1, x1] - images[state_1][y2, x2]
    d4 = images[state_2][y1, x1] - images[state_2][y2, x2]
    return (norm(d1) + norm(d2)) / (norm(d3) + norm(d4) + 1)

def compute_unary_potentials_family(images_data: ImagesData):
    INF = 10**5

    height, width, _ = images_data.shape 
    n_states = len(images_data.images)

    markup = images_data.markup
    unary_potentials = np.zeros(shape=(height, width, n_states))
    for y in range(height):
        for x in range(width):
            if markup[y, x, 0] < n_states:              
                for state in range(n_states):
                    if state != markup[y, x, 0]:
                        unary_potentials[y, x, state] = INF
    return unary_potentials  

def compute_unary_potentials_pano(images_data: ImagesData):
    INF = 10**5

    height, width, _ = images_data.shape 
    n_states = len(images_data.images)
    

    unary_potentials = np.zeros(shape=(height, width, n_states))
    for y in range(height):
        for x in range(width):
            for state, image in enumerate(images_data.images):
                if np.all(image[y, x] == 0):
                    unary_potentials[y, x, state] = INF
                
    return unary_potentials  

def task2_family():
    images_names = [
                        'data/family/small_DSC_0168.png',
                        'data/family/small_DSC_0170.png',
                        'data/family/small_DSC_0173.png',
                        'data/family/small_DSC_0174.png',
                        'data/family/small_DSC_0176.png',
                    ]
    markup_name = 'data/family/familydatacost.png'
    images_data = ImagesData(images_names, markup_name)
    images_data.scale(2)

    unary_potentials = compute_unary_potentials_family(images_data)

    task2 = Task2(images_data, unary_potentials)
    task2.set_dump_file('data/dump/family_alternative_2')
    task2.set_diff_function(diff_alternative)
    task2.compute(eta=0.05, from_file=True, save_to_file=False)

    task2.draw_regions()
    task2.draw_picture()
    input('Press any key')

def task2_pano():
    images_names = [
                        'data/pano/NQIMG_0257.PNG',
                        'data/pano/NQIMG_0258.PNG',
                        'data/pano/NQIMG_0259.PNG',
                        'data/pano/NQIMG_0260.PNG',
                        'data/pano/NQIMG_0261.PNG',
                        'data/pano/NQIMG_0263.PNG',
                        'data/pano/NQIMG_0264.PNG',
                    ]
    images_data = ImagesData(images_names)
    images_data.scale(2)

    unary_potentials = compute_unary_potentials_pano(images_data)

    task2 = Task2(images_data, unary_potentials)
    task2.set_dump_file('data/dump/pano_standart_2')
    task2.set_diff_function(diff_standart)
    task2.compute(eta=0.5, from_file=False, save_to_file=True)

    task2.draw_regions()
    task2.draw_picture()
    input('Press any key')


if __name__ == '__main__':
    #task2_family()
    task2_pano()