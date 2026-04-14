import numpy as np
from math import floor


def nearestNeighbor(image_original, r):
    rows, cols, channels = image_original.shape
    M = rows * r
    N = cols * r

    image = np.zeros((M, N, channels), np.uint8)

    for y in range(M):
        for x in range(N):
            y0 = floor(y)
            x0 = floor(x)

            image[y, x] = image_original[y0, x0]

    return image


def bicubic(image_original, r):
    rows, cols, channels = image_original.shape
    M = rows * r
    N = cols * r

    image = np.zeros((M, N, channels), np.uint8)

    for y in range(M):
        for x in range(N):
            y0 = floor(y)
            y1 = min(y0 + 1, M)
            x0 = floor(x)
            x1 = min(x0 + 1, N)

            p00 = image_original[y0, x0]
            p10 = image_original[y0, x1]
            p01 = image_original[y1, x0]
            p11 = image_original[y1, x1]

            dy = (y / r) - y0
            dx = (x / r) - x0

            d = p00
            a = p10 - d
            b = p01 - d
            c = a + b + d - p11

            p = a * dx + b * dy + c * dx * dy + d
            image[y, x] = p.astype(np.uint8)

    return image
