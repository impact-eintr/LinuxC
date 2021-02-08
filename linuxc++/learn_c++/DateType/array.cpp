#include <cstdlib>
#include <iostream>
#include <math.h>

const int N = 10;
const int M = 5;

static void arrayinit(int *array) {
    for (int i = 0; i < N; i++) {
        *(array+i) = rand()%100;
    }
}
static void arrayinit(int (*array)[M]) {
    for (int i = 0; i < N; i++){
        for(int j = 0;j < M;j++){
            *(*(array+i)+j) = rand()%100;
        }
    }
}


int main() {
    int arr1[N];
    int arr2[N][M];
    arrayinit(arr1);
    arrayinit(arr2);
    for (int i = 0; i < N; i++) {
        std::cout<<i<<std::endl;
        for(int j = 0;j < M;j++)
          std::cout << arr2[i][j] << " ";
        std::cout<<'\n';

    }

}
