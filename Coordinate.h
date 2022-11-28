
#ifndef coords
#define coords
#include <cmath>
#include <vector>
class Coordinate {
    public:
        Coordinate(unsigned int x_value, unsigned int y_value) : x(x_value), y(y_value) {}

        unsigned int getX() const { return x; } //getters for the x and y coords of the a coordinate object
        unsigned int getY() const { return y; }

    private:
        unsigned int x, y;

};

bool operator<(const std::vector<Coordinate>& one, const std::vector<Coordinate>& two) {
    return (one.size() < two.size());
} // operator< to sort the vector of zones

bool adjacent(const Coordinate& temp, const Coordinate& temp2) { //function to check adjacent coords and stars 
    int dx = temp.getX() - temp2.getX();
    int dy = temp.getY() - temp2.getY();
    return (std::abs(dx)+std::abs(dy) == 1 || (std::abs(dx) == 1 && std::abs(dy) == 1)); //gets the diagonals and left, right, up and down coordinates
}
#endif