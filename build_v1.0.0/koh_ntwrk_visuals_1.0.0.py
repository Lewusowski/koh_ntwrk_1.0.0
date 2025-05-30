import matplotlib
import matplotlib.pyplot as plt
matplotlib.use("TkAgg")
import numpy as np
from sklearn.decomposition import PCA

raw_data = []
clusters = []
fin_clusters = []


def get_dataset(data_path="dataset"):
    dataset = np.loadtxt(f"{data_path}.txt")
    return dataset


np_array = get_dataset()
np_clusters = get_dataset("clusters")
np_ch_clusters = get_dataset("changed_clusters")


def pca_vis():
    pca = PCA(n_components=2)
    compr_pca_data = pca.fit_transform(np_array)
    compr_pca_clust = pca.fit_transform(np_clusters)
    compr_pca_ch_clust = pca.fit_transform(np_ch_clusters)
    plt.scatter(compr_pca_data[:, 0], compr_pca_data[:, 1])
    plt.scatter(compr_pca_clust[:, 0], compr_pca_clust[:, 1], color='red')
    plt.scatter(compr_pca_ch_clust[:, 0], compr_pca_ch_clust[:, 1], color='green', linewidth=5)
    plt.show()


pca_vis()
