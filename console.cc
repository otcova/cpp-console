#include "console.h"
#include <windows.h>
#include <conio.h>
#include <sstream>
using namespace std;


// I2 ----------------------

void operator+=(I2& a, const I2& b) {
    a.x += b.x;
    a.y += b.y;
}

void operator-=(I2& a, const I2& b) {
    a.x -= b.x;
    a.y -= b.y;
}

I2 operator+(const I2& a, const I2& b) {
    return I2(a.x + b.x, a.y + b.y);
}

I2 operator-(const I2& a, const I2& b) {
    return I2(a.x - b.x, a.y - b.y);
}

bool operator==(const I2& a, const I2& b) {
    return a.x == b.x and a.y == b.y;
}

bool operator!=(const I2& a, const I2& b) {
    return a.x != b.x or a.y != b.y;
}

std::ostream& operator<<(std::ostream& os, const I2& v) {
    os << '[' << v.x << ", " << v.y << ']';
    return os;
}

I2::I2() : x(0), y(0) {}
I2::I2(int n) : x(n), y(n) {}
I2::I2(F2 v) : x(v.x), y(v.y) {}
I2::I2(int x, int y) : x(x), y(y) {}

// F2 ----------------------

void operator+=(F2& a, const F2& b) {
    a.x += b.x;
    a.y += b.y;
}

void operator-=(F2& a, const F2& b) {
    a.x -= b.x;
    a.y -= b.y;
}

F2 operator+(const F2& a, const F2& b) {
    return F2(a.x + b.x, a.y + b.y);
}

F2 operator-(const F2& a, const F2& b) {
    return F2(a.x - b.x, a.y - b.y);
}

bool operator==(const F2& a, const F2& b) {
    return a.x == b.x and a.y == b.y;
}

bool operator!=(const F2& a, const F2& b) {
    return a.x != b.x or a.y != b.y;
}

std::ostream& operator<<(std::ostream& os, const F2& v) {
    os << '[' << v.x << ", " << v.y << ']';
    return os;
}

F2::F2() : x(0), y(0) {}
F2::F2(float n) : x(n), y(n) {}
F2::F2(I2 v) : x(v.x), y(v.y) {}
F2::F2(float x, float y) : x(x), y(y) {}

// Color ----------------------

Color::Color() : r(0), g(0), b(0) {}
Color::Color(int gray) : r(gray), g(gray), b(gray) {}
Color::Color(int r, int g, int b) : r(r), g(g), b(b) {}

// Pixel -------------------

Pixel::Pixel(char c) {
    data = "\x1b[0m";
    data += c;
}

Pixel::Pixel(char c, Color rgb, Color background_rgb) {
    data = "\x1b[0;48;2;";
    data += to_string(background_rgb.r);
    data += ';';
    data += to_string(background_rgb.g);
    data += ';';
    data += to_string(background_rgb.b);
    // If space => ignore color
    if (c != ' ') {
        data += ";38;2;";
        data += to_string(rgb.r);
        data += ';';
        data += to_string(rgb.g);
        data += ';';
        data += to_string(rgb.b);
    }
    data += 'm';
    data += c;
}

Pixel::Pixel(const string& c, Color rgb, Color background_rgb) {
    data = "\x1b[0;48;2;";
    data += to_string(background_rgb.r);
    data += ';';
    data += to_string(background_rgb.g);
    data += ';';
    data += to_string(background_rgb.b);
    // If space => ignore color
    if (c != " ") {
        data += ";38;2;";
        data += to_string(rgb.r);
        data += ';';
        data += to_string(rgb.g);
        data += ';';
        data += to_string(rgb.b);
    }
    data += 'm';
    data += c;
}

// Canvas -------------------

Canvas::Canvas(I2 size) 
	: size(size), pixels(size.x * size.y, Pixel(' ')) {}

Pixel& Canvas::pixel_at(I2 pos) {
    return pixels[pos.x + pos.y * size.x];
}

void Canvas::set_pixel(I2 pos, Pixel pixel) {
    if (0 <= pos.x and pos.x < size.x and 0 <= pos.y and pos.y < size.y)
        pixel_at(pos) = pixel;
}

// Console -------------------

void show_console_cursor(bool show_cursor) {
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO cursorInfo;

    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = show_cursor;
    SetConsoleCursorInfo(out, &cursorInfo);
}

void set_cursor_position(I2 pos) {
    static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    std::cout.flush();
    COORD coord = { (SHORT)pos.x, (SHORT)pos.y };
    SetConsoleCursorPosition(hOut, coord);
}

Console::Console(I2 size)
    : canvas(size), past_canvas(size), color(204), background_color(37)
{
    show_console_cursor(false);
    system("cls");
    SetConsoleOutputCP(65001); 
}

Console::Console(int width, int height) : Console(I2(width, height)) {}

void Console::render() {
    I2 pos;
    string buffer = "";
    
    for (int y = 0; y < canvas.size.y; ++y) {
        for (int x = 0; x < canvas.size.x; ++x) {
            Pixel& pixel = canvas.pixel_at(I2(x, y));
            Pixel& past_pixel = past_canvas.pixel_at(I2(x, y));
            
            if (pixel.data != past_pixel.data) {
                if (buffer.length() == 0) pos = I2(x, y);
                buffer += pixel.data;
                past_pixel = pixel;
            } else if (buffer.length() > 0) {
                set_cursor_position(pos);
                cout << buffer;
                buffer = "";
            }
        }
        if (buffer.length() > 0) {
            set_cursor_position(pos);
            cout << buffer;
            buffer = "";
        }
    }
    
    if (buffer.length() > 0) {
        set_cursor_position(pos);
        cout << buffer;
    }
}

char Console::get_key() {
	if (_kbhit()) return getch();
	return 0; 
}


void Console::write(int x, int y, char c) {
    write(I2(x, y), c);
}

void Console::write(I2 pos, char c) {
	canvas.set_pixel(pos, Pixel(c, color, background_color));
}

void Console::write(int x, int y, int n) {
    write(I2(x, y), n);
}

void Console::write(I2 pos, int n) {
	write(pos, to_string(n));
}

void Console::write(int x, int y, I2 v) {
    write(I2(x, y), v);
}

void Console::write(I2 pos, I2 v) {
    ostringstream o;
    o << v;
	write(pos, o.str());
}

void Console::write(int x, int y, const string& text) {
    write(I2(x, y), text);
}

vector<string> unicode_into_chars(const string& s) {
	vector<string> chars;
	for (int i = 0; i < s.length(); ++i) {
		if ((s[i] & 128) and not (s[i] & 64)) chars.back() += s[i];
		else chars.emplace_back(1, s[i]);
	}
	return chars;
}

void Console::write(I2 pos, const string& text) {
    vector<string> chars = unicode_into_chars(text);
	for (int i = 0; i < chars.size(); ++i) {
	    canvas.set_pixel(pos + I2(i, 0), Pixel(chars[i], color, background_color));
	}
}


void Console::fill_rectangle(int x, int y, int width, int height, char c) {
    fill_rectangle(I2(x, y), I2(width, height), c);
}
void Console::fill_rectangle(int x, int y, I2 size, char c) {
    fill_rectangle(I2(x, y), size, c);
}
void Console::fill_rectangle(I2 pos, int width, int height, char c) {
    fill_rectangle(pos, I2(width, height), c);
}

void Console::fill_rectangle(I2 pos, I2 size, char c) {
    for (int dx = 0; dx < size.x; ++dx) {
        for (int dy = 0; dy < size.y; ++dy) {
            write(pos + I2(dx, dy), c);
        }
    }
}


void Console::pause_ms(int milliseconds) {
	Sleep(milliseconds);
}

I2 Console::size() {
    return canvas.size;
}