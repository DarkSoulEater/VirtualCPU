#pragma once

#include "SFML/Graphics.hpp"
#include "CPU.hpp"

enum class VideoMode {
    Default,
    Dump,
    VideoMemmory
};


class Core {
public:
    Core(int width, int height);

    void SetCPU(CPU *cpu, VideoMode mode);

    bool Update();
private:
    const unsigned int kWinWidth_  = 1920;
    const unsigned int kWinHeight_ = 1080;
    sf::RenderWindow window_;

    VideoMode vmode_ = VideoMode::Default;

    CPU* CPU_;

    struct Dump{
        struct Reg{
            sf::RectangleShape rect;
            sf::Text text;

            void SetRectView(sf::Vector2f pos, sf::Vector2f size, 
                         sf::Color fill_color, sf::Color out_line_coloe);
            void UpdateDate(const char*name, Register val);
            void Draw(sf::RenderWindow &window);
        } CS, DS, SS, MS, IP, SP, BP, SI, DI, AX, BX, CX, DX,
          R8, R9, R10, R11, R12, R13, R14, R15,
          YMM0, YMM1, YMM2, YMM3, YMM4, YMM5, YMM6, YMM7,
          YMM8, YMM9, YMM10, YMM11, YMM12, YMM13, YMM14, YMM15;
        
        void SetView();
        void UpdateDate(CPU *cpu);
        void Draw(sf::RenderWindow &window);
    } dump_;
    

    void MainLoop();

    bool WindowOpen();

    bool CPUSleep();

    void CloseWindow();

    void SetDumpView();

    void UpdateDumpDate();

    void DrawVideoMemory();
};
