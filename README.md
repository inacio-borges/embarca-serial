# Projeto de Display SSD1306 e Controle de LEDs com Raspberry Pi Pico

Aluno: Inacio Oliveira Borges

link para o video da execucao do projeto: [Projeto em execucao](https://youtu.be/DyWpsKNwqAA)

## Descrição do Projeto

Este projeto utiliza um microcontrolador RP2040 para controlar LEDs e exibir informações em um display OLED SSD1306. Ele também permite interações com botões 

Neste projeto, você deverá utilizar os seguintes componentes conectados à placa BitDogLab:
- Matriz 5x5 de LEDs (endereçáveis) WS2812, conectada à GPIO 7.
- LED RGB, com os pinos conectados às GPIOs (11, 12 e 13).
- Botão A conectado à GPIO 5.
- Botão B conectado à GPIO 6.
- Display SSD1306 conectado via I2C (GPIO 14 e GPIO 

## Funcionalidades do Projeto

1. **Modificação da Biblioteca font.h**
   - Adicionados caracteres minúsculos à biblioteca font.h.

2. **Entrada de Caracteres via PC**
   - Utilizando o Serial Monitor do VS Code, é possível digitar caracteres que são exibidos no display SSD1306.
   - Apenas um caractere é enviado de cada vez, e números entre 0 e 9 são exibidos na matriz 5x5 WS2812.

3. **Interação com o Botão A**
   - Pressionar o botão A alterna o estado do LED RGB Verde (ligado/desligado).
   - A operação é registrada com uma mensagem no display SSD1306 e um texto descritivo enviado ao Serial Monitor.

4. **Interação com o Botão B**
   - Pressionar o botão B alterna o estado do LED RGB Azul (ligado/desligado).
   - A operação é registrada com uma mensagem no display SSD1306 e um texto descritivo enviado ao Serial Monitor.

## Como Funciona

### Inicialização

- **Configuração dos GPIOs**: Os pinos GPIO são configurados para controlar os LEDs RGB, a matriz de LEDs e os botões.
- **Configuração do Display**: O display SSD1306 é inicializado e configurado para comunicação via I2C.
- **Configuração das Interrupções**: As interrupções para os botões A e B são configuradas para detectar bordas de descida.

### Modos de Operação

1. **Entrada de Caracteres**:
   - Cada caractere digitado no Serial Monitor é exibido no display SSD1306.
   - Se um número entre 0 e 9 for digitado, a matriz 5x5 WS2812 exibe o símbolo correspondente ao número.

2. **Botão A**:
   - Alterna o estado do LED RGB Verde.
   - Exibe uma mensagem informativa no display SSD1306.
   - Envia uma mensagem descritiva ao Serial Monitor.

3. **Botão B**:
   - Alterna o estado do LED RGB Azul.
   - Exibe uma mensagem informativa no display SSD1306.
   - Envia uma mensagem descritiva ao Serial Monitor.

### Tratamento de Debouncing

- Implementado via software para garantir a leitura precisa dos botões.

### Controle da Matriz de LEDs

- A matriz 5x5 WS2812 é controlada para exibir números conforme 