from matplotlib import pyplot as plt
import matplotlib as mpl
import argparse
from argparse import ArgumentParser
import numpy as np
import result_pb2


class Result(object):

    def __init__(self, proto_res) -> None:
        self.proto_file = proto_res
        self.config = proto_res.Config
        self.solver_matrix : np.ndarray | None = None
        self.real_solution : np.ndarray | None = None
        self.real_solution_name : str | None = None 

        self.field = np.array(proto_res.Field.Data)
        self.field.resize((proto_res.Field.Rows, proto_res.Field.Columns))

        if proto_res.HasField("SolveMatrix"):
            self.solver_matrix = np.array(proto_res.SolveMatrix.Data)
            self.solver_matrix.resize((proto_res.SolveMatrix.Rows, proto_res.SolveMatrix.Columns))

        if proto_res.HasField("RealSolution"):
            self.real_solution = np.array(proto_res.RealSolution.Data)
            self.real_solution.resize((proto_res.RealSolution.Rows, proto_res.RealSolution.Columns))
            self.real_solution_name = proto_res.RealSolutionName


def parse_args() -> argparse.Namespace:
    parser = ArgumentParser()

    parser.add_argument("-f", "--file", 
                        required=True, help="file path")
    parser.add_argument("-o", "--out", default="./",
                        required=False, help="path for saving images")

    return parser.parse_args()


def draw_flat_field(field, config):
    fig, ax = plt.subplots(1, 1, figsize=(10, 7))
    im = ax.imshow(field, origin="lower", aspect='auto',
              extent=(config.LeftBound, config.RightBound, 0, config.MaxTime))
    fig.colorbar(im, ax=ax)
    ax.set(xlabel='x')
    ax.set(ylabel='t')
    ax.set_title("Heat map of solution by MFDES")


def draw_surface(solution : tuple[np.ndarray, str], config, real_solution : tuple[np.ndarray, str] | None = None ):
    tasks = [solution]

    if real_solution is not None and real_solution[0] is not None:
        tasks.append(real_solution)

    fig, ax = plt.subplots(1, len(tasks), figsize=(10, 7), subplot_kw={"projection": "3d"})

    if len(tasks) == 1:
        ax = [ax]

    X = np.linspace(config.LeftBound, config.RightBound, config.SpaceCount + 1)
    Y = np.linspace(0, config.MaxTime, config.TimeCount + 1)
    X, Y = np.meshgrid(X, Y)

    for i, (matrix, title) in enumerate(tasks):
        ax[i].plot_surface(X, Y, matrix)
        ax[i].set_title(title)
        ax[i].set(xlabel='x', ylabel="y", zlabel="u(x, t)")
        ax[i].zaxis.set_rotate_label(False)


def draw_text_matrix(matrix):
    fig, ax = plt.subplots(1, 1, figsize=(10, 7))
    fig.patch.set_visible(False)
    ax.axis('off')
    ax.set_title("Matrix A")
    str_matrix = " \n ".join([" ".join([f"{i:.3f}" for i in row]) for row in matrix.tolist()])
    # ax.text(0.0, 0.5, f"$$\\begin{{bmatrix}} {str_matrix} \\end{{bmatrix}}$$")
    ax.text(0.5, 0.0, str_matrix, ha="center", va="center")
    

def draw(result : Result, arguments : argparse.Namespace):
    if result.solver_matrix is not None and \
    result.solver_matrix.shape[0] * result.solver_matrix.shape[1] < 10000:
        draw_text_matrix(result.solver_matrix)
        plt.savefig(arguments.out + 'SolveMatrixA.jpg', dpi=700, bbox_inches='tight')
    plt.close()
    
    draw_flat_field(result.field, result.config)
    plt.savefig(arguments.out + 'SolutionHeatMap.jpg', dpi=700, bbox_inches='tight')

    draw_surface((result.field, "Solution by MFDES"), result.config, (result.real_solution, result.real_solution_name))
    plt.savefig(arguments.out + 'SolutionSurface.jpg', dpi=700, bbox_inches='tight')

    if result.real_solution is not None:
        draw_surface((np.abs(result.field - result.real_solution), "Error"), result.config)
        plt.savefig(arguments.out + 'Error.jpg', dpi=700, bbox_inches='tight')

    plt.show()


def main(arguments : argparse.Namespace):
    res = result_pb2.TResult()
    
    with open(arguments.file, mode="rb") as file:
        res.ParseFromString(file.read())

    result = Result(res)
    draw(result, arguments)
    plt.close()


if __name__ == "__main__":
    args = parse_args()

    main(args)