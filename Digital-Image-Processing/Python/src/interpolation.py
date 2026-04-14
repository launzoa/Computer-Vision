import numpy as np
from math import floor, modf


def nearestNeighbor(image_original, r):
    rows, cols, channels = image_original.shape
    M = int(rows * r)
    N = int(cols * r)

    image = np.zeros((M, N, channels), np.uint8)

    for y in range(M):
        for x in range(N):
            y0 = floor(y / r)
            x0 = floor(x / r)

            image[y, x] = image_original[y0, x0]

    return image


def bilinear(image_original, r):
    rows, cols, channels = image_original.shape
    M = int(rows * r)
    N = int(cols * r)

    image = np.zeros((M, N, channels), np.uint8)
    image_float = image_original.astype(np.float32)

    for y in range(M):
        for x in range(N):
            y0 = floor(y / r)
            y1 = min(y0 + 1, rows - 1)
            x0 = floor(x / r)
            x1 = min(x0 + 1, cols - 1)

            p00 = image_float[y0, x0]
            p10 = image_float[y0, x1]
            p01 = image_float[y1, x0]
            p11 = image_float[y1, x1]

            dy = (y / r) - y0
            dx = (x / r) - x0

            d = p00
            a = p10 - p00
            b = p01 - p00
            c = p11 + p00 - p10 - p01

            p = a * dx + b * dy + c * dx * dy + d
            image[y, x] = np.clip(p, 0, 255).astype(np.uint8)

    return image


def cubic_kernel(t):
    t = abs(t)

    if t <= 1:
        return 1.5 * t**3 - 2.5 * t**2 + 1

    elif t >= 1 and t < 2:
        return -0.5 * t**3 + 2.5 * t**2 - 4 * t + 2

    else:
        return 0


def bicubic(image_original, r):
    rows, cols, channels = image_original.shape
    M = int(rows * r)
    N = int(cols * r)

    image = np.zeros((M, N, channels), np.uint8)

    for y in range(M):
        for x in range(N):
            y_0 = floor(y / r)
            x_0 = floor(x / r)

            y_idx = [
                max(y_0 - 1, 0),
                max(y_0, 0),
                min(y_0 + 1, rows - 1),
                min(y_0 + 2, rows - 1),
            ]

            x_idx = [
                max(x_0 - 1, 0),
                max(x_0, 0),
                min(x_0 + 1, cols - 1),
                min(x_0 + 2, cols - 1),
            ]

            dy = (y / r) - y_0
            dx = (x / r) - x_0

            wy = [
                cubic_kernel(1 + dy),
                cubic_kernel(dy),
                cubic_kernel(1 - dy),
                cubic_kernel(2 - dy),
            ]

            wx = [
                cubic_kernel(1 + dx),
                cubic_kernel(dx),
                cubic_kernel(1 - dx),
                cubic_kernel(2 - dx),
            ]

            p = np.zeros(channels, dtype=np.float32)

            for i in range(4):
                for j in range(4):
                    v = image_original[y_idx[i], x_idx[j]]
                    w = wy[i] * wx[j]
                    p += v * w

            image[y, x] = np.clip(p, 0, 255).astype(np.uint8)

    return image
