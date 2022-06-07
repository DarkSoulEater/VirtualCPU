#include <iostream>
#include <string>
#include "SFML/Graphics.hpp"

#include "CPU.hpp"
#include "Program.hpp"

#include "Core.hpp"

static unsigned int WinWidth  = 1920;
static unsigned int WinHeight = 1080;

sf::Font RegFont;
const char* kRegFontPath = "CPU/src/graphics/fonts/FiraCode-SemiBold.ttf";

Core::Core(int width, int height) : kWinWidth_(width), kWinHeight_(height), window_(sf::VideoMode(kWinWidth_, kWinHeight_), "") {
    WinHeight = kWinWidth_;
    WinHeight = kWinHeight_;
    SetDumpView();

    if (!RegFont.loadFromFile(kRegFontPath)) {
        perror("Failed to load font for registers");
    }

}

void Core::SetCPU(CPU *cpu, VideoMode mode) {
    CPU_ = cpu;
    vmode_ = mode;
}

bool Core::Update() {
    if (CPUSleep()) {
        vmode_ = VideoMode::Default;
    }

    if (!WindowOpen()) {
        return false;
    }


    if (vmode_ == VideoMode::Default) {
        sf::Event event;
        while (window_.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window_.close();

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                window_.close();
            }
        }
    } else if (vmode_ == VideoMode::Dump) {
        UpdateDumpDate();

        sf::Event event;
        while (window_.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window_.close();
        }

        window_.clear(sf::Color::White);
        
        dump_.Draw(window_);

        window_.display();
    } else if (vmode_ == VideoMode::VideoMemmory) {
        sf::Event event;
        while (window_.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window_.close();

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                window_.close();
            }
        }

        window_.clear(sf::Color::Black);
        
        DrawVideoMemory();

        window_.display();
    }

    return true;
}

bool Core::WindowOpen() {
    return window_.isOpen();
}

bool Core::CPUSleep() {
    if (CPU_) {
        return CPU_->is_sleep_;
    }
    return true;
}

void Core::CloseWindow() {
    window_.close();
}

void Core::Dump::Reg::SetRectView(sf::Vector2f pos, sf::Vector2f size, sf::Color fill_color, sf::Color out_line_coloe) {
    rect.setPosition(pos);
    rect.setSize(size);
    rect.setFillColor(fill_color);
    rect.setOutlineColor(out_line_coloe);

    rect.setOutlineThickness(2);

    text.setPosition(pos + sf::Vector2f(2, 2));
    text.setFillColor(sf::Color::Black);
    text.setFont(RegFont);
    text.setCharacterSize(15);
    text.setString("REG");
}

void Core::Dump::Reg::UpdateDate(const char* name, Register val) {
    if (*name == 'Y') text.setString(std::string(name) + std::string(": ") + std::to_string(*(double *)&val));
    else text.setString(name + std::string(": ") + std::to_string(val));
}

void Core::Dump::Reg::Draw(sf::RenderWindow &window) {
    window.draw(rect);
    window.draw(text);
}

sf::Vector2f operator*(const sf::Vector2f &v, int k) {
    return {v.x * k, v.y * k};
}

#define SRV(R, i) R.SetRectView(pos + shift * (i), size, fill_color, out_line_color)
void Core::Dump::SetView() {
    sf::Vector2f size = sf::Vector2f(250.f, 25.f);
    sf::Vector2f shift = sf::Vector2f(0.f, 35.f);
    sf::Color fill_color = sf::Color::Green;
    sf::Color out_line_color = sf::Color::Red;
    
    sf::Vector2f pos = sf::Vector2f(20.f, 20.f);
    
    int i = 0;
    SRV(CS, i++);
    SRV(DS, i++); 
    SRV(SS, i++);
    SRV(MS, i++);
    
    pos = pos + shift * i + sf::Vector2f(0.f, 10.f);
    i = 0;
    SRV(IP, i++);
    SRV(SP, i++);
    SRV(BP, i++);

    pos = pos + shift * i + sf::Vector2f(0.f, 10.f);
    i = 0;
    SRV(SI, i++);
    SRV(DI, i++);
    
    pos = sf::Vector2f(20.f, float(WinHeight - shift.y * 12 - 10));
    i = 0;
    SRV(AX, i++);
    SRV(BX, i++);
    SRV(CX, i++);
    SRV(DX, i++);
    
    pos = pos + shift * i + sf::Vector2f(0.f, 10.f);
    i = 0;
    SRV(R8, i++);
    SRV(R9, i++);
    SRV(R10, i++);
    SRV(R11, i++);
    SRV(R12, i++);
    SRV(R13, i++);
    SRV(R14, i++);
    SRV(R15, i++);

    pos = sf::Vector2f(float(WinWidth - size.x * 2 - 30), float(WinHeight - shift.y * 8 - 10));
    i = 0;
    SRV(YMM0, i++);
    SRV(YMM1, i++);
    SRV(YMM2, i++);
    SRV(YMM3, i++);
    SRV(YMM4, i++);
    SRV(YMM5, i++);
    SRV(YMM6, i++); 
    SRV(YMM7, i++);

    pos = sf::Vector2f(float(WinWidth - size.x - 20), float(WinHeight - shift.y * 8 - 10));
    i = 0;
    SRV(YMM8, i++); 
    SRV(YMM9, i++); 
    SRV(YMM10, i++); 
    SRV(YMM11, i++); 
    SRV(YMM12, i++); 
    SRV(YMM13, i++); 
    SRV(YMM14, i++); 
    SRV(YMM15, i++);
}
#undef SRV

#define RG(R) R.UpdateDate(#R, cpu->R);
void Core::Dump::UpdateDate(CPU *cpu) {
    #include "GRegs.hpp"
}

#define RG(Name) Name.Draw(window);
void Core::Dump::Draw(sf::RenderWindow &window) {
    AX.Draw(window);
    #include "GRegs.hpp"
}

void Core::SetDumpView() {
    dump_.SetView();
}

void Core::UpdateDumpDate() {
    dump_.UpdateDate(CPU_);
}

void Core::DrawVideoMemory() {
    sf::Uint8 *pixels = (sf::Uint8*)((char*)CPU_->MS + CPU_->VS);

    sf::Texture texture;
    texture.create(kWinWidth_, kWinHeight_);
    sf::Sprite sprite(texture);

    texture.update(pixels);
    
    window_.draw(sprite);
}
