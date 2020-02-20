import subprocess
import os
import numpy as np
import pandas as pd
from matplotlib import pyplot as plt

try:
    print("Generating data...")
    if not os.path.isfile("data.sqlite"):
        subprocess.call("../generate_data/generate_data --size=100000 --output=data.sqlite", shell=True)
    print("Done!")

    if not os.path.isfile("sizes.txt"):
        f = open("sizes.txt", "w")
        f.write(" ".join(np.logspace(2, 5, 20).astype(int).astype(str)) + "\n")
        f.close()

    print("Running tests...")
    if not os.path.isfile("results.csv"):
        subprocess.call("./3_hashes --input=data.sqlite --sizes=sizes.txt --output_time=results_time.csv --output_collision=results_collision.csv 2> test_hash.log", shell=True)
    print("Done!")

    raw_time = pd.read_csv("results_time.csv", sep=';')
    data_time = dict()
    columns_time = list(raw_time.columns_time[1:].astype(int))
    for i in range(raw_time.shape[0]):
        data_time[raw_time.iloc[i]["name"]] = list(raw_time.iloc[i][1:])
    plt.figure(figsize=(12, 8))
    for name, timings in data_time.items():
        plt.plot(columns_time, timings, label=name)
        plt.xscale('log')
        plt.yscale('log')
        plt.legend()
    plt.show()
    
    raw_collision = pd.read_csv("results_collision.csv", sep=';')
    data_collision = dict()
    columns_collision = list(raw_collision.columns_collision[1:].astype(int))
    for i in range(raw_collision.shape[0]):
        data_collision[raw_collision.iloc[i]["name"]] = list(raw_collision.iloc[i][1:])
    plt.figure(figsize=(12, 8))
    for name, collisions in data_collision.items():
        plt.plot(columns_collision, sollisions, label=name)
        plt.xscale('log')
        plt.yscale('log')
        plt.legend()
    plt.show()
except Exception as e:
    print(e)
