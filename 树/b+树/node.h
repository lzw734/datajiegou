#pragma once
#include<iostream>
#include<vector>
#include<algorithm>

class BPTnode
{
public:
    class keyNode
    {
    public:
        keyNode(){};
        keyNode(const int data):data(data){};
        keyNode(const int data,BPTnode *left,BPTnode *right):data(data),left(left),right(right){};
        int data;
        BPTnode *left = nullptr,*right = nullptr;
    };
    BPTnode();
    BPTnode(const int data,int order = 3);  // 支持单一数据创造节点
    BPTnode(std::vector<int> datas,int order = 3);  // 支持多个数据创造节点
    void setOrder(int order){this->order = order;};
    bool insert(const int data,BPTnode *&node);
    bool find(const int key);
    bool remove(const int data,BPTnode *&root);
    
    std::vector<keyNode> key = {};
    BPTnode *father = nullptr;
    BPTnode *next = nullptr;
private:
    int order = 3; // 阶数
    BPTnode *getLeft(BPTnode *&node,BPTnode *&root); // 找到当前节点的左兄弟节点
    BPTnode *getHead(BPTnode *&root);
    bool remove(BPTnode *node,BPTnode *&root);
    bool insert(keyNode *&node,BPTnode *&root);
};

BPTnode::BPTnode(){}

BPTnode::BPTnode(const int data,int order)
{
    this->order = order;
    keyNode k(data);
    this->key.push_back(k);
}

BPTnode::BPTnode(std::vector<int> datas,int order)
{
    this->order = order;
    std::sort(datas.begin(),datas.end());
    for(auto k:datas)
    {
        keyNode temp(k);
        this->key.push_back(temp);
    }
    
}

bool BPTnode::find(const int data)
{
    for(auto &k : key)
    {
        if(data == k.data)
        {
            return true;
        }
    }
    return false;
}

bool BPTnode::insert(const int k,BPTnode *&root)
{
    if(find(k))
        return false;
    if(this->key.size() == 0)
        this->key.push_back(keyNode(k));
    else if(k > (*this->key.rbegin()).data) // k比节点所有数据大，直接插到最后
        this->key.push_back(keyNode(k));
    else
    {
        int i = 0;
        for(i; i < this->key.size();i++)
        {
            if(k < this->key[i].data)
                break;
        }
        key.emplace(key.begin() + i,keyNode(k));
    }
    if(key.size() < order)
        return true;
    else
    {
        // 分裂节点
        int m = order / 2;
        keyNode *newkey = new keyNode(key[m]);
        BPTnode *newbpt = new BPTnode();
        newbpt->father = father;
        newbpt->order = order;
        newbpt->next = this->next;
        this->next = newbpt;
        newkey->left = this;
        newkey->right = newbpt;
        for(int i = m; i < key.size();i++)
        {
            keyNode *tmp = new keyNode;
            tmp->data = key[i].data;
            tmp->left = key[i].left;
            tmp->right = key[i].right;
            newbpt->key.push_back(*tmp);
        }
        key.resize(m);
        if(father)
        {
            father->insert(newkey,root);
            return true;
        }
        else
        {
            BPTnode *tmp = new BPTnode();
            tmp->order = order;
            this->father = tmp;
            newbpt->father = tmp;
            tmp->key.push_back(*newkey);
            root = tmp;
            return true;
        }
    }
    return true;
}

bool BPTnode::insert(keyNode *&node,BPTnode *&root)
{
    if(node->data > (*key.rbegin()).data)
    {
        key.push_back(*node);
    }
    else
    {
        int i = 0;
        for(i; i < key.size(); i++)
        {
            if(key[i].data > node->data)
            {
                break;
            }
        }
        if(i != 0)key[i - 1].right = node->left;
        key[i].left = node->right;
        key.emplace(key.begin() + i,*node);
    }
    if(key.size() < order){return true;}
    else
    {
        int m = (order - 1) / 2;    
        keyNode *newkey = new keyNode(key[m]);
        BPTnode *newbpt = new BPTnode();
        newbpt->father = father;
        newbpt->order = order;
        newkey->left = this;
        newkey->right = newbpt;
        key[m].right->father = newbpt;
        for(int i = m + 1; i < key.size(); i++)
        {
            if(key[i].right)key[i].right->father = newbpt; 
            keyNode *tmp = new keyNode(key[i]);
            tmp->left = key[i].left;
            tmp->right = key[i].right;
            newbpt->key.push_back(*tmp);
        }
        key.resize(m);
        if(father)
            father->insert(newkey,root);
        else
        {
            // 父节点不存在，创造一个父节点
            BPTnode *father = new BPTnode();
            this->father = father;
            newbpt->father = father;
            father->key.push_back(*newkey);
            father->order = order;
            root = father;
            return true;
        }
    }
    return false;
}

bool BPTnode::remove(const int data,BPTnode *&root)
{
    if(!find(data))
        return false;
    for(int i = 0; i < key.size();i++)
    {
        if(key[i].data == data)
        {
             key.erase(key.begin() + i);
             break;
        }
    }
    if(father == nullptr)return true;
    int m = (order + 1) / 2 -1;
    if(key.size() >= m)
    {
        for(auto &k : father->key)
        {
            if(k.right == this)
            {
                k.data = key[0].data;
                break;
            }
        }
        return true;
    }
    else
    {
        // 左右兄弟可以借
        int leftloc = 0,rightloc = 0;
        BPTnode *leftbro = nullptr,*rightbro = nullptr;
        for(int i = 0; i < father->key.size();i++)
        {
            if(this == father->key[i].left)
            {
                rightloc = i;
                rightbro = father->key[i].right;
            }
            if(this == father->key[i].right)
            {
                leftloc = i;
                leftbro = father->key[i].left;
            }
        }
        // 左兄弟够借
        if(leftbro && leftbro->key.size() > m)
        {
            keyNode data = leftbro->key[leftbro->key.size() - 1];
            key.emplace(key.begin(),data);
            leftbro->key.erase(leftbro->key.end() - 1);
            father->key[leftloc].data = key[0].data;
            return true;
        }
        else if(rightbro && rightbro->key.size() > m)
        {
            keyNode data = rightbro->key[0];
            key.emplace_back(data);
            rightbro->key.erase(rightbro->key.begin());
            father->key[rightloc].data = rightbro->key[0].data;
            return true;
        }
        else
        {
             // 左右兄弟都不够借，和左兄弟合并
            if(leftbro)
            {
                for(int i = 0; i < leftbro->key.size();i++)
                {   
                    key.emplace(key.begin() + i,leftbro->key[i]);
                }
                BPTnode *cousin = getLeft(leftbro,root);
                if(cousin)cousin->next = this;
                if(leftloc > 0)
                    father->key[leftloc - 1].right = this;
                father->key.erase(father->key.begin() + leftloc);
                delete leftbro;
                // 父节点中删除了数据，同时也需要判断下父节点有没有出现下溢出
                return this->remove(father,root);
            }
            else if(rightbro)
            {
                for(int i = 0; i < rightbro->key.size();i++)
                {
                    key.push_back(rightbro->key[i]);
                }
                this->next = rightbro->next;
                if(rightloc + 1 < father->key.size())
                {
                    father->key[rightloc + 1].left = this;
                }
                father->key.erase(father->key.begin() + rightloc);
                delete rightbro;
                return this->remove(father,root);
             }   
        }
       
    }
    return false;
}

bool BPTnode::remove(BPTnode *now,BPTnode *&root)
{
    int m = (order + 1) / 2 - 1;
    if(now == root)
    {
        if(now->key.size() == 0)
        {
            this->father = nullptr;
            root = this;
            delete now;
            return true;
        }
        return true;
    }
    if(now->key.size() >= m)return true;
    BPTnode *dad = now->father;
    BPTnode *leftbro = nullptr;
    BPTnode *rightbro = nullptr;
    int leftLoc = 0,rightloc = 0;
    for(int i = 0; i < dad->key.size();i++)
    {
        if(now == dad->key[i].left)
        {
            rightbro = dad->key[i].right;
            rightloc = i;
        }
        if(now == dad->key[i].right)
        {
            leftbro = dad->key[i].left;
            leftLoc = i;
        }
    }
    if(leftbro && leftbro->key.size() > m)
    {
       keyNode newkey(dad->key[leftLoc].data);
       newkey.left = leftbro->key[leftbro->key.size() - 1].right;
       // 怀疑点
       if(now->key.size() != 0)
            newkey.right = now->key[0].left;
        now->key.push_back(newkey);
        dad->key[leftLoc].data = leftbro->key[leftbro->key.size() - 1].data;
        leftbro->key.erase(leftbro->key.end() - 1);
        return true;
    }
    if(rightbro && rightbro->key.size() > m)
    {
        keyNode newkey(dad->key[rightloc].data);
        newkey.right = rightbro->key[0].left;
        if(now->key.size() > 0)
            newkey.left = now->key[now->key.size() - 1].right;
        now->key.push_back(newkey);
        dad->key[rightloc].data = rightbro->key[0].data; 
        rightbro->key.erase(rightbro->key.begin());
        return true;
    }
    if(leftbro)
    {
        BPTnode *dad = now->father;
        keyNode newkey;
        newkey.data = dad->key[leftLoc].data;
        newkey.left = leftbro->key[leftbro->key.size() - 1].right;
        if(now->key.size() > 0)
            newkey.right = now->key[0].left;
        dad->key.erase(dad->key.begin() + leftLoc);
        now->key.emplace(now->key.begin(),newkey);
        for(int i = 0; i < leftbro->key.size();i++)
        {
            now->key.emplace(now->key.begin() + i,leftbro->key[i]);
            leftbro->key[i].left->father = now;
            leftbro->key[i].right->father = now;
        }
        delete leftbro;
        return now->remove(dad,root);
    }
    if(rightbro)
    {
        BPTnode *dad = now->father;
        keyNode newkey;
        newkey.data = dad->key[rightloc].data;
        newkey.right = rightbro->key[0].left;
        if(now->key.size() > 0)
            newkey.left = now->key[now->key.size() - 1].right;
        dad->key.erase(dad->key.begin() + rightloc);
        now->key.push_back(newkey);
        for(int i =0; i < rightbro->key.size(); i++)
        {
            now->key.push_back(rightbro->key[i]);
            rightbro->key[i].left->father = now;
            rightbro->key[i].right->father = now;
        }
        delete rightbro;
        return now->remove(dad,root);
    }
    return false;
}

BPTnode *BPTnode::getLeft(BPTnode *&node,BPTnode *&root)
{
    BPTnode *head = getHead(root);
    while(head->next)
    {
        if(head->next == node)
        {
            return head;
        }
        head = head->next;
    }
    return nullptr;
}

BPTnode *BPTnode::getHead(BPTnode *&root)
{
    BPTnode *tmp = root;
    while(tmp->key.size() > 0 && tmp->key[0].left)
    {
        tmp = tmp->key[0].left;
    }
    return tmp;
}