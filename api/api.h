#pragma once
#include <windows.h>
#include <iostream>
#include <cmath>


struct Color {
    BYTE r, g, b;
    Color(BYTE r = 0, BYTE g = 0, BYTE b = 0) : r(r), g(g), b(b) {};
};


struct Vector {
    float x, y, z;
    Vector(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {};

    Vector operator+(Vector other) {
        return Vector(this->x + other.x, this->y + other.y, this->z + other.z);
    }
    Vector operator-(Vector other) {
        return Vector(this->x - other.x, this->y - other.y, this->z - other.z);
    }
    Vector operator*(float multiplicador) {
        return Vector(this->x * multiplicador, this->y * multiplicador, this->z * multiplicador);
    }
    Vector operator/(float divisor) {
        return Vector(this->x / divisor, this->y / divisor, this->z / divisor);
    }
    float Dist(Vector other) {
        return pow(pow(this->x - other.x, 2) + pow(this->y - other.y, 2) + pow(this->z = other.z, 2), 0.5);
    }
};

struct UiVector {
    int x, y;
    UiVector(int x = 0, int y = 0) : x(x), y(y) {};

    Vector ToVector() {
        return Vector(x, y);
    }

    UiVector operator+(UiVector other) {
        return UiVector(this->x + other.x, this->y + other.y);
    }
    UiVector operator-(UiVector other) {
        return UiVector(this->x - other.x, this->y - other.y);
    }
    UiVector operator/(int divisor) {
        return UiVector(this->x / divisor, this->y / divisor);
    }
    UiVector operator*(int multiplicador) {
        return UiVector(this->x * multiplicador, this->y * multiplicador);
    }
    float Dist(UiVector other) {
        return pow(pow(this->x - other.x, 2) + pow(this->y - other.y, 2), 0.5);
    }
};



void CreateConsole();
void Start(std::string Title, HINSTANCE hInstance, int nCmdShow);
void SetMainFunction(void (*func)(float));
void ClearScreen(Color color);
void RenderText(std::string text, UiVector pos, Color color);
void RenderRect(UiVector pos, UiVector size, Color color, bool centralized = false);
void RenderCircle(UiVector pos, float raio, Color color);
void RenderCircleOutline(UiVector pos, float raio, Color color);
void UpdateMousePos();
UiVector* GetWindowSize();
UiVector* GetMousePos();
UiVector* GetWindowPos();
