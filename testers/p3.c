#include <stdio.h>

int add_2(int x) {
    int y;
    
    y = x + 2;

    return y;
}

int main(){
    int x = 0;
    int y;
    char s[] = "Hello, world!\n";
    
    for(x; x < 10;x++) {
        y = add_2(x);
    }

    printf("Hello, world");


    return 0;
}


