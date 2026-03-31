# Interpolação

Este repositório contém a implementação em C++ (puro, ou quase) dos três algoritmos fundamentais de interpolação para o redimensionamento de imagens.

A biblioteca **OpenCV** é utilizada de forma estrita e exclusiva para operações de I/O (abertura das imagens) e interface gráfica (exibição das matrizes). Todo o núcleo matemático e o mapeamento inverso de memória foram escritos do zero.

---

A interpolação é o processo matemático de estimar valores em coordenadas fracionárias contínuas dentro de uma grade discreta, utilizado sempre que aplicamos transformações geométricas (como escala ou rotação) em uma imagem digital.

Os seguintes algoritmos foram implementados utilizando a técnica de **Mapeamento Inverso** para evitar lacunas na matriz de destino:

* **Vizinho Mais Próximo (Nearest Neighbor):** Abordagem que arredonda a coordenada geométrica flutuante para o índice inteiro mais próximo. Devido à sua natureza de truncamento bruto, pode introduzir artefatos severos de *aliasing* (efeito quadriculado/serrilhado) em altas taxas de escala.

* **Interpolação Bilinear:** Extrai a vizinhança geométrica de $2 \times 2$ pixels (4 vizinhos) ao redor da coordenada. Utiliza as distâncias fracionárias ($dx, dy$) como pesos para calcular uma média ponderada linear. O resultado atua como um filtro passa-baixa, eliminando o serrilhado, mas introduzindo um leve borramento nas bordas.

* **Interpolação Bicúbica:** O padrão-ouro para redimensionamento de alta qualidade. Avalia uma vizinhança de $4 \times 4$ pixels (16 vizinhos) e ajusta uma superfície baseada em polinômios de grau 3 (splines cúbicos). A equação de peso permite coeficientes negativos, o que injeta contraste artificial nas bordas, preservando a nitidez geométrica da imagem original.

---

Este projeto utiliza o **CMake** para o gerenciamento do processo de *build* e *linkagem*.

### Pré-requisitos

* Compilador C++ (GCC/Clang)
* CMake
* OpenCV 4.x instalada no sistema

### Instalação

1. Clone o repositório e navegue até a pasta:

   ```bash
   git clone [https://github.com/launzoa/Computer-Vision.git] 
   cd Digital-Image-Processing/interpolation

2. Gere os arquivos de compilação na pasta build:

  ``` bash
  cmake -B build
  ```

1. Compile e linke o executável:

  ``` bash
  make -C build
  ```

1. Execute o app:

  ``` bash
  ./build/app
  ```
