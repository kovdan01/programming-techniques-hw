import subprocess
import os
import numpy as np
import pandas as pd
from matplotlib import pyplot as plt

try:
    print("Generating data...")
    if not os.path.isfile("data.sqlite"):
        subprocess.call("../generate_data/generate_data 100000 data.sqlite", shell=True)
    print("Done!")

    if not os.path.isfile("sizes.txt"):
        f = open("sizes.txt", "w")
        f.write(" ".join(np.logspace(2, 5, 20).astype(int).astype(str)) + "\n")
        f.close()

    print("Running tests...")
    if not os.path.isfile("results.csv"):
        subprocess.call("./2_test_search data.sqlite sizes.txt results.csv 2> test_search.log", shell=True)
    print("Done!")

    raw = pd.read_csv("results.csv", sep=';')
    data = dict()
    columns = list(raw.columns[1:].astype(int))
    for i in range(raw.shape[0]):
        data[raw.iloc[i]["name"]] = list(raw.iloc[i][1:])
    plt.figure(figsize=(12, 8))
    for name, timings in data.items():
        plt.plot(columns, timings, label=name)
        plt.xscale('log')
        plt.yscale('log')
        plt.legend()
    plt.show()
except Exception as e:
    print(e)
