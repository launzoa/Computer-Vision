**Limiarização Invertida (Inverse Thresholding):** Converte a imagem para um formato binário (preto e branco) a partir de um valor de corte (limiar) predefinido. O algoritmo calcula primeiro a intensidade em tons de cinza do pixel através da média aritmética de seus canais BGR. Em seguida, aplica a seguinte função condicional (onde $f(x,y)$ é a intensidade original e $g(x,y)$ é o pixel resultante):
$$
g(x,y) = \begin{cases} 0 & \text{se } f(x,y) > \text{limiar} \\ 255 & \text{caso contrário} \end{cases}
$$
  
Esta técnica atribui o valor preto (0) aos pixels mais claros que o limiar e branco (255) aos pixels mais escuros. É uma operação fundamental para a segmentação de imagens, permitindo isolar objetos de interesse do fundo com base na diferença de suas intensidades luminosas.
