#include <stdio.h>
int main(){

    int a[10][10], b[10][10], mul[10][10], m, n, i, j, k;
    printf("Enter order of matrix A: ");
    scanf("%d%d", &m, &n);
    for (i = 1; i <= m; i++){
        for (j = 1; j <= n; j++){
            printf("Enter value of a[%d][%d]: ", i, j);
            scanf("%d", &a[i][j]);
            b[j][i] = a[i][j];
        }
    }
    printf("\nAnswer:\n");
    for (i = 1; i <= m; i++){
        for (j = 1; j <= m; j++){
            mul[i][j] = 0;
            for (k = 1; k <= n; k++)
                mul[i][j] = mul[i][j] + a[i][k] * b[k][j];
            printf("%4d", mul[i][j]);
        }
        printf("\n");
    }
    return 0;
}