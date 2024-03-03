from matplotlib import pyplot as plt
import matplotlib.animation as animation
import matplotlib as mpl
import argparse
from argparse import ArgumentParser
import numpy as np

try:
    import result_pb2
except:
    from ...build.libs.python import result_pb2

mpl.rcParams["savefig.format"] = "jpg"
mpl.rcParams["savefig.dpi"] = 'figure'
mpl.rcParams["savefig.bbox"] = 'tight'
plt.rcParams['animation.ffmpeg_path'] ='C:\\ProgramData\\chocolatey\\lib\\ffmpeg-full\\tools\\ffmpeg\\bin\\ffmpeg.exe'

class Result(object):

    def __init__(self, proto_res) -> None:
        self.proto_file = proto_res
        self.method_name : str = proto_res.MethodName
        self.config = proto_res.Config
        self.solver_matrix : np.ndarray | None = None

        self.field = np.array(proto_res.Field.Data)
        self.field.resize((proto_res.Field.Rows, proto_res.Field.Columns))

        if proto_res.HasField("SolveMatrix"):
            self.solver_matrix = np.array(proto_res.SolveMatrix.Data)
            self.solver_matrix.resize((proto_res.SolveMatrix.Rows, proto_res.SolveMatrix.Columns))


class Results(object):

    def __init__(self, results):
        self.results : list[Result] = []
        self.real_solution : np.ndarray | None = None
        self.real_solution_name : str | None = None 

        self.config = results.Task
        for result in results.Results:
            self.results.append(Result(result))

        if results.HasField("RealSolution"):
            self.real_solution = np.array(results.RealSolution.Data)
            self.real_solution.resize((results.RealSolution.Rows, results.RealSolution.Columns))
            self.real_solution_name = results.RealSolutionName


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


def draw_flat_field(config, results : list[Result]):
    h, w = 1, len(results)

    fig, ax = plt.subplots(h, w, figsize=(10*w, 7))

    for result_id, result in enumerate(results):
        im = ax[result_id].imshow(result.field, origin="lower", aspect='auto',
              extent=(config.LeftBound, config.RightBound, 0, config.MaxTime))
        ax[result_id].set(xlabel='x', ylabel="t")
        ax[result_id].set_title(result.method_name)

        fig.colorbar(im, ax=ax[result_id])


def draw_surface(config, results : Results):
    tasks = results.results

    h, w = 1, len(tasks)

    if results.real_solution is not None:
        w += 1

    _, ax = plt.subplots(h, w, figsize=(10*w, 7), subplot_kw={"projection": "3d"})

    X = np.linspace(config.LeftBound, config.RightBound, config.SpaceCount + 1)
    Y = np.linspace(0, config.MaxTime, config.TimeCount + 1)
    X, Y = np.meshgrid(X, Y)

    for i, task in enumerate(tasks):
        ax[i].plot_surface(X, Y, task.field)
        ax[i].set_title(task.method_name)
        ax[i].set(xlabel='x', ylabel="y", zlabel="u(x, t)")
        ax[i].zaxis.set_rotate_label(False)

    if results.real_solution is not None:
        ax[-1].plot_surface(X, Y, results.real_solution)
        ax[-1].set_title(results.real_solution_name)
        ax[-1].set(xlabel='x', ylabel="y", zlabel="u(x, t)")
        ax[-1].zaxis.set_rotate_label(False)


def draw_error(config, results : Results):
    tasks = results.results

    h, w = 1, len(tasks)

    fig = plt.figure(figsize=(10*w, 8))
    figs = fig.subfigures(2, 1)

    ax1 = figs[0].subplots(h, w, subplot_kw={"projection": "3d"})
    ax2 = figs[1].subplots(h, w)

    X = np.linspace(config.LeftBound, config.RightBound, config.SpaceCount + 1)
    Y = np.linspace(0, config.MaxTime, config.TimeCount + 1)
    Xm, Ym = np.meshgrid(X, Y)

    for i, task in enumerate(tasks):
        error = np.abs(task.field - results.real_solution)
        ax1[i].plot_surface(Xm, Ym, error)
        ax1[i].set_title(task.method_name, fontsize=12)
        ax1[i].set(xlabel='x', ylabel="y", zlabel="u(x, t)")
        ax1[i].zaxis.set_rotate_label(False)
        ax2[i].boxplot(error.T, showmeans=True, showfliers=False)
        ax2[i].set_xticks(ax2[i].get_xticks()[::max(len(Y)//11, 2)], Y[::max(len(Y)//11, 2)])
        ax2[i].set_title(f"Error increasing")


def draw_time_slice(config, time_slice : int, results : Results):
    tasks = results.results

    _, ax = plt.subplots(1, 1, figsize=(10, 7))

    X = np.linspace(config.LeftBound, config.RightBound, config.SpaceCount + 1)

    if results.real_solution is not None:
        Y_real_solution = results.real_solution[time_slice]
        ax.plot(X, Y_real_solution, label=f"{results.real_solution_name}", linewidth=1.5)

    for i, task in enumerate(tasks):
        Y_solution = task.field[time_slice]
        ax.plot(X, Y_solution, label=f"{task.method_name}", linewidth=2.0)

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


def draw_slices_gif(config, results : Results):
    tasks = results.results

    count = len(tasks) + (1 if results.real_solution is not None else 0)

    ymin, ymax = np.min([np.min(i.field) for i in tasks]), np.max([np.max(i.field) for i in tasks])
    fig, ax = plt.subplots(1, 1, figsize=(15, 10))
    lines = ax.plot(*[[] for i in range(2 * count)])

    def init():
        ax.set_xlim((config.LeftBound, config.RightBound))
        ax.set_ylim((ymin, ymax))
        
        for line in lines:
            line.set_data([], [])
        return lines

    X = np.linspace(config.LeftBound, config.RightBound, config.SpaceCount + 1)

    def animate(i):
        for line_id, line in enumerate(lines):
            if results.real_solution is not None and line_id == len(lines) - 1:
                Y = results.real_solution
                label = results.real_solution_name
            else:
                Y = tasks[line_id].field
                label = tasks[line_id].method_name
            
            line.set_data(X, Y[i])
            line.set_label(f"{label}")
        ymin, ymax = ax.get_ylim()
        ax.set_ylim((ymin, np.max([np.max(task.field[i]) for task in tasks] + [ymin + 0.1])))
        ax.set_title(f"Time slice t={i * config.TimeStep:.2f}")
        ax.grid(True)
        ax.legend(loc='best')
        return lines
    
    anim = animation.FuncAnimation(fig, func=animate, init_func=init,
                                   frames=config.TimeCount + 1, interval=20)
    
    return anim


def draw(results : Results, arguments : argparse.Namespace):
    outputs = {
        "SM" : arguments.out + f'Solve Matrix A',
        "HM" : arguments.out + f'Solution Heat Map',
        "SS" : arguments.out + f'Solution Surface',
        "ER" : arguments.out + f'Error',
        "TS" : arguments.out + f'Time Slice',
        "DS" : arguments.out + f'Dynamic time slices',
    }

    draw_flat_field(results.config, results.results)
    plt.savefig(outputs['HM'])

    draw_surface(results.config, results)
    plt.savefig(outputs['SS'])

    if arguments.time_slice is not None:
        draw_time_slice(results.config, 
                        np.clip(arguments.time_slice, 0, results.config.TimeCount), 
                        results)
        plt.savefig(outputs['TS'])
    
    if results.real_solution is not None:
        draw_error(results.config, results)
        plt.savefig(outputs['ER'])

    gif = draw_slices_gif(results.config, results)
    FFwriter = animation.FFMpegWriter(fps=60)
    gif.save(outputs["DS"] + ".mp4", writer = FFwriter)
    gif.save(outputs["DS"] + ".gif", fps=60)

    if arguments.verbose:
        for task in results.results:
            if task.solver_matrix is not None:
                draw_text_matrix(task.solver_matrix)
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