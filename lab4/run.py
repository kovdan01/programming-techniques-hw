import subprocess
import os
import numpy as np
import pandas as pd
from matplotlib import pyplot as plt

try:
    if not os.path.isfile("sizes.txt"):
        f = open("sizes.txt", "w")
        f.write(" ".join(np.linspace(10000, 1000000, 20).astype(int).astype(str)) + "\n")
        f.close()

    print("Running tests...")
    if not os.path.isfile("timings.csv"):
        subprocess.call("./4_prng --sizes=sizes.txt --output=timings.csv 2> test_prng.log", shell=True)
    print("Done!")

    raw_time = pd.read_csv("timings.csv", sep=';')
    data_time = dict()
    columns_time = list(raw_time.columns[1:].astype(int))
    for i in range(raw_time.shape[0]):
        data_time[raw_time.iloc[i]["name"]] = list(raw_time.iloc[i][1:])
    plt.figure(figsize=(12, 8))
    for name, timings in data_time.items():
        plt.plot(columns_time, timings, label=name)
        plt.legend()
    plt.show()

except Exception as e:
    print(e)
