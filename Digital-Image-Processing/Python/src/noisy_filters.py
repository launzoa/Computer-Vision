import numpy as np
import random


def salt_and_pepper(image_original, prob):
    M, N, _ = image_original.shape
    image = image_original.copy()
    n = int(M * N * prob)

    for _ in range(n):
        y = random.randint(0, M - 1)
        x = random.randint(0, N - 1)

        c = random.random()
        if c >= 0.5:
            image[y, x] = [255, 255, 255]
        else:
            image[y, x] = [0, 0, 0]

    return image
