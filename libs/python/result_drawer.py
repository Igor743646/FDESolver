from matplotlib import pyplot as plt
import matplotlib as mpl
import argparse
from argparse import ArgumentParser
import numpy as np
import result_pb2

mpl.rcParams["savefig.format"] = "jpg"
mpl.rcParams["savefig.dpi"] = 'figure'
mpl.rcParams["savefig.bbox"] = 'tight'

class Result(object):

    def __init__(self, proto_res) -> None:
        self.proto_file = proto_res
        self.method_name : str = proto_res.MethodName
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

class Results(object):

    def __init__(self, results):
        self.results : list[Result] = []

        self.config = results.Task
        for result in results.Results:
            self.results.append(Result(result))

def parse_args() -> argparse.Namespace:
    parser = ArgumentParser()

    parser.add_argument("-f", "--file", 
                        required=True, help="file path")
    parser.add_argument("-o", "--out", default="./",
                        required=False, help="path for saving images")
    parser.add_argument("-ts", "--time-slice", default=None, type=int,
                        required=False, help="Time slice of solution")
    parser.add_argument("-v", "--verbose", action='store_false', 
                        help="Show images or not")

    return parser.parse_args()


def draw_flat_field(config, field, title = "Heat map of solution by MFDES"):
    fig, ax = plt.subplots(1, 1, figsize=(10, 7))
    im = ax.imshow(field, origin="lower", aspect='auto',
              extent=(config.LeftBound, config.RightBound, 0, config.MaxTime))
    fig.colorbar(im, ax=ax)
    ax.set(xlabel='x', ylabel="t")
    ax.set_title(title)


def draw_surface(config, solution : tuple[np.ndarray, str], real_solution : tuple[np.ndarray, str] | None = None ):
    tasks = [solution]

    if real_solution is not None and real_solution[0] is not None:
        tasks.append(real_solution)

    _, ax = plt.subplots(1, len(tasks), figsize=(10, 7), subplot_kw={"projection": "3d"})

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


def draw_time_slice(config, time_slice, solution : tuple[np.ndarray, str], real_solution : tuple[np.ndarray, str] | None = None):
    _, ax = plt.subplots(1, 1, figsize=(10, 7))

    X = np.linspace(config.LeftBound, config.RightBound, config.SpaceCount + 1)
    Y_solution = solution[0][time_slice]
    ax.plot(X, Y_solution, label=f"{solution[1]}", linewidth=2.0)

    if real_solution is not None and real_solution[0] is not None:
        Y_real_solution = real_solution[0][time_slice]
        ax.plot(X, Y_real_solution, label=f"{real_solution[1]}", linewidth=1.5)

    ax.legend(loc='best')
    ax.grid(True)
    ax.set_title(f"Time slice t={time_slice * config.TimeStep}")
    ax.set(xlabel='x', ylabel=f"u(x, {time_slice * config.TimeStep})")


def draw_text_matrix(matrix):
    fig, ax = plt.subplots(1, 1, figsize=(10, 7))
    fig.patch.set_visible(False)
    ax.axis('off')
    ax.set_title("Matrix A")
    str_matrix = " \n ".join([" ".join([f"{i:.3f}" for i in row]) for row in matrix.tolist()])
    ax.text(0.5, 0.5, str_matrix, ha="center", va="center")
    

def draw(results : Results, arguments : argparse.Namespace):
    
    for result in results.results:
        outputs = {
            "SM" : arguments.out + f'Solve Matrix A ({result.method_name})',
            "HM" : arguments.out + f'Solution Heat Map ({result.method_name})',
            "SS" : arguments.out + f'Solution Surface ({result.method_name})',
            "ER" : arguments.out + f'Error ({result.method_name})',
            "TS" : arguments.out + f'Time Slice ({result.method_name})',
        }
        
        if result.solver_matrix is not None and \
        result.solver_matrix.shape[0] * result.solver_matrix.shape[1] < 5000:
            draw_text_matrix(result.solver_matrix)
            plt.savefig(outputs['SM'])
        plt.close()
        
        draw_flat_field(result.config, result.field, title=f"HeatMap of solution ({result.method_name})")
        plt.savefig(outputs['HM'])

        draw_surface(result.config, (result.field, result.method_name), (result.real_solution, result.real_solution_name))
        plt.savefig(outputs['SS'])

        if arguments.time_slice is not None:
            draw_time_slice(result.config, 
                            np.clip(arguments.time_slice, 0, result.config.TimeCount), 
                            (result.field, result.method_name), 
                            (result.real_solution, result.real_solution_name))
            plt.savefig(outputs['TS'])

        if result.real_solution is not None:
            draw_surface(result.config, (np.abs(result.field - result.real_solution), f"Error ({result.method_name})"))
            plt.savefig(outputs['ER'])

    if arguments.verbose:
        plt.show()


def main(arguments : argparse.Namespace):
    res = result_pb2.TResults()
    
    with open(arguments.file, mode="rb") as file:
        res.ParseFromString(file.read())

    results = Results(res)
    draw(results, arguments)
    plt.close()


if __name__ == "__main__":
    args = parse_args()

    main(args)