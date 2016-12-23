#include <iostream>

constexpr unsigned long colour_count = 3u;
constexpr unsigned long bits_per_byte = 8u;
constexpr unsigned long bits_per_LED = colour_count*bits_per_byte;

constexpr unsigned long LED_count = 7;
constexpr unsigned long com_size = LED_count*bits_per_LED;
float m_colour_commands[com_size] = {0};

float bit_to_ontime(std::uint8_t bit){
    return bit ? 0.8
               : 0.4
               ;
}

void set_pixel( std::size_t  pos,
                std::uint8_t red,
                std::uint8_t green,
                std::uint8_t blue
){
    auto set_colour = [](std::size_t p, std::uint8_t colour){
        for(unsigned i = 0; i < 8; ++i){
            m_colour_commands[p+i] = bit_to_ontime((colour >> (8-1-i)) & 0x1);
        }
        };

    if(pos >= LED_count){
        std::cerr << "pos out of range (" << pos << ").\n";
        return;
    }

    set_colour(pos*bits_per_LED,    green);
    set_colour(pos*bits_per_LED+8,  red);
    set_colour(pos*bits_per_LED+16, blue);
}

int main(){

    int sw = 1;
    for(unsigned long i = 0; i < LED_count; ++i, ++sw){
        switch(sw){
            case 1:
                set_pixel(i, 0, 255, 0xAA);
                break;
            case 2:
                set_pixel(i, 255, 0xC5, 0);
                break;
            case 3:
                set_pixel(i, 0xAA, 0, 255);
            default:
                sw = 0;
                break;
        }
    }

    for(unsigned long led = 0; led < LED_count; ++led){
        std::cout << "Pixel #" << led << ":\n";
        for(unsigned long colour = 0; colour < colour_count; ++colour){
            std::cout << '\t';
            switch(colour){
                case 0:
                    std::cout << "Green: ";
                    break;
                case 1:
                    std::cout << "Red: ";
                    break;
                case 2:
                    std::cout << "Blue: ";
                    break;
                default:
                    break;
            }
            for(unsigned long bit = 0; bit < bits_per_byte; ++bit){
                std::cout << m_colour_commands[ led*bits_per_LED
                                              + colour*bits_per_byte
                                              + bit
                                              ]
                          << ' '
                          ;
            }
            std::cout << '\n';
        }
        std::cout << '\n';
    }

    return 0;
}