import matplotlib.pyplot as plt
import matplotlib
matplotlib.use("TkAgg")

wcss = []

with open("wcss.txt", "r") as file:
    content = file.readline()
    while content:
        wcss.append(float(content.strip()))
        content = file.readline()

file.close()
plt.plot(wcss)

plt.show()
