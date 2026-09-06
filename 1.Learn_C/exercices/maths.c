#include <stdio.h>

double perimeter(double length, double width){
    return 2 * (length + width);
}

double area(double length, double width){
    return length * width;
}

int main(){
    double length = 5.0;
    double width = 3.0;

    printf("Perimeter: %.2f\n", perimeter(length, width));
    printf("Area: %.2f\n", area(length, width));

    return 0;

}