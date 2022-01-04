// Project UID db1f506d06d84ab787baf250c265e24e
#include <sstream>
#include <fstream>
#include <map>
#include <set>
#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <cmath>
#include "csvstream.h"
using namespace std;

/*
//set contains unique words
//std::set<string> set;
//set.insert("Euchre";
//set.end() returns an iterator one past the set
//set.find("p5");if doesnt exist, returns set.end()
//if does find, returns iterator to it


What info do we need to hold?
We need to keep track of the following:
(1) # of training posts with a given label
(2) # of training posts with a given label that contains a given word
(3) # of training posts with a given  word
Mathematically, we represent those as follows
Note:1,2,3, are pulled from above
1.ln( (1)/# of training posts)
2.ln( (2)/# of training posts with given label)
3.ln( (3)/# of training posts)
//content is bag of words
//set must store words. 
since we need to keep track # of posts and # unique words
we can always get size of our map using .size()
ex:vocabulary_size = my_word_map.size();
AFTER WE'VE PROCESSED OUR DATA, CALCULATIONS CAN BE DONE
-WE CAN JUST FEED IN GIVEN WORD AND LABEL to get a numeraic value

How many words in vocabulary? 
If reading in every single post, use a set to store unique values and call .size()
# of training posts? simply use a while loop
# of training posts with label X
    Use a Map! If label is Euchre, use it as a key, then count its value
# of training posts with word W
    Use a Map
    for word in content;   unique; only once
      map[word]++ only if in content
# of training posts with label X that contain word W


use set locally when going thru each post, use map globally(# of times each label appears
and # of times each word appears)

Predicting!
How do we parse over a posts content
Let map<string,int> label_map maps labels to their freqs
-auto it=label_map.beegin()
while(it!=label_map.end()){
    update data for given label!
    auto it2 = word_given_label_map.begin()
    while(it2!=word_given_label_map.end()){//cycles thru words of a post
        update data for a given word( u already know label)
    }
}
ex:Post:"What is the nullptr in a linked list is it like 0 for a linked list"
1-test out every label we have
-while we are testing it out, we need to calculate P(word|label)
-remember to get a set of words(no repeats)
    -what is the nullptr in a linkedlist is it like 0 for
calculate P(word)
    -remember if word does not occur, set it equal to log(1/num training posts)
-keep track of whether u classify post correctly

*/

class Classifier{
    /*
    map<word,frequency> word_freq keeps track hm times a word appears overall
    map<label,frequency> label_map
    map<label,map<word,freq>> word_map records for a given label, freq of the word
    int numPosts;
    Set<words>
    if have tag=euchre,
    1.label_map["Euchre"]++
    2.numPosts++

    wordmap["Euchre"] returns a map 
    3.wormap['Euchre']['what']++  /loop
    4.word_freq["What"]++   /loop
    5.set.insert('what')  
    ..if find another duplicate word in post, keep a local set and get rid

    
    */
    int numPosts;
    int numLabels;
    int numUniqueWords;
  
    set<string> vocabulary_set; //size of word_freq
    set<string> label_set;
    map<string,int> word_freq;
    map<string,int> word_posts_freq;
    map<string,int> label_freq;
    map<string,map<string,int>> PostsWithWordAndLabel;
public:
    Classifier()
        {
        }
    
    int getNumPosts(ifstream &is){
        int rows = 0;
        string line;
        while(getline(is,line)){
            ++rows;
        }
        numPosts=rows-1;
        return numPosts;
    }
    void getSets(ifstream &is){
        csvstream csvin(is);
        map<string,string> row;
        set<string> local_vocab_set;
        while(csvin>>row){
            local_vocab_set = unique_words(row["content"]);
            vocabulary_set.insert(local_vocab_set.begin(),local_vocab_set.end());
            string label = row["tag"];
            label_set.insert(label);
            label_freq[label]++;
        }
        numLabels = label_freq.size();

    }
    // EFFECTS: Returns a set containing the unique "words" in the original
    //string, delimited by whitespace.
        set<string> unique_words(const string &str) {
            istringstream source(str);
            set<string> words;
            string word;
            // Read word by word from the stringstream and insert into the set
            while (source >> word) {
                words.insert(word);
            }
            return words;
        }
    void getFreq(ifstream &is){//find # of times each word shows up
        csvstream csvin(is);
        map<string,string> row;
        set<string> local_set;
        set<string> localWordSet;
         set<string> localLabelWordSet;
        string tag;
        while(csvin>>row){
            // label_freq[row["tag"]]++;
            string buffer;
            stringstream ss(row["content"]);
            vector<string> wordsVec;
            while(ss>>buffer){
                wordsVec.push_back(buffer);
            }
            for(const string &word : wordsVec){
                word_freq[word]++;
            }
            localWordSet = unique_words(row["content"]);
            for(const string &word:localWordSet){
                word_posts_freq[word]++;
            } 
            localLabelWordSet=unique_words(row["content"]);
            tag=row["tag"];
            for(const string &word:localLabelWordSet){
                PostsWithWordAndLabel[tag][word]++;
            }
        }
        numUniqueWords = word_freq.size();
    }
 

  
    //Trainer function takes in training file
    //testing function reads thru posts and words 
    //calculate log prob function
    void trainer(ifstream &is, bool &debug){
        getSets(is);
        getFreq(is);
        csvstream csvin(is);
        map<string, string> row;
        if(debug){
            cout<<"training data:"<<endl;
        }
       while(csvin>>row){
           if(debug){
               cout<<" label= "<<row["tag"]<<", content = "<<row["content"]<<endl;
           }
       }
      
    }
    double labelLogProb(const string &label){
        double logP_C=0.0;//log prob of label C and how common it is
        logP_C+=log((double)label_freq[label]/(double)numPosts);
        return logP_C;
    }
    double wordLogProb(const string &label, const string &word,double logP_W=0.0){
        if(PostsWithWordAndLabel[label][word]==0&&word_posts_freq[word]==0){
            logP_W=log(1.0/double(numPosts));
        }else if(PostsWithWordAndLabel[label][word]==0){
            logP_W=log(double(word_posts_freq[word])/(double)numPosts);
        }else if(PostsWithWordAndLabel[label][word]!=0){
            logP_W=log((double)PostsWithWordAndLabel[label][word]/
                        label_freq[word]);
        }
        return logP_W;
    }
     pair<string,double> findMaxProb(vector<pair<string,double>> probLabel){
        double max=probLabel[0].second;
        int idx=0;
        for(size_t i=0; i<static_cast<int>(probLabel.size());++i){
            if(probLabel[i].second>max){
                max=probLabel[i].second;
                idx=i;
            }
        }
        return probLabel[idx];

    }
    pair<string,double> testing(string &post,double priorProb=0.0){
        //go thru every post and calculate the log prob
        //must do it multiple times for every label
        //go thru all labels    
    //now find log prob of a word w given a label c,
    //measure of how likely it is to see word w in posts with label C
        pair<string,double> logProbLabelAndWord;
        vector<pair<string,double>> probLabel;
        set<string> postsSet=unique_words(post);
    for(auto label : label_set){
        priorProb=labelLogProb(label);
        for(auto word:postsSet){
            priorProb+=wordLogProb(label,word);
        }
        logProbLabelAndWord.first=label;
        logProbLabelAndWord.second=priorProb;
        probLabel.push_back(logProbLabelAndWord);
    }
    return findMaxProb(probLabel);
    
    }
   
    void test(string &file){
        csvstream csvin(file);
        map<string,string>row;
        cout << "test data:" << endl;
        int correct_pred = 0;
        int total_pred = 0;
        while (csvin >> row) {
            string label = row["tag"];
            string content = row["content"];
            pair<string, double> prediction = testing(content);
            cout << "  correct = " << label << ", predicted = " << prediction.first
            << ", log-probability score = " << prediction.second << endl;
            cout << "  content = " << content << endl << endl;
            if (label == prediction.first) {
                correct_pred++;
            }
            total_pred++;
        }
        cout << "performance: " << correct_pred << " / " << total_pred
        << " posts predicted correctly" << endl;
    }
    void print_debug() {
        cout << "classes:" << endl;
        for (auto label : label_set) {
            cout << "  " << label << ", " << label_freq[label] <<
            " examples, log-prior = " << labelLogProb(label) << endl;
        }
        cout << "classifier parameters:" << endl;
        for (auto label : label_set) {
            for (auto word : vocabulary_set) {
                if (PostsWithWordAndLabel[label][word]>0) {
                    cout << "  " << label << ":" << word << ", count = " 
                    << PostsWithWordAndLabel[label][word]
                    << ", log-likelihood = " << wordLogProb(label, word) << endl;
                }
            }
        }
        cout << "\n";
    }
    void printCount(int count, bool &debug){
        cout<<"trained on "<<count<<" examples"<<endl;
        if(!debug){
            cout<<"\n";
        }
        if(debug){
            cout<<"vocabulary size = "<<vocabulary_set.size()<<endl<<endl;
        }
    }

};


bool debugCheck(int argc, char *argv[]){
      if (argc == 4 && string(argv[3]) == "--debug") {
        return true;
    }
    return false;
}

int main(int argc, char *argv[]){
   cout.precision(3);
   if((argc<3||argc>4)||(
    argc==4&&string(argv[3])!="--debug")){
        cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << endl;
        return 1;
    }
   ifstream fileOne(argv[1]);
   ifstream fileTwo(argv[2]);
   if(!fileOne.is_open()){
       cout << "Error opening file: " << string(argv[1]) << endl;
       return 1;
   }
   if(!fileTwo.is_open()){
       cout << "Error opening file: " << string(argv[2]) << endl;
       return 1;
   }
    bool debug=false;
    debugCheck(argc,argv); 
    Classifier PiazzaClassifier;
    PiazzaClassifier.trainer(fileOne,debug);
    int posts = PiazzaClassifier.getNumPosts(fileOne);
    PiazzaClassifier.printCount(posts,debug);
    // PiazzaClassifier.test(fileNameTwo);
    fileOne.close();
    fileTwo.close();
    return 0;
}