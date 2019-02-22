#include <iostream>
#include <stdio.h>
#include <vector>
#include <cstdlib>
#include <string.h>
#include <algorithm>
#include <ctime>
#include <iomanip>
using namespace std;

struct Flower
{
    double attribute[4];
    int type;
};

struct Node
{
    Node()
    {
        parent = NULL, left = NULL, right = NULL, type=0;
    }
    double value;
    int attribute;
    Node *parent;
    Node *right;
    Node *left;
    vector<Flower> v;
    int type;
};

bool DataSort0(Flower a,Flower b)
{
    return a.attribute[0]<b.attribute[0];
}
bool DataSort1(Flower a,Flower b)
{
    return a.attribute[1]<b.attribute[1];
}
bool DataSort2(Flower a,Flower b)
{
    return a.attribute[2]<b.attribute[2];
}
bool DataSort3(Flower a,Flower b)
{
    return a.attribute[3]<b.attribute[3];
}

void CoutData(vector<Flower> v)
{
    for(vector<Flower>::iterator it = v.begin(); it!=v.end(); it++)
    {
        for(int i=0; i<4; i++)
        {
            cout<<(*it).attribute[i]<<" ";
        }
        cout<<(*it).type<<endl;
    }
}

double ComputeGini(vector<Flower> v,double threshold,int attributeIdx)
{
    int classNum[6] ={0,0,0,0,0,0};

    for(vector<Flower>::iterator it = v.begin();it!=v.end();it++){
        if((*it).attribute[attributeIdx] < threshold){
            classNum[(*it).type -1]++;
        }
        else{
            classNum[(*it).type +2]++;
        }
    }
    double leftnum = classNum[0] + classNum[1] + classNum[2];
    double rightnum = classNum[3] + classNum[4] + classNum[5];
    double giniLeft =1;
    double giniRight = 1;

    for(int i=0;i<3;i++){
        giniLeft-= classNum[i]/leftnum * classNum[i]/leftnum;
        giniRight-= classNum[i+3]/rightnum * classNum[i+3]/rightnum;
    }

    return giniLeft + giniRight;
}

void BuildTree(Node *parent)
{
    struct Gini{
        Gini(){
            gini = 1e9;
            value = 0;
            attribute =0;
        }
        double gini;
        double value;
        int attribute;
    };
    Gini g;
    //choose feature
    for(int i=0; i<4; i++)
    {
        switch(i)
        {
        case 0:
            sort(parent->v.begin(),parent->v.end(),DataSort0);
            break;
        case 1:
            sort(parent->v.begin(),parent->v.end(),DataSort1);
            break;
        case 2:
            sort(parent->v.begin(),parent->v.end(),DataSort2);
            break;
        case 3:
            sort(parent->v.begin(),parent->v.end(),DataSort3);
            break;
        }
        double threshold = 0;

        for(int j=0; j<parent->v.size()-1; j++)
        {
            double a1 = parent->v[j].attribute[i];
            double a2 = parent->v[j+1].attribute[i];

            if(threshold != (a1+a2)/2 && a1!=a2)
            {
                threshold = (a1+a2)/2;
            }
            else
            {
                continue;
            }
            double tempGini = ComputeGini(parent->v,threshold,i);
            if(tempGini < g.gini){
                g.gini = tempGini;
                g.value = threshold;
                g.attribute = i;
            }
        }
    }
    // divide left tree and right tree
    Node *leftChild = new Node;
    Node *rightChild = new Node;

    int leftType = 0;
    int rightType = 0;
    for(vector<Flower>::iterator it = parent->v.begin();it!=parent->v.end();it++){
        if((*it).attribute[g.attribute] < g.value){
            leftChild->v.push_back((*it));

            if(leftType == -1){
                continue;
            }
            else if(leftType == 0){
                leftType = (*it).type;
            }
            else if(leftType != (*it).type){
                leftType = -1;
            }
        }
        else{
            rightChild->v.push_back((*it));

            if(rightType == -1){
                continue;
            }
            else if(rightType == 0){
                rightType = (*it).type;
            }
            else if(rightType != (*it).type){
                rightType = -1;
            }
        }
    }
    leftChild->type = leftType;
    rightChild->type = rightType;

    // connect
    parent->left = leftChild;
    parent->right = rightChild;
    leftChild ->parent = parent;
    rightChild->parent = parent;

    parent->attribute = g.attribute;
    parent->value = g.value;

    if(leftType== -1){
        BuildTree(leftChild);
    }
    if(rightType == -1){
        BuildTree(rightChild);
    }
}

int FindType(Node *p,Flower test){
    if(p->type > 0){
        return p->type;
    }
    else if(test.attribute[p->attribute] < p->value){
        return FindType(p->left,test);
    }
    else if(test.attribute[p->attribute] >= p->value){
        return FindType(p->right,test);
    }
    else{
        return -100;
    }
}


void postOrderTraversal(Node *p)
{
    if (!p)
        return;
    postOrderTraversal(p->left);
    postOrderTraversal(p->right);
    delete p;
}


int main()
{
    srand(time(0));
    FILE *fp;
    fp = fopen("iris.txt","r");

    Flower temp;
    int datanum = 0;
    char tempStr[20];

    vector<Flower> data;
    while(fscanf(fp,"%lf%*c%lf%*c%lf%*c%lf%*c%s",&temp.attribute[0],&temp.attribute[1],&temp.attribute[2],&temp.attribute[3],tempStr)!=EOF)
    {
        datanum ++;
        if(strcmp(tempStr,"Iris-setosa")==0)
            temp.type = 1;
        else if(strcmp(tempStr,"Iris-versicolor")==0)
            temp.type = 2;
        else
            temp.type = 3;
        data.push_back(temp);
    }
    fclose(fp);



    int totalCorrect = 0;
    int totalWrong = 0;
    for(int j =0; j<10; j++){
        int correct = 0;
        int wrong = 0;
        random_shuffle(data.begin(),data.end());
        for(int k=0;k<5;k++){
            vector<Flower>trainData;
            vector<Flower>testData;
            for(int i=0;i<data.size();i++){
                if( i < (data.size()/5) * (k+1) && i >= (data.size()/5) * k){
                    testData.push_back(data[i]);
                }
                else{
                    trainData.push_back(data[i]);
                }
            }
            int treeNum = 10;
            Node *root[treeNum];
            for(int i=0;i<treeNum;i++){
                root[i] = new Node;
                random_shuffle(trainData.begin(),trainData.end());
                root[i]->v.assign(trainData.begin(),trainData.begin()+ 100);
                BuildTree(root[i]);
            }

            for(vector<Flower>::iterator it = trainData.begin();it!=trainData.end();it++){
                int ans = (*it).type;
                int guess[3] = {0,0,0};
                for(int i=0;i<treeNum;i++){
                    guess[FindType(root[i],(*it)) -1] ++;
                }

                int finalGuess = max(guess[0],max(guess[1],guess[2]));
                for(int i=0;i<3;i++){
                    if(finalGuess == guess[i]){
                        finalGuess = i+1;
                        break;
                    }
                }
                (ans == finalGuess)?correct++:wrong++;
            }
            for(int i=0;i<treeNum;i++){
                postOrderTraversal(root[i]);
            }
        }
        totalCorrect += correct;
        totalWrong += wrong;
        cout<<"Test "<<j+1<<": ";
        cout<<"Correctness "<<((correct)/(double)(correct+wrong))*100<<"%\n";
    }
    cout<<endl;
    cout<<"Average : "<<((totalCorrect)/(double)(totalCorrect + totalWrong))*100<<"%";




    return 0;
}
