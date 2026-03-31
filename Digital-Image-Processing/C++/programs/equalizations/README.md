* **Frequência Cumulativa (Cumulative Distribution Function - CDF):** O algoritmo realiza uma varredura inicial na imagem para construir o histograma de tons de cinza (convertendo os canais BGR através de média aritmética). Em seguida, calcula a soma cumulativa das ocorrências de cada nível de intensidade de 0 a 255. Esta etapa representa a aproximação discreta da função de distribuição acumulada da imagem original.
* **Equalização de Histograma:** O processo principal que mapeia os níveis de cinza originais para novos valores, buscando uma distribuição de intensidades mais uniforme ("plana"). O algoritmo percorre a imagem uma segunda vez e aplica a função de transformação aos pixels originais, definida matematicamente pela fórmula:

$$
s = \text{round}\left( \frac{L-1}{M \times N} \times \text{cdf}(r) \right)
$$

Onde $s$ é a nova intensidade equalizada, $r$ é a intensidade original do pixel, $L-1 = 255$ é o nível máximo de cinza, $M \times N$ representa o número total de pixels da imagem (área) e $\text{cdf}(r)$ é o valor da frequência cumulativa para aquele tom de cinza específico. Esta técnica aumenta o contraste global da imagem, revelando detalhes em áreas que originalmente se apresentavam sub ou superexpostas.

O algoritmo simplifica o processamento realizando a equalização diretamente sobre uma estimativa em tons de cinza de uma imagem colorida, replicando o valor equalizado ($s$) de volta para os três canais BGR do pixel de saída.
