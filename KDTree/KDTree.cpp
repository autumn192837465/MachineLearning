#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <cmath>
#include <ctime>
#include <algorithm>
using namespace std;

int k=0;
struct DataNode{
    float data[9];
    int  type;
    int index;
};
struct Node{
    Node(){
        parent = NULL,
        left = NULL,
        right = NULL,
        feature = 0;
        }

    vector<DataNode>v;
    Node *parent;
    Node *left;
    Node *right;
    int feature;
    DataNode splitNode;
};
struct DistanceNode{
    int index;
    int type;
};
vector<DistanceNode>dn;
DistanceNode temp;

int total=0;
void coutData(vector<DataNode>v){
    int j=1;
    for(vector<DataNode>::iterator it=v.begin();it!=v.end();it++){
        cout<<"Data "<<j<<" ";
        for(int i=0;i<9;i++)
            cout<<(*it).data[i]<<" ";
        cout<<(*it).type<<endl;
        j++;
    }
}
int splitId(vector<DataNode>v){
    int id;
    int max=0;
    float avg[9];
    for(int i=0;i<9;i++){
        avg[i]=0;
        for(vector<DataNode>::iterator it=v.begin();it!=v.end();it++){
            avg[i]+=(*it).data[i];
        }
        avg[i]/=v.size();
    }
    for(int i=0;i<9;i++){
        float tempsum=0;
        for(vector<DataNode>::iterator it=v.begin();it!=v.end();it++){
            tempsum+=pow((*it).data[i]-avg[i],2);
        }
        if(tempsum>max){
            max=tempsum;
            id=i;
        }
    }
    return id;
}

vector<DataNode>Data,testData;   //all data

void postOrderTraversal(Node *p) {
    if (!p) return;
        postOrderTraversal(p->left);
        postOrderTraversal(p->right);
        delete p;
}

int compFeature=0;

bool comp(const DataNode a,const DataNode b){
    return a.data[compFeature]<b.data[compFeature];
}

void buildTree(Node *n){
    total++;
    if(n->v.size()==1){
        n->splitNode=n->v[0];
        return;
    }

    compFeature=splitId(n->v);
    n->feature=compFeature;
    sort(n->v.begin(),n->v.end(),comp);
    int a=n->v.size()/2;
    n->splitNode=n->v[a];
    n->v.erase(n->v.begin()+a);
    Node *leftChild=new Node;
    Node *rightChild=new Node;

    for(vector<DataNode>::iterator it=n->v.begin();it!=n->v.end();it++){
        if((*it).data[compFeature] <= n->splitNode.data[compFeature]){
            leftChild->v.push_back(*it);
        }
        else{
            rightChild->v.push_back(*it);
        }
    }

    if(leftChild->v.size()==0){
        delete leftChild;
    }
    else{
        n->left=leftChild;
        leftChild->parent=n;
        buildTree(n->left);
    }
    if(rightChild->v.size()==0){
        delete rightChild;
    }
    else{
        n->right=rightChild;
        rightChild->parent=n;
        buildTree(n->right);
    }

    return;
}

float Distance(DataNode a,DataNode b){
    float temp=0;
    for(int i=0;i<9;i++){
        temp+=pow((a.data[i]-b.data[i]),2);
    }
    return temp;
}
float minDis=0;
int type;

void findType(Node *n,DataNode query){

    float d=Distance(n->splitNode,query);
    if(d<=minDis){
        bool record=false;
        for(vector<DistanceNode>::iterator it=dn.begin();it!=dn.end();it++){
            if((*it).index==n->splitNode.index){
                record=true;
            }
        }
        if(!record){
            minDis=d;
            type=n->splitNode.type;
            temp.type=type;
            temp.index=n->splitNode.index;
        }
    }

    if(n->splitNode.data[n->feature]<query.data[n->feature]){
        if(n->right!=NULL){
            findType(n->right,query);
        }
        if(n->left!= NULL && minDis >= pow((n->splitNode.data[n->feature]-query.data[n->feature]),2)){
            findType(n->left,query);
        }
    }
    else if(n->left!=NULL && n->splitNode.data[n->feature]>=query.data[n->feature]){
        if(n->left!=NULL){
            findType(n->left,query);
        }
        if(n->right!= NULL && minDis >= pow((n->splitNode.data[n->feature]-query.data[n->feature]),2)){
            findType(n->right,query);
        }
    }
}

int main(int argc,char *argv[])
{
    //Data Input
    FILE *fp1,*fp2,*fout;
    fp1=fopen(argv[1],"r");
    fp2=fopen(argv[2],"r");
    fscanf(fp1,"%*[^\n]\n");
    fscanf(fp2,"%*[^\n]\n");
    int index;
    char fileMessage[200];

    DataNode t;//temp data node

    while(fscanf(fp1,"%s\n",fileMessage)!=EOF){
        char *token;
        token=strtok(fileMessage,",\n");    //index
        t.index=atoi(token);
        token=strtok(NULL,",\n");    //name
        int i=0;
        while((token=strtok(NULL,",\n"))!=NULL){
            if(i<=8){
                t.data[i]=atof(token);
                i++;
            }
            else{
                if(!strcmp(token,"cp"))         t.type=1;
                else if(!strcmp(token,"im"))    t.type=2;
                else if(!strcmp(token,"pp"))    t.type=3;
                else if(!strcmp(token,"imU"))   t.type=4;
                else if(!strcmp(token,"om"))    t.type=5;
                else if(!strcmp(token,"omL"))   t.type=6;
                else if(!strcmp(token,"imL"))   t.type=7;
                else if(!strcmp(token,"imS"))   t.type=8;
            }
        }

        Data.push_back(t);
    }

    while(fscanf(fp2,"%s\n",fileMessage)!=EOF){
        char *token;
        token=strtok(fileMessage,",\n");    //index
        t.index=atoi(token);
        token=strtok(NULL,",\n");    //name
        int i=0;
        while((token=strtok(NULL,",\n"))!=NULL){
            if(i<=8){
                t.data[i]=atof(token);
                i++;
            }
            else{
                if(!strcmp(token,"cp"))         t.type=1;
                else if(!strcmp(token,"im"))    t.type=2;
                else if(!strcmp(token,"pp"))    t.type=3;
                else if(!strcmp(token,"imU"))   t.type=4;
                else if(!strcmp(token,"om"))    t.type=5;
                else if(!strcmp(token,"omL"))   t.type=6;
                else if(!strcmp(token,"imL"))   t.type=7;
                else if(!strcmp(token,"imS"))   t.type=8;
            }
        }
        testData.push_back(t);
    }
    fclose(fp1);
    fclose(fp2);


    Node *root = new Node;
    (root->v).assign(Data.begin(),Data.end());
    buildTree(root);



    vector<DistanceNode> v1,v2,v3;

    int j=0;
    int correct=0;
    int total=0;

    k=1;
    int voteType[9];

    for(vector<DataNode>::iterator it=testData.begin();it!=testData.end();it++){
        for(int i=0;i<9;i++)    voteType[i]=0;
        for(int i=0;i<k;i++){
            minDis=99999;
            type=-1;
            findType(root,(*it));
            dn.push_back(temp);
        }
        if(j<3){
            switch(j){
                case 0:
                    v1.assign(dn.begin(),dn.end());
                    break;
                case 1:
                    v2.assign(dn.begin(),dn.end());
                    break;
                case 2:
                    v3.assign(dn.begin(),dn.end());
                    break;
            }
            j++;
        }
        for(vector<DistanceNode>::iterator it=dn.begin();it!=dn.end();it++){
            voteType[(*it).type-1]++;
        }
        int guess=0;
        int maxVote=0;
        for(int i=0;i<9;i++){
            if(voteType[i]>maxVote){
                maxVote=voteType[i];
                guess=i+1;
            }
        }
        total++;
        if(guess==(*it).type){
            correct++;
        }
        dn.clear();
    }

    cout<<"KNN accuracy: "<<float(correct)/(float)total<<endl;
    for(vector<DistanceNode>::iterator it=v1.begin();it!=v1.end();it++){
        cout<<(*it).index<<" ";
    }
    cout<<endl;
    for(vector<DistanceNode>::iterator it=v2.begin();it!=v2.end();it++){
        cout<<(*it).index<<" ";
    }
    cout<<endl;
    for(vector<DistanceNode>::iterator it=v3.begin();it!=v3.end();it++){
        cout<<(*it).index<<" ";
    }
    cout<<endl<<endl;
    v1.clear();
    v2.clear();
    v3.clear();

    j=0;
    correct=0;
    total=0;

    k=5;
    for(vector<DataNode>::iterator it=testData.begin();it!=testData.end();it++){
        for(int i=0;i<9;i++)    voteType[i]=0;
        for(int i=0;i<k;i++){
            minDis=99999;
            type=-1;
            findType(root,(*it));
            dn.push_back(temp);
        }
        if(j<3){
            switch(j){
                case 0:
                    v1.assign(dn.begin(),dn.end());
                    break;
                case 1:
                    v2.assign(dn.begin(),dn.end());
                    break;
                case 2:
                    v3.assign(dn.begin(),dn.end());
                    break;
            }
            j++;
        }
        for(vector<DistanceNode>::iterator it=dn.begin();it!=dn.end();it++){
            voteType[(*it).type-1]++;
        }
        int guess=0;
        int maxVote=0;
        for(int i=0;i<9;i++){
            if(voteType[i]>maxVote){
                maxVote=voteType[i];
                guess=i+1;
            }
        }
        total++;
        if(guess==(*it).type){
            correct++;
        }
        dn.clear();
    }

    cout<<"KNN accuracy: "<<float(correct)/(float)total<<endl;
    for(vector<DistanceNode>::iterator it=v1.begin();it!=v1.end();it++){
        cout<<(*it).index<<" ";
    }
    cout<<endl;
    for(vector<DistanceNode>::iterator it=v2.begin();it!=v2.end();it++){
        cout<<(*it).index<<" ";
    }
    cout<<endl;
    for(vector<DistanceNode>::iterator it=v3.begin();it!=v3.end();it++){
        cout<<(*it).index<<" ";
    }
    cout<<endl<<endl;
    v1.clear();
    v2.clear();
    v3.clear();

    j=0;
    correct=0;
    total=0;

    k=10;
    for(vector<DataNode>::iterator it=testData.begin();it!=testData.end();it++){
        for(int i=0;i<9;i++)    voteType[i]=0;
        for(int i=0;i<k;i++){
            minDis=99999;
            type=-1;
            findType(root,(*it));
            dn.push_back(temp);
        }
        if(j<3){
            switch(j){
                case 0:
                    v1.assign(dn.begin(),dn.end());
                    break;
                case 1:
                    v2.assign(dn.begin(),dn.end());
                    break;
                case 2:
                    v3.assign(dn.begin(),dn.end());
                    break;
            }
            j++;
        }
        for(vector<DistanceNode>::iterator it=dn.begin();it!=dn.end();it++){
            voteType[(*it).type-1]++;
        }
        int guess=0;
        int maxVote=0;
        for(int i=0;i<9;i++){
            if(voteType[i]>maxVote){
                maxVote=voteType[i];
                guess=i+1;
            }
        }

        total++;
        if(guess==(*it).type){
            correct++;
        }
        dn.clear();
    }
    cout<<"KNN accuracy: "<<float(correct)/(float)total<<endl;

    for(vector<DistanceNode>::iterator it=v1.begin();it!=v1.end();it++){
        cout<<(*it).index<<" ";
    }
    cout<<endl;
    for(vector<DistanceNode>::iterator it=v2.begin();it!=v2.end();it++){
        cout<<(*it).index<<" ";
    }
    cout<<endl;
    for(vector<DistanceNode>::iterator it=v3.begin();it!=v3.end();it++){
        cout<<(*it).index<<" ";
    }
    cout<<endl<<endl;
    v1.clear();
    v2.clear();
    v3.clear();

    j=0;
    correct=0;
    total=0;

    k=100;
    for(vector<DataNode>::iterator it=testData.begin();it!=testData.end();it++){
        for(int i=0;i<9;i++)    voteType[i]=0;
        for(int i=0;i<k;i++){
            minDis=99999;
            type=-1;
            findType(root,(*it));
            dn.push_back(temp);
        }
        if(j<3){
            switch(j){
                case 0:
                    v1.assign(dn.begin(),dn.end());
                    break;
                case 1:
                    v2.assign(dn.begin(),dn.end());
                    break;
                case 2:
                    v3.assign(dn.begin(),dn.end());
                    break;
            }
            j++;
        }
        for(vector<DistanceNode>::iterator it=dn.begin();it!=dn.end();it++){
            voteType[(*it).type-1]++;
        }
        int guess=0;
        int maxVote=0;
        for(int i=0;i<9;i++){
            if(voteType[i]>maxVote){
                maxVote=voteType[i];
                guess=i+1;
            }
        }
        //cout<<"Guess "<<guess<<" Actual: "<<(*it).type<<endl;
        total++;
        if(guess==(*it).type){
            correct++;
        }
        dn.clear();
    }
    cout<<"KNN accuracy: "<<float(correct)/(float)total<<endl;

    for(vector<DistanceNode>::iterator it=v1.begin();it!=v1.end();it++){
        cout<<(*it).index<<" ";
    }
    cout<<endl;
    for(vector<DistanceNode>::iterator it=v2.begin();it!=v2.end();it++){
        cout<<(*it).index<<" ";
    }
    cout<<endl;
    for(vector<DistanceNode>::iterator it=v3.begin();it!=v3.end();it++){
        cout<<(*it).index<<" ";
    }
    cout<<endl;
    v1.clear();
    v2.clear();
    v3.clear();
    postOrderTraversal(root);
    return 0;
}


