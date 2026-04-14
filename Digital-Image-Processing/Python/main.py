import cv2
from src.interpolation import nearestNeighbor, bilinear, bicubic


def interpolation():
    img = cv2.imread("assets/interpolation/flowers.webp")
    r = float(input("Entre com o valor de escala:"))

    img_nn = nearestNeighbor(img, r)
    cv2.imwrite("assets/interpolation/nearest_neighboor.jpg", img_nn)

    img_bilinear = bilinear(img, r)
    cv2.imwrite("assets/interpolation/bilinear.jpg", img_bilinear)

    img_bicubic = bicubic(img, r)
    cv2.imwrite("assets/interpolation/bicubic.jpg", img_bicubic)
    print("Imagens salvas na pasta 'interpolation'")


if __name__ == "__main__":
    interpolation()
