#include "stdio.h"

extern float target();

int main() {
    float return_value = target();
    printf("%.3f\n", return_value);
}
