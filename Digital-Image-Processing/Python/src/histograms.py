import numpy as np


def cdf(image, L):
    M, N, C = image.shape

    c_dict = np.zeros(256)
    for y in range(M):
        for x in range(N):
            c = image[y][x][0]
            c_dict[c] += 1

    cdf_arr = np.zeros(256)
    cumulative = 0
    for i in range(L):
        cumulative += c_dict[i]
        cdf_arr[i] = cumulative

    return cdf_arr


def equalization(image_original):
    M, N, C = image_original.shape
    image = np.zeros((M, N, C), np.uint8)
    cdf_arr = cdf(image_original, 256)

    for y in range(M):
        for x in range(N):
            c = image_original[y][x][0]
            r = (256 - 1) / (M * N) * cdf_arr[c]
            r = int(np.round(r))

            image[y][x] = [r, r, r]
    return image
