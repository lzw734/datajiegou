#include<iostream>
using namespace std;

void print(int arr[],int n)
{
    for(int  i = 0; i < n; i++)
    {
        printf("%d ",arr[i]);
    }
}

void quick_sort(int arr[],int low,int height)
{
    if(low >= height)
    {
        return;
    }
    int p = arr[low];  // 基准
    int lt = low,rt = height;

    while(lt < rt)
    {
        // 右游标找比基准值小的数
        while(arr[rt] >= p && rt > lt)rt--;
        // 左游标找比基准大的数
        while(arr[lt] <= p && rt > lt)lt++;

        int t = arr[lt];
        arr[lt] = arr[rt];
        arr[rt] = t;
    }

    arr[low] = arr[lt];
    arr[lt] = p;

    quick_sort(arr,lt + 1,height);
    quick_sort(arr,low,lt - 1);
}

int main()
{
    int aa[7] = {3,4,6,1,2,4,7};
    quick_sort(aa,0,6);
    print(aa,7);
}
