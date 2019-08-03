import matplotlib.pyplot as plt
import imageio
import numpy
import os

from conversion import *
from walk_analysis import paths


def create_frames(cors):
    """
    For each cordinate pair, output a graph to be used as a frame
    """

    for i in range(len(cors)):
        fig = plt.figure()
        plt.ylim(-50, 50)
        plt.xlim(-50, 50)

        plt.plot(cors[i][0], cors[i][1], "ro")
        fig.savefig("frames/{}.png".format(i))
        plt.close()


def create_video():
    """
    Merges images in /frames into a 60 fps prediction video
    """
    fnames = os.listdir("frames/")
    fnames.sort(key= lambda a : int(a[:-4]))
    videowriter = imageio.get_writer('prediction_video.mp4', fps=60)
    for fname in fnames:
        videowriter.append_data(plt.imread("frames/{}".format(fname)))
    videowriter.close()


positions = []
for path in paths:
    for c in path:
        positions.append((hex_str_to_single(c[0]),
                          hex_str_to_single(c[1])))

create_frames(positions)
create_video()