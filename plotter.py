import os
from collections import defaultdict
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
    def __init__(self, df: DataFrame, name: str, type_str: str, fit_func: FitFunc, equation: str):
        self.df = df
        self.name = name
        self.x = df["n"].values
        self.y = df["t_mean"].values
        self.x_trend = np.linspace(df["n"].min(), df["n"].max(), 100)
        self.type = type_str
        self.fit_func = fit_func
        self.equation = equation


def process_csv_file(file_path: Path) -> FitData:
    base_filename = os.path.splitext(os.path.basename(file_path))[0]
    df = pd.read_csv(file_path)
    z = np.polyfit(df["n"], df["t_mean"], 2)

    return FitData(
        df,
        base_filename,
        "polynomial fit",
        np.poly1d(z),
        f"{z[0]:.4f}n² + {z[1]:.2f}n + {z[2]:.2f}",
    )


def create_grouped_plots(grouped_fits: dict[str, list[FitData]]) -> None:
    # noinspection PyUnresolvedReferences
    colors = plt.cm.tab10.colors

    for group_name, fits in grouped_fits.items():
        if not fits:
            continue

        display_name = group_name.replace("_", " ").title()

        plt.figure(figsize=(21, 7))

        x_min = min([min(fit.x) for fit in fits])
        x_max = max([max(fit.x) for fit in fits])
        y_max = max([max(fit.y) for fit in fits])
        y_max_with_buffer = y_max * 1.1
        x_combined = np.linspace(x_min, x_max, 200)

        # Subplot 1: Data points with error bars
        plt.subplot(1, 2, 1)

        for i, fit_data in enumerate(fits):
            extract_numbers = fit_data.name.split('_')[-2:]
            extract_label = f"Extracts {extract_numbers[0]}-{extract_numbers[1]}"

            plt.errorbar(fit_data.df["n"], fit_data.df["t_mean"], yerr=fit_data.df["t_stdev"],
                         fmt="o-", capsize=5, color=colors[i % len(colors)],
                         markersize=5, label=extract_label, alpha=0.7)

        plt.title(f"{display_name} Algorithm - Data Points with Error Bars", fontsize=16)
        plt.xlabel("Number of elements (n)", fontsize=16)
        plt.ylabel("Execution time (ns)", fontsize=16)
        plt.grid(True, linestyle="--", alpha=0.7)
        plt.ylim(0, y_max_with_buffer)
        plt.legend(fontsize=14, loc="best")

        # Subplot 2: Fit curves
        plt.subplot(1, 2, 2)

        for i, fit_data in enumerate(fits):
            extract_numbers = fit_data.name.split('_')[-2:]
            extract_label = f"Extracts {extract_numbers[0]}-{extract_numbers[1]}"

            y_fit = fit_data.fit_func(x_combined)
            plt.plot(x_combined, y_fit, linestyle="-", linewidth=2,
                     color=colors[i % len(colors)],
                     label=f"{extract_label}: {fit_data.equation}")

        plt.title(f"{display_name} Algorithm - Fit Curves Comparison", fontsize=16)
        plt.xlabel("Number of elements (n)", fontsize=16)
        plt.ylabel("Execution time (ns)", fontsize=16)
        plt.grid(True, linestyle="--", alpha=0.7)
        plt.ylim(0, y_max_with_buffer)
        plt.legend(fontsize=12, loc="best")

        plt.tight_layout(w_pad=2, h_pad=2)

        output_path = os.path.join(PLOTS_DIR, f"{group_name}_grouped_analysis.png")
        plt.savefig(output_path, dpi=300)
        plt.close()

        print(f"Grouped plot for {group_name} saved as '{output_path}'")


def create_combined_fit_plot(all_fits: list[FitData]) -> None:
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
        plt.plot(x_combined, y_fit, linestyle="-", linewidth=2, label=fit_data.name)

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


def group_files_by_algorithm(csv_files: list[Path]) -> dict[str, list[Path]]:
    groups = defaultdict(list)

    for file_path in csv_files:
        filename = file_path.stem

        if filename.startswith("dp_optimized_"):
            groups["dp_optimized"].append(file_path)
        elif filename.startswith("dp_"):
            groups["dp"].append(file_path)
        elif filename.startswith("memo_"):
            groups["memo"].append(file_path)
        else:
            print(f"Warning: Unknown file pattern for {filename}")

    return dict(groups)


def main():
    data_dir = "data"

    if not os.path.exists(data_dir):
        print(f"Error: Directory '{data_dir}' not found.")
        return

    csv_files = list(Path(data_dir).glob("*.csv"))

    if not csv_files:
        print(f"No CSV files found in '{data_dir}' directory.")
        return

    print(f"Found {len(csv_files)} CSV files to process.")

    grouped_files = group_files_by_algorithm(csv_files)

    print("File grouping:")
    for group_name, files in grouped_files.items():
        print(f"  {group_name}: {len(files)} files")

    all_fits: list[FitData] = []
    grouped_fits: dict[str, list[FitData]] = defaultdict(list)

    for file_path in csv_files:
        try:
            print(f"Processing {file_path}...")
            fit_data = process_csv_file(file_path)
            all_fits.append(fit_data)

            filename = file_path.stem
            if filename.startswith("dp_optimized_"):
                grouped_fits["dp_optimized"].append(fit_data)
            elif filename.startswith("dp_"):
                grouped_fits["dp"].append(fit_data)
            elif filename.startswith("memo_"):
                grouped_fits["memo"].append(fit_data)

        except Exception as e:
            print(f"Error processing {file_path}: {e}")

    create_grouped_plots(grouped_fits)
    create_combined_fit_plot(all_fits)

    print("Processing complete.")


if __name__ == "__main__":
    main()
