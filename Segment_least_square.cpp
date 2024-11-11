#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
typedef struct {
    double x;
    double y;
} Point;

typedef struct {
    int* points;
    int Number;
} SegmentPoints;

double Calaculate_Error(Point* points, int i, int j) { //calculate error(ij) from i to j
    double sum_x = 0, sum_y = 0, sum_xy = 0, sum_x2 = 0;
    int num_points = j - i + 1;
    for (int k = i; k <= j; k++) {
        sum_x += points[k].x;
        sum_y += points[k].y;
        sum_xy += points[k].x * points[k].y; //sum of xi*yi
        sum_x2 += points[k].x * points[k].x; //sum of x^2
    }
    //(y = ax + b)
    double a = (num_points * sum_xy - sum_x * sum_y) / 
               (num_points * sum_x2 - sum_x * sum_x);
    double b = (sum_y - a * sum_x) / num_points;
    //calculate Error
    double error = 0;
    for (int k = i; k <= j; k++) {
        double predicted = a * points[k].x + b;
        error += (predicted - points[k].y) * (predicted - points[k].y);
    }
    return error;
    }

double SegmentedLeastSquares(Point* points, int n, int k, SegmentPoints* segment_points) {
    if (k <= 0 || k > n) {
        printf("Invalid number of k\n");
        return -1;
    }
    //create n*n array
    double** E = (double**)malloc(n * sizeof(double*));
    for (int i = 0; i < n; ++i) {
        E[i] = (double*)malloc(n * sizeof(double));
    }
    //create k+1*n+1 array
    double** M = (double**)malloc((k + 1) * sizeof(double*));
    for (int i = 0; i < k + 1; ++i) {
        M[i] = (double*)malloc((n + 1) * sizeof(double));
    }
    // computeAllErrors(points, n, e);
    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {
            E[i][j] = Calaculate_Error(points, i, j);
        }
    }
    //Inilialize M
    for (int i = 0; i <= k; i++) {
        for (int j = 0; j <= n; j++) {
            M[i][j] = DBL_MAX;
        }
    }
    M[0][0] = 0;  
    // Fill in the table
    for (int segments = 1; segments <= k; segments++) {
        for (int j = 1; j <= n; j++) {
            for (int i = 1; i <= j; i++) {
                double Errorij = E[i-1][j-1];
                double Cij = Errorij;
                if (i > 1) {
                    Cij += M[segments-1][i-1];
                }
                if (Cij < M[segments][j]) {
                    M[segments][j] = Cij;
                }
            }
        }
    }
    double result = M[k][n];
    //calculate min error results depends on k value; each time will use the min value saved before
    if (segment_points != NULL) {
        segment_points->points = (int*)malloc(k * sizeof(int));
        segment_points->Number = 0;
        
        int K = k;
        int F = n;
        int idx = k - 1;  
        while (K > 0 && F > 0) {
            double min_error = DBL_MAX;
            int i_location = 0;
            
            for (int i = F; i >= 1; i--) {
                double Errorij = E[i-1][F-1];
                if (i > 1) {
                    Errorij += M[K-1][i-1];
                }
                
                if (Errorij < min_error) {
                    min_error = Errorij;
                    i_location = i;
                }
            }
            segment_points->points[idx--] = i_location;
            segment_points->Number++;
            F = i_location - 1;
            K--;
        }
    }
    for (int i = 0; i < n; ++i) {
        free(E[i]);
    }
    free(E);
    for (int i = 0; i < k + 1; ++i) {
        free(M[i]);
    }
    free(M);

    return result;
}

int main(){
    FILE* file = fopen("hw6test.txt", "r");
    Point* points = (Point*)malloc(110 * sizeof(Point));
    int n = 0;
    while (fscanf(file, "%lf %lf", &points[n].x, &points[n].y) == 2) {
        n++;
    }
    fclose(file);
    SegmentPoints segment_points;
    for (int k = 1; k <= 4; k++) {
        double result = SegmentedLeastSquares(points, n, k, &segment_points);
        printf("_____________________________\n");
        printf("k=%d Results:%lf \n", k, result);
        for (int i = 0; i < segment_points.Number; i++) {
            printf("partition[%d]:%d ", i,segment_points.points[i]);
        }
        printf("\n");
    }
    printf("_____________________________\n");
    free(points);
    free(segment_points.points);
    return 0;

}