// DO NOT EDIT REST OF THE FILE
// ONLY WRITE CODE BELOW THE "WRITE BELOW" COMMENTS

#include<cmath>
#include<eigen3/Eigen/Core>
#include<eigen3/Eigen/Dense>
#include<iostream>

int main() {
    // You are required to create the matrices for 
    // the mentioned questions and print them to 
    // std::cout

    //---------------------------------------------------------------------
    // Question 2
    std::cout << "------------------------\n";
    std::cout << "Question 2\n------------------------" << std::endl;

    // matrix definition for Question 2
    Eigen::Matrix3f rot_45;

    // Write the 3×3 transformation matrix for a 45 degrees
    // clockwise rotation in 2D (assuming homogeneous coordinates) 
    // Use the << operator to create the rot_45 matrix (WRITE BELOW)
    rot_45 << 0.7071, -0.7071, 0, 0.7071, 0.7071, 0, 0, 0, 1.0;

    std::cout << rot_45 << std::endl;
    std::cout << "------------------------\n";
    
    //---------------------------------------------------------------------
    // Question 3
    std::cout << "Question 3\n------------------------" << std::endl;

    // matrix definition for Question 3
    Eigen::Matrix4f translation;

    // Write the 4×4 transform matrix to move a point by (3,4,5)
    // Use the << operator to create the translation matrix (WRITE BELOW)
    translation << 1.0, 0, 0, 3.0, 0, 1.0, 0, 4.0, 0, 0, 1.0, 5.0, 0, 0, 0, 1.0;


    std::cout << translation << std::endl;
    std::cout << "------------------------\n";

    //---------------------------------------------------------------------
    // Question 4
    std::cout << "Question 4\n------------------------" << std::endl;
    // Define and create (using << operator) the matrices here. Make sure
    // that they have the correct dimensions and ordering.
    // Print the matrices to std out with each matrix in a new line
    // WRITE BELOW

    Eigen::Matrix3f base, translate_to_origin, scale, translate_back;
    base << 1.0, 0, 0, 0, 1.0, 0, 0, 0, 1.0;
    translate_to_origin << 1.0, 0, -2.0, 0, 1.0, -3.0, 0, 0, 1.0;
    scale << 1.0, 0, 0, 0, 1.0, 0, 0, 0, 1.0;
    translate_back << 1.0, 0, 3.0, 0, 1.0, 2.0, 0, 0, 1.0;

    std::cout << "Start with base translation \n";
    std::cout << base << std::endl;
    std::cout << "Translate to the origin \n";
    std::cout << translate_to_origin * base << std::endl;
    std::cout << "Scale to new size \n";
    std::cout << scale * translate_to_origin * base << std::endl;
    std::cout << "Translate Back \n";
    std::cout << translate_back * scale * translate_to_origin * base << std::endl;

    return 0;
}