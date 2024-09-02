#include<iostream>
using namespace std;

void print(int arr[],int n)
{
    for(int  i = 0; i < n; i++)
    {
        printf("%d ",arr[i]);
    }
}


// 直接插入排序
void zhijiecharu(int arr[],int n)
{
    // 12,11,13,5,6
    for(int i = 1; i < n; i++)
    {
        
        int key = arr[i];
        int  j = i - 1;
        while(j >= 0 && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            j--;
        }

        arr[j + 1] = key;
    }
}

int main()
{
    int a[7] = {3,1,4,1,5,9,2};
    zhijiecharu(a,7);
    print(a,7);
}
