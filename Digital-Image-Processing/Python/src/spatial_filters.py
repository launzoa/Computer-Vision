import numpy as np
import bisect


def sobel(image_original):
    M, N, C = image_original.shape
    image = np.zeros((M, N, C), np.float32)

    kernel_y = [-1, -2, -1, 0, 0, 0, 1, 2, 1]
    kernel_x = [-1, 0, 1, -2, 0, 2, -1, 0, 1]

    offset = int(np.sqrt(len(kernel_y)) // 2)

    for y in range(offset, M - offset):
        for x in range(offset, N - offset):
            c_y = np.zeros(C, np.float32)
            c_x = np.zeros(C, np.float32)
            k = 0

            for y_o in range(y - offset, y + offset + 1):
                for x_o in range(x - offset, x + offset + 1):
                    c = image_original[y_o, x_o].astype(np.float32)
                    c_y += c * kernel_y[k]
                    c_x += c * kernel_x[k]
                    k += 1

            image[y, x] = np.sqrt(c_x**2 + c_y**2)

    image_min = np.min(image)
    image_max = np.max(image)
    if image_max > image_min:
        image_normalized = ((image - image_min) / (image_max - image_min)) * 255.0
    else:
        image_normalized = image

    return image_normalized.astype(np.uint8)


def laplacian_N4(image_original):
    M, N, C = image_original.shape
    image = np.zeros((M, N, C), np.float32)

    kernel = [0, -1, 0, -1, 4, -1, 0, -1, 0]
    offset = int(np.sqrt(len(kernel)) // 2)

    for y in range(offset, M - offset):
        for x in range(offset, N - offset):
            c = np.zeros(C, np.float32)
            k = 0

            for y_o in range(y - offset, y + offset + 1):
                for x_o in range(x - offset, x + offset + 1):
                    c += image_original[y_o, x_o].astype(np.float32) * kernel[k]
                    k += 1

            image[y, x] = c

    return np.clip(image, 0, 255).astype(np.uint8)


def median_convulation(image_original, kernel_size):
    M, N, C = image_original.shape
    image = np.zeros((M, N, C), np.uint8)

    offset = kernel_size // 2
    median_idx = (kernel_size**2) // 2

    for y in range(offset, M - offset):
        for x in range(offset, N - offset):
            for c in range(C):
                arr = []

                for y_o in range(y - offset, y + offset + 1):
                    for x_o in range(x - offset, x + offset + 1):
                        v = image_original[y_o, x_o, c]
                        bisect.insort(arr, v)

                image[y, x, c] = arr[median_idx]

    return image


def mean_convulation(image_original, kernel_size):
    M, N, C = image_original.shape
    image = np.zeros((M, N, C), np.float32)

    kernel = [1.0 / (kernel_size**2)] * kernel_size**2
    offset = kernel_size // 2

    for y in range(offset, M - offset):
        for x in range(offset, N - offset):
            k = 0
            c = np.zeros(C, np.float32)

            for y_o in range(y - offset, y + offset + 1):
                for x_o in range(x - offset, x + offset + 1):
                    c += image_original[y_o, x_o].astype(np.float32) * kernel[k]
                    k += 1

            image[y, x] = c

    return np.clip(image, 0, 255).astype(np.uint8)
