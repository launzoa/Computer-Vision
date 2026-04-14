import cv2
from src.interpolation import nearestNeighboor, bilinear


def interpolation():
    img = cv2.imread("assets/interpolation/flowers.webp")
    r = float(input("Entre com o valor de escala:"))

    img_nn = nearestNeighboor(img, r)
    cv2.imwrite("assets/interpolation/nearest_neighboor.jpg", img_nn)

    img_bilinear = bilinear(img, r)
    cv2.imwrite("assets/interpolation/bilinear.jpg", img_bilinear)

    print("Imagens salvas na pasta 'interpolation'")


if __name__ == "__main__":
    interpolation()
