#include "tree.h"
#include<vector>
#include<algorithm>
int main()
{
    BPTree *tree = new BPTree(5);
    std::vector<int> nums1 = { 1,4,3,2,7,8,6,10,9,13,12,16,15,18,19,11,20,17,5,14 };
	for (int i = 0; i < nums1.size(); i++) {
		tree->insert(nums1[i]);
	}
    tree->printData();
    std::vector<int> nums2 = { 9,10,8,7,6,4,3,2,1,5,13,14,12,15,11,16,17,18,19 };
	for (int i = 0; i < nums2.size(); i++) {
		tree->mydelete(nums2[i]);
	}
	tree->printData();
}

