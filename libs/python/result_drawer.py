from matplotlib import pyplot as plt
from argparse import ArgumentParser
import numpy as np
import config_pb2



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
    res = config_pb2.TResult()
    
    with open(arguments.file, mode="rb") as file:
        res.ParseFromString(file.read())

    matrix = np.array(res.Matrix.Data)
    print(res.Matrix.Rows, res.Matrix.Columns)
    matrix.resize((res.Matrix.Rows, res.Matrix.Columns))
    draw(matrix)

if __name__ == "__main__":
    args = parse_args()

    main(args)