#include "console.h"
#include <windows.h>
#include <conio.h>
#include <sstream>
using namespace std;

// V2 ----------------------

void operator+=(V2& a, const V2& b) {
    a.x += b.x;
    a.y += b.y;
}

void operator-=(V2& a, const V2& b) {
    a.x -= b.x;
    a.y -= b.y;
}

V2 operator+(const V2& a, const V2& b) {
    return V2(a.x + b.x, a.y + b.y);
}

V2 operator-(const V2& a, const V2& b) {
    return V2(a.x - b.x, a.y - b.y);
}

bool operator==(const V2& a, const V2& b) {
    return a.x == b.x and a.y == b.y;
}

bool operator!=(const V2& a, const V2& b) {
    return a.x != b.x or a.y != b.y;
}

std::ostream& operator<<(std::ostream& os, const V2& v) {
    os << '[' << v.x << ", " << v.y << ']';
    return os;
}

V2::V2() : x(0), y(0) {}
V2::V2(int n) : x(n), y(n) {}
V2::V2(int x, int y) : x(x), y(y) {}


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

Canvas::Canvas(V2 size) 
	: size(size), pixels(size.x * size.y, Pixel(' ')) {}

Pixel& Canvas::pixel_at(V2 pos) {
    return pixels[pos.x + pos.y * size.x];
}

void Canvas::set_pixel(V2 pos, Pixel pixel) {
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

void set_cursor_position(V2 pos) {
    static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    std::cout.flush();
    COORD coord = { (SHORT)pos.x, (SHORT)pos.y };
    SetConsoleCursorPosition(hOut, coord);
}

Console::Console(V2 size)
    : canvas(size), past_canvas(size), color(204), background_color(37)
{
    show_console_cursor(false);
    system("cls");
    SetConsoleOutputCP(65001); 
}

Console::Console(int width, int height) : Console(V2(width, height)) {}

void Console::render() {
    V2 pos;
    string buffer = "";
    
    for (int y = 0; y < canvas.size.y; ++y) {
        for (int x = 0; x < canvas.size.x; ++x) {
            Pixel& pixel = canvas.pixel_at(V2(x, y));
            Pixel& past_pixel = past_canvas.pixel_at(V2(x, y));
            
            if (pixel.data != past_pixel.data) {
                if (buffer.length() == 0) pos = V2(x, y);
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
    write(V2(x, y), x);
}

void Console::write(V2 pos, char c) {
	canvas.set_pixel(pos, Pixel(c, color, background_color));
}

void Console::write(int x, int y, int n) {
    write(V2(x, y), n);
}

void Console::write(V2 pos, int n) {
	write(pos, to_string(n));
}

void Console::write(int x, int y, V2 v) {
    write(V2(x, y), v);
}

void Console::write(V2 pos, V2 v) {
    ostringstream o;
    o << v;
	write(pos, o.str());
}

void Console::write(int x, int y, const string& text) {
    write(V2(x, y), text);
}

vector<string> unicode_into_chars(const string& s) {
	vector<string> chars;
	for (int i = 0; i < s.length(); ++i) {
		if ((s[i] & 128) and not (s[i] & 64)) chars.back() += s[i];
		else chars.emplace_back(1, s[i]);
	}
	return chars;
}

void Console::write(V2 pos, const string& text) {
    vector<string> chars = unicode_into_chars(text);
	for (int i = 0; i < chars.size(); ++i) {
	    canvas.set_pixel(pos + V2(i, 0), Pixel(chars[i], color, background_color));
	}
}


void Console::fill_rectangle(int x, int y, int width, int height, char c) {
    fill_rectangle(V2(x, y), V2(width, height), c);
}
void Console::fill_rectangle(int x, int y, V2 size, char c) {
    fill_rectangle(V2(x, y), size, c);
}
void Console::fill_rectangle(V2 pos, int width, int height, char c) {
    fill_rectangle(pos, V2(width, height), c);
}

void Console::fill_rectangle(V2 pos, V2 size, char c) {
    for (int dx = 0; dx < size.x; ++dx) {
        for (int dy = 0; dy < size.y; ++dy) {
            write(pos + V2(dx, dy), c);
        }
    }
}


void Console::pause_ms(int milliseconds) {
	Sleep(milliseconds);
}

V2 Console::size() {
    return canvas.size;
}