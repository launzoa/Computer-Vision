Um simples programa introdutório em C++ utilizando a biblioteca OpenCV para abrir e manipular imagens. O objetivo principal é por em prática os fundamentos da manipulação direta de pixels e operações do livro de processamento digital de imagens do Gonzalez.

O programa carrega uma imagem base e abre uma janela que exibe a imagem original e a versão modificada lado a lado. Através do teclado, é possível aplicar os seguintes filtros em tempo real:

* **`1` - Escala de Cinza (Grayscale):** Converte a imagem colorida calculando a média aritmética dos canais BGR de cada pixel.
* **`2` - Inversão em Tons de Cinza:** Aplica o efeito de negativo fotográfico sobre a imagem já convertida para tons de cinza.
* **`3` - Inversão de Cores (Negativo):** Inverte os valores de cada canal de cor (RGB) subtraindo o valor atual de 255.
* **`Espaço` - Salvar/Acumular Efeito:** Retorna para a imagem original.
* **`ESC` - Sair:** Encerra a aplicação de forma segura.
