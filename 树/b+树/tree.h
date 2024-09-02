#include "node.h"
#include<queue>
class BPTree
{
public:
    BPTree(int order = 3);
    bool insert(const int data);
    bool mydelete(const int data);
    void printData();
private:
    BPTnode *root;
    int order;
    int count;
    BPTnode *findLoc(const int data); // 返回数据位置或者应该插入位置
    BPTnode *getHead();

};

BPTree::BPTree(int order)
{
    this->order = order;
    root = new BPTnode();
    root->father = nullptr;
    root->setOrder(order);
    count = 0;
}

bool BPTree::insert(const int data)
{
    BPTnode *loc = findLoc(data);
    // 在该位置插入
    bool flag = loc->insert(data,this->root);
    if(flag)
        this->count++;
    else
    {
        std::cout << "插入失败，数已存在" << std::endl;
        return false;
    }
    return true;
}

BPTnode *BPTree::findLoc(const int data)
{
    BPTnode *temp = root;
    if(this->count == 0)
        return root;
    if(root->key[0].left == nullptr)
        return root;
    while(temp->key[0].left)
    {
        for(int i = 0; i < temp->key.size();i++)
        {
            if(temp->key[i].data > data)
            {
                temp = temp->key[i].left;
                break;
            }
            if(i == temp->key.size() - 1)
            {
                temp = temp->key[i].right;
                break;
            }
        }
    }
    return temp;
}

bool BPTree::mydelete(const int data)
{
    BPTnode *loc = findLoc(data);
    bool flag = loc->remove(data,root);
    if(flag)
        this->count--;
    else
    {
        std::cout << "删除失败，无此数据" << std::endl;
        return false;
    }
    return true;
}

BPTnode *BPTree::getHead()
{
    if(!root)
    {
        std::cout << "the tree is empty";
        return nullptr;
    }  
    BPTnode *tmp = root;
    while(tmp->key.size() > 0 && tmp->key[0].left)
    {
        tmp = tmp->key[0].left;
    }
    return tmp;
}

void BPTree::printData()
{
    BPTnode *first = getHead();
    while(first)
    {
        int len = first->key.size();
        for(int i = 0; i < len;i++)
        {
            std::cout << first->key[i].data << " ";
            if(i == len - 1)
            {
                first = first->next;
            }
        }
    }
    std::cout << std::endl;
}