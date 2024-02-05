from matplotlib import pyplot as plt
from argparse import ArgumentParser
import numpy as np


def parse_args():
    parser = ArgumentParser()

    parser.add_argument("-f", "--file", 
                        required=True, help="file path")

    return parser.parse_args()


def draw(matrix):
    fig, ax = plt.subplots(1, 1, figsize=(10, 7))

    ax.imshow(matrix)
    # ax.bar()

    fig, ax = plt.subplots(1, 1, figsize=(10, 7), subplot_kw={"projection": "3d"})
    print(matrix.shape)

    X = np.arange(0, matrix.shape[1])
    Y = np.arange(0, matrix.shape[0])
    X, Y = np.meshgrid(X, Y)

    ax.plot_surface(X, Y, matrix)

    plt.show()



def main(arguments):
    values = []
    n = m = None
    with open(arguments.file, mode="r") as file:
        n, m = [int(i) for i in file.readline().strip(" \n").split(' ')]
        for row in file:
            s = row.strip(" \n").split(' ')
            values.extend(list(map(float, s)))

    matrix = np.array(values)
    matrix.resize((n, m))
    draw(matrix)

if __name__ == "__main__":
    args = parse_args()

    main(args)