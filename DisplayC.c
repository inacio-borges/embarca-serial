#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "inc/ssd1306.h"
#include "inc/font.h"
#include <stdio.h>
#include <math.h>
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/adc.h"
#include "pico/bootrom.h"

#include "main.pio.h"

// número de LEDs
#define NUM_PIXELS 25
// pino de saída
#define OUT_PIN 7

#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C

#define LED_VERMELHO 13
#define LED_AZUL 12
#define LED_VERDE 11

#define BOTAO_A 5
#define BOTAO_B 6

ssd1306_t ssd; // Inicializa a estrutura do display

int numero_matriz;
// numeros na matriz de led
double numero[10][25] = {
    {1.0, 1.0, 1.0, 1.0, 1.0,
     1.0, 0.0, 0.0, 0.0, 1.0,
     1.0, 0.0, 0.0, 0.0, 1.0,
     1.0, 0.0, 0.0, 0.0, 1.0,
     1.0, 1.0, 1.0, 1.0, 1.0}, // 0

    {0.0, 0.0, 1.0, 0.0, 0.0,
     0.0, 0.0, 1.0, 0.0, 0.0,
     0.0, 0.0, 1.0, 0.0, 0.0,
     0.0, 0.0, 1.0, 0.0, 0.0,
     0.0, 0.0, 1.0, 0.0, 0.0}, // 1

    {1.0, 1.0, 1.0, 1.0, 1.0,
     1.0, 0.0, 0.0, 0.0, 0.0,
     1.0, 1.0, 1.0, 1.0, 1.0,
     0.0, 0.0, 0.0, 0.0, 1.0,
     1.0, 1.0, 1.0, 1.0, 1.0}, // 2

    {1.0, 1.0, 1.0, 1.0, 1.0,
     1.0, 0.0, 0.0, 0.0, 0.0,
     1.0, 1.0, 1.0, 1.0, 1.0,
     1.0, 0.0, 0.0, 0.0, 0.0,
     1.0, 1.0, 1.0, 1.0, 1.0}, // 3

    {1.0, 0.0, 0.0, 0.0, 1.0,
     1.0, 0.0, 0.0, 0.0, 1.0,
     1.0, 1.0, 1.0, 1.0, 1.0,
     1.0, 0.0, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 0.0, 1.0}, // 4

    {1.0, 1.0, 1.0, 1.0, 1.0,
     0.0, 0.0, 0.0, 0.0, 1.0,
     1.0, 1.0, 1.0, 1.0, 1.0,
     1.0, 0.0, 0.0, 0.0, 0.0,
     1.0, 1.0, 1.0, 1.0, 1.0}, // 5

    {1.0, 1.0, 1.0, 1.0, 1.0,
     0.0, 0.0, 0.0, 0.0, 1.0,
     1.0, 1.0, 1.0, 1.0, 1.0,
     1.0, 0.0, 0.0, 0.0, 1.0,
     1.0, 1.0, 1.0, 1.0, 1.0}, // 6

    {1.0, 1.0, 1.0, 1.0, 1.0,
     1.0, 0.0, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 1.0, 0.0,
     0.0, 0.0, 1.0, 0.0, 0.0,
     0.0, 1.0, 0.0, 0.0, 0.0}, // 7

    {1.0, 1.0, 1.0, 1.0, 1.0,
     1.0, 0.0, 0.0, 0.0, 1.0,
     1.0, 1.0, 1.0, 1.0, 1.0,
     1.0, 0.0, 0.0, 0.0, 1.0,
     1.0, 1.0, 1.0, 1.0, 1.0}, // 8

    {1.0, 1.0, 1.0, 1.0, 1.0,
     1.0, 0.0, 0.0, 0.0, 1.0,
     1.0, 1.0, 1.0, 1.0, 1.0,
     1.0, 0.0, 0.0, 0.0, 0.0,
     1.0, 1.0, 1.0, 1.0, 1.0} // 9
};

bool debounce()
{
  static uint32_t ultimo_tempo_botao = 0;
  uint32_t tempo_atual = to_ms_since_boot(get_absolute_time());

  if (tempo_atual - ultimo_tempo_botao < 200)
  { // 200ms de debounce
    return false;
  }

  ultimo_tempo_botao = tempo_atual;
  return true;
}

bool flagA = true;
void botao_A_callback(uint gpio, uint32_t eventos)
{

  gpio_acknowledge_irq(gpio, eventos); // Garante que a interrupção foi reconhecida

  if (!debounce())
    return;

  printf("botao A pressionado \n");

  ssd1306_fill(&ssd, false); // Limpa o display
  ssd1306_draw_string(&ssd, "Botao A Pressionado", 0, 32);
  ssd1306_send_data(&ssd); // Atualiza o display
  flagA = !flagA;
  if (!flagA)
  {
    gpio_put(LED_VERDE, 1);
    ssd1306_draw_string(&ssd, "LED Verde ON", 0, 0);
    ssd1306_send_data(&ssd); // Atualiza o display
  }
  else
  {
    gpio_put(LED_VERDE, 0);
    ssd1306_draw_string(&ssd, "LED Verde OFF", 0, 0);
    ssd1306_send_data(&ssd); // Atualiza o display
  }
 
}

bool flagB = true;
void botao_B_callback(uint gpio, uint32_t eventos)
{
  gpio_acknowledge_irq(gpio, eventos); // Garante que a interrupção foi reconhecida

  if (!debounce())
    return;

  ssd1306_fill(&ssd, false); // Limpa o display
  ssd1306_draw_string(&ssd, "Botao B Pressionado", 0, 32);
  ssd1306_send_data(&ssd); // Atualiza o display

  printf("botao B pressionado \n");

  flagB = !flagB;
  if (!flagB)
  {
    gpio_put(LED_AZUL, 1);
    ssd1306_draw_string(&ssd, "LED Azul ON", 0, 0);
    ssd1306_send_data(&ssd); // Atualiza o display
  }
  else
  {
    gpio_put(LED_AZUL, 0);
    ssd1306_draw_string(&ssd, "LED Azul OFF", 0, 0);
    ssd1306_send_data(&ssd); // Atualiza o display
  }
}

// rotina para definição da intensidade de cores do led
uint32_t matrix_rgb(double r, double g, double b)
{
  unsigned char R, G, B;
  R = r * 10;
  G = g * 10;
  B = b * 10;
  return (G << 24) | (R << 16) | (B << 8);
}

void gpio_callback(uint gpio, uint32_t events)
{
  if (gpio == BOTAO_A)
  {
    botao_A_callback(gpio, events);
  }
  else if (gpio == BOTAO_B)
  {
    botao_B_callback(gpio, events);
  }
}

int main()
{
  numero_matriz = 0;

  PIO pio = pio0;
  bool ok;
  uint16_t i;
  uint32_t valor_led;
  double r = 0.0, b = 0.0, g = 0.0;

  // configurações da PIO
  uint offset = pio_add_program(pio, &main_program);
  uint sm = pio_claim_unused_sm(pio, true);
  main_program_init(pio, sm, offset, OUT_PIN);

  stdio_init_all(); // Inicializa comunicação USB CDC para monitor serial

  // Inicialização dos GPIOs
  gpio_init(LED_AZUL);
  gpio_init(LED_VERMELHO);
  gpio_init(LED_VERDE);
  gpio_init(BOTAO_A);
  gpio_init(BOTAO_B);

  // Configuração da direção dos GPIOs
  gpio_set_dir(LED_AZUL, GPIO_OUT);
  gpio_set_dir(LED_VERMELHO, GPIO_OUT);
  gpio_set_dir(LED_VERDE, GPIO_OUT);
  gpio_set_dir(BOTAO_A, GPIO_IN);
  gpio_set_dir(BOTAO_B, GPIO_IN);

  // Configuração do pull-up para o botão
  gpio_pull_up(BOTAO_A);
  gpio_pull_up(BOTAO_B);

  // Configuração da interrupção do botão (detecta borda de descida)
  gpio_set_irq_enabled_with_callback(BOTAO_A,
                                     GPIO_IRQ_EDGE_FALL,
                                     true,
                                     &gpio_callback);

  gpio_set_irq_enabled_with_callback(BOTAO_B,
                                     GPIO_IRQ_EDGE_FALL,
                                     true,
                                     &gpio_callback);

  // Estado inicial: todos os LEDs apagados
  gpio_put(LED_AZUL, 0);
  gpio_put(LED_VERMELHO, 0);
  gpio_put(LED_VERDE, 0);

  // I2C Initialisation. Using it at 400Khz.
  i2c_init(I2C_PORT, 400 * 1000);

  gpio_set_function(I2C_SDA, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
  gpio_set_function(I2C_SCL, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
  gpio_pull_up(I2C_SDA);                     // Pull up the data line
  gpio_pull_up(I2C_SCL);                     // Pull up the clock line

  ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display
  ssd1306_config(&ssd);                                         // Configura o display
  ssd1306_send_data(&ssd);                                      // Envia os dados para o display

  // Limpa o display. O display inicia com todos os pixels apagados.
  ssd1306_fill(&ssd, false);
  ssd1306_send_data(&ssd);

  bool cor = true;

  for (int i = 0; i < 10; i++)
  {
    for (int j = 0; j < NUM_PIXELS; j++)
    {
      valor_led = matrix_rgb(b, r = numero[numero_matriz][24 - j], g); // Usa o padrão atual
      pio_sm_put_blocking(pio, sm, valor_led);
    }
  }

  while (true)
  {
    printf("RP2040 inicializado. Envie um caractere\n");

    char c;
    if (scanf("%c", &c) == 1)
    { // Lê caractere da entrada padrão
      printf("Recebido: '%c'\n", c);
    }

    switch (c)
    {
    case '0':
      numero_matriz = 0;
      break;
    case '1':
      numero_matriz = 1;
      break;
    case '2':
      numero_matriz = 2;
      break;
    case '3':
      numero_matriz = 3;
      break;
    case '4':
      numero_matriz = 4;
      break;
    case '5':
      numero_matriz = 5;
      break;
    case '6':
      numero_matriz = 6;
      break;
    case '7':
      numero_matriz = 7;
      break;
    case '8':
      numero_matriz = 8;
      break;
    case '9':
      numero_matriz = 9;
      break;
    default:
      break;
    }

    // imprimir numero na matriz de leds
    for (int i = 0; i < 10; i++)
    {
      for (int j = 0; j < NUM_PIXELS; j++)
      {
        valor_led = matrix_rgb(b, r = numero[numero_matriz][24 - j], g); // Usa o padrão atual
        pio_sm_put_blocking(pio, sm, valor_led);
      }
    }

    cor = !cor;
    // Atualiza o conteúdo do display com animações

    ssd1306_fill(&ssd, false);          // Limpa o display
    ssd1306_draw_char(&ssd, c, 64, 32); // Desenha um char
    ssd1306_send_data(&ssd);            // Atualiza o display

    sleep_ms(500);
  }
}