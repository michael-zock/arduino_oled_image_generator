#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <Wire.h>

const uint8_t display_width = 128;
const uint8_t display_height = 32;
const int display_buffer_size = (display_width * display_height) / 8;
Adafruit_SSD1306 display(display_width, display_height, &Wire);

uint16_t bitmap_angle = 0;
const uint8_t bitmap[] PROGMEM = {
    0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x0f, 0xf0, 0x00,
    0x00, 0x79, 0x9e, 0x00, 0x00, 0xc1, 0x83, 0x00, 0x01, 0x81, 0x81, 0x80, 0x03, 0x0f, 0xf0, 0xc0,
    0x06, 0x39, 0x9c, 0x60, 0x0c, 0x61, 0x86, 0x30, 0x08, 0xc0, 0x03, 0x10, 0x08, 0x80, 0x01, 0x10,
    0x19, 0x80, 0x01, 0x98, 0x11, 0x00, 0x00, 0x88, 0x11, 0x01, 0x80, 0x88, 0xff, 0xc3, 0xc3, 0xff,
    0xff, 0xc3, 0xc3, 0xff, 0x11, 0x01, 0x80, 0x88, 0x11, 0x00, 0x00, 0x88, 0x19, 0x80, 0x01, 0x98,
    0x08, 0x80, 0x01, 0x10, 0x08, 0xc0, 0x03, 0x10, 0x0c, 0x61, 0x86, 0x30, 0x06, 0x39, 0x9c, 0x60,
    0x03, 0x0f, 0xf0, 0xc0, 0x01, 0x81, 0x81, 0x80, 0x00, 0xc1, 0x83, 0x00, 0x00, 0x79, 0x9e, 0x00,
    0x00, 0x0f, 0xf0, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00};

void setup() {
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  delay(100);
  display.clearDisplay();
}

void loop() {
  for (int i = 0; i < 25; i++) {
    drawWhiteNoise();
  }

  animateRandomWaves();
  animateRandomWaveform();
  animateRandomGraph();
  animateRandomText();

  displayTargetGrid();
  delay(1000);

  drawVerticalRandomBarChart(0, 0, display_width / 2 - 1, display_height, 6, 2);
  drawHorizontalRandomBarChart(display_width / 2 + 1, 0, display_width / 2, display_height, 4, 1);
  delay(1000);

  for (uint16_t angle = 0; angle < 360; angle += 6) {
    display.clearDisplay();
    drawRotatedBitmap((display_width - 32) * 0.1, 0, bitmap, 32, 32, WHITE, angle);
    drawRotatedBitmap((display_width - 32) * 0.5, 0, bitmap, 32, 32, WHITE, angle);
    drawRotatedBitmap((display_width - 32) * 0.9, 0, bitmap, 32, 32, WHITE, angle);
    display.display(); //sends the buffer to the OLED
  }

  display.clearDisplay();
  for (int percentage = 0; percentage <= 100; percentage += 10) {
    drawHorizontalBarGauge(0, 0, display_width * .75, display_height, WHITE, WHITE, percentage);
    drawVerticalBarGauge(display_width * .8, 0, display_width * .2, display_height, WHITE, WHITE, percentage);
    delay(50);
  }
  for (int percentage = 100; percentage > 0; percentage -= 10) {
    drawHorizontalBarGauge(0, 0, display_width * .75, display_height, WHITE, WHITE, percentage);
    drawVerticalBarGauge(display_width * .8, 0, display_width * .2, display_height, WHITE, WHITE, percentage);
    delay(50);
  }

  display.clearDisplay();
  for (int percentage = 0; percentage <= 100; percentage += 10) {
    drawHorizontalTriangleGauge(0, 0, display_width * .75, display_height, WHITE, WHITE, percentage);
    drawVerticalTriangleGauge(display_width * .8, 0, display_width * .2, display_height, WHITE, WHITE, percentage);
    delay(50);
  }
  for (int percentage = 100; percentage > 0; percentage -= 10) {
    drawHorizontalTriangleGauge(0, 0, display_width * .75, display_height, WHITE, WHITE, percentage);
    drawVerticalTriangleGauge(display_width * .8, 0, display_width * .2, display_height, WHITE, WHITE, percentage);
    delay(50);
  }

  display.clearDisplay();
  for (int i = 0; i < 25; i++) {
    drawCircularGauge(display_width / 2 - display_height / 2, 0, display_height, display_height, 3, 0, random(10, 90), WHITE);
  }

  display.clearDisplay();
  for (int phase = 0; phase < 90; phase++) {
    drawSineWave(0.8, 5, phase);
  }

  display.clearDisplay();
  for (int phase = 0; phase < 90; phase++) {
    drawCompositeWave(2, 2, 0.5, 3, 0.1, 6, phase);
  }

  display.clearDisplay();
  for (int phase = 0; phase < 90; phase++) {
    drawFakeHeartbeat(phase);
    delay(50);
  }

  display.clearDisplay();
  for (int phase = 0; phase < 90; phase++) {
    drawSquareWave(0.6, 10, phase);
  }
}

void drawWhiteNoise() {
  uint8_t *p = display.getBuffer();
  for (int i = 0; i < display_buffer_size; i++) {
    p[i] = random(0, 255); //write 8-pixels direct to buffer
  }
  drawBorder();
  display.display(); //sends the buffer to the OLED
}

void drawBorder() {
  display.drawRect(0, 0, display_width, display_height, WHITE);
}

void drawGrid() {
  drawBorder();

  display.drawLine(display_width * 0.50, 0, display_width * 0.50, display_height, WHITE);
  if (display_width > display_height) {
    display.drawLine(display_width * 0.25, 0, display_width * 0.25, display_height, WHITE);
    display.drawLine(display_width * 0.75, 0, display_width * 0.75, display_height, WHITE);
  }

  display.drawLine(0, display_height * 0.50, display_width, display_height * 0.50, WHITE);
  if (display_height > display_width) {
    display.drawLine(0, display_height * 0.25, display_width, display_height * 0.25, WHITE);
    display.drawLine(0, display_height * 0.75, display_width, display_height * 0.75, WHITE);
  }
}

void animateRandomWaves() {
  int old_x, old_cos, old_sin, new_cos, new_sin, vCenter = display_height / 2;
  uint8_t Radius = random(display_height / 16, display_height / 2);
  float offset = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 4));

  display.clearDisplay();
  drawGrid();

  for (int new_x = 0; new_x < display_width; new_x++) {
    float angle = (new_x * 3) * offset;
    new_cos = (vCenter + (sin(angle * (M_PI / 180)) * Radius) * offset);
    new_sin = (vCenter + (cos(angle * (M_PI / 180)) * Radius) * offset);

    if (new_x == 0) {
      display.drawPixel(new_x, new_cos, WHITE);
      display.drawPixel(new_x, new_sin, WHITE);
    } else {
      display.drawLine(old_x, old_cos, new_x, new_cos, WHITE);
      display.drawLine(old_x, old_sin, new_x, new_sin, WHITE);
    }

    old_x = new_x;
    old_sin = min(max(new_sin, 0), display_height);
    old_cos = min(max(new_cos, 0), display_height);

    display.display();
  }
}

void drawSineWave(float amplitude, float frequency, float phase) {
  //  amplitude: vertical compression (1 = screen height)
  //  frequency: horizontal compression (1 = screen width)
  //  phase: horitontal shift (in degrees)

  int old_x, old_y, new_y;
  uint8_t vCenter = display_height / 2;

  display.clearDisplay();
  drawGrid();

  for (int new_x = 0; new_x < display_width; new_x++) {
    float value = amplitude * sin((frequency * M_PI / 180 * new_x) + phase);
    new_y = (value * display_height + display_height) / 2;

    new_y = min(max(new_y, 0), display_height - 1);

    if (new_x == 0) {
      display.drawPixel(new_x, new_y, WHITE);
    } else {
      display.drawLine(old_x, old_y, new_x, new_y, WHITE);
    }

    old_x = new_x;
    old_y = new_y;
  }
  display.display(); //sends the buffer to the OLED
}

void drawCompositeWave(float amplitude1, float frequency1, float amplitude2, float frequency2, float amplitude3, float frequency3, float phase) {
  //  amplitude: vertical compression (1 = screen height)
  //  frequency: horizontal compression (1 = screen width)
  //  phase: horitontal shift (in degrees)

  int old_x, old_y, new_y;
  uint8_t vCenter = display_height / 2;

  display.clearDisplay();
  drawGrid();

  for (int new_x = 0; new_x < display_width; new_x++) {
    float value1 = amplitude1 * sin((frequency1 * M_PI / 180 * new_x) + phase);
    float value2 = amplitude2 * cos((frequency2 * M_PI / 180 * new_x) + phase);
    float value3 = amplitude3 * tan((frequency3 * M_PI / 180 * new_x) + phase);
    float value = value1 * value2 * value3;
    new_y = (value * display_height + display_height) / 2;

    new_y = min(max(new_y, 0), display_height - 1);

    if (new_x == 0) {
      display.drawPixel(new_x, new_y, WHITE);
    } else {
      display.drawLine(old_x, old_y, new_x, new_y, WHITE);
    }

    old_x = new_x;
    old_y = new_y;
  }
  display.display(); //sends the buffer to the OLED
}

void drawFakeHeartbeat(float phase) {
  //  phase: horitontal shift (in degrees)

  int old_x, old_y, new_y;
  uint8_t vCenter = display_height / 2;

  display.clearDisplay();
  //drawGrid();

  for (int new_x = 0; new_x < display_width; new_x++) {
    float value1 = pow(sin(new_x), 63) * 360;
    float value2 = pow(sin(new_x + 1.5), 8) * 360;
    float value = value1 * value2;

    new_y = (value * display_height + display_height) / 2;
    new_y = min(max(new_y, 0), display_height - 1);

    if (new_x == 0) {
      display.drawPixel(new_x, new_y, WHITE);
    } else {
      display.drawLine(old_x, old_y, new_x, new_y, WHITE);
    }

    old_x = new_x;
    old_y = new_y;
  }
  display.display(); //sends the buffer to the OLED
}

void drawSquareWave(float amplitude, float frequency, float phase) {
  //  amplitude: vertical compression (1 = screen height)
  //  frequency: horizontal compression (1 = screen width)
  //  phase: horitontal shift (in degrees)

  int old_x, old_y, new_y;
  uint8_t vCenter = display_height / 2;

  display.clearDisplay();
  drawGrid();

  for (int new_x = 0; new_x < display_width; new_x++) {
    float value = amplitude * sin((frequency * M_PI / 180 * new_x) + phase);
    value = ((value > 0) ? 1 : -1);

    new_y = vCenter + (value * vCenter * amplitude);
    new_y = min(max(new_y, 0), display_height - 1);

    if (new_x == 0) {
      display.drawPixel(new_x, new_y, WHITE);
    } else {
      display.drawLine(old_x, old_y, new_x, new_y, WHITE);
    }

    old_x = new_x;
    old_y = new_y;
  }
  display.display(); //sends the buffer to the OLED
}

void animateRandomWaveform() {
  uint8_t vCenter = display_height / 2;
  display.clearDisplay();

  drawGrid();

  for (int i = 0; i < display_width; i++) {
    uint8_t val = random(0, vCenter);
    display.drawLine(i, vCenter - val, i, vCenter + val, WHITE);
    display.display(); //sends the buffer to the OLED
  }
}

void animateRandomGraph() {

  uint8_t prev_x = 0;
  uint8_t prev_val = 0;
  display.clearDisplay();

  drawGrid();

  for (int i = 0; i < display_width; i++) {
    uint8_t val = random(0, display_height);
    if (i == 0 || i == display_width || (random(0, 64) % 4 == 0)) {
      display.drawLine(prev_x, prev_val, i, val, WHITE);
      prev_x = i;
      prev_val = val;
    } else {
      delay(5);
    }
    display.display(); //sends the buffer to the OLED
  }
}

void animateRandomText() {
  uint8_t border = 3;
  display.clearDisplay();
  drawBorder();

  for (int y = border; y < (display_height - border); y = (y + random(1, 3))) {
    if ((random(1, 15) % 4) == 0) {
      continue;
    }

    for (int x = border; x < display_width - border - random(1, (display_width / 4)); x++) {
      int x_end = x + random(2, 16);
      display.drawLine(x, y, x_end, y, WHITE);
      x = x_end + 1;
      display.display(); //sends the buffer to the OLED
    }
  }
}

void drawRotatedBitmap(uint8_t x, uint8_t y, const uint8_t *bitmap, uint8_t width, uint8_t height, uint8_t color, int angle) {
  int8_t old_x, old_y, new_x, new_y; // old and new (rotated) Pixel-Coordinates
  uint8_t pivot_x = width / 2;       // Calculate the (rotation) center of the image (x fraction)
  uint8_t pivot_y = height / 2;      // Calculate the (rotation) center of the image (y fraction)
  float angle_rad = angle / 57.3;
  float sin_angle = sin(angle_rad); // Lookup the Sine
  float cos_angle = cos(angle_rad); // Lookup the coSine
  for (int row = 0; row < height; row++) {
    uint8_t displayData, mask = 0;
    for (int col = 0; col < width; col++) {
      if (mask == 0) {
        displayData = pgm_read_byte(bitmap++); // Read the image data from PROGMEM
        mask = 0x80;                           // MSB first
      }
      if (displayData & mask) {                               // i.e. a pixel
        old_x = col - pivot_x;                                // Calculate the x-position of the Pixel to be rotated
        old_y = row - pivot_y;                                // Calculate the y-position of the Pixel to be rotated
        new_x = (int)(old_x * cos_angle - old_y * sin_angle); // Calculate the x-position of the rotated pixel
        new_y = (int)(old_y * cos_angle + old_x * sin_angle); // Calculate the y-position of the rotated pixel
        display.drawPixel(x + pivot_x + new_x, y + pivot_y + new_y, color);
      }
      mask >>= 1; //ready for next bit
    }
  }
}

void drawHorizontalBarGauge(uint16_t x0, uint16_t y0, uint16_t width, uint16_t height, uint16_t color_frame, uint16_t color_fill, float percentage) {
  uint16_t width_filled = map(percentage, 0, 100, 0, width - 2);
  display.fillRect(x0, y0, width, height, BLACK); // Reset the display area's background
  display.fillRect(x0 + 1, y0 + 1, width_filled, height - 2, color_fill);
  display.drawRect(x0, y0, width, height, color_frame);
  display.display(); //sends the buffer to the OLED
}

void drawVerticalBarGauge(uint16_t x0, uint16_t y0, uint16_t width, uint16_t height, uint16_t color_frame, uint16_t color_fill, float percentage) {
  uint16_t height_filled = map(percentage, 0, 100, 2, height);
  display.fillRect(x0, y0, width, height, BLACK); // Reset the display area's background
  display.fillRect(x0 + 1, y0 + 1 + height - height_filled, width - 2, height_filled, color_fill);
  display.drawRect(x0, y0, width, height, color_frame);
  display.display(); //sends the buffer to the OLED
}

void drawHorizontalTriangleGauge(uint16_t x0, uint16_t y0, uint16_t width, uint16_t height, uint16_t color_frame, uint16_t color_fill, float percentage) {
  uint16_t width_filled = map(percentage, 0, 100, 0, width - 2),
           x1 = x0,
           y1 = y0 + height - 1,
           x2 = x0 + width - 1,
           y2 = y1;

  display.fillRect(x0, y0, width, height, BLACK); // Reset the display area's background
  display.fillRect(x0 + 1, y0, width_filled, height - 1, color_fill);
  display.fillTriangle(x0, y0, x2, y0, x0, y1 - 1, BLACK);
  display.drawTriangle(x0, y1, x2, y0, x2, y2, color_frame);
  display.display(); //sends the buffer to the OLED
}

void drawVerticalTriangleGauge(uint16_t x0, uint16_t y0, uint16_t width, uint16_t height, uint16_t color_frame, uint16_t color_fill, float percentage) {
  uint16_t height_empty = map(percentage, 0, 100, height - 2, 0),
           x1 = x0,
           y1 = y0 + height - 1,
           x2 = x0 + width - 1,
           y2 = y1;

  display.fillRect(x0, y0, width, height, BLACK); // Reset the display area's background
  display.fillTriangle(x0, y0, x2, y0, x0, y2, color_frame);
  display.fillRect(x0 + 1, y0 + 1, width - 2, height_empty, BLACK);
  display.drawTriangle(x0, y0, x2, y0, x0, y2, color_frame);
  display.drawLine(x0, y2, x2, y0, color_frame);
  display.display(); //sends the buffer to the OLED
}

void drawCircularGauge(uint16_t x, uint16_t y, uint16_t width, uint16_t height, int line_width, int start_angle, float percentage, uint16_t color) {
  int radius_x = width / 2,
      radius_y = height / 2,
      x_center = x + radius_x,
      y_center = y + radius_y;
  float DEG2RAD = 0.0174532925;
  byte seg_degrees = 6;
  int seg_count = map(percentage, 0, 100, 0, 360) / seg_degrees;

  // Calculate first pair of coordinates for segment start
  float start_x = cos((start_angle - 90) * DEG2RAD);
  float start_y = sin((start_angle - 90) * DEG2RAD);
  uint16_t x0 = start_x * (radius_x - line_width) + x_center;
  uint16_t y0 = start_y * (radius_y - line_width) + y_center;
  uint16_t x1 = start_x * radius_x + x_center;
  uint16_t y1 = start_y * radius_y + y_center;

  display.fillRect(x, y, width, height, BLACK); // Reset the display area's background

  for (int current_angle = start_angle; current_angle < start_angle + seg_degrees * seg_count; current_angle += seg_degrees) {
    float start_x2 = cos((current_angle + 1 - 90) * DEG2RAD);
    float start_y2 = sin((current_angle + 1 - 90) * DEG2RAD);
    int x2 = start_x2 * (radius_x - line_width) + x_center;
    int y2 = start_y2 * (radius_y - line_width) + y_center;
    int x3 = start_x2 * radius_x + x_center;
    int y3 = start_y2 * radius_y + y_center;

    display.fillTriangle(x0, y0, x1, y1, x2, y2, color);
    display.fillTriangle(x1, y1, x2, y2, x3, y3, color);

    // Copy segment end to segement start for next segment
    x0 = x2;
    y0 = y2;
    x1 = x3;
    y1 = y3;
  }
  display.display(); //sends the buffer to the OLED
}

void drawVerticalRandomBarChart(uint16_t x0, uint16_t y0, uint16_t width, uint16_t height, uint16_t bar_width, uint16_t bar_spacing) {
  display.fillRect(x0, y0, width, height, BLACK); // Reset the display area's background
  display.drawLine(x0, y0, x0, y0 + height - 1, WHITE);
  display.drawLine(x0, y0 + height - 1, x0 + width - 1, y0 + height - 1, WHITE);

  for (int x1 = x0 + bar_spacing; x1 < width - bar_spacing; x1 += (bar_width + bar_spacing)) {
    int bar_height = random(0, height);
    display.fillRect(x1, x0 + height - bar_height, bar_width, bar_height, WHITE);
  }

  display.display(); //sends the buffer to the OLED
}

void drawHorizontalRandomBarChart(uint16_t x0, uint16_t y0, uint16_t width, uint16_t height, uint16_t bar_height, uint16_t bar_spacing) {
  display.fillRect(x0, y0, width, height, BLACK); // Reset the display area's background
  display.drawLine(x0, y0, x0, y0 + height - 1, WHITE);
  display.drawLine(x0, y0 + height - 1, x0 + width - 1, y0 + height - 1, WHITE);

  for (int y1 = (y0 + bar_spacing); y1 < (y0 + height); y1 += (bar_height + bar_spacing)) {
    int bar_width = random(0, width);
    display.fillRect(x0, y1, bar_width, bar_height, WHITE);
  }

  display.display(); //sends the buffer to the OLED
}

void displayTargetGrid() {
  int x_center = display_width / 2, y_center = display_height / 2, step_size = 8, center_height = 0.4 * display_height, dot_radius = 2;
  display.fillScreen(BLACK);
  drawBorder();
  for (int x = 0; x <= display_width; x += step_size) {
    display.drawLine(x, 0, x_center, y_center, WHITE);
    display.drawLine(x, display_height, x_center, y_center, WHITE);
  }
  for (int y = 0; y <= display_height; y += step_size / 2) {
    display.drawLine(0, y, x_center, y_center, WHITE);
    display.drawLine(display_width, y, x_center, y_center, WHITE);
  }
  display.fillRect(0, y_center - (center_height / 2) - 1, display_width, center_height, WHITE);
  display.fillRect(0, y_center - (center_height / 2), display_width, center_height - 2, BLACK);
  display.fillCircle(x_center, y_center, dot_radius, WHITE);
  display.display(); //sends the buffer to the OLED
}