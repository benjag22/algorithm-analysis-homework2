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
    def __init__(self, df: DataFrame, name: str, algorithm: str):
        self.df = df
        self.name = name
        self.algorithm = algorithm
        self.x = df["n"].values
        self.y = df["t_mean"].values

        match algorithm:
            case "memo" | "dp" | "dp_optimized":
                z = np.polyfit(df["n"], df["t_mean"], 2)

                self.fit_func = np.poly1d(z)
                self.equation = f"{z[0]:.4f}n² + {z[1]:.2f}n + {z[2]:.2f}"

            case "recursive":
                print("TODO")


def get_algorithm(filename: str) -> str:
    parts = filename.split("_")

    if len(parts) >= 3:
        if parts[1].isdigit():
            algorithm = parts[0]
        else:
            algorithm = "_".join(parts[:-2])
    else:
        algorithm = parts[0] if parts else filename

    return algorithm


def create_grouped_plots(grouped_fits: dict[str, list[FitData]]) -> None:
    # noinspection PyUnresolvedReferences
    colors = plt.cm.tab10.colors

    for group_name, fits in grouped_fits.items():
        if not fits:
            continue

        display_name = group_name.replace("_", " ").title()

        plt.figure(figsize=(21, 14))

        x_min = min([min(fit.x) for fit in fits])
        x_max = max([max(fit.x) for fit in fits])
        y_max = max([max(fit.y) for fit in fits])
        y_max_with_buffer = y_max * 1.1
        mem_max = max([max(fit.df["mem"]) for fit in fits])
        mem_max_with_buffer = mem_max * 1.1
        x_combined = np.linspace(x_min, x_max, 200)

        # Subplot 1: Data points with error bars (top left)
        plt.subplot2grid((4, 4), (0, 0), 2, 2)

        for i, fit_data in enumerate(fits):
            extract_numbers = fit_data.name.split("_")[-2:]
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

        # Subplot 2: Fit curves (top right)
        plt.subplot2grid((4, 4), (0, 2), 2, 2)

        for i, fit_data in enumerate(fits):
            extract_numbers = fit_data.name.split("_")[-2:]
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

        # Subplot 3: Memory usage (bottom center, spanning 2 columns)
        plt.subplot2grid((4, 4), (2, 1), 2, 2)

        for i, fit_data in enumerate(fits):
            extract_numbers = fit_data.name.split("_")[-2:]
            extract_label = f"Extracts {extract_numbers[0]}-{extract_numbers[1]}"

            plt.plot(fit_data.df["n"], fit_data.df["mem"], "o-",
                     color=colors[i % len(colors)], markersize=5,
                     label=extract_label, alpha=0.7)

        plt.title(f"{display_name} Algorithm - Memory Usage", fontsize=16)
        plt.xlabel("Number of elements (n)", fontsize=16)
        plt.ylabel("Memory usage (bytes)", fontsize=16)
        plt.grid(True, linestyle="--", alpha=0.7)
        plt.ylim(0, mem_max_with_buffer)
        plt.legend(fontsize=14, loc="best")

        plt.tight_layout(w_pad=2, h_pad=2)

        output_path = os.path.join(PLOTS_DIR, f"{group_name}_grouped_analysis.png")
        plt.savefig(output_path, dpi=300)
        plt.close()

        print(f"Grouped plot for {group_name} saved as '{output_path}'")


def calculate_group_averages(grouped_fits: dict[str, list[FitData]]) -> dict[str, FitData]:
    group_averages = {}

    for algorithm, fits in grouped_fits.items():
        if not fits:
            continue

        all_n_values = [set(fit.x) for fit in fits]
        common_n_values = sorted(set.intersection(*all_n_values)) or sorted(set.union(*all_n_values))

        avg_data = []
        for n in common_n_values:
            t_means = []
            t_stdevs = []
            mems = []

            for fit in fits:
                closest_idx = np.argmin(np.abs(fit.x - n))
                if abs(fit.x[closest_idx] - n) <= (max(fit.x) - min(fit.x)) * 0.01:
                    t_means.append(fit.y[closest_idx])
                    t_stdevs.append(fit.df.iloc[closest_idx]["t_stdev"])
                    mems.append(fit.df.iloc[closest_idx]["mem"])

            if t_means:
                avg_data.append({
                    "n": n,
                    "t_mean": np.mean(t_means),
                    "t_stdev": np.sqrt(np.mean(np.array(t_stdevs) ** 2)),
                    "mem": np.mean(mems)
                })

        if not avg_data:
            continue

        avg_df = pd.DataFrame(avg_data)

        group_averages[algorithm] = FitData(
            avg_df,
            f"{algorithm}_average",
            algorithm,
        )

    return group_averages


def create_combined_fit_plot(grouped_fits: dict[str, list[FitData]]) -> None:
    group_averages = calculate_group_averages(grouped_fits)

    if not group_averages:
        print("No group averages could be calculated.")
        return

    plt.figure(figsize=(21, 14))

    all_fits = list(group_averages.values())
    x_min = min([min(fit.x) for fit in all_fits])
    x_max = max([max(fit.x) for fit in all_fits])
    y_max = max([max(fit.y) for fit in all_fits])
    y_max_with_buffer = y_max * 1.1
    mem_max = max([max(fit.df["mem"]) for fit in all_fits])
    mem_max_with_buffer = mem_max * 1.1
    x_combined = np.linspace(x_min, x_max, 200)

    # Subplot 1: Data points (top left)
    plt.subplot2grid((4, 4), (0, 0), 2, 2)

    for fit_data in all_fits:
        algorithm_name = fit_data.algorithm.replace("_", " ").title()
        plt.errorbar(fit_data.x, fit_data.y, yerr=fit_data.df["t_stdev"],
                     fmt="o-", capsize=5, markersize=6, label=algorithm_name, alpha=0.8)

    plt.title("Algorithm Performance Comparison - Group Averages (Data Points)", fontsize=16)
    plt.xlabel("Number of elements (n)", fontsize=16)
    plt.ylabel("Execution time (ns)", fontsize=16)
    plt.grid(True, linestyle="--", alpha=0.7)
    plt.ylim(0, y_max_with_buffer)
    plt.legend(fontsize=12, loc="best")

    # Subplot 2: Fit curves (top right)
    plt.subplot2grid((4, 4), (0, 2), 2, 2)

    for fit_data in all_fits:
        algorithm_name = fit_data.algorithm.replace("_", " ").title()
        y_fit = fit_data.fit_func(x_combined)
        plt.plot(x_combined, y_fit, linestyle="-", linewidth=3,
                 label=f"{algorithm_name}: {fit_data.equation}")

    plt.title("Algorithm Performance Comparison - Group Averages (Fit Curves)", fontsize=16)
    plt.xlabel("Number of elements (n)", fontsize=16)
    plt.ylabel("Execution time (ns)", fontsize=16)
    plt.grid(True, linestyle="--", alpha=0.7)
    plt.ylim(0, y_max_with_buffer)
    plt.legend(fontsize=12, loc="best")

    # Subplot 3: Memory usage (bottom center, spanning 2 columns)
    plt.subplot2grid((4, 4), (2, 1), 2, 2)

    for fit_data in all_fits:
        algorithm_name = fit_data.name.replace("_average", "").replace("_", " ").title()
        plt.plot(fit_data.df["n"], fit_data.df["mem"], "o-",
                 markersize=6, label=algorithm_name, alpha=0.8, linewidth=2)

    plt.title("Algorithm Memory Usage Comparison - Group Averages", fontsize=16)
    plt.xlabel("Number of elements (n)", fontsize=16)
    plt.ylabel("Memory usage (bytes)", fontsize=16)
    plt.grid(True, linestyle="--", alpha=0.7)
    plt.ylim(0, mem_max_with_buffer)
    plt.legend(fontsize=12, loc="best")

    output_path = os.path.join(PLOTS_DIR, "combined_fit_curves_group_averages.png")
    plt.tight_layout(w_pad=2, h_pad=2)
    plt.savefig(output_path, dpi=300)
    plt.close()

    print(f"Combined group averages plot saved as '{output_path}'")


def group_files_by_algorithm(csv_files: list[Path]) -> dict[str, list[Path]]:
    groups = defaultdict(list)

    for file_path in csv_files:
        algorithm = get_algorithm(file_path.stem)
        groups[algorithm].append(file_path)

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

    grouped_fits: dict[str, list[FitData]] = defaultdict(list)

    for file_path in csv_files:
        try:
            print(f"Processing {file_path}...")
            fit_data = FitData(
                pd.read_csv(file_path),
                os.path.splitext(os.path.basename(file_path))[0],
                get_algorithm(file_path.stem),
            )

            # TODO this one's special c:
            if fit_data.algorithm != "recursive":
                grouped_fits[fit_data.algorithm].append(fit_data)
        except Exception as e:
            print(f"Error processing {file_path}: {e}")

    create_grouped_plots(grouped_fits)
    create_combined_fit_plot(grouped_fits)

    print("Processing complete.")


if __name__ == "__main__":
    main()
