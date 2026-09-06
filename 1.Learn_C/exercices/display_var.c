#include <stdio.h>

int main(){
    int age = 30;
    char name = 'Y';
    double height = 1.83;

    printf("Hi, I'm %c, I'm %d years old and my height is %.2f meters.", name, age, height);

    printf("sizeof(age) = %lu\n", sizeof(age));
    printf("sizeof(name) = %lu\n", sizeof(name));
    printf("sizeof(height) = %lu\n", sizeof(height));
}