import cv2
from src.interpolation import nearestNeighbor, bilinear, bicubic
from src.intensity_transformations import (
    gray_scale,
    color_inversion,
    color_bright,
    color_contrast,
    thresholding,
    convertRGBtoHSL,
    convertHSLtoRGB,
)


def intensity_transformations():
    img = cv2.imread("assets/intensity_transformations/return_of_the_jedi.webp")

    """
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

    """
    img_RGBtoHSL = convertRGBtoHSL(img)
    cv2.imwrite("assets/intensity_transformations/color_RGBtoHSL.jpg", img_RGBtoHSL)

    img_HSLtoRGB = convertHSLtoRGB(img_RGBtoHSL)
    cv2.imwrite("assets/intensity_transformations/color_HSLtoRGB.jpg", img_HSLtoRGB)

    print("Images saves on folder 'assets/intensity_transformations'.")


def interpolation():
    img = cv2.imread("assets/interpolation/flowers.webp")
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
    intensity_transformations()
