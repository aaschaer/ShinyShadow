import matplotlib.pyplot as plt
import imageio
import numpy
import os


def create_frame(frame, x, y):
    """
    For given frame and position output a graph to be used as a frame
    """
    fig = plt.figure()
    plt.ylim(-50, 50)
    plt.xlim(-50, 50)
    plt.plot(x, -y, "ro")
    fig.savefig("frames/{}.png".format(frame))
    plt.close()


def create_video(fps=60):
    """
    Merges images in /frames into a prediction video
    """
    fnames = os.listdir("frames/")
    fnames.sort(key= lambda a : int(a[:-4]))
    videowriter = imageio.get_writer('prediction_video.mp4', fps=fps)
    for fname in fnames:
        videowriter.append_data(plt.imread("frames/{}".format(fname)))
    videowriter.close()

