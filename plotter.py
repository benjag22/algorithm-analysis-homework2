import os
from collections.abc import Callable
from pathlib import Path
from typing import Union

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from pandas import DataFrame
from pandas.core.arrays import ExtensionArray

type ArrayLike = Union[ExtensionArray, np.ndarray]
type PlotData = np.ndarray[tuple[int, ...], np.dtype]
type FitFunc = Callable[[PlotData], PlotData] | np.poly1d

PLOTS_DIR = "plots"
os.makedirs(PLOTS_DIR, exist_ok=True)


class FitData:
    def __init__(self, name: str, x: ArrayLike, y: ArrayLike, x_trend: PlotData):
        self.name = name
        self.x = x
        self.y = y
        self.x_trend = x_trend
        self.fit_func: FitFunc = lambda _x: _x
        self.equation = ""
        self.type = ""


def nlogn_function(x: PlotData, a: np.float64, b: np.float64) -> PlotData:
    """n*log(n) function for curve fitting."""
    return a * x * np.log2(x) + b


def nlog2n_function(x: PlotData, a: np.float64, b: np.float64) -> PlotData:
    """n*log²(n) function for curve fitting."""
    return a * x * np.log2(x) ** 2 + b


def plot_fit_data(df: DataFrame, base_filename: str, color: str) -> FitData:
    x_trend = np.linspace(df["n"].min(), df["n"].max(), 100)
    fit_data = FitData(base_filename, df["n"].values, df["t_mean"].values, x_trend)

    fit_data.type = "polynomial fit"

    z = np.polyfit(df["n"], df["t_mean"], 2)
    p = np.poly1d(z)
    y_trend = p(x_trend)
    plt.plot(x_trend, y_trend, "--", linewidth=2, zorder=99, color=color)

    fit_data.fit_func = p
    fit_data.equation = f"{z[0]:.4f}n² + {z[1]:.2f}n + {z[2]:.2f}"

    return fit_data


def process_csv_file(file_path: Path) -> FitData:
    """Process a single CSV file and create plots. Returns fit data for combined plot."""
    base_filename = os.path.splitext(os.path.basename(file_path))[0]
    # noinspection PyUnresolvedReferences
    colors: list[tuple[float, float, float]] = plt.cm.tab10.colors

    df = pd.read_csv(file_path)
    plt.figure(figsize=(21, 7))

    # subplot 1
    plt.subplot(1, 2, 1)

    plt.errorbar(df["n"], df["t_mean"], yerr=df["t_stdev"], fmt="o-", capsize=5,
                 ecolor=colors[1], color=colors[2], markersize=5)
    fit_data = plot_fit_data(df, base_filename, "black")

    plt.title(f"Mean execution time with standard deviation using {fit_data.type} - {base_filename}", fontsize=16)
    plt.xlabel("Number of Elements (n)", fontsize=16)
    plt.ylabel("Execution time (ns)", fontsize=16)
    plt.grid(True, linestyle="--", alpha=0.7)
    plt.annotate(fit_data.equation, xy=(0.05, 0.95), xycoords="axes fraction",
                 fontsize=16, bbox=dict(boxstyle="round,pad=0.3", fc="white", ec="gray", alpha=0.8))

    # subplot 2
    plt.subplot(1, 2, 2)
    plt.plot(df["n"], df["t_Q0"], "o-", label="Minimum (Q0)")
    plt.plot(df["n"], df["t_Q1"], "s-", label="Q1")
    plt.plot(df["n"], df["t_Q2"], "^-", label="Median (Q2)")
    plt.plot(df["n"], df["t_Q3"], "d-", label="Q3")
    plt.plot(df["n"], df["t_Q4"], "x-", label="Maximum (Q4)")
    plt.title(f"Quartile values vs. Number of elements - {base_filename}", fontsize=16)
    plt.xlabel("Number of elements (n)", fontsize=16)
    plt.ylabel("Execution time (ns)", fontsize=16)
    plt.legend(fontsize=16, loc="best")
    plt.grid(True, linestyle="--", alpha=0.7)

    plt.tight_layout(w_pad=2, h_pad=2)

    output_path = os.path.join(PLOTS_DIR, f"{base_filename}_analysis.png")
    plt.savefig(output_path, dpi=300)
    plt.close()  # Close the figure to free memory

    print(f"Plot saved as '{output_path}'")

    return fit_data


def create_combined_fit_plot(all_fits: list[FitData]) -> None:
    """Create a plot combining all fit curves for comparison."""
    plt.figure(figsize=(21, 7))

    x_min = min([min(fit.x) for fit in all_fits])
    x_max = max([max(fit.x) for fit in all_fits])

    y_max = max([max(fit.y) for fit in all_fits])

    y_max_with_buffer = y_max * 1.1

    x_combined = np.linspace(x_min, x_max, 200)

    plt.subplot(1, 2, 1)

    for fit_data in all_fits:
        plt.scatter(fit_data.x, fit_data.y, s=50, alpha=0.4, label=fit_data.name)

    plt.title("Comparison of algorithm performance - data points", fontsize=16)
    plt.xlabel("Number of elements (n)", fontsize=16)
    plt.ylabel("Execution time (ns)", fontsize=16)
    plt.grid(True, linestyle="--", alpha=0.7)
    plt.ylim(0, y_max_with_buffer)
    plt.legend(fontsize=16, loc="best")

    plt.subplot(1, 2, 2)

    for fit_data in all_fits:
        y_fit = fit_data.fit_func(x_combined)
        plt.plot(x_combined, y_fit, linestyle="-", linewidth=2, label=f"{fit_data.name}: {fit_data.equation}")

    plt.title("Comparison of algorithm performance - fit curves", fontsize=16)
    plt.xlabel("Number of elements (n)", fontsize=16)
    plt.ylabel("Execution time (ns)", fontsize=16)
    plt.grid(True, linestyle="--", alpha=0.7)
    plt.ylim(0, y_max_with_buffer)
    plt.legend(fontsize=16, loc="best")

    output_path = os.path.join(PLOTS_DIR, "combined_fit_curves.png")
    plt.tight_layout(w_pad=2, h_pad=2)
    plt.savefig(output_path, dpi=300)
    plt.close()

    print(f"Combined fit curves plot saved as '{output_path}'")


def main():
    """Process all CSV files in the data directory."""
    data_dir = "data"

    if not os.path.exists(data_dir):
        print(f"Error: Directory '{data_dir}' not found.")
        return

    csv_files = list(Path(data_dir).glob("*.csv"))

    if not csv_files:
        print(f"No CSV files found in '{data_dir}' directory.")
        return

    print(f"Found {len(csv_files)} CSV files to process.")

    all_fits: list[FitData] = []

    for file_path in csv_files:
        try:
            print(f"Processing {file_path}...")
            fit_data = process_csv_file(file_path)
            all_fits.append(fit_data)
        except Exception as e:
            print(f"Error processing {file_path}: {e}")

    create_combined_fit_plot(all_fits)

    print("Processing complete.")


if __name__ == "__main__":
    main()
