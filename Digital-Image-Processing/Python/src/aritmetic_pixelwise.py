import numpy as np
import bisect


def median_filter(image_original_array):
    M, N, C = image_original_array[0].shape
    image = np.zeros((M, N, C), np.uint8)
    median_idx = len(image_original_array) // 2

    for y in range(M):
        for x in range(N):
            for c in range(C):
                arr = []

                for img in image_original_array:
                    v = img[y, x, c]
                    bisect.insort(arr, v)

                image[y, x, c] = arr[median_idx]

    return image


def mean_filter(image_original_array):
    M, N, C = image_original_array[0].shape
    image = np.zeros((M, N, C), np.uint8)
    cumulative_sum = np.zeros((M, N, C), np.float32)

    for img in image_original_array:
        cumulative_sum += img

    for y in range(M):
        for x in range(N):
            image[y, x] = cumulative_sum[y, x] / len(image_original_array)

    return image
