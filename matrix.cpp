#include<stdio.h>
#include <conio.h>

void nhap(int a[10][10], int m, int n)
{
    int i, j;
    for (i = 0; i < m; i++)
    {
        for (j = 0; j < n; j++)
        {
            printf("a[%d][%d]= ", i + 1, j + 1);
            scanf("%d", &a[i][j]);
            printf("\n");
        }
    }

}
void inmatran(int a[10][10], int m, int n)
{
    int i, j;
    for (i = 0; i < m; i++)
    {
        for (j = 0; j < n; j++)
        {
            printf("%d\t", a[i][j]);
        }
        printf("\n");
    }
}

/*Cau B*/
int findMax(int a[10][10], int m, int n)
{
    int i, j;
    int maxValue = a[0][0];
    for (i = 0; i < m; i++)
    {
        for (j = 0; j < n; j++)
        {
            if (a[i][j] >= maxValue)
            {
                maxValue = a[i][j];
            }
        }
    }
    return maxValue;
}

/* Cau D*/
int findCol(int a[10][10], int row, int col)
{
    int col_cantim, col_hientai;
    int demSoduong;
    int demSoduongMax = 0;
    int i, j;
    for (i = 0; i < col; i++)
    {
        col_hientai = i;
        demSoduong = 0;
        for (j = 0; j < row; j++)
        {
            if (a[j][col_hientai] > 0)
            {
                demSoduong++;
            }
        }
        if (demSoduong > demSoduongMax)
        {
            demSoduongMax = demSoduong;
            col_cantim = col_hientai;
        }
    }
    return col_cantim;
}

/* Cau E*/
void checkMatrixTamGiac(int a[10][10], int row, int col)
{
    int i, j;
    if(row != col)
    {
        printf("Khong phai la matrix vuong nen khong phai matrix tam giac\n");
    }
    else
    {
        int laTamGiacTren = -1;
        for (i = 0; i < row; i++)
        {
            for (j = 0; j < col; j++)
            {
                /*check matrix tam giac tren*/
                if (j < i && a[i][j] != 0 )
                {
                    printf("Khong phai la matrix tam giac \n");
                    return;
                }
            }
        }
        printf("La matrix tam giac tren \n");
    }
}
int main()
{
    int a[10][10], n, m;
    printf("nhap so dong cua ma tran:\n"); scanf("%d", &m);
    printf("nhap so hang cua ma tran:\n"); scanf("%d", &n);
    nhap(a, m, n);
    printf("ma tran a la:\n");
    inmatran(a, m, n);
    printf("Column finding %d:\n", findCol(a, m, n));

    checkMatrixTamGiac(a, m, n);
    _getch();
}
