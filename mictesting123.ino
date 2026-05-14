#include <driver/i2s.h>

#define I2S_WS 42
#define I2S_SD 41

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Starting mic...");

  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = 16000,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_STAND_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 4,
    .dma_buf_len = 256,
    .use_apll = false,
    .tx_desc_auto_clear = false,
    .fixed_mclk = 0
  };

  i2s_pin_config_t pin_config = {
    .bck_io_num = -1,
    .ws_io_num = I2S_WS,
    .data_out_num = -1,
    .data_in_num = I2S_SD
  };

  esp_err_t err;

  err = i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
  if (err != ESP_OK) {
    Serial.println("I2S driver install failed");
    return;
  }

  err = i2s_set_pin(I2S_NUM_0, &pin_config);
  if (err != ESP_OK) {
    Serial.println("I2S pin config failed");
    return;
  }

  Serial.println("Mic started!");
}

void loop() {
  int32_t sample = 0;
  size_t bytes_read = 0;

  esp_err_t result = i2s_read(
    I2S_NUM_0,
    &sample,
    sizeof(sample),
    &bytes_read,
    100
  );

  if (result == ESP_OK && bytes_read > 0) {
    Serial.println(sample);
  }
}
