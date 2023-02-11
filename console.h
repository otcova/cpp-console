#include <iostream>
#include <string>
#include <vector>

struct V2 {
    int x, y;
    V2();
    V2(int n);
    V2(int x, int y);
};

void operator+=(V2& a, const V2& b);
void operator-=(V2& a, const V2& b);
V2 operator+(const V2& a, const V2& b);
V2 operator-(const V2& a, const V2& b);
bool operator==(const V2& a, const V2& b);
bool operator!=(const V2& a, const V2& b);
std::ostream& operator<<(std::ostream& os, const V2& v);

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
    V2 size;

    Canvas(V2 size);
    Pixel& pixel_at(V2 pos);
    void set_pixel(V2 pos, Pixel pixel);
    void swap(Canvas& canvas);
};

class Console {
    Canvas canvas;
    Canvas past_canvas;

public:
    Color color;
    Color background_color;

    Console(V2 size);
    Console(int width, int height);
    void render();

    char get_key();

    void pause_ms(int milliseconds);

    void write(int x, int y, char c);
    void write(V2 pos, char c);
    void write(int x, int y, int n);
    void write(V2 pos, int n);
    void write(int x, int y, V2 v);
    void write(V2 pos, V2 v);
    void write(int x, int y, const std::string& text);
    void write(V2 pos, const std::string& text);

    void fill_rectangle(int x, int y, int width, int height, char c);
    void fill_rectangle(int x, int y, V2 size, char c);
    void fill_rectangle(V2 pos, int width, int height, char c);
    void fill_rectangle(V2 pos, V2 size, char c);

    V2 size();
};
