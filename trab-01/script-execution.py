import os
import subprocess

commands = []

path = "MBV_instances"
instance = os.listdir(path)
for inst in instance:
	commands.append("./relaxlag " + path + "/" + inst + " " + "outputs/heuristics_result_" + inst + " 300")

for c in commands:
    print(c)
    p = subprocess.Popen(c, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    msg, err = p.communicate()
    if msg:
        print(msg)
    print("OK!!")
