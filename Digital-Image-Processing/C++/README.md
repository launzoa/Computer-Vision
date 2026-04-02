### Pré-requisitos

* Compilador C++ (GCC/Clang)
* CMake
* OpenCV 4.x instalada no sistema

### Instalação

1. Clone o repositório e navegue até a pasta:

   ```bash
   git clone [https://github.com/launzoa/Computer-Vision.git] 
   cd Digital-Image-Processing/C++

2. Gere os arquivos de compilação na pasta build (é preciso excluir os arquivos antes):

  ``` bash
  cmake ..
  ```

1. Volte para a pasta principal:

  ```bash
  cd ..
  ```

1. Compile e linke o executável:

  ``` bash
  make -C build
  ```

1. Execute o app:

  ``` bash
  ./build/app
  ```
