#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>
#include <cmath>
#include <sstream>
#include <map>
#include <list>
#include <fstream>

using namespace std;

class Decrypt{
	private:
		string curr;
		string cipherText;
		int keyLength;
		int cipherTextLength;
		string plainText;
		string key;
		vector<string> dictionary1contents;
		string dictionary1;
		string *divs;
		string *probablechars;
		vector<string> dictionary2contents; 
		string ppt;
		static int engfreqs[26];
	public:
		void decrypt();
		void divider();
		void validate();
		void getData();
		void freqDistributor(int);
		void keyGenerator(int);
		int getScore(string,string);
		bool wordBreak(string);
		bool check(string);
};

int Decrypt::engfreqs[26]={8,2,3,4,13,2,2,6,7,0,1,4,2,7,8,2,0,6,6,9,3,1,2,0,2,0};

//method to get input from stdin
void Decrypt::getData(){
	cout<<"Enter the key length:"<<endl;
	cin>>keyLength;
	cout<<endl;
	cout<<"Enter the cipher text:"<<endl;
	cin>>cipherText;
	cout<<endl;
	cipherTextLength=cipherText.size();
	string temp;
	ifstream ifs("Dictionary1.txt");
		while(getline(ifs,temp)){
			dictionary1+=temp;
			dictionary1contents.push_back(temp);
		}
	ifs.close();	
	ifstream ifs1("Dictionary2.txt");
		while(getline(ifs1,temp)){
			dictionary2contents.push_back(temp);
		}		
	ifs1.close();						
}

//method to create a frequency distribution table for each group and perform frequency analysis
void Decrypt::freqDistributor(int group){
	int f[26];
	memset(f,0,sizeof(f));
	for(int i=0;i<(int)divs[group].size();i++){
		f[(int)divs[group][i]-97]++;
	}

	//calculates the score of each shift
	int score[26];
	memset(score,0,sizeof(score));
	for(int offset=0;offset<26;offset++){
		int k=offset;
		for(int j=0;j<26;j++){
			if(k==26){
				k=0;
			}
			score[offset]+=engfreqs[j]*f[k];
			k++;
		}
	}

	//sort the score and get the index with the max score
	vector<pair<int,int> > v;
	for(int i=0;i<26;i++){
		v.push_back(make_pair(score[i],i));
	}
	sort(v.rbegin(),v.rend());
	vector<pair<int,int> >::iterator i;
	i=v.begin();

	//append the probable character to the key
	key+=(char)(97+i->second);
}

//method to divide the plaintext into groups based on the position in the ciphertext mod key length
void Decrypt::divider(){
	divs= new string[keyLength];
	probablechars= new string[keyLength];
	for(int i=0;i<cipherTextLength;i++){
		divs[i%keyLength]+=cipherText[i];
	}
	for(int i=0;i<keyLength;i++){
		freqDistributor(i);
	}
	validate();
}

//method to calculate the levenshtein distance to get the score for string matching
int Decrypt::getScore(string s1,string s2){
    unsigned int len1=s1.size();
    unsigned int len2=s2.size();
    vector<vector<unsigned int> > d(len1+1, vector<unsigned int>(len2+1));
    d[0][0]=0;
    for(unsigned int i=1;i<=len1;++i) d[i][0]=i;
	for(unsigned int i=1;i<=len2;++i) d[0][i]=i;
 
	for(unsigned int i=1;i<=len1;++i)
		for(unsigned int j=1;j<=len2;++j)
 
                      d[i][j]=min(min(d[i-1][j]+1,d[i][j-1]+1),
                                          d[i-1][j-1]+(s1[i-1] == s2[j-1] ? 0:1));
	return d[len1][len2];
}

//method to check if the string exist in the dictionary 2
bool Decrypt::check(string s){
	for(int i=0;i<(int)dictionary2contents.size();i++){
		if(dictionary2contents[i].compare(s)==0){
			return true;
		}
	}
	return false;
}

//method to check if the probable plaintext can be formed from the list of words in dictionary 2
bool Decrypt::wordBreak(string str){
	int size=str.size();
	if(size==0) return true;
	bool wb[size+1];
	memset(wb,0,sizeof(wb));
	for(int i=1;i<=size;i++){
		if(wb[i]==false&&check(str.substr(0,i)))
			wb[i]=true;
		if(wb[i]==true){
			if(i==size)
				return true;
			for(int j=i+1;j<=size;j++){
				if(wb[j]==false&&check(str.substr(i,j-i)));
					wb[j]=true;
				if(j==size&&wb[j]==true)
					return true;
			}
		}
	}
	return false;
}

//method to validate the probable plaintext to the contents of dictionary 1 and 2
void Decrypt::validate(){
	//cout<<key<<endl;
	map<int,string> lscore1;
	map<int,string> lscore2;
	string plainText=cipherText;
	int k=0;
	int t=1;
	while(t--){
		for(int i=0;i<cipherTextLength;i++){
			if(k==keyLength){
				k=0;
			}
			plainText[i]=char(((((int)cipherText[i]-97)-((int)key[k]-97)+26)%26)+97);
			k++;
		}
		if(wordBreak(plainText)==true){
			cout<<"Plaintext:"<<endl<<plainText<<endl;
			break;
		}
		int filepointer=0;
		int filepos=filepointer+cipherTextLength;
		while(filepos<=(int)dictionary1.size()){
			string temp=dictionary1.substr(filepointer,cipherTextLength);
			lscore1[getScore(temp,plainText)]=temp;
			filepointer++;
			filepos++;
		}
		lscore2[getScore(ppt,plainText)]=plainText;
		map<int,string>::iterator i1;
		map<int,string>::iterator i2;
		i1=lscore1.begin();
		i2=lscore2.begin();
		if(i1->first<i2->first)
			cout<<"Plaintext:"<<endl<<i1->second<<endl<<endl;
		else
			cout<<"Plaintext:"<<endl<<i2->second<<endl<<endl;
		lscore1.clear();
		lscore2.clear();
	}
}

void Decrypt::decrypt(){
	getData();
	divider();
}

int main(){
	Decrypt d;
	d.decrypt();
	system("pause");
}
