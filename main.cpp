#include <iostream>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <string>
#include <ctype.h>
#include <list>
#include "Coordinate.h"
#include <algorithm>
#include <iterator>

typedef std::vector<std::vector<std::string> > BList;
typedef std::vector<Coordinate> Grid;
typedef std::vector<unsigned int> VectInt;


void printBoards(const std::vector<Grid>& VectBoard, std::ofstream& out_str, BList& board) {
    out_str << "Number of solution(s): " << VectBoard.size() << std::endl;
    out_str << std::endl;
    std::vector<std::string> original;
    for (unsigned int i = 0; i < VectBoard.size(); i++) { //goes through the vector of solutions 
        if (!original.empty()) {
            original.clear();
        }
        for (unsigned int j = 0; j < VectBoard[i].size(); j++) { 
            original.push_back(board[(VectBoard[i][j]).getY()][(VectBoard[i][j]).getX()]); //saves the boards characters that will change to a vector of stirngs
            board[(VectBoard[i][j]).getY()][(VectBoard[i][j]).getX()] = "@"; //changes those coordinates from each solution to stars
        }
        out_str << "Solution " << i+1 << ":" << std::endl;
        for (unsigned int a = 0; a < board.size(); a++) { //writes the board to the file
            for (unsigned int b = 0; b < board[a].size(); b++) {
                out_str << board[a][b];
            }
            out_str << std::endl;
        }

        for (unsigned int c = 0; c < VectBoard[i].size(); c++) { //changes the board back to the original 
            board[(VectBoard[i][c]).getY()][(VectBoard[i][c]).getX()] = original[c];
        }
        out_str << std::endl;
    }
}


void PrintOne(const Grid& one, std::ofstream& out_str, BList& board) { //gets one solution from the vector of solutions and changes the strings to stars based on the coordinates
    out_str << "Number of solution(s): 1" << std::endl;
    out_str << std::endl;
    out_str << "Solution 1" << ":" << std::endl;
    for (unsigned int z = 0; z < one.size(); z++) {
        board[(one[z]).getY()][(one[z]).getX()] = "@"; //changes boards elements based on the solution from the vector 
    } 
    for (unsigned int a = 0; a < board.size(); a++) {
        for (unsigned int b = 0; b < board[a].size(); b++) {
            out_str << board[a][b];
        }
        out_str << std::endl;
    }
}

void solveBoard(unsigned int stars, VectInt& rowVect, VectInt& colVect, std::vector<Grid >& VectBoard, const std::vector<Grid >& zones, 
std::vector<Grid>::const_iterator& it, Grid::const_iterator it2 , Grid& starList, unsigned int count2, bool starter) {
    

    if (count2 == stars * zones.size() && it == zones.end()) {  //base case when the stars * zones is equal to the count variable
        VectBoard.push_back(starList);
        return;
    }
    else if (it == zones.end()) {
        return;
    }
    else {  
        if (stars == 1) { //for one star
            for (it2 = it->begin(); it2 != it->end(); it2++) {
                unsigned int zonecount = 0; 
                Grid possible; //creates a vector of coordinates to store possible solutions
                if (possible.empty()) {
                    possible.push_back(*it2);
                }
                for (unsigned int j = 0; j < starList.size(); j++) { //checks for adjacent stars and if there are, it clears the vector
                    if (adjacent(starList[j],*it2)) {
                        possible.clear();
                    }
                }
                if (rowVect[it2->getY()] < stars && colVect[it2->getX()] < stars && !possible.empty()) { //checks for any conflict with the columns and rows 
                    rowVect[it2->getY()]++;
                    colVect[it2->getX()]++;
                    zonecount++;
                }
                else {
                    possible.clear();
                }
                if (!possible.empty()) { //if the vector isnt empty then it is added to the starList
                    starList.push_back(possible[0]);
                    count2 += zonecount;
                    if (it != zones.end()) { //checks to see if we aren't at the end iterator for zones
                        it++; //goes to recurse to the next zone
                        solveBoard(stars, rowVect, colVect, VectBoard, zones, it, it2, starList, count2, starter);
                        count2--; //decreases the count of stars on the board by 1
                        unsigned int size = starList.size(); 
                        rowVect[starList[size-1].getY()]--; //decreases values at both column and row vectors by 1
                        colVect[starList[size-1].getX()]--;
                        starList.pop_back(); //gets rid of the last element to find another possible solution
                        it--; //goes back to the previous spot the iterator was pointing at 
                    }
                }

                else {
                    if (it2 == it->end()-1) { //returns nothing if the zone is at the last coordinate and no solutions were found
                        return;
                    }
                }
            }
        }
        else {
            Grid possible; //has a possible solution for each zone
            unsigned int zonecount = 0; 
            for (it2 = it->begin(); it2 != it->end(); it2++) { 
                bool found = false;
                if (possible.empty()) { //starting coordinate for that solution zone
                    possible.push_back(*it2);
                    zonecount++;
                    starter = true;
                }
                else {
                    starter = false;
                }
                if (!starter) { //if it isn't a starter solution it checks adjacent points in the possible solutions
                    for (unsigned int q = 0; q < possible.size(); q++) {
                        if (adjacent(possible[q],*it2)) {
                            found = true;
                            break; 
                        }
                    }
                }
                for (unsigned int j = 0; j < starList.size(); j++) { //checks if the possible star is adjacent to other stars 
                    if (adjacent(starList[j],*it2)) {
                        found = true;
                        break;
                    }
                }
                if (rowVect[it2->getY()] == stars || colVect[it2->getX()] == stars) { //checks for any conflicts in the rows and columns vector 
                    found = true;
                }

                if (found) { //if the coordinate is bad 
                    if (starter) { //clears the possible vector and decreases zonecount 
                        possible.clear();
                        zonecount--;
                        continue;
                    }
                    else {
                        if (it2 == it->end()-1 && zonecount != stars) { //gets rid of this solution because it wasnt able to find a proper coordinate in the zone
                            zonecount--;
                            count2--;
                            rowVect[it2->getY()]--;
                            colVect[it2->getX()]--;
                            possible.pop_back();
                            return;
                        }
                        else { //keeps iterating through the coordinates in that zone 
                            continue;
                        }
                    }
                }
                else if (!found && !starter) { //if nothing is wrong with the coordinate it gets added to the possible solution set 
                    possible.push_back(*it2);
                    zonecount++;
                    if (zonecount < stars) { //stops considering a solution set if there arent enough stars and we are at the end of the zone
                        if (it2 == it->end()-1) {
                            zonecount--; //adjusts the associate data structures for the solution set
                            count2--;
                            rowVect[it2->getY()]--;
                            colVect[it2->getX()]--;
                            possible.pop_back();
                            return;
                        }
                        else { //continues to try and find a star in the zone
                            continue;
                        } 
                    }
                    else if (zonecount == stars) { //if the zone is good it recurses to the next zone
                        for (unsigned int h = 0; h < possible.size(); h++) {
                            starList.push_back(possible[h]);
                        } //passes the solution set for the zone to the vector of stars 
                        count2 += zonecount;
                        if (it != zones.end()) { 
                            it++;
                            starter = true;
                            solveBoard(stars, rowVect, colVect, VectBoard, zones, it,it2, starList, count2, starter); //starts recursing for another possible solution in the next zone
                            --it;
                            if (it2 != it->end()) { //checks to see if the coordinate iterator isnt at the end iterator of the zone
                                starter = false; //changes the associated data structures to continue to find more possible solutions 
                                zonecount--;
                                count2--;
                                rowVect[it2->getY()]--;
                                colVect[it2->getX()]--;
                                possible.pop_back();
                            }

                        }
                    }
                }
                else { 
                    if (it2 == it->end()-1) {
                        return;
                    }
                    else {
                        continue;
                    }
                }
            }
        }
    }
}


int main(int argc, char* argv[]) {

    std::ifstream in_str(argv[1]);
    std::ofstream out_str(argv[2]);

    if (argc != 6) { //error checking for arguments 
        std::cerr << "Need 5 arguments for the program to execute" << std::endl;
        exit(1);
    }


    if (!in_str.good()) {  //sees if the input file was opened correctly
        std::cerr << "Can't open " << argv[1] << " to read." << std::endl;
        exit(1);
    }

    if (!out_str.good()) { //sees if the input file was opened correctly
        std::cerr << "Can't open " << argv[2] << " to write." << std::endl;
        exit(1);
    }


    if (!(std::isdigit(*argv[3]))) {
        std::cerr << "Invalid entry of stars." << std::endl;
        exit(1);
    }
    

    unsigned int stars = atoi(argv[3]); //converting string to int


    unsigned int column, row, num, coord1, coord2; 
    std::string letter;
    in_str >> row >> column;

    VectInt rowVect(row, 0); //creating a vector to keep track of stars on the rows and columns
    VectInt colVect(column, 0); 

    BList board; //creates a fill in board based on the rows and column
    for (unsigned int i = 0; i < row; i++) {
        std::vector<std::string> vec1;
        for (unsigned int j = 0; j < column; j++) {
            vec1.push_back("-");
        }
        board.push_back(vec1);
    }

    std::vector<Grid > zones; //creating a vector of vector of coordinates 

    while (!in_str.eof()) {
        in_str >> letter >> num;

        unsigned int count = 0;

        Grid zone;

        while (in_str >> coord1 >> coord2) { //changes the boards characters based on the coordinates and the zone it is in
            zone.push_back(Coordinate(coord1,coord2));
            board[coord2][coord1] = letter;
            count++;
            if (count == num) { //breaks out of the loop when the number of zones are done
                break;
            }
        }
        zones.push_back(zone); //pushing back vector of coordinates 
    }

    sort(zones.begin(),zones.end());
    std::vector<Grid > VectBoard;
    std::vector<Grid >::const_iterator it = zones.begin();
    Grid::const_iterator it2;
    unsigned int count2 = 0; 
    Grid starList;
    bool starter = true;


    if (std::string(argv[4]) == "count") { //returns the number of solutions if it is on count 
        if (std::string(argv[5]) == "all_solutions") {
            solveBoard(stars, rowVect, colVect, VectBoard, zones, it, it2, starList, count2, starter);
            out_str << "Number of solution(s): " << VectBoard.size() << std::endl;
        }
        else if (std::string(argv[5]) == "one_solution") {
            out_str << "Number of solution(s): 1" << std::endl;
        }
        else {
            std::cerr << "Command not found" << std::endl; //error checking 
        }
    }

    else if (std::string(argv[4]) == "print") { //returns the number of solutions and the each solution board in the text file
        if (std::string(argv[5]) == "all_solutions") { 
            solveBoard(stars, rowVect, colVect, VectBoard, zones, it, it2, starList, count2, starter);
            printBoards(VectBoard, out_str, board);
        }
        else if (std::string(argv[5]) == "one_solution") { 
            solveBoard(stars, rowVect, colVect, VectBoard, zones, it, it2, starList, count2, starter);
            PrintOne(VectBoard[0],out_str, board);
        }
        else {
            std::cerr << "Command not found" << std::endl; //error checking 
        }
    }

    else {
        std::cerr << "Command not found" << std::endl; //error checking
    }

    in_str.close();
    out_str.close();

    return 0;
}