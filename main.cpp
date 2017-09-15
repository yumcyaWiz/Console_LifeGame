#include <iostream>
#include <string>
#include <random>
#include <chrono>
#include <thread>
#include <cstdlib>


std::random_device rand_dev;
std::mt19937 mt(rand_dev());
std::uniform_int_distribution<> irand(0, 1);
inline int rand() {
    return irand(mt);
}


std::chrono::milliseconds duration(100);
inline void sleep() {
    std::this_thread::sleep_for(duration);
}


inline std::string green() {
    return "\e[48;5;2m \e[0m";
}
inline std::string  black() {
    return " ";
}


class Cell {
    public:
        int* cell_p;
        int* cell;
        int width;
        int height;
        bool torus;

        Cell(int w, int h, bool _torus) {
            width = w;
            height = h;
            cell = new int[width*height];
            cell_p = new int[width*height];
            torus = _torus;
        };
        ~Cell() {
            delete[] cell;
            delete[] cell_p;
        };
        
         void set(int i, int j, int val) {
             cell[i + j*width] = val;
         };
         int get(int i, int j) const {
             if(torus) {
                 if(i < 0)
                     i = width - 1;
                 else if(i == width)
                     i = 0;
                 if(j < 0) 
                     j = height - 1;
                 else if(j == height - 1)
                     j = 0;
             }
             else {
                 if(i < 0 || i >= width || j < 0 || j >= height)
                     return 0;
             }
             return cell[i + j*width];
         };
         int getP(int i, int j) const {
             if(torus) {
                 if(i < 0)
                     i = width - 1;
                 else if(i == width)
                     i = 0;
                 if(j < 0) 
                     j = height - 1;
                 else if(j == height - 1)
                     j = 0;
             }
             else {
                 if(i < 0 || i >= width || j < 0 || j >= height)
                     return 0;
             }
             return cell_p[i + j*width];

         }
         std::string draw() const {
             std::string str;
             for(int j = 0; j < height; j++) {
                 for(int i = 0; i < width; i++) {
                     if(this->get(i, j) == 0)
                         str += black();
                     else
                         str += green();
                 }
                 str += "\n";
             }
             str += "\r\e[" + std::to_string(width) + "A";
             return str;
         }
         void update() {
            for(int i = 0; i < width; i++) {
                for(int j = 0; j < height; j++) {
                    cell_p[i + j*width] = cell[i + j*width];
                }
            }

            for(int i = 0; i < width; i++) {
                for(int j = 0; j < height; j++) {
                    int upLeft = this->getP(i-1, j-1);
                    int up = this->getP(i, j-1);
                    int upRight = this->getP(i+1, j-1);
                    int left = this->getP(i-1, j);
                    int right = this->getP(i+1, j);
                    int downLeft = this->getP(i-1, j+1);
                    int down = this->getP(i, j+1);
                    int downRight = this->getP(i+1, j+1);
                    int val = upLeft + up + upRight + left + right + downLeft + down + downRight;
                    if(val == 3)
                        this->set(i, j, 1);
                    else if(val == 2) 
                        this->set(i, j, this->getP(i, j));
                    else
                        this->set(i, j, 0);
                }
            }
         }
};


int main(int argc, char** argv) {
    if(argc != 3) {
        std::cout << "invalid parameters" << std::endl;
        exit(1);
    }
    
    
    int width = std::stoi(argv[1]);
    int height = std::stoi(argv[2]);
    Cell cell(width, height, false);
    for(int i = 0; i < cell.width; i++) {
        for(int j = 0; j < cell.height; j++) {
            cell.set(i, j, rand());
        }
    }


    while(true) {
        cell.update();
        std::cout << cell.draw() << std::flush;
        sleep();
    }
}
