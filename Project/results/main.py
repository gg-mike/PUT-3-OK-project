from bokeh.plotting import figure, show
from bokeh.layouts import column, row
from numpy import random

filepaths = [
    "res_m10n21.csv",
    "res_m10n200.csv",
    "res_m20n41.csv",
    "res_m25n198.csv",
    "res_m50n101.csv",
    "res_m50n200.csv",
    "res_m50n200lpt.csv",
    "res_m50n1000.csv"
]


def ns_hex(n):
    s1 = f"{hex((random.randint(50, 200) + int(n)) % 256)}"[2:]
    s2 = f"{hex(random.randint(50, 200))}"[2:]
    s3 = f"{hex(random.randint(50, 200))}"[2:]
    s1 = s1 if len(s1) == 2 else "0" + s1
    s2 = s2 if len(s2) == 2 else "0" + s2
    s3 = s3 if len(s3) == 2 else "0" + s3
    return s1 + s2 + s3


def get_data(filepath):
    file = open(filepath)
    algorithms = []
    nums = []
    bests = []
    averages = []
    instance = file.readline()
    num = []
    best = []
    avg = []
    i = 0
    for line in file:
        if line[0] == "(":
            algorithms.append(line)
        elif line[0].upper() != "X" and line != "\n":
            b, a = [float(i) for i in line.split(";")]
            num.append(i)
            best.append(b)
            avg.append(a)
            i += 1
        elif line[0] == "X":
            nums.append(num)
            bests.append(best)
            averages.append(avg)
            num = []
            best = []
            avg = []
            i = 0
            file.readline()
        else:
            break
    file.close()
    return instance, algorithms, nums, bests, averages


def make_plots(filepath):
    instance, algorithms, nums, bests, averages = get_data(filepath)
    p_best = figure(
        tools="pan,box_zoom,reset,save",
        title=instance + " - Best",
        plot_width=700,
        plot_height=500)
    p_avg = figure(
        tools="pan,box_zoom,reset,save",
        title=instance + " - Avg",
        plot_width=700,
        plot_height=500)
    for i in range(len(algorithms)):
        p_best.line(nums[i], bests[i], color=f"#{ns_hex(bests[i][-1])}", legend_label=algorithms[i])
        p_avg.line(nums[i], averages[i], color=f"#{ns_hex(bests[i][-1])}", legend_label=algorithms[i])
    return row(p_best, p_avg)


def present_data():
    rows = []
    for filepath in filepaths:
        rows.append(make_plots(filepath))
    return column(rows)


if __name__ == '__main__':
    show(present_data())
