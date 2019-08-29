import matplotlib.pyplot as plt
import imageio
import numpy
import os
import csv

def create_frame(frame, x, y):
    """
    For given frame and position output a graph to be used as a frame
    """
    fig = plt.figure()
    plt.xlim(100, 200)
    plt.ylim(500, 600)
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


def main():
    with open("out.csv") as csv_file:
        csv_reader = csv.reader(csv_file, delimiter=",")
        frame = 0
        for row in csv_reader:
            create_frame(frame, float(row[0]), float(row[1]))
            frame += 1
    create_video(fps=18)


if __name__ == "__main__":
    main()
