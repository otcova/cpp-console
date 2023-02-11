#include <iostream>
#include <string>
#include <vector>

struct F2;

struct I2 {
    int x, y;
    I2();
    I2(int n);
    I2(F2 v);
    I2(int x, int y);
};

void operator+=(I2& a, const I2& b);
void operator-=(I2& a, const I2& b);
I2 operator+(const I2& a, const I2& b);
I2 operator-(const I2& a, const I2& b);
bool operator==(const I2& a, const I2& b);
bool operator!=(const I2& a, const I2& b);
std::ostream& operator<<(std::ostream& os, const I2& v);


struct F2 {
    float x, y;
    F2();
    F2(float n);
    F2(I2 v);
    F2(float x, float y);
};

void operator+=(F2& a, const F2& b);
void operator-=(F2& a, const F2& b);
F2 operator+(const F2& a, const F2& b);
F2 operator-(const F2& a, const F2& b);
bool operator==(const F2& a, const F2& b);
bool operator!=(const F2& a, const F2& b);
std::ostream& operator<<(std::ostream& os, const F2& v);

struct Color {
    int r, g, b;
    Color();
    Color(int gray);
    Color(int r, int g, int b);
};

struct Pixel {
    std::string data;

    Pixel(char c);
    Pixel(char c, Color rgb, Color background_rgb);
    Pixel(const std::string& c, Color rgb, Color background_rgb);
};

class Canvas {
    std::vector<Pixel> pixels;

public:
    I2 size;

    Canvas(I2 size);
    Pixel& pixel_at(I2 pos);
    void set_pixel(I2 pos, Pixel pixel);
    void swap(Canvas& canvas);
};

class Console {
    Canvas canvas;
    Canvas past_canvas;

public:
    Color color;
    Color background_color;

    Console(I2 size);
    Console(int width, int height);
    void render();

    char get_key();

    void pause_ms(int milliseconds);

    void write(int x, int y, char c);
    void write(I2 pos, char c);
    void write(int x, int y, int n);
    void write(I2 pos, int n);
    void write(int x, int y, I2 v);
    void write(I2 pos, I2 v);
    void write(int x, int y, const std::string& text);
    void write(I2 pos, const std::string& text);

    void fill_rectangle(int x, int y, int width, int height, char c);
    void fill_rectangle(int x, int y, I2 size, char c);
    void fill_rectangle(I2 pos, int width, int height, char c);
    void fill_rectangle(I2 pos, I2 size, char c);

    I2 size();
};
