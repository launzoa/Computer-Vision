import numpy as np


def gray_scale(image_original):
    M, N, C = image_original.shape
    image = np.zeros((M, N, C), np.uint8)
    image_float = image_original.astype(np.float32)

    for y in range(M):
        for x in range(N):
            c = image_float[y, x]

            Y = 0.299 * c[2] + 0.587 * c[1] + 0.114 * c[0]

            image[y, x] = [Y, Y, Y]

    return image


def color_inversion(image_original):
    M, N, C = image_original.shape
    image = np.zeros((M, N, C), np.uint8)

    for y in range(M):
        for x in range(N):
            c = image_original[y, x]
            B = 255 - c[0]
            G = 255 - c[1]
            R = 255 - c[2]

            image[y, x] = [B, G, R]

    return image


def color_bright(image_original, bright):
    M, N, C = image_original.shape
    image = np.zeros((M, N, C), np.uint8)
    image_float = image_original.astype(np.float32)

    for y in range(M):
        for x in range(N):
            c = image_float[y, x]

            B = np.clip(c[0] + bright, 0, 255)
            G = np.clip(c[1] + bright, 0, 255)
            R = np.clip(c[2] + bright, 0, 255)

            image[y, x] = [B, G, R]

    return image


def color_contrast(image_original, contrast):
    M, N, C = image_original.shape
    image = np.zeros((M, N, C), np.uint8)
    image_float = image_original.astype(np.float32)

    for y in range(M):
        for x in range(N):
            c = image_float[y, x]
            B = np.clip(c[0] * contrast, 0, 255)
            G = np.clip(c[1] * contrast, 0, 255)
            R = np.clip(c[2] * contrast, 0, 255)

            image[y, x] = [B, G, R]

    return image


def thresholding(image_original, threshold):
    M, N, C = image_original.shape
    image = np.zeros((M, N, C), np.uint8)

    for y in range(M):
        for x in range(N):
            c = image_original[y, x]

            if c[0] > threshold:  # assumed that the image in in grayscale
                image[y, x] = [255, 255, 255]
            else:
                image[y, x] = c

    return image


def convertRGBtoHSL(image_original):
    M, N, C = image_original.shape
    image = np.zeros((M, N, C), np.uint8)

    for y in range(M):
        for x in range(N):
            c = image_original[y, x]

            B = c[0] / 255.0
            G = c[1] / 255.0
            R = c[2] / 255.0

            c_max = max(B, G, R)
            c_min = min(B, G, R)
            delta = c_max - c_min

            if delta != 0:
                L = (c_max + c_min) / 2.0
                S = delta / (1 - abs(2 * L - 1))

                if c_max == B:
                    H = 60 * ((R - G) / delta + 4)
                elif c_max == G:
                    H = 60 * ((B - R) / delta + 2)
                else:  # c_max == R
                    H = 60 * (((G - B) / delta) % 6)

                if H < 0:
                    H += 360

                H = H / 2.0
                S = S * 255.0
                L = L * 255.0
            else:
                L = ((c_max + c_min) / 2.0) * 255.0
                H = 0.0
                S = 0.0

            image[y, x] = [H, S, L]

    return image


def convertHSLtoRGB(image_original):
    M, N, Channels = image_original.shape
    image = np.zeros((M, N, Channels), np.uint8)

    for y in range(M):
        for x in range(N):
            h, s, l = image_original[y, x]
            H = h * 2.0
            S = s / 255.0
            L = l / 255.0

            C = (1 - abs(2 * L - 1)) * S
            H_theta = H / 60.0
            X = C * (1 - abs(H_theta % 2 - 1))
            m = L - C / 2.0

            if 0 <= H_theta < 1:
                r, g, b = C, X, 0
            elif 1 <= H_theta < 2:
                r, g, b = X, C, 0
            elif 2 <= H_theta < 3:
                r, g, b = 0, C, X
            elif 3 <= H_theta < 4:
                r, g, b = 0, X, C
            elif 4 <= H_theta < 5:
                r, g, b = X, 0, C
            elif 5 <= H_theta < 6:
                r, g, b = C, 0, X

            B = (b + m) * 255.0
            G = (g + m) * 255.0
            R = (r + m) * 255.0

            image[y, x] = [np.clip(B, 0, 255), np.clip(G, 0, 255), np.clip(R, 0, 255)]

    return image


def a(image_original):
    M, N, C = image_original.shape
    image = np.zeros((M, N, C), np.uint8)

    for y in range(M):
        for x in range(N):
            pass

    return image


def b(image_original):
    M, N, C = image_original.shape
    image = np.zeros((M, N, C), np.uint8)

    for y in range(M):
        for x in range(N):
            pass
    return image
