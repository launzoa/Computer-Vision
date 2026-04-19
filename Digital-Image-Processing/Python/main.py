import cv2
import random
from src.spatial_filters import (
    laplacian_N4,
    mean_convulation,
    median_convulation,
    sobel,
)
from src.histograms import equalization
from src.aritmetic_pixelwise import mean_filter
from src.noisy_filters import salt_and_pepper
from src.intensity_transformations import (
    gray_scale,
    color_inversion,
    color_bright,
    color_contrast,
    thresholding,
    convertRGBtoHSL,
    convertHSLtoRGB,
)
from src.interpolation import nearestNeighbor, bilinear, bicubic


def spatial_filters():
    img = cv2.imread("assets/spatial_filters/minecraft.webp")
    """ 
    img_SAP = salt_and_pepper(img, 0.042)
    cv2.imwrite("assets/spatial_filters/salt_and_pepper.jpg", img_SAP)

    n = int(input("Length of kernel: "))

    img_equalization = mean_convulation(img_SAP, n)
    cv2.imwrite("assets/spatial_filters/mean.jpg", img_equalization)

    img_median = median_convulation(img_SAP, n)
    cv2.imwrite("assets/spatial_filters/median.jpg", img_median)
    """

    img_laplacian_N4 = laplacian_N4(img)
    cv2.imwrite("assets/spatial_filters/laplacian_N4.jpg", img_laplacian_N4)

    img_sobel = sobel(img)
    cv2.imwrite("assets/spatial_filters/sobel.jpg", img_sobel)

    print("Images saves on folder 'assets/spatial_filters'.")


def histograms():
    img = cv2.imread("assets/histograms/rio_de_janeiro.webp")
    img_gray = gray_scale(img)
    cv2.imwrite("assets/histograms/gray.jpg", img_gray)

    img_equalization = equalization(img_gray)
    cv2.imwrite("assets/histograms/equalization.jpg", img_equalization)

    print("Images saves on folder 'assets/histograms'.")


def aritmetic_pixelwise():
    img = cv2.imread("assets/aritmetic_pixelwise/terraria.webp")

    n = int(input("Number of noisy images: "))
    img_SAP_array = []
    for _ in range(n):
        p = random.random() % 0.42
        img_SAP_array.append(salt_and_pepper(img, p))
    cv2.imwrite("assets/aritmetic_pixelwise/salt_and_pepper.jpg", img_SAP_array[0])

    img_mean = mean_filter(img_SAP_array)
    cv2.imwrite("assets/aritmetic_pixelwise/mean_filter.jpg", img_mean)

    img_median = mean_filter(img_SAP_array)
    cv2.imwrite("assets/aritmetic_pixelwise/median_filter.jpg", img_median)

    print("Images saves on folder 'assets/aritmetic_pixelwise'.")


def intensity_transformations():
    img = cv2.imread("assets/intensity_transformations/return_of_the_jedi.webp")

    img_gray = gray_scale(img)
    cv2.imwrite("assets/intensity_transformations/gray_scale.jpg", img_gray)

    img_inversion = color_inversion(img)
    cv2.imwrite("assets/intensity_transformations/color_inversion.jpg", img_inversion)

    bright = int(input("Enter to bright value: "))
    img_bright = color_bright(img, bright)
    cv2.imwrite("assets/intensity_transformations/color_bright.jpg", img_bright)

    constrast = int(input("Enter to constrast value: "))
    img_contrast = color_contrast(img, constrast)
    cv2.imwrite("assets/intensity_transformations/color_contrast.jpg", img_contrast)

    threshold = int(input("Enter to threhsold value: "))
    img_threshold = thresholding(img_gray, threshold)
    cv2.imwrite(
        "assets/intensity_transformations/color_thresholding.jpg", img_threshold
    )

    img_RGBtoHSL = convertRGBtoHSL(img)
    cv2.imwrite("assets/intensity_transformations/color_RGBtoHSL.jpg", img_RGBtoHSL)

    img_HSLtoRGB = convertHSLtoRGB(img_RGBtoHSL)
    cv2.imwrite("assets/intensity_transformations/color_HSLtoRGB.jpg", img_HSLtoRGB)

    print("Images saves on folder 'assets/intensity_transformations'.")


def interpolation():
    img = cv2.imread("assets/interpolation/steve.webp")
    r = float(input("Enter to scale value: "))

    img_nn = nearestNeighbor(img, r)
    cv2.imwrite("assets/interpolation/nearest_neighboor.jpg", img_nn)

    img_bilinear = bilinear(img, r)
    cv2.imwrite("assets/interpolation/bilinear.jpg", img_bilinear)

    img_bicubic = bicubic(img, r)
    cv2.imwrite("assets/interpolation/bicubic.jpg", img_bicubic)
    print("Images saves on folder 'assets/interpolation'.")


if __name__ == "__main__":
    # interpolation()
    # intensity_transformations()
    # aritmetic_pixelwise()
    # histograms()
    spatial_filters()
