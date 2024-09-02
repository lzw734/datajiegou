#include <iostream>
#include <vector>

// 希尔排序函数
void shellSort(std::vector<int>& arr) {
    int n = arr.size();

    // 初始间隔为数组长度的一半
    for (int gap = n / 2; gap > 0; gap /= 2) {
        // 对每个间隔gap进行插入排序
        for (int i = gap; i < n; i++) {
            int temp = arr[i];
            int j;
            for (j = i; j >= gap && arr[j - gap] > temp; j -= gap) {
                arr[j] = arr[j - gap];
            }
            arr[j] = temp;
        }
    }
}

int main() {
    std::vector<int> arr = {12, 34, 54, 2, 3};

    std::cout << "原始数组: ";
    for (int num : arr) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    shellSort(arr);

    std::cout << "排序后数组: ";
    for (int num : arr) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    return 0;
}
