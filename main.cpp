// Project UID db1f506d06d84ab787baf250c265e24e
#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <cassert> 
#include "csvstream.h"
#include <set>
#include <sstream>
#include "math.h" //allowed??
using namespace std;

// TODO: 

set<std::string> unique_words(const std::string &str); 

// EFFECTS: compares string alphabetically
bool stringLess(std::string a, std::string b) {return a<b;} 


class Classifier {
public:
    Classifier(bool isDebugging) {
        numTrainingPosts = 0;
        debug = isDebugging;
        numTestPosts = 0;
        numCorrect = 0;
    }

    // get the 5 values

    int getNumTrainingPosts() {
        return numTrainingPosts;
    }
    int getNumTrainingWords() {
        return numPostsWithWord.size();
    }
    // REQUIRES: word is a string
    // MODIFIES: n/a
    // EFFECTS: returns the number of training posts that include the input word
    int getNumPostsWithWord(std::string word) {
        return numPostsWithWord[word];
    }
    // point 4
    int getNumPostsWithTag(std::string tag) {
        return numPostsOfTag[tag];
    }
    // EFFECTS: returns the number of posts with label tag containing word, point 5
    int getNumPostsOfPair(std::string tag, std::string word) {
        pair<std::string, std::string> tup;
        tup.first = tag;
        tup.second = word;
        return freqs[tup];
    }
    int getNumPostsOfPair(pair<std::string, std::string> inPair) {
        return freqs[inPair];
    }



    // REQUIRES: trainFile is a csv file with tag and content columns to train the classifier
    // MODIFIES: all private members
    // EFFECTS: stores data in private members that can yield the 5 data points
    void train(std::string trainFileName) {
        csvstream trainFile(trainFileName);

        // DEBUG
        if (debug) {
            std::cout << "training data:" << endl;
        }
        
       // iterate over each row, for each row iterate over every word in content
       map<string, string> row;
       while (trainFile >> row) {
           numTrainingPosts += 1; // point 1

           // update num posts with tag, point 4
           if (numPostsOfTag.find(row["tag"]) == numPostsOfTag.end()) {
               numPostsOfTag[row["tag"]] = 1;
           }
           else {
               numPostsOfTag[row["tag"]] += 1;
           }

           // DEBUG
            if (debug) {
                std::cout << "  label = " << row["tag"] << ", content = " << row["content"] << endl;
            }

           for (string word : unique_words(row["content"])) {

               // update number of posts with word, by extension number of unique words, point 2,3
               if (numPostsWithWord.find(word) == numPostsWithWord.end()) {
                   numPostsWithWord[word] = 1;
               }
               else {
                   numPostsWithWord[word] += 1;
               }

               // create key to check for, key = <tag, word>
               pair<string, string> key;
               key.first = row["tag"];
               key.second = word;

               // count the instance of this pair, point 5
               if (freqs.find(key) == freqs.end() ) {
                   freqs[key] = 1;
               }
               else {
                   freqs[key] += 1;
               }
           } 
       }

        // get vector of all tags
        for (auto elt : numPostsOfTag) {
            tags.push_back(elt.first);
        }

        // sort vector alphgabetically, TODO:check if it's the right order
        sort(tags.begin(),tags.end(),stringLess);


        // vector of all words
        for (auto elt : numPostsWithWord) {
            words.push_back(elt.first);
        }

        // sort vector alphgabetically, TODO:check if it's the right order
        sort(words.begin(),words.end(),stringLess);

        std::cout << "trained on " << getNumTrainingPosts() << " examples" << endl;


        // nesting?
        // DEBUG
        if (debug) {
            std::cout << "vocabulary size = " << words.size() << endl;
        }
        
        // extra newline
        std::cout << endl;

        // debug
        if (debug) {
            debugEndTrain();
        }

    }

    void debugEndTrain() {
        assert(debug);
        // tag information
        std::cout << "classes:" << endl;
        for (std::string t : tags) {
            std::cout << "  " << t << ", " << getNumPostsWithTag(t) << " examples, log-prior = "
                      << log(static_cast<double>(getNumPostsWithTag(t)) / 
                            static_cast<double>(getNumTrainingPosts())) << endl;
        }

        vector<pair<std::string, std::string> > pairs;
        for (auto elt : freqs) {
            pairs.push_back(elt.first);
        }

        // info on each pair of {tag, word}
        std::cout << "classifier parameters:" << endl;

        vector<pair<std::string, std::string> > iterPairs;
        pair<std::string, std::string> ip;
        for (std::string t : tags) {
            for (std::string w : words) {
                ip.first = t;
                ip.second = w;
                iterPairs.push_back(ip);
            }
        }
        for (auto p : iterPairs) {
            if (freqs.find(p) != freqs.end()) {
                std::cout << "  " << p.first << ":" << p.second << ", count = " << getNumPostsOfPair(p) 
                          << ", log-likelihood = " << logProbWordGivenTag(p.second, p.first) << endl;
            }
        }
        
                
            
        

        //extra debug endl
        std::cout << endl;
    }

    double logProbWordGivenTag(std::string w, std::string t) {
        pair<std::string, std::string> p;
        p.first = t;
        p.second = w;

        // if {tag, word} in freqs
        if (freqs.find(p) != freqs.end()) {
            return log(static_cast<double>(freqs[p]) / static_cast<double>(getNumPostsWithTag(t)));

                
        }

        // if word in words (all words in training)
        else if (std::find(words.begin(), words.end(), w) != words.end()) {
            return log(static_cast<double>(getNumPostsWithWord(w)) / static_cast<double>(getNumTrainingPosts()));    
        }

        // word not in training set
        else {
            return log(1 / static_cast<double>(getNumTrainingPosts()));
        }
    }



    void test(std::string testFileName) {
        csvstream testFile(testFileName);
        
        std::cout << "test data:" << endl;

        pair<std::string, double> prediction;
        map<std::string, string> row;
        while (testFile >> row) {
            numTestPosts += 1;
            
            prediction = predictTag(row);
            if (prediction.first == row["tag"]) {
                numCorrect += 1;
            }

            // test data
            std::cout << "  correct = " << row["tag"] << ", predicted = " << prediction.first 
                      << ", log-probability score = " << prediction.second << endl;

            std::cout << "  content = " << row["content"] << endl << endl;
        }

        // accuracy
        std::cout << "performance: " << numCorrect << " / " << numTestPosts 
                  << " posts predicted correctly" << endl;

    }


    // EFFECTS: returns log prob of a post being labeled with input tag
    double probabilityOfTag(map<std::string, std::string> row, std::string tag) {

        
        double prob = log(static_cast<double>(getNumPostsWithTag(tag)) / static_cast<double>(getNumTrainingPosts())); // base
        
        // add P(word | tag) for every word
        for (std::string word : unique_words(row["content"])) {

            // if {tag, word} in freqs
            pair<std::string, std::string> p;
            p.first = tag;
            p.second = word;
            if (freqs.find(p) != freqs.end()) {
                prob += log(static_cast<double>(freqs[p]) / static_cast<double>(getNumPostsWithTag(tag)));

                
            }

            // if word in words (all words in training)
            else if (std::find(words.begin(), words.end(), word) != words.end()) {
                prob += log(static_cast<double>(getNumPostsWithWord(word)) / static_cast<double>(getNumTrainingPosts()));

                
            }

            // word not in training set
            else {
                prob += log(1 / static_cast<double>(getNumTrainingPosts()));

                
            }
        }

        return prob;
    }
        
    // REQUIRES: arg is a row of a test csv file. rows are type map<string, string>, map[col] = cellDatum
    // MODIFIES: n/a
    // EFFECTS: returns pair of most likely tag and its probability
    pair<std::string, double> predictTag(map<std::string, std::string> row) {
        map<string, double> probabilityOf;

        for (std::string tag : tags) {
            probabilityOf[tag] = probabilityOfTag(row, tag);
        }

        vector<std::string> ks;
        for (auto elt : probabilityOf) {
            ks.push_back(elt.first);
        }

        // sort vector alphgabetically, TODO:check if it's the right order
        sort(ks.begin(),ks.end(),stringLess);


        std::string probableTag = tags[0];
        double highestProb = probabilityOf[ks[0]];

        for (auto elt : ks) {
            //delete me
            //std::cout << "prob of " << elt << ": " << probabilityOf[elt] << endl;

            if (probabilityOf[elt] > highestProb) {
                highestProb = probabilityOf[elt];
                probableTag = elt;
                //cout << "prob tag is now " << probableTag << "with prob " << highestProb << endl;
            }
        }

        // TODO: debug printing here i think
        pair<std::string, double> r;
        r.first = probableTag;
        r.second = probabilityOf[probableTag];
        return r;
    }




private:
    vector<std::string> tags;
    vector<std::string> words;

    map<pair<std::string, std::string>, int> freqs; // maps pairs of <tag, word> to that pair's freq, 5
    //vector<std::string> tags; // vector of all tags, not rly necessary?

    // used for 5 values
    int numTrainingPosts;                        // 1
    map<std::string, int> numPostsWithWord;      // 2,3 with operation
    map<std::string, int> numPostsOfTag;         // 4

    map<std::string, double> baseProbabilityOfTag; // maps tag to its base probability, needed?

    bool debug;
    int numCorrect;
    int numTestPosts;
};
















int main (int argc, char *argv[]) {

    // CHECKING ARGUMENTS

    //check number of args 
    if(argc != 4 && argc != 3){
        std::cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << std::endl;
        return 62;
    }
    //if four args, check if fourth arg is correct
    if (argc == 4){
        if (strcmp(argv[3], "--debug") != 0) {
            std::cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << std::endl;
            return 62;
        }
    }

    // END OF ARGUMENT CHECKS

    // open files?
    string trainFileName = argv[1];
    string testFileName = argv[2];

    // check opening file errors
    try {
        csvstream csvTrain(trainFileName, ',', false); // not sure if this fixed more than 2 cols
    }
    catch(const csvstream_exception &e) {
        cout << "Error opening file: " << trainFileName << endl;
        return 69;
    }

    try {
        csvstream csvTest(testFileName, ',', false);
    }
    catch(const csvstream_exception &e) {
        cout << "Error opening file: " << testFileName << endl;
        return 69;
    }

    // try block variables are local but idk how else to check so this janky ass workaround will have to do
    //csvstream csvTrain(trainFileName, ',', false);
    //csvstream csvTest(testFileName, ',', false);
    // or not idk
    
   
    // set precision to 3 decimal places
    std::cout.precision(3);

    // Create classifier, argc == 4 returns bool value of debug
    Classifier postClassifier(argc == 4);

    // Train classifier
    postClassifier.train(trainFileName); // can't copy stream, so pass name

    // Test
    postClassifier.test(testFileName);
    








    return 0;
}

set<std::string> unique_words(const std::string &str) {
 istringstream source(str);
 set<std::string> ws;
 std::string w;

 // Read word by word from the stringstream and insert into the set
 while (source >> w) {
   ws.insert(w);
 }
 return ws;
}

/*
// Open file
  csvstream csvin("input.csv");

  // A row is a vector<pair<string, string>>
  // key = column name, value = cell datum
  vector<pair<string, string>> row;

  // Read file
  while (csvin >> row) {
    cout << "row:" << "\n";
    for (unsigned int i=0; i < row.size(); ++i) {
      const string &column_name = row[i].first;
      const string &datum = row[i].second;
      cout << "  " << column_name << ": " << datum << "\n";
    }
  }

*/

/*
// 1. Accepting the correct arguments from the command line
    //Open train file
    ifstream trainFile(trainFileName);
    if (trainFile.fail()) {
    cout << "Error opening file: " << fileOne << endl;
    return 1;
  }
    //Open test file
    ifstream testFile(fileTwo);
    if (testFile.fail()){
    cout << "Error opening file: " << fileTwo << endl;
    
    return 1;
  }
*/

//map of pairs for number of posts with (label) that contain (word)
    //Map<pair<string, string>, int> my_map;
    //how to add to it
        //my_map[{"euchre", "bower"}] = 1;

/*


void classifierTest() {
    assert(false);
}
template <typename keyType, typename mapType>
void classifierTrain(std::string fileName, mapType m) {


}

void predictionOutput() {
    assert(false);
}
*/

// EFFECTS: Returns a set containing the unique "words" in the original
//          string, delimited by whitespace.
