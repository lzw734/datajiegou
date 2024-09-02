#include<iostream>
#include<vector>
using namespace std;

void merge(vector<int> &arr,int left,int mid , int right)
{
    int n1 = mid - left + 1; // 左子数组长度
    int n2 = right - mid; // 右子数组长度

    vector<int> L(n1);
    vector<int> R(n2);

    for(int i = 0; i < n1; i++)
    {
        L[i] = arr[left + i];
    }
    for(int i = 0; i < n2; i++)
    {
        R[i] = arr[mid + 1 + i]; 
    }

    // i为左子数组的游标，j为右子数组的游标，k为原始数组的游标
    int i = 0, j = 0,k = left;

    // 子数组相比，取小值赋值到原数组
    while(i < n1 && j < n2)
    {
        if(L[i] <= R[j])
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // 把剩下的元素赋值回元素组
    while(i < n1)
    {
        arr[k++] = L[i++];
    }

    while(j < n2)
    {
        arr[k++] = R[j++];
    }

}

// 将数组分为两部分，也就是分治中的"分"
void mergeSort(vector<int> &arr,int left,int right)
{
    if(left < right)
    {
        int mid = left + (right - left) / 2;
        mergeSort(arr,left,mid);
        mergeSort(arr,mid + 1,right);

        merge(arr,left,mid,right);
    }
}


int main()
{
    vector<int> arr = {38, 27, 43, 3, 9, 82, 10};
    mergeSort(arr,0,6);

    vector<int>::iterator it;
    for (it = arr.begin(); it != arr.end();it++)
    {
        /* code */
        std::cout << *it << " ";
    }
    
}