#include <stdio.h>
#include <math.h>

int add(int x, int y){
    return x + y;
}

int subtract(int x, int y){
    return x - y;
}

int multiply(int x, int y){
    return x * y;
}

double divide(int x, int y) {
    if (y == 0) {
        return NAN;
    }
    return (double)x / y;
}

int main(){
    int a = 10;
    int b = 5;
    int add_result = add(a, b);
    int subtract_result = subtract(a, b);
    int multiply_result = multiply(a, b);
    double divide_result = divide(a, b);

    printf("The sum of %d and %d is %d\n", a, b, add_result);
    printf("The difference of %d and %d is %d\n", a, b, subtract_result);
    printf("The product of %d and %d is %d\n", a, b, multiply_result);
    printf("The quotient of %d and %d is %.2f\n", a, b, divide_result);

    return 0;
}